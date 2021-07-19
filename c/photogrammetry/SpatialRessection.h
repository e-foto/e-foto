#ifndef SPATIALRESSECTION_H
#define SPATIALRESSECTION_H
/**************************************************************************
	  SpatialRessection.h
**************************************************************************/
/*Copyright 2002-2021 e-foto team (UERJ)
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

#include "CommonDef.h"
#include "Matrix.h"
#include "ExteriorOrientation.h"
#include "ImageSpaceCoordinate.h"
#include "DetectorSpaceCoordinate.h"

/**
  * class SpatialRessection
  */

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class RayTester;

class SpatialRessection {
    Matrix La;
    Matrix A;
    Matrix P;
    Matrix X0;
    Matrix L0;
    Matrix Lb;
    Matrix lastL0;
    Matrix Xa;
    std::deque<double> rmse;

    RayTester* rt;

    double X00, Y00, Z00, omega0, phi0,
           kappa0; // Variables just for speeding up calculations, not really needed.
    std::deque<int> selectedPoints;
    bool flightDirectionAvailable{false};
    bool pointForFlightDirectionAvailable{false};
    ImageSpaceCoordinate pointForFlightDirection;

    bool gnssConverged{false};
    bool insConverged{false};
    bool useDistortions{true};
    int imageId;
    std::string type;

    int totalIterations;
    EOQuality myQuality;

    Image* myImage;

public:
    SpatialRessection();
    // Constructor with ids only, needed in project use.
    explicit SpatialRessection(int myImageId);
    virtual ~SpatialRessection();

    Matrix getLa() const;
    Matrix getA() const;
    Matrix getP() const;
    Matrix getLb() const;
    Matrix getLastL0() const;

    std::deque<int> getSelectedPoints() const;

    bool getConverged() const;
    std::deque<double> getRMSE() const
    {
        return rmse;
    }


    // Selected points list manipulators
    void selectPoint(int id);
    void unselectPoint(int id);
    int countSelectedPoints() const;

    //  Selected fiducial mark or point to indicate the direction of flight manipulators
    void setFlightDirection(double kappa0);
    void setPointForFlightDirection(double col, double row);
    void selectFiducialMarkForFlightDirection(int id);

    void setImageId(int newImageId);
    void setImage(Image* newImage);

    // EObject methods
    std::string objectType(void);
    std::string objectAssociations(void);
    bool is(const std::string& s) const;

    // XML methods
    void xmlSetData(std::string xml);
    std::string xmlGetData();
    std::string xmlGetDataEO();

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
    DetectorSpaceCoordinate applyDistortions(double xi, double eta);
    DetectorSpaceCoordinate applyDistortions(DetectorSpaceCoordinate
            myAnalogCoordinate);
    DetectorSpaceCoordinate removeDistortions(double xi, double eta);
    DetectorSpaceCoordinate removeDistortions(DetectorSpaceCoordinate
            myAnalogCoordinate);
    DetectorSpaceCoordinate getRadialDistortions(double xi, double eta);
    DetectorSpaceCoordinate getDecenteredDistortions(double xi, double eta);
    DetectorSpaceCoordinate getAtmosphereDistortions(double xi, double eta);
    DetectorSpaceCoordinate getCurvatureDistortions(double xi, double eta);

    int getImageId()const
    {
      return imageId;
    }
    Image* getImage() const
    {
      return myImage;
    }
    Matrix getXa()
    {
        return Xa;
    }

    void setXa(const Matrix& newXa);


    int getTotalIterations()
    {
        return totalIterations;
    }

    EOQuality getQuality()
    {
      return myQuality;
    }

    std::string getType();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //SPATIALRESSECTION_H
