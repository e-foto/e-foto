#ifndef DEMFEATURES_H
#define DEMFEATURES_H
/*******************************************************************************
                              DemFeatures.h
*******************************************************************************/
/*Copyright 2002-2023 e-foto team (UERJ)
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

#include <ogr_spatialref.h>
#include <ogrsf_frmts.h>

#include <string>
#include <vector>

#include "CommonDef.h"
#include "GeoSystem.h"
#include "Matrix.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class MatchingPointsList;

class FeatureClass {
 public:
  std::string name;
  std::string description;
  int type;
  int outline_color, fill_color, fill_transparency;
  int line_type;
};

class DemFeaturePoints {
 public:
  DemFeaturePoints() { X = Y = Z = 0.0; };
  double X, Y, Z;
  double left_x, left_y, right_x, right_y;
};

class DemFeature {
 public:
  explicit DemFeature() {
    feature_class = feature_type = layer = 1;
    perimeter = area = 0.0;
    flag_calc_centroid = 0;
    is_on_screen = 0;
  };
  int feature_class, feature_type, layer, is_on_screen;
  double perimeter, area;
  std::string name, description;
  DemFeaturePoints centroid;
  bool flag_calc_centroid;
  std::vector<DemFeaturePoints> points;
};

class DemFeatures {
 private:
  int img_left_width, img_left_height, img_right_width, img_right_height;
  int loadFeatSp165(char *filename, bool append);
  int createShapefile(FeatureClass *fc, OGRSpatialReference *oSRS,
                      GDALDataset *poDS);
  bool hasFeatureClass(FeatureClass *fc);
  int exportShpFeatures(char *filename);
  int exportTxtFeatures(char *filename);
  int saveFeatSp165(char *filename);
  void convertClassesIdsFromSp165();
  void calculateFeaturesAttributes();
  void calculateFeatureAttributes(int featid);
  void calculateCentroid(int featid);
  void calculatePerimeter(int featid);
  void calculateArea(int featid);
  void checkAllIsOnScreen();
  void checkIsOnScreen(int featid);
  void addPolygonToMap(int feat_id, Matrix *map, double Xi, double Yi,
                       double res_x, double res_y);
  Matrix mapPolygon(int feat_id, double res_x, double res_y);
  double angle2D(double X1, double Y1, double X2, double Y2);
  int selected_feat, selected_pt;
  std::vector<DemFeature> features;
  std::vector<FeatureClass> feature_classes;
  Matrix polygonMap;
  GeoSystem GRS;
  int utmFuse;
  bool spheroid;

 public:
  DemFeatures();
  ~DemFeatures() { features.clear(); }
  void createClassesFromSp165();
  int convClassFromSp165(int ft, int fc);
  int getClassIdToSp165(int new_class);
  FeatureClass *getFeatureClass(int);
  std::string getFeatureTypeName(int);
  int loadFeatures(char *filename, bool append);
  int saveFeatures(char *filename);
  int exportFeatures(char *filename, int mode = 0);
  DemFeature getFeature(int feat);
  DemFeature *getFeatureLink(int feat);
  void showFeatures(bool full);
  void showClasses();
  int selectedFeature() { return selected_feat; }
  void setSelectedFeature(int _ft) { selected_feat = _ft; }
  int selectedPoint() { return selected_pt; }
  void setSelectedPoint(int _pt) { selected_pt = _pt; }
  int addNewFeature(std::string name, std::string fdesc, int fclass, int ftype,
                    int layer);
  void deleteAllFeatures() { features.clear(); }
  int deleteFeature(int featid);
  int copyFeature(int featid, double shift);
  void addNewPoint(int featid, int pointid, double X, double Y, double Z);
  void updatePoint(int featid, int pointid, double X, double Y, double Z);
  void update2DPoint(int featid, int pointid, double lx, double ly, double rx,
                     double ry);
  int deletePoint(int featid, int pointid);
  void setName(int featid, std::string _name);
  std::string getName(int featid);
  void setDescription(int featid, std::string _desc);
  std::string getDescription(int featid);
  void setFeatureClassId(int featid, int fclass);
  int getFeatureClassId(int featid);
  void setFeatureType(int featid, int ftype);
  int getFeatureType(int featid);
  void setLayer(int featid, int layer);
  int getLayer(int featid);
  int getNumPoints(int featid);
  int getNumFeatures() { return features.size(); }
  int addFeatureClass(std::string name, std::string description, int ccolor,
                      int fcolor);       // Return class id
  int addFeatureClass(FeatureClass fc);  // Return class id
  int getNearestFeature(double X, double Y, double Z);
  int getNearestPoint(int fid, double X, double Y, double Z);
  void getNearestPoint(double X, double Y, double Z, int &fid, int &pid);
  void addFeaturesToPairList(MatchingPointsList *mpl, bool usePolygons);
  Matrix createPolygonMap(double Xi, double Yi, double Xf, double Yf,
                          double res_x, double res_y);
  bool isInside(int feat_id, double X, double Y);
  void calculateBoundingBox(int feat_id, double &Xi, double &Yi, double &Xf,
                            double &Yf);
  double interpolateXYPolygon(int feat_id, double X, double Y, double D0);
  std::string getFeaturesList();
  std::string getFeaturesToDisplay(int mode = 0);
  void setImagePairSize(int lw, int lh, int rw, int rh);
  void setGRS(std::string _gs) { GRS = GeoSystem(_gs); }
  void setUtmFuse(int _fuse) { utmFuse = _fuse; }
  void setHemisphere(bool _hs) { spheroid = _hs; }
};

}  // namespace efoto
}  // namespace eng
}  // namespace uerj
}  // namespace br

#endif
