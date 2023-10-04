/**
 * File: block.cpp
 * Description: student implementation for Block functions
 *              for CPSC 221 2023S PA1
 *
 *              THIS FILE WILL BE SUBMITTED.
 */

#include "block.h"

/**
 * Creates a square Block that is dimension x dimension pixels in size,
 * starting at (x, y) on img. Assumes that the requested
 * Block fits on the image (inputs are feasible)
 */
void Block::Build(PNG& img, unsigned int x, unsigned int y, unsigned int dimension) {
	data.clear();
	for (unsigned a = 0; a < dimension;a++)	{
		vector<HSLAPixel> pixels;
		for (unsigned b = 0; b < dimension; b++){
			HSLAPixel* img_pixel = img.getPixel(x+a, y+b);
			pixels.push_back(*img_pixel);
		}
		data.push_back(pixels);
	}
}

/**
 * Renders this Block onto img with its upper-left corner at (x, y).
 * Assumes block fits on the image.
 */
void Block::Render(PNG& img, unsigned int x, unsigned int y) const {;
	for (unsigned i = 0; i<Dimension(); i++) {
		for (unsigned j = 0; j<Dimension();j++) {
			HSLAPixel* img_pixel = img.getPixel(x+i,y+j);
			
			*img_pixel = data[i][j];
		}

	}
}

/**
 * Flips the Block's pixel data horizontally by a simple reflection
 * across the vertical axis through its middle.
 */
void Block::FlipHorizontal() {
	unsigned size = Dimension();
	for (unsigned j = 0; j < size; j++) {
		for (unsigned i = 0; i < size/2; i++) {
			HSLAPixel temp;
			temp = data[i][j];
			data[i][j] = data[size-1-i][j];
			data[size-1-i][j] = temp;
		}
	}

}

/**
 * Rotates the Block's pixel data 90 degrees counter-clockwise.
 */
void Block::RotateCCW() {
	unsigned size = Dimension();
	vector<HSLAPixel> temp_col;
	for (unsigned i = size; i > 0; i--) {
		for (unsigned j = size; j>0; j--) {
			temp_col.push_back(data[j-1][i-1]);
		}
	}


	unsigned position = 0;
	data.clear();
	
	for (unsigned a = 0; a < size;a++)	{
		vector<HSLAPixel> pixels;
		for (unsigned b = 0; b < size; b++)
		{	
			pixels.push_back(temp_col[position]);
			position++;
			// std::cout << "line " << __LINE__ << ": x = " << position << std::endl;
		}
		data.push_back(pixels);
	}
	FlipHorizontal();
}

/**
 * Returns the dimension of the block (its width or height).
 */
unsigned int Block::Dimension() const {
	return data.size();
}