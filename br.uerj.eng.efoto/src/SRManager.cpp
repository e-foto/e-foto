/**************************************************************************
                               SRManager.cpp
**************************************************************************/

#include "SRManager.h"
#include "EFotoManager.h"
#include "EDom.h"
#include "Terrain.h"
#include "SensorWithFiducialMarks.h"
#include "Image.h"
#include "Point.h"
#include "InteriorOrientation.h"
#include "SpatialRessection.h"
#include "SRUserInterface_Qt.h"

// Constructors and destructors
//

SRManager::SRManager()
{
    manager = NULL;
    mySensor = NULL;
    myFlight = NULL;
    myImage = NULL;
    myIO = NULL;
    mySR = NULL;
	myTerrain = NULL;
    started = false;
    status = false;
}

SRManager::SRManager(EFotoManager* manager, Terrain* myTerrain, Sensor* mySensor, Flight* myFlight, Image* myImage, InteriorOrientation* myIO, SpatialRessection* mySR)
{
    this->manager = manager;
    this->mySensor = mySensor;
    this->myFlight = myFlight;
    this->myImage = myImage;
    this->myIO = myIO;
    this->mySR = mySR;
	this->myTerrain = myTerrain;
    started = false;
    status = false;
}

SRManager::~SRManager()
{

}

// Association Methods
//

void SRManager::setInterface(SRUserInterface* newInterface)
{
    myInterface = newInterface;
}

SRUserInterface* SRManager::getInterface()
{
    return myInterface;
}

// Other Methods
//

bool SRManager::measurePoint(int id, int col, int lin)
{
    if (started)
    {
        Point* pointToMeasure = myImage->getPoint(id);
        if (pointToMeasure != NULL)
        {
            pointToMeasure->putDigitalCoordinate(DigitalImageSpaceCoordinate(myImage->getId(), col, lin));
            mySR->selectPoint(id);
        }
        /* ERRO: ponto fora da imagem. */
        return true;
    }
    return false;
}

void SRManager::selectPoint(int id)
{
    if (started)
    {
        mySR->selectPoint(id);
    }
}

void SRManager::unselectPoint(int id)
{
    if (started)
    {
        mySR->unselectPoint(id);
    }
}

deque<double> SRManager::pointToAnalog(int col, int lin)
{
    deque<double> result;
    result.push_back(myIO->digitalToAnalog(col, lin).getXi());
    result.push_back(myIO->digitalToAnalog(col, lin).getEta());
    return result;
}

bool SRManager::removePoint(int id)
{
    if (started)
    {
        Point* pointToRemove = myImage->getPoint(id);
        if (pointToRemove != NULL)
        {
            mySR->unselectPoint(id);
        }
        /* ERRO: ponto fora da imagem. */
        return true;
    }
    return false;
}

bool SRManager::insertPointOnImage(int id)
{
    if (started)
    {
        Point* pointToInsert = manager->instancePoint(id);
        if (pointToInsert != NULL)
        {
            myImage->putPoint(pointToInsert);
        }
        /* ERRO: ponto n√£o existe. */
        return true;
    }
    return false;
}

bool SRManager::removePointFromImage(int id)
{
    if (started)
    {
        Point* pointToRemove = manager->instancePoint(id);
        if (pointToRemove != NULL)
        {
            pointToRemove->deleteDigitalCoordinate(myImage->getId());
        }
        return true;
        /* ERRO: ponto n√£o existe. */
    }
    return false;
}

deque<string> SRManager::listSelectedPoints()
{
    deque<string> result;
    if (started)
    {
        deque<int> selectedPoints = mySR->getSelectedPoints();
        for (unsigned int i = 0; i < selectedPoints.size(); i++)
        {
            Point* myPoint = myImage->getPoint(selectedPoints.at(i));
            string value = intToString(myPoint->getId());
            value += " ";
			value += myPoint->getPointId();
            value += " ";
            value += myPoint->getDescription();
            result.push_back(value);
        }
    }
    return result;
}

deque<string> SRManager::listImagePoints()
{
    deque<string> result;
    if (started)
    {
        for (int i = 0; i < myImage->countPoints(); i++)
        {
            Point* myPoint = myImage->getPointAt(i);
            string value = intToString(myPoint->getId());
            value += " ";
			value += myPoint->getPointId();
            value += " ";
            value += myPoint->getDescription();
            result.push_back(value);
        }
    }
    return result;
}

