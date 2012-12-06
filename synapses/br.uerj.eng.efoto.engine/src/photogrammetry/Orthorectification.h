/*******************************************************************************
							  Orthorectification.h
*******************************************************************************/

#ifndef ORTHORECTIFICATION_H
#define ORTHORECTIFICATION_H

#include <stdio.h>
#include <iostream>
#include <fstream>
#include "Matrix.h"
#include <terralib/kernel/TeRaster.h>
#include <terralib/kernel/TeProjection.h>


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
        //int datum, coord_system;   --Tipo alterado para std::string em 24/09/2012 para atender as necessidades para geração do GeoTiff via TerraLib
        int color_depth, no_bands, spheroid, coord_system, datum, utmFuse;
        //std::string datum, coord_system;
	int ortho_width, ortho_height;
	void saveOrthoEfoto(char * filename);
	void loadOrthoEfoto(char * filename);
        void saveOrthoGeoTiffEfoto(char * filename);

public:
	Orthorectification(double, double, double, double, double, double);
	void createNewGrid(double, double, double, double, double, double);
	void changeGridResolution(double, double);
	void saveOrtho(char *, int);
        void saveOrthoGeoTiff(char *, int);
	void loadOrtho(char *, int);
	void printData();
	Matrix getOrthoData() { return orthoimage; }; // Read only
	Matrix * getOrthoImage();
	int getWidth() { return ortho_width; };
	int getHeight() { return ortho_height; };
	double getOrthoimagePixel(int, int);
	double getOrthoimagePixel(double, double);
	void setOrthoimagePixel(int, int, double);
	void setOrthoimagePixel(double, double, double);
	void getXYAt(int, int, double &, double &);
	double getGridResX() { return res_x; };
	double getGridResY() { return res_y; };
	void setColorDepth(int _cd) { color_depth = _cd; };
	int getColorDepth() { return color_depth; };
	void setNumberOfBands(int _nb) { no_bands = _nb; };
	int getNumberOfBands() { return no_bands; };
	void setCoordinateSystem(int _cs) { coord_system = _cs; };
	int getCoordinateSystem() { return coord_system; };
	void setSpheroid(int _sp) { spheroid = _sp; };
	int getSpheroid() { return spheroid; };
	void setDatum(int _dt) { datum = _dt; };
	int getDatum() { return datum; };
        void setUtmFuse(int _uf) { utmFuse = _uf; };
        int getUtmFuse() { return utmFuse; };
	void getOrthoParametersA(double &_Xi, double &_Yi, double &_Xf, double &_Yf, double &_res_x, double &_res_y) { _Xi = Xi; _Yi = Yi; _Xf = Xf; _Yf = Yf; _res_x = res_x; _res_y = res_y; };
	void getOrthoParametersB(int &_color, int &_bands, int &_coord, int &_sph, int &_datum) { _color = color_depth; _bands = no_bands; _coord = coord_system; _sph = spheroid; _datum = datum; };

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif
