// scan_full.cu
// Полный пример: заменяет только логику exclusive-scan.
// - Пытается выполнить single-kernel вариант с grid-sync через cooperative groups.
// - Если cooperative не поддерживается, использует fallback: три отдельных ядра.
// Комментарии поясняют идею и этапы.

// Компилировать: nvcc scan_full.cu -o scan
// Требования для single-kernel (cooperative):
//  - CUDA runtime + driver поддерживают cooperative launches на данном GPU.
//  - compute capability и драйвер соответствуют (обычно Pascal и новее, но зависит от карт и драйвера).
// Если cooperative недоступен — будет использован fallback (мульти-ядра), который работает везде.

#include <bits/stdc++.h>
#include <cuda_runtime.h>
#include <cooperative_groups.h>

using namespace std;
namespace cg = cooperative_groups;

const int N = 2048 * 1;
const int M = 2048 * 2;

#define BLOCK_SIZE 64
#define ELEMS_PER_BLOCK (2 * BLOCK_SIZE)

// -------------------------------------------------------------
// rearrange (оставлена без изменений)
__global__ void rearrange(float *in, float *out, int rows, int cols) {
    int i = blockIdx.y * blockDim.y + threadIdx.y;
    int j = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < rows && j < cols) {
        int half_rows = rows / 2;
        int orig_i = (i < half_rows) ? 2 * i : 2 * (i - half_rows) + 1;
        int half_cols = cols / 2;
        int orig_j = (j < half_cols) ? 2 * j : 2 * (j - half_cols) + 1;

        if (orig_i < rows && orig_j < cols) {
            out[i * cols + j] = in[orig_i * cols + orig_j];
        } else {
            out[i * cols + j] = 0.0f;
        }
    }
}

// -------------------------------------------------------------
// FALLBACK IMPLEMENTATION (три ядра) — используется если cooperative не поддерживается.
// Эти ядра ровно реализуют три шага: block-scan, scan по block_sums, добавление offsets.
// Я включаю fallback, чтобы код был рабочим на всех GPU.

// 1) per-block Blelloch scan; writes exclusive results to output and block totals to block_sums[row*numBlocksPerRow + block_in_row]
__global__ void scanBlockKernel_fallback(const float *input, float *output,
                                 float *block_sums, int rows, int cols, int numBlocksPerRow) {
    int row = blockIdx.y;
    int block_in_row = blockIdx.x;
    int tid = threadIdx.x;

    if (row >= rows) return;

    int base = row * cols + block_in_row * ELEMS_PER_BLOCK;
    int row_end = (row + 1) * cols;

    __shared__ float sdata[ELEMS_PER_BLOCK];

    // load up to 2 elements per thread
    int idx1 = base + tid;
    int idx2 = base + tid + BLOCK_SIZE;
    float v1 = (idx1 < row_end) ? input[idx1] : 0.0f;
    float v2 = (idx2 < row_end) ? input[idx2] : 0.0f;
    sdata[tid] = v1;
    sdata[tid + BLOCK_SIZE] = v2;
    __syncthreads();

    int n = ELEMS_PER_BLOCK;
    // upsweep
    for (int offset = 1; offset < n; offset <<= 1) {
        int idx = (tid + 1) * (offset << 1) - 1;
        if (idx < n) sdata[idx] += sdata[idx - offset];
        __syncthreads();
    }

    float total = 0.0f;
    if (tid == 0) total = sdata[n - 1];

    if (tid == 0) sdata[n - 1] = 0.0f;
    __syncthreads();

    // downsweep
    for (int offset = n >> 1; offset > 0; offset >>= 1) {
        int idx = (tid + 1) * (offset << 1) - 1;
        if (idx < n) {
            float tmp = sdata[idx - offset];
            sdata[idx - offset] = sdata[idx];
            sdata[idx] += tmp;
        }
        __syncthreads();
    }

    if (idx1 < row_end) output[idx1] = sdata[tid];
    if (idx2 < row_end) output[idx2] = sdata[tid + BLOCK_SIZE];

    if (tid == 0) {
        int out_index = row * numBlocksPerRow + block_in_row;
        block_sums[out_index] = total;
    }
}

