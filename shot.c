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
		fprintf(stderr, "Error: Unable to establish connection to X server\n");
		return NULL;
	}
	/*Getting default screen*/
	screen_num = DefaultScreen(dpy);

	/*Doing a full-screen dump*/
	image = XGetImage (dpy, RootWindow(dpy, screen_num), 0, 0, DisplayWidth(dpy,screen_num),  DisplayHeight(dpy,screen_num), AllPlanes, ZPixmap);

	if (!image) {
		fprintf (stderr, "Error: Unable to get image from X server\n");
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
			fprintf(stderr, "Error: Unable to establish connection to X server\n");
			return NULL;
		}
		/*Getting default screen*/
		screen_num = DefaultScreen(dpy);
		/*Doing a dump*/
		image = XGetImage (dpy, RootWindow(dpy, screen_num), x_top_left, y_top_left, width,  height, AllPlanes, ZPixmap);
		if (!image) {
			fprintf (stderr, "Error: Unable to get image from X server\n");
			XCloseDisplay(dpy);
			return NULL;
			}
		/*Finishing*/
		XCloseDisplay(dpy);
		return image;
}
/*
 * Function for getting active window handler
 * Returns window handler or ERROR_HANDLER if fails*/
Window getActiveWindowHandler()
{
	Display  *display;
		Atom     actual_type;
		int      actual_format;
		long     nitems;
		long     bytes;
		unsigned long     *data;
		int      status;
		Window res=ERROR_HANDLER;
				/*Connecting to X server*/
		        display = XOpenDisplay(NULL);
		        if (!display) {
		                fprintf(stderr,"Error:Unable to connect to display\n");
		                return ERROR_HANDLER;
		        }
		        /*Fetching active window handler as a property of window manager*/
		        status = XGetWindowProperty(
		                display,
		                RootWindow(display, 0),
		                XInternAtom(display, "_NET_ACTIVE_WINDOW", True),
		                0,
		                (~0L),
		                False,
		                AnyPropertyType,
		                &actual_type,
		                &actual_format,
		                &nitems,
		                &bytes,
		                (unsigned char**)&data);

		        if (status != Success) {
		                fprintf(stderr, "Error: Getting active window was not successful\n");
		                XCloseDisplay(display);
		                return ERROR_HANDLER;
		        }
		        /*Finishing*/
		        if(nitems==1)res=data[0];
		        free(data);
		        XCloseDisplay(display);
		        return res;

}
/*Function gets handler for visible window with specified name
 * Parameters: full window name
 * Returns window handler or ERROR_HANDLER if fails*/
Window getWindowHandler(char* window_name){
	Display  *display;
	Atom     actual_type;
	int      actual_format;
	long     nitems;
	long     bytes;
	unsigned long     *data;
	int      status;
	int i;
	char*name;
	Window res=ERROR_HANDLER;
	/*Connecting to X server*/
	display = XOpenDisplay(NULL);
	if (!display) {
			fprintf(stderr,"Error: Unable to connect to display\n");
			return ERROR_HANDLER;
	}
	/*Fetching all windows as a property of window manager*/
	status = XGetWindowProperty(
			display,
			RootWindow(display, 0),
			XInternAtom(display, "_NET_CLIENT_LIST_STACKING", True),
			0,
			(~0L),
			False,
			AnyPropertyType,
			&actual_type,
			&actual_format,
			&nitems,
			&bytes,
			(unsigned char**)&data);
	if (status != Success) {
			fprintf(stderr, "Error: Getting visible windows was not successful\n");
			return ERROR_HANDLER;
	}
	/*Looking for window with the specified name*/
	for (i=0; i < nitems; i++){
		XFetchName(display,data[i],&name);
		if((name!=NULL)&&(strcmp(name,window_name)==0)){
			res=data[i];
		}
		free(name);
	}
	/*Finishing*/
	free(data);
	XCloseDisplay(display);
	return res;

}
/*
 * Function fetches names of all windows on the screen
 * Parameters: all fetched window names(return parameter)
 * 				number of fetched window names
 * Returns 0 if ok, -1 if fails
 * */
