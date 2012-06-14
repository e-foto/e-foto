/**************************************************************************
		  IOManager.cpp
**************************************************************************/

#include "IOManager.h"
#include "EFotoManager.h"
#include "IOUserInterface.h"
#include "IOUserInterface_Qt.h"

#include "Image.h"
#include "InteriorOrientation.h"
#include "SensorWithFiducialMarks.h"
#include "SensorWithKnowDimensions.h"
#include "SensorWithKnowParameters.h"

// Constructors and destructors
//

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

IOManager::IOManager()
{
    manager = NULL;
#ifdef INTEGRATED_EFOTO
    project = NULL;
#endif //INTEGRATED_EFOTO
	started = false;
	status = false;
    imageId = 0;
}

IOManager::IOManager(EFotoManager* manager, int id)
{
    this->manager = manager;
    project = manager->getProject();
	started = false;
	status = false;
    imageId = id;
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
    if (project != NULL && project->sensor(1) != NULL)
#endif //INTEGRATED_EFOTO
#ifdef SYNAPSE_EFOTO
    if (!project.isNull() && project->image(imageId) != NULL && project->image(imageId)->getSensor() != NULL)
#endif //SYNAPSE_EFOTO
	{
        Sensor* mySensor = project->image(imageId)->getSensor();
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
	return PositionMatrix();
}

bool IOManager::measureMark(int id, double col, double lin)
{
	if (started)
    {
        ImageFiducialMark* myMark = new ImageFiducialMark(id, imageId, col, lin);
        project->image(imageId)->deleteDigFidMark(id);
        project->image(imageId)->putDigFidMark(*myMark);
		return true;
	}
	return false;
}

unsigned int IOManager::countMarks()
{
    return project->image(imageId)->countDigFidMarks();
}

deque<string> IOManager::markData(int index)
{
	deque<string> result;
	if (started)
	{
		PositionMatrix analogMarks = getAnalogMarks();

		result.push_back(Conversion::doubleToString(analogMarks.get(2 * index + 1),3));
        result.push_back(Conversion::doubleToString(analogMarks.get(2 * index + 2),3));
        ImageFiducialMark myMark = project->image(imageId)->getDigFidMark(index + 1);
		if (myMark.isAvailable())
		{
			result.push_back(Conversion::doubleToString(myMark.getCol()));
			result.push_back(Conversion::doubleToString(myMark.getLin()));
        }
	}
	return result;
}

unsigned int IOManager::getTotalMarks()
{
	return getAnalogMarks().getRows() / 2;
}

int IOManager::getCalculationMode()
{
    string mode = project->image(imageId)->getSensor()->getCalculationMode();
    return mode == "With Fiducial Marks"? 1 : mode == "With Sensor Dimensions" ? 2 : mode == "Fixed Parameters" ? 3 : 0;
}

bool IOManager::calculateIO()
{
	if (started)
    {
        if (project->image(imageId)->getSensor()->is("SensorWithFiducialMarks"))
		{
            SensorWithFiducialMarks* mySensorWithFiducialMarks = (SensorWithFiducialMarks*) project->image(imageId)->getSensor();
            if ((unsigned int) project->image(imageId)->countDigFidMarks() == (unsigned int) mySensorWithFiducialMarks->getLb().getRows() / 2) // Comparação entre o número de marcas digitais e analógicas. "mySensorWithFiducialMarks->getLb()->getCols() / 2" dá o número de marcas analógicas validadas, tendo Lb sido carregado direto ou através das marcas.
			{
                project->image(imageId)->getIO()->calculate();
				status = true;
			}
		}
        else if (project->image(imageId)->getSensor()->is("SensorWithKnowDimensions"))
		{
			//SensorWithKnowDimensions* mySensorWithKnowDimensions = (SensorWithKnowDimensions*) mySensor;
            project->image(imageId)->getIO()->calculate();
			status = true;
		}
        else if (project->image(imageId)->getSensor()->is("SensorWithKnowParameters"))
		{
			//SensorWithKnowParameters* mySensorWithKnowParameters = (SensorWithKnowParameters*) mySensor;
            project->image(imageId)->getIO()->calculate();
			status = true;
        }
	}
	return status;
}

bool IOManager::interiorDone()
{
    if (project->image(imageId)->getIO()->getXa().getRows() != 6)
        return false;
	return true;
}

deque<string> IOManager::makeReport()
{
    deque<string> result;
    InteriorOrientation* myIO = project->image(imageId)->getIO();
	result.push_back(myIO->getXa().xmlGetData());
	result.push_back(myIO->getLa().xmlGetData());
	result.push_back(Conversion::doubleToString(myIO->getQuality().getsigma0Squared()));
	result.push_back(myIO->getQuality().getV().xmlGetData());
	result.push_back(myIO->getQuality().getSigmaXa().xmlGetData());
    result.push_back(myIO->getQuality().getSigmaLa().xmlGetData());
	return result;
}

bool IOManager::exec()
{
#ifdef INTEGRATED_EFOTO
    if (manager != NULL && project != NULL)
#endif //INTEGRATED_EFOTO
#ifdef SYNAPSE_EFOTO
    if (manager != NULL && !project.isNull())
#endif //SYNAPSE_EFOTO
        if (project->image(imageId) != NULL && project->image(imageId)->getSensor() != NULL)
		{
            if (project->image(imageId)->getIO() == NULL)
            {
                InteriorOrientation io(imageId);
                project->addIO(io.xmlGetData());
            }
			if (manager->getInterfaceType().compare("Qt") == 0)
            {
				myInterface = IOUserInterface_Qt::instance(this);
            }
			started = true;
			if (myInterface != NULL)
			{
				myInterface->exec();
            }
        }
	return status;
}

int IOManager::getId() //Deprecated
{
    return imageId;
}

void IOManager::returnProject()
{
	manager->reloadProject();
}

bool IOManager::save(string path)
{
	if (started)
    {
		FILE* pFile;
        string output = "IO state data for Image " + Conversion::intToString(project->image(imageId)->getId()) + "\n\n";

        output += project->image(imageId)->getSensor()->xmlGetData();
		output += "\n";
        output += project->image(imageId)->xmlGetData();
		output += "\n";
        output += project->image(imageId)->getIO()->xmlGetData();
		output += "\n";

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

bool IOManager::load(string path) // Deprecated
{
	return false;
}

string IOManager::getImageFile()
{
    if (project->image(imageId)->getFilepath() == ".")
        return project->image(imageId)->getFilename();
	else
	{
		string result = "";
        result += project->image(imageId)->getFilepath();
		result += "/";
        result += project->image(imageId)->getFilename();
		return result;
    }
}

int IOManager::getFrameRows()
{
    int rows = 0;
    if (project->image(imageId)->getSensor()->is("SensorWithKnowDimensions"))
    {
        SensorWithKnowDimensions* sensor = (SensorWithKnowDimensions*)project->image(imageId)->getSensor();
		rows = sensor->getFrameRows();
    }
	return rows;
}

int IOManager::getFrameColumns()
{
    int cols = 0;
    if (project->image(imageId)->getSensor()->is("SensorWithKnowDimensions"))
	{
        SensorWithKnowDimensions* sensor = (SensorWithKnowDimensions*) project->image(imageId)->getSensor();
		cols = sensor->getFrameColumns();
    }
	return cols;
}

void IOManager::acceptIO()
{
    // Rever!, pois o accept agora é quase o automático. E deve ser previsto um rollback.
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
