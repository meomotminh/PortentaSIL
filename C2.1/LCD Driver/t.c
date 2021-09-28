/********** t.c file of program C2.6 *************/
#include "define.h"     // device base address, etc
#include "vid.c"        // LCD driver
#include "uart.c"       // UART driver
extern char _binary_image1_start, _binary_image2_start;

#define WIDTH 640
int show_bmp(char *p, int start_row, int start_col)
{
    int h, w, pixel, rsize, i, j;
    unsigned char r,g,b;
    char *pp;
    int *q = (int *)(p + 14); // skip over 14-byte file header
    w = *(q + 1);             // image width in pixels
    h = *(q + 2);             // image height in pixels
    p += 54;                  // p-> pixels in image
    // BMP images are upside down, each row is a multiple of 4 bytes
    rsize = 4*((3*w + 3)/4);  // multiple of 4
    p += (h - 1)*rsize;       // last row of pixels
    for (i= start_row; i<start_row+h; i++){
        pp = p;
        for (j=start_col; j<start_col + w; j++){
            b = *pp; g = *(pp+1); r = *(pp+2);  // RGB values
            pixel = (b<<16) | (g<<8) | r;       // pixel values
            fb[i*WIDTH + j] = pixel;            // write to frame buffer
            pp += 3;
        }
        p -= rsize;             // to preceding row
    }
    uprintf("\nBMP image height=%d width=%d\n",h,w);
}

int main()
{
    char c,*p;
    uart_init();        // initialize UARTs
    up = upp[0];
    fbuf_init();
    while (1){
        p = &_binary_image1_start;
        show_bmp(p, 0, 80); // display image1
        uprintf("enter a key from this UART: ");
        ugetc(up);
        p = &_binary_image2_start;
        show_bmp(p, 120, 0); // display image2
    }
    while (1);              // loop here

}