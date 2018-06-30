/**************************************************************************
		  InteriorOrientation.cpp
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

#include "InteriorOrientation.h"
#include "SensorWithFiducialMarks.h"
#include "SensorWithKnowDimensions.h"
#include "SensorWithKnowParameters.h"
#include "PositionMatrix.h"
#include "Image.h"

#include <stdlib.h>
#include <sstream>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

InteriorOrientation::InteriorOrientation()
{
}

/**
 * Constructor with ids only, needed in project use.
 */
InteriorOrientation::InteriorOrientation(int myImageId):
    imageId{myImageId}
{
}

InteriorOrientation::~InteriorOrientation()
{

}

/**
 * Get the value of imageId
 * @return the value of imageId
 */
int InteriorOrientation::getImageId() const
{
		return imageId;
}

/**
 * Get the value of Xa
 * @return the value of Xa
 */
Matrix InteriorOrientation::getXa() const
{
		return Xa;
}

/**
 * Get the value of La
 * @return the value of La
 */
Matrix InteriorOrientation::getLa() const
{
		return La;
}

/**
 * Get the value of A
 * @return the value of A
 */
Matrix InteriorOrientation::getA() const
{
		return A;
}

/**
 * Get the value of P
 * @return the value of P
 */
Matrix InteriorOrientation::getP() const
{
		return P;
}


// Composed object accessors
//

/**
 * Get the value of myQuality
 * @return the value of myQuality
 */
IOQuality InteriorOrientation::getQuality() const
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

// XML methods
//

/**
 *
 */
void InteriorOrientation::xmlSetData(std::string xml)
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

        std::deque<EDomElement> xmlFiducialMarks = root.elementsByTagName("fiducialMark");
		if (myImage != NULL)
		{
				myImage->clearDigFidMarks();
				for (unsigned int i = 0; i < xmlFiducialMarks.size(); i++)
				{
						ImageFiducialMark* mark = new ImageFiducialMark;
						mark->xmlSetData(xmlFiducialMarks.at(i).getContent());
						myImage->putDigFidMark(*mark);
				}
		}
}

/**
 *
 */
std::string InteriorOrientation::xmlGetData()
{
        std::stringstream result;
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
		result << "<fiducialMarks uom=\"#px\">\n";
		if (myImage != NULL)
		{
                std::deque<ImageFiducialMark> digFidMarks = myImage->getDigFidMarks();
				for (unsigned int i = 0; i < digFidMarks.size(); i++)
				{
						result << digFidMarks.at(i).xmlGetData();
						result << "\n";
				}
		}
		result << "</fiducialMarks>\n";
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
                SensorWithFiducialMarks* sensor = dynamic_cast<SensorWithFiducialMarks*>(myImage->getSensor());
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
                SensorWithKnowDimensions* sensor = dynamic_cast<SensorWithKnowDimensions*>(myImage->getSensor());
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
                SensorWithKnowParameters* sensor = dynamic_cast<SensorWithKnowParameters*>(myImage->getSensor());
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
        std::deque<ImageFiducialMark> myMarks = myImage->getDigFidMarks();
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
DetectorSpaceCoordinate InteriorOrientation::imageToDetector(double col, double lin)
{
		// For affine transformation
		DetectorSpaceCoordinate result;

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
DetectorSpaceCoordinate InteriorOrientation::imageToDetector(ImageSpaceCoordinate myDigitalCoordinate)
{
		// For affine transformation
		DetectorSpaceCoordinate result;

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
PositionMatrix InteriorOrientation::imageToDetector(const PositionMatrix& myDigitalPositions)
{
		// For affine transformation
		PositionMatrix result(myDigitalPositions.getRows(), "mm");
		DetectorSpaceCoordinate analogCoordinate;

		for (unsigned int i = 1; i <= myDigitalPositions.getRows(); i += 2)
		{
				analogCoordinate = imageToDetector(myDigitalPositions.get(i),myDigitalPositions.get(i+1));
				result.set(i,analogCoordinate.getXi());
				result.set(i+1,analogCoordinate.getEta());
		}

		return result;
}

/**
 * This method transforms analogic coordinates into pixel coordinates.
 * @param x, y
 */
ImageSpaceCoordinate InteriorOrientation::detectorToImage(double x, double y) const
{
		// For affine transformation
		ImageSpaceCoordinate result;
		double a0, a1, a2, b0, b1, b2;
		a0 = Xa.get(1,1);
		a1 = Xa.get(2,1);
		a2 = Xa.get(3,1);
		b0 = Xa.get(4,1);
		b1 = Xa.get(5,1);
		b2 = Xa.get(6,1);

		// From Silveira, M.T. - Master Thesis - UERJ, Geomatica 2005
		result.setCol(((b2*x - b2*a0 - a2*y + b0*a2) / (a1*b2 - b1*a2)));
		result.setLin(((a1*y - a1*b0 - b1*x + b1*a0) / (a1*b2 - b1*a2)));

		return result;
}

/**
 * This overloaded method transforms analogic coordinates into pixel coordinates.
 * @param myAnalogCoordinate
 */
ImageSpaceCoordinate InteriorOrientation::detectorToImage(DetectorSpaceCoordinate myAnalogCoordinate) const
{
		// For affine transformation
		ImageSpaceCoordinate result;
		double a0, a1, a2, b0, b1, b2;
		a0 = Xa.get(1,1);
		a1 = Xa.get(2,1);
		a2 = Xa.get(3,1);
		b0 = Xa.get(4,1);
		b1 = Xa.get(5,1);
		b2 = Xa.get(6,1);

		// From Silveira, M.T. - Master Thesis - UERJ, Geomatica 2005
		result.setCol(((b2*myAnalogCoordinate.getXi() - b2*a0 - a2*myAnalogCoordinate.getEta() + b0*a2) / (a1*b2 - b1*a2)));
		result.setLin(((a1*myAnalogCoordinate.getEta() - a1*b0 - b1*myAnalogCoordinate.getXi() + b1*a0) / (a1*b2 - b1*a2)));

		return result;
}

/**
 * This overloaded method transforms analogic coordinates into pixel coordinates.
 * @param myAnalogPositions
 */
PositionMatrix InteriorOrientation::detectorToImage(const PositionMatrix& myAnalogPositions) const
{
		// For affine transformation
		PositionMatrix result(myAnalogPositions.getRows(), "px");
		ImageSpaceCoordinate digitalCoordinate;

		for (unsigned int i = 1; i <= myAnalogPositions.getRows(); i += 2)
		{
				digitalCoordinate = detectorToImage(myAnalogPositions.get(i),myAnalogPositions.get(i+1));
				result.set(i,digitalCoordinate.getCol());
				result.set(i+1,digitalCoordinate.getLin());
		}

		return result;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