// 2) scan per-row block_sums (one block per row)
__global__ void scanSumsKernel_fallback(float *block_sums, int numBlocksPerRow) {
    int row = blockIdx.x;
    int t = threadIdx.x;
    extern __shared__ float s[]; // use dynamic shared mem sized to ELEMS_PER_BLOCK floats

    float *row_ptr = block_sums + (size_t)row * numBlocksPerRow;

    // load
    if (t < numBlocksPerRow) s[t] = row_ptr[t];
    else s[t] = 0.0f;
    // zero upper part if any
    if (t + BLOCK_SIZE < ELEMS_PER_BLOCK) s[t + BLOCK_SIZE] = 0.0f;
    __syncthreads();

    int n = ELEMS_PER_BLOCK;
    for (int offset = 1; offset < n; offset <<= 1) {
        int idx = (t + 1) * (offset << 1) - 1;
        if (idx < n) s[idx] += s[idx - offset];
        __syncthreads();
    }
    if (t == 0) s[n - 1] = 0.0f;
    __syncthreads();
    for (int offset = n >> 1; offset > 0; offset >>= 1) {
        int idx = (t + 1) * (offset << 1) - 1;
        if (idx < n) {
            float tmp = s[idx - offset];
            s[idx - offset] = s[idx];
            s[idx] += tmp;
        }
        __syncthreads();
    }

    if (t < numBlocksPerRow) row_ptr[t] = s[t];
}

// 3) add offsets to each block's output
__global__ void addOffsetsKernel_fallback(float *data, const float *scanned_block_sums,
                                  int rows, int cols, int numBlocksPerRow) {
    int row = blockIdx.y;
    int block_in_row = blockIdx.x;
    int tid = threadIdx.x;
    if (row >= rows) return;

    int base = row * cols + block_in_row * ELEMS_PER_BLOCK;
    int row_end = (row + 1) * cols;
    float add = scanned_block_sums[row * numBlocksPerRow + block_in_row];

    int idx1 = base + tid;
    int idx2 = base + tid + BLOCK_SIZE;
    if (idx1 < row_end) data[idx1] += add;
    if (idx2 < row_end) data[idx2] += add;
}

