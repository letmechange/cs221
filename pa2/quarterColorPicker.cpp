/**
 * @file quarterColorPicker.cpp
 * @description Implements the QuarterColorPicker constructor and () function
 *              for CPSC 221 2023S PA2
 *
 *              THIS FILE WILL BE SUBMITTED
 */
#include "quarterColorPicker.h"

QuarterColorPicker::QuarterColorPicker(PNG& inputimg)
{
    referenceimg = inputimg;
}

/**
 * Picks the color for pixel (x, y).
 *
 * Using the private reference image, scale each dimension by half
 * and tile the smaller image in a 2 x 2 grid over the original dimensions
 * of the image, and return the pixel at the appropriate coordinate from
 * the tiled image.
 *
 * The value of each pixel in the scaled image will be a bilinear interpolation
 * of a 2x2 pixel region from the original image, with each of the H/S/L/A channels
 * processed individually. Interpolate over the x-axis before the y-axis.
 *
 * Since interpolation will be the exact average between exactly two pixels, then 
 * the Hue channel should be averaged over the shortest arc between the two values.
 * e.g. The average of H_1 = 10 and H_2 = 320 is H_avg = 345 and NOT 165.
 * In the case of two diametric hues being averaged, e.g. 10 and 190, the average will
 * have the smaller value, i.e. 100 and NOT 280.
 * 
 * @pre referenceimg width and height are both even
 */
HSLAPixel QuarterColorPicker::operator()(PixelPoint p)
{  
    unsigned x = p.x;
    unsigned y = p.y;
    unsigned w = referenceimg.width();
    unsigned height = referenceimg.height();

    if (x < w /2 && y< height/2) {
        p.color.a = recreate(x,y).a;
        p.color.h = recreate(x,y).h;
        p.color.l = recreate(x,y).l;
        p.color.s = recreate(x,y).s;
    } else if (w/2 <= x && x < w && y < height/2) {
        p.color.a = recreate(x - w/2, y).a;
        p.color.h = recreate(x - w/2, y).h;
        p.color.l = recreate(x - w/2, y).l;
        p.color.s = recreate(x - w/2, y).s;
    } else if (x < w/2 && height/2 <= y &&y < height)
    {
        p.color.a = recreate(x, y - height/2).a;
        p.color.h = recreate(x, y - height/2).h;
        p.color.l = recreate(x, y - height/2).l;
        p.color.s = recreate(x, y - height/2).s;
    } else if (w/2 <= x&&x < w && height/2 <= y&&y < height)
    {
        p.color.a = recreate(x - w/2, y - height/2).a;
        p.color.h = recreate(x - w/2, y - height/2).h;
        p.color.l = recreate(x - w/2, y - height/2).l;
        p.color.s = recreate(x - w/2, y - height/2).s;
    }     

    return p.color;
}

/**
 * Add your private QuarterColorPicker function implementations below
 */
HSLAPixel QuarterColorPicker::recreate(unsigned x, unsigned y) {
    HSLAPixel* p1 = referenceimg.getPixel(2*x,2*y);
    HSLAPixel* p2 = referenceimg.getPixel(2*x + 1, 2*y);
    HSLAPixel* p3 = referenceimg.getPixel(2*x,2*y +1);
    HSLAPixel* p4 = referenceimg.getPixel(2*x+1,2*y+1);

    double h1 = p1->h;
    double h2 = p2->h;
    double h3 = p3->h;
    double h4 = p4->h;
    double h_12 = abs(h1 - h2);
    double h_34 = abs(h3 - h4);
    double h_avg_12 = 0.0;
    double h_avg_34 = 0.0;
    double h_avg = 0.0;

   if (h_12<= 180) {
        h_avg_12 = h1 / 2+ h2 / 2;
        if (h_12 == 180 && h_avg_12 >= 180) {
            h_avg_12 -= 180;
        }
    } else {
        h_avg_12 = h1 / 2+ h2 / 2+ 180;
        if (h_avg_12 >= 360) {
            h_avg_12 -= 360;
        }
    }

    if (h_34 <= 180) {
        h_avg_34 = h3 / 2+ h4 / 2;
        if (h_34 == 180 && h_avg_34  >= 180) {
            h_avg_34  -= 180;
        }
    } else {
        h_avg_34 = h3/2 + h4/2 + 180;
        if (h_avg_34 >= 360) {
            h_avg_34 -= 360;
        }
    }

    double h_dif = abs(h_avg_34 - h_avg_12);
    if (h_dif <= 180) {
        h_avg = h_avg_34/ 2 + h_avg_12/ 2;
        if (h_dif == 180 && h_avg >= 180) {
            h_avg -= 180;
        }
    } else {
        h_avg = h_avg_34 / 2+ h_avg_12 / 2+ 180;
        if (h_avg >= 360) {
            h_avg -= 360;
        }
    }
    
    double h = h_avg;
    double a = p1->a/4 + p2->a/4 + p3->a/4 + p4->a/4;
    double l = p1->l/4+ p2->l /4+ p3->l/4 + p4->l/4;
    double s = p1->s /4+ p2->s /4+ p3->s /4+ p4->s/4;

    HSLAPixel* pixel = new HSLAPixel(h, s, l, a);
    return *pixel;
}