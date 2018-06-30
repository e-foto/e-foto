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
    Matrix La{};
    Matrix A{};
    Matrix P{};
    Matrix X0{};
    Matrix L0{};
    Matrix Lb{};
    Matrix lastL0{};
    std::deque<double> rmse{};

    RayTester* rt{nullptr};

    // Variables just for speeding up calculations, not really needed.
    double X00{0.0}, Y00{0.0}, Z00{0.0}, omega0{0.0}, phi0{0.0}, kappa0{0.0};

    std::deque<int> selectedPoints{};
    bool flightDirectionAvailable{false};
    bool pointForFlightDirectionAvailable{false};
    ImageSpaceCoordinate pointForFlightDirection{};

    bool gnssConverged{false}, insConverged{false};
    bool useDistortions{false};

    // Sets probably accessible only by the Mounter class.
    //
    void setXa(const Matrix& newXa);
    void setLb(const Matrix& newLb);

public:

	// Constructors and destructors
	//
	SpatialRessection();
    explicit SpatialRessection(int myImageId); // Constructor with ids only, needed in project use.
	virtual ~SpatialRessection();

	// Private attribute accessors
	//
    Matrix getLa() const;
    Matrix getA() const;
    Matrix getP() const;
    Matrix getLb() const;
    Matrix getLastL0() const;

    std::deque<int> getSelectedPoints() const;

    bool getConverged() const;
    std::deque<double> getRMSE() const {return rmse;}


	// Selected points list manipulators
	//
	void selectPoint(int id);
	void unselectPoint(int id);
    int countSelectedPoints() const;

	//  Selected fiducial mark or point to indicate the direction of flight manipulators
	//
	void setFlightDirection(double kappa0);
	void setPointForFlightDirection(double col, double row);
	void selectFiducialMarkForFlightDirection(int id);

    static bool is(std::string s);

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
