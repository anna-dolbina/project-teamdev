/*
 * handlers.h
 *
 *  Created on: 04.05.2012
 *      Author: anna
 */

#ifndef WINDOWS_H_
#define WINDOWS_H_

#ifndef X11_HEADERS
	#define X11_HEADERS

	#include <X11/Xlib.h>
	#include <X11/Xutil.h>
	#include <X11/Xresource.h>
#endif /*X11_HEADERS*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma pack (1)

#ifndef ERROR_HANDLER
#define ERROR_HANDLER (unsigned long)(-1)
#endif /*ERROR_HANDLER*/

/*Functions*/
Window getActiveWindowHandler();
Window getWindowHandler(char* window_name);
int getWindowsNames(char*** names, int*size);
int isVisible(Window w);

#endif /* WINDOWS_H_ */
