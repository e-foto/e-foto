#include "PTManager.h"
#include "EFotoManager.h"
#include "PTUserInterface_Qt.h"

#include "EDom.h"
#include "Point.h"
#include "Image.h"
#include "InteriorOrientation.h"
#include "SensorWithFiducialMarks.h"
#include "Dms.h"

//#include <qdebug.h>


PTManager::PTManager()
{
	efotoManager  = NULL;
	mySensor = NULL;
	myFlight = NULL;

	//	Matrix d(2,2);
	//d.set(1,1,3.0);

	//for (int j=1;j<=d.getCols();j++)
	//qDebug("%f",d.get(1,j));

	//afp=d;
	started = false;
	status = false;
}

PTManager::PTManager(EFotoManager *newManager, deque<Image*>images, deque<InteriorOrientation*> ois,Sensor *sensor)//, Flight *flight)
{
	efotoManager = newManager;
	listAllImages = images;
	listOis = ois;
	setListPoint();//lista todos os pontos
	mySensor = sensor;
	//	myFlight = flight;

	started = false;
	status = false;

	setENH();
//	setColLin();
	/*setBLC();*/
}

PTManager::~PTManager()
{

}

void PTManager::setInterface(PTUserInterface *newInterface)
{
	myInterface = newInterface;
}

PTUserInterface* PTManager::getInterface()
{
	return myInterface;
}

bool PTManager::exec()
{
	if (efotoManager != NULL && mySensor != NULL && /*myFlight != NULL &&*/ listAllImages.size()> 1 && listOis.size()>1)
	{
		if (efotoManager->getInterfaceType().compare("Qt") == 0)
		{
			myInterface = PTUserInterface_Qt::instance(this);
		}
		//myFlight->setSensor(mySensor);
		for (int i=0; i<listAllImages.size(); i++)
		{
			mySensor->putImage(listAllImages.at(i));
			listAllImages.at(i)->setSensor(mySensor);
			//listAllImages.at(i)->setFlight(myFlight);
			listAllImages.at(i)->setIO(listOis.at(i));
			listOis.at(i)->setImage(listAllImages.at(i));
		}
		connectImagePoints();
		started=true;
		if (myInterface != NULL)
		{
			myInterface->exec();
		}
	}
	return status=true;
}

void PTManager::returnProject()
{
	//if (pt!=NULL)
		// delete pt;
	//delete myInterface;
	//delete mySensor;
	//delete myFlight;
	listAllImages.clear();
	listAllPoints.clear();
	listOis.clear();
	listSelectedImages.clear();
	listSelectedPoints.clear();

	efotoManager->reloadProject();
}

string PTManager::getImagefile(int imageId)
{
	string imagefile=listAllImages.at(imageId)->getFilepath();
    imagefile +="/";
	imagefile +=listAllImages.at(imageId)->getFilename();

    return imagefile;
}

bool PTManager::calculatePT()
{
	sortPointsSelected();
	pt= new BundleAdjustment(listSelectedImages,listSelectedPoints,1);
	bool result=pt->calculate();
	setMatrixAFP(pt->getAFP());
    return result;
}

PositionMatrix PTManager::getImageDimensions(string filename)
{
	for (int i=0;i<listAllImages.size();i++)
	{
		//qDebug("Imagens %s",listAllImages.at(i)->getFilename().c_str());
		//qDebug("Antes %s",imageFileName.c_str());
		if(listAllImages.at(i)->getFilename() == filename)
		{
			//qDebug("Achou %s",imageFileName.c_str());
			Image *temp=listAllImages.at(i);
			PositionMatrix dimensions(2,"");
			qDebug("height:= %d",temp->getHeight());
			dimensions.setInt(1,temp->getHeight());
			dimensions.setInt(2,temp->getWidth());
			return dimensions;
		}
	}
}

void PTManager::setMatrixAFP(Matrix afp)
{
    AFP=afp;
}

Matrix PTManager::getMatrixOE()
{
	return AFP;
}