// -------------------------------------------------------------
// SINGLE-KERNEL implementation using cooperative groups:
// This kernel performs all steps for the entire grid (all rows and all blocks per row).
// It requires grid-level synchronization (cg::this_grid().sync()) which works only
// if kernel is launched as a cooperative kernel (cudaLaunchCooperativeKernel) and
// the device & driver support that.
//
// Layout: gridDim.x = numBlocksPerRow, gridDim.y = rows
__global__ void exclusive_scan_coop(const float *input, float *output,
                                    float *block_sums, int rows, int cols, int numBlocksPerRow) {
    // Obtain a handle to the whole grid for grid-level sync
    cg::grid_group grid = cg::this_grid();

    int row = blockIdx.y;
    int block_in_row = blockIdx.x;
    int tid = threadIdx.x;

    if (row >= rows) return;
    int base = row * cols + block_in_row * ELEMS_PER_BLOCK;
    int row_end = (row + 1) * cols;

    // Shared mem for block-local scan
    __shared__ float sdata[ELEMS_PER_BLOCK];

    // --- Phase A: local (block) Blelloch scan over up to ELEMS_PER_BLOCK elements ---
    int idx1 = base + tid;
    int idx2 = base + tid + BLOCK_SIZE;
    float v1 = (idx1 < row_end) ? input[idx1] : 0.0f;
    float v2 = (idx2 < row_end) ? input[idx2] : 0.0f;
    sdata[tid] = v1;
    sdata[tid + BLOCK_SIZE] = v2;
    __syncthreads();

    int n = ELEMS_PER_BLOCK;
    // upsweep
    for (int offset = 1; offset < n; offset <<= 1) {
        int idx = (tid + 1) * (offset << 1) - 1;
        if (idx < n) sdata[idx] += sdata[idx - offset];
        __syncthreads();
    }

    float block_total = 0.0f;
    if (tid == 0) block_total = sdata[n - 1];

    if (tid == 0) sdata[n - 1] = 0.0f;
    __syncthreads();

    // downsweep
    for (int offset = n >> 1; offset > 0; offset >>= 1) {
        int idx = (tid + 1) * (offset << 1) - 1;
        if (idx < n) {
            float tmp = sdata[idx - offset];
            sdata[idx - offset] = sdata[idx];
            sdata[idx] += tmp;
        }
        __syncthreads();
    }

    // write block-local exclusive results
    if (idx1 < row_end) output[idx1] = sdata[tid];
    if (idx2 < row_end) output[idx2] = sdata[tid + BLOCK_SIZE];

    // write block total into global block_sums array (one entry per block)
    int bs_idx = row * numBlocksPerRow + block_in_row;
    if (tid == 0) block_sums[bs_idx] = block_total;

    // --- Grid sync: all blocks must finish writing their block_sums ---
    grid.sync();

    // --- Phase B: scan block_sums per row ---
    // Let the first block in the row (block_in_row == 0) perform the scan of that row's block_sums.
    if (block_in_row == 0) {
        // We'll use the same shared memory area (sdata) but reinterpret size.
        // Only threads with tid < numBlocksPerRow participate in the scan.
        // Initialize shared buffer (pad with zeros up to ELEMS_PER_BLOCK)
        if (tid < numBlocksPerRow) {
            sdata[tid] = block_sums[row * numBlocksPerRow + tid];
        } else {
            sdata[tid] = 0.0f;
        }
        // zeros for upper half
        if (tid + BLOCK_SIZE < ELEMS_PER_BLOCK) sdata[tid + BLOCK_SIZE] = 0.0f;
        __syncthreads();

        // do Blelloch on sdata (size ELEMS_PER_BLOCK, but effective numBlocksPerRow values used)
        for (int offset = 1; offset < n; offset <<= 1) {
            int idx = (tid + 1) * (offset << 1) - 1;
            if (idx < n) sdata[idx] += sdata[idx - offset];
            __syncthreads();
        }
        if (tid == 0) sdata[n - 1] = 0.0f;
        __syncthreads();
        for (int offset = n >> 1; offset > 0; offset >>= 1) {
            int idx = (tid + 1) * (offset << 1) - 1;
            if (idx < n) {
                float tmp = sdata[idx - offset];
                sdata[idx - offset] = sdata[idx];
                sdata[idx] += tmp;
            }
            __syncthreads();
        }

        // write back only first numBlocksPerRow results
        if (tid < numBlocksPerRow) {
            block_sums[row * numBlocksPerRow + tid] = sdata[tid];
        }
    }

    // --- Grid sync: ensure scanned block_sums are visible to all blocks ---
    grid.sync();

    // --- Phase C: add offsets to each element in block (scanned block_sums holds exclusive-sum_of_previous_blocks) ---
    float add = block_sums[bs_idx];

    // add to output elements (which currently contain local exclusive result)
    if (idx1 < row_end) output[idx1] += add;
    if (idx2 < row_end) output[idx2] += add;

    // done
}

// -------------------------------------------------------------
// Host helper: launch either cooperative single-kernel or fallback sequence
void run_scan_rows_single_or_fallback(const float *d_input, float *d_output,
                                      int rows, int cols) {
    int numBlocksPerRow = (cols + ELEMS_PER_BLOCK - 1) / ELEMS_PER_BLOCK;
    int totalBlockSums = rows * numBlocksPerRow;

    // allocate global block_sums buffer
    float *d_block_sums = nullptr;
    cudaCheck(cudaMalloc(&d_block_sums, sizeof(float) * (size_t)totalBlockSums));
    cudaCheck(cudaMemset(d_block_sums, 0, sizeof(float) * (size_t)totalBlockSums));

    // Grid / block sizes
    dim3 block(BLOCK_SIZE);
    dim3 grid(numBlocksPerRow, rows);

    // Check device capability for cooperative launch
    int canCoop = 0;
    cudaDeviceGetAttribute(&canCoop, cudaDevAttrCooperativeLaunch, 0);
    if (canCoop) {
        // cooperative launch supported — prepare args and call
        void *kernelArgs[] = {
            (void*)&d_input, (void*)&d_output, (void*)&d_block_sums,
            (void*)&rows, (void*)&cols, (void*)&numBlocksPerRow
        };

        // Try to launch cooperative kernel. If it fails, fall back.
        cudaError_t err = cudaLaunchCooperativeKernel((void*)exclusive_scan_coop,
                                                      grid, block,
                                                      kernelArgs);
        if (err != cudaSuccess) {
            fprintf(stderr, "cooperative kernel launch failed (%s). Falling back.\n", cudaGetErrorString(err));
            // fallback sequence below
        } else {
            // sync and cleanup
            cudaCheck(cudaDeviceSynchronize());
            cudaFree(d_block_sums);
            return;
        }
    }

    // FALLBACK (regular kernels): 3-step approach
    {
        // 1) per-block scan -> writes block_sums (scanBlockKernel_fallback)
        scanBlockKernel_fallback<<<grid, block>>>(d_input, d_output, d_block_sums, rows, cols, numBlocksPerRow);
        cudaCheck(cudaGetLastError());
        cudaCheck(cudaDeviceSynchronize());

        // 2) scan block_sums per row -> we launch rows blocks (grid.x = rows), each block with BLOCK_SIZE threads
        //    and use shared memory ELEMS_PER_BLOCK * sizeof(float)
        dim3 grid2(rows, 1);
        scanSumsKernel_fallback<<<grid2, BLOCK_SIZE, ELEMS_PER_BLOCK * sizeof(float)>>>(d_block_sums, numBlocksPerRow);
        cudaCheck(cudaGetLastError());
        cudaCheck(cudaDeviceSynchronize());

        // 3) add offsets
        addOffsetsKernel_fallback<<<grid, block>>>(d_output, d_block_sums, rows, cols, numBlocksPerRow);
        cudaCheck(cudaGetLastError());
        cudaCheck(cudaDeviceSynchronize());
    }

    cudaFree(d_block_sums);
}

// small helper macro for brevity
inline void cudaCheck(cudaError_t e) {
    if (e != cudaSuccess) {
        fprintf(stderr, "CUDA Error: %s\n", cudaGetErrorString(e));
        exit(1);
    }
}

