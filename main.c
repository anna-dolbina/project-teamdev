/*
 * main.c
 *
 *  Created on: 22.03.2012
 *      Author: anna
 */

#include "shot.h"
int main(int argc, char ** argv){
	XImage* img;

	img=fullScreenCapture();
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
	}
	return 0;

}
