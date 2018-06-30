#ifndef DEMFEATURES_H
#define DEMFEATURES_H
/*******************************************************************************
                              DemFeatures.h
*******************************************************************************/
/*Copyright 2002- e-foto team (UERJ)
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

#include <string>
#include <vector>
#include "Matrix.h"
#include "shapefil.h"

/**
* class DemFeatures
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 21/11/2011
* @version 1.0 - Marcelo Teixeira Silveira
*/

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class  MatchingPointsList;

// Feature Class
class FeatureClass {
  public:
    std::string name;
    std::string description;
    int type;
    int outline_color, fill_color, fill_transparency;
    int line_type;
};

// One point
class DemFeaturePoints {
  public:
    DemFeaturePoints():
        X{0.0},
        Y{0.0},
        Z{0.0},
        left_x{0.0},
        left_y{0.0},
        right_x{0.0},
        right_y{0.0} {
    };
    double X, Y, Z;
    double left_x, left_y, right_x, right_y;
};

// One feature
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
    std::vector <DemFeaturePoints> points;
};

// All features
class DemFeatures {
  public:
    DemFeatures();
    void createClassesFromSp165();
    static int convClassFromSp165(int ft, int fc);
    static int getClassIdToSp165(int new_class);
    FeatureClass* getFeatureClass(int);
    static std::string getFeatureTypeName(int);
    int loadFeatures(char* filename, bool append);
    int saveFeatures(char* filename);
    int exportFeatures(char* filename, int mode = 0);
    DemFeature getFeature(int feat); // Read only
    DemFeature* getFeatureLink(int feat);
    void showFeatures(bool full);
    int selectedFeature() const {
        return selected_feat;
    }
    void setSelectedFeature(int _ft) {
        selected_feat = _ft;
    }
    int selectedPoint() const {
        return selected_pt;
    }
    void setSelectedPoint(int _pt) {
        selected_pt = _pt;
    }
    int addNewFeature(std::string name, std::string fdesc, int fclass, int ftype,
                      int layer); // Returns feature id
    void deleteAllFeatures() {
        features.clear();
    }
    int deleteFeature(int featid);
    int copyFeature(int featid, double shift);
    void addNewPoint(int featid, int pointid, double X, double Y,
                     double Z); // Returns point id
    void updatePoint(int featid, int pointid, double X, double Y, double Z);
    void update2DPoint(int featid, int pointid, double lx, double ly, double rx,
                       double ry);
    int deletePoint(int featid, int pointid);
    void setName(int featid, std::string _name);
    std::string getName(int featid);
    void setDescription(int featid, std::string _desc);
    std::string getDescription(int featid);
    int getFeatureClassId(int featid);
    int getNumPoints(int featid);
    int getNumFeatures() const {
        return features.size();
    }
    int addFeatureClass(std::string name, std::string description, int ccolor,
                        int fcolor); // Return class id
    int addFeatureClass(FeatureClass fc); // Return class id
    int getNearestPoint(int fid, double X, double Y, double Z);
    void getNearestPoint(double X, double Y, double Z, int& fid, int& pid);
    void addFeaturesToPairList(MatchingPointsList* mpl, bool usePolygons);
    Matrix createPolygonMap(double Xi, double Yi, double Xf, double Yf,
                            double res_x, double res_y);
    bool isInside(int feat_id, double X, double Y);
    void calculateBoundingBox(int feat_id, double& Xi, double& Yi, double& Xf,
                              double& Yf);
    double interpolateXYPolygon(int feat_id, double X, double Y, double D0);
    std::string getFeaturesList();

  private:
    int img_left_width{ -1}, img_left_height{ -1}, img_right_width{ -1},
        img_right_height{ -1};
    int selected_feat{ -1}, selected_pt{ -1};
    std::vector <DemFeature> features;
    std::vector <FeatureClass> feature_classes;
    Matrix polygonMap;

    int loadFeatSp165(char* filename, bool append);
    int exportShpFeatures(char* filename);
    int exportTxtFeatures(char* filename);
    int saveFeatSp165(char* filename/*, bool append*/);
    void convertClassesIdsFromSp165();
    void calculateFeaturesAttributes();
    void calculateFeatureAttributes(int featid);
    void calculateCentroid(int featid);
    void calculatePerimeter(int featid);
    void calculateArea(int featid);
    void checkAllIsOnScreen();
    void checkIsOnScreen(int featid);
    void addPolygonToMap(int feat_id, Matrix* map, double Xi, double Yi,
                         double res_x, double res_y);
    Matrix mapPolygon(int feat_id, double res_x, double res_y);
    static double angle2D(double X1, double Y1, double X2, double Y2);
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif
