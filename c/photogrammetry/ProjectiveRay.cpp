/**************************************************************************
           ProjectiveRay.cpp
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

#include "ProjectiveRay.h"
#include "PositionMatrix.h"

#include "Image.h"
#include "SpatialRessection.h"
#include "InteriorOrientation.h"
#include "FrameSensor.h"
#include "Terrain.h"
#include "Flight.h"

#include <math.h>

#include <string>
#include <exception>


// Constructor

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

ProjectiveRay::ProjectiveRay(Image *myImage)
{
        this->myImage = myImage;
        this->hasRotations = false;
}

// Internal methods

PositionMatrix ProjectiveRay::imageZToDetectorZ(const PositionMatrix& myDigitalPositionsWithZ)
{
        if (myImage != NULL && myImage->getIO() != NULL)
        {
                PositionMatrix result(myDigitalPositionsWithZ.getRows(), "mm");
                DetectorSpaceCoordinate analogCoordinate;

                for (unsigned int i = 1; i+2 <= myDigitalPositionsWithZ.getRows(); i += 3)
                {
                        analogCoordinate = imageToDetector(myDigitalPositionsWithZ.get(i),myDigitalPositionsWithZ.get(i+1));
                        result.set(i,analogCoordinate.getXi());
                        result.set(i+1,analogCoordinate.getEta());
                        result.set(i+2,myDigitalPositionsWithZ.get(i+2));
                }

                return result;
        }
        else
        {
                return PositionMatrix();
        }
}

PositionMatrix ProjectiveRay::detectorZToImageZ(const PositionMatrix& myAnalogPositionsWithZ)
{
        if (myImage != NULL && myImage->getIO() != NULL)
        {
                PositionMatrix result(myAnalogPositionsWithZ.getRows(), "px");
                ImageSpaceCoordinate digitalCoordinate;

                for (unsigned int i = 1; i+2 <= myAnalogPositionsWithZ.getRows(); i += 3)
                {
                        digitalCoordinate = detectorToImage(myAnalogPositionsWithZ.get(i),myAnalogPositionsWithZ.get(i+1));
                        result.set(i,digitalCoordinate.getCol());
                        result.set(i+1,digitalCoordinate.getLin());
                        result.set(i+2,myAnalogPositionsWithZ.get(i+2));
                }

                return result;
        }
        else
        {
                return PositionMatrix();
        }
}

PositionMatrix ProjectiveRay::applyDistortionsZ(const PositionMatrix& myAnalogPositionsWithZ)
{
        if (myImage != NULL && myImage->getSensor() != NULL && myImage->getEO() != NULL)
        {
                PositionMatrix result(myAnalogPositionsWithZ.getRows(), "mm");
                DetectorSpaceCoordinate analogCoordinate;

                for (unsigned int i = 1; i+2 <= myAnalogPositionsWithZ.getRows(); i += 3)
                {
                        analogCoordinate = applyDistortions(myAnalogPositionsWithZ.get(i),myAnalogPositionsWithZ.get(i+1));
                        result.set(i,analogCoordinate.getXi());
                        result.set(i+1,analogCoordinate.getEta());
                        result.set(i+1,myAnalogPositionsWithZ.get(i+2));
                }

                return result;
        }
        else
        {
                return PositionMatrix();
        }
}

void ProjectiveRay::calculateRotations()
{
        if (myImage != NULL && myImage->getEO() != NULL)
        {
                double phi = myImage->getEO()->getXa().get(4,1);
                double omega = myImage->getEO()->getXa().get(5,1);
                double kappa = myImage->getEO()->getXa().get(6,1);

                r11 = cos(phi)*cos(kappa);
                r12 = -cos(phi)*sin(kappa);
                r13 = sin(phi);
                r21 = cos(omega)*sin(kappa)+sin(omega)*sin(phi)*cos(kappa);
                r22 = cos(omega)*cos(kappa)-sin(omega)*sin(phi)*sin(kappa);
                r23 = -sin(omega)*cos(phi);
                r31 = sin(omega)*sin(kappa)-cos(omega)*sin(phi)*cos(kappa);
                r32 = sin(omega)*cos(kappa)+cos(omega)*sin(phi)*sin(kappa);
                r33 = cos(omega)*cos(phi);

                hasRotations = true;
        }
}

double ProjectiveRay::getKx(double xi, double eta)
{
        if (myImage != NULL && myImage->getSensor() != NULL && myImage->getEO() != NULL)
        {
                if (!hasRotations)
                        calculateRotations();

                double xi0 = myImage->getSensor()->getPrincipalPointCoordinates().getXi();
                double eta0 = myImage->getSensor()->getPrincipalPointCoordinates().getEta();
                double c = myImage->getSensor()->getFocalDistance();

                return (r11*(xi-xi0) + r12*(eta-eta0) - r13*c) / (r31*(xi-xi0) + r32*(eta-eta0) - r33*c);
        }
        else
        {
                return 0.0;
        }
}

double ProjectiveRay::getKy(double xi, double eta)
{
        if (myImage != NULL && myImage->getSensor() != NULL && myImage->getEO() != NULL)
        {
                if (!hasRotations)
                        calculateRotations();

                double xi0 = myImage->getSensor()->getPrincipalPointCoordinates().getXi();
                double eta0 = myImage->getSensor()->getPrincipalPointCoordinates().getEta();
                double c = myImage->getSensor()->getFocalDistance();

                return (r21*(xi-xi0) + r22*(eta-eta0) - r23*c) / (r31*(xi-xi0) + r32*(eta-eta0) - r33*c);
        }
        else
        {
                return 0.0;
        }
}

// Other methods

DetectorSpaceCoordinate ProjectiveRay::imageToDetector(double col, double lin)
{
        if (myImage != NULL && myImage->getIO() != NULL)
        {
                DetectorSpaceCoordinate result;

                Matrix Xa = myImage->getIO()->getXa();

                result.setXi(Xa.get(1,1) + Xa.get(2,1)*col + Xa.get(3,1)*lin);
                result.setEta(Xa.get(4,1) + Xa.get(5,1)*col + Xa.get(6,1)*lin);

                Matrix positionA(2,6);
                positionA.set(1,1,1);
                positionA.set(1,2,col);
                positionA.set(1,3,lin);
                positionA.set(2,4,1);
                positionA.set(2,5,col);
                positionA.set(2,6,lin);

                Matrix positionSigmas = positionA * myImage->getIO()->getQuality().getSigmaXa() * positionA.transpose();
                result.setPositionSigmas(positionSigmas);

                result.setAvailable(true);
                result.setSigmaAvailable(true);

                return result;
        }
        else
        {
                return DetectorSpaceCoordinate();
        }
}

DetectorSpaceCoordinate ProjectiveRay::imageToDetector(ImageSpaceCoordinate myDigitalCoordinate)
{
        return imageToDetector(myDigitalCoordinate.getCol(), myDigitalCoordinate.getLin());
}

PositionMatrix ProjectiveRay::imageToDetector(const PositionMatrix& myDigitalPositions)
{
        if (myImage != NULL && myImage->getIO() != NULL)
        {
                PositionMatrix result(myDigitalPositions.getRows(), "mm");
                DetectorSpaceCoordinate analogCoordinate;

                for (unsigned int i = 1; i+1 <= myDigitalPositions.getRows(); i += 2)
                {
                        analogCoordinate = imageToDetector(myDigitalPositions.get(i),myDigitalPositions.get(i+1));
                        result.set(i,analogCoordinate.getXi());
                        result.set(i+1,analogCoordinate.getEta());
                }

                return result;
        }
        else
        {
                return PositionMatrix();
        }
}

void ProjectiveRay::detectorToImage(double xi, double eta, double &x, double &y)
{
        if (myImage != NULL && myImage->getIO() != NULL)
        {
                ImageSpaceCoordinate result;

                Matrix Xa = myImage->getIO()->getXa();
                double a0, a1, a2, b0, b1, b2;
                a0 = Xa.get(1,1);
                a1 = Xa.get(2,1);
                a2 = Xa.get(3,1);
                b0 = Xa.get(4,1);
                b1 = Xa.get(5,1);
                b2 = Xa.get(6,1);

                // From Silveira, M.T. - Master Thesis - UERJ, Geomatica 2005
                x = ((b2*xi - b2*a0 - a2*eta + b0*a2) / (a1*b2 - b1*a2));
                y = ((a1*eta - a1*b0 - b1*xi + b1*a0) / (a1*b2 - b1*a2));
        }
}

ImageSpaceCoordinate ProjectiveRay::detectorToImage(double xi, double eta)
{
        if (myImage != NULL && myImage->getIO() != NULL)
        {
                ImageSpaceCoordinate result;

                Matrix Xa = myImage->getIO()->getXa();
                double a0, a1, a2, b0, b1, b2;
                a0 = Xa.get(1,1);
                a1 = Xa.get(2,1);
                a2 = Xa.get(3,1);
                b0 = Xa.get(4,1);
                b1 = Xa.get(5,1);
                b2 = Xa.get(6,1);

                // From Silveira, M.T. - Master Thesis - UERJ, Geomatica 2005
                result.setCol(((b2*xi - b2*a0 - a2*eta + b0*a2) / (a1*b2 - b1*a2)));
                result.setLin(((a1*eta - a1*b0 - b1*xi + b1*a0) / (a1*b2 - b1*a2)));

                result.setAvailable(true);

                return result;
        }
        else
        {
                return ImageSpaceCoordinate();
        }
}

ImageSpaceCoordinate ProjectiveRay::detectorToImage(DetectorSpaceCoordinate myAnalogCoordinate)
{
        return detectorToImage(myAnalogCoordinate.getXi(), myAnalogCoordinate.getEta());
}

PositionMatrix ProjectiveRay::detectorToImage(const PositionMatrix& myAnalogPositions)
{
        if (myImage != NULL && myImage->getIO() != NULL)
        {
                PositionMatrix result(myAnalogPositions.getRows(), "px");
                ImageSpaceCoordinate digitalCoordinate;

                for (unsigned int i = 1; i+1 <= myAnalogPositions.getRows(); i += 2)
                {
                        digitalCoordinate = detectorToImage(myAnalogPositions.get(i),myAnalogPositions.get(i+1));
                        result.set(i,digitalCoordinate.getCol());
                        result.set(i+1,digitalCoordinate.getLin());
                }

                return result;
        }
        else
        {
                return PositionMatrix();
        }
}

ObjectSpaceCoordinate ProjectiveRay::detectorToObject(double xi, double eta, double Z)
{
        if (myImage != NULL && myImage->getSensor() != NULL && myImage->getEO() != NULL)
        {
                ObjectSpaceCoordinate result;

                if (!hasRotations)
                        calculateRotations();

                double xi0 = myImage->getSensor()->getPrincipalPointCoordinates().getXi();
                double eta0 = myImage->getSensor()->getPrincipalPointCoordinates().getEta();
                double c = myImage->getSensor()->getFocalDistance();

                Matrix Xa = myImage->getEO()->getXa();
                double X0 = Xa.get(1,1);
                double Y0 = Xa.get(2,1);
                double Z0 = Xa.get(3,1);

                double kx = (r11*(xi-xi0) + r12*(eta-eta0) - r13*c) / (r31*(xi-xi0) + r32*(eta-eta0) - r33*c);
                double ky = (r21*(xi-xi0) + r22*(eta-eta0) - r23*c) / (r31*(xi-xi0) + r32*(eta-eta0) - r33*c);

                double X = X0 + (Z - Z0) * kx;
                double Y = Y0 + (Z - Z0) * ky;

                result.setX(X);
                result.setY(Y);
                result.setZ(Z);
                result.setAvailable(true);

                return result;
        }
        else
        {
                return ObjectSpaceCoordinate();
        }
}

ObjectSpaceCoordinate ProjectiveRay::detectorToObject(DetectorSpaceCoordinate myAnalogCoordinate, double Z)
{
        return detectorToObject(myAnalogCoordinate.getXi(), myAnalogCoordinate.getEta(), Z);
}

// Note: this only works with fixed Z value!
PositionMatrix ProjectiveRay::detectorToObject(const PositionMatrix& myAnalogPositions, double Z)
{
        if (myImage != NULL && myImage->getSensor() != NULL && myImage->getEO() != NULL && myAnalogPositions.getRows()%2 == 0)
        {
                PositionMatrix result(myAnalogPositions.getRows() * 3 / 2, "m");
                ObjectSpaceCoordinate objectCoordinate;

                unsigned int i, j;
                for (i = 1, j = 1; i+1 <= myAnalogPositions.getRows(); i += 2, j += 3)
                {
                        objectCoordinate = detectorToObject(myAnalogPositions.get(i),myAnalogPositions.get(i+1),Z);
                        result.set(j,objectCoordinate.getX());
                        result.set(j+1,objectCoordinate.getY());
                        result.set(j+2,objectCoordinate.getZ());
                }

                return result;
        }
        else
        {
                return PositionMatrix();
        }
}

// Note: for this one, the analogic positions matrix must have the Z value for each coordinate!
PositionMatrix ProjectiveRay::detectorToObject(const PositionMatrix& myAnalogPositionsWithZ)
{
        if (myImage != NULL && myImage->getSensor() != NULL && myImage->getEO() != NULL)
        {
                PositionMatrix result(myAnalogPositionsWithZ.getRows(), "m");
                ObjectSpaceCoordinate objectCoordinate;

                for (unsigned int i = 1; i+2 <= myAnalogPositionsWithZ.getRows(); i += 3)
                {
                        objectCoordinate = detectorToObject(myAnalogPositionsWithZ.get(i),myAnalogPositionsWithZ.get(i+1),myAnalogPositionsWithZ.get(i+2));
                        result.set(i,objectCoordinate.getX());
                        result.set(i+1,objectCoordinate.getY());
                        result.set(i+2,objectCoordinate.getZ());
                }

                return result;
        }
        else
        {
                return PositionMatrix();
        }
}

DetectorSpaceCoordinate ProjectiveRay::objectToDetector(double X, double Y, double Z)
{
        if (myImage != NULL && myImage->getSensor() != NULL && myImage->getEO() != NULL)
        {
                DetectorSpaceCoordinate result;

                if (!hasRotations)
                        calculateRotations();

                double xi0 = myImage->getSensor()->getPrincipalPointCoordinates().getXi();
                double eta0 = myImage->getSensor()->getPrincipalPointCoordinates().getEta();
                double c = myImage->getSensor()->getFocalDistance();

                Matrix Xa = myImage->getEO()->getXa();
                double X0 = Xa.get(1,1);
                double Y0 = Xa.get(2,1);
                double Z0 = Xa.get(3,1);

                double xi = xi0 - c*(r11*(X-X0)+r21*(Y-Y0)+r31*(Z-Z0)) / (r13*(X-X0)+r23*(Y-Y0)+r33*(Z-Z0));
                double eta = eta0 - c*(r12*(X-X0)+r22*(Y-Y0)+r32*(Z-Z0)) / (r13*(X-X0)+r23*(Y-Y0)+r33*(Z-Z0));

                result.setXi(xi);
                result.setEta(eta);
                result.setAvailable(true);

                return result;
        }
        else
        {
                return DetectorSpaceCoordinate();
        }
}

DetectorSpaceCoordinate ProjectiveRay::objectToDetector(ObjectSpaceCoordinate myObjectCoordinate)
{
        return objectToDetector(myObjectCoordinate.getX(), myObjectCoordinate.getY(), myObjectCoordinate.getZ());
}

PositionMatrix ProjectiveRay::objectToDetector(const PositionMatrix& myObjectPositions)
{
        if (myImage != NULL && myImage->getSensor() != NULL && myImage->getEO() != NULL && myObjectPositions.getRows()%3 == 0)
        {
                PositionMatrix result(myObjectPositions.getRows() * 2 / 3, "mm");
                DetectorSpaceCoordinate analogCoordinate;

                unsigned int i, j;
                for (i = 1, j = 1; i+2 <= myObjectPositions.getRows(); i += 3, j += 2)
                {
                        analogCoordinate = objectToDetector(myObjectPositions.get(i),myObjectPositions.get(i+1),myObjectPositions.get(i+2));
                        result.set(j,analogCoordinate.getXi());
                        result.set(j+1,analogCoordinate.getEta());
                }

                return result;
        }
        else
        {
                return PositionMatrix();
        }
}

DetectorSpaceCoordinate ProjectiveRay::applyDistortions(double xi, double eta)
{
        // Por enquanto estou aplicando todas. Crio a flag depois.

        DetectorSpaceCoordinate radial = getRadialDistortions(xi, eta);
        DetectorSpaceCoordinate decentered = getRadialDistortions(xi, eta);

        double xiLens = xi + radial.getXi() + decentered.getXi();
        double etaLens = eta + radial.getEta() + decentered.getEta();

        DetectorSpaceCoordinate atmosphere = getAtmosphereDistortions(xiLens, etaLens);
        DetectorSpaceCoordinate curvature = getCurvatureDistortions(xiLens, etaLens);

        double xiFinal = xiLens + atmosphere.getXi() + curvature.getXi();
        double etaFinal = etaLens + atmosphere.getEta() + curvature.getEta();

        DetectorSpaceCoordinate result;
        result.setXi(xiFinal);
        result.setEta(etaFinal);
        result.setAvailable(true);

        return result;
}

DetectorSpaceCoordinate ProjectiveRay::applyDistortions(DetectorSpaceCoordinate myAnalogCoordinate)
{
        return applyDistortions(myAnalogCoordinate.getXi(), myAnalogCoordinate.getEta());
}

PositionMatrix ProjectiveRay::applyDistortions(const PositionMatrix& myAnalogPositions)
{
        if (myImage != NULL && myImage->getSensor() != NULL && myImage->getEO() != NULL)
        {
                PositionMatrix result(myAnalogPositions.getRows(), "mm");
                DetectorSpaceCoordinate analogCoordinate;

                for (unsigned int i = 1; i+1 <= myAnalogPositions.getRows(); i += 2)
                {
                        analogCoordinate = applyDistortions(myAnalogPositions.get(i),myAnalogPositions.get(i+1));
                        result.set(i,analogCoordinate.getXi());
                        result.set(i+1,analogCoordinate.getEta());
                }

                return result;
        }
        else
        {
                return PositionMatrix();
        }
}

// Todo: Solve urgently: The method must return the corrected coordinates.
DetectorSpaceCoordinate ProjectiveRay::removeDistortions(double xi, double eta)
{
        return DetectorSpaceCoordinate(0,"",xi,eta);
}

DetectorSpaceCoordinate ProjectiveRay::removeDistortions(DetectorSpaceCoordinate myAnalogCoordinate)
{
        return removeDistortions(myAnalogCoordinate.getXi(), myAnalogCoordinate.getEta());
}

PositionMatrix ProjectiveRay::removeDistortions(const PositionMatrix& myAnalogPositions)
{
        if (myImage != NULL && myImage->getSensor() != NULL && myImage->getEO() != NULL)
        {
                PositionMatrix result(myAnalogPositions.getRows(), "mm");
                DetectorSpaceCoordinate analogCoordinate;

                for (unsigned int i = 1; i+1 <= myAnalogPositions.getRows(); i += 2)
                {
                        analogCoordinate = removeDistortions(myAnalogPositions.get(i),myAnalogPositions.get(i+1));
                        result.set(i,analogCoordinate.getXi());
                        result.set(i+1,analogCoordinate.getEta());
                }

                return result;
        }
        else
        {
                return PositionMatrix();
        }
}

ObjectSpaceCoordinate ProjectiveRay::imageToObject(double col, double lin, double Z, bool useDistortions)
{
        if (myImage != NULL && myImage->getSensor() != NULL && myImage->getIO() != NULL && myImage->getEO() != NULL)
        {
                DetectorSpaceCoordinate analog = imageToDetector(col, lin);
                if (useDistortions)
                        analog = applyDistortions(analog);
                return detectorToObject(analog, Z);
        }
        else
        {
                return ObjectSpaceCoordinate();
        }
}

ObjectSpaceCoordinate ProjectiveRay::imageToObject(ImageSpaceCoordinate myDigitalCoordinate, double Z, bool useDistortions)
{
        return imageToObject(myDigitalCoordinate.getCol(), myDigitalCoordinate.getLin(), Z, useDistortions);
}

// Note: this only works with fixed Z value!
PositionMatrix ProjectiveRay::imageToObject(const PositionMatrix& myDigitalPositions, double Z, bool useDistortions)
{
        if (myImage != NULL && myImage->getSensor() != NULL && myImage->getIO() != NULL && myImage->getEO() != NULL)
        {
                PositionMatrix myAnalogPositions = imageToDetector(myDigitalPositions);
                if (useDistortions)
                        myAnalogPositions = applyDistortions(myAnalogPositions);
                return detectorToObject(myAnalogPositions, Z);
        }
        else
        {
                return PositionMatrix();
        }
}

// Note: for this one, the digital positions matrix must have the Z value for each coordinate!
PositionMatrix ProjectiveRay::imageToObject(const PositionMatrix& myDigitalPositionsWithZ, bool useDistortions)
{
        if (myImage != NULL && myImage->getSensor() != NULL && myImage->getIO() != NULL && myImage->getEO() != NULL)
        {
                PositionMatrix myAnalogPositions = imageZToDetectorZ(myDigitalPositionsWithZ);
                if (useDistortions)
                        myAnalogPositions = applyDistortionsZ(myAnalogPositions);
                return detectorToObject(myAnalogPositions);
        }
        else
        {
                return PositionMatrix();
        }
}

ImageSpaceCoordinate ProjectiveRay::objectToImage(double X, double Y, double Z, bool useDistortions)
{
        if (myImage != NULL && myImage->getSensor() != NULL && myImage->getIO() != NULL && myImage->getEO() != NULL)
        {
                DetectorSpaceCoordinate analog = objectToDetector(X, Y, Z);
                if (useDistortions)
                        analog = removeDistortions(analog);
                return detectorToImage(analog);
        }
        else
        {
                return ImageSpaceCoordinate();
        }
}

ImageSpaceCoordinate ProjectiveRay::objectToImage(ObjectSpaceCoordinate myObjectCoordinate, bool useDistortions)
{
        return objectToImage(myObjectCoordinate.getX(), myObjectCoordinate.getY(), myObjectCoordinate.getZ(), useDistortions);
}

PositionMatrix ProjectiveRay::objectToImage(const PositionMatrix& myObjectPositions, bool useDistortions)
{
        if (myImage != NULL && myImage->getSensor() != NULL && myImage->getIO() != NULL && myImage->getEO() != NULL)
        {
                PositionMatrix myAnalogPositions = objectToDetector(myObjectPositions);
                if (useDistortions)
                        myAnalogPositions = applyDistortions(myAnalogPositions);
                return detectorToImage(myAnalogPositions);
        }
        else
        {
                return PositionMatrix();
        }
}

// Private support methods

DetectorSpaceCoordinate ProjectiveRay::getRadialDistortions(double xi, double eta)
{
        DetectorSpaceCoordinate result;
        result.setXi(0.0);
        result.setEta(0.0);
        result.setAvailable(true);

        Sensor* sensor = myImage->getSensor();
        FrameSensor* frame = dynamic_cast<FrameSensor*>(sensor);
        if (frame != NULL)
        {
                double r = sqrt(xi*xi+eta*eta);
                std::deque<RadialSymmetricDistortionCoefficient> rs = frame->getRadialSymmetricCoefficients();
                double dr = 0;

                for (unsigned int i = 0; i < rs.size(); i++)
                {
                        dr += rs.at(i).getValue() * pow(r,2*i+1);
                }

                double dx = dr * xi / r;
                double dy = dr * eta / r;

                result.setXi(dx);
                result.setEta(dy);
        }

        return result;
}

DetectorSpaceCoordinate ProjectiveRay::getDecenteredDistortions(double xi, double eta)
{
        DetectorSpaceCoordinate result;
        result.setXi(0.0);
        result.setEta(0.0);
        result.setAvailable(true);

        Sensor* sensor = myImage->getSensor();
        FrameSensor* frame = dynamic_cast<FrameSensor*>(sensor);
        if (frame != NULL)
        {
                double r = sqrt(xi*xi+eta*eta);
                std::deque<DecenteredDistortionCoefficient> dec = frame->getDecenteredCoefficients();
                double P1 = dec.at(0).getValue();
                double P2 = dec.at(1).getValue();

                double dx = P1 * (r*r + 2*xi*xi) + 2*P2*xi*eta;
                double dy = 2*P1*xi*eta + P2 * (r*r + 2*eta*eta);

                result.setXi(dx);
                result.setEta(dy);
        }

        return result;
}

DetectorSpaceCoordinate ProjectiveRay::getAtmosphereDistortions(double xi, double eta)
{
        DetectorSpaceCoordinate result;
        result.setXi(0.0);
        result.setEta(0.0);
        result.setAvailable(true);

        Flight* flight = myImage->getFlight();
        Sensor* sensor = myImage->getSensor();
        if (flight != NULL && sensor != NULL)
        {
                Terrain* terrain = flight->getTerrain();
                if (terrain != NULL)
                {
                        double Z0, Zp;
                        if (myImage->getEO() != NULL)
                        {
                                Z0 = myImage->getEO()->getXa().get(3,1);
                        }
                        else
                        {
                                Z0 = flight->getHeight()+terrain->getMeanAltitude();
                        }
                        Zp = terrain->getMeanAltitude();

                        Z0 /= 1000;
                        Zp /= 1000;

                        double aux1 = Z0 / ((Z0*Z0) - 6*Z0 + 250);
                        double aux2 = Zp / Z0*((Zp*Zp) - 6*Zp + 250);

                        double K = 2410 * 0.000001 * (aux1 - aux2);

                        double r = sqrt(xi*xi+eta*eta);
                        double f = sensor->getFocalDistance();

                        double dr = r * (1 + (r*r/f*f)) * K;

                        double dx = dr * xi / r;
                        double dy = dr * eta / r;

                        result.setXi(dx);
                        result.setEta(dy);
                }
        }

        return result;
}

DetectorSpaceCoordinate ProjectiveRay::getCurvatureDistortions(double xi, double eta)
{
        DetectorSpaceCoordinate result;
        result.setXi(0.0);
        result.setEta(0.0);
        result.setAvailable(true);

        Flight* flight = myImage->getFlight();
        Sensor* sensor = myImage->getSensor();
        if (flight != NULL && sensor != NULL)
        {
                Terrain* terrain = flight->getTerrain();
                if (terrain != NULL)
                {
                        double Z0;
                        if (myImage->getEO() != NULL)
                        {
                                Z0 = myImage->getEO()->getXa().get(3,1);
                        }
                        else
                        {
                                Z0 = flight->getHeight()+terrain->getMeanAltitude();
                        }

                        Z0 /= 1000;

                        double R = 6371004.0; // De onde eu tiro isso? Deixo essa constante?
                        double r = sqrt(xi*xi+eta*eta);
                        double f = sensor->getFocalDistance();

                        double aux = (r*r*Z0)/(2*R*f*f);

                        double dx = xi*aux;
                        double dy = eta*aux;

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
