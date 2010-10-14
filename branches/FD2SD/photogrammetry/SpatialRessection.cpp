/**************************************************************************
                        SpatialRessection.cpp
**************************************************************************/

#include "SpatialRessection.h"
#include "Image.h"
#include "Point.h"
#include "InteriorOrientation.h"
#include "Sensor.h"
#include "Terrain.h"


// Constructors and destructors
//

/**
 *
 */
SpatialRessection::SpatialRessection()
{
    pointForFlightDirectionAvailable = false;
    totalIterations = 0;
    converged = false;
}

/**
 *
 */
SpatialRessection::SpatialRessection(int myImageId) // Constructor with ids only, needed in project use.
{
    imageId = myImageId;
    pointForFlightDirectionAvailable = false;
    totalIterations = 0;
    converged = false;
}

/**
 *
 */
SpatialRessection::~SpatialRessection()
{

}


// Private attribute accessors
//

/**
 * Set the value of Xa
 * This method is only used by the Mounter.
 * @param newXa the new value of Xa
 */
void SpatialRessection::setXa(const Matrix& newXa)
{
    Xa = newXa;
}

/**
 * Set the value of La
 * This method is only used by the Mounter.
 * @param newLa the new value of La
 */
void SpatialRessection::setLa(const Matrix& newLa)
{
    La = newLa;
}

/**
 * Set the value of A
 * This method is only used by the Mounter.
 * @param newA the new value of A
 */
void SpatialRessection::setA(const Matrix& newA)
{
    A = newA;
}

/**
 * Set the value of P
 * This method is only used by the Mounter.
 * @param newP the new value of P
 */
void SpatialRessection::setP(const Matrix& newP)
{
    P = newP;
}

/**
 * Set the value of X0
 * This method is only used by the Mounter.
 * @param newX0 the new value of X0
 */
void SpatialRessection::setX0(const Matrix& newX0)
{
    X0 = newX0;
}

/**
 * Set the value of L0
 * This method is only used by the Mounter.
 * @param newL0 the new value of L0
 */
void SpatialRessection::setL0(const Matrix& newL0)
{
    L0 = newL0;
}

/**
 * Set the value of Lb
 * This method is only used by the Mounter.
 * @param newLb the new value of Lb
 */
void SpatialRessection::setLb(const Matrix& newLb)
{
    Lb = newLb;
}

/**
 * Get the value of Xa
 * @return the value of Xa
 */
Matrix SpatialRessection::getXa()
{
    return Xa;
}

/**
 * Get the value of La
 * @return the value of La
 */
Matrix SpatialRessection::getLa()
{
    return La;
}

/**
 * Get the value of A
 * @return the value of A
 */
Matrix SpatialRessection::getA()
{
    return A;
}

/**
 * Get the value of P
 * @return the value of P
 */
Matrix SpatialRessection::getP()
{
    return P;
}

/**
 * Get the value of X0
 * @return the value of X0
 */
Matrix SpatialRessection::getX0()
{
    return X0;
}

/**
 * Get the value of L0
 * @return the value of L0
 */
Matrix SpatialRessection::getL0()
{
    return L0;
}

/**
 * Get the value of Lb
 * @return the value of Lb
 */
Matrix SpatialRessection::getLb()
{
    return Lb;
}

/**
 * Get the value of lastL0
 * @return the value of lastL0
 */
Matrix SpatialRessection::getLastL0()
{
    return lastL0;
}

/**
 *
 */
deque<int> SpatialRessection::getSelectedPoints()
{
    return selectedPoints;
}

/**
 *
 */
DigitalImageSpaceCoordinate* SpatialRessection::getPointForFlightDirection()
{
    if (pointForFlightDirectionAvailable)
        return &pointForFlightDirection;
    else
        return NULL;
}

int SpatialRessection::getTotalIterations()
{
    return totalIterations;
}

bool SpatialRessection::getConverged()
{
    return converged;
}

