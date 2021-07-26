/**************************************************************************
	  SensorWithFiducialMarks.cpp
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
#include "SensorWithFiducialMarks.h"

#include <sstream>
#include <iostream>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

// Constructors
//


/**
 * SensorWithFiducialMarks's default empty constructor.
 */
SensorWithFiducialMarks::SensorWithFiducialMarks():FrameSensor()
{

}

/**
 * SensorWithFiducialMarks's default empty constructor.
 */
SensorWithFiducialMarks::SensorWithFiducialMarks(const Sensor& sensor):FrameSensor(sensor)
{

}

/**
 * SensorWithFiducialMarks's default empty constructor.
 */
SensorWithFiducialMarks::SensorWithFiducialMarks(int myId):FrameSensor(myId)
{

}

/**
 * This constructor fills Lb but leaves SigmaLb as a matrix containing ones, so P will be an identity.
 * @param myLb
 */
SensorWithFiducialMarks::SensorWithFiducialMarks(int myId, const Matrix& myLb):FrameSensor(myId)
{
	setLb(myLb);
}

/**
 * This constructor fills both Lb and SigmaLb on creation.
 * If mySigmaLb has a different number of rows and columns than Lb, its values will be set as ones, like in the previous constructor.
 * @param myLb, mySigmaLb
 */
SensorWithFiducialMarks::SensorWithFiducialMarks(int myId, const Matrix& myLb, const Matrix& mySigmaLb):FrameSensor(myId)
{
	setLb(myLb, mySigmaLb);
}



// Private attribute accessor methods
//

/**
 * Set the value of Lb.
 * @param newLb the new value of Lb
 */
void SensorWithFiducialMarks::setLb(const Matrix& newLb)
{
	if (newLb.getRows() >= 8 && newLb.getCols() == 1)
	{
		Lb = newLb;
		SigmaLb.resize(newLb.getRows(), newLb.getCols()).ones();
	}
	else
        std::cerr << "Lb is not in the expected format.";
}

/**
 * Set the value of SigmaLb.
 * @param newSigmaLb the new value of SigmaLb
 */
void SensorWithFiducialMarks::setLb(const Matrix& newLb, const Matrix& newSigmaLb)
{
	if (newLb.getRows() >= 8 && newLb.getCols() == 1)
	{
		Lb = newLb;
		if (newSigmaLb.getRows() == newLb.getRows() && newSigmaLb.getCols() == newLb.getCols())
			SigmaLb = newSigmaLb;
		else if (newSigmaLb.getRows() == newLb.getRows() && newSigmaLb.getCols() == newLb.getRows())
			SigmaLb = newSigmaLb;
		else
		{
			SigmaLb.resize(newLb.getRows(), newLb.getCols()).ones();
            std::cerr << "SigmaLb is not in the expected format.";
		}
	}
	else
        std::cerr << "Lb is not in the expected format.";
}

/**
 * Get the value of Lb.
 * @return the value of Lb
 */
Matrix SensorWithFiducialMarks::getLb()
{
	if (Lb.getCols() == 0)
		marksToLb();
	return Lb;
}

/**
 * Get the value of Lb.
 * @return the value of Lb
 */
Matrix SensorWithFiducialMarks::getSigmaLb()
{
	return SigmaLb;
}



// Composed objects accessor methods
//

/**
 * Set all the values of anaFidMarks deque at once
 * @param newAnaFidMarks a deque with the new values
 */
void SensorWithFiducialMarks::setAnaFidMarks(std::deque<DetectorFiducialMark> newAnaFidMarks)
{
	anaFidMarks = newAnaFidMarks;
}

/**
 * Get all the values of anaFidMarks deque at once
 * @return a deque the values of anaFidMarks
 */
std::deque<DetectorFiducialMark> SensorWithFiducialMarks::getAnaFidMarks()
{
	return anaFidMarks;
}

/**
 * Add one value to anaFidMarks deque
 * @param newAnaFidMark the value to be added
 */
void SensorWithFiducialMarks::putAnaFidMark(DetectorFiducialMark newAnaFidMark)
{
	anaFidMarks.push_back(newAnaFidMark);
}

/**
 * Get one value from anaFidMarks deque
 * @param id the id of the value
 * @return the value of the DetectorFiducialMark
 */
DetectorFiducialMark SensorWithFiducialMarks::getAnaFidMark(int id)
{
	for (unsigned int i = 0; i < anaFidMarks.size(); i++)
		if (anaFidMarks.at(i).getId() == id)
			return anaFidMarks.at(i);
	return DetectorFiducialMark();
}

/**
 * Get one value from anaFidMarks deque
 * @param index the position of the value
 * @return the value of the DetectorFiducialMark
 */
DetectorFiducialMark SensorWithFiducialMarks::getAnaFidMarkAt(unsigned int index)
{
	if (index < anaFidMarks.size())
		return anaFidMarks.at(index);
	return DetectorFiducialMark();
}

/**
 * Count the number of value in anaFidMarks deque
 * @return the number of values
 */
int SensorWithFiducialMarks::countAnaFidMarks()
{
	return anaFidMarks.size();
}

