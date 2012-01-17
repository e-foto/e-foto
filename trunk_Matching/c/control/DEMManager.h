/**************************************************************************
                                                           DEMManager.h
**************************************************************************/


#ifndef DEMMANAGER_H
#define DEMMANAGER_H

#include "EObject.h"
#include <deque>
#include "Image.h"
#include "ExteriorOrientation.h"
#include "Matrix.h"
#include "ImageMatching.h"
#include "MatchingPoints.h"
#include "DemGrid.h"
#include "SpatialIntersection.h"
#include "DemFeatures.h"
#include <string>

class DEMUserInterface;
class EFotoManager;

class DEMManager
{
	// Private Attributes
	//
        double bb_Xi, bb_Yi, bb_Xf, bb_Yf;
        bool bb_empty;
        void updateBoundingBox(double Xi, double Yi, double Xf, double Yf);
        int rad_cor, match_method, rgx, rgy, lsm_temp, lsm_it, lsm_dist, ncc_temp, ncc_sw;
        double lsm_th, lsm_std, lsm_shift, lsm_shear, lsm_scale, ncc_th, ncc_std, downsample;
        bool isShowImage;
	bool started;
	bool status;
        bool over_it;
        bool cancel_flag;
        bool dem_unsaved;
        bool grid_unsaved;
        bool elim_bad_pts;
        double over_it_dist;
        DEMUserInterface* myInterface;
        EFotoManager* manager;
        deque<Image*> listAllImages;
        deque<Point*> listAllPoints;
        deque<int> listPairs;
        deque<ExteriorOrientation*> listEOs;
        void updateNoSeeds();
        Image * getImage(int);
        void setListPoint();
        bool connectImagePoints();
        MatchingPointsList seeds, pairs;
        void getImagesId(int, int&, int&);
        void addPairsToInterface();
        void resamplePoints(MatchingPointsList *list, double resample);
        void createInitialSeeds();
        void extractDEMPair(int);
        void calcPointsXYZ();
        double fixAngle(double angle);
        double getAngleBetweenImages(double X1, double Y1, double X2, double Y2);
        bool checkAnglesAlligned(double angle1, double angle2, double tolerance);
        ImageMatching *im;
        DemGrid *grid;
        DemFeatures *df;
        StereoPair sp;

public:

	// Constructors and Destructors
	//
        DEMManager();
        DEMManager(EFotoManager* manager, deque<Image*> images, deque<ExteriorOrientation*> eos);
        ~DEMManager();

	// Association Methods
	//
        void setInterface(DEMUserInterface* newInterface);
        DEMUserInterface* getInterface();

	// Other Methods
	//
        bool exec();
        void returnProject();
        int getPairs();
        int extractDEM(int, bool);
        void setAutoExtractionSettings(int, int, int, int, double);
        void setNCCSettings(int, int, double, double);
        void setLSMSettings(int, int, double, double, int, double, double, double, int, double);
        void setProgress(int);
        void saveDem(char *, int);
        void saveDemGrid(char *, int);
        int loadDem(char *, int);
        void interpolateGrid(int source, int method, int garea, double Xi, double Yi, double Xf, double Yf, double res_x, double res_y, int tsurface, double ma_exp, double ma_dist, int ma_weight, int gridSource);
        void setCancel();
        void setShowImage(int _state) { isShowImage = _state; };
        void setEliminateBadPoints(bool _el) { elim_bad_pts = _el; };
        bool cancelFlag() { return cancel_flag; };
        bool isDemUnsaved() { return dem_unsaved; };
        bool isGridUnsaved() { return grid_unsaved; };
        void getPointList(MatchingPointsList &sd, MatchingPointsList &pr);
        deque<Image*> getImages() { return listAllImages; };
        deque<int> getImagesPairs() { return listPairs; };
        void overwriteSeedsList(MatchingPointsList sedlist) { seeds = sedlist; updateNoSeeds(); };
        int loadDemFeature(char * filename);
        string getDemQuality(char * filename, int option);

};

#endif //DEMMANAGER_H