// Composed object accessors
//
void SpatialRessection::setQuality(EOQuality newQuality)
{
    myQuality = newQuality;
}

EOQuality SpatialRessection::getQuality()
{
    return myQuality;
}

// Selected points list manipulators
//

void SpatialRessection::selectPoint(int id)
{
    bool insert = true;
    for (unsigned int i = 0; i < selectedPoints.size(); i++)
        if (selectedPoints.at(i) == id)
            insert = false;
    if (insert)
        selectedPoints.push_back(id);
}

void SpatialRessection::unselectPoint(int id)
{
    for (unsigned int i = 0; i < selectedPoints.size(); i++)
        if (selectedPoints.at(i) == id)
            selectedPoints.erase(selectedPoints.begin() + i);
}

void SpatialRessection::unselectAllPoints()
{
    selectedPoints.clear();
}

int SpatialRessection::countSelectedPoints()
{
    return selectedPoints.size();
}

// Selected fiductial mark or point to indicate the direction of flight manipulators
//

void SpatialRessection::setPointForFlightDirection(int col, int lin)
{
    pointForFlightDirection.setCol(col);
    pointForFlightDirection.setLin(lin);
    pointForFlightDirectionAvailable = true;
}

void SpatialRessection::selectFiductialMarkForFlightDirection(int id)
{
    if (myImage != NULL)
    {
        pointForFlightDirection.setCol(myImage->getDigFidMark(id).getCol());
        pointForFlightDirection.setLin(myImage->getDigFidMark(id).getLin());
        pointForFlightDirectionAvailable = true;
    }
}

void SpatialRessection::unsetPointForFlightDirection()
{
    pointForFlightDirectionAvailable = false;
}

// EObject methods
//

/**
 *
 */
string SpatialRessection::objectType(void)
{
    stringstream result;
    result << "SpatialRessection " << imageId;
    return result.str();
}

/**
 *
 */
string SpatialRessection::objectAssociations(void)
{
    return myImage->objectType();
}

/**
 *
 */
bool SpatialRessection::is(string s)
{
    return (s == "SpatialRessection" ? true : false);
}

// XML methods
//

/**
 * 
 */
void SpatialRessection::xmlSetData(string xml)
{
    EDomElement root(xml);
    imageId = stringToInt(root.attribute("image_key"));
    totalIterations = root.elementByTagName("iterations").toInt();
    if (root.elementByTagName("converged").toString() == "true")
        converged = true;
    else
        converged = false;
    Lb.xmlSetData(root.elementByTagName("Lb").elementByTagName("mml:matrix").getContent());

    Xa.resize(6,1);
    EDomElement xmlXa = root.elementByTagName("Xa");
    Xa.set(1,1, xmlXa.elementByTagName("X0").toDouble());
    Xa.set(2,1, xmlXa.elementByTagName("Y0").toDouble());
    Xa.set(3,1, xmlXa.elementByTagName("Z0").toDouble());
    Xa.set(4,1, xmlXa.elementByTagName("phi").toDouble());
    Xa.set(5,1, xmlXa.elementByTagName("omega").toDouble());
    Xa.set(6,1, xmlXa.elementByTagName("kappa").toDouble());

    L0.xmlSetData(root.elementByTagName("L0").elementByTagName("mml:matrix").getContent());

    X0.resize(6,1);
    // Aqui vem a maldi��o do nome igual. Tem um elemento X0 dentro do Xa, e um X0 fora. Quero achar o fora.
    // Eu poderia fazer o acesso diretamente pelo nome dos elementos internos, mas seria dar muitas voltas no XML.
    EDomElement xmlX0;
    deque<EDomElement> xmlX0s = root.elementsByTagName("X0");
    for (unsigned int i = 0; i < xmlX0s.size(); i++)
        if (xmlX0s.at(i).children().size() == 6)
            xmlX0 = xmlX0s.at(i);
    X0.set(1,1, xmlX0.elementByTagName("X00").toDouble());
    X0.set(2,1, xmlX0.elementByTagName("Y00").toDouble());
    X0.set(3,1, xmlX0.elementByTagName("Z00").toDouble());
    X0.set(4,1, xmlX0.elementByTagName("phi0").toDouble());
    X0.set(5,1, xmlX0.elementByTagName("omega0").toDouble());
    X0.set(6,1, xmlX0.elementByTagName("kappa0").toDouble());

    myQuality.xmlSetData(root.elementByTagName("quality").getContent());
}

