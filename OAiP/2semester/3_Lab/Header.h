#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

typedef struct {
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
} BITMAPFILEHEADER;
typedef struct {
    unsigned int biSize;
    int biWidth;
    int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
} BITMAPINFOHEADER;
typedef struct {
    unsigned char rgbtBlue;
    unsigned char rgbtGreen;
    unsigned char rgbtRed;
} RGBTRIPLE;

void input_bmp_filename(char* filename, int max_length);
bool is_bmp_file(const char* filename);
int is_bmp_24bit(const char* filename);
void print_menu();
int get_choice(int kolichestvo);
void save_image(const char* file_name, BITMAPFILEHEADER file_header, BITMAPINFOHEADER info_header, RGBTRIPLE* pixels, int row_size);
void to_negative(RGBTRIPLE* pixels, int width, int height, int row_size);
void to_black_and_white(RGBTRIPLE* pixels, int width, int height, int row_size);
void black_and_white(RGBTRIPLE* pixel);
void negative(RGBTRIPLE* pixel);
void to_gamma_correction(RGBTRIPLE* pixels, int width, int height, int row_size, float gamma);
void to_median_filter(RGBTRIPLE* pixels, int width, int height, int row_size, int filter_size);
int compare(const void* a, const void* b);
void read_file(char* filename, BITMAPFILEHEADER* file_header, BITMAPINFOHEADER* info_header, RGBTRIPLE* pixels, RGBTRIPLE* original_pixels, int row_size);