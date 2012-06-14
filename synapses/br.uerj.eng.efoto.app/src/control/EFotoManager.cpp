/*******************************************************************************
		  EFotoManager.cpp
*******************************************************************************/

#include "EFotoManager.h"
#include "IOManager.h"
#include "SRManager.h"
#include "PTManager.h"
#include "ProjectManager.h"
#include "DEMManager.h"
#include "OrthoManager.h"
#include "SPManager.h"

#include <QApplication>

#ifdef SYNAPSE_EFOTO
#include <IPhotogrammetricEngine.h>
#include <ICortex.h>
using namespace cortex;
#endif //SYNAPSE_EFOTO

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

// Constructors and destructor
//

EFotoManager::EFotoManager()
{
	projectManager = NULL;
	interiorOrientation = NULL;
	spatialRessection = NULL;
	fotoTri = NULL;
	dem = NULL;
	ortho = NULL;
    sp = NULL;
	nextModule = 1;

#ifdef INTEGRATED_EFOTO
    project = new Project();
#endif //INTEGRATED_EFOTO
#ifdef SYNAPSE_EFOTO
    enginePtr = ICortex::getInstance()->getSynapse<engine::IPhotogrammetricEngine>();
    project = enginePtr->getProject();
#endif //SYNAPSE_EFOTO

}

EFotoManager::~EFotoManager()
{

}

// XML methods
//
void EFotoManager::setXml(string xml)
{
    project->setXml(xml);
}

string EFotoManager::getXml()
{
    return project->getXml();
}


//Other methods
//

void EFotoManager::setInterfaceType(string newInterfaceType)
{
	interfaceType = newInterfaceType;
}

string EFotoManager::getInterfaceType()
{
	return interfaceType;
}

void EFotoManager::setNextModule(int newModule)
{
	nextModule = newModule;
}

void EFotoManager::setNextImage(int newImage)
{
	nextImage = newImage;
}

bool EFotoManager::execProject(string filename)
{
	bool result;
	nextModule = 0;
	if (projectManager == NULL)
	{
		projectManager = new ProjectManager(this);
	}
	result = projectManager->exec(filename);
	return result;
}

bool EFotoManager::reloadProject()
{
	nextModule = 0;
	if (projectManager != NULL)
	{
		if (fotoTri !=NULL)
		{
			stopPT();
		}
		if (interiorOrientation != NULL)
		{
			stopIO(interiorOrientation->getId());
		}
		if (spatialRessection != NULL)
		{
			stopSR(spatialRessection->getId());
		}
		if (dem != NULL)
		{
			stopDEM();
		}
		if (ortho != NULL)
		{
			stopOrtho();
		}
				if (sp != NULL)
				{
						stopSP();
				}
		return projectManager->reload();
	}
	else
	{
		return false;
	}
}

bool EFotoManager::execIO(int id)
{
	bool result;
    /* rever!
	nextModule = 2;
	Image* ioImage = instanceImage(id);
	if (ioImage == NULL)
	{
		return false;
	}
	Sensor* ioSensor = instanceSensor(ioImage->getSensorId());
	InteriorOrientation* io = instanceIO(id);
	if (io == NULL)
	{
		io = new InteriorOrientation(id);
		IOs.push_back(io);
	}
	else
	{

	}
	interiorOrientation = new IOManager(this, ioSensor, ioImage, io);
	result = interiorOrientation->exec();
	*/
	return result;
}

void EFotoManager::stopIO(int id)
{
    /* rever!
	Image* ioImage = instanceImage(id);
	delete interiorOrientation;
	interiorOrientation = NULL;
	deleteIO(id);
	deleteSensor(ioImage->getSensorId());
	deleteImage(id);
	*/
}

bool EFotoManager::execSR(int id)
{
	bool result;
    /* rever!
	nextModule = 2;
	Image* srImage = instanceImage(id);
	if (srImage == NULL)
	{
		return false;
	}
	Sensor* srSensor = instanceSensor(srImage->getSensorId());
	Flight* srFlight = instanceFlight(srImage->getFlightId());
	InteriorOrientation* srIO = instanceIO(id);
	SpatialRessection* sr = (SpatialRessection*) instanceEO(id);
	Terrain* srTerrain = instanceTerrain();
	srFlight->setTerrain(srTerrain);
	if (sr == NULL)
	{
		sr = new SpatialRessection(id);
		EOs.push_back(sr);
	}
	else
	{

	}
	//instanceAllPoints();
	spatialRessection = new SRManager(this, srTerrain, srSensor, srFlight, srImage, srIO, sr, getProject()->allPoints());

	result = spatialRessection->exec();
 */
	return result;
}

void EFotoManager::stopSR(int id)
{
    /* rever!
	Image* srImage = instanceImage(id);
	delete spatialRessection;
	spatialRessection = NULL;
	deleteEO(id);
	deleteIO(id);
	deleteFlight(srImage->getFlightId());
	deleteSensor(srImage->getSensorId());
	deleteTerrain();
	deleteImage(id);
	*/
}

