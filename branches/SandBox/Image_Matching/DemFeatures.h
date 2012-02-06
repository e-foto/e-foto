/*******************************************************************************
							  DemFeatures.h
*******************************************************************************/

#ifndef DEMFEATURES_H
#define DEMFEATURES_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "MatchingPoints.h"
#include "Matrix.h"

/**
* class DemFeatures
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 21/11/2011
* @version 1.0 - Marcelo Teixeira Silveira
*/

using namespace std;

// Feature Class
class FeatureClass
{
public:
	string name;
	string description;
	int type;
	int outline_color, fill_color, fill_transparency;
	int line_type;
};

// One point
class DemFeaturePoints
{
public:
	DemFeaturePoints() { X = Y = Z = 0.0; };
	double X, Y, Z;
        double left_x, left_y, right_x, right_y;
};

// One feature
class DemFeature
{
public:
        DemFeature() { feature_class = feature_type = layer = 1; perimeter = area = 0.0; flag_calc_centroid = 0; is_on_screen = 0; };
        int feature_class, feature_type, layer, is_on_screen;
	double perimeter, area;
	string name, description;
	DemFeaturePoints centroid;
        bool flag_calc_centroid;
	vector <DemFeaturePoints> points;
};

// All features
class DemFeatures
{
private:
        int img_left_width, img_left_height, img_right_width, img_right_height;
	int loadFeatSp165(char *filename, bool append);
        int saveFeatSp165(char *filename, bool append);
        void convertClassesIdsFromSp165();
        int getClassIdToSp165(int new_class);
	void createClassesFromSp165();
	void calculateFeaturesAttributes();
	void calculateFeatureAttributes(int featid);
	void calculateCentroid(int featid);
	void calculatePerimeter(int featid);
	void calculateArea(int featid);
        void checkAllIsOnScreen();
        void checkIsOnScreen(int featid);
	void addPolygonToMap(int feat_id, Matrix *map, double Xi, double Yi, double res_x, double res_y);
	Matrix mapPolygon(int feat_id, double res_x, double res_y);
	double angle2D(double X1, double Y1, double X2, double Y2);
	int selected_feat, selected_pt;
	vector <DemFeature> features;
	vector <FeatureClass> feature_classes;
	Matrix polygonMap;

public:
	DemFeatures();
	FeatureClass * getFeatureClass(int);
	string getFeatureTypeName(int);
	int loadFeatures(char *filename, int mode, bool append);
        int saveFeatures(char *filename, int mode, bool append);
	DemFeature getFeature(int feat); // Read only
	void showFeatures(bool full);
	void showClasses();
	int selectedFeature() { return selected_feat; };
	void setSelectedFeature(int _ft) { selected_feat = _ft; };
	int selectedPoint() { return selected_pt; };
	void setSelectedPoint(int _pt) { selected_pt = _pt; };
	int addNewFeature(string name, string fdesc, int fclass, int ftype, int layer); // Returns feature id
	int deleteFeature(int featid);
	int copyFeature(int featid, double shift);
	void addNewPoint(int featid, int pointid, double X, double Y, double Z); // Returns point id
	void updatePoint(int featid, int pointid, double X, double Y, double Z);
        void update2DPoint(int featid, int pointid, double lx, double ly, double rx, double ry);
	int deletePoint(int featid, int pointid);
	void setName(int featid, string _name);
	string getName(int featid);
	void setDescription(int featid, string _desc);
	string getDescription(int featid);
	void setFeatureClassId(int featid, int fclass);
	int getFeatureClassId(int featid);
	void setFeatureType(int featid, int ftype);
	int getFeatureType(int featid);
	void setLayer(int featid, int layer);
	int getLayer(int featid);
	int getNumPoints(int featid);
	int getNumFeatures() { return features.size(); };
	int addFeatureClass(string name, string description, int ccolor, int fcolor); // Return class id
	int addFeatureClass(FeatureClass fc); // Return class id
	void setFeatureClass(int classid, FeatureClass fc);
	int getNearestFeature(double X, double Y, double Z);
	void addFeaturesToPairList(MatchingPointsList *mpl, bool usePolygons);
	Matrix createPolygonMap(double Xi, double Yi, double Xf, double Yf, double res_x, double res_y);
	bool isInside(int feat_id, double X, double Y);
	void calculateBoundingBox(int feat_id, double &Xi, double &Yi, double &Xf, double &Yf);
	double interpolateXYPolygon(int feat_id, double X, double Y, double D0);
        string getFeaturesList();
        string getFeaturesToDisplay(int mode=0);
        void setImagePairSize(int lw, int lh, int rw, int rh);
};

#endif
