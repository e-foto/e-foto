#ifndef SENSORWITHKNOWDIMENSIONS_H
#define SENSORWITHKNOWDIMENSIONS_H
/**************************************************************************
	  SensorWithKnowDimensions.h
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
  * class SensorWithKnowDimensions
  *
  * @author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 07/02/2011
  * @version 1.4 - Rafael Alves de Aguiar & Irving da Silva Badolato
  */

class SensorWithKnowDimensions : public FrameSensor
{

	//EOBJECT

private:

	//Private attributes
	//
	double pixelSize;
	int frameColumns; int frameRows;

	//Composed objects
	//

public:

	//Constructors
	//
	SensorWithKnowDimensions();
	SensorWithKnowDimensions(const Sensor& sensor);
    explicit SensorWithKnowDimensions(int myId);
    explicit SensorWithKnowDimensions(int myId, double myPixelSize, int myFrameRows, int myFrameColumns);

	//Private attribute accessor methods
	//
	void setPixelSize(double newPixelSize);
	void setFrameDimensions(int newFrameRows, int newFrameColumns);
	double getPixelSize();
	int getFrameRows();
	int getFrameColumns();
	Matrix forgeLb();

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

#endif // SENSORWITHKNOWDIMENSIONS_H
