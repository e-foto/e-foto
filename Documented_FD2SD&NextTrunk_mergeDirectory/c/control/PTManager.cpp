#include "PTManager.h"
#include "EFotoManager.h"
#include "PTUserInterface_Qt.h"

#include "EDom.h"
#include "Point.h"
#include "Image.h"
#include "InteriorOrientation.h"
#include "SensorWithFiducialMarks.h"
#include "Dms.h"
#include "ConvertionsSystems.h"

#include <stdlib.h>


PTManager::PTManager()
{

	efotoManager  = NULL;
	mySensor = NULL;
	myFlight = NULL;
	marksSaveState=true;
	//afp=d;
	started = false;
	status = false;
	previousData= false;
	maxIterations = 6;
	metricConvergency = 0.001;
	angularConvergency = 0.001;
}

PTManager::PTManager(EFotoManager *newManager, deque<Image*>images, deque<InteriorOrientation*> ois,Sensor *sensor, Flight *flight)
{
	efotoManager = newManager;
	listAllImages = images;
	listOis = ois;
	setListPoint();//lista todos os pontos
	mySensor = sensor;
	myFlight = flight;
	marksSaveState= true;
	started = false;
	status = false;

	setENH();

	previousData=false;
	/*
	EDomElement fotoTri(efotoManager->getXml("exteriorOrientation"));
	if (fotoTri.hasTagName("imageEO"))
	{
		EDomElement oe=fotoTri.elementByTagName("imageEO");
		if (oe.attribute("type")=="photoTriangulation")
		{
			loadFotoTriData(oe.getContent());
			previousData= true;
		}
	}*/
	maxIterations = 6;
	metricConvergency = 0.001;
	angularConvergency = 0.001;
	flightScale=myFlight->getScaleDen();
}

double PTManager::getFlightScale()
{
	return flightScale;
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
		qApp->processEvents();
		if (efotoManager->getInterfaceType().compare("Qt") == 0)
		{
			myInterface = PTUserInterface_Qt::instance(this);
		}
		//myFlight->setSensor(mySensor);
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
	//if (pt!=NULL)
		// delete pt;
	//delete myInterface;
	//delete mySensor;
	//delete myFlight;
	//listAllImages.clear();
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
	pt->setMaxNumberIterations(maxIterations);
	pt->setMetricConvergencyValue(metricConvergency);
	pt->setAngularConvergencyValue(angularConvergency);

	//photogrammetricSort(listSelectedPoints);

	if (pt->isPossibleCalculate())
	{
		bool result=pt->calculate();
		setMatrixAFP(pt->getAFP());
		residuos=pt->getMatRes();
		//residuos.show('f',4,"residuos");
		//deque<Point*> ptlist=pt->getPhotogrammetricList();
		/*
		for (int i=0;i<ptlist.size();i++)
		{
			pt->getResiduo(ptlist.at(i)).show('f',6,ptlist.at(i)->getPointId().c_str());
		}*/


		return result;
	}
	else
		return false;
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
			//qDebug("height:= %d",temp->getHeight());
			dimensions.setInt(1,temp->getHeight());
			dimensions.setInt(2,temp->getWidth());
			return dimensions;
		}

	}
}

void PTManager::setMatrixAFP(Matrix afp)
{
	Matrix result(afp.getRows(),afp.getCols());
	for (int i=1;i<=afp.getRows();i++)
	{
		result.set(i,1,Dms::radianoToDegreeDecimal(afp.get(i,1)));
		result.set(i,2,Dms::radianoToDegreeDecimal(afp.get(i,2)));
		double kappa0 = Dms::radianoToDegreeDecimal(afp.get(i,3));
		result.set(i,3,kappa0 > 180.0 ? kappa0-360: kappa0 );
		result.set(i,4,afp.get(i,4));
		result.set(i,5,afp.get(i,5));
		result.set(i,6,afp.get(i,6));
	}
	AFP=result;
}

Matrix PTManager::getMatrixOE()
{
	return AFP;
}

/**novo*/

void PTManager::setENH()
{
    //string pointxml=efotoManager->getXml("point");
	//EDomElement pointsXml(efotoManager->getXml("points"));
	//int children=pointsXml.children().size();

	Matrix points(listAllPoints.size(),3);
	Matrix pointKeys(listAllPoints.size(),1);
	//for (int i=1;i<=children;i++)
	for (int i=0;i<listAllPoints.size();i++)
    {
		//EDomElement point=pointsXml.elementByTagAtt("point","key",intToString(i));

		//string enh=point.elementByTagName("gml:pos").toString().c_str();
		Point* p = listAllPoints.at(i);
		double E = p->getObjectCoordinate().getX();
		double N = p->getObjectCoordinate().getY();
		double H = p->getObjectCoordinate().getZ();


		//int ini=enh.find_first_of(" ");
		//int fim=enh.find_last_of(" ");
		//double E=stringToDouble(enh.substr(0,ini).c_str());
		//double N=stringToDouble(enh.substr(ini+1,fim).c_str());
		//double H=stringToDouble(enh.substr(fim+1,enh.size()).c_str());
		//qDebug("%d\tE=%.4f\tN=%.4f\tH=%.4f",i,E,N,H);
		pointKeys.set(i+1,1,p->getId());
		points.set(i+1,1,E);
		points.set(i+1,2,N);
		points.set(i+1,3,H);
    }
	//	points.show('f',4);
	ENH=points;
	spareENH=ENH;
	spareENH.putMatrix(pointKeys,1,ENH.getCols()+1);
}

