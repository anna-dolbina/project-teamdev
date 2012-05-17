/*
 * main.c
 *
 *  This file is an example of using libshot.so
 *
 *  Here you can see and test several examples of usage
 *  Please read help.txt for detailed explanation
 *
 *  Created on: 22.03.2012
 *      Author: anna
 */

#include "shot.h"
#include "ximage_processing.h"
#include "windows.h"


int main(int argc, char ** argv){
	XImage* img,*img1;
	Window w;
	char** windows_names;
	int size;
	int i;
	char filename[20];
	

	/*This part contains an example of capturing and resizing a full-screen image*/
	img=fullScreenCapture();
	if(img!=NULL)
	{
		saveXImageToBitmap(img,"fullscreen1.bmp");
		img1=resizeBilinear(img,2048,1536);
		if(img1!=NULL)
		{
			saveXImageToBitmap(img1,"fullscreen1_resized.bmp");
			XDestroyImage(img1);
		}
		else fprintf(stderr,"No resized image\n");

		XDestroyImage(img);
	}

	/*Here the rectangular part is captured, resized and stored*/

	img=rectangleCapture(15,50,320,240);
	if(img!=NULL)
	{
		saveXImageToBitmap(img,"rectanglecapture_original.bmp");
		img1=resizeBicubic(img,800,600);
		if(img1!=NULL)
		{
			saveXImageToBitmap(img1,"rectanglecapture_800x600.bmp");
			XDestroyImage(img1);
		}
		else fprintf(stderr,"No resized image\n");
		XDestroyImage(img);
	}

	/*Here is the example of capturing particular windows*/

	getWindowsNames(&windows_names,&size);
	for(i=0;i<size;i++)
	{
		w=getWindowHandler(windows_names[i]);

		printf("%d\t%d\t%s\n",i,(int)w,windows_names[i]);

		if(w!=ERROR_HANDLER){
			img=windowCapture(w);
			if(img!=NULL)
			{
				sprintf(filename,"window%d.bmp",i);
				saveXImageToBitmap(img,filename);
				XDestroyImage(img);
			}
		}
	}

	for(i=0;i<size;i++){
		free(windows_names[i]);
	}
	free(windows_names);

	/*Example of capturing the contents of active window*/
	img=windowCapture(getActiveWindowHandler());
	if(img!=NULL)
		{
			saveXImageToBitmap(img,"active.bmp");
			XDestroyImage(img);
		}

	return 0;

}
