/**
 * @file negativeBorderColorPicker.cpp
 * @description Implements the NegativeBorderColorPicker constructor and () function
 *              for CPSC 221 2023S PA2
 *
 *              THIS FILE WILL BE SUBMITTED
 */

#include "negativeBorderColorPicker.h"
#include <math.h>

NegativeBorderColorPicker::NegativeBorderColorPicker(PNG& inputimg, PixelPoint src_px, unsigned int width, double tol)
{
    referenceimg = inputimg;
    source_px = src_px;
    borderwidth = width;
    tolerance = tol;
}

/**
 * Picks the color for pixel (x, y).
 *
 * Leaves a pixel unchanged if it is not on the border of a fill region
 *
 * If a pixel is on the border of a fill region, change it to the "negative"
 * of the original colour value in the Hue and Luminance channels.
 * Saturation and Alpha should remain unchanged.
 * Based on your understanding of HSL colour space from Lab_intro,
 * think about what would be an appropriate "negative" transformation for the
 * Hue and Luminance channels.
 *
 * Hint: a pixel is on the border of a fill region, if it is within "width"
 * pixels of a pixel which is NOT in the fill region!
 * 
 * Also: the border of the image is considered to be a border of the fill region.
 */
HSLAPixel NegativeBorderColorPicker::operator()(PixelPoint p)
{
    HSLAPixel* pixel = referenceimg.getPixel(p.x, p.y) ;
    int img_w = referenceimg.width();
    int img_h = referenceimg.height();

    p.color.s = pixel->s;
    p.color.a = pixel->a;

    int x_start = p.x - borderwidth;
    int x_end = p.x + borderwidth;
    int y_start = p.y - borderwidth;
    int y_end = p.y + borderwidth;

    for (int i=x_start; i<=x_end; i++) {
        for (int j = y_start; j<=y_end; j++) {
            double diagonal = sqrt((p.x - (double)i) * (p.x - (double)i) + (p.y - (double)j) *(p.y - (double)j));
            if ((i<0 || i>=img_w || j<0 || j>=img_h) && diagonal <= borderwidth) {
                p.color.h = (180.0 + pixel->h) - (int)((180.0 + pixel->h) / 360.0) * 360.0;
                p.color.l = 1.0 - pixel->l;
                return p.color;               
            } else {
                HSLAPixel* tmp = referenceimg.getPixel(i,j);
                if (source_px.color.distanceTo(*tmp) > tolerance && diagonal <= borderwidth) {
                    p.color.h = (180.0 + pixel->h) - (int)((180.0 + pixel->h) / 360.0) * 360.0;
                    p.color.l = 1.0 - pixel->l;
                    return p.color;
                } 
            }
        }
    }

    p.color.h = pixel->h;
    p.color.l = pixel->l;
    return p.color;
}

/**
 * Add your private QuarterColorPicker function implementations below
 */