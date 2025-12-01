#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <cstdint>
#include <cstring>

// -------------------------- BMP headers --------------------------
#pragma pack(push,1)
struct BITMAPFILEHEADER {
    uint16_t bfType; // 'BM'
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};
struct BITMAPINFOHEADER {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};
struct RGBQUAD {
    unsigned char rgbBlue;
    unsigned char rgbGreen;
    unsigned char rgbRed;
    unsigned char rgbReserved;
};
#pragma pack(pop)

// -------------------------- Settings --------------------------
const std::string INPUT_FILE = "1.bmp";
// MODE: 0 = auto, 1 = color, 2 = gray
const int MODE = 0;
#define BLOCK_DIM_X 16
#define BLOCK_DIM_Y 16
// -----------------------------------------------------------------

// Gaussian kernel (5x5)
__constant__ float gaussian_kernel[25] = {
    0.03685f, 0.03884f, 0.03984f, 0.03884f, 0.03685f,
    0.03884f, 0.04183f, 0.04283f, 0.04183f, 0.03884f,
    0.03984f, 0.04283f, 0.04382f, 0.04283f, 0.03984f,
    0.03884f, 0.04183f, 0.04283f, 0.04183f, 0.03884f,
    0.03685f, 0.03884f, 0.03984f, 0.03884f, 0.03685f
};
const float host_kernel[25] = {
    0.03685f, 0.03884f, 0.03984f, 0.03884f, 0.03685f,
    0.03884f, 0.04183f, 0.04283f, 0.04183f, 0.03884f,
    0.03984f, 0.04283f, 0.04382f, 0.04283f, 0.03984f,
    0.03884f, 0.04183f, 0.04283f, 0.04183f, 0.03884f,
    0.03685f, 0.03884f, 0.03984f, 0.03884f, 0.03685f
};

// -------------------------- Helpers --------------------------
void cuda_check(cudaError_t e, const char* where) {
    if (e != cudaSuccess) {
        std::cerr << "CUDA error at " << where << ": " << cudaGetErrorString(e) << std::endl;
        std::exit(1);
    }
}

std::string get_filename_from_path(const std::string& path) {
    size_t last_slash = path.find_last_of("\\/");
    if (last_slash == std::string::npos) return path;
    return path.substr(last_slash + 1);
}

std::string get_stem_from_filename(const std::string& filename) {
    size_t last_dot = filename.find_last_of('.');
    if (last_dot == std::string::npos) return filename;
    return filename.substr(0, last_dot);
}

std::string get_directory_from_path(const std::string& path) {
    size_t last_slash = path.find_last_of("\\/");
    if (last_slash == std::string::npos) return ".";
    return path.substr(0, last_slash);
}

void remove_if_exists(const std::string& path) {
    std::error_code ec;
    if (std::filesystem::exists(path, ec)) {
        std::filesystem::remove(path, ec);
    }
}

