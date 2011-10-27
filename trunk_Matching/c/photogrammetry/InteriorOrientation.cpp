/**************************************************************************
						InteriorOrientation.cpp
**************************************************************************/

#include "InteriorOrientation.h"
#include "SensorWithFiducialMarks.h"
#include "SensorWithKnowDimensions.h"
#include "SensorWithKnowParameters.h"
#include "Image.h"

// Constructors and destructors
//

/**
 *
 */
InteriorOrientation::InteriorOrientation()
{

}

/**
 *
 */
InteriorOrientation::InteriorOrientation(int myImageId) // Constructor with ids only, needed in project use.
{
	imageId = myImageId;
}

/**
 *
 */
InteriorOrientation::~InteriorOrientation()
{

}



// Private attribute accessors
//

/**
 * Get the value of imageId
 * @return the value of imageId
 */
int InteriorOrientation::getImageId()
{
	return imageId;
}

/**
 * Get the value of Xa
 * @return the value of Xa
 */
Matrix InteriorOrientation::getXa()
{
	return Xa;
}

/**
 * Get the value of La
 * @return the value of La
 */
Matrix InteriorOrientation::getLa()
{
	return La;
}

/**
 * Get the value of A
 * @return the value of A
 */
Matrix InteriorOrientation::getA()
{
	return A;
}

/**
 * Get the value of P
 * @return the value of P
 */
Matrix InteriorOrientation::getP()
{
	return P;
}


// Composed object accessors
//

/**
 * Get the value of myQuality
 * @return the value of myQuality
 */
IOQuality InteriorOrientation::getQuality()
{
	return myQuality;
}

// Associated object accessors
//

/**
 *
 */
void InteriorOrientation::setImage(Image *myNewImage)
{
	myImage = myNewImage;
}

Image* InteriorOrientation::getImage()
{
	return myImage;
}

// EObject methods
//

/**
 *
 */
string InteriorOrientation::objectType(void)
{
	stringstream result;
	result << "InteriorOrientation " << imageId;
	return result.str();
}

/**
 *
 */
string InteriorOrientation::objectAssociations(void)
{
	return myImage->objectType();
}

/**
 *
 */
bool InteriorOrientation::is(string s)
{
	return (s == "InteriorOrientation" ? true : false);
}

// XML methods
//

/**
 * 
 */
void InteriorOrientation::xmlSetData(string xml)
{
	EDomElement root(xml);
        imageId = Conversion::stringToInt(root.attribute("image_key"));
	Xa.resize(6,1);
	EDomElement xmlXa = root.elementByTagName("Xa");
	Xa.set(1,1, xmlXa.elementByTagName("a0").toDouble());
	Xa.set(2,1, xmlXa.elementByTagName("a1").toDouble());
	Xa.set(3,1, xmlXa.elementByTagName("a2").toDouble());
	Xa.set(4,1, xmlXa.elementByTagName("b0").toDouble());
	Xa.set(5,1, xmlXa.elementByTagName("b1").toDouble());
	Xa.set(6,1, xmlXa.elementByTagName("b2").toDouble());
	//La.xmlSetData(root.elementByTagName("La"));
	myQuality.xmlSetData(root.elementByTagName("quality").getContent());

	deque<EDomElement> xmlFiductialMarks = root.elementsByTagName("fiductialMark");
	if (myImage != NULL)
	{
		myImage->clearDigFidMarks();
		for (unsigned int i = 0; i < xmlFiductialMarks.size(); i++)
		{
			DigitalFiductialMark* mark = new DigitalFiductialMark;
			mark->xmlSetData(xmlFiductialMarks.at(i).getContent());
			myImage->putDigFidMark(*mark);
		}
	}
}

/**
 * 
 */
string InteriorOrientation::xmlGetData()
{
	stringstream result;
        result << "<imageIO type=\"Affine\" image_key=\"" << Conversion::intToString(imageId) << "\">\n";
	result << "<parameters>\n";
	result << "<Xa>\n";
        result << "<a0>" << Conversion::doubleToString(Xa.get(1,1)) << "</a0>\n";
        result << "<a1>" << Conversion::doubleToString(Xa.get(2,1)) << "</a1>\n";
        result << "<a2>" << Conversion::doubleToString(Xa.get(3,1)) << "</a2>\n";
        result << "<b0>" << Conversion::doubleToString(Xa.get(4,1)) << "</b0>\n";
        result << "<b1>" << Conversion::doubleToString(Xa.get(5,1)) << "</b1>\n";
        result << "<b2>" << Conversion::doubleToString(Xa.get(6,1)) << "</b2>\n";
	result << "</Xa>\n";
	result << "</parameters>\n";
	//result << "<La>\n";
	//result << La.xmlGetData();
	//result << "</La>\n";
	result << myQuality.xmlGetData();
	result << "<fiductialMarks uom=\"#px\">\n";
	if (myImage != NULL)
	{
		deque<DigitalFiductialMark> digFidMarks = myImage->getDigFidMarks();
		for (unsigned int i = 0; i < digFidMarks.size(); i++)
		{
			result << digFidMarks.at(i).xmlGetData();
			result << "\n";
		}
	}
	result << "</fiductialMarks>\n";
	result << "</imageIO>\n";
	return result.str();
}

