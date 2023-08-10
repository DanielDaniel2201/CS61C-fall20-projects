/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	//YOUR CODE HERE
	//the white or black color to be returned
	Color *secret = (Color *) malloc (sizeof(Color));
	//find the specified color
	Color *c = *(image->image + image->cols * row + col);
	/* 
		0101 0110 & 0000 0001 is equal to 0000 0001
		0101 1110 & 0000 0001 is equal to 0000 0000
	   	& operator can be used to identify whether the binary representation of a decimal ends with 1 or not
	*/
	int tmp = (c->B) & 1;
	secret->B = secret->G = secret->R = tmp * 255;
	return secret;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	//YOUR CODE HERE
	Image *img = (Image *) malloc (sizeof(Image));
	img->cols = image->cols;
	img->rows = image->rows;
	img->image = (Color **) malloc (sizeof(Color) * image->cols * image->rows);
	Color **colors = img->image;
	for (int i = 0; i < image->rows; i += 1) {
		for (int j = 0; j < image->cols; j += 1) {
			*colors = evaluateOnePixel(image, i, j);
			colors += 1;
		}
	}
	return img;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc != 2) {
		printf("wrong number of arguments");
		return -1;
	}
	char file_name = argv[1];
	Image *origial = readData(file_name);
	Image *new = steganography(origial);
	writeData(new);
	freeImage(origial);
	freeImage(new);
	return 0;
}
