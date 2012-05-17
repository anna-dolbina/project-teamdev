# Makefile for creating libshot.so

libshot.so: shot.o windows.o ximage_processing.o
	gcc -shared -o libshot.so shot.o windows.o ximage_processing.o -lX11 -lm

shot.o: shot.c
	gcc -c -fPIC shot.c -lX11

windows.o: windows.c
	gcc -c -fPIC windows.c -lX11

ximage_processing.o: ximage_processing.c
	gcc -c -fPIC ximage_processing.c -lX11 -lm
