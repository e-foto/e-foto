#ifndef SENSORWITHKNOWPARAMETERS_H
#define SENSORWITHKNOWPARAMETERS_H
/**************************************************************************
	  SensorWithKnowParameters.h
**************************************************************************/
/*Copyright 2002-2014 e-foto team (UERJ)
  This file is part of e-foto.

    e-foto is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    e-foto is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with e-foto.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "FrameSensor.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
  * class SensorWithKnowParameters
  *
  * @author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 07/02/2011
  * @version 1.4 - Rafael Alves de Aguiar & Irving da Silva Badolato
  */

class SensorWithKnowParameters : public FrameSensor
{

	//EOBJECT

private:

	//Private attributes
	//
	Matrix Xa;
	Matrix SigmaXa;

	//Composed objects
	//

public:

	//Constructors
	//
	SensorWithKnowParameters();
	SensorWithKnowParameters(const Sensor& sensor);
	SensorWithKnowParameters(int myId);
	SensorWithKnowParameters(int myId, const Matrix& myXa);
	SensorWithKnowParameters(int myId, const Matrix& myXa, const Matrix& mySigmaXa);

	//Private attribute accessor methods
	//
	void setXa(const Matrix& newXa);
	void setXa(const Matrix& myXa, const Matrix& mySigmaXa);
	Matrix getXa();
	Matrix getSigmaXa();

	//Composed object accessor methods
	//

	// EObject methods
	//
    std::string objectType(void);
    std::string objectAssociations(void);
    bool is(std::string s);

	// XML methods
	//
    void xmlSetData(std::string xml);
    std::string xmlGetData();

	//Other methods
	//

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // SENSORWITHKNOWPARAMETERS_H
