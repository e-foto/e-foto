/*******************************************************************************
						AnalogFiductialMark.h
*******************************************************************************/

#ifndef ANALOGFIDUCTIALMARK_H
#define ANALOGFIDUCTIALMARK_H

#include "AnalogImageSpaceCoordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
* class AnalogFiductialMark
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 06/05/2009
* @version 1.2 - Rafael Alves de Aguiar & Irving da Silva Badolato.
*/

class AnalogFiductialMark : public AbstractAnalogCoordinate
{
	//EOBJECT

private:

	// Private attributes
	//
	int id;
	int sensorId;


public:

	// Constructors
	//
	AnalogFiductialMark();
	AnalogFiductialMark(int myId, int mySensorId);
	AnalogFiductialMark(int myId, int mySensorId, string myUnit, double myXi, double myEta, double mySigmaXi = 1, double mySigmaEta = 1, double mySigmaXiEta = 0);
	AnalogFiductialMark(int myId, int mySensorId, const PositionMatrix& myPosition);
	AnalogFiductialMark(int myId, int mySensorId, const PositionMatrix& myPosition, const Matrix& myPositionSigmas);
	~AnalogFiductialMark();

	// Private attribute accessor methods
	//
	void setId(int newId);
	void setSensorId(int newSensorId);
	int getId();
	int getSensorId();

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

#endif // ANALOGFIDUCTIALMARK_H
