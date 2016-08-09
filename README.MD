Library libshot.so

[![Stories in Ready](https://badge.waffle.io/ann-dolbina/project-teamdev.png?label=ready&title=Ready)](http://waffle.io/ann-dolbina/project-teamdev)



libshot.so is a C library for doing screenshots in Linux. 
It has functions for capturing:
- full screen;
- rectangular part of the screen;
- particular windows (even if they are overlapped by other windows) and active window.
Also these screenshots can be resized and stored as uncompressed BMP files.
Library uses Xlib functions, so check if you have X11 library on your computer. C Math library is also necessary.

src:
contains source files for library and a makefile for creating a dynamic-link library "libshot.so"

example:
contains a ready-to-use library "libshot.so", an example of main with usage of functions from this library, necessary headers and a corresponding makefile for creating a binary.

Read help.txt to learn how to use functions from libshot.so

[![Throughput Graph](https://graphs.waffle.io/ann-dolbina/project-teamdev/throughput.svg)](https://waffle.io/ann-dolbina/project-teamdev/metrics/throughput) 