/**
 * 
 */
string SpatialRessection::xmlGetData()
{
    stringstream result;
    result << "<imageEO type=\"spatialRessection\" image_key=\"" << intToString(imageId) << "\">\n";
    result << "<iterations>" << intToString(totalIterations) << "</iterations>\n";
    if (converged)
        result << "<converged>true</converged>\n";
    else
        result << "<converged>false</converged>\n";
    result << "<parameters>\n";
    result << "<Lb>\n";
    result << Lb.xmlGetData();
    result << "</Lb>\n";
    result << "<Xa>\n";
    result << "<X0 uom=\"#m\">" << doubleToString(Xa.get(1,1)) << "</X0>\n";
    result << "<Y0 uom=\"#m\">" << doubleToString(Xa.get(2,1)) << "</Y0>\n";
    result << "<Z0 uom=\"#m\">" << doubleToString(Xa.get(3,1)) << "</Z0>\n";
    result << "<phi uom=\"#rad\">" << doubleToString(Xa.get(4,1)) << "</phi>\n";
    result << "<omega uom=\"#rad\">" << doubleToString(Xa.get(5,1)) << "</omega>\n";
    result << "<kappa uom=\"#rad\">" << doubleToString(Xa.get(6,1)) << "</kappa>\n";
    result << "</Xa>\n";
    result << "<L0>\n";
    result << L0.xmlGetData();
    result << "</L0>\n";
    result << "<X0>\n";
    result << "<X00 uom=\"#m\">" << doubleToString(X0.get(1,1)) << "</X00>\n";
    result << "<Y00 uom=\"#m\">" << doubleToString(X0.get(2,1)) << "</Y00>\n";
    result << "<Z00 uom=\"#m\">" << doubleToString(X0.get(3,1)) << "</Z00>\n";
    result << "<phi0 uom=\"#rad\">" << doubleToString(X0.get(4,1)) << "</phi0>\n";
    result << "<omega0 uom=\"#rad\">" << doubleToString(X0.get(5,1)) << "</omega0>\n";
    result << "<kappa0 uom=\"#rad\">" << doubleToString(X0.get(6,1)) << "</kappa0>\n";
    result << "</X0>\n";
    result << "</parameters>\n";
    result << myQuality.xmlGetData();
    result << "</imageEO>\n";
    return result.str();
}

// Other methods
//

void SpatialRessection::generateR()
{
    r11 = cos(phi0) * cos(kappa0);
    r12 = -cos(phi0) * sin(kappa0);
    r13 = sin(phi0);
    r21 = cos(omega0) * sin(kappa0) + sin(omega0) * sin(phi0) * cos(kappa0);
    r22 = cos(omega0) * cos(kappa0) - sin(omega0) * sin(phi0) * sin(kappa0);
    r23 = -sin(omega0) * cos(phi0);
    r31 = sin(omega0) * sin(kappa0) - cos(omega0) * sin(phi0) * cos(kappa0);
    r32 = sin(omega0) * cos(kappa0) + cos(omega0) * sin(phi0) * sin(kappa0);
    r33 = cos(omega0) * cos(phi0);
}