/**novo*/

void PTManager::setENH()
{
    //string pointxml=efotoManager->getXml("point");
    EDomElement pointsXml(efotoManager->getXml("points"));
    int children=pointsXml.children().size();

    Matrix points(children,3);
    for (int i=1;i<=children;i++)
    {
        EDomElement point=pointsXml.elementByTagAtt("point","key",intToString(i));
        string enh=point.elementByTagName("gml:pos").toString().c_str();
        int ini=enh.find_first_of(" ");
        int fim=enh.find_last_of(" ");
		double E=stringToDouble(enh.substr(0,ini).c_str());
		double N=stringToDouble(enh.substr(ini+1,fim).c_str());
		double H=stringToDouble(enh.substr(fim+1,enh.size()).c_str());
		//qDebug("%d\tE=%.4f\tN=%.4f\tH=%.4f",i,E,N,H);
        points.set(i,1,E);
        points.set(i,2,N);
        points.set(i,3,H);
    }
	//	points.show('f',4);
    ENH=points;
}

Matrix PTManager::getResiduos()
{
    return pt->getMatRes();
}

Matrix PTManager::getENH()
{
    return ENH;
}

bool PTManager::connectImagePoints()
{
	if (!(started))
	{
		EDomElement xmlPoints(efotoManager->getXml("points"));
		deque<EDomElement> allPoints = xmlPoints.children();
		for (unsigned int j = 0; j < listAllImages.size(); j++)
		{
			for (unsigned int i = 0; i < allPoints.size(); i++)
			{
				string data = allPoints.at(i).elementByTagAtt("imageCoordinates", "image_key", intToString(listAllImages.at(j)->getId())).getContent();
				if (data != "")
				{
					Point* pointToInsert = efotoManager->instancePoint(stringToInt(allPoints.at(i).attribute("key")));
					if (pointToInsert != NULL)
					{
						//qDebug("connectImagePoints(): colocou um ponto");
						listAllImages.at(j)->putPoint(pointToInsert);
						pointToInsert->putImage(listAllImages.at(j));//nove em teste:06/08/2011
					}
				}
			}
			listAllImages.at(j)->sortPoints();
		}
		return true;
	}
	return false;
}

void PTManager::setListPoint()
{
	EDomElement xmlPoints(efotoManager->getXml("points"));
	deque<EDomElement> allPoints = xmlPoints.children();
	for(int i=0;i<allPoints.size();i++)
	{
		Point* point= efotoManager->instancePoint(stringToInt(allPoints.at(i).attribute("key")));
		listAllPoints.push_back(point);
	}
}
// retorna uma lista com os caminhos das imagens
deque<string> PTManager::getStringImages()
{
	deque<string> list;
	int numImages=listAllImages.size();
	for (int i=0;i<numImages;i++)
		list.push_back(listAllImages.at(i)->getFilename());
	return list;
}

deque<string> PTManager::getStringTypePoints(string imageFileName)
{
	deque<string> list;
	if (imageFileName =="")
	{
		int numPnts=listAllPoints.size();
		for (int i=0; i<numPnts; i++)
		{
			if (listAllPoints.at(i)->is("ControlPoint"))
				list.push_back("Control");
			if (listAllPoints.at(i)->is("PhotogrammetricPoint"))
				list.push_back("Photogrammetric");
			if (listAllPoints.at(i)->is("CheckingPoint"))
				list.push_back("Checking");
		}
		return list;
	}else
	{
		int numImages= listAllImages.size();
		for (int i=0;i<numImages;i++)
		{
			//qDebug("Imagens %s",listAllImages.at(i)->getFilename().c_str());
			//qDebug("Antes %s",imageFileName.c_str());
			if(listAllImages.at(i)->getFilename() == imageFileName)
			{
				//qDebug("Achou %s",imageFileName.c_str());
				Image *temp=listAllImages.at(i);
				int numpnts=temp->countPoints();
				for (int j=0;j<numpnts;j++)
				{
					//qDebug("%s from %s",temp->getPointAt(j)->getPointId().c_str() , imageFileName.c_str());
					if (temp->getPointAt(j)->is("ControlPoint"))
						list.push_back("Control");
					if (temp->getPointAt(j)->is("PhotogrammetricPoint"))
						list.push_back("Photogrammetric");
					if (temp->getPointAt(j)->is("CheckingPoint"))
						list.push_back("Checking");
				}
				return list;
			}
		}
	}
}