// -------------------------- Correct load_bmp (supports 1/4/8/24/32 bpp) --------------------------
// Returns bitsPerPixel via out_bpp (1/4/8/24/32)
bool load_bmp(const std::string& filename, std::vector<unsigned char>& image, int& width, int& height, int& out_bpp) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return false;
    }
    // Read BITMAPFILEHEADER
    BITMAPFILEHEADER fh;
    file.read(reinterpret_cast<char*>(&fh), sizeof(fh));
    if (!file) {
        std::cerr << "Failed to read BITMAPFILEHEADER\n";
        return false;
    }
    if (fh.bfType != 0x4D42) {
        std::cerr << "Not a BMP file\n";
        return false;
    }
    // Read biSize
    uint32_t biSize = 0;
    file.read(reinterpret_cast<char*>(&biSize), sizeof(biSize));
    if (!file) {
        std::cerr << "Failed to read biSize\n";
        return false;
    }
    BITMAPINFOHEADER ih{};
    ih.biSize = biSize;
    if (biSize >= sizeof(BITMAPINFOHEADER)) {
        // read remaining 36 bytes of standard header
        file.read(reinterpret_cast<char*>(((unsigned char*)&ih) + sizeof(biSize)), sizeof(BITMAPINFOHEADER) - sizeof(biSize));
        if (!file) {
            std::cerr << "Failed to read BITMAPINFOHEADER\n";
            return false;
        }
        if (biSize > sizeof(BITMAPINFOHEADER)) {
            file.seekg(biSize - sizeof(BITMAPINFOHEADER), std::ios::cur);
        }
    } else {
        size_t rest = (biSize > sizeof(biSize)) ? (biSize - sizeof(biSize)) : 0;
        file.read(reinterpret_cast<char*>(((unsigned char*)&ih) + sizeof(biSize)), rest);
        if (biSize < 16) {
            std::cerr << "Unsupported BMP info header size: " << biSize << std::endl;
            return false;
        }
    }
    int bpp = ih.biBitCount;
    if (bpp != 1 && bpp != 4 && bpp != 8 && bpp != 24 && bpp != 32) {
        std::cerr << "Only 1/4/8/24/32-bit BMP supported (got " << bpp << ")\n";
        return false;
    }
    out_bpp = bpp;
    width = ih.biWidth;
    height = std::abs(ih.biHeight);
    bool topDown = ih.biHeight < 0;
    image.resize(static_cast<size_t>(width) * height * 3);
    // Read palette if paletted image (1/4/8)
    std::vector<RGBQUAD> palette;
    if (bpp == 1 || bpp == 4 || bpp == 8) {
        uint32_t numColors = ih.biClrUsed ? ih.biClrUsed : (1u << bpp);
        palette.resize(numColors);
        std::streampos palettePos = sizeof(BITMAPFILEHEADER) + biSize;
        file.seekg(palettePos, std::ios::beg);
        file.read(reinterpret_cast<char*>(palette.data()), numColors * sizeof(RGBQUAD));
        // after this we'll seek to bfOffBits
    }
    // Move to pixel data
    file.seekg(fh.bfOffBits, std::ios::beg);
    if (!file) {
        std::cerr << "Failed to seek to pixel data\n";
        return false;
    }
    // Correct row size calculation for any bpp:
    // rowBytes = (((width * bitsPerPixel) + 31) / 32) * 4
    uint32_t bitsPerLine = static_cast<uint32_t>(width) * static_cast<uint32_t>(bpp);
    uint32_t rowBytes = ((bitsPerLine + 31) / 32) * 4;
    std::vector<unsigned char> rowbuf(rowBytes);
    for (int y = 0; y < height; ++y) {
        int actualY = topDown ? y : (height - 1 - y);
        file.read(reinterpret_cast<char*>(rowbuf.data()), rowBytes);
        if (!file) {
            std::cerr << "Failed to read row " << y << std::endl;
            return false;
        }
        if (bpp == 24) {
            for (int x = 0; x < width; ++x) {
                unsigned char b = rowbuf[x*3 + 0];
                unsigned char g = rowbuf[x*3 + 1];
                unsigned char r = rowbuf[x*3 + 2];
                image[actualY * width * 3 + x * 3 + 0] = r;
                image[actualY * width * 3 + x * 3 + 1] = g;
                image[actualY * width * 3 + x * 3 + 2] = b;
            }
        } else if (bpp == 32) {
            for (int x = 0; x < width; ++x) {
                unsigned char b = rowbuf[x*4 + 0];
                unsigned char g = rowbuf[x*4 + 1];
                unsigned char r = rowbuf[x*4 + 2];
                // unsigned char a = rowbuf[x*4 + 3]; ignored
                image[actualY * width * 3 + x * 3 + 0] = r;
                image[actualY * width * 3 + x * 3 + 1] = g;
                image[actualY * width * 3 + x * 3 + 2] = b;
            }
        } else if (bpp == 8) {
            for (int x = 0; x < width; ++x) {
                unsigned char idx = rowbuf[x];
                if (idx < palette.size()) {
                    RGBQUAD q = palette[idx];
                    image[actualY * width * 3 + x * 3 + 0] = q.rgbRed;
                    image[actualY * width * 3 + x * 3 + 1] = q.rgbGreen;
                    image[actualY * width * 3 + x * 3 + 2] = q.rgbBlue;
                } else {
                    unsigned char v = idx;
                    image[actualY * width * 3 + x * 3 + 0] = v;
                    image[actualY * width * 3 + x * 3 + 1] = v;
                    image[actualY * width * 3 + x * 3 + 2] = v;
                }
            }
        } else if (bpp == 4) {
            // each byte has two 4-bit indices: high nibble -> left pixel
            for (int x = 0; x < width; ++x) {
                int byteIndex = x / 2;
                bool high = (x % 2) == 0;
                unsigned char byte = rowbuf[byteIndex];
                unsigned char idx = high ? (byte >> 4) : (byte & 0x0F);
                if (idx < palette.size()) {
                    RGBQUAD q = palette[idx];
                    image[actualY * width * 3 + x * 3 + 0] = q.rgbRed;
                    image[actualY * width * 3 + x * 3 + 1] = q.rgbGreen;
                    image[actualY * width * 3 + x * 3 + 2] = q.rgbBlue;
                } else {
                    unsigned char v = idx * 17;
                    image[actualY * width * 3 + x * 3 + 0] = v;
                    image[actualY * width * 3 + x * 3 + 1] = v;
                    image[actualY * width * 3 + x * 3 + 2] = v;
                }
            }
        } else if (bpp == 1) {
            // each bit is a pixel: bit7 -> leftmost pixel
            for (int x = 0; x < width; ++x) {
                int byteIndex = x / 8;
                int bit = 7 - (x % 8);
                unsigned char byte = rowbuf[byteIndex];
                unsigned char idx = (byte >> bit) & 1;
                if (idx < palette.size()) {
                    RGBQUAD q = palette[idx];
                    image[actualY * width * 3 + x * 3 + 0] = q.rgbRed;
                    image[actualY * width * 3 + x * 3 + 1] = q.rgbGreen;
                    image[actualY * width * 3 + x * 3 + 2] = q.rgbBlue;
                } else {
                    unsigned char v = idx ? 255 : 0;
                    image[actualY * width * 3 + x * 3 + 0] = v;
                    image[actualY * width * 3 + x * 3 + 1] = v;
                    image[actualY * width * 3 + x * 3 + 2] = v;
                }
            }
        }
    }
    return true;
}

