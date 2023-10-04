/**
 * @file stats.cpp
 * @description implementation of Stats class used for fast computation of
 *              colour information from rectangular image regions
 *              CPSC 221 2023S PA3
 *
 *              SUBMIT THIS FILE WITH YOUR MODIFICATIONS
 */

#include "stats.h"

#define NUMBINS 36 // number of histogram bins

Stats::Stats(const PNG& im){
    unsigned w = im.width();
    unsigned h = im.height();
    unsigned curr_k;

    sumHueX.resize(w, vector<double>(h));
    sumHueY.resize(w, vector<double>(h));
    sumSat.resize(w, vector<double>(h));
    sumLum.resize(w, vector<double>(h));
    vector<vector<unsigned int>> his;
    his.resize(h, vector<unsigned int>(36));
    hist.resize(w, his);


    for (unsigned i = 0; i < w; i++) {
        for (unsigned j = 0; j < h; j++) {
            HSLAPixel* pixel = im.getPixel(i,j);
            if (i == 0 && j == 0) {
                sumHueX[i][j]=cos(pixel->h * PI / 180.0);
                sumHueY[i][j]=sin(pixel->h * PI / 180.0);
                sumSat[i][j]=pixel->s;
                sumLum[i][j]=pixel->l;

                // only one pixel
                curr_k = (int)((pixel->h) / 10);
                hist[i][j][curr_k] = 1;

            } else if (i == 0 && j != 0) 
            {
                sumHueX[i][j] = cos(pixel->h * PI / 180.0) + sumHueX[i][j-1];
                sumHueY[i][j] = sin(pixel->h * PI / 180.0) + sumHueY[i][j-1];
                sumSat[i][j]= pixel->s + sumSat[i][j-1];
                sumLum[i][j]= pixel->l + sumLum[i][j-1];

                hist[i][j] = hist[i][j-1];
                curr_k = (int)((pixel->h) / 10);
                hist[i][j][curr_k] ++;

            } else if (i != 0 && j == 0)
            {
                sumHueX[i][j] = cos(pixel->h * PI / 180.0) + sumHueX[i-1][j];
                sumHueY[i][j] = sin(pixel->h * PI / 180.0) + sumHueY[i-1][j];
                sumSat[i][j]= pixel->s + sumSat[i-1][j];
                sumLum[i][j]= pixel->l + sumLum[i-1][j];

                hist[i][j] = hist[i-1][j];
                curr_k = (int)((pixel->h) / 10);
                hist[i][j][curr_k] ++;

            } else {
                sumHueX[i][j] = cos(pixel->h * PI / 180.0) + sumHueX[i-1][j] + sumHueX[i][j-1] - sumHueX[i-1][j-1];
                sumHueY[i][j] = sin(pixel->h * PI / 180.0) + sumHueY[i-1][j] + sumHueY[i][j-1] - sumHueY[i-1][j-1];
                sumSat[i][j]= pixel->s + sumSat[i-1][j] + sumSat[i][j-1] - sumSat[i-1][j-1];
                sumLum[i][j]= pixel->l + sumLum[i-1][j] + sumLum[i][j-1] - sumLum[i-1][j-1];

                for (unsigned k = 0; k < 36; k++) {
                    hist[i][j][k] = hist[i-1][j][k] + hist[i][j-1][k] - hist[i-1][j-1][k];
                }

                curr_k = (int)((pixel->h) / 10);
                hist[i][j][curr_k] ++;
            }
        }
    }

}

long Stats::RectArea(pair<unsigned int, unsigned int> ul, pair<unsigned int, unsigned int> lr){
    long start_x = ul.first;
    long start_y = ul.second;
    long end_x = lr.first;
    long end_y = lr.second;
    long area = (end_x - start_x + 1) * (end_y - start_y + 1);
    return area;
}

