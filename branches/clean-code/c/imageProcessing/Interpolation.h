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


#ifndef INTERPOLATION_H
#define INTERPOLATION_H

/**
* class ProjectiveRay
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 14/12/2011
* @version 1.0 - Marcelo Teixeira Silveira
*/

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Matrix;

class Interpolation {
 public:
    Interpolation();
    double interpolate(Matrix* _img, double col, double lin, int method);
    void setMode(int _mode)
    {
        mode = _mode % 2;
    };

 private:
    double dx, dy;
    int mode, ccolor;
    Matrix* img_;

    double interpolateNearestNeighbor(double col, double lin) const;
    double interpolateBilinear(double col, double lin);
    double interpolateBicubic(double col, double lin);
    static double df(double x);
    double a(int,int,int);
    double interpolateLagrange(double col, double lin);
    double aa(int n, int i, int j) const;
    bool checkLimits(int col, int row) const;
    double I(int i, int j) const;
};

}  // namespace efoto
}  // namespace eng
}  // namespace uerj
}  // namespace br

#endif