// -------------------------- Save BMP (24-bit) --------------------------
bool save_bmp(const std::string& filename, const std::vector<unsigned char>& image, int width, int height) {
    BITMAPFILEHEADER fh{};
    BITMAPINFOHEADER ih{};
    int bytesPerPixel = 3;
    int rowBytes = ((width * bytesPerPixel + 3) / 4) * 4;
    ih.biSize = sizeof(BITMAPINFOHEADER);
    ih.biWidth = width;
    ih.biHeight = height;
    ih.biPlanes = 1;
    ih.biBitCount = bytesPerPixel * 8;
    ih.biCompression = 0;
    ih.biSizeImage = rowBytes * height;
    fh.bfType = 0x4D42;
    fh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    fh.bfSize = fh.bfOffBits + ih.biSizeImage;
    std::ofstream f(filename, std::ios::binary);
    if (!f) return false;
    f.write(reinterpret_cast<const char*>(&fh), sizeof(fh));
    f.write(reinterpret_cast<const char*>(&ih), sizeof(ih));
    std::vector<unsigned char> row(rowBytes, 0);
    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            unsigned char r = image[y * width * 3 + x * 3 + 0];
            unsigned char g = image[y * width * 3 + x * 3 + 1];
            unsigned char b = image[y * width * 3 + x * 3 + 2];
            row[x*3 + 0] = b;
            row[x*3 + 1] = g;
            row[x*3 + 2] = r;
        }
        f.write(reinterpret_cast<const char*>(row.data()), rowBytes);
    }
    return true;
}

bool save_bmp_grayscale(const std::string& filename, const std::vector<unsigned char>& image, int width, int height) {
    BITMAPFILEHEADER fh{};
    BITMAPINFOHEADER ih{};
    int bytesPerPixel = 3;
    int rowBytes = ((width * bytesPerPixel + 3) / 4) * 4;
    ih.biSize = sizeof(BITMAPINFOHEADER);
    ih.biWidth = width;
    ih.biHeight = height;
    ih.biPlanes = 1;
    ih.biBitCount = bytesPerPixel * 8;
    ih.biCompression = 0;
    ih.biSizeImage = rowBytes * height;
    fh.bfType = 0x4D42;
    fh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    fh.bfSize = fh.bfOffBits + ih.biSizeImage;
    std::ofstream f(filename, std::ios::binary);
    if (!f) return false;
    f.write(reinterpret_cast<const char*>(&fh), sizeof(fh));
    f.write(reinterpret_cast<const char*>(&ih), sizeof(ih));
    std::vector<unsigned char> row(rowBytes, 0);
    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            unsigned char g = image[y * width + x];
            row[x*3 + 0] = g;
            row[x*3 + 1] = g;
            row[x*3 + 2] = g;
        }
        f.write(reinterpret_cast<const char*>(row.data()), rowBytes);
    }
    return true;
}

