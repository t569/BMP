#ifndef BITMAP_H
#define BITMAP_H
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
/* NOTE: most of these functions work on a little endian machine. For a proper library we treat big endian cases appropritely */
typedef uint32_t color_t;

typedef struct{
    int width;
    int height;
    color_t* pixels;
}bitmap;

// create and free the bitmap 
bitmap init_bitmap(int height, int width);
void free_bitmap(bitmap bitmap_to_free);


// drawing on the bitmap
void set_pixel(bitmap bitmap, int x, int y, color_t color);
void fill(bitmap bitmap, int x1, int x2, int y1, int y2, color_t color);


// writing to file

/* this is the predominant function */
void write_bitmap(bitmap bitmap, const char* filename);


// helper functions
void write_fileheader(int width, int height, FILE* file);
void write_infoheader(int width, int height, FILE* file);
void write_pixeldata(bitmap mybitmap, FILE* file);


// utils
int calcsize(int width, int height);
#endif