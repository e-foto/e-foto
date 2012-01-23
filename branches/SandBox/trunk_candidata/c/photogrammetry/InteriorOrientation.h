/**************************************************************************
	  InteriorOrientation.h
**************************************************************************/


#ifndef INTERIORORIENTATION_H
#define INTERIORORIENTATION_H

#include "EObject.h"
#include "IOQuality.h"
#include "DigitalImageSpaceCoordinate.h"
#include "AnalogImageSpaceCoordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class SensorWithFiducialMarks;
class Image;

/**
  * class InteriorOrientation
  *
  * @author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 06/05/2009
  * @version 1.2 - Rafael Alves de Aguiar & Irving da Silva Badolato
  *
  */

class InteriorOrientation : public EObject
{

	//EOBJECT

	// Private attributes
	//
	int imageId;
	Matrix Xa;
	Matrix La;
	Matrix A;
	Matrix P;

	// Composed objects
	//
	IOQuality myQuality;

	// Associated objects
	//
	Image* myImage;

	// Private method
	//
	void generateA();

public:

	// Constructors and destructors
	//
	InteriorOrientation();
	InteriorOrientation(int myImageId); // Constructor with ids only, needed in project use.
	virtual ~InteriorOrientation();

	// Private attribute accessors
	//

	int getImageId();
	Matrix getXa();
	Matrix getLa();
	Matrix getA();
	Matrix getP();


	// Composed objects accessors
	//

	IOQuality getQuality();

	// Associated objects accessors
	//

	void setImage(Image *myNewImage);
	Image* getImage();

	// EObject methods
	//
	string objectType(void);
	string objectAssociations(void);
	bool is(string s);

	// XML methods
	//
	void xmlSetData(string xml);
	string xmlGetData();

	// Other methods
	//

	void calculate();
	AnalogImageSpaceCoordinate digitalToAnalog(unsigned int col, unsigned int lin);
	AnalogImageSpaceCoordinate digitalToAnalog(DigitalImageSpaceCoordinate myDigitalCoordinate);
	PositionMatrix digitalToAnalog(const PositionMatrix& myDigitalPositions);
	DigitalImageSpaceCoordinate analogToDigital(double x, double y);
	DigitalImageSpaceCoordinate analogToDigital(AnalogImageSpaceCoordinate myAnalogCoordinate);
	PositionMatrix analogToDigital(const PositionMatrix& myAnalogPositions);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // INTERIORORIENTATION_H
