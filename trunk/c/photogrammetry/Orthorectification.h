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

class Orthorectification
{

private:
    Matrix orthoimage;
    double Xi, Yi, Xf, Yf, res_x, res_y;
    int color_depth, no_bands, coord_system, spheroid, utmFuse;
    DatumType datum;
    int ortho_width, ortho_height;
    void saveOrthoEfoto(char * filename);
    void loadOrthoEfoto(char * filename);
    void saveOrthoGeoTiffEfoto(char * filename);
    void changeGridResolution(double, double);
    void printData();
    Matrix getOrthoData() { return orthoimage; } // Read only
    int getWidth() { return ortho_width; }
    double getOrthoimagePixel(int, int);
    double getOrthoimagePixel(double, double);
    void setColorDepth(int _cd) { color_depth = _cd; }
    int getColorDepth() { return color_depth; }
    int getNumberOfBands() { return no_bands; }
    int getCoordinateSystem() { return coord_system; }
    void setSpheroid(int _sp) { spheroid = _sp; }
    int getSpheroid() { return spheroid; }
    int getDatum() { return datum; }
    int getUtmFuse() { return utmFuse; };
    void getOrthoParametersB(int &_color, int &_bands, int &_coord, int &_sph, int &_datum) { _color = color_depth; _bands = no_bands; _coord = coord_system; _sph = spheroid; _datum = datum; }
    void setOrthoimagePixel(int, int, double);
    void getXYAt(int col, int row, double &X, double &Y);
    void getColRowAt(double X, double Y, int &col, int &row);

public:
    explicit Orthorectification(double, double, double, double, double, double);
    void createNewGrid(double, double, double, double, double, double);
    // todo: solve unused parameter mode
    void saveOrtho(char */*, int mode*/);
    void saveOrthoGeoTiff(char *, int);
    void loadOrtho(char */*, int mode*/);
    Matrix * getOrthoImage();
    int getHeight() { return ortho_height; }
    void setOrthoimagePixel(double, double, double);
    void getXYAt(double col, double row, double &X, double &Y);
    void getColRowAt(double X, double Y, double &col, double &row);
    double getGridResX() { return res_x; }
    double getGridResY() { return res_y; }
    void setUtmFuse(int _uf) { utmFuse = _uf; };
    void setNumberOfBands(int _nb) { no_bands = _nb; }
    void setCoordinateSystem(int _cs) { coord_system = _cs; }
    void setDatum(DatumType _dt) { datum = _dt; }
    void getOrthoParametersA(double &_Xi, double &_Yi, double &_Xf, double &_Yf, double &_res_x, double &_res_y) { _Xi = Xi; _Yi = Yi; _Xf = Xf; _Yf = Yf; _res_x = res_x; _res_y = res_y; }

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif
