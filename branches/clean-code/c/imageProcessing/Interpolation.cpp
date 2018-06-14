/*Copyright 2002-2014 e-foto team (UERJ)
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
#include <math.h>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

Interpolation::Interpolation()
{
    // Mode:
    // 0 - Grayscale (0-255) in double format (0.0 - 1.0)
    // 1 - Color (24-bit RRGGBB) in double format (0.0 - 1.0)
    mode = 0;
}

double Interpolation::interpolate(Matrix* img,
                                  const double col,
                                  const double lin,
                                  const int method)
{
    // Codes:
    // 0 - Nearest Neighbor
    // 1 - Bilinear
    // 2 - Bicubic
    // 3- Lagrange
    if (method < 0 || method > 3) {
        return 0.0;
    }

    img_ = img;
    ccolor = 0;

    if (method == 0) {
        return interpolateNearestNeighbor(col, lin);
    }

    double pixel[3], fpixel;
    int rounds;
    (mode == 1) ? rounds = 3 : rounds = 1;

    for (int f = 0; f < rounds; f++) {
        (mode == 1) ? ccolor = f + 1 : ccolor = 0;

        switch (method) {
        case 1 :
            pixel[f] = interpolateBilinear(col, lin);
            break;

        case 2 :
            pixel[f] = interpolateBicubic(col, lin);
            break;

        case 3 :
            pixel[f] = interpolateLagrange(col, lin);
            break;
        }
    }

    fpixel = (mode == 1) ?
            static_cast<double>((static_cast<int>(pixel[0] * 255.0) << 16)
                              + (static_cast<int>(pixel[1] * 255.0) << 8)
                              + static_cast<int>(pixel[2] * 255.0)) / 0xFFFFFF
             : pixel[0];
    return fpixel;
}

/*
 * Nearest Neighbor
 **/

double Interpolation::interpolateNearestNeighbor(const double col,
                                                 const double lin)
{
    auto i = round(lin);
    auto j = round(col);

    if (!checkLimits(j, i)) {
        return 0.0;
    }

    return I(i, j);
}


/*
 * Bilinear
 **/

double Interpolation::interpolateBilinear(const double col, const double lin)
{
    auto i = floor(lin);
    auto j = floor(col);
    dx = col - floor(col);
    dy = lin - floor(lin);

    if (!checkLimits(j + 1, i + 1)) {
        return interpolateNearestNeighbor(j, i);
    }

    return (I(i, j)) * (1.0 - dx) * (1.0 - dy)
           + I(i + 1, j) * (1.0 - dx) * dy
           + I(i, j + 1) * dx * (1.0 - dy)
           + I(i + 1, j + 1) * dx * dy;
}

/*
 * Bicubic
 **/

double Interpolation::interpolateBicubic(const double col, const double lin)
{
    dx = col - floor(col);
    dy = lin - floor(lin);
    auto i = floor(lin);
    auto j = floor(col);

    if (!checkLimits(j + 2, i + 2) || !checkLimits(j - 1, i - 1)) {
        return interpolateNearestNeighbor(j, i);
    }

    double pixel = a(1,i,j) * df(dy + 1)
            + a(2,i,j) * df(dy)
            + a(3,i,j) * df(dy - 1)
            + a(4,i,j) * df(dy - 2);

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

double Interpolation::a(const int n, const int i, const int j)
{
    return df(dx + 1.0) * I(i - 1, j + n - 2)
           + I(i, j + n - 2) * df(dx)
           + I(i + 1, j + n - 2) * df(dx - 1.0)
           + I(i + 2, j + n - 2) * df(dx - 2.0);
}


/*
 * Lagrange
 **/

double Interpolation::interpolateLagrange(const double col, const double lin)
{
    dx = col - floor(col);
    dy = lin - floor(lin);
    auto i = floor(lin);
    auto j = floor(col);

    if (!checkLimits(j + 2, i + 2) || !checkLimits(j - 1, i - 1)) {
        return interpolateNearestNeighbor(j, i);
    }

    double pixel = aa(1,i,j) * (dy - 1.0) * (dy - 2.0) * (-dy / 6.0)
                   + aa(2,i,j) * (dy + 1.0) * (dy - 1.0) * ((dy - 2.0) / 2.0)
                   + aa(3,i,j) * (dy + 1.0) * (dy - 2.0) * (-dy / 2.0)
                   + aa(4,i,j) * (dy + 1.0) * (dy - 1.0) * (dy / 6.0);

    if (pixel > 1.0) {
        pixel = 1.0;
    }

    if (pixel < 0.0) {
        pixel = 0.0;
    }

    return pixel;
}

double Interpolation::aa(const int n, const int i, const int j)
{
    return I(i - 1, j + n - 2) * (dx - 1.0) * (dx - 2.0) * (-dx / 6.0)
            + I(i, j + n - 2) * (dx + 1.0) * (dx - 1.0) * ((dx - 2.0) / 2.0)
            + I(i + 1, j + n - 2) * (dx + 1.0) * (dx - 2.0) * (-dx / 2.0)
            + I(i + 2, j + n - 2) * (dx + 1.0) * (dx - 1.0) * (dx / 6.0);
}


/*
 * Read pixel
 **/

double Interpolation::I(const int i, const int j)
{
    if (ccolor == 0) {
        return img_->get(i, j);
    }

    // Get pixel from matrix
    int pixel = static_cast<int>(img_->get(i, j) * 0xFFFFFF);;

    // Red
    if (ccolor == 1) {
        pixel = (pixel & 0xFF0000) >> 16;
        return static_cast<double>(pixel) / 255.0;
    }

    // Green
    if (ccolor == 2) {
        pixel = (pixel & 0xFF00) >> 8;
        return static_cast<double>(pixel) / 255.0;
    }

    // Blue
    if (ccolor == 3) {
        pixel = pixel & 0xFF;
        return static_cast<double>(pixel) / 255.0;
    }

    return 0.0;
}

/*
 * Check limits
 **/

bool Interpolation::checkLimits(int col, int row)
{
    return (col >= 1 && row >= 1 && unsigned(col) <= img_->getCols()
            && unsigned(row) <= img_->getRows());
}

}  // namespace efoto
}  // namespace eng
}  // namespace uerj
}  // namespace br