Matrix PTManager::getResiduos()
{
	return residuos;
}

Matrix PTManager::getENH()
{
    return ENH;
}

bool PTManager::connectImagePoints()
{
	if (!(started))
	{
		qApp->processEvents();
		EDomElement xmlPoints(efotoManager->getXml("points"));
		deque<EDomElement> allPoints = xmlPoints.elementsByTagName("point");
		for (unsigned int j = 0; j < listAllImages.size(); j++)
		{
			//listAllImages.at(j)->clearPoints();
			for (unsigned int i = 0; i < allPoints.size(); i++)
			{
				string data = allPoints.at(i).elementByTagAtt("imageCoordinates", "image_key", intToString(listAllImages.at(j)->getId())).getContent();
				//qDebug("%s\n",data.c_str());
				if (data != "")
				{
					Point* pointToInsert = efotoManager->instancePoint(stringToInt(allPoints.at(i).attribute("key")));
					if (pointToInsert != NULL)
					{
						//qDebug("connectImagePoints(): colocou um ponto: %s",pointToInsert->getPointId().c_str());
						listAllImages.at(j)->putPoint(pointToInsert);
						pointToInsert->putImage(listAllImages.at(j));//novo em teste:06/08/2011
					}
				}
			}
			//qDebug("Image %s",listAllImages.at(j)->getFilename().c_str());
			listAllImages.at(j)->sortPoints();
		}
		//qDebug("\n\n");
		return true;
	}
	return false;
}

void PTManager::setListPoint()
{
	EDomElement xmlPoints(efotoManager->getXml("points"));
	deque<EDomElement> allPoints = xmlPoints.elementsByTagName("point");
	for(int i=0;i<allPoints.size();i++)
	{
		Point* point= efotoManager->instancePoint(stringToInt(allPoints.at(i).attribute("key")));
		if (point != NULL)// && !point->is("CheckingPoint"))
			listAllPoints.push_back(point);
	}
}
// retorna uma lista com os nomes das imagens
deque<string> PTManager::getStringImages()
{
	deque<string> list;
	int numImages=listAllImages.size();
	for (int i=0;i<numImages;i++)
		list.push_back(listAllImages.at(i)->getFilename());
	return list;
}

string PTManager::getFilePath(string fileName)
{
	int numImages=listAllImages.size();
	for (int i=0;i<numImages;i++)
		if(listAllImages.at(i)->getFilename()==fileName)
		{
			string file = listAllImages.at(i)->getFilepath() + "/" +fileName;
			return file;
		}
	return "";
}

