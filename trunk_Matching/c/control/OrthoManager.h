/**************************************************************************
                                                           OrthoManager.h
**************************************************************************/


#ifndef ORTHOMANAGER_H
#define ORTHOMANAGER_H

#include "EObject.h"
#include <deque>
#include "Image.h"
#include "ExteriorOrientation.h"
#include "Matrix.h"
#include "DemGrid.h"
#include "SpatialIntersection.h"

class OrthoUserInterface;
class EFotoManager;

class OrthoManager
{
	// Private Attributes
	//
	bool started;
	bool status;
        OrthoUserInterface* myInterface;
        EFotoManager* manager;
        deque<Image*> listAllImages;
        deque<ExteriorOrientation*> listEOs;
        DemGrid *grid;
        void addImagesToForm();

public:

	// Constructors and Destructors
	//
        OrthoManager();
        OrthoManager(EFotoManager* manager, deque<Image*> images, deque<ExteriorOrientation*> eos);
        ~OrthoManager();

	// Association Methods
	//
        void setInterface(OrthoUserInterface* newInterface);
        OrthoUserInterface* getInterface();

	// Other Methods
	//
        bool exec();
        void returnProject();
        void setProgress(int);
        int loadDemGrid(char * filename, int fileType);
        int orthoRectification(char * filename, int fileType);

};

#endif //OrthoMANAGER_H
