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

class DEMUserInterface;
class EFotoManager;

class DEMManager
{
	// Private Attributes
	//
        int rad_cor, match_method, rgx, rgy, lsm_temp, lsm_it, lsm_dist, ncc_temp, ncc_sw;
        double lsm_th, lsm_std, lsm_shift, lsm_shear, lsm_scale, ncc_th, ncc_std, downsample;
	bool started;
	bool status;
        bool over_it;
        double over_it_dist;
        DEMUserInterface* myInterface;
        EFotoManager* manager;
        deque<Image*> listAllImages;
        deque<Point*> listAllPoints;
        deque<int> listPairs;
        deque<ExteriorOrientation*> listEOs;
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
        void getPairs();
        void extractDEM(int);
        void setAutoExtractionSettings(int, int, int, int, double);
        void setNCCSettings(int, int, double, double);
        void setLSMSettings(int, int, double, double, int, double, double, double, int, double);
        void setProgress(int);
        void saveDem(char *, int);
        void saveDemGrid(char *, int);
        int loadDem(char *, int);
        void interpolateGrid(int source, int method, int garea, double Xi, double Yi, double Xf, double Yf, double res_x, double res_y, int tsurface, double ma_exp, double ma_dist, int ma_weight);

};

#endif //DEMMANAGER_H
