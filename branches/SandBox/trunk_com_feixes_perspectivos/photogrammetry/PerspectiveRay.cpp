/**************************************************************************
							PerspectiveRay.cpp
**************************************************************************/

#include "PerspectiveRay.h"

// Constructor

PerspectiveRay::PerspectiveRay(Image *myImage)
{
	this->myImage = myImage;
	this->hasRotations = false;
}

// Internal methods

PositionMatrix PerspectiveRay::digitalZToAnalogZ(const PositionMatrix& myDigitalPositionsWithZ)
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

PositionMatrix PerspectiveRay::analogZToDigitalZ(const PositionMatrix& myAnalogPositionsWithZ)
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

PositionMatrix PerspectiveRay::applyDistortionsZ(const PositionMatrix& myAnalogPositionsWithZ)
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

void PerspectiveRay::calculateRotations()
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

double PerspectiveRay::getKx(double xi, double eta)
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

double PerspectiveRay::getKy(double xi, double eta)
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

AnalogImageSpaceCoordinate PerspectiveRay::digitalToAnalog(unsigned int col, unsigned int lin)
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

AnalogImageSpaceCoordinate PerspectiveRay::digitalToAnalog(DigitalImageSpaceCoordinate myDigitalCoordinate)
{
	return digitalToAnalog(myDigitalCoordinate.getCol(), myDigitalCoordinate.getLin());
}

PositionMatrix PerspectiveRay::digitalToAnalog(const PositionMatrix& myDigitalPositions)
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

DigitalImageSpaceCoordinate PerspectiveRay::analogToDigital(double xi, double eta)
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

DigitalImageSpaceCoordinate PerspectiveRay::analogToDigital(AnalogImageSpaceCoordinate myAnalogCoordinate)
{
	return analogToDigital(myAnalogCoordinate.getXi(), myAnalogCoordinate.getEta());
}

PositionMatrix PerspectiveRay::analogToDigital(const PositionMatrix& myAnalogPositions)
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

ObjectSpaceCoordinate PerspectiveRay::analogToObject(double xi, double eta, double Z)
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

ObjectSpaceCoordinate PerspectiveRay::analogToObject(AnalogImageSpaceCoordinate myAnalogCoordinate, double Z)
{
	return analogToObject(myAnalogCoordinate.getXi(), myAnalogCoordinate.getEta(), Z);
}

// Note: this only works with fixed Z value!
PositionMatrix PerspectiveRay::analogToObject(const PositionMatrix& myAnalogPositions, double Z)
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
PositionMatrix PerspectiveRay::analogToObject(const PositionMatrix& myAnalogPositionsWithZ)
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

AnalogImageSpaceCoordinate PerspectiveRay::objectToAnalog(double X, double Y, double Z)
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

AnalogImageSpaceCoordinate PerspectiveRay::objectToAnalog(ObjectSpaceCoordinate myObjectCoordinate)
{
	return objectToAnalog(myObjectCoordinate.getX(), myObjectCoordinate.getY(), myObjectCoordinate.getZ());
}

PositionMatrix PerspectiveRay::objectToAnalog(const PositionMatrix& myObjectPositions)
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

AnalogImageSpaceCoordinate PerspectiveRay::applyDistortions(double xi, double eta)
{

}

AnalogImageSpaceCoordinate PerspectiveRay::applyDistortions(AnalogImageSpaceCoordinate myAnalogCoordinate)
{
	return applyDistortions(myAnalogCoordinate.getXi(), myAnalogCoordinate.getEta());
}

PositionMatrix PerspectiveRay::applyDistortions(const PositionMatrix& myAnalogPositions)
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

AnalogImageSpaceCoordinate PerspectiveRay::removeDistortions(double xi, double eta)
{

}

AnalogImageSpaceCoordinate PerspectiveRay::removeDistortions(AnalogImageSpaceCoordinate myAnalogCoordinate)
{
	return removeDistortions(myAnalogCoordinate.getXi(), myAnalogCoordinate.getEta());
}

PositionMatrix PerspectiveRay::removeDistortions(const PositionMatrix& myAnalogPositions)
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

ObjectSpaceCoordinate PerspectiveRay::digitalToObject(unsigned int col, unsigned int lin, double Z, bool useDistortions)
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

ObjectSpaceCoordinate PerspectiveRay::digitalToObject(DigitalImageSpaceCoordinate myDigitalCoordinate, double Z, bool useDistortions)
{
	return digitalToObject(myDigitalCoordinate.getCol(), myDigitalCoordinate.getLin(), Z, useDistortions);
}

// Note: this only works with fixed Z value!
PositionMatrix PerspectiveRay::digitalToObject(const PositionMatrix& myDigitalPositions, double Z, bool useDistortions)
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
PositionMatrix PerspectiveRay::digitalToObject(const PositionMatrix& myDigitalPositionsWithZ, bool useDistortions)
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

DigitalImageSpaceCoordinate PerspectiveRay::objectToDigital(double X, double Y, double Z, bool useDistortions)
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

DigitalImageSpaceCoordinate PerspectiveRay::objectToDigital(ObjectSpaceCoordinate myObjectCoordinate, bool useDistortions)
{
	return objectToDigital(myObjectCoordinate.getX(), myObjectCoordinate.getY(), myObjectCoordinate.getZ(), useDistortions);
}

PositionMatrix PerspectiveRay::objectToDigital(const PositionMatrix& myObjectPositions, bool useDistortions)
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
