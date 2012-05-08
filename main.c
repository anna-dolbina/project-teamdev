/*
 * main.c
 *
 *  Created on: 22.03.2012
 *      Author: anna
 */

#include "shot.h"
#include "ximage_processing.h"
#include "windows.h"


int main(int argc, char ** argv){
	XImage* img,*img1;
	/*Window w;
	char** windows_names;
	int size;
	int i;
	char filename[20];*/
	/*img=fullScreenCapture();
	if(img!=NULL)
	{
		saveXImageToBitmap(img,"fullscreen1.bmp");
		img1=resizeBilinear(img,800,600);
		if(img1!=NULL)
		{
			saveXImageToBitmap(img1,"fullscreen1_resized.bmp");
			XDestroyImage(img1);
		}
		else fprintf(stderr,"No resized image\n");

		XDestroyImage(img);
	}*/

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

		img1=resizeBicubic(img,1280,1024);
		if(img1!=NULL)
		{
			saveXImageToBitmap(img1,"rectanglecapture_1280x1024.bmp");
			XDestroyImage(img1);
		}
		else fprintf(stderr,"No resized image\n");
		img1=resizeBicubic(img,200,150);
		if(img1!=NULL)
		{
			saveXImageToBitmap(img1,"rectanglecapture_200x150.bmp");
			XDestroyImage(img1);
		}
		else fprintf(stderr,"No resized image\n");

		img1=resizeBicubic(img,280,210);
		if(img1!=NULL)
		{
			saveXImageToBitmap(img1,"rectanglecapture_280x210.bmp");
			XDestroyImage(img1);
		}
		else fprintf(stderr,"No resized image\n");
		XDestroyImage(img);
	}
	/*getWindowsNames(&windows_names,&size);
	for(i=0;i<size;i++){
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
	free(windows_names);*/
/*
	img=windowCapture(getActiveWindowHandler());
	if(img!=NULL)
		{
			saveXImageToBitmap(img,"active.bmp");
			XDestroyImage(img);
		}*/
	return 0;

}