// Other methods.
//

/**
 * This method calculates the values of the InteriorOrientation's attributes
 * @param mySensorWithFiducialMarks
 */
void InteriorOrientation::calculate()
{
	if (myImage != NULL && myImage->getSensor() != NULL && myImage->getSensor()->is("SensorWithFiducialMarks"))
	{
		SensorWithFiducialMarks* sensor = (SensorWithFiducialMarks*) myImage->getSensor();
		if (sensor!=NULL && myImage->getDigFidMarks().size() >= 4 && myImage->getDigFidMarks().size() <= 8)
		{
			//Generate A from digMarks.
			generateA();

			//Find Lb in SensorWithFiducialMarks.
			Matrix Lb = sensor->getLb();

			//Calculate P.
			Matrix SigmaLb = sensor->getSigmaLb();
			double variance = SigmaLb.highestValue();
			if (abs((long)variance) > 0.000001)
				if (SigmaLb.getCols() == 1)
					P = SigmaLb.toDiagonal() * (1/variance);
			else
				P = SigmaLb * (1/variance); // Isto só é válido se a covariancia é sempre menor do que as variancias em sigmaLb.
			else
				P.identity(Lb.getRows());

			//Calculate Xa.
			Xa = ((A.transpose() * P) * A).inverse() * ((A.transpose() * P) * Lb);

			//Compose myQuality.
			myQuality.calculate(this, myImage->getSensor());

			//Calculate La.
			La = Lb + (myQuality.getV());
		}
	}
	else if (myImage != NULL && myImage->getSensor() != NULL && myImage->getSensor()->is("SensorWithKnowDimensions"))
	{
		SensorWithKnowDimensions* sensor = (SensorWithKnowDimensions*) myImage->getSensor();
		if (sensor!=NULL && myImage->getDigFidMarks().size() == 4)
		{
			//Generate A from digMarks.
			generateA();

			//Forge Lb.
			Matrix Lb = sensor->forgeLb();

			//Calculate P.
			P.identity(Lb.getRows());

			//Calculate Xa.
			Xa = ((A.transpose() * P) * A).inverse() * ((A.transpose() * P) * Lb);

			//Compose myQuality.
			myQuality.calculate(this, myImage->getSensor()); // calculate is not implementade for KnowDimensions.

			//Calculate La.
			La = Lb + (myQuality.getV());
		}
	}
	else if (myImage != NULL && myImage->getSensor() != NULL && myImage->getSensor()->is("SensorWithKnowParameters"))
	{
		SensorWithKnowParameters* sensor = (SensorWithKnowParameters*) myImage->getSensor();
		if (sensor!=NULL)
		{
			//Generate A from digMarks is unnecessary.
			//generateA();

			//Calculate P is unnecessary.
			//Matrix SigmaLb = sensor->getSigmaLb();
			//double variance = SigmaLb.highestValue();
			//if (abs((long)variance) > 0.000001)
			//	if (SigmaLb.getCols() == 1)
			//		P = SigmaLb.toDiagonal() * (1/variance);
			//else
			//	P = SigmaLb * (1/variance); // Isto só é válido se a covariancia é sempre menor do que as variancias em sigmaLb.
			//else
			//	P.identity(Lb.getRows());

			//Calculate Xa.
			Xa = sensor->getXa();

			//Compose myQuality.
			myQuality.calculate(this, myImage->getSensor()); // calculate is not implementade for KnowParameters.

			//Calculate La is unnecessary.
			//La = Lb + (myQuality.getV());
		}
	}
}

/**
 * This method generates the A matrix
 * @return Matrix
 */
void InteriorOrientation::generateA()
{
	Matrix newMatrix;
	deque<DigitalFiductialMark> myMarks = myImage->getDigFidMarks();
	unsigned int size = myMarks.size();

	newMatrix.resize(size * 2, 6).zero();

	for(unsigned int i = 0; i < size; i++)
	{
		newMatrix.set((2*i+1),1,1);
		newMatrix.set((2*i+1),2,myMarks.at(i).getCol());
		newMatrix.set((2*i+1),3,myMarks.at(i).getLin());
		newMatrix.set((2*i+2),4,1);
		newMatrix.set((2*i+2),5,myMarks.at(i).getCol());
		newMatrix.set((2*i+2),6,myMarks.at(i).getLin());
	}

	A = newMatrix;
}

/**
 * This method transforms pixel coordinates into analogic coordinates.
 * @param col, lin
 */
AnalogImageSpaceCoordinate InteriorOrientation::digitalToAnalog(unsigned int col, unsigned int lin)
{
	// For affine transformation
	AnalogImageSpaceCoordinate result;

	result.setXi(Xa.get(1,1) + Xa.get(2,1)*col + Xa.get(3,1)*lin);
	result.setEta(Xa.get(4,1) + Xa.get(5,1)*col + Xa.get(6,1)*lin);

	Matrix positionA(2,6);
	positionA.set(1,1,1);
	positionA.set(1,2,col);
	positionA.set(1,3,lin);
	positionA.set(2,4,1);
	positionA.set(2,5,col);
	positionA.set(2,6,lin);

	Matrix positionSigmas = positionA * myQuality.getSigmaXa() * positionA.transpose();
	result.setPositionSigmas(positionSigmas);

	return result;
}

