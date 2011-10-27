/*******************************************************************************
						AnalogImageSpaceCoordinate.h
*******************************************************************************/


#ifndef ANALOGIMAGESPACECOORDINATE_H
#define ANALOGIMAGESPACECOORDINATE_H

#include "AbstractAnalogCoordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
* class AnalogImageSpaceCoordinate
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 06/05/2009
* @version 1.2 - Rafael Alves de Aguiar & Irving da Silva Badolato.
*/

class AnalogImageSpaceCoordinate : public AbstractAnalogCoordinate
{
	//EOBJECT

private:

	// Private attributes
	//
	int imageId;


public:

	// Constructors
	//
	AnalogImageSpaceCoordinate();
	AnalogImageSpaceCoordinate(int myImageId);
	AnalogImageSpaceCoordinate(int myImageId, string myUnit, double myXi, double myEta, double mySigmaXi = 1, double mySigmaEta = 1, double mySigmaXiEta = 0);
	AnalogImageSpaceCoordinate(int myImageId, const PositionMatrix& myPosition);
	AnalogImageSpaceCoordinate(int myImageId, const PositionMatrix& myPosition, const Matrix& myPositionSigmas);
	~AnalogImageSpaceCoordinate();

	// Private attribute accessor methods
	//
	void setImageId(int newImageId);
	int getImageId();

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

#endif // ANALOGIMAGESPACECOORDINATE_H
