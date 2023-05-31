#include "Header1.h"

bool is_bmp_file(const char* filename) {
    int len = strlen(filename);
    if (len <= 4) {
        return false;
    }
    return (strcmp(&filename[len - 4], ".bmp") == 0 || strcmp(&filename[len - 4], ".BMP") == 0);
}

int is_bmp_24bit(const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0; // не удалось открыть файл
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, fp);

    if (header[0] != 'B' || header[1] != 'M') {
        fclose(fp);
        return 0; // файл не является BMP изображением
    }

    int bit_count = *(int*)&header[28];
    if (bit_count != 24) {
        fclose(fp);
        return 0; // файл не является 24-bit BMP изображением
    }

    fclose(fp);
    return 1; // файл является 24-bit BMP изображением
}

void input_bmp_filename(char* filename, int max_length) {
    int attermet = 0;
    bool valid = false;
    while (!valid) {
        printf("Enter filename BMP: ");
        fgets(filename, max_length, stdin);

        // Удаляем символ новой строки
        filename[strcspn(filename, "\n")] = '\0';
        if (is_bmp_file(filename) && is_bmp_24bit(filename)) {
            valid = true;
        }
        else {
            printf("Invalid filename. Make sure the file extension is 24-bit.bmp\n");
            attermet++;
        }
        if (attermet > 2)
            exit(1);
    }
}


void print_menu() {
    printf("0 - Exit\n");
    printf("1 - Negative\n");
    printf("2 - Black and White\n");
    //    printf("3 - Apply multiple effects\n");
    printf("3 - Gamma Correction\n");
    printf("4 - Median Filter\n");
}
/*
void read_file(char *filename, BITMAPFILEHEADER *file_header, BITMAPINFOHEADER *info_header, RGBTRIPLE* pixels, RGBTRIPLE* original_pixels, int row_size){
    FILE* input_file = fopen(filename, "rb");

    fread(&file_header, sizeof(BITMAPFILEHEADER), 1, input_file);
    fread(&info_header, sizeof(BITMAPINFOHEADER), 1, input_file);
    row_size = (info_header.biWidth * 3 + 3) & ~3;
    pixels = malloc(row_size * info_header.biHeight);
    original_pixels = malloc(row_size * info_header.biHeight);
    fseek(input_file, file_header.bfOffBits, SEEK_SET);
    for (int i = 0; i < info_header.biHeight; i++) {
        fread(pixels + row_size / sizeof(RGBTRIPLE) * i, row_size, 1, input_file);
        memcpy(original_pixels + row_size / sizeof(RGBTRIPLE) * i, pixels + row_size / sizeof(RGBTRIPLE) * i, row_size);
    }
    fclose(input_file);
}*/

void save_image(const char* file_name, BITMAPFILEHEADER file_header, BITMAPINFOHEADER info_header, RGBTRIPLE* pixels, int row_size) {
    FILE* output_file = fopen(file_name, "wb");
    fwrite(&file_header, sizeof(BITMAPFILEHEADER), 1, output_file);
    fwrite(&info_header, sizeof(BITMAPINFOHEADER), 1, output_file);
    fseek(output_file, file_header.bfOffBits, SEEK_SET);
    for (int i = 0; i < info_header.biHeight; i++) {
        fwrite(pixels + row_size / sizeof(RGBTRIPLE) * i, row_size, 1, output_file);
    }
    fclose(output_file);
}

void negative(RGBTRIPLE* pixel) {
    pixel->rgbtBlue = ~pixel->rgbtBlue;
    pixel->rgbtGreen = ~pixel->rgbtGreen;
    pixel->rgbtRed = ~pixel->rgbtRed;
}

void to_negative(RGBTRIPLE* pixels, int width, int height, int row_size) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            negative(pixels + row_size / sizeof(RGBTRIPLE) * i + j);
        }
    }
}

void to_gamma_correction(RGBTRIPLE* pixels, int width, int height, int row_size, float gamma) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            RGBTRIPLE* pixel = pixels + row_size / sizeof(RGBTRIPLE) * i + j;
            pixel->rgbtBlue = 255 * pow((pixel->rgbtBlue / 255.0), gamma);
            pixel->rgbtGreen = 255 * pow((pixel->rgbtGreen / 255.0), gamma);
            pixel->rgbtRed = 255 * pow((pixel->rgbtRed / 255.0), gamma);
        }
    }
}

int compare(const void* a, const void* b) {
    return (*(unsigned char*)a - *(unsigned char*)b);
}
void to_median_filter(RGBTRIPLE* pixels, int width, int height, int row_size, int filter_size) {
    RGBTRIPLE* temp_pixels = malloc(row_size * height);
    memcpy(temp_pixels, pixels, row_size * height);

    int half_filter_size = filter_size / 2;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            unsigned char* window_red = malloc(filter_size * filter_size * sizeof(unsigned char));
            unsigned char* window_green = malloc(filter_size * filter_size * sizeof(unsigned char));
            unsigned char* window_blue = malloc(filter_size * filter_size * sizeof(unsigned char));
            int window_index = 0;
            for (int k = -half_filter_size; k <= half_filter_size; k++) {
                for (int l = -half_filter_size; l <= half_filter_size; l++) {
                    int x = j + l;
                    int y = i + k;
                    if (x >= 0 && x < width && y >= 0 && y < height) {
                        RGBTRIPLE* pixel = temp_pixels + row_size / sizeof(RGBTRIPLE) * y + x;
                        window_red[window_index] = pixel->rgbtRed;
                        window_green[window_index] = pixel->rgbtGreen;
                        window_blue[window_index] = pixel->rgbtBlue;
                        window_index++;
                    }
                }
            }
            qsort(window_red, window_index, sizeof(unsigned char), compare);
            qsort(window_green, window_index, sizeof(unsigned char), compare);
            qsort(window_blue, window_index, sizeof(unsigned char), compare);
            RGBTRIPLE* pixel = pixels + row_size / sizeof(RGBTRIPLE) * i + j;
            pixel->rgbtRed = window_red[window_index / 2];
            pixel->rgbtGreen = window_green[window_index / 2];
            pixel->rgbtBlue = window_blue[window_index / 2];
            free(window_red);
            free(window_green);
            free(window_blue);
        }
    }
    free(temp_pixels);
}

void to_black_and_white(RGBTRIPLE* pixels, int width, int height, int row_size) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            black_and_white(pixels + row_size / sizeof(RGBTRIPLE) * i + j);
        }
    }
}

void black_and_white(RGBTRIPLE* pixel) {
    float gray = (pixel->rgbtBlue + pixel->rgbtGreen + pixel->rgbtRed) / 3.0f;
    pixel->rgbtBlue = gray;
    pixel->rgbtGreen = gray;
    pixel->rgbtRed = gray;
}

void black_and_whitechar(char* filename) {

}

void gamma_correction(char* filename) {

}

void median_filter(char* filename) {

}


int get_choice(int kolichestvo) { //прием циферки
    char num;
    fflush(stdin);
    printf("Enter your choice: ");
    do {
        scanf("%c", &num);
        num -= '0';
        if (num < 0 || num > kolichestvo) {
            printf("Wrong number, try again\n");
            rewind(stdin);
            num = -1;
        }
    } while (num < 0 || num > kolichestvo);
    fflush(stdin);
    printf("\n");
    return (int)num;
}