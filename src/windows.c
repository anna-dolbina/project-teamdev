/*
 * handlers.c
 *
 *  Created on: 04.05.2012
 *      Author: anna
 */

/*
 * Function for getting active window handler
 * Returns window handler or ERROR_HANDLER if fails*/
#include "windows.h"

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
