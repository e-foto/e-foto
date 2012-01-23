/**************************************************************************
	   ProjectiveRay.cpp
**************************************************************************/

#include "ProjectiveRay.h"
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

PositionMatrix ProjectiveRay::digitalZToAnalogZ(const PositionMatrix& myDigitalPositionsWithZ)
{
	if (myImage != NULL && myImage->getIO() != NULL)
	{
		PositionMatrix result(myDigitalPositionsWithZ.getRows(), "mm");
		AnalogImageSpaceCoordinate analogCoordinate;

		for (unsigned int i = 1; i+2 <= myDigitalPositionsWithZ.getRows(); i += 3)
		{
			analogCoordinate = digitalToAnalog(myDigitalPositionsWithZ.getInt(i),myDigitalPositionsWithZ.getInt(i+1));
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

PositionMatrix ProjectiveRay::analogZToDigitalZ(const PositionMatrix& myAnalogPositionsWithZ)
{
	if (myImage != NULL && myImage->getIO() != NULL)
	{
		PositionMatrix result(myAnalogPositionsWithZ.getRows(), "px");
		DigitalImageSpaceCoordinate digitalCoordinate;

		for (unsigned int i = 1; i+2 <= myAnalogPositionsWithZ.getRows(); i += 3)
		{
			digitalCoordinate = analogToDigital(myAnalogPositionsWithZ.get(i),myAnalogPositionsWithZ.get(i+1));
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
		AnalogImageSpaceCoordinate analogCoordinate;

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

AnalogImageSpaceCoordinate ProjectiveRay::digitalToAnalog(unsigned int col, unsigned int lin)
{
	if (myImage != NULL && myImage->getIO() != NULL)
	{
		AnalogImageSpaceCoordinate result;

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
		return AnalogImageSpaceCoordinate();
	}
}

AnalogImageSpaceCoordinate ProjectiveRay::digitalToAnalog(DigitalImageSpaceCoordinate myDigitalCoordinate)
{
	return digitalToAnalog(myDigitalCoordinate.getCol(), myDigitalCoordinate.getLin());
}

PositionMatrix ProjectiveRay::digitalToAnalog(const PositionMatrix& myDigitalPositions)
{
	if (myImage != NULL && myImage->getIO() != NULL)
	{
		PositionMatrix result(myDigitalPositions.getRows(), "mm");
		AnalogImageSpaceCoordinate analogCoordinate;

		for (unsigned int i = 1; i+1 <= myDigitalPositions.getRows(); i += 2)
		{
			analogCoordinate = digitalToAnalog(myDigitalPositions.getInt(i),myDigitalPositions.getInt(i+1));
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

void ProjectiveRay::analogToDigital(double xi, double eta, double &x, double &y)
{
	if (myImage != NULL && myImage->getIO() != NULL)
	{
		DigitalImageSpaceCoordinate result;

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

DigitalImageSpaceCoordinate ProjectiveRay::analogToDigital(double xi, double eta)
{
	if (myImage != NULL && myImage->getIO() != NULL)
	{
		DigitalImageSpaceCoordinate result;

		Matrix Xa = myImage->getIO()->getXa();
		double a0, a1, a2, b0, b1, b2;
		a0 = Xa.get(1,1);
		a1 = Xa.get(2,1);
		a2 = Xa.get(3,1);
		b0 = Xa.get(4,1);
		b1 = Xa.get(5,1);
		b2 = Xa.get(6,1);

		// From Silveira, M.T. - Master Thesis - UERJ, Geomatica 2005
		result.setCol((int)((b2*xi - b2*a0 - a2*eta + b0*a2) / (a1*b2 - b1*a2)));
		result.setLin((int)((a1*eta - a1*b0 - b1*xi + b1*a0) / (a1*b2 - b1*a2)));

		result.setAvailable(true);

		return result;
	}
	else
	{
		return DigitalImageSpaceCoordinate();
	}
}

DigitalImageSpaceCoordinate ProjectiveRay::analogToDigital(AnalogImageSpaceCoordinate myAnalogCoordinate)
{
	return analogToDigital(myAnalogCoordinate.getXi(), myAnalogCoordinate.getEta());
}

PositionMatrix ProjectiveRay::analogToDigital(const PositionMatrix& myAnalogPositions)
{
	if (myImage != NULL && myImage->getIO() != NULL)
	{
		PositionMatrix result(myAnalogPositions.getRows(), "px");
		DigitalImageSpaceCoordinate digitalCoordinate;

		for (unsigned int i = 1; i+1 <= myAnalogPositions.getRows(); i += 2)
		{
			digitalCoordinate = analogToDigital(myAnalogPositions.get(i),myAnalogPositions.get(i+1));
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

ObjectSpaceCoordinate ProjectiveRay::analogToObject(double xi, double eta, double Z)
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

ObjectSpaceCoordinate ProjectiveRay::analogToObject(AnalogImageSpaceCoordinate myAnalogCoordinate, double Z)
{
	return analogToObject(myAnalogCoordinate.getXi(), myAnalogCoordinate.getEta(), Z);
}

// Note: this only works with fixed Z value!
PositionMatrix ProjectiveRay::analogToObject(const PositionMatrix& myAnalogPositions, double Z)
{
	if (myImage != NULL && myImage->getSensor() != NULL && myImage->getEO() != NULL && myAnalogPositions.getRows()%2 == 0)
	{
		PositionMatrix result(myAnalogPositions.getRows() * 3 / 2, "m");
		ObjectSpaceCoordinate objectCoordinate;

		unsigned int i, j;
		for (i = 1, j = 1; i+1 <= myAnalogPositions.getRows(); i += 2, j += 3)
		{
			objectCoordinate = analogToObject(myAnalogPositions.get(i),myAnalogPositions.get(i+1),Z);
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
PositionMatrix ProjectiveRay::analogToObject(const PositionMatrix& myAnalogPositionsWithZ)
{
	if (myImage != NULL && myImage->getSensor() != NULL && myImage->getEO() != NULL)
	{
		PositionMatrix result(myAnalogPositionsWithZ.getRows(), "m");
		ObjectSpaceCoordinate objectCoordinate;

		for (unsigned int i = 1; i+2 <= myAnalogPositionsWithZ.getRows(); i += 3)
		{
			objectCoordinate = analogToObject(myAnalogPositionsWithZ.get(i),myAnalogPositionsWithZ.get(i+1),myAnalogPositionsWithZ.get(i+2));
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

AnalogImageSpaceCoordinate ProjectiveRay::objectToAnalog(double X, double Y, double Z)
{
	if (myImage != NULL && myImage->getSensor() != NULL && myImage->getEO() != NULL)
	{
		AnalogImageSpaceCoordinate result;

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
		return AnalogImageSpaceCoordinate();
	}
}

AnalogImageSpaceCoordinate ProjectiveRay::objectToAnalog(ObjectSpaceCoordinate myObjectCoordinate)
{
	return objectToAnalog(myObjectCoordinate.getX(), myObjectCoordinate.getY(), myObjectCoordinate.getZ());
}

PositionMatrix ProjectiveRay::objectToAnalog(const PositionMatrix& myObjectPositions)
{
	if (myImage != NULL && myImage->getSensor() != NULL && myImage->getEO() != NULL && myObjectPositions.getRows()%3 == 0)
	{
		PositionMatrix result(myObjectPositions.getRows() * 2 / 3, "mm");
		AnalogImageSpaceCoordinate analogCoordinate;

		unsigned int i, j;
		for (i = 1, j = 1; i+2 <= myObjectPositions.getRows(); i += 3, j += 2)
		{
			analogCoordinate = objectToAnalog(myObjectPositions.get(i),myObjectPositions.get(i+1),myObjectPositions.get(i+2));
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

AnalogImageSpaceCoordinate ProjectiveRay::applyDistortions(double xi, double eta)
{
	// Por enquanto estou aplicando todas. Crio a flag depois.

	AnalogImageSpaceCoordinate radial = getRadialDistortions(xi, eta);
	AnalogImageSpaceCoordinate decentered = getRadialDistortions(xi, eta);

	double xiLens = xi + radial.getXi() + decentered.getXi();
	double etaLens = eta + radial.getEta() + decentered.getEta();

	AnalogImageSpaceCoordinate atmosphere = getAtmosphereDistortions(xiLens, etaLens);
	AnalogImageSpaceCoordinate curvature = getCurvatureDistortions(xiLens, etaLens);

	double xiFinal = xiLens + atmosphere.getXi() + curvature.getXi();
	double etaFinal = etaLens + atmosphere.getEta() + curvature.getEta();

	AnalogImageSpaceCoordinate result;
	result.setXi(xiFinal);
	result.setEta(etaFinal);
	result.setAvailable(true);

	return result;
}

AnalogImageSpaceCoordinate ProjectiveRay::applyDistortions(AnalogImageSpaceCoordinate myAnalogCoordinate)
{
	return applyDistortions(myAnalogCoordinate.getXi(), myAnalogCoordinate.getEta());
}

PositionMatrix ProjectiveRay::applyDistortions(const PositionMatrix& myAnalogPositions)
{
	if (myImage != NULL && myImage->getSensor() != NULL && myImage->getEO() != NULL)
	{
		PositionMatrix result(myAnalogPositions.getRows(), "mm");
		AnalogImageSpaceCoordinate analogCoordinate;

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

AnalogImageSpaceCoordinate ProjectiveRay::removeDistortions(double xi, double eta)
{
	return AnalogImageSpaceCoordinate();
}

AnalogImageSpaceCoordinate ProjectiveRay::removeDistortions(AnalogImageSpaceCoordinate myAnalogCoordinate)
{
	return removeDistortions(myAnalogCoordinate.getXi(), myAnalogCoordinate.getEta());
}

PositionMatrix ProjectiveRay::removeDistortions(const PositionMatrix& myAnalogPositions)
{
	if (myImage != NULL && myImage->getSensor() != NULL && myImage->getEO() != NULL)
	{
		PositionMatrix result(myAnalogPositions.getRows(), "mm");
		AnalogImageSpaceCoordinate analogCoordinate;

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

ObjectSpaceCoordinate ProjectiveRay::digitalToObject(unsigned int col, unsigned int lin, double Z, bool useDistortions)
{
	if (myImage != NULL && myImage->getSensor() != NULL && myImage->getIO() != NULL && myImage->getEO() != NULL)
	{
		AnalogImageSpaceCoordinate analog = digitalToAnalog(col, lin);
		if (useDistortions)
			analog = applyDistortions(analog);
		return analogToObject(analog, Z);
	}
	else
	{
		return ObjectSpaceCoordinate();
	}
}

ObjectSpaceCoordinate ProjectiveRay::digitalToObject(DigitalImageSpaceCoordinate myDigitalCoordinate, double Z, bool useDistortions)
{
	return digitalToObject(myDigitalCoordinate.getCol(), myDigitalCoordinate.getLin(), Z, useDistortions);
}

// Note: this only works with fixed Z value!
PositionMatrix ProjectiveRay::digitalToObject(const PositionMatrix& myDigitalPositions, double Z, bool useDistortions)
{
	if (myImage != NULL && myImage->getSensor() != NULL && myImage->getIO() != NULL && myImage->getEO() != NULL)
	{
		PositionMatrix myAnalogPositions = digitalToAnalog(myDigitalPositions);
		if (useDistortions)
			myAnalogPositions = applyDistortions(myAnalogPositions);
		return analogToObject(myAnalogPositions, Z);
	}
	else
	{
		return PositionMatrix();
	}
}

// Note: for this one, the digital positions matrix must have the Z value for each coordinate!
PositionMatrix ProjectiveRay::digitalToObject(const PositionMatrix& myDigitalPositionsWithZ, bool useDistortions)
{
	if (myImage != NULL && myImage->getSensor() != NULL && myImage->getIO() != NULL && myImage->getEO() != NULL)
	{
		PositionMatrix myAnalogPositions = digitalZToAnalogZ(myDigitalPositionsWithZ);
		if (useDistortions)
			myAnalogPositions = applyDistortionsZ(myAnalogPositions);
		return analogToObject(myAnalogPositions);
	}
	else
	{
		return PositionMatrix();
	}
}

DigitalImageSpaceCoordinate ProjectiveRay::objectToDigital(double X, double Y, double Z, bool useDistortions)
{
	if (myImage != NULL && myImage->getSensor() != NULL && myImage->getIO() != NULL && myImage->getEO() != NULL)
	{
		AnalogImageSpaceCoordinate analog = objectToAnalog(X, Y, Z);
		if (useDistortions)
			analog = removeDistortions(analog);
		return analogToDigital(analog);
	}
	else
	{
		return DigitalImageSpaceCoordinate();
	}
}

DigitalImageSpaceCoordinate ProjectiveRay::objectToDigital(ObjectSpaceCoordinate myObjectCoordinate, bool useDistortions)
{
	return objectToDigital(myObjectCoordinate.getX(), myObjectCoordinate.getY(), myObjectCoordinate.getZ(), useDistortions);
}

PositionMatrix ProjectiveRay::objectToDigital(const PositionMatrix& myObjectPositions, bool useDistortions)
{
	if (myImage != NULL && myImage->getSensor() != NULL && myImage->getIO() != NULL && myImage->getEO() != NULL)
	{
		PositionMatrix myAnalogPositions = objectToAnalog(myObjectPositions);
		if (useDistortions)
			myAnalogPositions = applyDistortions(myAnalogPositions);
		return analogToDigital(myAnalogPositions);
	}
	else
	{
		return PositionMatrix();
	}
}

// Private support methods

AnalogImageSpaceCoordinate ProjectiveRay::getRadialDistortions(double xi, double eta)
{
	AnalogImageSpaceCoordinate result;
	result.setXi(0.0);
	result.setEta(0.0);
	result.setAvailable(true);

	Sensor* sensor = myImage->getSensor();
	FrameSensor* frame = dynamic_cast<FrameSensor*>(sensor);
	if (frame != NULL)
	{
		double r = sqrt(xi*xi+eta*eta);
		deque<RadialSymmetricDistortionCoefficient> rs = frame->getRadialSymmetricCoefficients();
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

AnalogImageSpaceCoordinate ProjectiveRay::getDecenteredDistortions(double xi, double eta)
{
	AnalogImageSpaceCoordinate result;
	result.setXi(0.0);
	result.setEta(0.0);
	result.setAvailable(true);

	Sensor* sensor = myImage->getSensor();
	FrameSensor* frame = dynamic_cast<FrameSensor*>(sensor);
	if (frame != NULL)
	{
		double r = sqrt(xi*xi+eta*eta);
		deque<DecenteredDistortionCoefficient> dec = frame->getDecenteredCoefficients();
		double P1 = dec.at(0).getValue();
		double P2 = dec.at(1).getValue();

		double dx = P1 * (r*r + 2*xi*xi) + 2*P2*xi*eta;
		double dy = 2*P1*xi*eta + P2 * (r*r + 2*eta*eta);

		result.setXi(dx);
		result.setEta(dy);
	}

	return result;
}

AnalogImageSpaceCoordinate ProjectiveRay::getAtmosphereDistortions(double xi, double eta)
{
	AnalogImageSpaceCoordinate result;
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

AnalogImageSpaceCoordinate ProjectiveRay::getCurvatureDistortions(double xi, double eta)
{
	AnalogImageSpaceCoordinate result;
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