// -------------------------- CPU implementations --------------------------
std::vector<unsigned char> gaussian_filter_color_cpu(const std::vector<unsigned char>& input, int width, int height, double& cpu_ms) {
    std::vector<unsigned char> output(static_cast<size_t>(width) * height * 3);
    auto t0 = std::chrono::high_resolution_clock::now();
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float sumr = 0.0f, sumg = 0.0f, sumb = 0.0f;
            for (int ky = -2; ky <= 2; ++ky) {
                for (int kx = -2; kx <= 2; ++kx) {
                    int px = x + kx;
                    int py = y + ky;
                    px = std::max(0, std::min(px, width - 1));
                    py = std::max(0, std::min(py, height - 1));
                    int idx = py * width * 3 + px * 3;
                    float w = host_kernel[(ky + 2) * 5 + (kx + 2)];
                    sumr += input[idx + 0] * w;
                    sumg += input[idx + 1] * w;
                    sumb += input[idx + 2] * w;
                }
            }
            output[y * width * 3 + x * 3 + 0] = static_cast<unsigned char>(std::min(std::max(sumr, 0.0f), 255.0f));
            output[y * width * 3 + x * 3 + 1] = static_cast<unsigned char>(std::min(std::max(sumg, 0.0f), 255.0f));
            output[y * width * 3 + x * 3 + 2] = static_cast<unsigned char>(std::min(std::max(sumb, 0.0f), 255.0f));
        }
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    cpu_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    return output;
}

std::vector<unsigned char> gaussian_filter_gray_cpu(const std::vector<unsigned char>& input, int width, int height, double& cpu_ms) {
    std::vector<unsigned char> output(static_cast<size_t>(width) * height);
    auto t0 = std::chrono::high_resolution_clock::now();
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float sum = 0.0f;
            for (int ky = -2; ky <= 2; ++ky) {
                for (int kx = -2; kx <= 2; ++kx) {
                    int px = x + kx;
                    int py = y + ky;
                    px = std::max(0, std::min(px, width - 1));
                    py = std::max(0, std::min(py, height - 1));
                    sum += input[py * width + px] * host_kernel[(ky + 2) * 5 + (kx + 2)];
                }
            }
            unsigned char v = static_cast<unsigned char>(std::min(std::max(sum, 0.0f), 255.0f));
            output[y * width + x] = v;
        }
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    cpu_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    return output;
}

// -------------------------- CUDA kernels --------------------------
__global__ void gaussian_filter_color_kernel(const unsigned char* input, unsigned char* output, int width, int height, size_t pitch) {
    __shared__ unsigned smem[BLOCK_DIM_Y * (BLOCK_DIM_X * 3)];

    int tx = threadIdx.x;
    int ty = threadIdx.y;
    int bx = blockIdx.x;
    int by = blockIdx.y;

    int baseX = (bx * BLOCK_DIM_X + tx) * 3;
    int baseY = by * BLOCK_DIM_Y + ty;

    if (baseY < height) {
        unsigned* input_uint = (unsigned*)input;
        int inputWidth = pitch / 4; // ширина в 4-байтных словах

        smem[ty * (BLOCK_DIM_X * 3) + tx] = input_uint[baseY * inputWidth + baseX];
        if (tx + BLOCK_DIM_X < BLOCK_DIM_X * 3) {
            smem[ty * (BLOCK_DIM_X * 3) + tx + BLOCK_DIM_X] = input_uint[baseY * inputWidth + baseX + BLOCK_DIM_X];
        }
        if (tx + 2 * BLOCK_DIM_X < BLOCK_DIM_X * 3) {
            smem[ty * (BLOCK_DIM_X * 3) + tx + 2 * BLOCK_DIM_X] = input_uint[baseY * inputWidth + baseX + 2 * BLOCK_DIM_X];
        }
    }

    __syncthreads();

    int x = bx * BLOCK_DIM_X * 4 + tx * 4; // каждая нить обрабатывает 4 пикселя
    int y = by * BLOCK_DIM_Y + ty;

    if (y >= height) return;

    for (int p = 0; p < 4; ++p) {
        int px = x + p;
        if (px >= width) break;

        float sumr = 0.0f, sumg = 0.0f, sumb = 0.0f;
        for (int ky = -2; ky <= 2; ++ky) {
            for (int kx = -2; kx <= 2; ++kx) {
                int pos_x = px + kx;
                int pos_y = y + ky;
                pos_x = max(0, min(pos_x, width - 1));
                pos_y = max(0, min(pos_y, height - 1));
                int global_idx = pos_y * pitch + pos_x * 3;
                float weight = gaussian_kernel[(ky + 2) * 5 + (kx + 2)];
                sumr += input[global_idx + 0] * weight;
                sumg += input[global_idx + 1] * weight;
                sumb += input[global_idx + 2] * weight;
            }
        }
        int out_idx = y * pitch + px * 3;
        output[out_idx + 0] = static_cast<unsigned char>(min(max(sumr, 0.0f), 255.0f));
        output[out_idx + 1] = static_cast<unsigned char>(min(max(sumg, 0.0f), 255.0f));
        output[out_idx + 2] = static_cast<unsigned char>(min(max(sumb, 0.0f), 255.0f));
    }
}

