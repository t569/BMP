#include "bitmap.h"



bitmap init_bitmap(int height, int width)
{
    bitmap mybitmap;
    mybitmap.height = height;
    mybitmap.width = width;
    mybitmap.pixels = malloc(width * height * sizeof(color_t));
    return mybitmap;
}

void free_bitmap(bitmap mybitmap)
{
    if(mybitmap.pixels == NULL)
        return;
    
    // the pixel data needs to be removed
    free(mybitmap.pixels);
}

void set_pixel(bitmap mybitmap, int x , int y, color_t color)
{
    if(x < 0 || y < 0 || x > mybitmap.width || y > mybitmap.height)
        return;
    
    mybitmap.pixels[y* mybitmap.width + x] = color;
}

void fill(bitmap mybitmap, int x1, int x2, int y1, int y2, color_t color)
{
    /*
    // fill starting from (x1, y1) to (x2, y2)
    
    int pos1 = y1*mybitmap.width + x1;
    // int pos2 = y2*mybitmap.width + x2;
    int diff = (y2*mybitmap.height + x2) - (y1*mybitmap.height + x1);

    for(int pos = 0; pos <= diff; pos++)
    {
        mybitmap.pixels[pos1 + pos] = color;
    }
    */
    if(x1 < 0 || y1 < 0 || x1 > mybitmap.width || y1 > mybitmap.height || x2 < 0 || y2 < 0 || x2 > mybitmap.width || y2 > mybitmap.height || y2 < y1 || x2 < x1)
    {
        printf("bad arguments");
        return;
    }
    int start = y1*mybitmap.width + x1;
    int diff = (y2*mybitmap.width + x2) - start;
    
    for(int i = 0; i <= diff; i++)
    {
        mybitmap.pixels[start + i] = color;
    }
}

void write_bitmap(bitmap mybitmap, const char* filename){
    FILE *file = fopen(filename, "wb");

    if(file == NULL)
        return;
    

    write_fileheader(mybitmap.width, mybitmap.height, file);
    write_infoheader(mybitmap.width, mybitmap.height, file);
    write_pixeldata(mybitmap,file);
    fclose(file);
}
int calcsize(int width, int height)
{
    int row_length = width * 3;
    row_length += 4 - (row_length % 4);
    return row_length * height;
}
void write_fileheader(int width, int height, FILE *file)
{
    // tell the file that it's a bitmap file lol
    int bm = 0x4d42;
    fwrite(&bm, 2, 1, file);
    /* making space for the data to be written */


    // calculate the row length for the data
    int row_length = width * 3;
    int offset = 14 + 40;
    int reserved = 0;

    row_length += 4 - (row_length % 4);
    int file_size = offset + calcsize(width, height);
    fwrite(&file_size, 4, 1, file);

    // write the reserved field
    fwrite(&reserved, 4, 1, file);

    // write offset bytes
    fwrite(&offset, 4, 1, file);
}
void write_infoheader(int width, int height, FILE* file)
{
    int info_size = 40;
    int planes = 1;
    int bitcount = 24;   // bits per pixel
    int compression = 0;
    int size_image = calcsize(width, height);
    int resolution_per_x = 0;
    int resolution_per_y = 0;
    int colour_used_index = 0;
    int colour_important_count = 0;



    // write all of the information
    fwrite(&info_size, 4, 1, file);
    fwrite(&width, 4, 1, file);
    fwrite(&height, 4, 1, file);
    fwrite(&planes, 2, 1, file);
    fwrite(&bitcount, 2, 1, file);
    fwrite(&compression, 4, 1, file);
    fwrite(&size_image, 4, 1, file);
    fwrite(&resolution_per_x, 4, 1, file);
    fwrite(&resolution_per_y, 4, 1, file);
    fwrite(&colour_used_index, 4, 1, file);
    fwrite(&colour_important_count, 4, 1, file);
}
void write_pixeldata(bitmap mybitmap, FILE* file)
{
    int row_padding = 4 - ((mybitmap.width * 3) % 4);
    // starting from the bottom of the image to the top
    for(int y = mybitmap.height - 1; y >= 0; y--)
    {
        for(int x = 0; x < mybitmap.height; x++)
        {
            color_t pixel = mybitmap.pixels[y*mybitmap.width + x];

            uint8_t red = (pixel & 0xFF0000) >> 16;
            uint8_t green = (pixel & 0x00FF00) >> 8;
            uint8_t blue = (pixel & 0x0000FF);

            // standard specifies blue green red
            fwrite(&blue, 1, 1, file);
            fwrite(&green, 1, 1, file);
            fwrite(&red, 1, 1, file);
        }

        // now write the padding
        for(int i = 0; i < row_padding; i++)
        {
          fputc(0,file);  
        }
    }  
}