void SpatialRessection::generateInitialA()
{
    if (myImage != NULL)
    {
        A.resize(selectedPoints.size() * 2, 6).zero();
        for (unsigned int i = 0, j = 1; i < selectedPoints.size(); i++, j+=2)
        {
            // Essas linhas est�o horr�veis de ler!!! Mas n�o consigo melhorar mais que isso...
            Point* myPoint = myImage->getPoint(selectedPoints.at(i)); // Escolhi aqui o ponto que estou usando.
            AnalogImageSpaceCoordinate myCoordinate = myPoint->getAnalogCoordinate(myImage->getId()); // Tento tirar as coordenadas anal�gicas dele.
            if (myCoordinate.getUnit() == "") // Se essas coordenadas anal�gicas n�o existirem, acusadas pela falta de unidade...
                myCoordinate = myImage->getIO()->digitalToAnalog(myPoint->getDigitalCoordinate(myImage->getId())); // Crio elas usando digitalToAnalog nas coordenadas digitais.

            A.set(j,1,1.0);
            A.set(j,2,myCoordinate.getXi());
            A.set(j,3,myCoordinate.getEta());
            A.set(j+1,4,1.0);
            A.set(j+1,5,myCoordinate.getXi());
            A.set(j+1,6,myCoordinate.getEta());
        }
    }
}

void SpatialRessection::generateInitialL0()
{
    if (myImage != NULL)
    {
        L0.resize(selectedPoints.size() * 2, 1);
        for (unsigned int i = 0, j = 1; i < selectedPoints.size(); i++, j+=2)
        {
            Point* myPoint = myImage->getPoint(selectedPoints.at(i));
            ObjectSpaceCoordinate myCoordinate = myPoint->getObjectCoordinate();

            L0.set(j,1,myCoordinate.getX());
            L0.set(j+1,1,myCoordinate.getY());
        }
    }
}

void SpatialRessection::generateInitialP()
{
    P.identity(selectedPoints.size() * 2);
}

