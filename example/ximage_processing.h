#ifndef XIMAGE_PROCESSING_H_
#define	XIMAGE_PROCESSING_H_

#ifndef X11_HEADERS
	#define X11_HEADERS

	#include <X11/Xlib.h>
	#include <X11/Xutil.h>
	#include <X11/Xresource.h>
#endif /*X11_HEADERS*/


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#pragma pack (1)

#ifndef MASK
#define MASK(value,mask,roffset) ((value&mask)>>roffset)
#endif /*MASK*/
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
XImage* resizeBilinear(XImage* src,int new_width, int new_height);
XImage* resizeBicubic(XImage* src,int new_width, int new_height);

#endif /*XIMAGE_PROCESSING_H_*/
