/*******************************************************************************
							   interpolation.h
*******************************************************************************/


#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <stdio.h>
#include <math.h>
#include "Matrix.h"

/**
* class ProjectiveRay
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 14/12/2011
* @version 1.0 - Marcelo Teixeira Silveira
*/

using namespace std;

class Interpolation
{
public:
    Interpolation();
    double interpolate(Matrix *_img, double col, double lin, int method);
    void setMode(int _mode) { mode = _mode%2; };

private:
    double interpolateNearestNeighbor(double col, double lin);
    double interpolateBilinear(double col, double lin);
    double interpolateBicubic(double col, double lin);
    double df(double x);
    double a(int n);
    double interpolateLagrange(double col, double lin);
    double aa(int n);
    bool checkLimits(int col, int row);
    double dx, dy;
    int i, j, mode, ccolor;
    Matrix *img;
    double I(int i, int j);

};

#endif