deque<string> PTManager::getStringTypePoints(string imageFileName)
{
	deque<string> list;
	if (imageFileName =="")
	{
		int numPnts=listAllPoints.size();
		for (int i=0; i<numPnts; i++)
		{
			if (listAllPoints.at(i) && listAllPoints.at(i)->is("ControlPoint"))
				list.push_back("Control");
			if (listAllPoints.at(i) && listAllPoints.at(i)->is("PhotogrammetricPoint"))
				list.push_back("Photogrammetric");
			if (listAllPoints.at(i) && listAllPoints.at(i)->is("CheckingPoint"))
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
			if (listAllPoints.at(j)->getPointId()==selectedPointsList.at(i) && !listAllPoints.at(j)->is("CheckingPoint"))
			{

				listSelectedPoints.push_back(listAllPoints.at(j));
				//qDebug("Achei IdPoint: %s",listAllPoints.at(j)->getPointId().c_str());
			}

}

BundleAdjustment* PTManager::getBundleAdjustment()
{
	return pt;
}

// retorna uma lista com os ids dos pontos
deque<string> PTManager::getStringIdPoints(string imageFileName, string cond)
{
	deque<string> list;
	if (imageFileName =="")
	{
		int numPnts=listAllPoints.size();
		for (int i=0; i<numPnts; i++)
		{
			Point *pnt=listAllPoints.at(i);
			if(!pnt->is("CheckingPoint") || (pnt->is("CheckingPoint") && cond!="noCheckingPoint"))
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
void PTManager::updateDigitalCoordinatesPoint(int imageId, int pointKey, int col, int lin)
{
	Point *pnt=efotoManager->instancePoint(pointKey);
//	qDebug("Antes ponto %d:\n%s\n\n",pointKey,pnt->xmlGetData().c_str());
	pnt->deleteDigitalCoordinate(imageId);
	DigitalImageSpaceCoordinate temp= DigitalImageSpaceCoordinate(imageId,col,lin);
	pnt->putDigitalCoordinate(temp);
	//qDebug("Depois ponto %d:\n%s\n\n",pointKey,pnt->xmlGetData().c_str());

//	qDebug("updateDigitalCoordinates:%dx%d %s->col: %d lin: %d",col,lin,pnt->getPointId().c_str(),pnt->getDigitalCoordinate(imageId).getCol(),pnt->getDigitalCoordinate(imageId).getLin());
}


bool PTManager::isAvailablePoint(int imageId, int pointKey)
{
//	Point *pnt=efotoManager->instancePoint(pointKey);
	return efotoManager->instancePoint(pointKey)->getDigitalCoordinate(imageId).isAvailable();
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

Matrix PTManager::getResiduoPhotogrammetric()
{
	int points=listSelectedPoints.size();
	Matrix residuos(0,3);
	for (int i=0;i<points;i++)
	{
		Point *pnt=listSelectedPoints.at(i);
		if (pnt->is("PhotogrammetricPoint"))
		{
			Matrix temp=pt->getResiduo(pnt);
			residuos=residuos|temp;
		}
	}
	return residuos;
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

void PTManager::saveResults()
{
	saveBundleAdjustment();
	saveMarks();
	setMarksSavedState(true);
	efotoManager->setSavedState(false);
}

Matrix PTManager::getMVC()
{
	return pt->getMVC();
}

// Retorna uma lista dos fileNames das imagens que contem o ponto
deque<string> PTManager::getImagesAppearances(int pointKey)
{
	deque<string> appearance;
	Point *pnt=efotoManager->instancePoint(pointKey);
	int numImages=pnt->countImages();
	for(int i=0;i<numImages;i++)
		appearance.push_back(pnt->getImageAt(i)->getFilename());
	return appearance;
}
void PTManager::saveMarks()
{
	//qDebug("Chamado metodo para salvar as marcas");
	EDomElement newXml(efotoManager->xmlGetData());
	/*
	for (int i=0; i<listAllImages.size(); i++)
	{
		Image *myImage = listAllImages.at(i);
		for (int j = 0; j < myImage->countPoints(); j++)
		{
			int currentPointId = myImage->getPointAt(j)->getId();
			newXml.replaceChildByTagAtt("point", "key", intToString(currentPointId), myImage->getPointAt(j)->xmlGetData().c_str());
			qDebug("ponto %d:\n%s\n\n",currentPointId,myImage->getPointAt(j)->xmlGetData().c_str());
		}
	}
	qDebug("NEWXML:\n%s",newXml.elementByTagName("points").getContent().c_str());*/

	for (int i=0; i<listAllPoints.size(); i++)
	{
		int currentPointId = listAllPoints.at(i)->getId();
		newXml.replaceChildByTagAtt("point", "key", intToString(currentPointId), listAllPoints.at(i)->xmlGetData().c_str());
		//qDebug("ponto %d:\n%s\n\n",currentPointId,listAllPoints.at(i)->xmlGetData().c_str());
	}
	//qDebug("NEWXML:\n%s",newXml.elementByTagName("points").getContent().c_str());
	efotoManager->xmlSetData(newXml.getContent());
}

void PTManager::saveBundleAdjustment()
{
	EDomElement newXml(efotoManager->xmlGetData());
	string fotoTriString= createBundleAdjustmentXml();
	newXml.replaceChildByTagName("exteriorOrientation",fotoTriString);
	efotoManager->xmlSetData(newXml.getContent());
}

string PTManager::createBundleAdjustmentXml()
{
	stringstream fotoTriXml;
//codigo de criaçao da xml da bundle(multiplas tags de Orientação Exterior)
	Matrix oe=pt->getAFP();
	string lb=pt->getLb().xmlGetData();
	string l0=pt->getL0().xmlGetData();
	fotoTriXml << "<exteriorOrientation>\n";
	for (int i=1;i<=oe.getRows();i++)
	{
		fotoTriXml << "\t<imageEO type=\"photoTriangulation\" image_key=\"" << intToString(i) << "\">\n";
		fotoTriXml << "\t\t<iterations>"<< pt->getTotalIterations() <<"</iterations>\n";
		fotoTriXml << "\t\t<converged>"<< (pt->isConverged()? "true":"false")<<"</converged>\n";
		fotoTriXml << "\t\t<parameters>\n";
		fotoTriXml << "\t\t\t<Lb>\n";
		fotoTriXml << lb << "\n</Lb>\n";
		fotoTriXml << "\t\t\t<Xa>\n";
		fotoTriXml << "\t\t\t\t<X0 uom=\"#m\">"<< doubleToString(oe.get(i,4)) << "</X0>\n";
		fotoTriXml << "\t\t\t\t<Y0 uom=\"#m\">"<< doubleToString(oe.get(i,5)) << "</Y0>\n";
		fotoTriXml << "\t\t\t\t<Z0 uom=\"#m\">"<< doubleToString(oe.get(i,6)) << "</Z0>\n";
		fotoTriXml << "\t\t\t\t<phi uom=\"#m\">"<< doubleToString(oe.get(i,2)) << "</phi>\n";
		fotoTriXml << "\t\t\t\t<omega uom=\"#m\">"<< doubleToString(oe.get(i,1)) << "</omega>\n";
		fotoTriXml << "\t\t\t\t<kappa uom=\"#m\">"<< doubleToString(oe.get(i,3)) << "</kappa>\n";
		fotoTriXml << "\t\t\t</Xa>\n";
		fotoTriXml << "\t\t\t<L0>\n";
		fotoTriXml << l0 << "\n</L0>\n";
		fotoTriXml << "\t\t</parameters>\n";
		fotoTriXml << "</imageEO>\n";
	}
	fotoTriXml << "</exteriorOrientation>\n";

	//qDebug("Metodo chamado create varias OEs");
	return fotoTriXml.str();
}

void PTManager::setMarksSavedState(bool marksState)
{
	marksSaveState=marksState;
	if (marksSaveState)
		efotoManager->setSavedState(false);
}

bool PTManager::getMarksSavedState()
{
	return marksSaveState;
}

void PTManager::loadFotoTriData(string fotoTriData)
{
	//Confirmar direitinho o XML
	//Lo Lb Iteracoes convergencia são comuns a todas as OES.
	// E necessario repetir para cada imagem??
	// Ou deixar esses elementos numa area comun e so subdividir os parametros da orientacao

	EDomElement oe(fotoTriData);
	int numIterations=oe.elementByTagName("iterations").toInt();
	//bool converged = oe.elementByTagName("converged");
	Matrix Lb;
	Lb.xmlSetData(oe.elementByTagName("Lb").getContent());

}

bool PTManager::hasPreviousData()
{
	return previousData;
}

void PTManager::setImageFlightDirection(string imageFile, double flightDirection)
{
	for (int i=0;i<listAllImages.size();i++)
		if(listAllImages.at(i)->getFilename() == imageFile)
		{
			listAllImages.at(i)->setFlightDirection(flightDirection);
			qDebug("File %s kappa0 = %.7f",imageFile.c_str(),flightDirection);
		}
}

double PTManager::getImageFlightDirection(string imageFile)
{
	for (int i=0;i<listAllImages.size();i++)
		if(listAllImages.at(i)->getFilename() == imageFile)
		{
			//qDebug("File %s kappa0 = %.7f",imageFile.c_str(),listAllImages.at(i)->getFlightDirection());
			return listAllImages.at(i)->getFlightDirection();
		}
	return -1;
}

double PTManager::getLongitudinalOverlap(string imageFile)
{
	for (int i=0;i<listAllImages.size();i++)
		if(listAllImages.at(i)->getFilename() == imageFile)
			return listAllImages.at(i)->getFlight()->getLongitudinalOverlap();
	return -1;
}

string PTManager::exportBlockTokml(string fileName)
{
	EDomElement terrain = efotoManager->getXml("terrain");
	char hemiLatitude = terrain.elementByTagName("Lat").attribute("direction")=="S" ? 'S':'N';
	int zona=stringToInt(terrain.elementByTagName("utmFuse").toString());
	GeoSystem sys(terrain.elementByTagName("GRS").toString());

	stringstream aux;
	aux << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<kml xmlns=\"http://www.opengis.net/kml/2.2\" xmlns:gx=\"http://www.google.com/kml/ext/2.2\" xmlns:kml=\"http://www.opengis.net/kml/2.2\" xmlns:atom=\"http://www.w3.org/2005/Atom\">\n";
	aux	<< "<Document>\n<name>" << fileName <<"</name>\n";

	string controlPointIcon         = "http://maps.google.com/mapfiles/kml/paddle/wht-blank.png";
	string photogrammetricPointIcon = "http://maps.google.com/mapfiles/kml/shapes/placemark_circle.png";
	//string photogrammetricPointIcon = "http://marmsx.msxall.com/artgallery/ink/efoto.png";
	string checkingPointIcon        = "http://maps.google.com/mapfiles/kml/paddle/grn-blank.png";

	string colorNormalControl="ff00ff00";
	string colorNormalPhotogrammetric = "ff00ffff";
	string colorNormalChecking ="ff12f580" ;//18--245-128

	string colorHighLightControl="ff0000ff";
	string colorHighLightPhotogrammetric="ff0000ff";
	string colorHighLightChecking="ff12f580";

	// Style para pontos de controle
	aux<<"<StyleMap id=\"controlPoint\">\n<Pair>\n<key>normal</key>\n<styleUrl>#sn_ControlPoint</styleUrl>\n</Pair>\n<Pair>\n<key>highlight</key>\n<styleUrl>#sh_ControlPoint</styleUrl>\n</Pair>\n</StyleMap>\n";
	aux<<"<Style id=\"sn_ControlPoint\">\n<IconStyle>\n<color>"<<colorNormalControl<<"</color>\n<scale>1.0</scale>\n<Icon>\n<href>"<< controlPointIcon <<"</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n</Style>\n";
	aux<<"<Style id=\"sh_ControlPoint\">\n<IconStyle>\n<color>"<<colorHighLightControl<<"</color>\n<scale>1.1</scale>\n<Icon>\n<href>"<< controlPointIcon <<"</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n</Style>\n";

	// Style para pontos fotogrametricos
	aux<<"<StyleMap id=\"photogrammetricPoint\">\n<Pair>\n<key>normal</key>\n<styleUrl>#sn_PhotogrammetricPoint</styleUrl>\n</Pair>\n<Pair>\n<key>highlight</key>\n<styleUrl>#sh_PhotogrammetricPoint</styleUrl>\n</Pair>\n</StyleMap>\n";
	aux<<"<Style id=\"sn_PhotogrammetricPoint\">\n<IconStyle>\n<color>"<<colorNormalPhotogrammetric<<"</color>\n<scale>1.0</scale>\n<Icon>\n<href>"<< photogrammetricPointIcon <<"</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n</Style>\n";
	aux<<"<Style id=\"sh_PhotogrammetricPoint\">\n<IconStyle>\n<color>"<<colorHighLightPhotogrammetric<<"</color>\n<scale>1.1</scale>\n<Icon>\n<href>"<< photogrammetricPointIcon <<"</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n</Style>\n";

	// Style para pontos de checking
	aux<<"<StyleMap id=\"checkingPoint\">\n<Pair>\n<key>normal</key>\n<styleUrl>#sn_CheckingPoint</styleUrl>\n</Pair>\n<Pair>\n<key>normal</key>\n<styleUrl>#sh_CheckingPoint</styleUrl>\n</Pair>\n</StyleMap>\n";
	aux<<"<Style id=\"sn_CheckingPoint\">\n<IconStyle>\n<color>"<<colorNormalChecking<<"</color>\n<scale>1.0</scale>\n<Icon>\n<href>"<< checkingPointIcon <<"</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n</Style>\n";
	aux<<"<Style id=\"sh_CheckingPoint\">\n<IconStyle>\n<color>"<<colorHighLightChecking<<"</color>\n<scale>1.1</scale>\n<Icon>\n<href>"<< checkingPointIcon <<"</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n</Style>\n";


	Matrix oe=getMatrixOE();
	//oe.show();
	for (int i=0;i<listAllImages.size();i++)
	{
		string icon= "http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png";
		int B=random()%15;
		int G=random()%15;
		int R=random()%15;

		stringstream aux1;

		aux1 << hex << "ff"<< B << G << R << B << G << R;
		string lineColor =aux1.str();
		//qDebug("B: %d\tG: %d\tR: %d string: %s",B,G,R,lineColor.c_str());
		//string lineColor= "ffff0000"; //codigo hexadecimal alphaBGR

		string lineWidth= "2";
		aux<<"<StyleMap id=\"msn_ylw-pushpin" << intToString(i) <<"\">\n<Pair>\n<key>normal</key>\n<styleUrl>#sn_ylw-pushpin"<<intToString(i)<< "</styleUrl>\n</Pair>\n<Pair>\n<key>highlight</key>\n<styleUrl>#sh_ylw-pushpin"<<intToString(i)<< "</styleUrl>\n</Pair>\n</StyleMap>\n";
		aux<<"<Style id=\"sn_ylw-pushpin"<<intToString(i)<< "\">\n<IconStyle>\n<scale>1.3</scale>\n<Icon>\n<href>"<< icon <<"</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n" << "<LineStyle>\n<color>"<< lineColor <<"</color>\n<width>"<< lineWidth<<"</width>\n</LineStyle>\n<PolyStyle>\n<fill>0</fill>\n</PolyStyle>\n</Style>\n";
		aux<<"<Style id=\"sh_ylw-pushpin"<<intToString(i)<< "\">\n<IconStyle>\n<scale>1.3</scale>\n<Icon>\n<href>"<< icon <<"</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n" << "<LineStyle>\n<color>"<< lineColor <<"</color>\n<width>"<< lineWidth<<"</width>\n</LineStyle>\n<PolyStyle>\n<fill>0</fill>\n</PolyStyle>\n</Style>\n";

		Image *img=listAllImages.at(i);
		int width=img->getWidth();
		int height=img->getHeight();
		double Z0=oe.get(i+1,6);
		double c=img->getSensor()->getFocalDistance();

		double P1x=pt->digitalToAnalog(listOis.at(i),height/2,0).get(1,1);
		double PPx=img->getSensor()->getPrincipalPointCoordinates().getXi();
		double PPy=img->getSensor()->getPrincipalPointCoordinates().getEta();
		double P2y=pt->digitalToAnalog(listOis.at(i),0,width/2).get(1,2);

		double deltaE=Z0*(P1x-PPx)/c;
		double deltaN=Z0*(P2y-PPy)/c;

		//qDebug("deltaE : %.3f",deltaE);
		//qDebug("deltaN : %.3f",deltaN);

		double E1=oe.get(i+1,4)-deltaE;
		double N1=oe.get(i+1,5)+deltaN;
		double E2=oe.get(i+1,4)+deltaE;
		double N2=oe.get(i+1,5)-deltaN;


		Matrix plh1=ConvertionsSystems::utmToGeoFran(E1,N1,0,zona,GeoSystem(),hemiLatitude);
		Matrix plh2=ConvertionsSystems::utmToGeoFran(E2,N2,0,zona,GeoSystem(),hemiLatitude);

		double lat1 = plh1.get(1,1)*180/M_PI;
		lat1 = (hemiLatitude =='S'? -lat1 : lat1);
		double longi1 = plh1.get(1,2)*180/M_PI;

		double lat2 = plh2.get(1,1)*180/M_PI;
		lat2 = (hemiLatitude =='S'? -lat2 : lat2);
		double longi2 =plh2.get(1,2)*180/M_PI;

		stringstream coord;
		coord << doubleToString(longi1) << "," <<doubleToString(lat1) << ",0 ";
		coord << doubleToString(longi2) << "," <<doubleToString(lat1) << ",0 ";
		coord << doubleToString(longi2) << "," <<doubleToString(lat2) << ",0 ";
		coord << doubleToString(longi1) << "," <<doubleToString(lat2) << ",0 ";
		coord << doubleToString(longi1) << "," <<doubleToString(lat1) << ",0 ";
		string coordenadas=coord.str();

		aux << "<Placemark>\n";
		aux << "<name>"<< img->getFilename() << "</name>\n";
		aux << "<styleUrl>";
		aux << "#msn_ylw-pushpin" <<intToString(i)<< "</styleUrl>\n";
		/*
		aux << "<Polygon>\n";
		aux << "<tessellate>1</tessellate>\n";
		aux << "<outerBoundaryIs>\n";
		aux << "<LinearRing>\n";
		aux << "<coordinates>\n";
		aux << coordenadas << "\n";
		aux << "</coordinates>\n";
		aux << "</LinearRing>\n";
		aux << "</outerBoundaryIs>\n";
		aux << "</Polygon>\n";
		*/
		aux << "<LineString>\n";
		aux << "<tessellate>1</tessellate>\n";
		aux << "<coordinates>\n";
		aux << coordenadas << "\n";
		aux << "</coordinates>\n";
		aux << "</LineString>\n";
		aux << "</Placemark>\n";
}

	aux << "<Folder>\n";
	aux << "<name>" << "Projeto do e-foto"<< "</name>\n";
	//qDebug("%c\t%d\t%s",latitude,zona,sys.getSystemName().c_str());
	for(int i=0;i<listAllPoints.size();i++)
	{
		Point *pnt=listAllPoints.at(i);
		string pointType;
		if (pnt->is("PhotogrammetricPoint"))
			pointType = "photogrammetricPoint";
		else if (pnt->is("ControlPoint"))
			pointType = "controlPoint";
		else if (pnt->is("CheckingPoint"))
			pointType = "checkingPoint";

		double E=pnt->getObjectCoordinate().getX();
		double N=pnt->getObjectCoordinate().getY();
		double H=pnt->getObjectCoordinate().getZ();

		Matrix plh=ConvertionsSystems::utmToGeoFran(E,N,H,zona,sys,hemiLatitude);

		double lat=plh.get(1,1)*180/M_PI;
		double longi=plh.get(1,2)*180/M_PI;

		lat = (hemiLatitude=='S' ? -lat:lat);

		aux << "<Placemark>\n";
		aux << "<name>" << pnt->getPointId() << "</name>\n";
		aux << "<description>" << pnt->getDescription() << "</description>\n";
		aux <<pnt->getDescription();
		aux << "<LookAt>\n";
		aux << "<longitude>";
		aux << doubleToString(longi);
		aux << "</longitude>\n";
		aux <<  "<latitude>";
		aux << doubleToString(lat);
		aux << "</latitude>\n";
		aux << "<altitude>";
		aux << doubleToString(H)<<"</altitude>\n";
		aux << "<heading>0.01795058696543714</heading>\n";
		aux << "<tilt>0</tilt>\n";
		aux << "<range>1463.920597456832</range>\n";
		aux << "<altitudeMode>relativeToGround</altitudeMode>\n";
		aux << "<gx:altitudeMode>relativeToSeaFloor</gx:altitudeMode>\n";
		aux << "</LookAt>\n";
		//aux << "<styleUrl>#msn_ylw-pushpin</styleUrl>\n";
		aux << "<styleUrl>#" << pointType <<"</styleUrl>\n";
		aux << "<Point>\n";
		aux << "<altitudeMode>clampToGround</altitudeMode>\n";
		aux << "<gx:altitudeMode>clampToSeaFloor</gx:altitudeMode>\n";
		aux << "<coordinates>";
		aux << doubleToString(longi);
		aux << ",";
		aux << doubleToString(lat);
		aux << ",";
		aux << doubleToString(H) <<"</coordinates>\n";
		aux << "</Point>\n";
		aux << "</Placemark>\n";
	}
	aux << "</Folder>\n";
	aux << "</Document>\n</kml>";
	EDomElement xmlgoogle(aux.str());

	return xmlgoogle.indent('\t').getContent();

}

void PTManager::setMetricConvergencyValue(double value)
{
	metricConvergency=value;
}

double PTManager::getMetricConvergencyValue()
{
	return metricConvergency;
}

void PTManager::setAngularConvergencyValue(double value)
{
	angularConvergency=value;
}

double PTManager::getAngularConvergencyValue()
{
	return angularConvergency;
}

void PTManager::setMaxIteration(int iterations)
{
	maxIterations=iterations;
}

int PTManager::getMaxIteration()
{
	return maxIterations;
}

void PTManager::reloadPointsCoordinates()
{
	int numPoints=listAllPoints.size();
	for (int i=0;i<numPoints;i++)
	{
		Point *pnt=efotoManager->instancePoint(spareENH.getInt(i+1,4));

		double x=spareENH.get(i+1,1);
		double y=spareENH.get(i+1,2);
		double z=spareENH.get(i+1,3);

		pnt->getObjectCoordinate().setX(x);
		pnt->getObjectCoordinate().setY(y);
		pnt->getObjectCoordinate().setZ(z);
	}

}












































/** Em teste de sort dos pontos fotogrametricos segundo Francisco,TFC.
*/
void PTManager::photogrammetricSort()
{
	pt->getInicialsValues();
	Matrix oe=pt->getMatrixInicialValues();
	for (int i=0;i<listAllPoints.size();i++)
	{
		Point *pnt=listAllPoints.at(i);
		Image *img=pnt->getImageAt(0);
		DigitalImageSpaceCoordinate coord=pnt->getDigitalCoordinate(img->getId());
		digitalToEN(img,coord.getCol(),coord.getLin(),oe);
	}
}

Matrix PTManager::digitalToEN(Image *img,int col, int row,Matrix oe)
{
	//oe.show('f',3,"OE INICIAL");
	int index=whereInImages(img);
	if (index<0)
		return Matrix(1,2);

	//int width=img->getWidth();
	//int height=img->getHeight();
	double Z0=oe.get(index+1,6);
	double c=img->getSensor()->getFocalDistance();

	double P1x=pt->digitalToAnalog(img->getIO(),row,col).get(1,1);
	double P1y=pt->digitalToAnalog(img->getIO(),row,col).get(1,2);

	double PPx=img->getSensor()->getPrincipalPointCoordinates().getXi();
	double PPy=img->getSensor()->getPrincipalPointCoordinates().getEta();

	//double P1y=pt->digitalToAnalog(listOis.at(i),0,width/2).get(1,2);

	double deltaE=Z0*(P1x-PPx)/c;
	double deltaN=Z0*(P1y-PPy)/c;

	//qDebug("deltaE : %.3f",deltaE);
	//qDebug("deltaN : %.3f",deltaN);

	double E1=oe.get(index+1,4)+deltaE;
	double N1=oe.get(index+1,5)+deltaN;
	//double E2=oe.get(i+1,4)+deltaE;
	//double N2=oe.get(i+1,5)-deltaN;


	//Matrix plh1=ConvertionsSystems::utmToGeoFran(E1,N1,0,zona,GeoSystem(),hemiLatitude);
	//Matrix plh2=ConvertionsSystems::utmToGeoFran(E2,N2,0,zona,GeoSystem(),hemiLatitude);
/*
	double lat1 = plh1.get(1,1)*180/M_PI;
	lat1 = (hemiLatitude =='S'? -lat1 : lat1);
	double longi1 = plh1.get(1,2)*180/M_PI;
/*
	double lat2 = plh2.get(1,1)*180/M_PI;
	lat2 = (hemiLatitude =='S'? -lat2 : lat2);
	double longi2 =plh2.get(1,2)*180/M_PI;*/
	Matrix result(1,2);
	result.set(1,1,E1);
	result.set(1,2,N1);
	qDebug("imagem 1 ponto 1, col: %d \tlin:%d",col,row);
	result.show('f',4,"Matrix EN");
	return result;
}

// Retorna a posiçao de 'img' no deque de imagens selecionadas, se nao estiver retorna -1
int PTManager::whereInSelectedImages(Image *img)
{
	for (int i=0;i<listSelectedImages.size();i++)
	{
		if (listSelectedImages.at(i)==img)
			return i;
	}
	return -1;
}

// Retorna a posiçao de 'img' no deque de imagens, se nao estiver retorna -1
int PTManager::whereInImages(Image *img)
{
	for (int i=0;i<listAllImages.size();i++)
	{
		if (listAllImages.at(i)==img)
			return i;
	}
	return -1;
}

/*
int numPoints=listAllPoints.size();
int numImages=listAllImages.size();

//Matrix overlapLongiTable(numImages,numImages);
//Matrix overlapTransTable(numImages,numImages);
//Matrix divisor(numImages,numImages);

Matrix overlapLongiTable(59,59);
Matrix overlapTransTable(59,59);
Matrix divisor(59,59);

//divisor.ones();
//overlapLongiTable.identity(listAllImages.size());

for (int i=0;i<numPoints;i++)
{
	Point *pnt=listAllPoints.at(i);
	deque<DigitalImageSpaceCoordinate> listCoord=pnt->getDigitalCoordinates();
	//printf("%s\n",pnt->getPointId().c_str());
	if (listCoord.size()>1)
	{

		for (int j=0;j<listCoord.size();j++)
		{
			DigitalImageSpaceCoordinate coord1=listCoord.at(j);
			int imageId1=coord1.getImageId();
			int col1=coord1.getCol();
			int row1=coord1.getLin();
			int width1=efotoManager->instanceImage(imageId1)->getWidth();

			//printf("col %d = %.3d\n",imageId1,col1);
			//printf("width = %d\n",width1);
			for (int k=j+1;k<listCoord.size();k++)
			{
				DigitalImageSpaceCoordinate coord2=listCoord.at(k);
				int imageId2=coord2.getImageId();
				int col2=coord2.getCol();
				int row2=coord2.getLin();
				//int width2=efotoManager->instanceImage(imageId2)->getWidth();

				double overlapLongi12=1.0*(col1 - col2)/width1;
				double overlapLongi21=-overlapLongi12;

				double overlapTrans12=1.0*(row1 - row2)/width1;
				double overlapTrans21=-overlapTrans12;

				//printf("overlap: [%d,%d] = %.3f \n",imageId1,imageId2,overlapLongi12);

				overlapLongiTable.set(imageId1,imageId2,overlapLongiTable.get(imageId1,imageId2)+overlapLongi12);
				overlapLongiTable.set(imageId2,imageId1,overlapLongiTable.get(imageId2,imageId1)+overlapLongi21);

				overlapTransTable.set(imageId1,imageId2,overlapTransTable.get(imageId1,imageId2)+overlapTrans12);
				overlapTransTable.set(imageId2,imageId1,overlapTransTable.get(imageId2,imageId1)+overlapTrans21);

				divisor.set(imageId1,imageId2,divisor.getInt(imageId1,imageId2)+1);
				divisor.set(imageId2,imageId1,divisor.getInt(imageId2,imageId1)+1);
			}
		}
	}
	//printf("\n\n");
}

for (int i=1;i<=overlapLongiTable.getRows();i++)
{
	for (int j=1;j<=overlapLongiTable.getCols();j++)
	{
		double overlap=overlapLongiTable.get(i,j);
		if(!isnan(overlap))
		{
			if (overlap<0)
				overlapLongiTable.set(i,j,-1-overlap/divisor.get(i,j));
			else
				overlapLongiTable.set(i,j,1-overlap/divisor.get(i,j));
		}
	}
}

//overlapLongiTable.show('f',3,"OverlapLongiTable");

for (int i=1;i<=overlapTransTable.getRows();i++)
{
	for (int j=1;j<=overlapTransTable.getCols();j++)
	{
		double overlap=overlapTransTable.get(i,j);
		if(!isnan(overlap))
		{
			if (overlap<0)
				overlapTransTable.set(i,j,-1-overlap/divisor.get(i,j));
			else
				overlapTransTable.set(i,j,1-overlap/divisor.get(i,j));
		}
	}
}

//overlapTransTable.show('f',3,"OverlapTransTable");

//deque<Image*> faixa;
for (int i=1;i<=overlapLongiTable.getRows();i++)
{
	Image *img1=efotoManager->instanceImage(i);
	for (int j=i+1; j<=overlapLongiTable.getCols();j++)
	{
		double longi=overlapLongiTable.get(i,j);
		double trans=overlapTransTable.get(i,j);

		Image *img2=efotoManager->instanceImage(j);
		if (!isnan(longi) && !isnan(trans))
		{
			if (trans>0.9 || trans <-0.9)
			{
				if (longi>0)
					qDebug("imagem %s esta a esquerda de %s\n",img1->getFilename().c_str(),img2->getFilename().c_str());
				if (longi<0)
					qDebug("imagem %s esta a direita de %s\n",img1->getFilename().c_str(),img2->getFilename().c_str());
			}
			/*
			else if (0<trans && trans<0.9)
			{
				if (longi>0.9 || longi<-0.9)
					qDebug("imagem %s esta acima de %s\n",img1->getFilename().c_str(),img2->getFilename().c_str());
				//if (longi<0)
				//printf("imagem %d esta a direita de %d",i,j);
			}
			else if (-0.9<trans && trans <0 )
				qDebug("imagem %s esta acima de %s\n",img1->getFilename().c_str(),img2->getFilename().c_str());*/
			/*
			if (longi>0.9 || longi<-0.9)
			{
				if (trans>0)
					qDebug("imagem %s esta acima de %s\n",img1->getFilename().c_str(),img2->getFilename().c_str());
				if (trans<0)
					qDebug("imagem %s esta abaixo de %s\n",img1->getFilename().c_str(),img2->getFilename().c_str());
			}
		}
	}
}
*/