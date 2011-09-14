/******************************************************************************
                           MatchingPoints.h
*******************************************************************************/


#ifndef MATCHINGPOINTS_H
#define MATCHINGPOINTS_H

/**
* class MatchingPoints
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 15/08/2011
* @version 1.0 - Marcelo Teixeira Silveira
*/

#include <stdio.h>
#include <math.h>
#include <vector>

using namespace std;

class MatchingPoints
{
public:
	MatchingPoints();
	int left_image_id, right_image_id;
	double left_x, left_y, right_x, right_y;
	double X, Y, Z;
	double matching_accuracy;

private:

};

class MatchingPointsList
{
public:
	MatchingPointsList() { bad_points = 0; };
	enum saveType { saveFull, savePairs, savePairsHann, saveXYZ };
	enum loadType { loadFull, loadPairs, loadPairsHann, loadXYZ };
	void clear();
	void add(int, int, double, double, double, double, double);
	void modify(int, int, int, double, double, double, double, double);
	void modify(int, double, double, double);
	void remove(int);
	void filterBadPoints2D(double sigma_x=3, double sigma_y=1.5);
	void listMp();
	void listMp(int);
	MatchingPoints* get(int);
	int save(char *, saveType);
	int load(char *, loadType, bool append=false, int left_id=-1, int right_id=-1);
	int size() { return list.size(); };
	unsigned int getNumBadPoints() { return bad_points; };

private:
	vector <MatchingPoints> list;
	unsigned int bad_points;
};

#endif

