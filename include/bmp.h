#ifndef _BMP_H
#define _BMP_H

#include <stdint.h>

typedef struct BITMAPFILEHEADER {
	int16_t bfType;
	uint32_t bfSize;
	uint32_t bfReserved;
	uint32_t bfOffBits;
} __attribute__((packed)) BITMAPFILEHEADER;

typedef struct BITMAPINFO {
 	uint32_t biSize;
	int32_t biWidth;
	int32_t biHeight;
	int16_t biPlanes;
	int16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	int32_t biXPelsPerMeter;
	int32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
} __attribute__((packed)) BITMAPINFO;
 
typedef struct Pixel {
	unsigned char B, G, R;
} Pixel;

typedef struct Image {
	BITMAPFILEHEADER head;
	BITMAPINFO info;
	Pixel **data;	
} Image;

Image load_bmp(char *file_name);
void save_bmp(char *file_name, Image a);
Image crop(Image a, int x, int y, int w, int h);
Image rotate(Image a);
void clean_matrix(Pixel **data, int h);

#endif
