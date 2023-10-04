/**
 * @file imageTileColorPicker.cpp
 * @description Implements the ImageTileColorPicker constructor and () function
 *              for CPSC 221 2023S PA2
 * 
 *              THIS FILE WILL BE SUBMITTED
 */

#include "imageTileColorPicker.h"

ImageTileColorPicker::ImageTileColorPicker(PNG& secondary_img)
{
    tileimg = secondary_img;
}

/**
 * Picks the color for pixel (x, y).
 *
 * The tile image should be tiled over the dimensions of the destination image,
 * aligned to (0, 0) in the destination image.
 *
 * The returned value will be the colour of the appropriate pixel in the tile image,
 * where it would be over the specified location in the destination image.
 */
HSLAPixel ImageTileColorPicker::operator()(PixelPoint p)
{   
    unsigned width = tileimg.width();
    unsigned height = tileimg.height();
    HSLAPixel* pixel = tileimg.getPixel(p.x%width, p.y%height);
    p.color.a = pixel->a;
    p.color.h = pixel->h;
    p.color.l = pixel->l;
    p.color.s = pixel->s;
    return p.color;
}

/**
 * Add your private QuarterColorPicker function implementations below
 */
