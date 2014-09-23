#ifndef DEMGRID_H
#define DEMGRID_H
/******************************************************************************
								DemGrid.h
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

/**
* class DemGrid
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 11/10/2011
* @version 1.0 - Marcelo Teixeira Silveira
*/

#include <string>
#include "MatchingPoints.h"
#include "Matrix.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class DEMManager;
class MatchingPointsGrid;

class DemGrid
{

public:
	DemGrid(double, double, double, double, double, double);
	void linkManager(DEMManager *_man) { manager = _man; };
	enum saveMode { efotoDEM, ascii };
	void saveDem(char *, int);
	void loadDem(char *, int);
	void createNewGrid(double, double, double, double, double, double);
	void changeGridResolution(double, double);
	void setPointList(MatchingPointsList *);
	void interpolateNearestPoint();
	void interpolateMovingAverage(double, double, int);
	void interpolateTrendSurface(int);
	void interpolateMovingSurface(double, double, int, int);
	double getElapsedTime() { return elap_time; };
	void cutGrid(double min, double max, bool fromList);
        void eliminateBadPointsGrid(double sigma);
	void getMinMax(double &, double &); // Return Min Z and Max Z
	double getMeanZ();
        double getStdZ();
	double getHeightXY(double X, double Y);
        double getHeight(double row, double col);
	Matrix *getDemImage(double min=0.0, double max=0.0);
	Matrix &getDem();
	int getWidth() { return dem_width; };
	int getHeight() { return dem_height; };
	void printData();
        void getXYAt(int col, int row, double &X, double &Y);
        void getXYAt(double col, double row, double &X, double &Y);
        void getColRowAt(double X, double Y, int &col, int &row);
        void getColRowAt(double X, double Y, double &col, double &row);
	void getDemParameters(double &_Xi, double &_Yi, double &_Xf, double &_Yf, double &_res_x, double &_res_y) { _Xi = Xi; _Yi = Yi; _Xf = Xf; _Yf = Yf; _res_x = res_x; _res_y = res_y; };
    std::string calculateDemQuality(MatchingPointsList mpl);
	void overlayMap(Matrix * map);
        void setCancel() { cancel_flag = true; };

private:
	Matrix DEM;
        bool cancel_flag;
	double Xi, Yi, Xf, Yf, res_x, res_y;
	double elap_time;
	int dem_width, dem_height;
	MatchingPointsList *point_list;
	MatchingPointsGrid *mpg;
	void saveDemEfoto(char *);
	void loadDemEfoto(char *);
	void saveDemAscii(char *);
    double getAsciiParameter(std::ifstream *, std::string);
	void loadDemAscii(char *);
	DEMManager* manager;

	// Used with a lot of pooints
	void interpolateNearestPointFast();
	void interpolateMovingAverageFast(double, double, int);
	void interpolateTrendSurfaceFast(int);
	void interpolateMovingSurfaceFast(double, double, int, int);

	// Used witha few points
	void interpolateNearestPointNormal();
	void interpolateMovingAverageNormal(double, double, int);
	void interpolateMovingSurfaceNormal(double, double, int, int);

        int chooseBestInterpolationMethod(double nf);
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif



