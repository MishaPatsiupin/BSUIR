#include <bits/stdc++.h>
#include <cuda_runtime.h>


using namespace std;

const int N = 2048 * 1;  
const int M = 2048 * 2;  

__global__ void rearrange(float *in, float *out, int rows, int cols) {
    int i = blockIdx.y * blockDim.y + threadIdx.y;
    int j = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < rows && j < cols) {
        int half_rows = rows / 2;
        int orig_i = (i < half_rows) ? 2 * i : 2 * (i - half_rows) + 1;
        int half_cols = cols / 2;
        int orig_j = (j < half_cols) ? 2 * j : 2 * (j - half_cols) + 1;
        
        // Обработка крайних случаев: если orig_i или orig_j за границами — ставим 0
        if (orig_i < rows && orig_j < cols) {
            out[i * cols + j] = in[orig_i * cols + orig_j];
        } else {
            out[i * cols + j] = 0.0f;
        }
    }
}


__global__ void exclusive_scan(float *input, float *output, int rows, int cols) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    if (row < rows) {
        // Каждый блок обрабатывает одну строку
        float sum = 0.0f;
        int base = row * cols;
        
        // Простой последовательный scan для строки
        for (int j = 0; j < cols; j++) {
            float curr = input[base + j];
            output[base + j] = sum;
            sum += curr;
        }
    }
}

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
uniform_int_distribution<int> dis(1, 2); 
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
            // Обработка крайних случаев: если orig_i или orig_j за границами — ставим 0
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
    float *d_in, *d_out;
    cudaMalloc(&d_in, N * M * sizeof(float));
    cudaMalloc(&d_out, N * M * sizeof(float));
    cudaMemcpy(d_in, h_in.data(), N * M * sizeof(float), cudaMemcpyHostToDevice);

    // Измерение времени на GPU (ядро перестановки)
    cudaEvent_t gpu_start, gpu_stop;
    cudaEventCreate(&gpu_start);
    cudaEventCreate(&gpu_stop);
    cudaEventRecord(gpu_start);

    // Запуск ядра
    dim3 block(16, 16);
    dim3 grid((M + 15) / 16, (N + 15) / 16);
    rearrange<<<grid, block>>>(d_in, d_out, N, M);

    cudaEventRecord(gpu_stop);
    cudaEventSynchronize(gpu_stop);
    float gpu_time_ms;
    cudaEventElapsedTime(&gpu_time_ms, gpu_start, gpu_stop);

    // -------------------------------
    // GPU exclusive-scan по строкам: выполняем один segmented scan через exclusive_scan_by_key
    float *d_scan_out;
    cudaMalloc(&d_scan_out, N * M * sizeof(float));

    cudaEvent_t gpu_scan_start, gpu_scan_stop;
    cudaEventCreate(&gpu_scan_start);
    cudaEventCreate(&gpu_scan_stop);
    cudaEventRecord(gpu_scan_start);

    // Запускаем наше простое ядро scan
    dim3 scan_block(1, 16);  // Один поток обрабатывает целую строку
    dim3 scan_grid(1, (N + scan_block.y - 1) / scan_block.y);
    exclusive_scan<<<scan_grid, scan_block>>>(d_out, d_scan_out, N, M);

    cudaEventRecord(gpu_scan_stop);
    cudaEventSynchronize(gpu_scan_stop);
    float gpu_scan_time_ms;
    cudaEventElapsedTime(&gpu_scan_time_ms, gpu_scan_start, gpu_scan_stop);
    // -------------------------------

    // Копируем результат перестановки обратно (оставлено для проверки оригинального вывода GPU)
    cudaMemcpy(h_out_gpu.data(), d_out, N * M * sizeof(float), cudaMemcpyDeviceToHost);
    // Копируем результат скана обратно
    cudaMemcpy(h_scan_gpu.data(), d_scan_out, N * M * sizeof(float), cudaMemcpyDeviceToHost);

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
            if (diff_count <= 5) {  // Показать первые 5 различий
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
    cout << "GPU time: " << fixed << setprecision(4) << gpu_time_ms << " ms" << endl;
    if (gpu_time_ms > 0) {
        cout << "GPU speedup: " << fixed << setprecision(2) << (cpu_time_ms / gpu_time_ms) << "x" << endl;
    }
    cout << "CPU exclusive-scan time (per-row): " << fixed << setprecision(4) << cpu_scan_time_ms << " ms" << endl;
    cout << "GPU exclusive-scan time (segmented, Thrust): " << fixed << setprecision(4) << gpu_scan_time_ms << " ms" << endl;
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