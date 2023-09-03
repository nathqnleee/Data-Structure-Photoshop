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
    int w = im.width();
    int h = im.height();

    sumHueX.resize(h);
    sumHueY.resize(h);
    sumSat.resize(h);
    sumLum.resize(h);

    hist.resize(h);

    for (int x = 0; x < h; x++)
    {
        sumHueX[x].resize(w);
        sumHueY[x].resize(w);
        sumSat[x].resize(w);
        sumLum[x].resize(w);

        for (int y = 0; y < w; y++)
        {
            sumHueX[x][y] = cos(im.getPixel(y, x)->h * PI / 180.0);

            sumHueY[x][y] = sin(im.getPixel(y, x)->h * PI / 180.0);

            sumSat[x][y] = im.getPixel(y, x)->s;

            sumLum[x][y] = im.getPixel(y, x)->l;

            if (x > 0)
            {
                sumHueX[x][y] += sumHueX[x-1][y];

                sumHueY[x][y] += sumHueY[x-1][y];

                sumSat[x][y] += sumSat[x-1][y];

                sumLum[x][y] += sumLum[x-1][y];
            }

            if (y > 0)
            {
                sumHueX[x][y] += sumHueX[x][y-1];

                sumHueY[x][y] += sumHueY[x][y-1];

                sumSat[x][y] += sumSat[x][y-1];

                sumLum[x][y] += sumLum[x][y-1];
            }

            if (x > 0 && y > 0)
            {
                sumHueX[x][y] -= sumHueX[x-1][y-1];

                sumHueY[x][y] -= sumHueY[x-1][y-1];

                sumSat[x][y] -= sumSat[x-1][y-1];

                sumLum[x][y] -= sumLum[x-1][y-1];
            }
        }
    }

    for(int x = 0; x < h; x++)
    {
        hist[x].resize(w);

        for(int y = 0; y < w; y++)
        {
            hist[x][y].resize(NUMBINS);

            for(int i = 0; i < NUMBINS; i++) 
            {
                hist[x][y][i] = 0;

                if (x > 0)
                {
                    hist[x][y][i] += hist[x-1][y][i];
                }

                if (y > 0)
                {
                    hist[x][y][i] += hist[x][y-1][i];
                }

                if (x > 0 && y > 0)
                {
                    hist[x][y][i] -= hist[x-1][y-1][i];
                }
                
                if (10*i <= (int)im.getPixel(y, x)->h && (int)im.getPixel(y, x)->h < 10*(1 + i))
                {
                    hist[x][y][i] += 1;
                }  
            }
        }
    }
}

long Stats::RectArea(pair<unsigned int, unsigned int> ul, pair<unsigned int, unsigned int> lr){
    unsigned int xPairOne = ul.first;
    unsigned int yPairOne = ul.second;
    unsigned int xPairTwo = lr.first;
    unsigned int yPairTwo = lr.second;

    return (xPairTwo - xPairOne + 1) * (yPairTwo - yPairOne + 1);
}

