/******************************************************************************
                                DemGrid.h
*******************************************************************************/

#ifndef DEMGRID_H
#define DEMGRID_H


/**
* class DemGrid
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 11/10/2011
* @version 1.0 - Marcelo Teixeira Silveira
*/

#include <stdio.h>
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "MatchingPoints.h"
#include "MatchingPointsGrid.h"
#include "Matrix.h"

using namespace std;

class DEMManager;

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
        void getMinMax(double &, double &); // Return Min Z and Max Z
        double getMeanZ();
	double getHeightXY(double X, double Y);
	double getHeight(double col, double row);
	Matrix *getDemImage(double min=0.0, double max=0.0);
	Matrix &getDem();
        int getWidth() { return dem_width; };
        int getHeight() { return dem_height; };
        void printData();
        void getXYAt(int, int, double &, double &);
        void getDemParameters(double &_Xi, double &_Yi, double &_Xf, double &_Yf, double &_res_x, double &_res_y) { _Xi = Xi; _Yi = Yi; _Xf = Xf; _Yf = Yf; _res_x = res_x; _res_y = res_y; };

private:
	Matrix DEM;
	double Xi, Yi, Xf, Yf, res_x, res_y;
	double elap_time;
	int dem_width, dem_height;
	MatchingPointsList *point_list;
	MatchingPointsGrid *mpg;
	void saveDemEfoto(char *);
	void loadDemEfoto(char *);
	void saveDemAscii(char *);
	double getAsciiParameter(ifstream *, string);
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

        int chooseBestInterpolationMathod();
};

#endif



