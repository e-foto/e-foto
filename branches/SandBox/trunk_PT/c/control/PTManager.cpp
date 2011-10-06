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


//#include <qdebug.h>

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
	if (pt->isPossibleCalculate())
	{
		bool result=pt->calculate();
		setMatrixAFP(pt->getAFP());
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
		points.set(i+1,1,E);
		points.set(i+1,2,N);
		points.set(i+1,3,H);
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
	qDebug("Chamado metodo para salvar as marcas");
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
		qDebug("ponto %d:\n%s\n\n",currentPointId,listAllPoints.at(i)->xmlGetData().c_str());
	}
	qDebug("NEWXML:\n%s",newXml.elementByTagName("points").getContent().c_str());

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

	qDebug("Metodo chamado create varias OEs");
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
	//Image *img;
//	img->setFlightDirection();
	for (int i=0;i<listAllImages.size();i++)
		if(listAllImages.at(i)->getFilename() == imageFile)
		{
			listAllImages.at(i)->setFlightDirection(flightDirection);
			qDebug("File %s kappa0 = %.7f",imageFile.c_str(),flightDirection);
		}
}

string PTManager::exportBlockTokml(string fileName)
{
	EDomElement terrain = efotoManager->getXml("terrain");
	char latitude = terrain.elementByTagName("Lat").attribute("direction")=="S" ? 'S':'N';
	int zona=stringToInt(terrain.elementByTagName("utmFuse").toString());
	GeoSystem sys(terrain.elementByTagName("GRS").toString());

	stringstream aux;
	aux << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<kml xmlns=\"http://www.opengis.net/kml/2.2\" xmlns:gx=\"http://www.google.com/kml/ext/2.2\" xmlns:kml=\"http://www.opengis.net/kml/2.2\" xmlns:atom=\"http://www.w3.org/2005/Atom\">\n";
	aux	<< "<Document>\n<name>" << fileName <<"</name>\n";
	aux<<"<StyleMap id=\"msn_ylw-pushpin\">\n<Pair>\n<key>normal</key>\n<styleUrl>#sn_ylw-pushpin1</styleUrl>\n</Pair>\n</StyleMap>";
	aux<<"<Style id=\"sn_ylw-pushpin\">\n<IconStyle>\n<scale>1.3</scale>\n<Icon>\n<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n</Style>\n";
	aux<<"<Style id=\"sh_ylw-pushpin\">\n<IconStyle>\n<scale>1.3</scale>\n<Icon>\n<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n</Style>\n";

	{	/*QString aux="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<kml xmlns=\"http://www.opengis.net/kml/2.2\" xmlns:gx=\"http://www.google.com/kml/ext/2.2\" xmlns:kml=\"http://www.opengis.net/kml/2.2\" xmlns:atom=\"http://www.w3.org/2005/Atom\">\n<Document>\n<name>Imagens 0489 e 0490.kml</name>\n<Style id=\"sn_ylw-pushpin9\">\n<IconStyle>\n<scale>1.1</scale>\n<Icon>\n<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n</Style>\n<Style id=\"sh_ylw-pushpin9\">\n<IconStyle>\n<scale>1.3</scale>\n<Icon>\n<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n</Style>\n<StyleMap id=\"msn_ylw-pushpin4\">\n<Pair>\n<key>normal</key>\n<styleUrl>#sn_ylw-pushpin1</styleUrl>\n</Pair>\n<Pair>\n";
	aux.append("<key>highlight</key>\n<styleUrl>#sh_ylw-pushpin6</styleUrl>\n</Pair>\n</StyleMap>\n<Style id=\"sh_ylw-pushpin4\">\n<IconStyle>\n<scale>1.3</scale>\n<Icon>\n<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>\n</Icon>\n	<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n</Style>\n<StyleMap id=\"msn_ylw-pushpin5\">\n<Pair>\n<key>normal</key>\n<styleUrl>#sn_ylw-pushpin3</styleUrl>\n</Pair>\n<Pair>\n<key>highlight</key>\n<styleUrl>#sh_ylw-pushpin9</styleUrl>\n</Pair>\n</StyleMap>\n<StyleMap id=\"msn_ylw-pushpin1\">\n<Pair>\n<key>normal</key>\n<styleUrl>#sn_ylw-pushpin</styleUrl>\n</Pair>\n<Pair>\n<key>highlight</key>\n<styleUrl>#sh_ylw-pushpin5</styleUrl>\n</Pair>\n</StyleMap>\n<Style id=\"sn_ylw-pushpin5\">\n<IconStyle>\n<scale>1.1</scale>\n<Icon>\n<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>\n</Icon>\n");
	aux.append("<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n</Style>\n<Style id=\"sh_ylw-pushpin1\">\n<IconStyle>\n<scale>1.3</scale>\n<Icon>\n<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n</Style>\n<StyleMap id=\"msn_ylw-pushpin6\">\n<Pair>\n<key>normal</key>\n<styleUrl>#sn_ylw-pushpin7</styleUrl>\n</Pair>\n<Pair>\n<key>highlight</key>\n<styleUrl>#sh_ylw-pushpin8</styleUrl>\n</Pair>\n</StyleMap>\n<StyleMap id=\"msn_ylw-pushpin\">\n<Pair>\n<key>normal</key>\n<styleUrl>#sn_ylw-pushpin4</styleUrl>\n</Pair>\n<Pair>\n<key>highlight</key>\n<styleUrl>#sh_ylw-pushpin</styleUrl>\n</Pair>\n</StyleMap>\n<Style id=\"sh_ylw-pushpin2\">\n<IconStyle>\n<scale>1.3</scale>\n<Icon>\n<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>\n");
	aux.append("</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n</Style>\n<Style id=\"sn_ylw-pushpin2\">\n<IconStyle>\n<scale>1.1</scale>\n<Icon>\n<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n</Style>\n<Style id=\"sh_ylw-pushpin7\">\n<IconStyle>\n<scale>1.3</scale>\n<Icon>\n<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n</Style>\n<StyleMap id=\"msn_ylw-pushpin0\">\n<Pair>\n<key>normal</key>\n<styleUrl>#sn_ylw-pushpin10</styleUrl>\n</Pair>\n<Pair>\n<key>highlight</key>\n<styleUrl>#sh_ylw-pushpin1</styleUrl>\n</Pair>\n</StyleMap>\n<Style id=\"sh_ylw-pushpin10\">\n<IconStyle>\n<scale>1.3</scale>\n<Icon>\n<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n</Style>\n<Style id=\"sh_ylw-pushpin3\">\n<IconStyle>\n");
	aux.append("<scale>1.3</scale>\n<Icon>\n<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n</Style>\n<Style id=\"sn_ylw-pushpin8\">\n<IconStyle>\n<scale>1.1</scale>\n<Icon>\n<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n<LineStyle>\n<color>ffffff00</color>\n<width>2</width>\n</LineStyle>\n<PolyStyle>\n<color>ff0000aa</color>\n<fill>0</fill>\n</PolyStyle>\n</Style>\n<Style id=\"sh_ylw-pushpin\">\n<IconStyle>\n<scale>1.3</scale>\n<Icon>\n<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n</Style>\n<StyleMap id=\"msn_ylw-pushpin9\">\n<Pair>\n<key>normal</key>\n<styleUrl>#sn_ylw-pushpin0</styleUrl>\n</Pair>\n<Pair>\n<key>highlight</key>\n<styleUrl>#sh_ylw-pushpin10</styleUrl>\n</Pair>\n</StyleMap>\n<Style id=\"sh_ylw-pushpin5\">\n<IconStyle>\n<scale>1.3</scale>\n");
	aux+="<Icon>\n";
					/*			<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>\n
							</Icon>\n
							<hotSpot x="20" y="2" xunits="pixels" yunits="pixels"/>\n
						</IconStyle>\n
					</Style>\n
					<StyleMap id="msn_ylw-pushpin10">\n
						<Pair>\n
							<key>normal</key>\n
							<styleUrl>#sn_ylw-pushpin5</styleUrl>\n
						</Pair>\n
						<Pair>\n
							<key>highlight</key>\n
							<styleUrl>#sh_ylw-pushpin2</styleUrl>\n
						</Pair>\n
					</StyleMap>\n
					<StyleMap id="msn_ylw-pushpin2">\n
						<Pair>\n
							<key>normal</key>\n
							<styleUrl>#sn_ylw-pushpin2</styleUrl>\n
						</Pair>\n
						<Pair>\n
							<key>highlight</key>\n
							<styleUrl>#sh_ylw-pushpin4</styleUrl>\n
						</Pair>\n
					</StyleMap>\n
					<Style id="sh_ylw-pushpin6">\n
						<IconStyle>\n
							<scale>1.3</scale>\n
							<Icon>\n
								<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>\n
							</Icon>\n
							<hotSpot x="20" y="2" xunits="pixels" yunits="pixels"/>\n
						</IconStyle>\n
						<LineStyle>\n
							<color>ffff0000</color>\n
							<width>2</width>\n
						</LineStyle>\n
						<PolyStyle>\n
							<color>ff0000aa</color>\n
							<fill>0</fill>\n
						</PolyStyle>\n
					</Style>
					<StyleMap id="msn_ylw-pushpin8">
						<Pair>
							<key>normal</key>
							<styleUrl>#sn_ylw-pushpin6</styleUrl>
						</Pair>
						<Pair>
							<key>highlight</key>
							<styleUrl>#sh_ylw-pushpin7</styleUrl>
						</Pair>
					</StyleMap>
					<Style id="sh_ylw-pushpin8">
						<IconStyle>
							<scale>1.3</scale>
							<Icon>
								<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>
							</Icon>
							<hotSpot x="20" y="2" xunits="pixels" yunits="pixels"/>
						</IconStyle>
					</Style>
					<Style id="sn_ylw-pushpin0">
						<IconStyle>
							<scale>1.1</scale>
							<Icon>
								<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>
							</Icon>
							<hotSpot x="20" y="2" xunits="pixels" yunits="pixels"/>
						</IconStyle>
					</Style>
					<Style id="sn_ylw-pushpin10">
						<IconStyle>
							<scale>1.1</scale>
							<Icon>
								<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>
							</Icon>
							<hotSpot x="20" y="2" xunits="pixels" yunits="pixels"/>
						</IconStyle>
					</Style>
					<Style id="sn_ylw-pushpin">
						<IconStyle>
							<scale>1.1</scale>
							<Icon>
								<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>
							</Icon>
							<hotSpot x="20" y="2" xunits="pixels" yunits="pixels"/>
						</IconStyle>
					</Style>
					<Style id="sn_ylw-pushpin7">
						<IconStyle>
							<scale>1.1</scale>
							<Icon>
								<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>
							</Icon>
							<hotSpot x="20" y="2" xunits="pixels" yunits="pixels"/>
						</IconStyle>
					</Style>
					<Style id="sn_ylw-pushpin1">
						<IconStyle>
							<scale>1.1</scale>
							<Icon>
								<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>
							</Icon>
							<hotSpot x="20" y="2" xunits="pixels" yunits="pixels"/>
						</IconStyle>
						<LineStyle>
							<color>ffff0000</color>
							<width>2</width>
						</LineStyle>
						<PolyStyle>
							<color>ff0000aa</color>
							<fill>0</fill>
						</PolyStyle>
					</Style>
					<Style id="sn_ylw-pushpin6">
						<IconStyle>
							<scale>1.1</scale>
							<Icon>
								<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>
							</Icon>
							<hotSpot x="20" y="2" xunits="pixels" yunits="pixels"/>
						</IconStyle>
					</Style>
					<StyleMap id="msn_ylw-pushpin3">
						<Pair>
							<key>normal</key>
							<styleUrl>#sn_ylw-pushpin8</styleUrl>
						</Pair>
						<Pair>
							<key>highlight</key>
							<styleUrl>#sh_ylw-pushpin0</styleUrl>
						</Pair>
					</StyleMap>
					<Style id="sh_ylw-pushpin0">
						<IconStyle>
							<scale>1.3</scale>
							<Icon>
								<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>
							</Icon>
							<hotSpot x="20" y="2" xunits="pixels" yunits="pixels"/>
						</IconStyle>
						<LineStyle>
							<color>ffffff00</color>
							<width>2</width>
						</LineStyle>
						<PolyStyle>
							<color>ff0000aa</color>
							<fill>0</fill>
						</PolyStyle>
					</Style>
					<Style id="sn_ylw-pushpin4">
						<IconStyle>
							<scale>1.1</scale>
							<Icon>
								<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>
							</Icon>
							<hotSpot x="20" y="2" xunits="pixels" yunits="pixels"/>
						</IconStyle>
					</Style>
					<StyleMap id="msn_ylw-pushpin7">
						<Pair>
							<key>normal</key>
							<styleUrl>#sn_ylw-pushpin9</styleUrl>
						</Pair>
						<Pair>
							<key>highlight</key>
							<styleUrl>#sh_ylw-pushpin3</styleUrl>
						</Pair>
					</StyleMap>
					<Style id="sn_ylw-pushpin3">
						<IconStyle>
							<scale>1.1</scale>
							<Icon>
								<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>
							</Icon>
							<hotSpot x="20" y="2" xunits="pixels" yunits="pixels"/>
						</IconStyle>
					</Style>
					<Folder>
						<name>rezende, rj</name>
						<open>1</open>
						<Placemark>
							<name>&lt;font color=&quot;red&quot;&gt;Você quis dizer:&lt;/font&gt; &lt;a href=&quot;http://maps.google.com/maps?q=Resende+-+Rio+de+Janeiro,+Brasil&amp;sll=-22.7865,-45.1847&amp;sspn=3.2247,5.01253&amp;ie=UTF8&amp;v=2.2&amp;cv=6.0.2.2074&amp;hl=pt-BR&amp;oi=geospell&amp;ct=clnk&amp;cd=1&amp;geocode=FUQ_qf4df6lZ_Q&amp;split=0&amp;output=kml&amp;ge_fileext=.kml&quot;&gt;Resende - RJ, Brasil&lt;/a&gt;</name>
							<visibility>0</visibility>
							<Snippet maxLines="2"><![CDATA[<br/>]]></Snippet>
							<Style>
								<ListStyle>
									<listItemType>check</listItemType>
									<ItemIcon>
										<state>open closed error fetching0 fetching1 fetching2</state>
									</ItemIcon>
									<bgColor>00ffffff</bgColor>
									<maxSnippetLines>2</maxSnippetLines>
								</ListStyle>
							</Style>
						</Placemark>
						<Folder>
							<name>&lt;div style=&quot;background-color:#FFF8DD&quot;&gt;Anúncios&lt;/div&gt;</name>
							<visibility>0</visibility>
							<open>1</open>
							<Snippet maxLines="4"><![CDATA[<html><div style="background-color:#FFF8DD"><a href="http://www.google.com/aclk?sa=L&amp;ai=C8Uh1Ctq2TcL8L4HctweQp8whw4Pu3QH7t6nEBbX15poBEAEg7tyoD1DI2OiPBWBNyAEBqQJZXaBIxWmlPqoEIU_Qtb6AAETzsi7_SrOqEPbq7rwOFyCFHLu2QraeWZrgeA&amp;num=2&amp;sig=AGiWqtxw9DVs9JzcscjkBF5UV0q3h-viIQ&amp;adurl=http://www.portaldosventos.com.br">Visconde de Mauá &lt;b&gt;RJ&lt;/b&gt;</a></div><div style="background-color:#FFF8DD;color:#008000">www.portaldosventos.com.br</div><nobr><div style="background-color:#FFF8DD">Chalés c/hidro dupla panorâmica</div><div style="background-color:#FFF8DD">vista cinematográfica/café/jantar..</div></nobr></html>]]></Snippet>
							<Style>
								<ListStyle>
									<listItemType>check</listItemType>
									<ItemIcon>
										<state>open closed error fetching0 fetching1 fetching2</state>
									</ItemIcon>
									<bgColor>00ffffff</bgColor>
									<maxSnippetLines>2</maxSnippetLines>
								</ListStyle>
							</Style>
						</Folder>*/
}
	aux << "<Folder>\n";
	aux << "<name>" << "Projeto do e-foto"<< "</name>";

	Matrix oe=getMatrixOE();
	for (int i=0;i<listAllImages.size();i++)
	{
		string icon= "http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png";
		string lineColor= "ffff0000"; //codigo hexadecimal alphaBGR
		string lineWidth= "1";
		aux<<"<Style id=\"sn_ylw-pushpin"<<intToString(i)<< "\">\n<IconStyle>\n<scale>1.3</scale>\n<Icon>\n<href>"<< icon <<"</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n" << "<LineStyle>\n<color>"<< lineColor <<"</color>\n<width>"<< lineWidth<<"</width>\n</LineStyle>\n<PolyStyle>\n<fill>0</fill>\n</PolyStyle></Style>\n";

		aux<<"<StyleMap id=\"msn_ylw-pushpin" << intToString(i) <<"\">\n<Pair>\n<key>normal</key>\n<styleUrl>#sn_ylw-pushpin"<<intToString(i)<< "</styleUrl>\n</Pair>\n</StyleMap>";


		Image *img=listAllImages.at(i);
		int width=img->getWidth();
		int height=img->getHeight();

		//mySensor->getFlight(0)->getScaleDen()

		double ert=25400*1e-6*(img->getFlight()->getScaleDen())/img->getResolution();

		double deltaE=(width/2)*ert;
		double deltaN=(height/2)*ert;
		double E1=oe.get(i+1,4)-deltaE;
		double N1=oe.get(i+1,5)+deltaN;
		double E2=oe.get(i+1,4)+deltaE;
		double N2=oe.get(i+1,5)+deltaN;

		Matrix plh1=ConvertionsSystems::utmToGeoFran(E1,N1,0,23,GeoSystem(),'S');
		Matrix plh2=ConvertionsSystems::utmToGeoFran(E2,N2,0,23,GeoSystem(),'S');

		double lat1=-22.9119;//-plh1.get(1,1)*180/M_PI;
		double longi1=-43.2406;//plh1.get(1,2)*180/M_PI;

		double lat2=-22.9213;//-plh2.get(1,1)*180/M_PI;
		double longi2=-43.2303;//plh2.get(1,2)*180/M_PI;

		stringstream coord;
		coord << doubleToString(longi1) << "," <<doubleToString(lat1) << ",0 ";
		coord << doubleToString(longi2) << "," <<doubleToString(lat1) << ",0 ";
		coord << doubleToString(longi2) << "," <<doubleToString(lat2) << ",0 ";
		coord << doubleToString(longi1) << "," <<doubleToString(lat2) << ",0 ";
		coord << doubleToString(longi1) << "," <<doubleToString(lat1) << ",0 ";
		string coordenadas=coord.str();

//-43.24428938257282,-22.92051144799519,0 -43.22905371658371,-22.92051151088507,0 -43.22897178369649,-22.90417775351901,0 -43.24347663910251,-22.90402206203603,0 -43.24428938257282,-22.92051144799519,0
		aux << "<Placemark>\n";
		aux << "<name>"<< img->getFilename() << "</name>\n";
		aux << "<styleUrl>";
		aux << "#msn_ylw-pushpin" <<intToString(i)<< "</styleUrl>\n";
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
		aux << "</Placemark>\n";
}

	//qDebug("%c\t%d\t%s",latitude,zona,sys.getSystemName().c_str());
	for(int i=0;i<listAllPoints.size();i++)
	{
		Point *pnt=listAllPoints.at(i);
		double E=pnt->getObjectCoordinate().getX();
		double N=pnt->getObjectCoordinate().getY();
		double H=pnt->getObjectCoordinate().getZ();

		Matrix plh=ConvertionsSystems::utmToGeoFran(E,N,H,zona,sys,latitude);

		double lat=plh.get(1,1)*180/M_PI;
		double longi=plh.get(1,2)*180/M_PI;

		lat = (latitude=='S' ? -lat:lat);

		aux << "<Placemark>\n";
		aux << "<name>";
		aux << pnt->getPointId();//namePoints->text();
		//aux << QString::number(i+1));
		aux << "</name>\n";
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
		aux << "<styleUrl>#msn_ylw-pushpin</styleUrl>\n";
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
	return aux.str();
}
