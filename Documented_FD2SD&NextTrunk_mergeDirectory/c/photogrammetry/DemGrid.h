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

class DemGrid
{

public:
	DemGrid(double, double, double, double, double, double);
	enum saveMode { efotoDEM, ascii };
	void saveDem(char *, saveMode);
	void loadDem(char *, saveMode);
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

private:
	Matrix DEM;
	double Xi, Yi, Xf, Yf, res_x, res_y;
	double elap_time;
	int dem_width, dem_height;
	MatchingPointsList *point_list;
	MatchingPointsGrid *mpg;
	void printData();
	void saveDemEfoto(char *);
	void loadDemEfoto(char *);
	void saveDemAscii(char *);
	double getAsciiParameter(ifstream *, string);
	void loadDemAscii(char *);

};

#endif



