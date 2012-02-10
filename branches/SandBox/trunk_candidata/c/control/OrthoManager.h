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
#include "ProjectiveRay.h"
#include "Orthorectification.h"
#include "Interpolation.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

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
	Orthorectification *ortho;
	void runOrthoIndividual(int);
	void runAllOrthoTogheter();
	bool flag_cancel;
	bool show_image;
	int inter_method;

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
	int orthoRectification(char * filename, int fileType, int option, double user_res_x, double user_res_y);
	void setFlagCancel() { flag_cancel = true; };
	void setShowImage(bool _shw) { show_image = _shw; };
	void setInterMethod(int _mth) { inter_method = _mth; };
        void loadOrtho(char *filename);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //OrthoMANAGER_H