// simple grayscale kernel (one thread per pixel)
__global__ void gaussian_filter_gray_simple_kernel(const unsigned char* input, unsigned char* output, int width, int height) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    if (x >= width || y >= height) return;
    float sum = 0.0f;
    for (int ky = -2; ky <= 2; ++ky) {
        for (int kx = -2; kx <= 2; ++kx) {
            int px = x + kx;
            int py = y + ky;
            px = max(0, min(px, width - 1));
            py = max(0, min(py, height - 1));
            int idx = py * width + px;
            float w = gaussian_kernel[(ky + 2) * 5 + (kx + 2)];
            sum += input[idx] * w;
        }
    }
    unsigned char r = static_cast<unsigned char>(min(max(sum, 0.0f), 255.0f));
    output[y * width + x] = r;
}

// -------------------------- GPU wrappers --------------------------
void gaussian_filter_color_gpu(const std::vector<unsigned char>& input, std::vector<unsigned char>& output, int width, int height, float& gpu_time_ms) {
    unsigned char* d_input = nullptr;
    unsigned char* d_output = nullptr;
    size_t row_bytes = static_cast<size_t>(width) * 3;
    size_t pitch;
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cuda_check(cudaMallocPitch(&d_input, &pitch, row_bytes, height), "cudaMallocPitch d_input color");
    cuda_check(cudaMallocPitch(&d_output, &pitch, row_bytes, height), "cudaMallocPitch d_output color");
    cuda_check(cudaMemcpy2D(d_input, pitch, input.data(), row_bytes, row_bytes, height, cudaMemcpyHostToDevice), "cudaMemcpy2D H2D color");
    dim3 blockSize(BLOCK_DIM_X, BLOCK_DIM_Y);
    dim3 gridSize((width + blockSize.x * 4 - 1) / (blockSize.x * 4), (height + blockSize.y - 1) / blockSize.y);
    cudaEventRecord(start);
    gaussian_filter_color_kernel<<<gridSize, blockSize>>>(d_input, d_output, width, height, pitch);
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&gpu_time_ms, start, stop);
    output.resize(row_bytes * height);
    cuda_check(cudaMemcpy2D(output.data(), row_bytes, d_output, pitch, row_bytes, height, cudaMemcpyDeviceToHost), "cudaMemcpy2D D2H color");
    cudaFree(d_input);
    cudaFree(d_output);
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
}

void gaussian_filter_gray_gpu(const std::vector<unsigned char>& input, std::vector<unsigned char>& output, int width, int height, float& gpu_time_ms) {
    size_t num_pixels = static_cast<size_t>(width) * height;
    unsigned char *d_in = nullptr, *d_out = nullptr;
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cuda_check(cudaMalloc(&d_in, num_pixels * sizeof(unsigned char)), "cudaMalloc d_in gray");
    cuda_check(cudaMalloc(&d_out, num_pixels * sizeof(unsigned char)), "cudaMalloc d_out gray");
    cuda_check(cudaMemcpy(d_in, input.data(), num_pixels * sizeof(unsigned char), cudaMemcpyHostToDevice), "cudaMemcpy H2D gray");
    cuda_check(cudaMemset(d_out, 0, num_pixels * sizeof(unsigned char)), "cudaMemset d_out gray");
    dim3 block(BLOCK_DIM_X, BLOCK_DIM_Y);
    dim3 grid((width + block.x - 1) / block.x, (height + block.y - 1) / block.y);
    cudaEventRecord(start);
    gaussian_filter_gray_simple_kernel<<<grid, block>>>(d_in, d_out, width, height);
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&gpu_time_ms, start, stop);
    output.resize(num_pixels);
    cuda_check(cudaMemcpy(output.data(), d_out, num_pixels * sizeof(unsigned char), cudaMemcpyDeviceToHost), "cudaMemcpy D2H gray");
    cudaFree(d_in);
    cudaFree(d_out);
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
}

