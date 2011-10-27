/************************************************************************
                        AnalogFiductialMark.cpp
**************************************************************************/

#include "AnalogFiductialMark.h"

// Constructors
//

/**
 * AnalogFiductialMark's default empty constructor
 */
AnalogFiductialMark::AnalogFiductialMark()
{
    id = 0;
    sensorId = 0;
    unit = "";
    xi = 0;
    eta = 0;
    sigmaXi = 0;
    sigmaEta = 0;
    sigmaXiEta = 0;
    available = false;
    sigmaAvailable = false;
}

/**
 *
 */
AnalogFiductialMark::AnalogFiductialMark(int myId, int mySensorId)
{
    id = myId;
    sensorId = mySensorId;
    unit = "";
    xi = 0;
    eta = 0;
    sigmaXi = 0;
    sigmaEta = 0;
    sigmaXiEta = 0;
    available = false;
    sigmaAvailable = false;
}

/**
 *
 */
AnalogFiductialMark::AnalogFiductialMark(int myId, int mySensorId, string myUnit, double myXi, double myEta, double mySigmaXi, double mySigmaEta, double mySigmaXiEta)
{
    id = myId;
    sensorId = mySensorId;
    unit = myUnit;
    xi = myXi;
    eta = myEta;
    sigmaXi = mySigmaXi;
    sigmaEta = mySigmaEta;
    sigmaXiEta = mySigmaXiEta;
    available = true;
    sigmaAvailable = true;
}

/**
 *
 */
AnalogFiductialMark::AnalogFiductialMark(int myId, int mySensorId, const PositionMatrix& myPosition)
{
    id = myId;
    sensorId = mySensorId;
    setPosition(myPosition);
    sigmaAvailable = false;
}

/**
 *
 */
AnalogFiductialMark::AnalogFiductialMark(int myId, int mySensorId, const PositionMatrix& myPosition, const Matrix& myPositionSigmas)
{
    id = myId;
    sensorId = mySensorId;
    setPosition(myPosition);
    setPositionSigmas(myPositionSigmas);
}

/**
 * 
 */
AnalogFiductialMark::~AnalogFiductialMark()
{

}



// Private attribute accessor methods
//

/**
 * Set the value of id
 * @param newId the new value of id
 */
void AnalogFiductialMark::setId(int newId)
{
    id = newId;
}

/**
 * Set the value of sensorId
 * @param newSensorId the new value of sensorId
 */
void AnalogFiductialMark::setSensorId(int newSensorId)
{
    sensorId = newSensorId;
}

/**
 * Get the value of id
 * @return the value of id
 */
int AnalogFiductialMark::getId()
{
    return id;
}

/**
 * Get the value of sensorId
 * @return the value of sensorId
 */
int AnalogFiductialMark::getSensorId()
{
    return sensorId;
}

// EObject methods
//

/**
 *
 */
string AnalogFiductialMark::objectType(void)
{
    stringstream result;
    result << "AnalogFiductialMark " << id;
    return result.str();
}

/**
 *
 */
string AnalogFiductialMark::objectAssociations(void)
{
    return "";
}

/**
 *
 */
bool AnalogFiductialMark::is(string s)
{
    return (s == "AnalogFiductialMark" ? true : false);
}

// XML methods
//

/**
 *
 */
void AnalogFiductialMark::xmlSetData(string xml)
{
    EDomElement root(xml);
    id = Conversion::stringToInt(root.attribute("key"));
    EDomElement xmlPos = root.elementByTagName("gml:pos");
    if (xmlPos.isAvailable())
    {
        deque<double> gmlPosValues = xmlPos.toGmlPos();
        xi = gmlPosValues.at(0);
        eta = gmlPosValues.at(1);
        available = true;
    }
    else
    {
        available = false;
    }
    Matrix sigmaMatrix;
    EDomElement xmlSigma = root.elementByTagName("sigma");
    if (xmlSigma.isAvailable())
    {
        sigmaMatrix.xmlSetData(root.elementByTagName("sigma").elementByTagName("mml:matrix").getContent());
        sigmaAvailable = true;
    }
    else
    {
        sigmaMatrix.identity(2);
        sigmaAvailable = false;
    }
    setPositionSigmas(sigmaMatrix);
}

/**
 *
 */
string AnalogFiductialMark::xmlGetData()
{
    stringstream result;
    result << "<fiductialMark key=\"" << Conversion::intToString(id) << "\">\n";
    if (available)
        result << "<gml:pos>" << Conversion::doubleToString(xi) << " " << Conversion::doubleToString(eta) << "</gml:pos>\n";
    else
        result << "<gml:pos>Not Available</gml:pos>\n";
    if (sigmaAvailable)
        result << "<sigma>\n" << getPositionSigmas().xmlGetData() << "</sigma>\n";
    else
        result << "<sigma>Not Available</sigma>\n";
    result << "</fiductialMark>";
    return result.str();
}
