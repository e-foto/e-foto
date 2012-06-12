/**************************************************************************
		  IOManager.cpp
**************************************************************************/

#include "IOManager.h"
#include "EFotoManager.h"
#include "IOUserInterface.h"
#include "IOUserInterface_Qt.h"

#ifdef INTEGRATED_EFOTO
#include "SensorWithFiducialMarks.h"
#include "SensorWithKnowDimensions.h"
#include "SensorWithKnowParameters.h"
#include "Image.h"
#include "InteriorOrientation.h"
#endif //INTEGRATED_EFOTO

// Constructors and destructors
//

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

IOManager::IOManager()
{
	manager = NULL;
	mySensor = NULL;
	myImage = NULL;
	myIO = NULL;
	started = false;
	status = false;
}

IOManager::IOManager(EFotoManager* manager, Sensor* mySensor, Image* myImage, InteriorOrientation* myIO)
{
	this->manager = manager;
	this->mySensor = mySensor;
	this->myImage = myImage;
	this->myIO = myIO;
	started = false;
	status = false;
}

IOManager::~IOManager()
{

}

// Association Methods
//

void IOManager::setInterface(IOUserInterface* newInterface)
{
	myInterface = newInterface;
}

IOUserInterface* IOManager::getInterface()
{
	return myInterface;
}

// Other Methods
//

PositionMatrix IOManager::getAnalogMarks()
{
#ifdef INTEGRATED_EFOTO
	if (mySensor != NULL)
	{
		if (mySensor->is("SensorWithFiducialMarks"))
		{
			SensorWithFiducialMarks* mySensorWithFiducialMarks = (SensorWithFiducialMarks*) mySensor;
			return PositionMatrix(mySensorWithFiducialMarks->getLb());
		}
		if (mySensor->is("SensorWithKnowDimensions"))
		{
			SensorWithKnowDimensions* mySensorWithKnowDimensions = (SensorWithKnowDimensions*) mySensor;
			return PositionMatrix(mySensorWithKnowDimensions->forgeLb());
		}
	}
#endif //INTEGRATED_EFOTO
	return PositionMatrix();
}

bool IOManager::measureMark(int id, double col, double lin)
{
	if (started)
	{
#ifdef INTEGRATED_EFOTO
		ImageFiducialMark* myMark = new ImageFiducialMark(id, myImage->getId(), col, lin);
		myImage->deleteDigFidMark(id);
		myImage->putDigFidMark(*myMark);
#endif //INTEGRATED_EFOTO
		return true;
	}
	return false;
}

unsigned int IOManager::countMarks()
{
#ifdef INTEGRATED_EFOTO
	return myImage->countDigFidMarks();
#endif //INTEGRATED_EFOTO
}

deque<string> IOManager::markData(int index)
{
	deque<string> result;
	if (started)
	{
		PositionMatrix analogMarks = getAnalogMarks();

		result.push_back(Conversion::doubleToString(analogMarks.get(2 * index + 1),3));
		result.push_back(Conversion::doubleToString(analogMarks.get(2 * index + 2),3));  
#ifdef INTEGRATED_EFOTO
		ImageFiducialMark myMark = myImage->getDigFidMark(index + 1);
		if (myMark.isAvailable())
		{
			result.push_back(Conversion::doubleToString(myMark.getCol()));
			result.push_back(Conversion::doubleToString(myMark.getLin()));
		}
#endif //INTEGRATED_EFOTO
	}
	return result;
}

unsigned int IOManager::getTotalMarks()
{
	return getAnalogMarks().getRows() / 2;
}

int IOManager::getCalculationMode()
{
#ifdef INTEGRATED_EFOTO
	string mode = mySensor->getCalculationMode();
	return mode == "With Fiducial Marks"? 1 : mode == "With Sensor Dimensions" ? 2 : mode == "Fixed Parameters" ? 3 : 0;
#endif //INTEGRATED_EFOTO
}

bool IOManager::calculateIO()
{
	if (started)
	{
#ifdef INTEGRATED_EFOTO
		if (mySensor->is("SensorWithFiducialMarks"))
		{
			SensorWithFiducialMarks* mySensorWithFiducialMarks = (SensorWithFiducialMarks*) mySensor;
			if ((unsigned int) myImage->countDigFidMarks() == (unsigned int) mySensorWithFiducialMarks->getLb().getRows() / 2) // Comparação entre o número de marcas digitais e analógicas. "mySensorWithFiducialMarks->getLb()->getCols() / 2" dá o número de marcas analógicas validadas, tendo Lb sido carregado direto ou através das marcas.
			{
				myIO->calculate();
				status = true;
			}
		}
		else if (mySensor->is("SensorWithKnowDimensions"))
		{
			//SensorWithKnowDimensions* mySensorWithKnowDimensions = (SensorWithKnowDimensions*) mySensor;
			myIO->calculate();
			status = true;
		}
		else if (mySensor->is("SensorWithKnowParameters"))
		{
			//SensorWithKnowParameters* mySensorWithKnowParameters = (SensorWithKnowParameters*) mySensor;
			myIO->calculate();
			status = true;
		}
#endif //INTEGRATED_EFOTO
	}
	return status;
}