// -------------------------------------------------------------
// main: оставил твою структуру, поменял только место вызова scan
int main() {
    // Выделение памяти на хосте
    vector<float> h_in(N * M);
    vector<float> h_out_cpu(N * M);
    vector<float> h_out_gpu(N * M);

    // Буферы для exclusive-scan по строкам
    vector<float> h_scan_cpu(N * M);
    vector<float> h_scan_gpu(N * M);

    // Заполнение входной матрицы числами (0 или 1)
    mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> dis(0, 1);
    for (auto &x : h_in) {
        x = static_cast<float>(dis(gen));
    }

    // CPU версия: перестановка элементов
    auto cpu_start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        int half_rows = N / 2;
        int orig_i = (i < half_rows) ? 2 * i : 2 * (i - half_rows) + 1;
        for (int j = 0; j < M; ++j) {
            int half_cols = M / 2;
            int orig_j = (j < half_cols) ? 2 * j : 2 * (j - half_cols) + 1;
            if (orig_i < N && orig_j < M) {
                h_out_cpu[i * M + j] = h_in[orig_i * M + orig_j];
            } else {
                h_out_cpu[i * M + j] = 0.0f;
            }
        }
    }
    auto cpu_end = chrono::high_resolution_clock::now();
    double cpu_time_ms = chrono::duration<double, milli>(cpu_end - cpu_start).count();

    // CPU exclusive-scan по строкам (последовательно, left-to-right)
    auto cpu_scan_start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        float acc = 0.0f;
        int base = i * M;
        for (int j = 0; j < M; ++j) {
            h_scan_cpu[base + j] = acc;
            acc += h_out_cpu[base + j];
        }
    }
    auto cpu_scan_end = chrono::high_resolution_clock::now();
    double cpu_scan_time_ms = chrono::duration<double, milli>(cpu_scan_end - cpu_scan_start).count();
    // -------------------------------

    // GPU версия: копируем данные на устройство
    float *d_in = nullptr, *d_out = nullptr;
    cudaCheck(cudaMalloc(&d_in, (size_t)N * M * sizeof(float)));
    cudaCheck(cudaMalloc(&d_out, (size_t)N * M * sizeof(float)));
    cudaCheck(cudaMemcpy(d_in, h_in.data(), (size_t)N * M * sizeof(float), cudaMemcpyHostToDevice));

    // Измерение времени на GPU (ядро перестановки)
    cudaEvent_t gpu_start, gpu_stop;
    cudaEventCreate(&gpu_start);
    cudaEventCreate(&gpu_stop);
    cudaEventRecord(gpu_start);

    // Запуск ядра rearrange
    dim3 blockR(16, 16);
    dim3 gridR((M + 15) / 16, (N + 15) / 16);
    rearrange<<<gridR, blockR>>>(d_in, d_out, N, M);

    cudaEventRecord(gpu_stop);
    cudaEventSynchronize(gpu_stop);
    float gpu_time_ms;
    cudaEventElapsedTime(&gpu_time_ms, gpu_start, gpu_stop);

    // -------------------------------
    // GPU exclusive-scan по строкам: single-kernel (coop) или fallback
    float *d_scan_out = nullptr;
    cudaCheck(cudaMalloc(&d_scan_out, (size_t)N * M * sizeof(float)));

    cudaEvent_t gpu_scan_start, gpu_scan_stop;
    cudaEventCreate(&gpu_scan_start);
    cudaEventCreate(&gpu_scan_stop);
    cudaEventRecord(gpu_scan_start);

    // Вызов нашей обёртки: внутри она либо выполнит cooperative single-kernel,
    // либо fallback (3 kernels). d_in->d_out was permutation output; we scan d_out into d_scan_out.
    run_scan_rows_single_or_fallback(d_out, d_scan_out, N, M);

    cudaEventRecord(gpu_scan_stop);
    cudaEventSynchronize(gpu_scan_stop);
    float gpu_scan_time_ms;
    cudaEventElapsedTime(&gpu_scan_time_ms, gpu_scan_start, gpu_scan_stop);
    // -------------------------------

    // Копируем результат перестановки обратно (оставлено для проверки оригинального вывода GPU)
    cudaCheck(cudaMemcpy(h_out_gpu.data(), d_out, (size_t)N * M * sizeof(float), cudaMemcpyDeviceToHost));
    // Копируем результат скана обратно
    cudaCheck(cudaMemcpy(h_scan_gpu.data(), d_scan_out, (size_t)N * M * sizeof(float), cudaMemcpyDeviceToHost));

    // Освобождение памяти на GPU
    cudaFree(d_in);
    cudaFree(d_out);
    cudaFree(d_scan_out);
    cudaEventDestroy(gpu_start);
    cudaEventDestroy(gpu_stop);
    cudaEventDestroy(gpu_scan_start);
    cudaEventDestroy(gpu_scan_stop);

    // Встроенная проверка: поэлементное сравнение CPU и GPU
    bool match = true;
    int diff_count = 0;
    for (int k = 0; k < N * M; ++k) {
        if (abs(h_out_cpu[k] - h_out_gpu[k]) > 1e-5f) {
            match = false;
            diff_count++;
            if (diff_count <= 5) {
                cout << "Различие в позиции " << k / M << "," << k % M
                     << ": CPU=" << fixed << setprecision(4) << h_out_cpu[k]
                     << ", GPU=" << h_out_gpu[k] << endl;
            }
        }
    }

    // Вывод времени и проверки
    cout << "Results match: " << (match ? "Yes" : "No") << endl;
    if (!match) {
        cout << "Количество различий: " << diff_count << endl;
    }
    cout << "CPU time: " << fixed << setprecision(4) << cpu_time_ms << " ms" << endl;
    cout << "GPU time (rearrange): " << fixed << setprecision(4) << gpu_time_ms << " ms" << endl;
    if (gpu_time_ms > 0) {
        cout << "GPU speedup (rearrange): " << fixed << setprecision(2) << (cpu_time_ms / gpu_time_ms) << "x" << endl;
    }
    cout << "CPU exclusive-scan time: " << fixed << setprecision(4) << cpu_scan_time_ms << " ms" << endl;
    cout << "GPU exclusive-scan time: " << fixed << setprecision(4) << gpu_scan_time_ms << " ms" << endl;
    if (gpu_scan_time_ms > 0) {
        cout << "Scan speedup (CPU/GPU): " << fixed << setprecision(2) << (cpu_scan_time_ms / gpu_scan_time_ms) << "x" << endl;
    }
    cout << endl;

    // Сравнение результатов скана (показываем первые несоответствия)
    bool scan_match = true;
    int scan_diff = 0;
    const float tol = 1e-3f;
    for (int k = 0; k < N * M; ++k) {
        if (isnan(h_scan_cpu[k]) != isnan(h_scan_gpu[k]) ||
            fabsf(h_scan_cpu[k] - h_scan_gpu[k]) > tol) {
            scan_match = false;
            scan_diff++;
            if (scan_diff <= 5) {
                cout << "Scan diff at " << k / M << "," << k % M << ": CPU=" << h_scan_cpu[k]
                     << ", GPU=" << h_scan_gpu[k] << endl;
            }
        }
    }
    cout << "Scan results match: " << (scan_match ? "Yes" : "No") << endl;
    if (!scan_match) cout << "Scan differences count: " << scan_diff << endl;
    cout << endl;

    // Сохранение input и результатов в файлы
    cout << "Сохранение в файлы..." << endl;
    // Удаление старых файлов
    remove("input.txt");
    remove("cpu_output.txt");
    remove("gpu_output.txt");

    // Сохранение input
    ofstream input_file("input.txt");
    input_file << fixed << setprecision(4);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            input_file << setw(10) << h_in[i * M + j];
        }
        input_file << endl;
    }
    input_file.close();
    cout << "Input saved to input.txt" << endl;

    // Сохранение CPU output
    ofstream cpu_file("cpu_output.txt");
    cpu_file << fixed << setprecision(4);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            cpu_file << setw(10) << h_out_cpu[i * M + j];
        }
        cpu_file << endl;
    }
    cpu_file.close();
    cout << "CPU output saved to cpu_output.txt" << endl;

    // Сохранение GPU output
    ofstream gpu_file("gpu_output.txt");
    gpu_file << fixed << setprecision(4);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            gpu_file << setw(10) << h_out_gpu[i * M + j];
        }
        gpu_file << endl;
    }
    gpu_file.close();
    cout << "GPU output saved to gpu_output.txt" << endl;
    cout << endl;

    // Сохранение результатов scan
    remove("cpu_scan.txt");
    remove("gpu_scan.txt");

    // Сохранение CPU scan results
    ofstream cpu_scan_file("cpu_scan.txt");
    cpu_scan_file << fixed << setprecision(4);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            cpu_scan_file << setw(10) << h_scan_cpu[i * M + j];
        }
        cpu_scan_file << endl;
    }
    cpu_scan_file.close();
    cout << "CPU scan results saved to cpu_scan.txt" << endl;

    // Сохранение GPU scan results
    ofstream gpu_scan_file("gpu_scan.txt");
    gpu_scan_file << fixed << setprecision(4);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            gpu_scan_file << setw(10) << h_scan_gpu[i * M + j];
        }
        gpu_scan_file << endl;
    }
    gpu_scan_file.close();
    cout << "GPU scan results saved to gpu_scan.txt" << endl;
    cout << endl;

    // Показ фрагмента 4x4 для визуальной проверки
    cout << "Input fragment (first 4x4):" << endl;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 8; ++j) {
            cout << setw(10) << fixed << setprecision(4) << h_in[i * M + j];
        }
        cout << endl;
    }
    cout << endl;

    cout << "CPU output fragment (first 4x4):" << endl;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 8; ++j) {
            cout << setw(10) << fixed << setprecision(4) << h_out_cpu[i * M + j];
        }
        cout << endl;
    }
    cout << endl;

    cout << "GPU output fragment (first 4x4):" << endl;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 8; ++j) {
            cout << setw(10) << fixed << setprecision(4) << h_out_gpu[i * M + j];
        }
        cout << endl;
    }
    cout << endl;

    // Показ фрагмента scan результатов (4x4)
    cout << "CPU scan results fragment (first 4x4):" << endl;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 8; ++j) {
            cout << setw(10) << fixed << setprecision(4) << h_scan_cpu[i * M + j];
        }
        cout << endl;
    }
    cout << endl;

    cout << "GPU scan results fragment (first 4x4):" << endl;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 8; ++j) {
            cout << setw(10) << fixed << setprecision(4) << h_scan_gpu[i * M + j];
        }
        cout << endl;
    }
    cout << endl;

    return 0;
}
