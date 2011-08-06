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
	listImages = images;
	listOis = ois;
	setListPoint();//lista todos os pontos
	mySensor = sensor;
	myFlight = flight;

	started = false;
	status = false;

	setENH();
	setColLin();
	setBLC();
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
	if (efotoManager != NULL && mySensor != NULL && myFlight != NULL && listImages.size()> 1 && listOis.size()>1)
	{
		if (efotoManager->getInterfaceType().compare("Qt") == 0)
		{
			myInterface = PTUserInterface_Qt::instance(this);
		}
		myFlight->setSensor(mySensor);
		for (int i=0; i<listImages.size(); i++)
		{
			mySensor->putImage(listImages.at(i));
			listImages.at(i)->setSensor(mySensor);
			listImages.at(i)->setFlight(myFlight);
			listImages.at(i)->setIO(listOis.at(i));
			listOis.at(i)->setImage(listImages.at(i));
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
    string imagefile=listImages.at(imageId)->getFilepath();
    imagefile +="/";
    imagefile +=listImages.at(imageId)->getFilename();

    return imagefile;
}

bool PTManager::calculatePT()
{
	//int cols=getX().getCols();
	//getY().show();
	//getZ().show();
	//getOis();
	pt= new BundleAdjustment(getX(),getY(),getZ(),getCol(),getLin(),getOis(),getBLC(),mySensor,1);

		//deque<InteriorOrientation*>listOis;// confirmar isso urgentemente;

		BundleAdjustment *teste= new BundleAdjustment(listImages,listPoints,1);
		teste->calculateObject();

        bool result=pt->calculate();
    //bool result = true;

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



Matrix PTManager::getX()
{
	int rows=BLC.getRows();
	int cols=BLC.getCols();
	Matrix X(rows,cols);
	for (int i=1;i<=rows;i++)
	{
		for (int j=1;j<=cols;j++)
		{
			if (BLC.getInt(i,j)==1)
				X.set(i,j,ENH.get(j,1));
			else if (BLC.getInt(i,j)==-1)
				X.set(i,j,-1);
			else
				X.set(i,j,0);
		}
	}
	return X;
}

Matrix PTManager::getY()
{
	int rows=BLC.getRows();
	int cols=BLC.getCols();
	Matrix Y(rows,cols);
	for (int i=1;i<=rows;i++)
	{
		for (int j=1;j<=cols;j++)
		{
			if (BLC.getInt(i,j)==1)
				Y.set(i,j,ENH.get(j,2));
			else if (BLC.getInt(i,j)==-1)
				Y.set(i,j,-1);
			else
				Y.set(i,j,0);
		}
	}
	return Y;
}
Matrix PTManager::getZ()
{
	int rows=BLC.getRows();
	int cols=BLC.getCols();
	Matrix Z(rows,cols);
	for (int i=1;i<=rows;i++)
	{
		for (int j=1;j<=cols;j++)
		{
			if (BLC.getInt(i,j)==1)
				Z.set(i,j,ENH.get(j,3));
			else if (BLC.getInt(i,j)==-1)
				Z.set(i,j,-1);
			else
				Z.set(i,j,0);
		}
	}
	return Z;
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

Matrix PTManager::getMatrixL0()
{
    return pt->getL0();
}

Matrix PTManager::getMatrixLb()
{
    return pt->getLb();
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
		for (unsigned int j = 0; j < listImages.size(); j++)
		{
			for (unsigned int i = 0; i < allPoints.size(); i++)
			{
				string data = allPoints.at(i).elementByTagAtt("imageCoordinates", "image_key", intToString(listImages.at(j)->getId())).getContent();
				if (data != "")
				{
					Point* pointToInsert = efotoManager->instancePoint(stringToInt(allPoints.at(i).attribute("key")));
					if (pointToInsert != NULL)
					{
						//qDebug("connectImagePoints(): colocou um ponto");
						listImages.at(j)->putPoint(pointToInsert);
						pointToInsert->putImage(listImages.at(j));//nove em teste:06/08/2011
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
		listPoints.push_back(point);
	}


}


