#include "Interpolation.h"

double Interpolation::interpolate(Matrix *_img, double col, double lin, int method)
{
    // Codes:
    // 0 - Nearest Neighbor
    // 1 - Bilinear
    // 2 - Bicubic
    // 3- Lagrange

    img = _img;

    switch (method)
    {
        case 0 : return interpolateNearestNeighbor(col, lin); break;
        case 1 : return interpolateBilinear(col, lin); break;
        case 2 : return interpolateBicubic(col, lin); break;
        case 3 : return interpolateLagrange(col, lin); break;
    }

    return 0.0;
}

/*
 * Nearest Neighbor
 **/

double Interpolation::interpolateNearestNeighbor(double col, double lin)
{
    i = round(lin);
    j = round(col);

    return img->get(i, j);
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

    return (img->get(i,j))*(1.0-dx)*(1.0-dy) + img->get(i+1,j)*(1.0-dx)*dy + img->get(i,j+1)*dx*(1.0-dy) + img->get(i+1,j+1)*dx*dy;
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

    return a(1)*df(dy+1) + a(2)*df(dy) + a(3)*df(dy-1) + a(4)*df(dy-2);
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
    return df(dx+1.0)*img->get(i-1, j+n-2) + img->get(i, j+n-2)*df(dx) + img->get(i+1, j+n-2)*df(dx-1.0) + img->get(i+2, j+n-2)*df(dx-2.0);
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

    return aa(1)*(dy-1.0)*(dy-2.0)*(-dy/6.0) + aa(2)*(dy+1.0)*(dy-1.0)*((dy-2.0)/2.0) + aa(3)*(dy+1.0)*(dy-2.0)*(-dy/2.0) + aa(4)*(dy+1.0)*(dy-1.0)*(dy/6.0);
}

double Interpolation::aa(int n)
{
    return img->get(i-1, j+n-2)*(dx-1.0)*(dx-2.0)*(-dx/6.0) + img->get(i, j+n-2)*(dx+1.0)*(dx-1.0)*((dx-2.0)/2.0) + img->get(i+1, j+n-2)*(dx+1.0)*(dx-2.0)*(-dx/2.0) + img->get(i+2, j+n-2)*(dx+1.0)*(dx-1.0)*(dx/6.0);
}
