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
	void interpolateNearestPoint();
	void interpolateMovingAverage(double, double, int);
	void interpolateTrendSurface(int);
	void interpolateMovingSurface(double, double, int, int);
	void setPointList(MatchingPointsList *);
	double getElapsedTime() { return elap_time; };
	void cutGrid(double min, double max, bool fromList);
	void getMinMax(double &, double &);
	double getHeightXY(double X, double Y);
	double getHeight(double col, double row);
	Matrix *getDemImage(double min=0.0, double max=0.0);
	Matrix &getDem();
        int getWidth() { return dem_width; };
        int getHeight() { return dem_height; };
        void printData();

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

};

#endif



