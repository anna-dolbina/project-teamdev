/*
 * shot.c
 *
 *  Created on: 22.03.2012
 *      Author: anna
 */
#include "shot.h"

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

