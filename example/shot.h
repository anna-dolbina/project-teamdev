/*
 * shot.h
 *
 *  Created on: 22.03.2012
 *      Author: anna
 */
#ifndef SHOT_H_
#define SHOT_H_

#ifndef X11_HEADERS
	#define X11_HEADERS

	#include <X11/Xlib.h>
	#include <X11/Xutil.h>
	#include <X11/Xresource.h>
#endif /*X11_HEADERS*/

#include <stdio.h>
#include <stdlib.h>
#include "windows.h"
#pragma pack (1)



/*Functions*/
XImage* fullScreenCapture();
XImage* rectangleCapture(int x_top_left, int y_top_left, int width, int height);
XImage* windowCapture(Window handler);

#endif /*SHOT_H_*/
