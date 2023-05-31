#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Header.h"


int main() {
    char filename[256];
    input_bmp_filename(filename, 256);
    printf("Valid input filename: %s\n", filename);

    char output_filename[] = "New_picture";

    int row_size;
    BITMAPFILEHEADER file_header;
    BITMAPINFOHEADER info_header;

    // read_file(filename, &file_header, &info_header, &pixels, &original_pixels, &row_size);

    FILE* input_file = fopen(filename, "rb");

    fread(&file_header, sizeof(BITMAPFILEHEADER), 1, input_file);
    fread(&info_header, sizeof(BITMAPINFOHEADER), 1, input_file);
    row_size = (info_header.biWidth * 3 + 3) & ~3;
    void* pixels_ptr = malloc(row_size * info_header.biHeight * sizeof(RGBTRIPLE));
    RGBTRIPLE* pixels = (RGBTRIPLE*)pixels_ptr;
    void* original_pixels_ptr = malloc(row_size * info_header.biHeight * sizeof(RGBTRIPLE));
    RGBTRIPLE* original_pixels = (RGBTRIPLE*)original_pixels_ptr;
    //пофиг
    fseek(input_file, file_header.bfOffBits, SEEK_SET);
    for (int i = 0; i < info_header.biHeight; i++) {
        fread(pixels + row_size / sizeof(RGBTRIPLE) * i, row_size, 1, input_file);
        memcpy(original_pixels + row_size / sizeof(RGBTRIPLE) * i, pixels + row_size / sizeof(RGBTRIPLE) * i, row_size);
    }
    fclose(input_file);



    int choice = -1;
    while (choice != 0) {
        print_menu();
        choice = get_choice(6);
        if (choice != 0) {
            //пофиг
            switch (choice) {
            case 1://negative
                to_negative(pixels, info_header.biWidth, info_header.biHeight, row_size);
                save_image(output_filename, file_header, info_header, pixels, row_size);
                memcpy(pixels, original_pixels, row_size * info_header.biHeight);
                break;
            case 2://black and white
                to_black_and_white(pixels, info_header.biWidth, info_header.biHeight, row_size);
                save_image(output_filename, file_header, info_header, pixels, row_size);
                memcpy(pixels, original_pixels, row_size * info_header.biHeight);
                break;
            case 3://gamma correction
                printf("\033[0;32m Enter the value for gamma correction: \033[0m");
                float gamma;
                scanf("%f", &gamma);
                while (gamma <= 0) {
                    printf("\033[0;33m Error: gamma value must be greater than 0\n \033[0m");
                    printf("\033[0;32m Enter the value for gamma correction: \033[0m");
                    scanf("%f", &gamma);
                }
                to_gamma_correction(pixels, info_header.biWidth, info_header.biHeight, row_size, gamma);
                save_image(output_filename, file_header, info_header, pixels, row_size);
                memcpy(pixels, original_pixels, row_size * info_header.biHeight);
                break;
            case 4://median filter
                printf("\033[0;32m Enter the size of the median filter: \033[0m");
                int filter_size;
                scanf("%d", &filter_size);
                while (filter_size < 3 || filter_size > 19) {
                    printf("\033[0;33m Error: filter size must be between 3 and 19\n \033[0m");
                    printf("\033[0;32m Enter the size of the median filter: \033[0m");
                    scanf("%d", &filter_size);
                }
                to_median_filter(pixels, info_header.biWidth, info_header.biHeight, row_size, filter_size);
                save_image(output_filename, file_header, info_header, pixels, row_size);
                memcpy(pixels, original_pixels, row_size * info_header.biHeight);
                break;
            }
        }
    }



    fflush(stdin);
    getchar();
    return 0;
}
