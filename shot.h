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
int fullScreenCapture(char*full_filename);



/*Function writes XImage structure contents as a BMP image to the specified file*/
void saveXImageToBitmap(XImage *pImage, char* full_filename)
{
	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	FILE *fp;
	int dummy,i,j;
	unsigned long pix;

	/*Setting all meanings to null*/
	memset(&bmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
	memset(&bmpInfoHeader, 0, sizeof(BITMAPINFOHEADER));

	/*Initializing header structures*/
	bmpFileHeader.bfType = 0x4D42;
	bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +  pImage->width*pImage->height*4;
	bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmpFileHeader.bfReserved1 = 0;
	bmpFileHeader.bfReserved2 = 0;

	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoHeader.biWidth = pImage->width;
	bmpInfoHeader.biHeight = pImage->height;
	bmpInfoHeader.biPlanes = 1;
	bmpInfoHeader.biBitCount = 32;
	dummy = (pImage->width * 3) % 4;
	if((4-dummy)==4)
		dummy=0;
	else
		dummy=4-dummy;
	bmpInfoHeader.biSizeImage = ((pImage->width*3)+dummy)*pImage->height;
	bmpInfoHeader.biCompression = 0;
	bmpInfoHeader.biXPelsPerMeter = 0;
	bmpInfoHeader.biYPelsPerMeter = 0;
	bmpInfoHeader.biClrUsed = 0;
	bmpInfoHeader.biClrImportant = 0;

	/*Opening file*/
	fp = fopen(full_filename,"wb");

	if(fp == NULL)
		return;

	/*Writing info*/
	fwrite(&bmpFileHeader, sizeof(bmpFileHeader), 1, fp);
	fwrite(&bmpInfoHeader, sizeof(bmpInfoHeader), 1, fp);
	for(j=pImage->height-1;j>=0;j--){
		for(i=0;i<pImage->width;i++){
			pix=XGetPixel(pImage,i,j);
			fwrite(&pix,sizeof(unsigned long),1,fp);
		}
	}

	/*Closing*/
	fclose(fp);
}


int fullScreenCapture(char*full_filename)
{
	int screen_num;
	Display *dpy;
	XImage *image;
	/*Connecting to X server*/
	dpy = XOpenDisplay(NULL);
	if (!dpy)
	{
		fprintf(stderr, "unable to connect to display");
		return 1;
	}
	/*Getting default screen*/
	screen_num = DefaultScreen(dpy);

	/*Doing a full-screen dump*/
	image = XGetImage (dpy, RootWindow(dpy, screen_num), 0, 0, DisplayWidth(dpy,screen_num),  DisplayHeight(dpy,screen_num), AllPlanes, ZPixmap);

	if (!image) {
		fprintf (stderr, "unable to get image ");
		XCloseDisplay(dpy);
		return 2;
		}
	/*Writing dump in BMP format to the specified file*/
	saveXImageToBitmap(image,full_filename);

	/*Finishing*/
	free(image);
	XCloseDisplay(dpy);
	return 0;
}
