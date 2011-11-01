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
	bool started;
	bool status;
        DEMUserInterface* myInterface;
        EFotoManager* manager;
        deque<Image*> listAllImages;
        deque<Point*> listAllPoints;
        deque<int> listPairs;
        deque<ExteriorOrientation*> listEOs;
        void setListPoint();
        bool connectImagePoints();
        ImageMatching im;
        Matrix *img1, *img2;
        MatchingPointsList seeds, pairs;
        void resamplePoints(MatchingPointsList *list, double resample);
        void createInitialSeeds();

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

};

#endif //DEMMANAGER_H
