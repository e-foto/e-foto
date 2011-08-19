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

PTManager::PTManager(EFotoManager *newManager, deque<Image*>images, deque<InteriorOrientation*> ois,Sensor *sensor, Flight *flight)
{
	efotoManager = newManager;
	listAllImages = images;
	listOis = ois;
	setListPoint();//lista todos os pontos
	mySensor = sensor;
	myFlight = flight;

	started = false;
	status = false;

	setENH();
	setColLin();
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
	if (efotoManager != NULL && mySensor != NULL && myFlight != NULL && listAllImages.size()> 1 && listOis.size()>1)
	{
		if (efotoManager->getInterfaceType().compare("Qt") == 0)
		{
			myInterface = PTUserInterface_Qt::instance(this);
		}
		myFlight->setSensor(mySensor);
		for (int i=0; i<listAllImages.size(); i++)
		{
			mySensor->putImage(listAllImages.at(i));
			listAllImages.at(i)->setSensor(mySensor);
			listAllImages.at(i)->setFlight(myFlight);
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
    //if (efotoManager->is("EFotoManager"))
	efotoManager->reloadProject();

        //delete(pt);
    //else
	//  qDebug("nao e efotomanager");
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
	pt= new BundleAdjustment(listSelectedImages,listSelectedPoints,1);
	bool result=pt->calculate();
	setMatrixAFP(pt->getAFP());
    return result;
}

void PTManager::setMatrixAFP(Matrix afp)
{
    AFP=afp;
}

Matrix PTManager::getMatrixAFP()
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

void PTManager::setColLin()
{
    EDomElement points(efotoManager->getXml("points"));
    int numPoints=points.children().size();
    EDomElement images(efotoManager->getXml("images"));
    int numimgs=images.children().size();

    //numimgs=3;
    //numPoints=13;

    Matrix col(numimgs,numPoints);
    Matrix lin(numimgs,numPoints);

    for (int i=1;i<=col.getRows();i++)
        for (int j=1;j<=col.getCols();j++)
        {
		col.set(i,j,-1);
		lin.set(i,j,-1);
	}

    for (int i=1;i<=numPoints;i++)
    {
        EDomElement point=points.elementByTagAtt("point","key",intToString(i));
        deque<EDomElement> imgPnts=point.elementsByTagName("imageCoordinates");
		int imgs=imgPnts.size();
		for(int j=0;j<imgs;j++)
        {
            string numImg=imgPnts.at(j).attribute("image_key");
            EDomElement coord=imgPnts.at(j).elementByTagAtt("imageCoordinates","image_key",numImg);
			// qDebug("%s",imgPnts.at(j).getContent().c_str());

            string coordenada=coord.elementByTagName("gml:pos").toString().c_str();
            int ini=coordenada.find_first_of(" ");
			int coluna=stringToInt(coordenada.substr(0,ini).c_str());
			int linha=stringToInt(coordenada.substr(ini+1,coordenada.size()).c_str());

            //qDebug("%d,%d\tCol=%d\tLin=%d\n",i,j,coluna,linha);

            col.setInt(stringToInt(numImg),i,coluna);
            lin.setInt(stringToInt(numImg),i,linha);
        }
    }

    Col=col;
    Lin=lin;
//    col.show();
 //   lin.show();
}

void PTManager::setBLC()
{
	//string pointxml=efotoManager->getXml("point");
	EDomElement pointsXml(efotoManager->getXml("points"));
	int numPnts=pointsXml.children().size();
	EDomElement images(efotoManager->getXml("images"));
	int numImgs=images.children().size();

	Matrix blc(numImgs,numPnts);
	for (int i=1;i<=numPnts;i++)
	{
		EDomElement point=pointsXml.elementByTagAtt("point","key",intToString(i));
		deque<EDomElement> imgPnts=point.elementsByTagName("imageCoordinates");
		int imgs=imgPnts.size();
		int type=0;
		if(point.attribute("type").compare("control")==0)
			type=1;
		else if(point.attribute("type").compare("photogrammetric")>=0)
			type=-1;
		for (int j=0;j<imgs;j++)
		{
			int img=stringToInt(imgPnts.at(j).attribute("image_key"));
			blc.set(img,i,type);
		}
	}
	BLC=blc;
	//BLC.show();
}


Matrix PTManager::getCol()
{
    return Col;
}

Matrix PTManager::getLin()
{
    return Lin;
}

Matrix PTManager::getBLC()
{
	return BLC;
}

Matrix PTManager::getOis()
{
	Matrix ois(BLC.getRows(),6);

	EDomElement oisXml=efotoManager->getXml("interiorOrientation");
	int rows = ois.getRows();
	for (int i=1;i<=rows;i++)
	{
		EDomElement xa=(oisXml.elementByTagAtt("imageIO","image_key",intToString(i))).elementByTagName("Xa");
		ois.set(i,1,xa.elementByTagName("a0").toDouble());
		ois.set(i,2,xa.elementByTagName("a1").toDouble());
		ois.set(i,3,xa.elementByTagName("a2").toDouble());
		ois.set(i,4,xa.elementByTagName("b0").toDouble());
		ois.set(i,5,xa.elementByTagName("b1").toDouble());
		ois.set(i,6,xa.elementByTagName("b2").toDouble());
	}
	//ois.show();
	return ois;
}

Matrix PTManager::getResiduos()
{
    return pt->getMatRes();
}

Matrix PTManager::getENH()
{
    return ENH;
}

PositionMatrix PTManager::getColLin(int imageId, int pointId)
{
    PositionMatrix coord(2,"pixel");
    coord.set(1,Col.get(imageId,pointId));
    coord.set(2,Lin.get(imageId,pointId));
    return coord;
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

deque<string> PTManager::getStringImages()
{
	deque<string> list;
	int numImages=listAllImages.size();
	for (int i=0;i<numImages;i++)
		list.push_back(listAllImages.at(i)->getFilename());
	return list;
}

deque<string> PTManager::getStringPoints()
{
	deque<string> list;
	int numPoints=listAllPoints.size();
	for (int i=0;i<numPoints;i++)
		list.push_back(listAllPoints.at(i)->getPointId());
	return list;
}

void PTManager::selectImages(deque<string> selectedImagesList)
{
	if(listSelectedImages.size()!=0)
		listSelectedImages.clear();

	int numImgs= listAllImages.size();
	int	numSelectedImgs=selectedImagesList.size();
//	Image *img;
	//img->getId()
	for(int i=0;i<numSelectedImgs;i++)
		for (int j=0;j<numImgs;j++)
			if (listAllImages.at(j)->getFilename()==selectedImagesList.at(i))
			{
				listSelectedImages.push_back(listAllImages.at(j));
				qDebug("IdImage: %d",listAllImages.at(j)->getId());
			}

	//listSelectedImages=listImages;
}

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


