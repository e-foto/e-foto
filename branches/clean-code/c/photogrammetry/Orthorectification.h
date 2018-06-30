#ifndef ORTHORECTIFICATION_H
#define ORTHORECTIFICATION_H
/*******************************************************************************
                              Orthorectification.h
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

#include <memory>
#include "Matrix.h"
#include "CommonDef.h"

/**
* class Orthorectification
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 24/11/2011
* @version 1.0 - Marcelo Teixeira Silveira
*/

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Orthorectification{

private:
    Matrix orthoimage;
    double Xi, Yi, Xf, Yf, res_x, res_y;
    int color_depth {8};
    int no_bands {1};
    int coord_system {0};
    int spheroid {0};
    int utmFuse  {SAD69};
    DatumType datum;
    int ortho_width, ortho_height;
    void saveOrthoEfoto(char * filename) const;
    void loadOrthoEfoto(char * filename);
    void saveOrthoGeoTiffEfoto(char * filename) const;
    void setOrthoimagePixel(int, int, double) const;

public:
    explicit Orthorectification(double, double, double, double, double, double);
    void createNewGrid(double, double, double, double, double, double);
    // todo: solve unused parameter mode
    void saveOrtho(char *) const;
    void saveOrthoGeoTiff(char *) const;
    void loadOrtho(char *);
    Matrix * getOrthoImage();
    int getHeight() const { return ortho_height; }
    void setOrthoimagePixel(double, double, double) const;
    void getXYAt(double col, double row, double &X, double &Y) const;
    void getColRowAt(double X, double Y, double &col, double &row) const;
    double getGridResX() const { return res_x; }
    double getGridResY() const { return res_y; }
    void setUtmFuse(int _uf) { utmFuse = _uf; };
    void setNumberOfBands(int _nb) { no_bands = _nb; }
    void setCoordinateSystem(int _cs) { coord_system = _cs; }
    void setDatum(DatumType _dt) { datum = _dt; }
    void getOrthoParametersA(double &_Xi,
                             double &_Yi,
                             double &_Xf,
                             double &_Yf,
                             double &_res_x,
                             double &_res_y) const
    { _Xi = Xi; _Yi = Yi; _Xf = Xf; _Yf = Yf; _res_x = res_x; _res_y = res_y; }
};

}  // namespace efoto
}  // namespace eng
}  // namespace uerj
}  // namespace br

#endif