bool EFotoManager::execSP()
{
	bool result;
    /* rever!
	nextModule = 2;

	instanceAllImages();
	instanceAllPoints();
	instanceAllIOs();
	instanceAllEOs();

	for (int i = images.size() - 1; i >=0; i--)
	{
			Image* img = images.at(i);
			Sensor* sensor = instanceSensor(img->getSensorId());
			InteriorOrientation* imgIO = instanceIO(img->getId());
			SpatialRessection* imgEO = (SpatialRessection*)instanceEO(img->getId());

			img->setSensor(sensor);
			img->setIO(imgIO);
			img->setEO(imgEO);

			if (imgIO == NULL || imgEO == NULL)
			{
					deleteImage(img->getId());
			}
	}

	sp = new SPManager(this, images, EOs);

	result = sp->exec();
*/
	return result;
}

void EFotoManager::stopSP()
{
    /* rever!
	delete sp;
	sp = NULL;
	//deleteSensor(images.at(0)->getSensorId());
	int numPoints=points.size();
	int numImages=images.size();

	for (int i=0;i<numPoints;i++)
	{
			deletePoint(points.at(0)->getId());
	}
	for (int i=0;i<numImages;i++)
	{
			deleteIO(images.at(0)->getId());
			deleteImage(images.at(0)->getId());
	}*/
}

bool EFotoManager::execPT()
{
	bool result;
    /* rever!
	nextModule = 2;

	instanceAllImages();
	instanceAllPoints();
	instanceAllIOs();

	EDomElement sensor(getXml("sensor"));
	Sensor *ptSensor = instanceSensor(Conversion::stringToInt(sensor.attribute("key")));

	Flight *ptFlight = instanceFlight(1);
	Terrain* ptTerrain = instanceTerrain();
	//ptFlight->setTerrain(ptTerrain);
	//fotoTri = new PTManager(this,ptImages,ptOis,ptSensor);//,ptFlight);
	fotoTri = new PTManager(this,images,IOs,ptSensor,ptFlight,ptTerrain);
	result = fotoTri->exec();
*/
	return result;
}

void EFotoManager::stopPT()
{
/* rever!
	delete fotoTri;
	fotoTri = NULL;
	deleteSensor(images.at(0)->getSensorId());
	int numPoints=points.size();
	int numImages=images.size();

	for (int i=0;i<numPoints;i++)
	{
		deletePoint(points.at(0)->getId());
	}
	for (int i=0;i<numImages;i++)
	{
		deleteIO(images.at(0)->getId());
		deleteImage(images.at(0)->getId());
	}
*/
}

bool EFotoManager::execDEM()
{
	bool result;
/* rever!
	nextModule = 2;

	instanceAllImages();
	instanceAllPoints();
	instanceAllIOs();
	instanceAllEOs();

	for (int i = images.size() - 1; i >=0; i--)
	{
		Image* img = images.at(i);
		Sensor* sensor = instanceSensor(img->getSensorId());
		InteriorOrientation* imgIO = instanceIO(img->getId());
		SpatialRessection* imgEO = (SpatialRessection*)instanceEO(img->getId());

		img->setSensor(sensor);
		img->setIO(imgIO);
		img->setEO(imgEO);

		if (imgIO == NULL || imgEO == NULL)
		{
			deleteImage(img->getId());
		}
	}

	dem = new DEMManager(this, images, EOs);

	result = dem->exec();
*/
	return result;
}

void EFotoManager::stopDEM()
{
    /* rever!
	delete dem;
	dem = NULL;
	//deleteSensor(images.at(0)->getSensorId());
	int numPoints=points.size();
	int numImages=images.size();

	for (int i=0;i<numPoints;i++)
	{
		deletePoint(points.at(0)->getId());
	}
	for (int i=0;i<numImages;i++)
	{
		deleteIO(images.at(0)->getId());
		deleteImage(images.at(0)->getId());
	}
	*/
}

bool EFotoManager::execOrtho()
{
	bool result;
/* rever!
	nextModule = 2;

	instanceAllImages();
	instanceAllIOs();
	instanceAllEOs();

	for (int i = images.size() - 1; i >=0; i--)
	{
		Image* img = images.at(i);
		Sensor* sensor = instanceSensor(img->getSensorId());
		InteriorOrientation* imgIO = instanceIO(img->getId());
		SpatialRessection* imgEO = (SpatialRessection*)instanceEO(img->getId());

		img->setSensor(sensor);
		img->setIO(imgIO);
		img->setEO(imgEO);

		if (imgIO == NULL || imgEO == NULL)
		{
			deleteImage(img->getId());
		}
	}

	ortho = new OrthoManager(this, images, EOs);

	result = ortho->exec();
*/
	return result;
}

void EFotoManager::stopOrtho()
{
    /* rever!
	delete ortho;
	ortho = NULL;
	//deleteSensor(images.at(0)->getSensorId());
	int numPoints=points.size();
	int numImages=images.size();

	for (int i=0;i<numPoints;i++)
	{
		deletePoint(points.at(0)->getId());
	}
	for (int i=0;i<numImages;i++)
	{
		deleteIO(images.at(0)->getId());
		deleteImage(images.at(0)->getId());
	}*/
}

bool EFotoManager::exec(string filename)
{
	if (filename != "")
	{
		execProject(filename);
    }
	switch (nextModule)
	{
	case 1:
		execProject();
		break;
	case 2:
		reloadProject();
		break;
    case 3:
		execIO(nextImage);
		break;
    case 4:
		execSR(nextImage);
		break;
    case 5:
		execPT();
		break;
    case 6:
		execDEM();
		break;
    case 7:
		execOrtho();
		break;
    case 8:
		execSP();
		break;
	default:
		nextModule = 0;
    }

	return true;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