void SpatialRessection::generateA()
{
    if (myImage != NULL)
    {
        A.resize(selectedPoints.size() * 2, 6);
        double c = myImage->getSensor()->getFocalDistance();

        for (unsigned int i = 0, j = 1; i < selectedPoints.size(); i++, j+=2)
        {
            Point* myPoint = myImage->getPoint(selectedPoints.at(i));
            ObjectSpaceCoordinate myCoordinate = myPoint->getObjectCoordinate();
            double X = myCoordinate.getX();
            double Y = myCoordinate.getY();
            double Z = myCoordinate.getZ();

            A.set(j,1,c*cos(kappa0)*cos(phi0)/(sin(phi0)*(X-X00)-cos(phi0)*sin(omega0)*(Y-Y00)+cos(phi0)*cos(omega0)*(Z-Z00))-c*(cos(kappa0)*cos(phi0)*(X-X00)+(sin(kappa0)*cos(omega0)+cos(kappa0)*sin(phi0)*sin(omega0))*(Y-Y00)+(sin(kappa0)*sin(omega0)-cos(kappa0)*sin(phi0)*cos(omega0))*(Z-Z00))/pow((sin(phi0)*(X-X00)-cos(phi0)*sin(omega0)*(Y-Y00)+cos(phi0)*cos(omega0)*(Z-Z00)),2)*sin(phi0));
            A.set(j,2,-c*(-sin(kappa0)*cos(omega0)-cos(kappa0)*sin(phi0)*sin(omega0))/(sin(phi0)*(X-X00)-cos(phi0)*sin(omega0)*(Y-Y00)+cos(phi0)*cos(omega0)*(Z-Z00))+c*(cos(kappa0)*cos(phi0)*(X-X00)+(sin(kappa0)*cos(omega0)+cos(kappa0)*sin(phi0)*sin(omega0))*(Y-Y00)+(sin(kappa0)*sin(omega0)-cos(kappa0)*sin(phi0)*cos(omega0))*(Z-Z00))/pow((sin(phi0)*(X-X00)-cos(phi0)*sin(omega0)*(Y-Y00)+cos(phi0)*cos(omega0)*(Z-Z00)),2)*cos(phi0)*sin(omega0));
            A.set(j,3,-c*(-sin(kappa0)*sin(omega0)+cos(kappa0)*sin(phi0)*cos(omega0))/(sin(phi0)*(X-X00)-cos(phi0)*sin(omega0)*(Y-Y00)+cos(phi0)*cos(omega0)*(Z-Z00))-c*(cos(kappa0)*cos(phi0)*(X-X00)+(sin(kappa0)*cos(omega0)+cos(kappa0)*sin(phi0)*sin(omega0))*(Y-Y00)+(sin(kappa0)*sin(omega0)-cos(kappa0)*sin(phi0)*cos(omega0))*(Z-Z00))/pow((sin(phi0)*(X-X00)-cos(phi0)*sin(omega0)*(Y-Y00)+cos(phi0)*cos(omega0)*(Z-Z00)),2)*cos(phi0)*cos(omega0));
            A.set(j,4,-c*(-cos(kappa0)*sin(phi0)*(X-X00)+cos(kappa0)*cos(phi0)*sin(omega0)*(Y-Y00)-cos(kappa0)*cos(phi0)*cos(omega0)*(Z-Z00))/(sin(phi0)*(X-X00)-cos(phi0)*sin(omega0)*(Y-Y00)+cos(phi0)*cos(omega0)*(Z-Z00))+c*(cos(kappa0)*cos(phi0)*(X-X00)+(sin(kappa0)*cos(omega0)+cos(kappa0)*sin(phi0)*sin(omega0))*(Y-Y00)+(sin(kappa0)*sin(omega0)-cos(kappa0)*sin(phi0)*cos(omega0))*(Z-Z00))/pow((sin(phi0)*(X-X00)-cos(phi0)*sin(omega0)*(Y-Y00)+cos(phi0)*cos(omega0)*(Z-Z00)),2)*(cos(phi0)*(X-X00)+sin(phi0)*sin(omega0)*(Y-Y00)-sin(phi0)*cos(omega0)*(Z-Z00)));
            A.set(j,5,-c*((-sin(kappa0)*sin(omega0)+cos(kappa0)*sin(phi0)*cos(omega0))*(Y-Y00)+(sin(kappa0)*cos(omega0)+cos(kappa0)*sin(phi0)*sin(omega0))*(Z-Z00))/(sin(phi0)*(X-X00)-cos(phi0)*sin(omega0)*(Y-Y00)+cos(phi0)*cos(omega0)*(Z-Z00))+c*(cos(kappa0)*cos(phi0)*(X-X00)+(sin(kappa0)*cos(omega0)+cos(kappa0)*sin(phi0)*sin(omega0))*(Y-Y00)+(sin(kappa0)*sin(omega0)-cos(kappa0)*sin(phi0)*cos(omega0))*(Z-Z00))/pow((sin(phi0)*(X-X00)-cos(phi0)*sin(omega0)*(Y-Y00)+cos(phi0)*cos(omega0)*(Z-Z00)),2)*(-cos(phi0)*cos(omega0)*(Y-Y00)-cos(phi0)*sin(omega0)*(Z-Z00)));
            A.set(j,6,-c*(-sin(kappa0)*cos(phi0)*(X-X00)+(cos(kappa0)*cos(omega0)-sin(kappa0)*sin(phi0)*sin(omega0))*(Y-Y00)+(cos(kappa0)*sin(omega0)+sin(kappa0)*sin(phi0)*cos(omega0))*(Z-Z00))/(sin(phi0)*(X-X00)-cos(phi0)*sin(omega0)*(Y-Y00)+cos(phi0)*cos(omega0)*(Z-Z00)));

            A.set(j+1,1,-c*sin(kappa0)*cos(phi0)/(sin(phi0)*(X-X00)-cos(phi0)*sin(omega0)*(Y-Y00)+cos(phi0)*cos(omega0)*(Z-Z00))-c*(-sin(kappa0)*cos(phi0)*(X-X00)+(cos(kappa0)*cos(omega0)-sin(kappa0)*sin(phi0)*sin(omega0))*(Y-Y00)+(cos(kappa0)*sin(omega0)+sin(kappa0)*sin(phi0)*cos(omega0))*(Z-Z00))/pow((sin(phi0)*(X-X00)-cos(phi0)*sin(omega0)*(Y-Y00)+cos(phi0)*cos(omega0)*(Z-Z00)),2)*sin(phi0));
            A.set(j+1,2,-c*(-cos(kappa0)*cos(omega0)+sin(kappa0)*sin(phi0)*sin(omega0))/(sin(phi0)*(X-X00)-cos(phi0)*sin(omega0)*(Y-Y00)+cos(phi0)*cos(omega0)*(Z-Z00))+c*(-sin(kappa0)*cos(phi0)*(X-X00)+(cos(kappa0)*cos(omega0)-sin(kappa0)*sin(phi0)*sin(omega0))*(Y-Y00)+(cos(kappa0)*sin(omega0)+sin(kappa0)*sin(phi0)*cos(omega0))*(Z-Z00))/pow((sin(phi0)*(X-X00)-cos(phi0)*sin(omega0)*(Y-Y00)+cos(phi0)*cos(omega0)*(Z-Z00)),2)*cos(phi0)*sin(omega0));
            A.set(j+1,3,-c*(-cos(kappa0)*sin(omega0)-sin(kappa0)*sin(phi0)*cos(omega0))/(sin(phi0)*(X-X00)-cos(phi0)*sin(omega0)*(Y-Y00)+cos(phi0)*cos(omega0)*(Z-Z00))-c*(-sin(kappa0)*cos(phi0)*(X-X00)+(cos(kappa0)*cos(omega0)-sin(kappa0)*sin(phi0)*sin(omega0))*(Y-Y00)+(cos(kappa0)*sin(omega0)+sin(kappa0)*sin(phi0)*cos(omega0))*(Z-Z00))/pow((sin(phi0)*(X-X00)-cos(phi0)*sin(omega0)*(Y-Y00)+cos(phi0)*cos(omega0)*(Z-Z00)),2)*cos(phi0)*cos(omega0));
            A.set(j+1,4,-c*(sin(kappa0)*sin(phi0)*(X-X00)-sin(kappa0)*cos(phi0)*sin(omega0)*(Y-Y00)+sin(kappa0)*cos(phi0)*cos(omega0)*(Z-Z00))/(sin(phi0)*(X-X00)-cos(phi0)*sin(omega0)*(Y-Y00)+cos(phi0)*cos(omega0)*(Z-Z00))+c*(-sin(kappa0)*cos(phi0)*(X-X00)+(cos(kappa0)*cos(omega0)-sin(kappa0)*sin(phi0)*sin(omega0))*(Y-Y00)+(cos(kappa0)*sin(omega0)+sin(kappa0)*sin(phi0)*cos(omega0))*(Z-Z00))/pow((sin(phi0)*(X-X00)-cos(phi0)*sin(omega0)*(Y-Y00)+cos(phi0)*cos(omega0)*(Z-Z00)),2)*(cos(phi0)*(X-X00)+sin(phi0)*sin(omega0)*(Y-Y00)-sin(phi0)*cos(omega0)*(Z-Z00)));
            A.set(j+1,5,-c*((-cos(kappa0)*sin(omega0)-sin(kappa0)*sin(phi0)*cos(omega0))*(Y-Y00)+(cos(kappa0)*cos(omega0)-sin(kappa0)*sin(phi0)*sin(omega0))*(Z-Z00))/(sin(phi0)*(X-X00)-cos(phi0)*sin(omega0)*(Y-Y00)+cos(phi0)*cos(omega0)*(Z-Z00))+c*(-sin(kappa0)*cos(phi0)*(X-X00)+(cos(kappa0)*cos(omega0)-sin(kappa0)*sin(phi0)*sin(omega0))*(Y-Y00)+(cos(kappa0)*sin(omega0)+sin(kappa0)*sin(phi0)*cos(omega0))*(Z-Z00))/pow((sin(phi0)*(X-X00)-cos(phi0)*sin(omega0)*(Y-Y00)+cos(phi0)*cos(omega0)*(Z-Z00)),2)*(-cos(phi0)*cos(omega0)*(Y-Y00)-cos(phi0)*sin(omega0)*(Z-Z00)));
            A.set(j+1,6,-c*(-cos(kappa0)*cos(phi0)*(X-X00)+(-sin(kappa0)*cos(omega0)-cos(kappa0)*sin(phi0)*sin(omega0))*(Y-Y00)+(-sin(kappa0)*sin(omega0)+cos(kappa0)*sin(phi0)*cos(omega0))*(Z-Z00))/(sin(phi0)*(X-X00)-cos(phi0)*sin(omega0)*(Y-Y00)+cos(phi0)*cos(omega0)*(Z-Z00)));
        }
    }
}