deque<string> SRManager::listAllPoints()
{
    deque<string> result;
    if (started)
    {
        EDomElement xmlPoints(manager->getXml("points"));
        deque<EDomElement> allPoints = xmlPoints.children();
        for (unsigned int i = 0; i < allPoints.size(); i++)
        {
            string data = allPoints.at(i).getContent();
            if (data != "")
            {
                string value = allPoints.at(i).attribute("key");
                value += " ";
				value += allPoints.at(i).elementByTagName("pointId").toString();
                value += " ";
                value += allPoints.at(i).elementByTagName("description").toString();
                result.push_back(value);
            }
        }
    }
    return result;
}

deque<string> SRManager::pointData(int index)
{
    deque<string> result;
    if (started)
    {
        Point* myPoint = myImage->getPointAt(index);
        result.push_back(intToString(myPoint->getId()));
		result.push_back(myPoint->getPointId());
        result.push_back(myPoint->getDescription());
		result.push_back(doubleToString(myPoint->getObjectCoordinate().getX(),3));
		result.push_back(doubleToString(myPoint->getObjectCoordinate().getY(),3));
		result.push_back(doubleToString(myPoint->getObjectCoordinate().getZ(),3));
        if (myPoint->hasDigitalCoordinate(myImage->getId()) && myPoint->getDigitalCoordinate(myImage->getId()).isAvailable())
        {
            result.push_back(intToString(myPoint->getDigitalCoordinate(myImage->getId()).getCol()));
            result.push_back(intToString(myPoint->getDigitalCoordinate(myImage->getId()).getLin()));
            AnalogImageSpaceCoordinate aisc = myImage->getIO()->digitalToAnalog(myPoint->getDigitalCoordinate(myImage->getId()));
			result.push_back(doubleToString(aisc.getXi(),3));
			result.push_back(doubleToString(aisc.getEta(),3));
        }
    }
    return result;
}

unsigned int SRManager::countSelectedPoints()
{
    return listSelectedPoints().size();
}

bool SRManager::connectImagePoints()
{
    if (!(started)) /* Sim, esse mÈtodo È executado antes do mÛdulo ser iniciado, e n„o deve ser executado depois. */
    {
        EDomElement xmlPoints(manager->getXml("points"));
        deque<EDomElement> allPoints = xmlPoints.children();
        for (unsigned int i = 0; i < allPoints.size(); i++)
        {
            string data = allPoints.at(i).elementByTagAtt("imageCoordinates", "image_key", intToString(myImage->getId())).getContent();
            if (data != "")
            {
                Point* pointToInsert = manager->instancePoint(stringToInt(allPoints.at(i).attribute("key")));
                if (pointToInsert != NULL)
                {
                    myImage->putPoint(pointToInsert);
                }
            }
        }
        return true;
    }
    return false;
}

bool SRManager::updatePointsTable()
{
    if (started)
    {
        return true;
    }
    return false;
}

bool SRManager::flightDirection(int MarkId)
{
    if (started)
    {
        mySR->selectFiductialMarkForFlightDirection(MarkId);
        return true;
    }
    return false;
}

bool SRManager::flightDirection(int col, int lin)
{
    if (started)
    {
        mySR->setPointForFlightDirection(col, lin);
        return true;
    }
    return false;
}

bool SRManager::calculateSR(int iterations, double gnssPrecision, double insPrecision)
{
    if (started)
    {
        if (mySR->countSelectedPoints() > 3)
        {
            mySR->initialize();
			mySR->calculate(iterations, gnssPrecision, insPrecision);
        }
        return true;
    }
    return false;
}

bool SRManager::exteriorDone()
{
	if (mySR->getXa().getRows() != 6)
		return false;
	return true;
}

deque<string> SRManager::makeReport()
{
    deque<string> result;
    result.push_back(mySR->getXa().xmlGetData());
    result.push_back(mySR->getLb().xmlGetData());
    result.push_back(doubleToString(myIO->getQuality().getsigma0Squared()));
    result.push_back(mySR->getQuality().getV().xmlGetData());
    result.push_back(mySR->getQuality().getSigmaXa().xmlGetData());
    result.push_back(mySR->getQuality().getSigmaLa().xmlGetData());
    result.push_back(intToString(mySR->getTotalIterations()));
    if (mySR->getConverged())
        result.push_back("yes");
    else
        result.push_back("no");
    return result;
}

