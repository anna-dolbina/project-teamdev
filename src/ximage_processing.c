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
	int x_low,x_high,x_lower,x_higher,y_low,y_high,y_lower,y_higher,j;
	int r_comp,g_comp,b_comp;
	int points[4][4],p[4];
	double rcoef[4],gcoef[4],bcoef[4];
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

		x_lower=(x_low>0)?x_low-1:0;
		y_lower=(y_low>0)?y_low-1:0;
		x_higher=(x_high==src->width)?(src->width):x_high+1;
		y_higher=(y_high==src->height)?(src->height):y_high+1;


		/*Calculating coefficients*/
		x_coef=fmod(hratio,x_low);
		y_coef=fmod(vratio,y_low);
		/*Loading points*/
		points[0][0]=XGetPixel(src,x_lower,y_lower);
		points[0][1]=XGetPixel(src,x_lower,y_low);
		points[0][2]=XGetPixel(src,x_lower,y_high);
		points[0][3]=XGetPixel(src,x_lower,y_higher);

		points[1][0]=XGetPixel(src,x_low,y_lower);
		points[1][1]=XGetPixel(src,x_low,y_low);
		points[1][2]=XGetPixel(src,x_low,y_high);
		points[1][3]=XGetPixel(src,x_low,y_higher);

		points[2][0]=XGetPixel(src,x_high,y_lower);
		points[2][1]=XGetPixel(src,x_high,y_low);
		points[2][2]=XGetPixel(src,x_high,y_high);
		points[2][3]=XGetPixel(src,x_high,y_higher);

		points[3][0]=XGetPixel(src,x_higher,y_lower);
		points[3][1]=XGetPixel(src,x_higher,y_low);
		points[3][2]=XGetPixel(src,x_higher,y_high);
		points[3][3]=XGetPixel(src,x_higher,y_higher);
		/*Filling coefficient array*/
		for(j=0;j<4;j++)
		{
			p[0]=MASK(points[0][j],src->red_mask,16);
			p[1]=MASK(points[1][j],src->red_mask,16);
			p[2]=MASK(points[2][j],src->red_mask,16);
			p[3]=MASK(points[3][j],src->red_mask,16);

			rcoef[j]=p[1] + 0.5 * x_coef*
					(p[2] - p[0] + x_coef*
					(2.0*p[0] - 5.0*p[1] + 4.0*p[2] - p[3] + x_coef*
					(3.0*(p[1] - p[2]) + p[3] - p[0])));


			p[0]=MASK(points[0][j],src->green_mask,8);
			p[1]=MASK(points[1][j],src->green_mask,8);
			p[2]=MASK(points[2][j],src->green_mask,8);
			p[3]=MASK(points[3][j],src->green_mask,8);

			gcoef[j]=p[1] + 0.5 * x_coef*
					(p[2] - p[0] + x_coef*
					(2.0*p[0] - 5.0*p[1] + 4.0*p[2] - p[3] + x_coef*
					(3.0*(p[1] - p[2]) + p[3] - p[0])));


			p[0]=MASK(points[0][j],src->blue_mask,0);
			p[1]=MASK(points[1][j],src->blue_mask,0);
			p[2]=MASK(points[2][j],src->blue_mask,0);
			p[3]=MASK(points[3][j],src->blue_mask,0);

			bcoef[j]=p[1] + 0.5 * x_coef*
					(p[2] - p[0] + x_coef*
					(2.0*p[0] - 5.0*p[1] + 4.0*p[2] - p[3] + x_coef*
					(3.0*(p[1] - p[2]) + p[3] - p[0])));

		}
		/*Calculating color components*/
		r_comp= rcoef[1] + 0.5 * y_coef*(
				rcoef[2] - rcoef[0] + y_coef*(
				2.0*rcoef[0] - 5.0*rcoef[1] + 4.0*rcoef[2] - rcoef[3] + y_coef*(
				3.0*(rcoef[1] - rcoef[2]) + rcoef[3] - rcoef[0])));
		r_comp=(r_comp<0)?0:r_comp;
		r_comp=(r_comp>255)?255:r_comp;
		g_comp= gcoef[1] + 0.5 * y_coef*(
				gcoef[2] - gcoef[0] + y_coef*(
				2.0*gcoef[0] - 5.0*gcoef[1] + 4.0*gcoef[2] - gcoef[3] + y_coef*(
				3.0*(gcoef[1] - gcoef[2]) + gcoef[3] - gcoef[0])));
		g_comp=(g_comp<0)?0:g_comp;
		g_comp=(g_comp>255)?255:g_comp;

		b_comp= bcoef[1] + 0.5 * y_coef*(
				bcoef[2] - bcoef[0] + y_coef*(
				2.0*bcoef[0] - 5.0*bcoef[1] + 4.0*bcoef[2] - bcoef[3] + y_coef*(
				3.0*(bcoef[1] - bcoef[2]) + bcoef[3] - bcoef[0])));
		b_comp=(b_comp<0)?0:b_comp;
		b_comp=(b_comp>255)?255:b_comp;
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