void SpatialRessection::generateL0()
{
    if (myImage != NULL)
    {
        L0.resize(selectedPoints.size() * 2, 1);
        double c = myImage->getSensor()->getFocalDistance();
        double xi0 = myImage->getSensor()->getPrincipalPointCoordinates().getXi();
        double eta0 = myImage->getSensor()->getPrincipalPointCoordinates().getEta();

        generateR();

        for (unsigned int i = 0, j = 1; i < selectedPoints.size(); i++, j+=2)
        {
            Point* myPoint = myImage->getPoint(selectedPoints.at(i));
            ObjectSpaceCoordinate myCoordinate = myPoint->getObjectCoordinate();
            double X = myCoordinate.getX();
            double Y = myCoordinate.getY();
            double Z = myCoordinate.getZ();

            L0.set(j,1,xi0-c*(r11*(X-X00)+r21*(Y-Y00)+r31*(Z-Z00))/(r13*(X-X00)+r23*(Y-Y00)+r33*(Z-Z00)));
            L0.set(j+1,1,eta0-c*(r12*(X-X00)+r22*(Y-Y00)+r32*(Z-Z00))/(r13*(X-X00)+r23*(Y-Y00)+r33*(Z-Z00)));
        }
    }
}

void SpatialRessection::generateLb()
{
    if (myImage != NULL)
    {
        Lb.resize(selectedPoints.size() * 2, 1);
        for (unsigned int i = 0, j = 1; i < selectedPoints.size(); i++, j+=2)
        {
            // Mesmo c�digo horr�vel do A inicial.
            Point* myPoint = myImage->getPoint(selectedPoints.at(i));
            AnalogImageSpaceCoordinate myCoordinate = myPoint->getAnalogCoordinate(myImage->getId());
            if (myCoordinate.getUnit() == "")
                myCoordinate = myImage->getIO()->digitalToAnalog(myPoint->getDigitalCoordinate(myImage->getId()));

            Lb.set(j,1,myCoordinate.getXi());
            Lb.set(j+1,1,myCoordinate.getEta());
        }
    }
}

