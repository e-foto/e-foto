#include "Interpolation.h"

Interpolation::Interpolation()
{
	// Mode:
	// 0 - Grayscale (0-255) in double format (0.0 - 1.0)
	// 1 - Color (24-bit RRGGBB) in double format (0.0 - 1.0)
	mode =0;
}

double Interpolation::interpolate(Matrix *_img, double col, double lin, int method)
{
    // Codes:
    // 0 - Nearest Neighbor
    // 1 - Bilinear
    // 2 - Bicubic
    // 3- Lagrange

    if (method < 0 || method > 3)
        return 0.0;

    img = _img;

    ccolor = 0;
    if (method == 0)
	return interpolateNearestNeighbor(col, lin);

    double pixel[3], fpixel, shft=1.0/256.0;
    int rounds;
    (mode==1) ? rounds=3 : rounds=1;

    for (int f=0; f<rounds; f++)
    {
        (mode==1) ? ccolor=f+1 : ccolor=0;

        switch (method)
        {
            case 1 : pixel[f] = interpolateBilinear(col, lin); break;
            case 2 : pixel[f] = interpolateBicubic(col, lin); break;
            case 3 : pixel[f] = interpolateLagrange(col, lin); break;
        }
    }

    (mode==1) ? fpixel = double((int(pixel[0]*255.0) << 16) + (int(pixel[1]*255.0) << 8) + int(pixel[2]*255.0))/0xFFFFFF : fpixel = pixel[0];

    return fpixel;
}

/*
 * Nearest Neighbor
 **/

double Interpolation::interpolateNearestNeighbor(double col, double lin)
{
    i = round(lin);
    j = round(col);

    if (!checkLimits(j, i))
        return 0.0;

    return I(i, j);
}


/*
 * Bilinear
 **/

double Interpolation::interpolateBilinear(double col, double lin)
{
    i = floor(lin);
    j = floor(col);
    dx = col - floor(col);
    dy = lin - floor(lin);

    if (!checkLimits(j+1, i+1))
        return interpolateNearestNeighbor(j,i);

    return (I(i,j))*(1.0-dx)*(1.0-dy) + I(i+1,j)*(1.0-dx)*dy + I(i,j+1)*dx*(1.0-dy) + I(i+1,j+1)*dx*dy;
}

/*
 * Bicubic
 **/

double Interpolation::interpolateBicubic(double col, double lin)
{
    dx = col - floor(col);
    dy = lin - floor(lin);
    i = floor(lin);
    j = floor(col);

    if (!checkLimits(j+2, i+2) || !checkLimits(j-1, i-1))
        return interpolateNearestNeighbor(j,i);

    double pixel = a(1)*df(dy+1) + a(2)*df(dy) + a(3)*df(dy-1) + a(4)*df(dy-2);

    if (pixel>1.0) pixel = 1.0;
    if (pixel<0.0) pixel = 0.0;

    return pixel;
}

double Interpolation::df(double x)
{
    // Get modulus
    x = fabs(x);

    if (x < 1.0)
        return pow(x,3) - 2*pow(x,2) + 1.0;

    if (x < 2.0)
        return -pow(x,3) + 5*pow(x,2) - 8*x + 4.0;

    return 0.0;
}

double Interpolation::a(int n)
{
    return df(dx+1.0)*I(i-1, j+n-2) + I(i, j+n-2)*df(dx) + I(i+1, j+n-2)*df(dx-1.0) + I(i+2, j+n-2)*df(dx-2.0);
}


/*
 * Lagrange
 **/

double Interpolation::interpolateLagrange(double col, double lin)
{
    dx = col - floor(col);
    dy = lin - floor(lin);
    i = floor(lin);
    j = floor(col);

    if (!checkLimits(j+2, i+2) || !checkLimits(j-1, i-1))
        return interpolateNearestNeighbor(j,i);

    double pixel = aa(1)*(dy-1.0)*(dy-2.0)*(-dy/6.0) + aa(2)*(dy+1.0)*(dy-1.0)*((dy-2.0)/2.0) + aa(3)*(dy+1.0)*(dy-2.0)*(-dy/2.0) + aa(4)*(dy+1.0)*(dy-1.0)*(dy/6.0);

    if (pixel>1.0) pixel = 1.0;
    if (pixel<0.0) pixel = 0.0;

    return pixel;
}

double Interpolation::aa(int n)
{
    return I(i-1, j+n-2)*(dx-1.0)*(dx-2.0)*(-dx/6.0) + I(i, j+n-2)*(dx+1.0)*(dx-1.0)*((dx-2.0)/2.0) + I(i+1, j+n-2)*(dx+1.0)*(dx-2.0)*(-dx/2.0) + I(i+2, j+n-2)*(dx+1.0)*(dx-1.0)*(dx/6.0);
}


/*
 * Read pixel
 **/

double Interpolation::I(int i, int j)
{
    if (ccolor == 0)
        return img->get(i,j);

    // Get pixel from matrix
    int pixel = int(img->get(i,j)*0xFFFFFF);;

    // Red
    if (ccolor == 1)
    {
        pixel = (pixel & 0xFF0000) >> 16;
        return double(pixel)/255.0;
    }

    // Green
    if (ccolor == 2)
    {
        pixel = (pixel & 0xFF00) >> 8;
        return double(pixel)/255.0;
    }

    // Blue
    if (ccolor == 3)
    {
        pixel = pixel & 0xFF;
        return double(pixel)/255.0;
    }

    return 0.0;
}

/*
 * Check limits
 **/

bool Interpolation::checkLimits(int col, int row)
{
    return (col >= 1 && row >=1 && col <= img->getCols() && row <= img->getRows());
}