// -------------------------- Autodetect mode --------------------------
int detect_image_mode(const std::vector<unsigned char>& img, int width, int height, int header_bpp) {
    if (header_bpp == 8 || header_bpp == 4 || header_bpp == 1) return 2; // paletted images likely grayscale
    size_t total = static_cast<size_t>(width) * height;
    const size_t max_checks = 10000;
    size_t checks = std::min(total, max_checks);
    size_t step = (total + checks - 1) / checks;
    size_t gray_count = 0;
    size_t checked = 0;
    for (size_t i = 0; i < total; i += step) {
        unsigned char r = img[i * 3 + 0];
        unsigned char g = img[i * 3 + 1];
        unsigned char b = img[i * 3 + 2];
        if (r == g && g == b) ++gray_count;
        ++checked;
    }
    double ratio = static_cast<double>(gray_count) / static_cast<double>(checked);
    if (ratio > 0.999) return 2;
    return 1;
}

// -------------------------- Comparison --------------------------
void compare_color_images(const std::vector<unsigned char>& a, const std::vector<unsigned char>& b, int width, int height) {
    size_t total = static_cast<size_t>(width) * height;
    size_t mismatches = 0;
    size_t first_idx = SIZE_MAX;
    for (size_t i = 0; i < total; ++i) {
        size_t idx = i * 3;
        if (a[idx + 0] != b[idx + 0] || a[idx + 1] != b[idx + 1] || a[idx + 2] != b[idx + 2]) {
            ++mismatches;
            if (first_idx == SIZE_MAX) first_idx = i;
        }
    }
    std::cout << "Color comparison: mismatches = " << mismatches << " / " << total;
    if (total) {
        double pct = 100.0 * mismatches / static_cast<double>(total);
        std::cout << " (" << pct << "%)";
    }
    std::cout << std::endl;
    if (mismatches && first_idx != SIZE_MAX) {
        int y = static_cast<int>(first_idx) / width;
        int x = static_cast<int>(first_idx) % width;
        std::cout << "First mismatch at pixel (x,y)=(" << x << "," << y << ")" << std::endl;
        size_t idx = first_idx * 3;
        std::cout << "CPU pixel: R=" << int(a[idx + 0]) << " G=" << int(a[idx + 1]) << " B=" << int(a[idx + 2]) << std::endl;
        std::cout << "GPU pixel: R=" << int(b[idx + 0]) << " G=" << int(b[idx + 1]) << " B=" << int(b[idx + 2]) << std::endl;
    }
}

void compare_gray_images(const std::vector<unsigned char>& a, const std::vector<unsigned char>& b, int width, int height) {
    size_t total = static_cast<size_t>(width) * height;
    size_t mismatches = 0;
    size_t first_idx = SIZE_MAX;
    for (size_t i = 0; i < total; ++i) {
        if (a[i] != b[i]) {
            ++mismatches;
            if (first_idx == SIZE_MAX) first_idx = i;
        }
    }
    std::cout << "Gray comparison: mismatches = " << mismatches << " / " << total;
    if (total) {
        double pct = 100.0 * mismatches / static_cast<double>(total);
        std::cout << " (" << pct << "%)";
    }
    std::cout << std::endl;
    if (mismatches && first_idx != SIZE_MAX) {
        int y = static_cast<int>(first_idx) / width;
        int x = static_cast<int>(first_idx) % width;
        std::cout << "First mismatch at pixel (x,y)=(" << x << "," << y << ")" << std::endl;
        std::cout << "CPU gray=" << int(a[first_idx]) << " GPU gray=" << int(b[first_idx]) << std::endl;
    }
}

