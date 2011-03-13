/*******************************************************************************
							   PerspectiveRay.h
*******************************************************************************/


#ifndef PERSPECTIVERAY_H
#define PERSPECTIVERAY_H

#include "Image.h"
#include "Sensor.h"
#include "InteriorOrientation.h"
#include "SpatialRessection.h"
#include "DigitalImageSpaceCoordinate.h"
#include "AnalogImageSpaceCoordinate.h"
#include "ObjectSpaceCoordinate.h"

/**
* class PerspectiveRay
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 12/03/2011
* @version 1.0 - Rafael Alves de Aguiar
*/

class PerspectiveRay
{

protected:

	Image* myImage;
	double r11, r12, r13, r21, r22, r23, r31, r32, r33;
	bool hasRotations;

protected:

	PositionMatrix digitalZToAnalogZ(const PositionMatrix& myDigitalPositionsWithZ);
	PositionMatrix analogZToDigitalZ(const PositionMatrix& myAnalogPositionsWithZ);
	PositionMatrix applyDistortionsZ(const PositionMatrix& myAnalogPositionsWithZ);
	void calculateRotations();
	double getKx(double xi, double eta); // to be friended by SpatialIntersection!
	double getKy(double xi, double eta); // to be friended by SpatialIntersection!

public:

	// Constructors
	//
	PerspectiveRay(Image* myImage);

	// Other methods
	//
	AnalogImageSpaceCoordinate digitalToAnalog(unsigned int col, unsigned int lin);
	AnalogImageSpaceCoordinate digitalToAnalog(DigitalImageSpaceCoordinate myDigitalCoordinate);
	PositionMatrix digitalToAnalog(const PositionMatrix& myDigitalPositions);
	DigitalImageSpaceCoordinate analogToDigital(double xi, double eta);
	DigitalImageSpaceCoordinate analogToDigital(AnalogImageSpaceCoordinate myAnalogCoordinate);
	PositionMatrix analogToDigital(const PositionMatrix& myAnalogPositions);

	ObjectSpaceCoordinate analogToObject(double xi, double eta, double Z);
	ObjectSpaceCoordinate analogToObject(AnalogImageSpaceCoordinate myAnalogCoordinate, double Z);
	PositionMatrix analogToObject(const PositionMatrix& myAnalogPositions, double Z);
	PositionMatrix analogToObject(const PositionMatrix& myAnalogPositionsWithZ);
	AnalogImageSpaceCoordinate objectToAnalog(double X, double Y, double Z);
	AnalogImageSpaceCoordinate objectToAnalog(ObjectSpaceCoordinate myObjectCoordinate);
	PositionMatrix objectToAnalog(const PositionMatrix& myObjectPositions);

	AnalogImageSpaceCoordinate applyDistortions(double xi, double eta);
	AnalogImageSpaceCoordinate applyDistortions(AnalogImageSpaceCoordinate myAnalogCoordinate);
	PositionMatrix applyDistortions(const PositionMatrix& myAnalogPositions);
	AnalogImageSpaceCoordinate removeDistortions(double xi, double eta);
	AnalogImageSpaceCoordinate removeDistortions(AnalogImageSpaceCoordinate myAnalogCoordinate);
	PositionMatrix removeDistortions(const PositionMatrix& myAnalogPositions);

	ObjectSpaceCoordinate digitalToObject(unsigned int col, unsigned int lin, double Z, bool useDistortions = true);
	ObjectSpaceCoordinate digitalToObject(DigitalImageSpaceCoordinate myDigitalCoordinate, double Z, bool useDistortions = true);
	PositionMatrix digitalToObject(const PositionMatrix& myDigitalPositions, double Z, bool useDistortions = true);
	PositionMatrix digitalToObject(const PositionMatrix& myDigitalPositionsWithZ, bool useDistortions = true);
	DigitalImageSpaceCoordinate objectToDigital(double X, double Y, double Z, bool useDistortions = true);
	DigitalImageSpaceCoordinate objectToDigital(ObjectSpaceCoordinate myObjectCoordinate, bool useDistortions = true);
	PositionMatrix objectToDigital(const PositionMatrix& myObjectPositions, bool useDistortions = true);
};

#endif // PERSPECTIVERAY_H