// preenche a lista de imagens selecionadas(listSelectedImages) atraves de um deque de strings com os caminhos das imagens
void PTManager::selectImages(deque<string> selectedImagesList)
{
	if(listSelectedImages.size()!=0)
		listSelectedImages.clear();

	int numImgs= listAllImages.size();
	int	numSelectedImgs=selectedImagesList.size();
	for(int i=0;i<numSelectedImgs;i++)
		for (int j=0;j<numImgs;j++)
			if (listAllImages.at(j)->getFilename()==selectedImagesList.at(i))
			{
				listSelectedImages.push_back(listAllImages.at(j));
				//qDebug("IdImage: %d",listAllImages.at(j)->getId());
			}

	//listSelectedImages=listImages;
}
// preenche a lista de pontos selecionados(listSelectedPoints) atraves de um deque de strings com Ids dos pontos
void PTManager::selectPoints(deque<string> selectedPointsList)
{
	if(listSelectedPoints.size()!=0)
		listSelectedPoints.clear();

	int numPnts= listAllPoints.size();
	int	numSelectedPnts=selectedPointsList.size();
	for(int i=0;i<numSelectedPnts;i++)
		for (int j=0;j<numPnts;j++)
			if (listAllPoints.at(j)->getPointId()==selectedPointsList.at(i))
			{
				listSelectedPoints.push_back(listAllPoints.at(j));
				//qDebug("Achei IdPoint: %s",listAllPoints.at(j)->getPointId().c_str());
			}
			else
			{
				//qDebug("Ponto nao encontrado");
			}
}

BundleAdjustment* PTManager::getBundleAdjustment()
{
	return pt;
}

// retorna uma lista com os ids dos pontos
deque<string> PTManager::getStringIdPoints(string imageFileName)
{
	deque<string> list;
	if (imageFileName =="")
	{
		int numPnts=listAllPoints.size();
		for (int i=0; i<numPnts; i++)
		{
			Point *pnt=listAllPoints.at(i);
			list.push_back(pnt->getPointId());
		}
		return list;
	}else
	{
		int numImages= listAllImages.size();
		//qDebug("lista de imagens %d",listAllImages.size());
		for (int i=0;i<numImages;i++)
		{
			//qDebug("Imagens %s",listAllImages.at(i)->getFilename().c_str());
			//qDebug("Antes %s",imageFileName.c_str());
			if(listAllImages.at(i)->getFilename() == imageFileName)
			{
				//qDebug("Achou %s",imageFileName.c_str());
				Image *temp=listAllImages.at(i);
				int numpnts=temp->countPoints();
				for (int j=0;j<numpnts;j++)
				{
					//qDebug("%s from %s",temp->getPointAt(j)->getPointId().c_str() , imageFileName.c_str());
					list.push_back(temp->getPointAt(j)->getPointId());
				}
				return list;
			}
		}
	}
	return list;
}

