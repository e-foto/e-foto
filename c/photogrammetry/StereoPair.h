/*******************************************************************************
		  StereoPair.h
*******************************************************************************/

#ifndef STEREOPAIR_H
#define STEREOPAIR_H

#include "Image.h"
#include "InteriorOrientation.h"
#include "SpatialRessection.h"

/**
* class StereoPair
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 07/04/2011
* @version 1.0 - Rafael Alves de Aguiar
*/

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class StereoPair : public EObject
{
	//EOBJECT

	// Aggregated objects
	//
	Image* leftImage;
	Image* rightImage;

public:

	// Constructors and destructors
	//
	StereoPair();
	StereoPair(Image* myLeftImage, Image* myRightImage);
	~StereoPair();

	// Aggregation modifiers
	//
	void setLeftImage(Image* newImage);
	void setRightImage(Image* newImage);
	Image* getLeftImage();
	Image* getRightImage();

	// Consistency testers
	//
	bool leftHasIO();
	bool leftHasEO();
	bool rightHasIO();
	bool rightHasEO();
	bool leftOriented();
	bool rightOriented();
	bool hasOrientations();

	// EObject methods
	//
	string objectType(void);
	string objectAssociations(void);
	bool is(string s);

	// XML methods
	//
	void xmlSetData(string xml);
	string xmlGetData();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // STEREOPAIR_H