/**
 * This method transforms pixel coordinates into analogic coordinates.
 * @param myDigitalCoordinate
 */
AnalogImageSpaceCoordinate InteriorOrientation::digitalToAnalog(DigitalImageSpaceCoordinate myDigitalCoordinate)
{
	// For affine transformation
	AnalogImageSpaceCoordinate result;

	result.setXi(Xa.get(1,1) + Xa.get(2,1)*myDigitalCoordinate.getCol() + Xa.get(3,1)*myDigitalCoordinate.getLin());
	result.setEta(Xa.get(4,1) + Xa.get(5,1)*myDigitalCoordinate.getCol() + Xa.get(6,1)*myDigitalCoordinate.getLin());

	Matrix positionA(2,6);
	positionA.set(1,1,1);
	positionA.set(1,2,myDigitalCoordinate.getCol());
	positionA.set(1,3,myDigitalCoordinate.getLin());
	positionA.set(2,4,1);
	positionA.set(2,5,myDigitalCoordinate.getCol());
	positionA.set(2,6,myDigitalCoordinate.getLin());

	Matrix positionSigmas = positionA * myQuality.getSigmaXa() * positionA.transpose();
	result.setPositionSigmas(positionSigmas);

	return result;
}

/**
 * This overloaded method transforms pixel coordinates into analogic coordinates.
 * @param myDigitalPositions
 */
PositionMatrix InteriorOrientation::digitalToAnalog(const PositionMatrix& myDigitalPositions)
{
	// For affine transformation
	PositionMatrix result(myDigitalPositions.getRows(), "mm");
	AnalogImageSpaceCoordinate analogCoordinate;

	for (unsigned int i = 1; i <= myDigitalPositions.getRows(); i += 2)
	{
		analogCoordinate = digitalToAnalog(myDigitalPositions.getInt(i),myDigitalPositions.getInt(i+1));
		result.set(i,analogCoordinate.getXi());
		result.set(i+1,analogCoordinate.getEta());
	}

	return result;
}

/**
 * This method transforms analogic coordinates into pixel coordinates.
 * @param x, y
 */
DigitalImageSpaceCoordinate InteriorOrientation::analogToDigital(double x, double y)
{
	// For affine transformation
	DigitalImageSpaceCoordinate result;
	double a0, a1, a2, b0, b1, b2;
	a0 = Xa.get(1,1);
	a1 = Xa.get(2,1);
	a2 = Xa.get(3,1);
	b0 = Xa.get(4,1);
	b1 = Xa.get(5,1);
	b2 = Xa.get(6,1);

	// From Silveira, M.T. - Master Thesis - UERJ, Geomatica 2005
	result.setCol((int)((b2*x - b2*a0 - a2*y + b0*a2) / (a1*b2 - b1*a2)));
	result.setLin((int)((a1*y - a1*b0 - b1*x + b1*a0) / (a1*b2 - b1*a2)));

	return result;
}

/**
 * This overloaded method transforms analogic coordinates into pixel coordinates.
 * @param myAnalogCoordinate
 */
DigitalImageSpaceCoordinate InteriorOrientation::analogToDigital(AnalogImageSpaceCoordinate myAnalogCoordinate)
{
	// For affine transformation
	DigitalImageSpaceCoordinate result;
	double a0, a1, a2, b0, b1, b2;
	a0 = Xa.get(1,1);
	a1 = Xa.get(2,1);
	a2 = Xa.get(3,1);
	b0 = Xa.get(4,1);
	b1 = Xa.get(5,1);
	b2 = Xa.get(6,1);

	// From Silveira, M.T. - Master Thesis - UERJ, Geomatica 2005
	result.setCol((int)((b2*myAnalogCoordinate.getXi() - b2*a0 - a2*myAnalogCoordinate.getEta() + b0*a2) / (a1*b2 - b1*a2)));
	result.setLin((int)((a1*myAnalogCoordinate.getEta() - a1*b0 - b1*myAnalogCoordinate.getXi() + b1*a0) / (a1*b2 - b1*a2)));

	return result;
}

/**
 * This overloaded method transforms analogic coordinates into pixel coordinates.
 * @param myAnalogPositions
 */
PositionMatrix InteriorOrientation::analogToDigital(const PositionMatrix& myAnalogPositions)
{
	// For affine transformation
	PositionMatrix result(myAnalogPositions.getRows(), "px");
	DigitalImageSpaceCoordinate digitalCoordinate;

	for (unsigned int i = 1; i <= myAnalogPositions.getRows(); i += 2)
	{
		digitalCoordinate = analogToDigital(myAnalogPositions.get(i),myAnalogPositions.get(i+1));
		result.set(i,digitalCoordinate.getCol());
		result.set(i+1,digitalCoordinate.getLin());
	}

	return result;
}
