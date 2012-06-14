/**************************************************************************
														   SPManager.h
**************************************************************************/


#ifndef SPMANAGER_H
#define SPMANAGER_H

#include "EObject.h"
#include "Features.h"
#include <deque>

#ifdef INTEGRATED_EFOTO
#include "Image.h"
#include "ExteriorOrientation.h"
#include "SpatialIntersection.h"
#endif //INTEGRATED_EFOTO REVER!

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class SPUserInterface;
class EFotoManager;

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
	Features spFeatures;
	int leftKey, rightKey;

	void getImagesId(int pos, int &left, int &right);
	void addPairsToInterface();
	int getPairs();
	bool checkAnglesAlligned(double angle1, double angle2, double tolerance);
	double getAngleBetweenImages(double X1, double Y1, double X2, double Y2);
	double fixAngle(double angle);
	deque<int> listPairs;

	SPUserInterface* myInterface;
	EFotoManager* manager;

#ifdef INTEGRATED_EFOTO
	deque<Image*> listAllImages;
	deque<Point*> listAllPoints;
	deque<ExteriorOrientation*> listEOs;

	Image* leftImage;
	Image* rightImage;
	ProjectiveRay prL;
	ProjectiveRay prR;
#endif //INTEGRATED_EFOTO REVER!

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
	~SPManager();


#ifdef INTEGRATED_EFOTO
    SPManager(EFotoManager* manager, deque<Image*> images, deque<ExteriorOrientation*> eos);
    ObjectSpaceCoordinate getBoundingBoxCenter();
    ObjectSpaceCoordinate getCentralPoint();
    ImageSpaceCoordinate getLeftPoint(ObjectSpaceCoordinate coord);
    ImageSpaceCoordinate getRightPoint(ObjectSpaceCoordinate coord);
#endif //INTEGRATED_EFOTO REVER!

	// Association Methods
	//
	void setInterface(SPUserInterface* newInterface);
	SPUserInterface* getInterface();

	// Other Methods
	//
	bool exec();
	void returnProject();
	int loadFeatures(char *filename);
	void saveFeatures(char *filename);
	void addFeature(string name, int feattype, int featclass);
	int removeFeature();
	void removeAllFeatures();
	int removePoint();
	void setSelected(int feat_id, int pt_id);
    void getSelected(int &fid, int &pid) { fid = spFeatures.selectedFeature(); pid = spFeatures.selectedPoint(); }
	Features* getFeaturesLink() { return &spFeatures; }
	string getFeaturesList() { return spFeatures.getFeaturesList(); }
	void updateProjections();
	void computeIntersection(double xl, double yl, double xr, double yr, double& X, double& Y, double& Z);
	string getFullImagePath(int imagekey);
	void getFeatureData(string &fname, int &ftype, int &fclass);
	void changePair(int pair, int &lk, int &rk);
	void addPoint(int fid, int pid, double lx, double ly, double rx, double ry, double X, double Y, double Z);
	void updatePoint(int fid, int pid, double lx, double ly, double rx, double ry, double X, double Y, double Z);
    void setSelectedXYZ(double X, double Y, double Z);
    double getBoundingBoxIdealZoom(int width, int height);

	/**
	* \brief Registra no XML o endereço de um arquivo de geometrias.
	*/
	void addGeometryToXML(string filename);


};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //SPMANAGER_H
