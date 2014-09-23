#ifndef SPMANAGER_H
#define SPMANAGER_H
/**************************************************************************
														   SPManager.h
**************************************************************************/
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

#include "DemFeatures.h"
#include "ProjectiveRay.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class SPUserInterface;
class EFotoManager;
class Point;
class ExteriorOrientation;

class SPManager
{
	// Private Attributes
	//
	int rad_cor, match_method, rgx, rgy, lsm_temp, lsm_it, lsm_dist, ncc_temp, ncc_sw;
	double lsm_th, lsm_std, lsm_shift, lsm_shear, lsm_scale, ncc_th, ncc_std, downsample;
	bool started;
	bool status;
	bool over_it;
	double over_it_dist;
	DemFeatures spFeatures;
	int leftKey, rightKey;

	void getImagesId(int pos, int &left, int &right);
	void addPairsToInterface();
	int getPairs();
	bool checkAnglesAlligned(double angle1, double angle2, double tolerance);
	double getAngleBetweenImages(double X1, double Y1, double X2, double Y2);
	double fixAngle(double angle);
        bool existPair(int &id1, int &id2);
        void setListPoint();
        double getTerrainMeanAltitude();
    std::deque<int> listPairs;

	SPUserInterface* myInterface;
	EFotoManager* manager;
    std::deque<Image*> listAllImages;
    std::deque<Point*> listAllPoints;
    std::deque<ExteriorOrientation*> listEOs;

        Image* leftImage;
        Image* rightImage;
        ProjectiveRay prL;
        ProjectiveRay prR;

	/*
		Image * getImage(int);
		void setListPoint();
		bool connectImagePoints();
		MatchingPointsList seeds, pairs;
		void getImagesId(int, int&, int&);
		void resamplePoints(MatchingPointsList *list, double resample);
		void createInitialSeeds();
		void extractDEMPair(int);
		void calcPointsXYZ();
		DemGrid *grid;
		StereoPair sp;
		*/

public:

	// Constructors and Destructors
	//
	SPManager();
    SPManager(EFotoManager* manager, std::deque<Image*> images, std::deque<ExteriorOrientation*> eos);
	~SPManager();

	// Association Methods
	//
	void setInterface(SPUserInterface* newInterface);
	SPUserInterface* getInterface();

	// Other Methods
	//
	bool exec();
	void returnProject();
        int loadFeatures(char *filename, bool append);
	void saveFeatures(char *filename);
        void exportFeatures(char *filename);
    void addFeature(std::string name, int feattype, int featclass);
	int removeFeature();
	void removeAllFeatures();
    bool removePoint();
    void setSelected(int feat_id, int pt_id);
	void getSelected(int &fid, int &pid) { fid = spFeatures.selectedFeature(); pid = spFeatures.selectedPoint(); };
	DemFeatures* getFeaturesLink() { return &spFeatures; }
    std::string getFeaturesList() { return spFeatures.getFeaturesList(); }
	void updateProjections();
	void computeIntersection(double xl, double yl, double xr, double yr, double& X, double& Y, double& Z);
    std::string getFullImagePath(int imagekey);
    void getSelectedFeatureData(int &sel_feat, std::string &fname, int &ftype, int &fclass, int &no_points, double &perimeter, double &area);
	void changePair(int pair, int &lk, int &rk);
	void addPoint(int fid, int pid, double lx, double ly, double rx, double ry, double X, double Y, double Z);
    void updatePoint(int fid, int pid, double lx, double ly, double rx, double ry, double X, double Y, double Z);
    void setSelectedXYZ(double X, double Y, double Z);
    int getNumFeatures() { return spFeatures.getNumFeatures(); };
    ObjectSpaceCoordinate getBoundingBoxCenter();
    double getBoundingBoxIdealZoom(int width, int height);
    ObjectSpaceCoordinate getCentralPoint();
    ImageSpaceCoordinate getLeftPoint(ObjectSpaceCoordinate coord);
    ImageSpaceCoordinate getRightPoint(ObjectSpaceCoordinate coord);

    /**
    * \brief Registra no XML o endereço de um arquivo de geometrias.
    */
    void addGeometryToXML(std::string filename);


};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //SPMANAGER_H