/**
 * Remove one value from anaFidMarks deque
 * @param id the id of the value
 */
void SensorWithFiducialMarks::deleteAnaFidMark(int id)
{
	for (unsigned int i = 0; i < anaFidMarks.size(); i++)
		if (anaFidMarks.at(i).getId() == id)
			anaFidMarks.erase(anaFidMarks.begin()+i);
}

/**
 * Remove one value from anaFidMarks deque
 * @param index the position of the value
 */
void SensorWithFiducialMarks::deleteAnaFidMarkAt(unsigned int index)
{
	if (index < anaFidMarks.size())
		anaFidMarks.erase(anaFidMarks.begin()+index);
}

/**
 * Clear all the values of the anaFidMarks deque
 */
void SensorWithFiducialMarks::clearAnaFidMarks()
{
	anaFidMarks.clear();
}

// EObject methods
//

/**
 *
 */
std::string SensorWithFiducialMarks::objectType(void)
{
    std::stringstream result;
	result << "SensorWithFiducialMarks " << id;
	return result.str();
}

/**
 *
 */
std::string SensorWithFiducialMarks::objectAssociations(void)
{
	return "";
}

/**
 *
 */
bool SensorWithFiducialMarks::is(std::string s)
{
	return (s == "SensorWithFiducialMarks" ? true : false);
}

// XML methods
//

/**
 *
 */
void SensorWithFiducialMarks::xmlSetData(std::string xml)
{
	EDomElement root(xml);
	id = Conversion::stringToInt(root.attribute("key"));
	sensorId = root.elementByTagName("sensorId").toString();
	geometry = root.elementByTagName("geometry").toString();
	detector = root.elementByTagName("detector").toString();
	energySource = root.elementByTagName("energySource").toString();
	calculationMode = root.elementByTagName("calculationMode").toString();

	spectralRangesUnit = root.elementByTagName("spectralRanges").attribute("uom");
    std::deque<EDomElement> xmlSpectralRanges = root.elementsByTagName("spectralRange");
	spectralRanges.clear();
	for (unsigned int i = 0; i < xmlSpectralRanges.size(); i++)
	{
		SpectralRange* spec = new SpectralRange;
		spec->band = Conversion::stringToInt(xmlSpectralRanges.at(i).attribute("band"));
		spec->inferiorLimit = xmlSpectralRanges.at(i).elementByTagName("inferiorLimit").toDouble();
		spec->superiorLimit = xmlSpectralRanges.at(i).elementByTagName("superiorLimit").toDouble();
		spectralRanges.push_back(*spec);
	}

	description = root.elementByTagName("description").toString();
	calibrationCertificateNumber = root.elementByTagName("number").toString();
	calibrationCertificateNumber = root.elementByTagName("dispatch").toString();
	calibrationCertificateExpiration = root.elementByTagName("expiration").toString();

	EDomElement xmlFocalDistance = root.elementByTagName("focalDistance");
	focalDistanceUnit = xmlFocalDistance.attribute("uom");
	focalDistance = xmlFocalDistance.elementByTagName("value").toDouble();
	if (xmlFocalDistance.elementByTagName("sigma").isAvailable())
		focalDistanceSigma = xmlFocalDistance.elementByTagName("sigma").toDouble();
	else
		focalDistanceSigma = 1.0;

    std::deque<EDomElement> xmlRadialSymmetric = root.elementByTagName("radialSymmetric").children();
	rsCoefficients.clear();
	for (unsigned int i = 0; i < xmlRadialSymmetric.size(); i++)
	{
		RadialSymmetricDistortionCoefficient* radial = new RadialSymmetricDistortionCoefficient;
		radial->value = xmlRadialSymmetric.at(i).elementByTagName("value").toDouble();
		if (xmlRadialSymmetric.at(i).elementByTagName("sigma").isAvailable())
			radial->sigma = xmlRadialSymmetric.at(i).elementByTagName("sigma").toDouble();
		else
			radial->sigma = 1.0;
		rsCoefficients.push_back(*radial);
	}

    std::deque<EDomElement> xmlDecentered = root.elementByTagName("decentered").children();
	dCoefficients.clear();
	for (unsigned int i = 0; i < xmlDecentered.size(); i++)
	{
		DecenteredDistortionCoefficient* decentered = new DecenteredDistortionCoefficient;
		decentered->value = xmlDecentered.at(i).elementByTagName("value").toDouble();
		if (xmlDecentered.at(i).elementByTagName("sigma").isAvailable())
			decentered->sigma = xmlDecentered.at(i).elementByTagName("sigma").toDouble();
		else
			decentered->sigma = 1.0;
		dCoefficients.push_back(*decentered);
	}

	principalPointCoordinates.xmlSetData(root.elementByTagName("principalPointCoordinates").getContent());

    std::deque<EDomElement> xmlFiducial = root.elementsByTagName("fiducialMark");
	anaFidMarks.clear();
	for (unsigned int i = 0; i < xmlFiducial.size(); i++)
	{
		DetectorFiducialMark* fiducial = new DetectorFiducialMark;
		fiducial->xmlSetData(xmlFiducial.at(i).getContent());
		anaFidMarks.push_back(*fiducial);
	}
}

