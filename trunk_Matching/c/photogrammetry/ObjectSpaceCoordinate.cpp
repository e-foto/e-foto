/*******************************************************************************
                        ObjectSpaceCoordinate.cpp
*******************************************************************************/

#include "ObjectSpaceCoordinate.h"

// Constructors
//

/**
 * AnalogImageSpaceCoordinate's default empty constructor
 */
ObjectSpaceCoordinate::ObjectSpaceCoordinate()
{
    available = false;
    sigmaAvailable = false;
}

/**
 *
 */
ObjectSpaceCoordinate::ObjectSpaceCoordinate(string myUnit, double myX, double myY, double myZ, double mySigmaX, double mySigmaY, double mySigmaZ, double mySigmaXY, double mySigmaXZ, double mySigmaYZ)
{
    setUnit(myUnit);
    setX(myX);
    setY(myY);
    setZ(myZ);
    setSigmaX(mySigmaX);
    setSigmaY(mySigmaY);
    setSigmaZ(mySigmaZ);
    setSigmaXY(mySigmaXY);
    setSigmaXZ(mySigmaXZ);
    setSigmaYZ(mySigmaYZ);
    available = true;
    sigmaAvailable = true;
}

/**
 *
 */
ObjectSpaceCoordinate::ObjectSpaceCoordinate(const PositionMatrix& myPosition)
{
    setPosition(myPosition);
    sigmaAvailable = false;
}

/**
 * 
 */
ObjectSpaceCoordinate::ObjectSpaceCoordinate(const PositionMatrix& myPosition, const Matrix& myPositionSigmas)
{
    setPosition(myPosition);
    setPositionSigmas(myPositionSigmas);
}

// Private attribute accessor methods
//

/**
 * Set the value of x
 * @param newX the new value of x
 */
void ObjectSpaceCoordinate::setX(double newX)
{
    x = newX;
}

/**
 * Set the value of y
 * @param newY the new value of y
 */
void ObjectSpaceCoordinate::setY(double newY)
{
    y = newY;
}

/**
 * Set the value of z
 * @param newZ the new value of z
 */
void ObjectSpaceCoordinate::setZ(double newZ)
{
    z = newZ;
}

/**
 * Set the value of sigmaX
 * @param newSigmaX the new value of sigmaX
 */
void ObjectSpaceCoordinate::setSigmaX(double newSigmaX)
{
    sigmaX = newSigmaX;
}

/**
 * Set the value of sigmaY
 * @param newSigmaY the new value of sigmaY
 */
void ObjectSpaceCoordinate::setSigmaY(double newSigmaY)
{
    sigmaY = newSigmaY;
}

/**
 * Set the value of sigmaZ
 * @param newSigmaZ the new value of sigmaZ
 */
void ObjectSpaceCoordinate::setSigmaZ(double newSigmaZ)
{
    sigmaZ = newSigmaZ;
}

/**
 * Set the value of sigmaXY
 * @param newSigmaXY the new value of sigmaXY
 */
void ObjectSpaceCoordinate::setSigmaXY(double newSigmaXY)
{
    sigmaXY = newSigmaXY;
}

/**
 * Set the value of sigmaXZ
 * @param newSigmaXZ the new value of sigmaXZ
 */
void ObjectSpaceCoordinate::setSigmaXZ(double newSigmaXZ)
{
    sigmaXZ = newSigmaXZ;
}

/**
 * Set the value of sigmaYZ
 * @param newSigmaYZ the new value of sigmaYZ
 */
void ObjectSpaceCoordinate::setSigmaYZ(double newSigmaYZ)
{
    sigmaYZ = newSigmaYZ;
}

/**
 * Set the value of unit
 * @param newUnit the new value of unit
 */
void ObjectSpaceCoordinate::setUnit(string newUnit)
{
    unit = newUnit;
}

/**
 * Get the value of x
 * @return the value of x
 */
double ObjectSpaceCoordinate::getX()
{
    return x;
}

/**
 * Get the value of y
 * @return the value of y
 */
double ObjectSpaceCoordinate::getY()
{
    return y;
}

/**
 * Get the value of z
 * @return the value of z
 */
double ObjectSpaceCoordinate::getZ()
{
    return z;
}

/**
 * Get the value of sigmaX
 * @return the value of sigmaX
 */
double ObjectSpaceCoordinate::getSigmaX()
{
    return sigmaX;
}

/**
 * Get the value of sigmaY
 * @return the value of sigmaY
 */
double ObjectSpaceCoordinate::getSigmaY()
{
    return sigmaY;
}

/**
 * Get the value of sigmaZ
 * @return the value of sigmaZ
 */
double ObjectSpaceCoordinate::getSigmaZ()
{
    return sigmaZ;
}

/**
 * Get the value of sigmaXY
 * @return the value of sigmaXY
 */
double ObjectSpaceCoordinate::getSigmaXY()
{
    return sigmaXY;
}

/**
 * Get the value of sigmaXZ
 * @return the value of sigmaXZ
 */
double ObjectSpaceCoordinate::getSigmaXZ()
{
    return sigmaXZ;
}

/**
 * Get the value of sigmaYZ
 * @return the value of sigmaYZ
 */
double ObjectSpaceCoordinate::getSigmaYZ()
{
    return sigmaYZ;
}

/**
 * Get the value of unit
 * @return the value of unit
 */
string ObjectSpaceCoordinate::getUnit()
{
    return unit;
}

// Matrix use methods
//

/**
 *
 * @param newPosition
 */
void ObjectSpaceCoordinate::setPosition(const PositionMatrix& newPosition)
{
    if (newPosition.getRows() == 3 && newPosition.getCols() == 1)
    {
        x = newPosition.get(1);
        y = newPosition.get(2);
        z = newPosition.get(3);
        unit = newPosition.getUnit();
    }
    available = true;
}

/**
 *
 * @param newPositionSigmas
 */
void ObjectSpaceCoordinate::setPositionSigmas(const Matrix& newPositionSigmas)
{
    if (newPositionSigmas.getRows() == 3 && newPositionSigmas.getCols() == 1)
    {
        sigmaX = newPositionSigmas.get(1,1);
        sigmaY = newPositionSigmas.get(2,1);
        sigmaZ = newPositionSigmas.get(3,1);
        sigmaXY = 0;
        sigmaXZ = 0;
        sigmaYZ = 0;
    }
    else if (newPositionSigmas.getRows() == 3 && newPositionSigmas.getCols() == 3)
    {
        sigmaX = newPositionSigmas.get(1,1);
        sigmaY = newPositionSigmas.get(2,2);
        sigmaZ = newPositionSigmas.get(3,3);
        sigmaXY = newPositionSigmas.get(1,2);
        sigmaXZ = newPositionSigmas.get(1,3);
        sigmaYZ = newPositionSigmas.get(2,3);
    }
    sigmaAvailable = true;
}

/**
 *
 * @return
 */
PositionMatrix ObjectSpaceCoordinate::getPosition()
{
    PositionMatrix position(3,unit);
    position.set(1,x);
    position.set(2,y);
    position.set(3,z);
    return position;
}

/**
 *
 * @return
 */
Matrix ObjectSpaceCoordinate::getPositionSigmas()
{
    Matrix positionSigmas(3,3);
    positionSigmas.set(1,1,sigmaX);
    positionSigmas.set(2,2,sigmaY);
    positionSigmas.set(3,3,sigmaZ);
    positionSigmas.set(1,2,sigmaXY);
    positionSigmas.set(2,1,sigmaXY);
    positionSigmas.set(1,3,sigmaXZ);
    positionSigmas.set(3,1,sigmaXZ);
    positionSigmas.set(2,3,sigmaYZ);
    positionSigmas.set(3,2,sigmaYZ);
    return positionSigmas;
}

// EObject methods
//

/**
 *
 */
string ObjectSpaceCoordinate::objectType(void)
{
    return "ObjectSpaceCoordinate";
}

/**
 *
 */
string ObjectSpaceCoordinate::objectAssociations(void)
{
    return "";
}

/**
 *
 */
bool ObjectSpaceCoordinate::is(string s)
{
    return (s == "ObjectSpaceCoordinate" ? true : false);
}

// XML methods
//

/**
 * 
 */
void ObjectSpaceCoordinate::xmlSetData(string xml)
{
    EDomElement root(xml);
    unit = root.attribute("uom");
    EDomElement xmlPos = root.elementByTagName("gml:pos");
    if (xmlPos.isAvailable())
    {
        deque<double> gmlPosValues = root.elementByTagName("gml:pos").toGmlPos();
        x = gmlPosValues.at(0);
        y = gmlPosValues.at(1);
        z = gmlPosValues.at(2);
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
        sigmaMatrix.xmlSetData(xmlSigma.elementByTagName("mml:matrix").getContent());
        sigmaAvailable = true;
    }
    else
    {
        sigmaMatrix.identity(3);
        sigmaAvailable = false;
    }
    setPositionSigmas(sigmaMatrix);
}

/**
 * 
 */
string ObjectSpaceCoordinate::xmlGetData()
{
    stringstream result;
    result << "<spatialCoordinates uom=\"" << unit << "\">\n";
    if (available)
        result << "<gml:pos>" << Conversion::doubleToString(x) << " " << Conversion::doubleToString(y) << " " << Conversion::doubleToString(z) << "</gml:pos>\n";
    else
        result << "<gml:pos>Not Available</gml:pos>\n";
    if (sigmaAvailable)
        result << "<sigma>\n" << getPositionSigmas().xmlGetData() << "</sigma>\n";
    else
        result << "<sigma>Not Available</sigma>\n";
    result << "</spatialCoordinates>";
    return result.str();
}

