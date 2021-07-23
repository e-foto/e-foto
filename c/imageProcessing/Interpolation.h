#ifndef INTERPOLATION_H
#define INTERPOLATION_H
/*******************************************************************************
		  interpolation.h
*******************************************************************************/
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

#include "CommonDef.h"

/**
* class ProjectiveRay
*/

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Matrix;

// Grayscale (0-255) in double format (0.0 - 1.0)
// Color (24-bit RRGGBB) in double format (0.0 - 1.0)
enum class ColorMode {Grayscale, Color};
enum class InterpMethode {nearestNeighbor, bilinear, bicubic, lagrange};
enum class Color {gray, red, green, blue};


class Interpolation
{
public:
	Interpolation();
	double interpolate(Matrix *_img, double col, double lin, InterpMethode method);
	void setMode(ColorMode _mode) { mode = _mode; };

private:
	double interpolateNearestNeighbor(double col, double lin);
	double interpolateBilinear(double col, double lin);
	double interpolateBicubic(double col, double lin);
	double df(double x);
	double a(int n);
	double interpolateLagrange(double col, double lin);
	double aa(int n);
	bool checkLimits(int col, int row) const;
	double dx{0.0};
	double dy{0.0};
	Color ccolor{Color::gray};
	int i, j;
	ColorMode mode{ColorMode::Grayscale};
	Matrix *img;
	double I(int i, int j) const;

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif

