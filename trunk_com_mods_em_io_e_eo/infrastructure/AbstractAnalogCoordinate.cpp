/************************************************************************
                        AbstractAnalogCoordinate.cpp
**************************************************************************/

#include "AbstractAnalogCoordinate.h"

//Constructors and destructors methods
//

AbstractAnalogCoordinate::~AbstractAnalogCoordinate()
{

}

// Private attribute accessor methods
//

/**
 * Set the value of xi
 * @param newXi the new value of xi
 */
void AbstractAnalogCoordinate::setXi(double newXi)
{
    xi = newXi;
}

/**
 * Set the value of eta
 * @param newEta the new value of eta
 */
void AbstractAnalogCoordinate::setEta(double newEta)
{
    eta = newEta;
}

/**
 * Set the value of sigmaXi
 * @param newSigmaXi the new value of sigmaXi
 */
void AbstractAnalogCoordinate::setSigmaXi(double newSigmaXi)
{
    sigmaXi = newSigmaXi;
}

/**
 * Set the value of sigmaEta
 * @param newSigmaEta the new value of sigmaEta
 */
void AbstractAnalogCoordinate::setSigmaEta(double newSigmaEta)
{
    sigmaEta = newSigmaEta;
}

/**
 * Set the value of sigmaXiEta
 * @param newSigmaXi the new value of sigmaXiEta
 */
void AbstractAnalogCoordinate::setSigmaXiEta(double newSigmaXiEta)
{
    sigmaXi = newSigmaXiEta;
}

/**
 * Set the value of unit
 * @param newUnit the new value of unit
 */
void AbstractAnalogCoordinate::setUnit(string newUnit)
{
    unit = newUnit;
}

/**
 * Get the value of xi
 * @return the value of xi
 */
double AbstractAnalogCoordinate::getXi()
{
    return xi;
}

/**
 * Get the value of eta
 * @return the value of eta
 */
double AbstractAnalogCoordinate::getEta()
{
    return eta;
}

/**
 * Get the value of sigmaXi
 * @return the value of sigmaXi
 */
double AbstractAnalogCoordinate::getSigmaXi()
{
    return sigmaXi;
}

/**
 * Get the value of sigmaEta
 * @return the value of sigmaEta
 */
double AbstractAnalogCoordinate::getSigmaEta()
{
    return sigmaEta;
}

/**
 * Get the value of sigmaXiEta
 * @return the value of sigmaXiEta
 */
double AbstractAnalogCoordinate::getSigmaXiEta()
{
    return sigmaXiEta;
}

/**
 * Get the value of unit
 * @return the value of unit
 */
string AbstractAnalogCoordinate::getUnit()
{
    return unit;
}

// Matrix use methods
//

/**
 *
 * @param newPosition
 */
void AbstractAnalogCoordinate::setPosition(const PositionMatrix& newPosition)
{
    if (newPosition.getRows() == 2 && newPosition.getCols() == 1)
    {
        xi = newPosition.get(1);
        eta = newPosition.get(2);
        unit = newPosition.getUnit();
    }
    available = true;
}

/**
 *
 * @param newPositionSigmas
 */
void AbstractAnalogCoordinate::setPositionSigmas(const Matrix& newPositionSigmas)
{
    if (newPositionSigmas.getRows() == 2 && newPositionSigmas.getCols() == 1)
    {
        sigmaXi = newPositionSigmas.get(1,1);
        sigmaEta = newPositionSigmas.get(2,1);
        sigmaXiEta = 0;
    }
    else if (newPositionSigmas.getRows() == 2 && newPositionSigmas.getCols() == 2)
    {
        sigmaXi = newPositionSigmas.get(1,1);
        sigmaEta = newPositionSigmas.get(2,2);
        sigmaXiEta = newPositionSigmas.get(2,1);
    }
    sigmaAvailable = true;
}

/**
 *
 * @return
 */
PositionMatrix AbstractAnalogCoordinate::getPosition()
{
    PositionMatrix position(2, unit);
    position.set(1,xi);
    position.set(2,eta);
    return position;
}

/**
 *
 * @return
 */
Matrix AbstractAnalogCoordinate::getPositionSigmas()
{
    Matrix positionSigmas(2,2);
    positionSigmas.set(1,1,sigmaXi);
    positionSigmas.set(2,2,sigmaEta);
    positionSigmas.set(1,2,sigmaXiEta);
    positionSigmas.set(2,1,sigmaXiEta);
    return positionSigmas;
}

// EObject methods
//

/**
 *
 */
string AbstractAnalogCoordinate::objectType(void)
{
    return "AbstractAnalogCoordinate";
}
