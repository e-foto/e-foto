#ifndef SPATIALRESSECTION_H
#define SPATIALRESSECTION_H
/**************************************************************************
	  SpatialRessection.h
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

#include "Matrix.h"
#include "ExteriorOrientation.h"
#include "ImageSpaceCoordinate.h"
#include "DetectorSpaceCoordinate.h"

/**
  * class SpatialRessection
  *
  * @author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 06/05/2009
  * @version 1.2 - Rafael Alves de Aguiar & Irving da Silva Badolato
  */

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class RayTester;

class SpatialRessection : public ExteriorOrientation
{
	// Private attributes
	//
	Matrix La;
	Matrix A;
	Matrix P;
	Matrix X0;
	Matrix L0;
	Matrix Lb;
	Matrix lastL0;
	std::deque<double> rmse;

	RayTester* rt;

	double X00, Y00, Z00, omega0, phi0, kappa0; // Variables just for speeding up calculations, not really needed.
	//double r11, r12, r13, r21, r22, r23, r31, r32, r33; // To make code reading and maintenance easier.
	std::deque<int> selectedPoints;
	bool flightDirectionAvailable;
	bool pointForFlightDirectionAvailable;
	ImageSpaceCoordinate pointForFlightDirection;

	bool gnssConverged, insConverged;
	bool useDistortions;

	// Sets probably accessible only by the Mounter class.
	//
	void setXa(const Matrix& newXa);
	void setLa(const Matrix& newLa);
	void setA(const Matrix& newA);
	void setP(const Matrix& newP);
	void setX0(const Matrix& newX0);
	void setL0(const Matrix& newL0);
	void setLb(const Matrix& newLb);

public:

	// Constructors and destructors
	//
	SpatialRessection();
    explicit SpatialRessection(int myImageId); // Constructor with ids only, needed in project use.
	virtual ~SpatialRessection();

	// Private attribute accessors
	//
	Matrix getLa();
	Matrix getA();
	Matrix getP();
	Matrix getX0();
	Matrix getL0();
	Matrix getLb();
	Matrix getLastL0();

	std::deque<int> getSelectedPoints();
	ImageSpaceCoordinate* getPointForFlightDirection();

	bool getConverged();
	bool getGnssConverged();
	bool getInsConverged();
	std::deque<double> getRMSE() {return rmse;}


	// Selected points list manipulators
	//
	void selectPoint(int id);
	void unselectPoint(int id);
	void unselectAllPoints();
	int countSelectedPoints();

	//  Selected fiducial mark or point to indicate the direction of flight manipulators
	//
	void setFlightDirection(double kappa0);
	void setPointForFlightDirection(double col, double row);
	void selectFiducialMarkForFlightDirection(int id);
	void unsetPointForFlightDirection();

	// EObject methods
	//
	std::string objectType(void);
	std::string objectAssociations(void);
	bool is(std::string s);

	// XML methods
	//
	void xmlSetData(std::string xml);
	std::string xmlGetData();
	std::string xmlGetDataEO();

	// Other methods
	//
	//void generateR();
	void generateInitialA();
	void generateInitialL0();
	void generateInitialP();
	void generateA();
	void generateL0();
	void generateRMSE();
	void generateLb();
	void generateP();
	void generateX0();
	void initialize();
	bool calculate(int maxIterations, double gnssPrecision, double insPrecision);

	// Private support methods
	//
	DetectorSpaceCoordinate applyDistortions(double xi, double eta);
	DetectorSpaceCoordinate applyDistortions(DetectorSpaceCoordinate myAnalogCoordinate);
	DetectorSpaceCoordinate removeDistortions(double xi, double eta);
	DetectorSpaceCoordinate removeDistortions(DetectorSpaceCoordinate myAnalogCoordinate);
	DetectorSpaceCoordinate getRadialDistortions(double xi, double eta);
	DetectorSpaceCoordinate getDecenteredDistortions(double xi, double eta);
	DetectorSpaceCoordinate getAtmosphereDistortions(double xi, double eta);
	DetectorSpaceCoordinate getCurvatureDistortions(double xi, double eta);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //SPATIALRESSECTION_H
