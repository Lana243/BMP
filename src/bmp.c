#include "bmp.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void* my_allocator(int size) {
	void *ans = malloc(size);
	if (ans == NULL)
		exit(-1);
	return ans;
}

static Pixel** alloc_matrix(int h, int w) {
	Pixel **data = my_allocator(h * sizeof(Pixel *));
	for (int i = 0; i < h; i++) 
		data[i] = my_allocator(w * sizeof(Pixel));
	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++) {
			data[i][j].R = 0;
			data[i][j].G = 0;
			data[i][j].B = 0;
		}
	return data;
}

void clean_matrix(Pixel **data, int h) {
	for (int i = 0; i < h; i++)
		free(data[i]);
	free(data);
}

static int make_div4(int x) {
	return (4 - x % 4) % 4;
}

static Image update_header(Image a, int w, int h, Pixel **new_data) {
	clean_matrix(a.data, a.info.biHeight);
	a.info.biWidth = w;
	a.info.biHeight = h;
	a.info.biSizeImage = (w * sizeof(Pixel) + make_div4(w * sizeof(Pixel))) * h;
	a.head.bfSize = a.head.bfOffBits + a.info.biSizeImage;
	a.data = new_data;	
	return a;
}

static void read(void *a, int size, FILE *fin) {
	int cur = fread(a, size, 1, fin);
	if (cur != 1)
		exit(-1);
}

static void write(void *a, int size, FILE *fout) {
	int cur = fwrite(a, size, 1, fout);
	if (cur != 1)
		exit(-1);
}

static FILE * open_file(char *name, char *rule) {
	FILE *f = fopen(name, rule);
	if (f == NULL)
		exit(-1);
	return f;
}

static void reverse_matrix(Pixel **data, int h, int w) {
	int sz = w * sizeof(Pixel);
	Pixel *tmp = my_allocator(sz);
	for (int i = 0; i < h / 2; i++) {
		memcpy(tmp, data[i], sz);
		memcpy(data[i], data[h - i - 1], sz);
		memcpy(data[h - i - 1], tmp, sz);
	}
	free(tmp);
}

Image load_bmp(char *file_name) {
	FILE *fin = open_file(file_name, "rb");
	Image ans;
	read(&ans.head, sizeof(ans.head), fin);
	read(&ans.info, sizeof(ans.info), fin);
	fseek(fin, ans.head.bfOffBits, SEEK_SET);
	ans.data = alloc_matrix(ans.info.biHeight, ans.info.biWidth);
	for (int i = 0; i < ans.info.biHeight; i++) {
		read(ans.data[i], ans.info.biWidth * sizeof(Pixel), fin);
		fseek(fin, make_div4(ans.info.biWidth * sizeof(Pixel)), SEEK_CUR);
	}
	reverse_matrix(ans.data, ans.info.biHeight, ans.info.biWidth);
	fclose(fin);
	return ans;
}

void save_bmp(char *file_name, Image a) {
	FILE *fout = open_file(file_name, "wb");
	a.head.bfOffBits = sizeof(a.head) + sizeof(a.info);
	write(&a.head, sizeof(a.head), fout);
	write(&a.info, sizeof(a.info), fout);
	reverse_matrix(a.data, a.info.biHeight, a.info.biWidth);
	for (int i = 0; i < a.info.biHeight; i++) {
		write(a.data[i], a.info.biWidth * sizeof(Pixel), fout);
		unsigned char c = 0;
		for (int j = 0; j < make_div4(a.info.biWidth * sizeof(Pixel)); j++)
			write(&c, sizeof(char), fout);
	}
	fclose(fout);
}

Image crop(Image a, int x, int y, int w, int h) {
	Pixel **new_data;
	new_data = alloc_matrix(h, w);
	if (x + w > a.info.biWidth || y + h > a.info.biHeight)
		exit(-1);
	for (int i = y; i < y + h; i++)
		for (int j = x; j < x + w; j++)
			new_data[i - y][j - x] = a.data[i][j];
	return update_header(a, w, h, new_data);
}

Image rotate(Image a) {
	int w = a.info.biHeight;
	int h = a.info.biWidth;
	Pixel **new_data = alloc_matrix(h, w);
	for (int i = 0; i < a.info.biHeight; i++)
		for (int j = 0; j < a.info.biWidth; j++)
			new_data[j][a.info.biHeight - i - 1] = a.data[i][j];
	return update_header(a, w, h, new_data);
}

