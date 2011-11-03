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

class DEMUserInterface;
class EFotoManager;

class DEMManager
{
	// Private Attributes
	//
        int rad_cor, match_method, rgx, rgy, lsm_temp, lsm_it, lsm_dist, ncc_temp, ncc_sw;
        double lsm_th, lsm_std, lsm_shift, lsm_shear, lsm_scale, ncc_th, ncc_std;
	bool started;
	bool status;
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
        void setAutoExtractionSettings(int, int, int, int);
        void setNCCSettings(int, int, double, double);
        void setLSMSettings(int, int, double, double, int, double, double, double);
        void setProgress(int);

};

#endif //DEMMANAGER_H
