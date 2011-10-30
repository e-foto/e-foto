/******************************************************************************
                           MatchingPointsGrid.h
*******************************************************************************/

/*
 * This class was designed to increase the performance to find point close to a given point.
 * The method used in E-FOTO DEM module, version 2.6, performe an exhaustive search for points,
 * in On^2 order.
 */

#ifndef MATCHINGPOINTSGRID_H
#define MATCHINGPOINTSGRID_H


/**
* class MatchingPointsGrid
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 10/10/2011
* @version 1.0 - Marcelo Teixeira Silveira
*/

#include <stdio.h>
#include <math.h>
#include <vector>
#include "MatchingPoints.h"

using namespace std;

class MPGCell
{
public:
	vector <int> points;

};

class MatchingPointsGrid
{
public:
	MatchingPointsGrid(MatchingPointsList *, double, double);
	void createNewGrid(double, double);
	int getNearestPoint(double, double);
	void getPointsClose(double, double, double);
	vector <int> selected_points;

private:
	int width, height;
	double Xi, Xf, Yi, Yf;
	double res_x, res_y;
	vector <MPGCell> gridMap;
	int no_inside_points, no_outside_points;
	MatchingPointsList *point_list;
	void createStructure();

};

#endif
