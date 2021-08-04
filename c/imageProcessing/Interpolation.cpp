/*Copyright 2002-2021 e-foto team (UERJ)
  This file is part of e-foto.

    e-foto is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    e-foto is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with e-foto.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Interpolation.h"
#include "Matrix.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

Interpolation::Interpolation() = default;

double Interpolation::interpolate(Matrix* _img,
                                  double col,
                                  double lin,
                                  InterpMethode method)
{
    img = _img;
    ccolor = Color::gray;

    double (Interpolation::*interpolateFunc)(double, double) =
        &Interpolation::interpolateNearestNeighbor;

    switch (method) {
    case InterpMethode::nearestNeighbor:
        return interpolateNearestNeighbor(col, lin);
        break;

    case InterpMethode::bilinear :
        interpolateFunc = &Interpolation::interpolateBilinear;
        break;

    case InterpMethode::bicubic :
        interpolateFunc = &Interpolation::interpolateBicubic;
        break;

    case InterpMethode::lagrange :
        interpolateFunc = &Interpolation::interpolateLagrange;
        break;
    }

    double pixel[3];
    int rounds = (mode == ColorMode::Color) ? 3 : 1;

    for (int f = 0; f < rounds; f++) {
        if (mode == ColorMode::Color) {
            switch (f) {
            case 0:
                ccolor = Color::red;
                break;

            case 1:
                ccolor = Color::green;
                break;

            case 2:
                ccolor = Color::blue;
                break;

            default:
                ccolor = Color::gray;
                break;
            }
        }
        else {
            ccolor = Color::gray;
        }

        pixel[f] = (this->*interpolateFunc)(col, lin);
    }

    if (mode == ColorMode::Color)
        return double((int(pixel[0] * 255.0) << 16) +
                      (int(pixel[1] * 255.0) << 8) +
                      int(pixel[2] * 255.0)) / 0xFFFFFF;

    return pixel[0];
}

/*
 * Nearest Neighbor
 **/

double Interpolation::interpolateNearestNeighbor(double col, double lin)
{
    i = round(lin);
    j = round(col);

    if (!checkLimits(j, i)) {
        return 0.0;
    }

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

    if (!checkLimits(j + 1, i + 1)) {
        return interpolateNearestNeighbor(j, i);
    }

    return (I(i, j))
           * (1.0 - dx)
           * (1.0 - dy)
           + I(i + 1, j)
           * (1.0 - dx)
           * dy
           + I(i, j + 1)
           * dx
           * (1.0 - dy)
           + I(i + 1, j + 1)
           * dx
           * dy;
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

    if (!checkLimits(j + 2, i + 2) || !checkLimits(j - 1, i - 1)) {
        return interpolateNearestNeighbor(j, i);
    }

    double pixel = a(1) * df(dy + 1) + a(2) * df(dy) + a(3) * df(dy - 1) + a(
                       4) * df(dy - 2);

    if (pixel > 1.0) {
        pixel = 1.0;
    }

    if (pixel < 0.0) {
        pixel = 0.0;
    }

    return pixel;
}

double Interpolation::df(double x)
{
    // Get modulus
    x = fabs(x);

    if (x < 1.0) {
        return pow(x, 3) - 2 * pow(x, 2) + 1.0;
    }

    if (x < 2.0) {
        return -pow(x, 3) + 5 * pow(x, 2) - 8 * x + 4.0;
    }

    return 0.0;
}

double Interpolation::a(int n)
{
    return df(dx + 1.0)
           * I(i - 1, j + n - 2)
           + I(i, j + n - 2)
           * df(dx)
           + I(i + 1, j + n - 2)
           * df(dx - 1.0)
           + I(i + 2, j + n - 2)
           * df(dx - 2.0);
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

    if (!checkLimits(j + 2, i + 2) || !checkLimits(j - 1, i - 1)) {
        return interpolateNearestNeighbor(j, i);
    }

    double pixel = aa(1)
                   * (dy - 1.0)
                   * (dy - 2.0)
                   * (-dy / 6.0)
                   + aa(2)
                   * (dy + 1.0)
                   * (dy - 1.0)
                   * ((dy - 2.0) / 2.0)
                   + aa(3)
                   * (dy + 1.0)
                   * (dy - 2.0)
                   * (-dy / 2.0)
                   + aa(4)
                   * (dy + 1.0)
                   * (dy - 1.0)
                   * (dy / 6.0);

    if (pixel > 1.0) {
        pixel = 1.0;
    }

    if (pixel < 0.0) {
        pixel = 0.0;
    }

    return pixel;
}

double Interpolation::aa(int n)
{
    return I(i - 1, j + n - 2)
           * (dx - 1.0)
           * (dx - 2.0)
           * (-dx / 6.0)
           + I(i, j + n - 2)
           * (dx + 1.0)
           * (dx - 1.0)
           * ((dx - 2.0) / 2.0)
           + I(i + 1, j + n - 2)
           * (dx + 1.0)
           * (dx - 2.0)
           * (-dx / 2.0)
           + I(i + 2, j + n - 2)
           * (dx + 1.0)
           * (dx - 1.0)
           * (dx / 6.0);
}


/*
 * Read pixel
 **/

double Interpolation::I(int i, int j) const
{
    if (ccolor == Color::gray) {
        return img->get(i, j);
    }

    // Get pixel from matrix
    int pixel = int(img->get(i, j) * 0xFFFFFF);;

    if (ccolor == Color::red) {
        pixel = (pixel & 0xFF0000) >> 16;
        return double(pixel) / 255.0;
    }

    if (ccolor == Color::green) {
        pixel = (pixel & 0xFF00) >> 8;
        return double(pixel) / 255.0;
    }

    if (ccolor == Color::blue) {
        pixel = pixel & 0xFF;
        return double(pixel) / 255.0;
    }

    return 0.0;
}

/*
 * Check limits
 **/

bool Interpolation::checkLimits(int col, int row) const
{
    return (col >= 1
            && row >= 1
            && unsigned(col) <= img->getCols()
            && unsigned(row) <= img->getRows());
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
