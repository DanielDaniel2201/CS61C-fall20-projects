/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	//YOUR CODE HERE
	//open a file pointer to the file
	FILE *file_pointer = fopen(filename, "r");
	if (file_pointer == NULL) {
		printf("Fail to open %s\n", filename);
		return NULL;
	}

	//check if the format is P3 
	char format[3];
	fscanf(file_pointer, "%s", format);
	if (format[0] != 'P' || format[1] != '3') {
		printf("Incorrect file format\n");
		return NULL;
	}
	
	//initialize an Image and set its col and row
	Image *ultimate_image = (Image *) malloc (sizeof(Image));
	fscanf(file_pointer, "%u", &ultimate_image->cols);
	fscanf(file_pointer, "%u", &ultimate_image->rows);

	//read in the color scale
	int color_scale;
	fscanf(file_pointer, "%u", &color_scale);

	/*read row by row, every three numbers comprise a color struct
	image of an Image is an array of color structs*/
	int row = ultimate_image->rows;
	int col = ultimate_image->cols; 
	Color **img;
	for (int i = 0; i < row * col; i += 1) {
		Color *pixel = (Color *) malloc (sizeof(Color));
		fscanf(file_pointer, "%hhu", &pixel->R);
		fscanf(file_pointer, "%hhu", &pixel->G);
		fscanf(file_pointer, "%hhu", &pixel->B);
		*(img + i) = pixel;
	}
	ultimate_image->image = img;

	//close the file pointer
	fclose(file_pointer);
	return ultimate_image;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
	//print the header
	printf("P3\n%d %d\n255\n", image->cols, image->rows);
	int row = image->rows;
	int col = image->cols;
	Color **p = image->image;
	for (int i = 0; i < row; i += 1) {
		//for each row print every RGB except the last one
		for (int j = 0; j < col - 1; j += 1) {
			printf("%3hhu %3hhu %3hhu   ", (*p)->R, (*p)->G, (*p)->B);
			p += 1;
		}
		//print the last RGB of each row
		printf("%3hhu %3hhu %3hhu\n", (*p)->R, (*p)->G, (*p)->B);
		p += 1;
	}
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
	for (int i = 0; i < image->cols * image->rows; i += 1) {
		free(*(image->image + i));
	}
	free(image->image);
	free(image);
}