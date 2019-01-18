#ifndef SENSORWITHFIDUCIALMARKS_H
#define SENSORWITHFIDUCIALMARKS_H
/**************************************************************************
	  SensorWithFiducialMarks.h
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
#include "DetectorFiducialMark.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
  * class SensorWithFiducialMarks
  *
  * @author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 04/02/2011
  * @version 1.3 - Rafael Alves de Aguiar & Irving da Silva Badolato
  */

class SensorWithFiducialMarks : public FrameSensor
{

	//EOBJECT

private:

	//Private attributes
	//
	Matrix Lb;
	Matrix SigmaLb;

	//Composed objects
	//
    std::deque<DetectorFiducialMark> anaFidMarks;

public:

	//Constructors
	//
	SensorWithFiducialMarks();
	SensorWithFiducialMarks(const Sensor& sensor);
    explicit SensorWithFiducialMarks(int myId);
    explicit SensorWithFiducialMarks(int myId, const Matrix& myLb);
    explicit SensorWithFiducialMarks(int myId, const Matrix& myLb, const Matrix& mySigmaLb);

	//Private attribute accessor methods
	//
	void setLb(const Matrix& newLb);
	void setLb(const Matrix& myLb, const Matrix& mySigmaLb);
	Matrix getLb();
	Matrix getSigmaLb();

	//Composed object accessor methods
	//
    void setAnaFidMarks(std::deque<DetectorFiducialMark> newAnaFidMarks);
    std::deque<DetectorFiducialMark> getAnaFidMarks();

	void putAnaFidMark(DetectorFiducialMark newAnaFidMark);
	DetectorFiducialMark getAnaFidMark(int id);
	DetectorFiducialMark getAnaFidMarkAt(unsigned int index);
	int countAnaFidMarks();

	void deleteAnaFidMark(int id);
	void deleteAnaFidMarkAt(unsigned int index);
	void clearAnaFidMarks();

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
	void marksToLb();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // SENSORWITHFIDUCIALMARKS_H