HSLAPixel Stats::GetAvg(pair<unsigned int, unsigned int> ul, pair<unsigned int, unsigned int> lr){
    unsigned int w = hist[0].size();
    unsigned int h = hist.size();

    double hueAverage;

    if (ul.second < 0 || lr.first >= w || lr.first < ul.first || ul.first < 0 || lr.second < ul.second || lr.second >= h) 
    {
        throw std::out_of_range("Invalid coordinates for a rectangle");
    }

    double area = static_cast<double>(RectArea(ul, lr));

    double saturationSum = sumSat[lr.second][lr.first];
    double luminSum = sumLum[lr.second][lr.first];
    
    double xHueSum = sumHueX[lr.second][lr.first];
    double yHueSum = sumHueY[lr.second][lr.first];

    if (ul.first > 0) 
    {
        saturationSum -= sumSat[lr.second][ul.first - 1];
        luminSum -= sumLum[lr.second][ul.first - 1];

        xHueSum -= sumHueX[lr.second][ul.first - 1];
        yHueSum -= sumHueY[lr.second][ul.first - 1];
    }

    if (ul.second > 0) 
    {
        saturationSum -= sumSat[ul.second - 1][lr.first];
        luminSum -= sumLum[ul.second - 1][lr.first];
        
        xHueSum -= sumHueX[ul.second - 1][lr.first];
        yHueSum -= sumHueY[ul.second - 1][lr.first];
    }

    if (ul.second > 0 && ul.first > 0) 
    {
        saturationSum += sumSat[ul.second - 1][ul.first - 1];
        luminSum += sumLum[ul.second - 1][ul.first - 1];
        
        xHueSum += sumHueX[ul.second - 1][ul.first - 1];
        yHueSum += sumHueY[ul.second - 1][ul.first - 1];
    }

    if (xHueSum == 0.0 && yHueSum == 0.0) 
    {
        hueAverage = 0.0;
    } 
    
    else 
    {
        hueAverage = std::atan2(yHueSum, xHueSum);
        hueAverage = (hueAverage >= 0.0) ? hueAverage : (hueAverage + (2.0 * PI));
    }

    hueAverage = hueAverage * (180.0 / PI);

    HSLAPixel pix = HSLAPixel(hueAverage, saturationSum / area, luminSum / area, 1.0);

    return pix;
}

double Stats::Entropy(pair<unsigned int, unsigned int> ul, pair<unsigned int, unsigned int> lr) 
{
    vector<unsigned int> dist;
    dist.resize(NUMBINS);
    double entropy = 0.0;
    long area = RectArea(ul, lr);

    for(unsigned int x = 0; x < NUMBINS; x++)
    {
        dist[x] = hist[lr.second][lr.first][x];

        if (ul.second > 0)
        {
            dist[x] -= hist[ul.second-1][lr.first][x];
        }

        if (ul.first > 0)
        {
            dist[x] -= hist[lr.second][ul.first-1][x];
        }

        if (ul.second>0 && ul.first>0)
        {
            dist[x] += hist[ul.second-1][ul.first-1][x];
        }
    }

    for (unsigned int x = 0; x < NUMBINS; x++)
    {
        if (dist[x] > 0) {
            entropy += ((double)dist[x] / (double)area) * log2((double)dist[x] / (double)area);
        }
    }

    return entropy * -1;
}

vector<unsigned int> Stats::buildHist(pair<unsigned int, unsigned int> ul, pair<unsigned int, unsigned int> lr)
{
   
    unsigned int xUpLeftCoord = ul.first;
    unsigned int yUpLeftCoord = ul.second;
    
    unsigned int xLeftRightCoord = lr.first;
    unsigned int yLeftRightCoord = lr.second;

    vector<unsigned int> returnVal = hist[xLeftRightCoord][yLeftRightCoord];

    for (unsigned int x = 0; x < NUMBINS; x++)
    {
        unsigned int bin = 0; 
        
        if (xUpLeftCoord == 0 && yUpLeftCoord == 0)
        {
            bin = hist[xLeftRightCoord][yLeftRightCoord][x];
        }

        else if (xUpLeftCoord == 0 && yUpLeftCoord != 0)
        {
            bin = hist[xLeftRightCoord][yLeftRightCoord][x] - hist[xLeftRightCoord][yUpLeftCoord - 1][x];
        }

        else if (xUpLeftCoord != 0 && yUpLeftCoord == 0)
        {
            bin = hist[xLeftRightCoord][yLeftRightCoord][x] - hist[xUpLeftCoord - 1][yLeftRightCoord][x];
        }

        else if (xUpLeftCoord != 0 && yUpLeftCoord != 0)
        {
            bin = hist[xLeftRightCoord][yLeftRightCoord][x] - hist[xLeftRightCoord][yUpLeftCoord - 1][x] - hist[xUpLeftCoord - 1][yLeftRightCoord][x] + hist[xUpLeftCoord - 1][yUpLeftCoord - 1][x];
        }

        returnVal[x] = bin;
    }

    return returnVal;
}
