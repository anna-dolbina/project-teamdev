/*
 * shot.h
 *
 *  Created on: 22.03.2012
 *      Author: anna
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#pragma pack (1)

typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef long LONG;

/*Standard BMP headers*/
typedef struct tagBITMAPFILEHEADER {
WORD    bfType;
DWORD   bfSize;
WORD    bfReserved1;
WORD    bfReserved2;
DWORD   bfOffBits;

} BITMAPFILEHEADER;
typedef struct tagBITMAPINFOHEADER{
DWORD  biSize;
LONG   biWidth;
LONG   biHeight;
WORD   biPlanes;
WORD   biBitCount;
DWORD  biCompression;
DWORD  biSizeImage;
LONG   biXPelsPerMeter;
LONG   biYPelsPerMeter;
DWORD  biClrUsed;
DWORD  biClrImportant;

} BITMAPINFOHEADER;
/*Functions*/
void saveXImageToBitmap(XImage *pImage, char* full_filename);
XImage* fullScreenCapture();
XImage* rectangleCapture(int x_top_left, int y_top_left, int width, int height);
Window getActiveWindowHandler();
Window getWindowHandler(char* window_name);
int getWindowsNames(char*** names, int*size);
int isVisible(Window w);
XImage* windowCapture(Window handler);

