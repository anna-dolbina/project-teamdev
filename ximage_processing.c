/*
 * ximage_processing.c
 *
 *  Created on: 04.05.2012
 *      Author: anna
 */
#include "ximage_processing.h"
/*Function writes XImage structure contents as a BMP image to the specified file*/
void saveXImageToBitmap(XImage *pImage, char* full_filename)
{
	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	FILE *fp;
	int dummy;
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
XImage* resizeBilinear(XImage* src,int new_width, int new_height){
	XImage* res=NULL;
	char* data;
	unsigned long size,i,row_counter,column_counter;
	int row_length=new_width*4;
	double hratio,vratio,x_coef,y_coef;
	int x_low,x_high,y_low,y_high;
	unsigned long r_comp,g_comp,b_comp;
	/* Allocating memory for new image */
	size = new_width * new_height * 4;
	data = (char*)malloc(size);


	/* Filling with zeros */
	memset(data, 0, size);
	/*Filling with data*/
	for(i=0;i<size;i+=4)
	{
		/*Calculating position in image*/
		row_counter=i/row_length;
		column_counter=(i%row_length)/4;
		/*Calculating ratio*/
		hratio=(((double)column_counter)/new_width)*src->width;
		vratio=(((double)row_counter)/new_height)*src->height;
		/*Calculating indexes*/
		x_low=(int)floor(hratio);
		x_high=(int)ceil(hratio);

		y_low=(int)floor(vratio);
		y_high=(int)ceil(vratio);

		x_low=(x_low>=src->width)?(src->width)-1:x_low;
		y_low=(y_low>=src->height)?(src->height)-1:y_low;

		x_high=(x_high>=src->width)?(src->width)-1:x_high;
		y_high=(y_high>=src->height)?(src->height)-1:y_high;


		/*Calculating coefficients*/
		x_coef=fmod(hratio,x_low);
		y_coef=fmod(vratio,y_low);
		/*Calculating color components*/
		r_comp= ((XGetPixel(src,x_low,y_low)&src->red_mask)>>16)*(1-x_coef)*(1-y_coef)+
				((XGetPixel(src,x_high,y_low)&src->red_mask)>>16)*x_coef*(1-y_coef)+
				((XGetPixel(src,x_low,y_high)&src->red_mask)>>16)*(1-x_coef)*y_coef+
				((XGetPixel(src,x_high,y_high)&src->red_mask)>>16)*x_coef*y_coef;

		g_comp= ((XGetPixel(src,x_low,y_low)&src->green_mask)>>8)*(1-x_coef)*(1-y_coef)+
				((XGetPixel(src,x_high,y_low)&src->green_mask)>>8)*x_coef*(1-y_coef)+
				((XGetPixel(src,x_low,y_high)&src->green_mask)>>8)*(1-x_coef)*y_coef+
				((XGetPixel(src,x_high,y_high)&src->green_mask)>>8)*x_coef*y_coef;

		b_comp= ((XGetPixel(src,x_low,y_low)&src->blue_mask)>>0)*(1-x_coef)*(1-y_coef)+
				((XGetPixel(src,x_high,y_low)&src->blue_mask)>>0)*x_coef*(1-y_coef)+
				((XGetPixel(src,x_low,y_high)&src->blue_mask)>>0)*(1-x_coef)*y_coef+
				((XGetPixel(src,x_high,y_high)&src->blue_mask)>>0)*x_coef*y_coef;
		/*Setting color*/
		data[i]=(char)b_comp;
		data[i+1]=(char)g_comp;
		data[i+2]=(char)r_comp;
		data[i+3]=(char)0;
	}
	/* Creating correspondent XImage */

	res=(XImage*)malloc(sizeof(XImage));
	memset(res,0,sizeof(XImage));
	res->width=new_width;
	res->height=new_height;           /* size of image */
	res->xoffset=src->xoffset;                 /* number of pixels offset in X direction */
	res->format=ZPixmap;                  /* XYBitmap, XYPixmap, ZPixmap */
	res->data=data;                  /* pointer to image data */
	res->byte_order=src->byte_order;              /* data byte order, LSBFirst, MSBFirst */
	res->bitmap_unit=32;             /* quant. of scanline 8, 16, 32 */
	res->bitmap_bit_order=MSBFirst;
	res->bitmap_pad=8;              /* 8, 16, 32 either XY or ZPixmap */
	res->depth=24;                   /* depth of image */
	res->bytes_per_line=row_length;          /* accelerator to next scanline */
	res->bits_per_pixel=32;          /* bits per pixel (ZPixmap) */
	res->red_mask=src->red_mask;      /* bits in z arrangement */
	res->green_mask=src->green_mask;
	res->blue_mask=src->blue_mask;
	XInitImage(res);

	return res;
}
XImage* resizeBicubic(XImage* src,int new_width, int new_height){
	XImage* res=NULL;
	char* data;
	unsigned long size,i,row_counter,column_counter;
	int row_length=new_width*4;
	double hratio,vratio,x_coef,y_coef;
	int x_low,x_high,x_lower,x_higher,y_low,y_high,y_lower,y_higher;
	unsigned long r_comp,g_comp,b_comp;
	/* Allocating memory for new image */
	size = new_width * new_height * 4;
	data = (char*)malloc(size);


	/* Filling with zeros */
	memset(data, 0, size);
	/*Filling with data*/
	for(i=0;i<size;i+=4)
	{
		/*Calculating position in image*/
		row_counter=i/row_length;
		column_counter=(i%row_length)/4;
		/*Calculating ratio*/
		hratio=(((double)column_counter)/new_width)*src->width;
		vratio=(((double)row_counter)/new_height)*src->height;
		/*Calculating indexes*/
		x_low=(int)floor(hratio);
		x_high=(int)ceil(hratio);
		x_lower=x_low-1;
		x_higher=x_high+1;


		y_low=(int)floor(vratio);
		y_high=(int)ceil(vratio);
		y_lower=y_low-1;
		y_higher=y_high+1;

		x_low=(x_low>=src->width)?(src->width)-1:x_low;
		y_low=(y_low>=src->height)?(src->height)-1:y_low;
		x_lower=(x_lower>=src->width)?(src->width)-1:x_lower;
		y_lower=(y_lower>=src->height)?(src->height)-1:y_lower;
		x_lower=(x_lower<0)?0:x_lower;
		y_lower=(y_lower<0)?0:y_lower;

		x_high=(x_high>=src->width)?(src->width)-1:x_high;
		y_high=(y_high>=src->height)?(src->height)-1:y_high;
		x_higher=(x_higher>=src->width)?(src->width)-1:x_higher;
		y_higher=(y_higher>=src->height)?(src->height)-1:y_higher;

		/*Calculating coefficients*/
		x_coef=fmod(hratio,x_low);
		y_coef=fmod(vratio,y_low);
		/*Calculating color components*/
		r_comp= ((XGetPixel(src,x_low,y_low)&src->red_mask)>>16)*(1-x_coef)*(1-y_coef)+
				((XGetPixel(src,x_high,y_low)&src->red_mask)>>16)*x_coef*(1-y_coef)+
				((XGetPixel(src,x_low,y_high)&src->red_mask)>>16)*(1-x_coef)*y_coef+
				((XGetPixel(src,x_high,y_high)&src->red_mask)>>16)*x_coef*y_coef;

		g_comp= ((XGetPixel(src,x_low,y_low)&src->green_mask)>>8)*(1-x_coef)*(1-y_coef)+
				((XGetPixel(src,x_high,y_low)&src->green_mask)>>8)*x_coef*(1-y_coef)+
				((XGetPixel(src,x_low,y_high)&src->green_mask)>>8)*(1-x_coef)*y_coef+
				((XGetPixel(src,x_high,y_high)&src->green_mask)>>8)*x_coef*y_coef;

		b_comp= ((XGetPixel(src,x_low,y_low)&src->blue_mask)>>0)*(1-x_coef)*(1-y_coef)+
				((XGetPixel(src,x_high,y_low)&src->blue_mask)>>0)*x_coef*(1-y_coef)+
				((XGetPixel(src,x_low,y_high)&src->blue_mask)>>0)*(1-x_coef)*y_coef+
				((XGetPixel(src,x_high,y_high)&src->blue_mask)>>0)*x_coef*y_coef;
		/*Setting color*/
		data[i]=(char)b_comp;
		data[i+1]=(char)g_comp;
		data[i+2]=(char)r_comp;
		data[i+3]=(char)0;
	}
	/* Creating correspondent XImage */

	res=(XImage*)malloc(sizeof(XImage));
	memset(res,0,sizeof(XImage));
	res->width=new_width;
	res->height=new_height;           /* size of image */
	res->xoffset=src->xoffset;                 /* number of pixels offset in X direction */
	res->format=ZPixmap;                  /* XYBitmap, XYPixmap, ZPixmap */
	res->data=data;                  /* pointer to image data */
	res->byte_order=src->byte_order;              /* data byte order, LSBFirst, MSBFirst */
	res->bitmap_unit=32;             /* quant. of scanline 8, 16, 32 */
	res->bitmap_bit_order=MSBFirst;
	res->bitmap_pad=8;              /* 8, 16, 32 either XY or ZPixmap */
	res->depth=24;                   /* depth of image */
	res->bytes_per_line=row_length;          /* accelerator to next scanline */
	res->bits_per_pixel=32;          /* bits per pixel (ZPixmap) */
	res->red_mask=src->red_mask;      /* bits in z arrangement */
	res->green_mask=src->green_mask;
	res->blue_mask=src->blue_mask;
	XInitImage(res);

	return res;
}