// -------------------------- Main --------------------------
int main() {
    std::cout << "INPUT_FILE = " << INPUT_FILE << std::endl;
    std::cout << "MODE config = " << MODE << " (0=auto,1=color,2=gray)" << std::endl;
    std::vector<unsigned char> input_image;
    int width = 0, height = 0, bpp = 0;
    if (!load_bmp(INPUT_FILE, input_image, width, height, bpp)) {
        std::cerr << "Failed to load input image." << std::endl;
        return 1;
    }
    std::cout << "Loaded: " << width << " x " << height << " bpp=" << bpp << std::endl;
    // If you want to inspect first pixels, uncomment:
    /* for (int i = 0; i < std::min(16, width*height); ++i) {
        auto p = input_image[i];
        std::cout << "px["<<i<<"] R="<<int(p.x)<<" G="<<int(p.y)<<" B="<<int(p.z)<<"\n";
    } */
    int mode = MODE;
    if (mode == 0) {
        mode = detect_image_mode(input_image, width, height, bpp);
    }
    std::cout << "Processing mode = " << (mode == 1 ? "color" : "gray") << std::endl;
    std::string filename_only = get_filename_from_path(INPUT_FILE);
    std::string stem = get_stem_from_filename(filename_only);
    std::string dir = get_directory_from_path(INPUT_FILE);
    if (mode == 1) {
        // Color path
        std::vector<unsigned char> cpu_out;
        std::vector<unsigned char> gpu_out;
        double cpu_ms = 0.0;
        float gpu_ms = 0.0f;
        std::cout << "Running CPU color filter..." << std::endl;
        cpu_out = gaussian_filter_color_cpu(input_image, width, height, cpu_ms);
        std::cout << "CPU done: " << cpu_ms << " ms" << std::endl;
        std::cout << "Running GPU color filter..." << std::endl;
        gaussian_filter_color_gpu(input_image, gpu_out, width, height, gpu_ms);
        std::cout << "GPU done: " << gpu_ms << " ms" << std::endl;
        std::string cpu_file = dir + "/cpu_" + stem + "_color.bmp";
        std::string gpu_file = dir + "/gpu_" + stem + "_color.bmp";
        remove_if_exists(cpu_file);
        remove_if_exists(gpu_file);
        if (!save_bmp(cpu_file, cpu_out, width, height)) {
            std::cerr << "Failed to save CPU color result to " << cpu_file << std::endl;
        } else std::cout << "Saved CPU color result: " << cpu_file << std::endl;
        if (!save_bmp(gpu_file, gpu_out, width, height)) {
            std::cerr << "Failed to save GPU color result to " << gpu_file << std::endl;
        } else std::cout << "Saved GPU color result: " << gpu_file << std::endl;
        compare_color_images(cpu_out, gpu_out, width, height);
    } else {
        // Gray path
        std::vector<unsigned char> gray_input(static_cast<size_t>(width) * height);
        for (int i = 0; i < width * height; ++i) {
            gray_input[i] = static_cast<unsigned char>(0.299f * input_image[i * 3 + 0] + 0.587f * input_image[i * 3 + 1] + 0.114f * input_image[i * 3 + 2]);
        }
        double cpu_ms = 0.0;
        float gpu_ms = 0.0f;
        std::cout << "Running CPU gray filter..." << std::endl;
        auto cpu_out = gaussian_filter_gray_cpu(gray_input, width, height, cpu_ms);
        std::cout << "CPU done: " << cpu_ms << " ms" << std::endl;
        std::cout << "Running GPU gray filter..." << std::endl;
        std::vector<unsigned char> gpu_out;
        gaussian_filter_gray_gpu(gray_input, gpu_out, width, height, gpu_ms);
        std::cout << "GPU done: " << gpu_ms << " ms" << std::endl;
        std::string cpu_file = dir + "/cpu_" + stem + "_gray.bmp";
        std::string gpu_file = dir + "/gpu_" + stem + "_gray.bmp";
        remove_if_exists(cpu_file);
        remove_if_exists(gpu_file);
        if (!save_bmp_grayscale(cpu_file, cpu_out, width, height)) {
            std::cerr << "Failed to save CPU gray result to " << cpu_file << std::endl;
        } else std::cout << "Saved CPU gray result: " << cpu_file << std::endl;
        if (!save_bmp_grayscale(gpu_file, gpu_out, width, height)) {
            std::cerr << "Failed to save GPU gray result to " << gpu_file << std::endl;
        } else std::cout << "Saved GPU gray result: " << gpu_file << std::endl;
        compare_gray_images(cpu_out, gpu_out, width, height);
    }
    std::cout << "Finished." << std::endl;
    return 0;
}