/*
 * main.c
 *
 *  Created on: 22.03.2012
 *      Author: anna
 */

#include "shot.h"
int main(int argc, char ** argv){
	XImage* img;
	Window w;
	char** windows_names;
	int size;
	int i;
	char filename[20];
	/*img=fullScreenCapture();
	if(img!=NULL)
	{
		saveXImageToBitmap(img,"fullscreen1.bmp");
		XDestroyImage(img);
	}

	img=rectangleCapture(15,50,320,240);
	if(img!=NULL)
	{
		saveXImageToBitmap(img,"rectangle1.bmp");
		XDestroyImage(img);
	}*/
	getWindowsNames(&windows_names,&size);
	for(i=0;i<size;i++){
		w=getWindowHandler(windows_names[i]);
		printf("%d\t%d\t%s\n",i,(int)w,windows_names[i]);

		if(w!=-1){
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
/*
	img=windowCapture(getActiveWindowHandler());
	if(img!=NULL)
		{
			saveXImageToBitmap(img,"active.bmp");
			XDestroyImage(img);
		}*/
	return 0;

}