int getWindowsNames(char*** names, int*size){
	Display  *display;
		Atom     actual_type;
		int      actual_format;
		long     nitems;
		long     bytes;
		unsigned long     *data;
		int      status;
		int i;
		char*name;
		char** tmp;
		int counter;
		/*Connecting to X server*/
	display = XOpenDisplay(NULL);
	if (!display) {
			fprintf(stderr,"Error:Unable to connect to X server\n");
			return -1;
	}
	/*Fetching all windows as the property of window manager*/
	status = XGetWindowProperty(
			display,
			RootWindow(display, 0),
			XInternAtom(display, "_NET_CLIENT_LIST_STACKING", True),
			0,
			(~0L),
			False,
			AnyPropertyType,
			&actual_type,
			&actual_format,
			&nitems,
			&bytes,
			(unsigned char**)&data);
	if (status != Success) {
			fprintf(stderr, "Error: Getting visible windows was not successful\n");
			return -1;
	}
	/*Getting window names*/
	tmp=(char**)malloc(nitems*sizeof(char*));
	counter=0;
	for (i=0; i < nitems; i++){
		XFetchName(display,data[i],&name);
		if(name!=0){
			tmp[counter]=name;
			counter+=1;
		}
	}
	free(data);
	/*Filling return parameters*/
	*size=counter;
	if(counter==0){
		fprintf(stderr,"Warning: no windows with names found\n");
		*names=NULL;
		free(tmp);
		XCloseDisplay(display);
		return -1;
	}
	*names=(char**)malloc(counter*sizeof(char*));
	for(i=0;i<counter;i++){
		(*names)[i]=tmp[i];
	}
	/*Finishing*/
	free(tmp);
	XCloseDisplay(display);
	return 0;
}
/*
 * Checks if the window is visible on the screen
 * Returns 1 as true, 0 as false, -1 if failed*/
int isVisible(Window w){
	Display  *display;
	Atom hidden;
	Atom     actual_type;
	int      actual_format;
	long     nitems;
	long     bytes;
	int      status;
	int 	i;

	Atom* props;

	/*Connecting to X server*/
	display = XOpenDisplay(NULL);
	if (!display) {
			fprintf(stderr,"Error:Unable to connect to X server\n");
			return -1;
	}
	/*Creating identifier for hidden property*/
	hidden = XInternAtom(display, "_NET_WM_STATE_HIDDEN", True);
	/*Fetching window state properties*/
	status = XGetWindowProperty(
			display,
			w,
			XInternAtom(display, "_NET_WM_STATE", True),
			0,
			(~0L),
			False,
			AnyPropertyType,
			&actual_type,
			&actual_format,
			&nitems,
			&bytes,
			(unsigned char**)&props);
	if (status != Success) {
			fprintf(stderr, "Error: Getting window state was not successful\n");
			return -1;
	}
	/*Searching hidden property among properties*/
	for(i=0;i<nitems;i++){
		if (props[i]==hidden){
			/*Window is not visible*/
			XFree(props);
			XCloseDisplay(display);
			return 0;
		}

	}
	/*Finishing*/
	XFree(props);
	XCloseDisplay(display);
	return 1;
}
/*
 * Function for window capturing
 * Parameters: a window handler for window to capture
 * Returns a ponter to created dump(XImage struct)*/
XImage* windowCapture(Window handler){
			Display *dpy;
			XImage *image;
			XWindowAttributes attr;
			int visibility=isVisible(handler);
			if(visibility==0){
				fprintf(stderr,"Error: Window %d is not visible\n",(int)handler);
				return NULL;
			}
			if(visibility==-1){
				fprintf(stderr,"Error: Failed fetching window visibility for %d\n", (int)handler);
				return NULL;
			}
			/*Connecting to X server*/
			dpy = XOpenDisplay(NULL);
			if (!dpy)
			{
                fprintf(stderr,"Error: Unable to connect to X server\n");
				return NULL;
			}
			/*Getting window attributes*/
			XGetWindowAttributes(dpy,handler,&attr);

			/*Doing a dump*/
			image = XGetImage (dpy, handler, 0, 0, attr.width,  attr.height, AllPlanes, ZPixmap);
			if (!image) {
				fprintf (stderr, "Error: Unable to get image from X server\n");
				XCloseDisplay(dpy);
				return NULL;
				}
			/*Finishing*/
			XCloseDisplay(dpy);
			return image;
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