HSLAPixel Stats::GetAvg(pair<unsigned int, unsigned int> ul, pair<unsigned int, unsigned int> lr){
    long start_x = ul.first;
    long start_y = ul.second;
    long end_x = lr.first;
    long end_y = lr.second;
    HSLAPixel* pixel = new HSLAPixel();
    long area = RectArea(ul, lr);
    double sumAreaHueX;
    double sumAreaHueY;
    double sumAreaL;
    double sumAreaS;
    if (start_x  == 0 && start_y == 0) {
        sumAreaHueX = sumHueX[end_x][end_y];
        sumAreaHueY = sumHueY[end_x][end_y];
        sumAreaL = sumLum[end_x][end_y];
        sumAreaS = sumSat[end_x][end_y];
    } else if (start_x == 0 && start_y != 0) 
    {
        sumAreaHueX = sumHueX[end_x][end_y] -sumHueX[end_x][start_y-1];
        sumAreaHueY = sumHueY[end_x][end_y] -sumHueY[end_x][start_y-1];
        sumAreaL = sumLum[end_x][end_y] - sumLum[end_x][start_y-1];
        sumAreaS = sumSat[end_x][end_y] - sumSat[end_x][start_y-1];
    } else if (start_x != 0 && start_y == 0)
    {
        sumAreaHueX = sumHueX[end_x][end_y] -sumHueX[start_x-1][end_y];
        sumAreaHueY = sumHueY[end_x][end_y] -sumHueY[start_x-1][end_y];
        sumAreaL = sumLum[end_x][end_y] - sumLum[start_x-1][end_y];
        sumAreaS = sumSat[end_x][end_y] - sumSat[start_x-1][end_y];
    } else {
        sumAreaHueX = sumHueX[end_x][end_y] - sumHueX[end_x][start_y-1] - sumHueX[start_x-1][end_y]+ sumHueX[start_x-1][start_y-1];
        sumAreaHueY = sumHueY[end_x][end_y] - sumHueY[end_x][start_y-1] - sumHueY[start_x-1][end_y]+ sumHueY[start_x-1][start_y-1];
        sumAreaL = sumLum[end_x][end_y] - sumLum[end_x][start_y-1] - sumLum[start_x-1][end_y]+ sumLum[start_x-1][start_y-1];
        sumAreaS = sumSat[end_x][end_y] - sumSat[end_x][start_y-1] - sumSat[start_x-1][end_y]+ sumSat[start_x-1][start_y-1];
    }

    double avg_Hue = atan2(sumAreaHueY, sumAreaHueX) * 180 / PI;
    if (avg_Hue < 0) {
        avg_Hue += 360;
    }
    double avg_Lum = sumAreaL / (double) area;
    double avg_Sat = sumAreaS / (double) area;
    pixel->a = 1.0;
    pixel->h = avg_Hue;
    pixel->s = avg_Sat;
    pixel->l = avg_Lum;
    return *pixel;
}

double Stats::Entropy(pair<unsigned int, unsigned int> ul, pair<unsigned int, unsigned int> lr){

    vector<int> distribution;

    double entropy = 0;
    long area = 0;

    /**
    *  Complete your implementation below.
    *  Using private member hist, assemble the distribution over the
    *  given rectangle defined by points ul, and lr into variable distribution.
    *  You will use distribution to compute the entropy over the rectangle.
    *  if any bin in the distribution has frequency 0, then do not add that 
    *  term to the entropy total. see .h file for more details.
    */

    /* your code should include the following lines (or something similar):
        if (distribution[i] > 0 ) 
            entropy += ((double) distribution[i]/(double) area) 
                                    * log2((double) distribution[i]/(double) area);
    */
   
    long start_x = ul.first;
    long start_y = ul.second;
    long end_x = lr.first;
    long end_y = lr.second;
    area = RectArea(ul,lr);
    distribution.resize(36);
    if (start_x  == 0 && start_y == 0) {
        for (unsigned k = 0; k < 36; k++) {
            distribution[k] = hist[end_x][end_y][k];
            if (distribution[k] > 0 ) {
                entropy += ((double) distribution[k]/(double) area) 
                                    * log2((double) distribution[k]/(double) area);
            }

        }
    } else if (start_x == 0 && start_y != 0) 
    {
        for (unsigned k = 0; k < 36; k++) {
            distribution[k] = hist[end_x][end_y][k] - hist[end_x][start_y-1][k];
            if (distribution[k] > 0 ) {
                entropy += ((double) distribution[k]/(double) area) 
                                    * log2((double) distribution[k]/(double) area);
            }

        }
    } else if (start_x != 0 && start_y == 0)
    {
        for (unsigned k = 0; k < 36; k++) {
            distribution[k] = hist[end_x][end_y][k] - hist[start_x-1][end_y][k];
            if (distribution[k] > 0 ) {
                entropy += ((double) distribution[k]/(double) area) 
                                    * log2((double) distribution[k]/(double) area);
            }

        }
    } else {
        for (unsigned k = 0; k < 36; k++) {
            distribution[k] = hist[end_x][end_y][k]  - hist[end_x][start_y-1][k] - hist[start_x-1][end_y][k] + hist[start_x-1][start_y-1][k];
            if (distribution[k] > 0 ) {
                entropy += ((double) distribution[k]/(double) area) 
                                    * log2((double) distribution[k]/(double) area);
            }
        }
    }
    return  -1 * entropy;
}

/**
 * Add your private Stats function implementations below
 */