bool SRManager::exec()
{
    if (manager != NULL && mySensor != NULL && myFlight != NULL && myImage != NULL && myIO != NULL && mySR != NULL)
        if (myImage->getSensorId() == mySensor->getId() && myImage->getFlightId() == myFlight->getId() &&
            myFlight->getSensorId() == mySensor->getId() && myIO->getImageId() == myImage->getId() &&
            mySR->getImageId() == myImage->getId())
        {
		if (manager->getInterfaceType().compare("Qt") == 0)
		{
			//myInterface = new SRUserInterface_Qt(this);
			myInterface = SRUserInterface_Qt::instance(this);
		}
        myImage->setSensor(mySensor);
        myImage->setFlight(myFlight);
        myImage->setIO(myIO);
        myFlight->setSensor(mySensor);
        myIO->setImage(myImage);
        mySR->setImage(myImage);
        connectImagePoints();
        started = true;
		if (myInterface != NULL)
		{
			myInterface->exec();
		}
    }
    return status;
}

void SRManager::stop()
{
	if (myInterface != NULL)
	{
		myInterface->stop();
	}
}

int SRManager::getId()
{
	if (myImage != NULL)
	{
		return myImage->getId();
	}
	return 0;
}

void SRManager::returnProject()
{
	manager->reloadProject();
}

bool SRManager::save(string path)
{
    if (started)
    {
        FILE* pFile;
        string output = "IO state data for Image " + intToString(myImage->getId()) + "\n\n";

        output += mySensor->xmlGetData();
        output += "\n";
        output += myFlight->xmlGetData();
        output += "\n";
        output += myImage->xmlGetData();
        output += "\n";
        output += myIO->xmlGetData();
        output += "\n";
        output += mySR->xmlGetData();
        output += "\n";
        for (unsigned int i = 0; i < myPoints.size(); i++)
        {
            output += myPoints.at(i)->xmlGetData();
            output += "\n";
        }

        EDomElement e(output);
		output = e.indent('\t').getContent();

        const char* buffer = output.c_str();
        pFile = fopen (path.c_str(), "wb");
        fwrite (buffer, 1, output.length(), pFile);
        fclose (pFile);
        return true;
    }
    return false;
}

bool SRManager::load(string path)
{
    if (started)
    {
        FILE* pFile;
        long lSize;
        char* buffer;
        size_t result;

        pFile = fopen (path.c_str(), "rb");
        if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

        fseek (pFile , 0 , SEEK_END);
        lSize = ftell (pFile);
        rewind (pFile);

        buffer = (char*) malloc (sizeof(char)*lSize);
        if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

        result = fread (buffer,1,lSize,pFile);
        if (result != lSize) {fputs ("Reading error",stderr); exit (3);}


        string strxml(buffer);
        EDomElement xml(strxml);

        mySensor->xmlSetData(xml.elementByTagName("Sensor").getContent());
        myFlight->xmlSetData(xml.elementByTagName("Flight").getContent());
        myImage->xmlSetData(xml.elementByTagName("image").getContent());
        myIO->xmlSetData(xml.elementByTagName("imageIO").getContent());
        mySR->xmlSetData(xml.elementByTagName("imageEO").getContent());

        deque<EDomElement> xmlPoints = xml.elementsByTagName("Point");
        for (unsigned int i = 0; i < xmlPoints.size(); i++)
        {

        }

        fclose (pFile);
        free (buffer);
    }
    return false;
}

string SRManager::getImageFile()
{
    if (myImage->getFilepath() == ".")
        return myImage->getFilename();
    else
    {
        string result = "";
        result += myImage->getFilepath();
        result += "/";
        result += myImage->getFilename();
        return result;
    }
}

void SRManager::acceptSR()
{
    EDomElement newXml(manager->xmlGetData());
    if (newXml.elementByTagAtt("imageEO", "image_key", intToString(myImage->getId())).getContent() != "")
        newXml.replaceChildByTagAtt("imageEO", "image_key", intToString(myImage->getId()), mySR->xmlGetData());
    else
        newXml.addChildAtTagName("exteriorOrientation", mySR->xmlGetData());
    int currentPointId;
	for (int i = 0; i < myImage->countPoints(); i++)
    {
        currentPointId = myImage->getPointAt(i)->getId();
        newXml.replaceChildByTagAtt("point", "key", intToString(currentPointId), myImage->getPointAt(i)->xmlGetData());
    }
    manager->xmlSetData(newXml.getContent());
	manager->setSavedState(false);
}
