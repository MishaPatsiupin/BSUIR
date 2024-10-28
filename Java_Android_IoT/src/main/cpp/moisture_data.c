#include <jni.h>
#include <math.h>
#include <stdlib.h>

void smooth_data(float* data, int size) {
    float filtered_data[101]; // Массив для хранения отфильтрованных данных
    int window_size = 3; // Размер окна для скользящего среднего

    for (int i = 0; i < size; i++) {
        float sum = 0.0;
        int count = 0;

        // Суммируем значения в окне
        for (int j = -window_size; j <= window_size; j++) {
            if (i + j >= 0 && i + j < size) {
                sum += data[i + j];
                count++;
            }
        }

        // Вычисляем среднее значение
        filtered_data[i] = sum / count;
    }

    // Копируем отфильтрованные данные обратно в исходный массив
    for (int i = 0; i < size; i++) {
        data[i] = filtered_data[i];
    }
}

JNIEXPORT void JNICALL Java_com_example_home_DeviceDetailActivity_smoothData(JNIEnv *env, jobject obj, jfloatArray dataArray) {
    jsize length = (*env)->GetArrayLength(env, dataArray);
    jfloat* data = (*env)->GetFloatArrayElements(env, dataArray, 0);

    smooth_data(data, length);

    (*env)->ReleaseFloatArrayElements(env, dataArray, data, 0);
}