main:main.o LCD.o BMP.o JPG.o 
	arm-linux-gcc -o main main.o LCD.o BMP.o JPG.o -L./libjpeg -ljpeg 
	rm main.o LCD.o BMP.o JPG.o
main.o:main.c ./lib/lcd/LCD.h ./lib/jpg/JPG.h
	arm-linux-gcc -c main.c
LCD.o:./lib/lcd/LCD.c ./lib/lcd/LCD.h
	arm-linux-gcc -c ./lib/lcd/LCD.c
BMP.o:./lib/bmp/BMP.c ./lib/bmp/BMP.h
	arm-linux-gcc -c ./lib/bmp/BMP.c
JPG.o:./lib/jpg/JPG.c ./lib/jpg/JPG.h ./lib/bmp/BMP.h
	arm-linux-gcc -c ./lib/jpg/JPG.c -I./libjpeg -L./libjpeg -ljpeg 

