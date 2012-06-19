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
            stopIO();
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
    nextModule = 2;
    interiorOrientation = new IOManager(this, id);
    return interiorOrientation->exec();
}

void EFotoManager::stopIO()
{
    if (interiorOrientation)
        delete interiorOrientation;
    interiorOrientation = NULL;
}

bool EFotoManager::execSR(int id)
{
    nextModule = 2;
    spatialRessection = new SRManager(this, id);
    return spatialRessection->exec();
}

void EFotoManager::stopSR(int id)
{
    if (spatialRessection)
        delete spatialRessection;
    spatialRessection = NULL;
}

bool EFotoManager::execSP()
{
	nextModule = 2;
    sp = new SPManager(this);
    return sp->exec();
}

void EFotoManager::stopSP()
{
    if (sp)
        delete sp;
    sp = NULL;
}

bool EFotoManager::execPT()
{
    nextModule = 2;
    fotoTri = new PTManager(this);
    return fotoTri->exec();
}

void EFotoManager::stopPT()
{
    if (fotoTri)
        delete fotoTri;
    fotoTri = NULL;
}

bool EFotoManager::execDEM()
{
	nextModule = 2;
    dem = new DEMManager(this);
    return dem->exec();
}

void EFotoManager::stopDEM()
{
    if (dem)
        delete dem;
    dem = NULL;
}

bool EFotoManager::execOrtho()
{
	nextModule = 2;
    ortho = new OrthoManager(this);
    return ortho->exec();
}

void EFotoManager::stopOrtho()
{
    if (ortho)
        delete ortho;
    ortho = NULL;
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
