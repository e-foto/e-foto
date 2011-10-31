/*******************************************************************************
							  SpatialIntersection.h
*******************************************************************************/

#ifndef SPATIALINTERSECTION_H
#define SPATIALINTERSECTION_H

#include "StereoPair.h"
#include "ProjectiveRay.h"

/**
* class SpatialIntersection
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 07/06/2011
* @version 1.0 - Rafael Alves de Aguiar
*/

class SpatialIntersection : public EObject
{
	//EOBJECT

	// Aggregated objects
	//
	StereoPair* stereoPair;
	ProjectiveRay* leftRay;
	ProjectiveRay* rightRay;

public:

	// Constructors and destructors
	//
	SpatialIntersection();
	SpatialIntersection(StereoPair* myStereoPair);
	~SpatialIntersection();

	// Aggregation modifiers
	//
	void setStereoPair(StereoPair* newPair);
	StereoPair* getStereoPair();

	// Other methods
	//
	ObjectSpaceCoordinate calculateIntersection(double xiLeft, double etaLeft, double xiRight, double etaRight);
	ObjectSpaceCoordinate calculateIntersection(AnalogImageSpaceCoordinate leftCoord, AnalogImageSpaceCoordinate rightCoord);
	ObjectSpaceCoordinate calculateIntersection(int colLeft, int rowLeft, int colRight, int rowRight);
	ObjectSpaceCoordinate calculateIntersection(DigitalImageSpaceCoordinate leftCoord, DigitalImageSpaceCoordinate rightCoord);
	PositionMatrix calculateIntersection(PositionMatrix leftPos, PositionMatrix rightPos);

	// EObject methods
	//
	string objectType(void);
	string objectAssociations(void);
	bool is(string s);

	// XML methods
	//
	void xmlSetData(string xml);
	string xmlGetData();

protected:

	// Support method
	//
	double calculateZ(double xiLeft, double etaLeft, double xiRight, double etaRight);
};

#endif // STEREOPAIR_H
