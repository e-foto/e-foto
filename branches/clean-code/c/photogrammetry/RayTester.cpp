/**************************************************************************
           RayTester.cpp
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

#include "RayTester.h"
#include "Image.h"
#include "FrameSensor.h"
#include "Terrain.h"
#include "Flight.h"

#include "math.h"

// Constructor

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

RayTester::RayTester(Image* myImage):
    myImage_{myImage}
{
}

// Attribute accessors

void RayTester::setIOParameters(Matrix params)
{
    if (params.getRows() == 6) {
        a0_ = params.get(1, 1);
        a1_ = params.get(2, 1);
        a2_ = params.get(3, 1);
        b0_ = params.get(4, 1);
        b1_ = params.get(5, 1);
        b2_ = params.get(6, 1);
        hasIO_ = true;
    }
}

void RayTester::setEOParameters(Matrix params)
{
    if (params.getRows() == 6) {
        X0_ = params.get(1, 1);
        Y0_ = params.get(2, 1);
        Z0_ = params.get(3, 1);
        phi_ = params.get(4, 1);
        omega_ = params.get(5, 1);
        kappa_ = params.get(6, 1);
        hasEO_ = true;
        hasRotations_ = false;
    }
}

// Other parameters
double RayTester::getKx(double xi, double eta)
{
    if (myImage_ != nullptr && myImage_->getSensor() != nullptr && hasEO_) {
        if (!hasRotations_) {
            calculateRotations();
        }

        double xi0 = myImage_->getSensor()->getPrincipalPointCoordinates().getXi();
        double eta0 = myImage_->getSensor()->getPrincipalPointCoordinates().getEta();
        double c = myImage_->getSensor()->getFocalDistance();
        return (r11_ * (xi - xi0) + r12_ * (eta - eta0) - r13_ * c) / (r31_ *
                (xi - xi0) + r32_ * (eta - eta0) - r33_ * c);
    }

    return 0.0;
}

double RayTester::getKy(double xi, double eta)
{
    if (myImage_ != nullptr && myImage_->getSensor() != nullptr && hasEO_) {
        if (!hasRotations_) {
            calculateRotations();
        }

        double xi0 = myImage_->getSensor()->getPrincipalPointCoordinates().getXi();
        double eta0 = myImage_->getSensor()->getPrincipalPointCoordinates().getEta();
        double c = myImage_->getSensor()->getFocalDistance();
        return (r21_ * (xi - xi0) + r22_ * (eta - eta0) - r23_ * c) / (r31_ *
                (xi - xi0) + r32_ * (eta - eta0) - r33_ * c);
    }

    return 0.0;
}

// Internal methods

PositionMatrix RayTester::imageZToDetectorZ(const PositionMatrix&
        myDigitalPositionsWithZ)
{
    if (myImage_ != nullptr && hasIO_) {
        PositionMatrix result(myDigitalPositionsWithZ.getRows(), "mm");
        DetectorSpaceCoordinate analogCoordinate;

        for (unsigned int i = 1; i + 2 <= myDigitalPositionsWithZ.getRows(); i += 3) {
            analogCoordinate = imageToDetector(myDigitalPositionsWithZ.get(i),
                                               myDigitalPositionsWithZ.get(i + 1));
            result.set(i, analogCoordinate.getXi());
            result.set(i + 1, analogCoordinate.getEta());
            result.set(i + 2, myDigitalPositionsWithZ.get(i + 2));
        }

        return result;
    }

    return PositionMatrix();
}

PositionMatrix RayTester::removeDistortionsZ(const PositionMatrix&
        myAnalogPositionsWithZ)
{
    if (myImage_ != nullptr && myImage_->getSensor() != nullptr && hasEO_) {
        PositionMatrix result(myAnalogPositionsWithZ.getRows(), "mm");
        DetectorSpaceCoordinate analogCoordinate;

        for (unsigned int i = 1; i + 2 <= myAnalogPositionsWithZ.getRows(); i += 3) {
            analogCoordinate = removeDistortions(myAnalogPositionsWithZ.get(i),
                                                 myAnalogPositionsWithZ.get(i + 1));
            result.set(i, analogCoordinate.getXi());
            result.set(i + 1, analogCoordinate.getEta());
            result.set(i + 1, myAnalogPositionsWithZ.get(i + 2));
        }

        return result;
    }

    return PositionMatrix();
}

void RayTester::calculateRotations()
{
    if (myImage_ != nullptr && hasEO_) {
        r11_ = cos(phi_) * cos(kappa_);
        r12_ = -cos(phi_) * sin(kappa_);
        r13_ = sin(phi_);
        r21_ = cos(omega_) * sin(kappa_) + sin(omega_) * sin(phi_) * cos(kappa_);
        r22_ = cos(omega_) * cos(kappa_) - sin(omega_) * sin(phi_) * sin(kappa_);
        r23_ = -sin(omega_) * cos(phi_);
        r31_ = sin(omega_) * sin(kappa_) - cos(omega_) * sin(phi_) * cos(kappa_);
        r32_ = sin(omega_) * cos(kappa_) + cos(omega_) * sin(phi_) * sin(kappa_);
        r33_ = cos(omega_) * cos(phi_);
        hasRotations_ = true;
    }
}

// Accessors

void RayTester::setImage(Image* newImage)
{
    myImage_ = newImage;
}

Image* RayTester::getImage()
{
    return myImage_;
}

// Other methods

DetectorSpaceCoordinate RayTester::imageToDetector(double col, double lin)
{
    if (myImage_ != nullptr && hasIO_) {
        DetectorSpaceCoordinate result;
        result.setXi(a0_ + a1_ * col + a2_ * lin);
        result.setEta(b0_ + b1_ * col + b2_ * lin);
        result.setAvailable(true);
        result.setSigmaAvailable(false);
        return result;
    }

    return DetectorSpaceCoordinate();
}

DetectorSpaceCoordinate RayTester::imageToDetector(ImageSpaceCoordinate
        myDigitalCoordinate)
{
    return imageToDetector(myDigitalCoordinate.getCol(),
                           myDigitalCoordinate.getLin());
}

PositionMatrix RayTester::imageToDetector(const PositionMatrix&
        myDigitalPositions)
{
    if (myImage_ != nullptr && hasIO_) {
        PositionMatrix result(myDigitalPositions.getRows(), "mm");
        DetectorSpaceCoordinate analogCoordinate;

        for (unsigned int i = 1; i + 1 <= myDigitalPositions.getRows(); i += 2) {
            analogCoordinate = imageToDetector(myDigitalPositions.get(i),
                                               myDigitalPositions.get(i + 1));
            result.set(i, analogCoordinate.getXi());
            result.set(i + 1, analogCoordinate.getEta());
        }

        return result;
    }

    return PositionMatrix();
}

ImageSpaceCoordinate RayTester::detectorToImage(double xi, double eta) const
{
    if (myImage_ != nullptr && hasIO_) {
        ImageSpaceCoordinate result;
        // From Silveira, M.T. - Master Thesis - UERJ, Geomatica 2005
        result.setCol(((b2_ * xi - b2_ * a0_ - a2_ * eta + b0_ * a2_) /
                       (a1_ * b2_ - b1_ * a2_)));
        result.setLin(((a1_ * eta - a1_ * b0_ - b1_ * xi + b1_ * a0_) /
                       (a1_ * b2_ - b1_ * a2_)));
        result.setAvailable(true);
        return result;
    }

    return ImageSpaceCoordinate();
}

ImageSpaceCoordinate RayTester::detectorToImage(DetectorSpaceCoordinate
        myAnalogCoordinate) const
{
    return detectorToImage(myAnalogCoordinate.getXi(), myAnalogCoordinate.getEta());
}

PositionMatrix RayTester::detectorToImage(const PositionMatrix&
        myAnalogPositions) const
{
    if (myImage_ != nullptr && hasIO_) {
        PositionMatrix result(myAnalogPositions.getRows(), "px");
        ImageSpaceCoordinate digitalCoordinate;

        for (unsigned int i = 1; i + 1 <= myAnalogPositions.getRows(); i += 2) {
            digitalCoordinate = detectorToImage(myAnalogPositions.get(i),
                                                myAnalogPositions.get(i + 1));
            result.set(i, digitalCoordinate.getCol());
            result.set(i + 1, digitalCoordinate.getLin());
        }

        return result;
    }

    return PositionMatrix();
}

ObjectSpaceCoordinate RayTester::detectorToObject(double xi, double eta,
        double Z)
{
    if (myImage_ != nullptr && myImage_->getSensor() != nullptr && hasEO_) {
        ObjectSpaceCoordinate result;

        if (!hasRotations_) {
            calculateRotations();
        }

        double xi0 = myImage_->getSensor()->getPrincipalPointCoordinates().getXi();
        double eta0 = myImage_->getSensor()->getPrincipalPointCoordinates().getEta();
        double c = myImage_->getSensor()->getFocalDistance();
        double kx = (r11_ * (xi - xi0) + r12_ * (eta - eta0) - r13_ * c) / (r31_ *
                    (xi - xi0) + r32_ * (eta - eta0) - r33_ * c);
        double ky = (r21_ * (xi - xi0) + r22_ * (eta - eta0) - r23_ * c) / (r31_ *
                    (xi - xi0) + r32_ * (eta - eta0) - r33_ * c);
        double X = X0_ + (Z - Z0_) * kx;
        double Y = Y0_ + (Z - Z0_) * ky;
        result.setX(X);
        result.setY(Y);
        result.setZ(Z);
        result.setAvailable(true);
        return result;
    }

    return ObjectSpaceCoordinate();
}

ObjectSpaceCoordinate RayTester::detectorToObject(DetectorSpaceCoordinate
        myAnalogCoordinate, double Z)
{
    return detectorToObject(myAnalogCoordinate.getXi(), myAnalogCoordinate.getEta(),
                            Z);
}

// Note: this only works with fixed Z value!
PositionMatrix RayTester::detectorToObject(const PositionMatrix&
        myAnalogPositions, double Z)
{
    if (myImage_ != nullptr && myImage_->getSensor() != nullptr && hasEO_
            && myAnalogPositions.getRows() % 2 == 0) {
        PositionMatrix result(myAnalogPositions.getRows() * 3 / 2, "m");
        ObjectSpaceCoordinate objectCoordinate;
        unsigned int i, j;

        for (i = 1, j = 1; i + 1 <= myAnalogPositions.getRows(); i += 2, j += 3) {
            objectCoordinate = detectorToObject(myAnalogPositions.get(i),
                                                myAnalogPositions.get(i + 1), Z);
            result.set(j, objectCoordinate.getX());
            result.set(j + 1, objectCoordinate.getY());
            result.set(j + 2, objectCoordinate.getZ());
        }

        return result;
    }

    return PositionMatrix();
}

// Note: for this one, the analogic positions matrix must have the Z value for each coordinate!
PositionMatrix RayTester::detectorToObject(const PositionMatrix&
        myAnalogPositionsWithZ)
{
    if (myImage_ != nullptr && myImage_->getSensor() != nullptr && hasEO_) {
        PositionMatrix result(myAnalogPositionsWithZ.getRows(), "m");
        ObjectSpaceCoordinate objectCoordinate;

        for (unsigned int i = 1; i + 2 <= myAnalogPositionsWithZ.getRows(); i += 3) {
            objectCoordinate = detectorToObject(myAnalogPositionsWithZ.get(i),
                                                myAnalogPositionsWithZ.get(i + 1), myAnalogPositionsWithZ.get(i + 2));
            result.set(i, objectCoordinate.getX());
            result.set(i + 1, objectCoordinate.getY());
            result.set(i + 2, objectCoordinate.getZ());
        }

        return result;
    }

    return PositionMatrix();
}

DetectorSpaceCoordinate RayTester::objectToDetector(double X, double Y,
        double Z)
{
    if (myImage_ != nullptr && myImage_->getSensor() != nullptr && hasEO_) {
        DetectorSpaceCoordinate result;

        if (!hasRotations_) {
            calculateRotations();
        }

        double xi0 = myImage_->getSensor()->getPrincipalPointCoordinates().getXi();
        double eta0 = myImage_->getSensor()->getPrincipalPointCoordinates().getEta();
        double c = myImage_->getSensor()->getFocalDistance();
        double xi = xi0 - c * (r11_ * (X - X0_)
                               + r21_ * (Y - Y0_)
                               + r31_ * (Z - Z0_)) / (r13_ * (X - X0_)
                                       + r23_ * (Y - Y0_)
                                       + r33_ * (Z - Z0_));
        double eta = eta0 - c * (r12_ * (X - X0_)
                                 + r22_ * (Y - Y0_)
                                 + r32_ * (Z - Z0_)) / (r13_ * (X - X0_)
                                         + r23_ * (Y - Y0_)
                                         + r33_ * (Z - Z0_));
        result.setXi(xi);
        result.setEta(eta);
        result.setAvailable(true);
        return result;
    }

    return DetectorSpaceCoordinate();
}

DetectorSpaceCoordinate RayTester::objectToDetector(ObjectSpaceCoordinate
        myObjectCoordinate)
{
    return objectToDetector(myObjectCoordinate.getX(), myObjectCoordinate.getY(),
                            myObjectCoordinate.getZ());
}

PositionMatrix RayTester::objectToDetector(const PositionMatrix&
        myObjectPositions)
{
    if (myImage_ != nullptr && myImage_->getSensor() != nullptr && hasEO_
            && myObjectPositions.getRows() % 3 == 0) {
        PositionMatrix result(myObjectPositions.getRows() * 2 / 3, "mm");
        DetectorSpaceCoordinate analogCoordinate;
        unsigned int i, j;

        for (i = 1, j = 1; i + 2 <= myObjectPositions.getRows(); i += 3, j += 2) {
            analogCoordinate = objectToDetector(myObjectPositions.get(i),
                                                myObjectPositions.get(i + 1),
                                                myObjectPositions.get(i + 2));
            result.set(j, analogCoordinate.getXi());
            result.set(j + 1, analogCoordinate.getEta());
        }

        return result;
    }

    return PositionMatrix();
}

DetectorSpaceCoordinate RayTester::applyDistortions(double xi, double eta)
{
    // Por enquanto estou aplicando todas. Crio a flag depois.
    DetectorSpaceCoordinate radial = getRadialDistortions(xi, eta);
    DetectorSpaceCoordinate decentered = getDecenteredDistortions(xi, eta);
    DetectorSpaceCoordinate atmosphere = getAtmosphereDistortions(xi, eta);
    DetectorSpaceCoordinate curvature = getCurvatureDistortions(xi, eta);
    double xiFinal = xi + (radial.getXi() + decentered.getXi() + atmosphere.getXi()
                           + curvature.getXi());
    double etaFinal = eta + (radial.getEta() + decentered.getEta() +
                             atmosphere.getEta() + curvature.getEta());
    DetectorSpaceCoordinate result;
    result.setXi(xiFinal);
    result.setEta(etaFinal);
    result.setAvailable(true);
    return result;
}

DetectorSpaceCoordinate RayTester::applyDistortions(DetectorSpaceCoordinate
        myAnalogCoordinate)
{
    return applyDistortions(myAnalogCoordinate.getXi(),
                            myAnalogCoordinate.getEta());
}

PositionMatrix RayTester::applyDistortions(const PositionMatrix&
        myAnalogPositions)
{
    if (myImage_ != nullptr && myImage_->getSensor() != nullptr && hasEO_) {
        PositionMatrix result(myAnalogPositions.getRows(), "mm");
        DetectorSpaceCoordinate analogCoordinate;

        for (unsigned int i = 1; i + 1 <= myAnalogPositions.getRows(); i += 2) {
            analogCoordinate = applyDistortions(myAnalogPositions.get(i),
                                                myAnalogPositions.get(i + 1));
            result.set(i, analogCoordinate.getXi());
            result.set(i + 1, analogCoordinate.getEta());
        }

        return result;
    }

    return PositionMatrix();
}

DetectorSpaceCoordinate RayTester::removeDistortions(double xi, double eta)
{
    // Por enquanto estou aplicando todas. Crio a flag depois.
    DetectorSpaceCoordinate radial = getRadialDistortions(xi, eta);
    DetectorSpaceCoordinate decentered = getDecenteredDistortions(xi, eta);
    DetectorSpaceCoordinate atmosphere = getAtmosphereDistortions(xi, eta);
    DetectorSpaceCoordinate curvature = getCurvatureDistortions(xi, eta);
    double xiFinal = xi - (radial.getXi()
                           + decentered.getXi()
                           + atmosphere.getXi()
                           + curvature.getXi());
    double etaFinal = eta - (radial.getEta()
                             + decentered.getEta()
                             + atmosphere.getEta()
                             + curvature.getEta());
    DetectorSpaceCoordinate result;
    result.setXi(xiFinal);
    result.setEta(etaFinal);
    result.setAvailable(true);
    return result;
}

DetectorSpaceCoordinate RayTester::removeDistortions(DetectorSpaceCoordinate
        myAnalogCoordinate)
{
    return removeDistortions(myAnalogCoordinate.getXi(),
                             myAnalogCoordinate.getEta());
}

PositionMatrix RayTester::removeDistortions(const PositionMatrix&
        myAnalogPositions)
{
    if (myImage_ != nullptr && myImage_->getSensor() != nullptr && hasEO_) {
        PositionMatrix result(myAnalogPositions.getRows(), "mm");
        DetectorSpaceCoordinate analogCoordinate;

        for (size_t i = 1; i + 1 <= myAnalogPositions.getRows(); i += 2) {
            analogCoordinate = removeDistortions(myAnalogPositions.get(i),
                                                 myAnalogPositions.get(i + 1));
            result.set(i, analogCoordinate.getXi());
            result.set(i + 1, analogCoordinate.getEta());
        }

        return result;
    }

    return PositionMatrix();
}

ObjectSpaceCoordinate RayTester::imageToObject(double col, double lin, double Z,
        bool useDistortions)
{
    if (myImage_ != nullptr && myImage_->getSensor() != nullptr && hasIO_
            && hasEO_) {
        DetectorSpaceCoordinate analog = imageToDetector(col, lin);

        if (useDistortions) {
            analog = removeDistortions(analog);
        }

        return detectorToObject(analog, Z);
    }

    return ObjectSpaceCoordinate();
}

ObjectSpaceCoordinate RayTester::imageToObject(ImageSpaceCoordinate
        myDigitalCoordinate, double Z, bool useDistortions)
{
    return imageToObject(myDigitalCoordinate.getCol(), myDigitalCoordinate.getLin(),
                         Z, useDistortions);
}

// Note: this only works with fixed Z value!
PositionMatrix RayTester::imageToObject(const PositionMatrix&
                                        myDigitalPositions, double Z, bool useDistortions)
{
    if (myImage_ != nullptr && myImage_->getSensor() != nullptr && hasIO_
            && hasEO_) {
        PositionMatrix myAnalogPositions = imageToDetector(myDigitalPositions);

        if (useDistortions) {
            myAnalogPositions = removeDistortions(myAnalogPositions);
        }

        return detectorToObject(myAnalogPositions, Z);
    }

    return PositionMatrix();
}

// Note: for this one, the digital positions matrix must have the Z value for each coordinate!
PositionMatrix RayTester::imageToObject(const PositionMatrix&
                                        myDigitalPositionsWithZ, bool useDistortions)
{
    if (myImage_ != nullptr && myImage_->getSensor() != nullptr && hasIO_
            && hasEO_) {
        PositionMatrix myAnalogPositions = imageZToDetectorZ(myDigitalPositionsWithZ);

        if (useDistortions) {
            myAnalogPositions = removeDistortionsZ(myAnalogPositions);
        }

        return detectorToObject(myAnalogPositions);
    }

    return PositionMatrix();
}

ImageSpaceCoordinate RayTester::objectToImage(double X, double Y, double Z,
        bool useDistortions)
{
    if (myImage_ != nullptr && myImage_->getSensor() != nullptr && hasIO_
            && hasEO_) {
        DetectorSpaceCoordinate analog = objectToDetector(X, Y, Z);

        if (useDistortions) {
            analog = applyDistortions(analog);
        }

        return detectorToImage(analog);
    }

    return ImageSpaceCoordinate();
}

ImageSpaceCoordinate RayTester::objectToImage(ObjectSpaceCoordinate
        myObjectCoordinate, bool useDistortions)
{
    return objectToImage(myObjectCoordinate.getX(), myObjectCoordinate.getY(),
                         myObjectCoordinate.getZ(), useDistortions);
}

PositionMatrix RayTester::objectToImage(const PositionMatrix& myObjectPositions,
                                        bool useDistortions)
{
    if (myImage_ != nullptr && myImage_->getSensor() != nullptr && hasIO_
            && hasEO_) {
        PositionMatrix myAnalogPositions = objectToDetector(myObjectPositions);

        if (useDistortions) {
            myAnalogPositions = applyDistortions(myAnalogPositions);
        }

        return detectorToImage(myAnalogPositions);
    }

    return PositionMatrix();
}

// Private support methods

DetectorSpaceCoordinate RayTester::getRadialDistortions(double xi, double eta)
{
    DetectorSpaceCoordinate result;
    result.setXi(0.0);
    result.setEta(0.0);
    result.setAvailable(true);
    Sensor* sensor = myImage_->getSensor();
    FrameSensor* frame = dynamic_cast<FrameSensor*>(sensor);

    if (frame != nullptr) {
        double r = sqrt(xi * xi + eta * eta);
        std::deque<RadialSymmetricDistortionCoefficient> rs =
            frame->getRadialSymmetricCoefficients();
        double dr = 0;

        for (unsigned int i = 0; i < rs.size(); i++) {
            dr += rs.at(i).getValue() * pow(r, 2 * i + 1);
        }

        double dx = dr * xi / r;
        double dy = dr * eta / r;
        result.setXi(dx);
        result.setEta(dy);
    }

    return result;
}

DetectorSpaceCoordinate RayTester::getDecenteredDistortions(double xi,
        double eta)
{
    DetectorSpaceCoordinate result;
    result.setXi(0.0);
    result.setEta(0.0);
    result.setAvailable(true);
    Sensor* sensor = myImage_->getSensor();
    FrameSensor* frame = dynamic_cast<FrameSensor*>(sensor);

    if (frame != nullptr) {
        double r = sqrt(xi * xi + eta * eta);
        std::deque<DecenteredDistortionCoefficient> dec =
            frame->getDecenteredCoefficients();
        double P1 = dec.at(0).getValue();
        double P2 = dec.at(1).getValue();
        double dx = P1 * (r * r + 2 * xi * xi) + 2 * P2 * xi * eta;
        double dy = 2 * P1 * xi * eta + P2 * (r * r + 2 * eta * eta);
        result.setXi(dx);
        result.setEta(dy);
    }

    return result;
}

DetectorSpaceCoordinate RayTester::getAtmosphereDistortions(double xi,
        double eta)
{
    DetectorSpaceCoordinate result;
    result.setXi(0.0);
    result.setEta(0.0);
    result.setAvailable(true);
    Flight* flight = myImage_->getFlight();
    Sensor* sensor = myImage_->getSensor();

    if (flight != nullptr && sensor != nullptr) {
        Terrain* terrain = flight->getTerrain();

        if (terrain != nullptr) {
            if (!hasEO_) {
                Z0_ = flight->getHeight() + terrain->getMeanAltitude();
            }

            double Zp = terrain->getMeanAltitude();
            Z0_ /= 1000;
            Zp /= 1000;
            double aux1 = Z0_ / ((Z0_ * Z0_) - 6 * Z0_ + 250);
            double aux2 = Zp / Z0_ * ((Zp * Zp) - 6 * Zp + 250);
            double K = 2410 * 0.000001 * (aux1 - aux2);
            double r = sqrt(xi * xi + eta * eta);
            double f = sensor->getFocalDistance();
            double dr = r * (1 + (r * r / f * f)) * K;
            double dx = dr * xi / r;
            double dy = dr * eta / r;
            result.setXi(dx);
            result.setEta(dy);
        }
    }

    return result;
}

DetectorSpaceCoordinate RayTester::getCurvatureDistortions(double xi,
        double eta)
{
    DetectorSpaceCoordinate result;
    result.setXi(0.0);
    result.setEta(0.0);
    result.setAvailable(true);
    Flight* flight = myImage_->getFlight();
    Sensor* sensor = myImage_->getSensor();

    if (flight != nullptr && sensor != nullptr) {
        Terrain* terrain = flight->getTerrain();

        if (terrain != nullptr) {
            if (!hasEO_) {
                Z0_ = flight->getHeight() + terrain->getMeanAltitude();
            }

            Z0_ /= 1000;
            double R = 6371004.0; // De onde eu tiro isso? Deixo essa constante?
            double r = sqrt(xi * xi + eta * eta);
            double f = sensor->getFocalDistance();
            double aux = (r * r * Z0_) / (2 * R * f * f);
            double dx = xi * aux;
            double dy = eta * aux;
            result.setXi(dx);
            result.setEta(dy);
        }
    }

    return result;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