void SpatialRessection::generateP()
{
    P.identity(selectedPoints.size() * 2);
}

void SpatialRessection::generateX0()
{
    X0 = Xa;
    X00 = X0.get(1,1);
    Y00 = X0.get(2,1);
    Z00 = X0.get(3,1);
    phi0 = X0.get(4,1);
    omega0 = X0.get(5,1);
    kappa0 = X0.get(6,1);
}

void SpatialRessection::initialize()
{
    if (myImage != NULL && myImage->getSensor() != NULL && myImage->getFlight() != NULL && myImage->getIO() != NULL && pointForFlightDirectionAvailable)
    {
        generateInitialA();
        generateInitialL0();
        generateInitialP();

        // Calculating X00 and Y00.
        double xi0 = myImage->getSensor()->getPrincipalPointCoordinates().getXi();
        double eta0 = myImage->getSensor()->getPrincipalPointCoordinates().getEta();

        // Warning: this Xa is NOT the one containing the EO's parameters.
        // It's just used for determining the initial X00 and Y00 and will be erased at the end of initialization.
        Matrix Xa = (A.transpose() * P * A).inverse() * A.transpose() * P * L0;
        X00 = Xa.get(1,1) + xi0 * Xa.get(2,1) + eta0 * Xa.get(3,1);
        Y00 = Xa.get(4,1) + xi0 * Xa.get(5,1) + eta0 * Xa.get(6,1);

		// Z00 comes from flight height.

		// If flightHeight is the flight's height, this is correct.
		/*
		double mediumPointHeight = 0;
		for (unsigned int i = 0; i < selectedPoints.size(); i++)
			mediumPointHeight += myImage->getPoint(selectedPoints.at(i))->getObjectCoordinate().getZ();
		mediumPointHeight /= (selectedPoints.size());
		Z00 = myImage->getFlight()->getHeight() + mediumPointHeight;
		*/

		//or
		double meanAltitude = myImage->getFlight()->getTerrain()->getMeanAltitude();
		Z00 = myImage->getFlight()->getHeight() + meanAltitude;

		// If flightHeight is the flight's altitude, this is correct.
		//Z00 = myImage->getFlight()->getHeight();

        // Omega0 and phi0 are initially set to 0.
        omega0 = 0;
        phi0 = 0;

        // Calculating kappa0.
        AnalogImageSpaceCoordinate fiductialCoordinate = myImage->getIO()->digitalToAnalog(pointForFlightDirection.getCol(),pointForFlightDirection.getLin());
        double fiductialXi = fiductialCoordinate.getXi();
        double fiductialEta = fiductialCoordinate.getEta();

        double fiductialX = Xa.get(1,1) + fiductialXi * Xa.get(2,1) + fiductialEta * Xa.get(3,1);
        double fiductialY = Xa.get(4,1) + fiductialXi * Xa.get(5,1) + fiductialEta * Xa.get(6,1);

        double deltaX = fiductialX - X00;
        double deltaY = fiductialY - Y00;

        double angle = atan(fabs(deltaY/deltaX));
        if ((deltaX >= 0.0) && (deltaY >= 0.0))
            kappa0 = angle;
        if ((deltaX < 0.0) && (deltaY >= 0.0))
            kappa0 = M_PI - angle;
        if ((deltaX < 0.0) && (deltaY < 0.0))
            kappa0 = M_PI + angle;
        if ((deltaX >= 0.0) && (deltaY < 0.0))
            kappa0 = -angle;

        // Setting the values to X0 and reseting Xa.
        X0.resize(6, 1);
        X0.set(1, 1, X00);
        X0.set(2, 1, Y00);
        X0.set(3, 1, Z00);
        X0.set(4, 1, phi0);
        X0.set(5, 1, omega0);
        X0.set(6, 1, kappa0);

        generateL0();
    }
}

bool SpatialRessection::calculate(int maxIterations, double precision)
{
    converged = false;
    if (myImage != NULL && myImage->getSensor() != NULL && myImage->getFlight() != NULL && myImage->getIO() != NULL && pointForFlightDirectionAvailable)
    {
        int iterations = 0;

        Matrix X0temp, L0temp;
        X0temp = X0;
        L0temp = L0;

        while ((!converged) && (iterations < maxIterations))
        {

            generateLb();
            generateA();
            generateP();

            Xa = X0 - ((A.transpose() * P * A).inverse() * A.transpose() * P * (L0 - Lb));

            converged = true;
            for (int i = 1; i <= 6; i++)
            {
                if (fabs(Xa.get(i,1)-X0.get(i,1))/X0.get(i,1)>precision)
                    converged=false;
            }

            generateX0();
            generateL0();
            iterations++;
        }

        totalIterations = iterations;

        lastL0 = L0;

        X0 = X0temp;
        L0 = L0temp;

        myQuality.calculate(this);
    }
    return converged;
}
