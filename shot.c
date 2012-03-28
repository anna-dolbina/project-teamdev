/*
 * shot.c
 *
 *  Created on: 22.03.2012
 *      Author: anna
 */
#include "shot.h"

/*Function writes XImage structure contents as a BMP image to the specified file*/
void saveXImageToBitmap(XImage *pImage, char* full_filename)
{
	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	FILE *fp;
	int dummy,i,j;
	unsigned long pix;
	void*data_ptr=NULL;
	int row_length;

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

	/*Writing image data*/
	row_length=4*pImage->width;
	data_ptr=pImage->data+row_length*((pImage->height)-1);
	while (data_ptr!=pImage->data)
	{
		fwrite(data_ptr,row_length,1,fp);
		data_ptr-=row_length;
	}
	fwrite(data_ptr,row_length,1,fp);

	/*Closing*/
	fclose(fp);
}

/*Function returns a pointer to a full-screen dump (XImage structure)*/
XImage* fullScreenCapture()
{
	int screen_num;
	Display *dpy;
	XImage *image;
	/*Connecting to X server*/
	dpy = XOpenDisplay(NULL);
	if (!dpy)
	{
		fprintf(stderr, "unable to connect to display");
		return NULL;
	}
	/*Getting default screen*/
	screen_num = DefaultScreen(dpy);

	/*Doing a full-screen dump*/
	image = XGetImage (dpy, RootWindow(dpy, screen_num), 0, 0, DisplayWidth(dpy,screen_num),  DisplayHeight(dpy,screen_num), AllPlanes, ZPixmap);

	if (!image) {
		fprintf (stderr, "unable to get image ");
		XCloseDisplay(dpy);
		return NULL;
		}
	/*Finishing*/
	XCloseDisplay(dpy);
	return image;
}
/*Function creates a dump of the specified rectangle and returns a pointer to XImage struct
 * Parameters: x and y coordinates of the top left corner
 * 				width and height of the rectangle*/
XImage* rectangleCapture(int x_top_left, int y_top_left, int width, int height){
	int screen_num;
		Display *dpy;
		XImage *image;

		/*Connecting to X server*/
		dpy = XOpenDisplay(NULL);
		if (!dpy)
		{
			fprintf(stderr, "unable to connect to display");
			return NULL;
		}
		/*Getting default screen*/
		screen_num = DefaultScreen(dpy);
		/*Doing a dump*/
		image = XGetImage (dpy, RootWindow(dpy, screen_num), x_top_left, y_top_left, width,  height, AllPlanes, ZPixmap);
		if (!image) {
			fprintf (stderr, "unable to get image ");
			XCloseDisplay(dpy);
			return NULL;
			}
		/*Finishing*/
		XCloseDisplay(dpy);
		return image;
}

