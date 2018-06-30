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
Matrix SensorWithFiducialMarks::getSigmaLb() const
{
	return SigmaLb;
}



// Composed objects accessor methods
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
	id_ = Conversion::stringToInt(root.attribute("key"));
	sensorId_ = root.elementByTagName("sensorId").toString();
	geometry_ = root.elementByTagName("geometry").toString();
	detector_ = root.elementByTagName("detector").toString();
	energySource_ = root.elementByTagName("energySource").toString();
	calculationMode_ = root.elementByTagName("calculationMode").toString();

	spectralRangesUnit_ = root.elementByTagName("spectralRanges").attribute("uom");
    std::deque<EDomElement> xmlSpectralRanges = root.elementsByTagName("spectralRange");
	spectralRanges_.clear();
	for (unsigned int i = 0; i < xmlSpectralRanges.size(); i++)
	{
		SpectralRange* spec = new SpectralRange;
		spec->band = Conversion::stringToInt(xmlSpectralRanges.at(i).attribute("band"));
		spec->inferiorLimit = xmlSpectralRanges.at(i).elementByTagName("inferiorLimit").toDouble();
		spec->superiorLimit = xmlSpectralRanges.at(i).elementByTagName("superiorLimit").toDouble();
		spectralRanges_.push_back(*spec);
	}

	description_ = root.elementByTagName("description").toString();
	calibrationCertificateNumber_ = root.elementByTagName("number").toString();
	calibrationCertificateNumber_ = root.elementByTagName("dispatch").toString();
	calibrationCertificateExpiration_ = root.elementByTagName("expiration").toString();

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
	result << "<sensor key=\"" << Conversion::intToString(id_) << "\">\n";
	result << "<sensorId>" << sensorId_ << "</sensorId>\n";
	result << "<type>\n";
	result << "<geometry>" << geometry_ << "</geometry>\n";
	result << "<platform>aerial</platform>\n";
	result << "<detector>" << detector_ << "</detector>\n";
	result << "<energySource>" << energySource_ << "</energySource>\n";
	result << "<calculationMode>" << calculationMode_ << "</calculationMode>\n";
	result << "<spectralRanges uom=\"" << spectralRangesUnit_ << "\">\n";
	for (unsigned int i = 0; i < spectralRanges_.size(); i++)
	{
		result << "<spectralRange band=\"" << Conversion::intToString(spectralRanges_.at(i).band) << "\">\n";
		result << "<inferiorLimit>" << Conversion::doubleToString(spectralRanges_.at(i).inferiorLimit) << "</inferiorLimit>\n";
		result << "<superiorLimit>" << Conversion::doubleToString(spectralRanges_.at(i).superiorLimit) << "</superiorLimit>\n";
		result << "</spectralRange>\n";
	}
	result << "</spectralRanges>\n";
	result << "</type>\n";
	result << "<description>" << description_ << "</description>\n";
	result << "<calibrationCertificate>\n";
	result << "<number>" << calibrationCertificateNumber_ << "</number>\n";
	result << "<expiration>" << calibrationCertificateExpiration_ << "</expiration>\n";
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
