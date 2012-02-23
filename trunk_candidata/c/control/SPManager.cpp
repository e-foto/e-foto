/**************************************************************************
														   SPManager.cpp
**************************************************************************/

#include "SPManager.h"
#include "EFotoManager.h"
#include "SPUserInterface_Qt.h"
#include "ProjectiveRay.h"


namespace br {
namespace uerj {
namespace eng {
namespace efoto {

// Constructors and destructors
//

SPManager::SPManager()
{
	started = false;
	status = false;
}

SPManager::SPManager(EFotoManager* manager, deque<Image*>images, deque<ExteriorOrientation*> eos)
{
	this->manager = manager;
	started = false;
	status = false;
	listAllImages = images;
	listEOs = eos;
		spFeatures.createClassesFromSp165();
}

SPManager::~SPManager()
{
	//if (grid != NULL)
	//delete grid;
}

// Association Methods
//

void SPManager::setInterface(SPUserInterface* newInterface)
{
	myInterface = newInterface;
}

SPUserInterface* SPManager::getInterface()
{
	return myInterface;
}

// Other Methods
//

bool SPManager::exec()
{
	if (manager != NULL)
	{
		if (manager->getInterfaceType().compare("Qt") == 0)
		{
			myInterface = SPUserInterface_Qt::instance(this);
		}
		//connectImagePoints();
		started = true;
		if (myInterface != NULL)
		{
			myInterface->exec();
			//getPairs();
			//createInitialSeeds();
		}
	}
	return status;
}

void SPManager::returnProject()
{
	manager->reloadProject();
}

int SPManager::loadFeatures(char *filename)
{
	return spFeatures.loadFeatures(filename,0,false);
}

void SPManager::saveFeatures(char *filename)
{
	spFeatures.saveFeatures(filename,0,false);
}

void SPManager::addFeature(string name, int feattype, int featclass)
{
	int f = 2;
	if (feattype == 2) f = 3;
	if (feattype == 3) f = 9;
	int fclass = featclass + f - 1;

	spFeatures.addNewFeature(name, "", fclass, feattype, 1);
}

int SPManager::removeFeature()
{
	int sel_feat = spFeatures.selectedFeature();

	if (sel_feat == -1)
		return 0;

	spFeatures.deleteFeature(sel_feat);

	return 1;
}

void SPManager::removeAllFeatures()
{
	spFeatures.deleteAllFeatures();
}

void SPManager::updateProjections()
{
	int leftKey = 1;
	int rightKey = 2;
	Image* leftImage = manager->instanceImage(leftKey);
	Image* rightImage = manager->instanceImage(rightKey);
	Sensor* sensor = manager->instanceSensor(1);
	InteriorOrientation* lio = manager->instanceIO(leftKey);
	InteriorOrientation* rio = manager->instanceIO(rightKey);
	SpatialRessection* lsr = (SpatialRessection*)manager->instanceEO(leftKey);
	SpatialRessection* rsr = (SpatialRessection*)manager->instanceEO(rightKey);
	leftImage->setSensor(sensor); leftImage->setIO(lio); leftImage->setEO(lsr);
	rightImage->setSensor(sensor); rightImage->setIO(rio); rightImage->setEO(rsr);
	if (leftImage == NULL || rightImage == NULL)
		return;
	ProjectiveRay prL(leftImage);
	ProjectiveRay prR(rightImage);

	// Aqui enquanto não definimos uma estrutura para as geometrias 2d eu vou coordenar a tradução para o que eu já possuia no display para pontos e retas.
	for (int i = 0; i < spFeatures.getNumFeatures(); i++)
	{
		DemFeature* df = spFeatures.getFeatureLink(i+1);
		for (int j = 0; j < df->points.size(); j++)
		{
			// Isso vai ficar um pouco ruim, mas vai melhorar em breve quando a classe DigitalImageCoordinates for substituida por algo equivalente usando double (completando o suporte a subpixels)
			ImageSpaceCoordinate pL0 = prL.objectToImage(df->points.at(j).X, df->points.at(j).Y, df->points.at(j).Z,false);
			ImageSpaceCoordinate pR0 = prR.objectToImage(df->points.at(j).X, df->points.at(j).Y, df->points.at(j).Z,false);
			df->points.at(j).left_x = pL0.getCol();
			df->points.at(j).left_y = pL0.getLin();
			df->points.at(j).right_x = pR0.getCol();
			df->points.at(j).right_y = pR0.getLin();
		}
	}
}

string SPManager::getFullImagePath(int imagekey)
{
	Image* img = manager->instanceImage(imagekey);
	string result;
	result.append(img->getFilepath());
	result.append("/");
	result.append(img->getFilename());
	manager->deleteImage(imagekey);
	return result;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