deque<string> PTManager::getStringKeysPoints(string imageFileName)
{
	deque<string> list;

	//qDebug("lista de imagens %d",listAllImages.size());
	if (imageFileName =="")
	{
		int numPnts=listAllPoints.size();
		for (int i=0; i<numPnts; i++)
		{
			Point *pnt=listAllPoints.at(i);
			list.push_back(intToString(pnt->getId()));
		}
		return list;
	}else
	{
		int numImages= listAllImages.size();
		for (int i=0;i<numImages;i++)
		{
			//qDebug("Imagens %s",listAllImages.at(i)->getFilename().c_str());
			//qDebug("Antes %s",imageFileName.c_str());
			if(listAllImages.at(i)->getFilename() == imageFileName)
			{
				//qDebug("Achou %s",imageFileName.c_str());
				Image *temp=listAllImages.at(i);
				int numpnts=temp->countPoints();
				for (int j=0;j<numpnts;j++)
				{
					//qDebug("%s from %s",temp->getPointAt(j)->getPointId().c_str() , imageFileName.c_str());
					list.push_back(intToString(temp->getPointAt(j)->getId()));
				}
				return list;
			}
		}
	}

}

Matrix PTManager::getColLin(string imageFilename)
{
	int numImages= listAllImages.size();
	//qDebug("lista de imagens %d",listAllImages.size());
	for (int i=0;i<numImages;i++)
	{
		//qDebug("Imagens %s",listAllImages.at(i)->getFilename().c_str());
		//qDebug("Antes %s",imageFileName.c_str());
		if(listAllImages.at(i)->getFilename() == imageFilename)
		{
			//qDebug("Achou %s",imageFileName.c_str());
			Image *temp=listAllImages.at(i);
			int numpnts=temp->countPoints();
			Matrix result(numpnts,2);
			for (int j=0;j<numpnts;j++)
			{
				DigitalImageSpaceCoordinate coord=temp->getPointAt(j)->getDigitalCoordinate(temp->getId());
				//qDebug("%s from %s",temp->getPointAt(j)->getPointId().c_str() , imageFileName.c_str());
				result.setInt(j+1,1,coord.getCol());
				result.setInt(j+1,2,coord.getLin());
			}
			return result;
		}
	}
}

//Faz um update das coordenadas digitais do ponto 'pointKey' na imagem 'imageKey'
void PTManager::updateDigitalCoordinatesPoint(int imageKey, int pointKey, int col, int lin)
{
	Point *pnt=efotoManager->instancePoint(pointKey);
	pnt->getDigitalCoordinate(imageKey).setCol(col);
	pnt->getDigitalCoordinate(imageKey).setLin(lin);
}

// Procura a key da imagem pelo nome do arquivo senao encontrar retorna -1
int PTManager::getImageId(string imageFilename)
{
	int numImages= listAllImages.size();
	for (int i=0;i<numImages;i++)
		if(listAllImages.at(i)->getFilename() == imageFilename)
			return listAllImages.at(i)->getId();
	return -1;
}

void PTManager::sortPointsSelected()
{
	deque<Point *>listCtrl;
	for(int i=0; i<listSelectedPoints.size() ;i++)
	{
		Point *pnt=listSelectedPoints.at(i);
		if (pnt->is("ControlPoint"))
			listCtrl.push_front(pnt);
		else if (pnt->is("PhotogrammetricPoint"))
			listCtrl.push_back(pnt);
	}
	listSelectedPoints=listCtrl;
}

// retorna o ENH dos pontos Fotogrammetricos
Matrix PTManager::getPhotogrammetricENH()
{
	int points=listSelectedPoints.size();
	Matrix enh(0,3);
	for (int i=0;i<points;i++)
	{
		Point *pnt=listSelectedPoints.at(i);
		if (pnt->is("PhotogrammetricPoint"))
		{
			ObjectSpaceCoordinate coor=pnt->getObjectCoordinate();
			Matrix temp(1,3);
			temp.set(1,1,coor.getX());
			temp.set(1,2,coor.getY());
			temp.set(1,3,coor.getZ());
			enh=enh|temp;
		}
	}
	return enh;
}

deque<string> PTManager::getSelectedPointIdPhotogrammetric()
{
	int points=listSelectedPoints.size();
	deque<string> selected;
	for (int i=0;i<points;i++)
	{
		Point *pnt=listSelectedPoints.at(i);
		if (pnt->is("PhotogrammetricPoint"))
			selected.push_back(pnt->getPointId());
	}
	return selected;
}
