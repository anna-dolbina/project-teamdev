# Makefile for screenshots project

binary: main.o libshot.so
	gcc -o binary main.o -L. -lshot -Wl,-rpath,.

main.o: main.c
	gcc -c main.c


