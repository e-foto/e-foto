/*******************************************************************************
						DigitalImageSpaceCoordinate.h
*******************************************************************************/


#ifndef DIGITALIMAGESPACECOORDINATE_H
#define DIGITALIMAGESPACECOORDINATE_H

#include "AbstractDigitalCoordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
* class DigitalImageSpaceCoordinate
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 06/05/2009
* @version 1.2 - Rafael Alves de Aguiar & Irving da Silva Badolato.
*/

class DigitalImageSpaceCoordinate : public AbstractDigitalCoordinate
{
	//EOBJECT

private:

	// Private attributes
	//
	int imageId;

public:

	// Constructors
	//
	DigitalImageSpaceCoordinate();
	DigitalImageSpaceCoordinate(int myImageId);
	DigitalImageSpaceCoordinate(int myImageId, int myCol, int myLin);
	DigitalImageSpaceCoordinate(int myImageId, const PositionMatrix& myPosition);
	~DigitalImageSpaceCoordinate();

	// Private attribute accessor methods
	//
	void setImageId(int newImageId);
	int getImageId();

	// EObject methods
	//
	string objectType(void);
	string objectAssociations(void);
	bool is(string s);

	// Xml methods
	//
	string xmlGetData();
	void xmlSetData(string xml);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // DIGITALIMAGESPACECOORDINATE_H