/**
 *
 */
std::string SensorWithFiducialMarks::xmlGetData()
{
    std::stringstream result;
	result << "<sensor key=\"" << Conversion::intToString(id) << "\">\n";
	result << "<sensorId>" << sensorId << "</sensorId>\n";
	result << "<type>\n";
	result << "<geometry>" << geometry << "</geometry>\n";
	result << "<platform>aerial</platform>\n";
	result << "<detector>" << detector << "</detector>\n";
	result << "<energySource>" << energySource << "</energySource>\n";
	result << "<calculationMode>" << calculationMode << "</calculationMode>\n";
	result << "<spectralRanges uom=\"" << spectralRangesUnit << "\">\n";
	for (unsigned int i = 0; i < spectralRanges.size(); i++)
	{
		result << "<spectralRange band=\"" << Conversion::intToString(spectralRanges.at(i).band) << "\">\n";
		result << "<inferiorLimit>" << Conversion::doubleToString(spectralRanges.at(i).inferiorLimit) << "</inferiorLimit>\n";
		result << "<superiorLimit>" << Conversion::doubleToString(spectralRanges.at(i).superiorLimit) << "</superiorLimit>\n";
		result << "</spectralRange>\n";
	}
	result << "</spectralRanges>\n";
	result << "</type>\n";
	result << "<description>" << description << "</description>\n";
	result << "<calibrationCertificate>\n";
	result << "<number>" << calibrationCertificateNumber << "</number>\n";
	result << "<expiration>" << calibrationCertificateExpiration << "</expiration>\n";
	result << "</calibrationCertificate>\n";
	result << "<focalDistance uom=\"" << focalDistanceUnit << "\">\n";
	result << "<value>" << Conversion::doubleToString(focalDistance) << "</value>\n";
	if (focalDistanceSigma == 1.0)
		result << "<sigma>Not Available</sigma>\n";
	else
		result << "<sigma>" << Conversion::doubleToString(focalDistanceSigma) << "</sigma>\n";
	result << "</focalDistance>\n";
	result << "<distortionCoefficients>\n";
	result << "<radialSymmetric>\n";
	for (unsigned int i = 0; i < rsCoefficients.size(); i++)
	{
		result << "<k" << i << ">\n";
		result << "<value>" << Conversion::doubleToString(rsCoefficients.at(i).value) << "</value>\n";
		if (rsCoefficients.at(i).sigma == 1.0)
			result << "<sigma>Not Available</sigma>\n";
		else
			result << "<sigma>" << Conversion::doubleToString(rsCoefficients.at(i).sigma) << "</sigma>\n";
		result << "</k" << i << ">\n";
	}
	result << "</radialSymmetric>\n";
	result << "<decentered>\n";
	for (unsigned int i = 0; i < dCoefficients.size(); i++)
	{
		result << "<P" << i+1 << ">\n";
		result << "<value>" << Conversion::doubleToString(dCoefficients.at(i).value) << "</value>\n";
		if (dCoefficients.at(i).sigma == 1.0)
			result << "<sigma>Not Available</sigma>\n";
		else
			result << "<sigma>" << Conversion::doubleToString(dCoefficients.at(i).sigma) << "</sigma>\n";
		result << "</P" << i+1 << ">\n";
	}
	result << "</decentered>\n";
	result << "</distortionCoefficients>\n";
	result << "<principalPointCoordinates uom=\"" << principalPointCoordinates.getUnit() << "\">\n";
	result << principalPointCoordinates.xmlGetData();
	result << "</principalPointCoordinates>\n";
	result << "<fiducialMarks uom=\"" << anaFidMarks.at(1).getUnit() << "\">\n";
	for (unsigned int i = 0; i < anaFidMarks.size(); i++)
	{
		result << anaFidMarks.at(i).xmlGetData() << "\n";
	}
	result << "</fiducialMarks>\n";
	result << "</sensor>\n";
	return result.str();
}

// Other methods
//

/**
 * Generate Lb from anaFidMarks
 */
void SensorWithFiducialMarks::marksToLb()
{
	Lb.resize(anaFidMarks.size() * 2,1);
	SigmaLb.resize(anaFidMarks.size() * 2,anaFidMarks.size() * 2).zero();
	for(unsigned int i = 0; i < anaFidMarks.size(); i++)
	{
		Lb.set(i*2+1,1,anaFidMarks.at(i).getXi());
		Lb.set(i*2+2,1,anaFidMarks.at(i).getEta());
		SigmaLb.set(i*2+1,i*2+1,anaFidMarks.at(i).getSigmaXi());
		SigmaLb.set(i*2+2,i*2+2,anaFidMarks.at(i).getSigmaEta());
		SigmaLb.set(i*2+1,i*2+2,anaFidMarks.at(i).getSigmaXiEta());
		SigmaLb.set(i*2+2,i*2+1,anaFidMarks.at(i).getSigmaXiEta());
	}
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