bool IOManager::interiorDone()
{
#ifdef INTEGRATED_EFOTO
	if (myIO->getXa().getRows() != 6)
		return false;
#endif //INTEGRATED_EFOTO
	return true;
}

deque<string> IOManager::makeReport()
{
	deque<string> result;
#ifdef INTEGRATED_EFOTO
	result.push_back(myIO->getXa().xmlGetData());
	result.push_back(myIO->getLa().xmlGetData());
	result.push_back(Conversion::doubleToString(myIO->getQuality().getsigma0Squared()));
	result.push_back(myIO->getQuality().getV().xmlGetData());
	result.push_back(myIO->getQuality().getSigmaXa().xmlGetData());
	result.push_back(myIO->getQuality().getSigmaLa().xmlGetData());
#endif //INTEGRATED_EFOTO
	return result;
}

bool IOManager::exec()
{
#ifdef INTEGRATED_EFOTO
	if (manager != NULL && mySensor != NULL && myImage != NULL && myIO != NULL)
		if (myImage->getSensorId() == mySensor->getId() && myIO->getImageId() == myImage->getId())
		{
			if (manager->getInterfaceType().compare("Qt") == 0)
			{
				//myInterface = new IOUserInterface_Qt(this);
				myInterface = IOUserInterface_Qt::instance(this);
			}
			myImage->setSensor(mySensor);
			myIO->setImage(myImage);
			started = true;
			if (myInterface != NULL)
			{
				myInterface->exec();
			}
			//if (manager->getInterfaceType().compare("Qt") == 0)
			//{
			//IOUserInterface_Qt::createInstance(this)->exec();
			//}
		}
#endif //INTEGRATED_EFOTO
	return status;
}

int IOManager::getId()
{
#ifdef INTEGRATED_EFOTO
	if (myImage != NULL)
	{
		return myImage->getId();
	}
#endif //INTEGRATED_EFOTO
	return 0;
}

void IOManager::returnProject()
{
	manager->reloadProject();
}

bool IOManager::save(string path)
{
	if (started)
	{
#ifdef INTEGRATED_EFOTO
		FILE* pFile;
		string output = "IO state data for Image " + Conversion::intToString(myImage->getId()) + "\n\n";

		output += mySensor->xmlGetData();
		output += "\n";
		output += myImage->xmlGetData();
		output += "\n";
		output += myIO->xmlGetData();
		output += "\n";

		EDomElement e(output);
		output = e.indent('\t').getContent();

		const char* buffer = output.c_str();
		pFile = fopen (path.c_str(), "wb");
		fwrite (buffer, 1, output.length(), pFile);
		fclose (pFile);
		return true;
#endif //INTEGRATED_EFOTO
	}
	return false;
}

bool IOManager::load(string path)
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
		if ((unsigned int) result != (unsigned int) lSize) {fputs ("Reading error",stderr); exit (3);}


		string strxml(buffer);
		EDomElement xml(strxml);

#ifdef INTEGRATED_EFOTO
		mySensor->xmlSetData(xml.elementByTagName("Sensor").getContent());
		myImage->xmlSetData(xml.elementByTagName("image").getContent());
		myIO->xmlSetData(xml.elementByTagName("imageIO").getContent());
#endif //INTEGRATED_EFOTO

		fclose (pFile);
		free (buffer);
	}
	return false;
}

string IOManager::getImageFile()
{
#ifdef INTEGRATED_EFOTO
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
#endif //INTEGRATED_EFOTO
}

int IOManager::getFrameRows()
{
	int rows = 0;
#ifdef INTEGRATED_EFOTO
	if (mySensor->is("SensorWithKnowDimensions"))
	{
		SensorWithKnowDimensions* sensor = (SensorWithKnowDimensions*)mySensor;
		rows = sensor->getFrameRows();
	}
#endif //INTEGRATED_EFOTO
	return rows;
}

int IOManager::getFrameColumns()
{
	int cols = 0;
#ifdef INTEGRATED_EFOTO
	if (mySensor->is("SensorWithKnowDimensions"))
	{
		SensorWithKnowDimensions* sensor = (SensorWithKnowDimensions*)mySensor;
		cols = sensor->getFrameColumns();
	}
#endif //INTEGRATED_EFOTO
	return cols;
}

void IOManager::acceptIO()
{
    // Rever, pois o accept agora é quase o automático. E deve ser previsto um rollback.
    /*
	EDomElement newXml(manager->xmlGetData());
	if (newXml.elementByTagAtt("imageIO", "image_key", Conversion::intToString(myImage->getId())).getContent() != "")
		newXml.replaceChildByTagAtt("imageIO", "image_key", Conversion::intToString(myImage->getId()), myIO->xmlGetData());
	else
		newXml.addChildAtTagName("interiorOrientation", myIO->xmlGetData());
	manager->xmlSetData(newXml.getContent());
	manager->setSavedState(false);
    */
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
