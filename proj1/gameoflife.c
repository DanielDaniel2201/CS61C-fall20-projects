/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

int delta_x[] = {-1, -1, -1, 0, 0, 1, 1, 1};
int delta_y[] = {-1, 0, 1, 1, 1, 0, -1, -1};

int ring(int x, int y) {
	return (x + y) % y;
}

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	//YOUR CODE HERE
	//initialize a new Color to be returned
	Color *new_color = (Color *) malloc (sizeof(Color));

	//seperate R G B to calculate respectively

	//check the R G B state of the specified Color
	int center_r = (*(image->image + row * image->cols + col))->R == 255;
	int center_g = (*(image->image + row * image->cols + col))->G == 255;
	int center_b = (*(image->image + row * image->cols + col))->B == 255;

	//count the number of live neighbors
	int neighbor_r = 0, neighbor_g = 0, neighbor_b = 0;
	for (int i = 0; i < 8; i += 1) {
		int new_row = ring(row + delta_x[i], image->rows);
		int new_col = ring(col + delta_y[i], image->cols);
		if ((*(image->image + new_row * image->cols+ new_col))->R == 255) {
			neighbor_r += 1;
		}
		if ((*(image->image + new_row * image->cols+ new_col))->G == 255) {
			neighbor_g += 1;
		}
		if ((*(image->image + new_row * image->cols+ new_col))->B == 255) {
			neighbor_b += 1;
		}
	}

	//set variables to compare with the rule
	int pos_r = 9 * center_r + neighbor_r;
	int pos_g = 9 * center_g + neighbor_g;
	int pos_b = 9 * center_b + neighbor_b;

	//compare with the rule with respect to R G B
	if (rule & (1 << pos_r)) {
		new_color->R = 255;
	} else {
		new_color->R = 0;
	}
	if (rule & (1 << pos_g)) {
		new_color->G = 255;
	} else {
		new_color->G = 0;
	}
	if (rule & (1 << pos_b)) {
		new_color->B = 255;
	} else {
		new_color->B = 0;
	}
	
	return new_color;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	//YOUR CODE HERE
	Image *img = (Image *) malloc (sizeof(Image));
	img->image = (Color**)malloc(sizeof(Color*) * (image->rows) * (image->cols));
	img->rows = image->rows;
	img->cols = image->cols;
	Color **p = image->image;
	int row = image->rows;
	int col = image->cols;
	for (int i = 0; i < row; i += 1) {
		for (int j = 0; j < col; j += 1) {
			p = evaluateOneCell(image, i, j, rule); 
			p += 1;
		}
	}
	return img;	
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc != 3) {
		printf("usage: ./gameOfLife filename rule\nfilename is an ASCII PPM file (type P3) with maximum value 255.\nrule is a hex number beginning with 0x; Life is 0x1808.");
		return 1;
	}
	char file_name = argv[1];
	Image *origial = readData(file_name);
	uint32_t rule = strtol(argv[2], NULL, 16);
	Image *new = life(origial, rule);
	writeData(new);
	freeImage(origial);
	freeImage(new);
	return 0;
}
