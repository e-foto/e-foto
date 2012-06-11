#include "PTManager.h"
#include "EFotoManager.h"
#include "PTUserInterface_Qt.h"

#include "EDom.h"
#include "Point.h"
#include "Image.h"
#include "InteriorOrientation.h"
#include "SensorWithFiducialMarks.h"
#include "Dms.h"

#include <stdlib.h>

#include "EPolygon.h"

//#include <QDebug>


#define NUNES

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

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

PTManager::PTManager(EFotoManager *newManager, deque<Image*>images, deque<InteriorOrientation*> ois,Sensor *sensor, Flight *flight, Terrain *terrain)
{
	efotoManager = newManager;
	listAllImages = images;
	listOis = ois;
	setListPoint();//lista todos os pontos
	mySensor = sensor;
	myFlight = flight;
	myTerrain= terrain;
	marksSaveState= true;
	started = false;
	status = false;
	localTopocentricMode = false;

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

Matrix PTManager::getFotoIndice(deque<Matrix*> imgs, deque<Matrix> IOs, deque<Matrix> EOs, int width, int height, Matrix& dim) //(GraphicWorkAround)
{
	// Note-se que o resultado deste mÃ©todo vai mostrar claramente a interseÃ§Ã£o entre as imagens, mas ele se aplica apenas para feedback com o usuÃ¡rio.
	// Ele nÃ£o serve para definirmos a exata sobreposiÃ§Ã£o de imagens no domÃ­nio do espaÃ§o objeto.
	// Esta Ã© uma primeira abordagem com apenas uma imagem... num futuro poderemos pensar em emitir todas as imagens, mas com as suas devidas reprojeÃ§Ãµes para o fotoindice.

	// Passo 1: Define o plano mÃ©dio do terreno como Z fixo para todo o fotoindice
    double z = efotoManager->getProject()->terrain()->getMeanAltitude();

	// Passo 2: Calcula as coordenadas x,y dos quatro cantos de todas as imagens usando o Raio Projetivo e as IOs e EOs informadas. Xmax, Xmin, Ymax e Ymin precisam ser guardados.

	// Passo 3: Computa uma IO fictÃ­cia que leva o BoundingBox definido por ((Xmin, Ymin), (Xmax, Ymax)) nos pixels da imagem definida por (width, height)

	// Passo 4: Gerar duas matrizes com width e height. A primeira vai acumular o valor radiomÃ©trico das imagens e a segunda vai acumular quantas contribuiÃ§Ãµes de cor cada pixel recebeu.
	// Aqui existe um problema tÃ©cnico:
	// Precisamos definir junto do Marcelo um padrÃ£o sobre transpor ou nÃ£o os pixels da imagem quando vertermos QImage em Matrix e isso define se width Ã© coluna ou linha e para  height se Ã© linha ou coluna.

	// Passo 5: Para cada pixel de cada imagem computar onde ele foi projetado sobre a imagem do fotoindice e acumular esse valor na imagem de acumulaÃ§Ã£o.
	// A imagem de contribuiÃ§Ãµes vai ter o pixel equivalente somado de um cada vez que um pixel de uma imagem for projetado.

	// Passo 6: Dividir cada pixel da primeira imagem gerada para o fotoindice pelo pixel correspondente na imagem de contribuiÃ§Ãµes. Ã preciso observar e evitar as divisÃµes por zero.
	// Onde a divisÃ£o por zero estiver marcada atualizem o valor a imagem final para -1. Isso significarÃ¡ pixel nulo (transparente) na imagem final a ser gerada pelo visualizador.
	// Depois da divisÃ£o de todos os pixels na primeira imagem teremos o fotoindice de nossa primeira abordagem.

	// Passo 7: Precisamos alimentar a matrix dim que receberÃ¡ o Xmin, o valor (Xmax-Xmin)/(width (ou height, que precisa ainda ser decidido)), o Ymin e o valor (Ymax-Ymin)/(height (ou width, tambÃ©m por definir))
	// Basta entÃ£o, retornarmos o fotoindice.

	// Obs.: Essa primeira abordagem deve gerar imagens com vultos nas sobreposiÃ§Ãµes e a segunda imagem, a de contruiÃ§Ãµes, nÃ£o necessÃ¡riamente revela as sobreposiÃ§Ãµes da maneira como esta feito.
	// Contudo podemos ter uma terceira e uma quarta matrizes para registrar essa sobreposiÃ§Ã£o real (sobre o domÃ­nio do espaÃ§o imagem). Nesse modo, a quarta matrix Ã© binÃ¡ria. Ou uma imagem desenhou um pixel ou nÃ£o. E ao final de cada imagem (do projeto) processada soma-se os valores da quarta matrix Ã  terceira e em seguida zera-se a quarta matrix. O resultado disto Ã© que no final o terceira matrix vai dizer quantas imagens contribuiram para cada pixel.



	// Vamos ver a abordagem 2 sÃ³ para entender o que mudaria. AtÃ© o passo 3 seria tudo igual.
	// O passo 4 seria gerar dois deques de matrizes, onde cada imagem entrada vai gerar duas matrizes uma de acumulo no primeiro deque e uma de contribuiÃ§Ãµes no segundo deque.
	// Os passos 5 e 6 vÃ£o gerar entÃ£o novas imagens que nÃ£o sÃ£o as melhores aproximaÃ§Ã´es lineares para cada imagem e sua reprojeÃ§Ã£o, mas passam perto.
	// Neste modo o passo 7 ainda Ã© identico ao anterior, pois geramos todas as camadas do fotoindice com a mesma resoluÃ§Ã£o.
	// Bastaria entÃ£o retornar o deque.
	// A melhoria desta abordagem Ã© nÃ£o misturarmos as imagens e podermos ver cada imagem do fotoindice isoladamente no visualizador. Isso consome ainda mais memÃ³ria, mas sÃ³.

	// Quanto a questÃ£o de projetarmos os pixels das imagens originais para o fotoindice e nÃ£o buscarmos os pixels do fotoindice entre todas as imagens, essa Ã© uma decisÃ£o que afeta principalmente o tempo de execuÃ§Ã£o da montagem.
	// A escolha feita Ã©, hoje, a mais rÃ¡pida, mas podemos ponderar por fazer as projeÃ§Ãµes no sentido oposto num outro momento.

	// Quanto a aproximaÃ§Ã£o linear para a radiometria dos pixels, ela pode ser feita nos dois sentidos, mas Ã© algo a ser deixado para depois.
	// No primeiro caso abordado ele seria simples de fazer: Ã© sÃ³ considerar o inverso da distÃ¢ncia dos centros (de cada pixel projetado para o centro do pixel de destino) ao realizar a acumulaÃ§Ã£o. Isso eu esclareÃ§o depois com calma.
	//Outro modo Ã© a tradicional interpolaÃ§Ã£o bilinear no caso de fazermos o trabalho no sentido do fotoindice para as imagens.

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
        //connectImagePoints();
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
	efotoManager->reloadProject();
}

/*
string PTManager::getImagefile(int imageId)
{
				string imagefile=listAllImages.at(imageId)->getFilepath();
				imagefile +="/";
				imagefile +=listAllImages.at(imageId)->getFilename();
				return imagefile;
}
*/

string PTManager::getImagefile(int imageKey)
{
    Image *img = efotoManager->getProject()->image(imageKey);
	return img->getFilename();
}


bool PTManager::calculatePT()
{
	sortPointsSelected();

	pt = new BundleAdjustment(listSelectedImages,listSelectedPoints);

	//qDebug("UTM:\n%s",pt->printAll().c_str());

if (localTopocentricMode)
	convertToNunes(listSelectedPoints,getTerrainDatum(),getTerrainLatHemisphere(),getTerrainZone());

	//qDebug("NUNES:\n%s",pt->printAll().c_str());
	//convertToUTM(listSelectedPoints,SAD69);
	//qDebug("UTM:\n%s",pt->printAll().c_str());

	pt->setMaxNumberIterations(maxIterations);
	pt->setMetricConvergencyValue(metricConvergency);
	pt->setAngularConvergencyValue(angularConvergency*M_PI/180);

	if (pt->isPossibleCalculate())
	{
		if (hasAllImagesInitialValues())
		{
			Matrix matInitialValues(listSelectedImages.size(),6);
			for (int i=0;i<listSelectedImages.size();i++)
			{
				Image *img=listSelectedImages.at(i);
				if(img->isInsAvailable() && img->getInsType()=="Initial")
				{
					matInitialValues.set(i+1,1,img->getInsOmega());
					matInitialValues.set(i+1,2,img->getInsPhi());
					matInitialValues.set(i+1,3,img->getInsKappa());
				}
				if(img->isGnssAvailable() && img->getGnssType()=="Initial")
				{
					matInitialValues.set(i+1,4,img->getGnssX0());
					matInitialValues.set(i+1,5,img->getGnssY0());
					matInitialValues.set(i+1,6,img->getGnssZ0());
				}
			}
			pt->setUserInitialValues(matInitialValues);
		}


		bool result=pt->calculate();
		setMatrixAFP(pt->getAFP());

		if (localTopocentricMode)
			convertToUTM(listSelectedPoints,getTerrainDatum());

		residuos=pt->getMatRes();

		return result;
	}
	else
		return false;
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
	if (pt==NULL)
	{
		return Matrix(0,0);
	}
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
		//EDomElement point=pointsXml.elementByTagAtt("point","key",Conversion::intToString(i));

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
	//points.show('f',4);
	ENH=points;
	spareENH=ENH;
	spareENH.putMatrix(pointKeys,1,ENH.getCols()+1);
}

Matrix PTManager::getENH()
{
	return ENH;
}

bool PTManager::connectImagePoints() // Deprecated
{
	if (!(started))
	{
		qApp->processEvents();
        EDomElement xmlPoints(efotoManager->getProject()->getXml("points"));
		deque<EDomElement> allPoints = xmlPoints.elementsByTagName("point");
		for (unsigned int j = 0; j < listAllImages.size(); j++)
		{
			//listAllImages.at(j)->clearPoints();
			for (unsigned int i = 0; i < allPoints.size(); i++)
			{
				string data = allPoints.at(i).elementByTagAtt("imageCoordinates", "image_key",Conversion::intToString(listAllImages.at(j)->getId())).getContent();
				//qDebug("%s\n",data.c_str());
				if (data != "")
				{
                    Point* pointToInsert = efotoManager->getProject()->point(Conversion::stringToInt(allPoints.at(i).attribute("key")));
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
    EDomElement xmlPoints(efotoManager->getProject()->getXml("points"));
	deque<EDomElement> allPoints = xmlPoints.elementsByTagName("point");
	for(int i=0;i<allPoints.size();i++)
	{
        Point* point= efotoManager->getProject()->point(Conversion::stringToInt(allPoints.at(i).attribute("key")));
		if (point != NULL)// && !point->getType() == Point::CHECKING)
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

string PTManager::getFilePath(int imageKey)
{
	int numImages=listAllImages.size();
	for (int i=0;i<numImages;i++)
		if(listAllImages.at(i)->getId()==imageKey)
		{
			string file = listAllImages.at(i)->getFilepath() + "/" + listAllImages.at(i)->getFilename();
			return file;
		}
	return "";
}


deque<string> PTManager::getStringTypePoints(string imageFileName)
{
	deque<string> list;
	if (imageFileName == "")
	{
		int numPnts=listAllPoints.size();
		for (int i=0; i<numPnts; i++)
		{
			if (listAllPoints.at(i) && listAllPoints.at(i)->getType() == Point::CONTROL)
				list.push_back("Control");
			if (listAllPoints.at(i) && listAllPoints.at(i)->getType() == Point::PHOTOGRAMMETRIC)
				list.push_back("Photogrammetric");
			if (listAllPoints.at(i) && listAllPoints.at(i)->getType() == Point::CHECKING)
				list.push_back("Checking");
			if (listAllPoints.at(i) && listAllPoints.at(i)->getType() == Point::UNKNOWN)
				list.push_back("Unknown");
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
					if (temp && temp->getPointAt(j) && temp->getPointAt(j)->getType() == Point::CONTROL)
						list.push_back("Control");
					if (temp && temp->getPointAt(j) && temp->getPointAt(j)->getType() == Point::PHOTOGRAMMETRIC)
						list.push_back("Photogrammetric");
					if (temp && temp->getPointAt(j) && temp->getPointAt(j)->getType() == Point::CHECKING)
						list.push_back("Checking");
					if (temp && temp->getPointAt(j) && temp->getPointAt(j)->getType() == Point::UNKNOWN)
						list.push_back("Unknown");
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
			if (listAllPoints.at(j)->getPointId()==selectedPointsList.at(i) && listAllPoints.at(j)->getType() != Point::CHECKING)
			{

				listSelectedPoints.push_back(listAllPoints.at(j));
				//qDebug("Achei IdPoint: %s",listAllPoints.at(j)->getPointId().c_str());
			}

}

BundleAdjustment* PTManager::getBundleAdjustment()
{
	return pt;
}

string PTManager::getPointId(int pointKey)
{
    return efotoManager->getProject()->point(pointKey)->getPointId();
}


// retorna uma lista com os ids dos pontos
deque<string> PTManager::getStringIdPoints(string imageFileName, string cond)
{
	deque<string> list;
	if (imageFileName =="" && cond!="")
	{
		int numPnts=listAllPoints.size();
		for (int i=0; i<numPnts; i++)
		{
			Point *pnt=listAllPoints.at(i);
			int appearances = getImagesAppearances(pnt->getId()).size();
			if (pnt->getType() == Point::PHOTOGRAMMETRIC && appearances < 2)
				continue;
			if(pnt->getType() != Point::CHECKING || (pnt->getType() == Point::CHECKING && cond!="noCheckingPoint"))
				list.push_back(pnt->getPointId());
		}
		return list;
	}else if (imageFileName =="" && cond=="")
	{
		int numPnts=listAllPoints.size();
		for (int i=0; i<numPnts; i++)
		{
			Point *pnt=listAllPoints.at(i);
			if(pnt->getType() != Point::CHECKING || (pnt->getType() == Point::CHECKING && cond!="noCheckingPoint"))
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
			list.push_back(Conversion::intToString(pnt->getId()));
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
					list.push_back(Conversion::intToString(temp->getPointAt(j)->getId()));
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
				ImageSpaceCoordinate coord=temp->getPointAt(j)->getImageCoordinate(temp->getId());
				//qDebug("%s from %s",temp->getPointAt(j)->getPointId().c_str() , imageFileName.c_str());
				result.set(j+1,1,coord.getCol());
				result.set(j+1,2,coord.getLin());
			}
			return result;
		}
	}
}

Matrix PTManager::getColLin(int imageKey)
{
	int numImages= listAllImages.size();
	//qDebug("lista de imagens %d",listAllImages.size());
	for (int i=0;i<numImages;i++)
	{
		//qDebug("Imagens %s",listAllImages.at(i)->getFilename().c_str());
		//qDebug("Antes %s",imageFileName.c_str());
		if(listAllImages.at(i)->getId() == imageKey)
		{
			//qDebug("Achou %s",imageFileName.c_str());
			Image *temp=listAllImages.at(i);
			int numpnts=temp->countPoints();
			Matrix result(numpnts,2);
			for (int j=0;j<numpnts;j++)
			{
				ImageSpaceCoordinate coord=temp->getPointAt(j)->getImageCoordinate(temp->getId());
				//qDebug("%s from %s",temp->getPointAt(j)->getPointId().c_str() , imageFileName.c_str());
				result.set(j+1,1,coord.getCol());
				result.set(j+1,2,coord.getLin());
			}
			return result;
		}
	}
}


//Faz um update das coordenadas digitais do ponto 'pointKey' na imagem 'imageKey'
void PTManager::updateDigitalCoordinatesPoint(int imageId, int pointKey, double col, double lin)
{
    Point *pnt=efotoManager->getProject()->point(pointKey);
	//	qDebug("Antes ponto %d:\n%s\n\n",pointKey,pnt->xmlGetData().c_str());
	pnt->deleteImageCoordinate(imageId);
	ImageSpaceCoordinate temp = ImageSpaceCoordinate(imageId,col,lin);
	pnt->putImageCoordinate(temp);
	//qDebug("Depois ponto %d:\n%s\n\n",pointKey,pnt->xmlGetData().c_str());

	//	qDebug("updateDigitalCoordinates:%dx%d %s->col: %d lin: %d",col,lin,pnt->getPointId().c_str(),pnt->getDigitalCoordinate(imageId).getCol(),pnt->getDigitalCoordinate(imageId).getLin());
}


bool PTManager::isAvailablePoint(int imageId, int pointKey)
{
	//	Point *pnt=efotoManager->instancePoint(pointKey);
    return efotoManager->getProject()->point(pointKey)->getImageCoordinate(imageId).isAvailable();
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
		if (pnt->getType() == Point::CONTROL)
			listCtrl.push_front(pnt);
		else if (pnt->getType() == Point::PHOTOGRAMMETRIC)
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
		if (pnt->getType() == Point::PHOTOGRAMMETRIC)
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
		if (pnt->getType() == Point::PHOTOGRAMMETRIC)
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
		if (pnt->getType() == Point::PHOTOGRAMMETRIC)
			selected.push_back(pnt->getPointId());
	}
	return selected;
}

void PTManager::saveResults()
{
	saveBundleAdjustment();
    saveImages();
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
    Point *pnt=efotoManager->getProject()->point(pointKey);
	int numImages=pnt->countImages();
	for(int i=0;i<numImages;i++)
		appearance.push_back(pnt->getImageAt(i)->getFilename());
	return appearance;
}

void PTManager::saveMarks()
{
    //qDebug("Chamado metodo para salvar as marcas");
    // Rever o uso deste método, pois as marcas já estão sendo atualizadas por default.
    // Pode-se prever um rollback ou mesmo fazer um modo especializado para realizar a permanencia só das marcas.
    /*
    string points="<points>\n";
    for (int i=0; i<listAllPoints.size(); i++)
        points += listAllPoints.at(i)->xmlGetData().c_str();
    points+="</points>\n";

    EDomElement newXml(efotoManager->xmlGetData());
    newXml.replaceChildByTagName("points",points);
    efotoManager->xmlSetData(newXml.getContent());
    */
    //qDebug("NEWXML:\n%s",newXml.elementByTagName("points").getContent().c_str());
}

void PTManager::saveImages()
{
    // Rever o uso deste método, pois as imagens já estão sendo atualizadas por default.
    // Pode-se prever um rollback ou mesmo fazer um modo especializado para realizar a permanencia só das imagens.
    /*
    string images="<images>\n";
    for (int i=0; i<listAllImages.size(); i++)
        images += listAllImages.at(i)->xmlGetData().c_str();
    images+="</images>\n";

    EDomElement newXml(efotoManager->xmlGetData());
    newXml.replaceChildByTagName("images",images);
    efotoManager->xmlSetData(newXml.getContent());
    */
}

void PTManager::saveBundleAdjustment()
{
    //Rever, pois a seção phototriangulation está hoje numa parte inacessível da classe Project.
    /*
	EDomElement newExteriorOrientationXml(createOESXml());
    EDomElement oldExteriorOrientationXml(efotoManager->getProject()->getXml("exteriorOrientation"));
	deque<EDomElement> oldEos = oldExteriorOrientationXml.elementsByTagName("imageEO");

	EDomElement oldEpp(efotoManager->xmlGetData());
	if (oldEpp.hasTagName("phototriangulation"))
		oldEpp.replaceChildByTagName("phototriangulation",getPhotoTriXml());
	else
		oldEpp.addChildAtTagName("efotoPhotogrammetricProject",getPhotoTriXml());

	EDomElement temp;
	for (int i=0; i<oldEos.size() ;i++)
	{
		temp.setContent(oldEos.at(i).getContent());
		string imageKey = temp.attribute("image_key");
		if (newExteriorOrientationXml.elementByTagAtt("imageEO","image_key",imageKey).getContent()==""  && temp.attribute("type")=="spatialRessection")
			newExteriorOrientationXml.addChildAtTagName("exteriorOrientation",temp.getContent());
	}

	oldEpp.replaceChildByTagName("exteriorOrientation",newExteriorOrientationXml.getContent());
	efotoManager->xmlSetData(oldEpp.getContent());
*/
}

string PTManager::createOESXml()
{
	stringstream fotoTriXml;
	//codigo de criacao da xml da bundle(multiplas tags de Orientacao Exterior)
	//Matrix oe=pt->getAFP();
	Matrix oe=AFP;
	fotoTriXml << "<exteriorOrientation>\n";
	for (int i=1;i<=oe.getRows();i++)
	{
		Image *img = listSelectedImages.at(i-1);
		fotoTriXml << "\t<imageEO type=\"photoTriangulation\" image_key=\"" << Conversion::intToString(img->getId()) << "\">\n";
		//fotoTriXml << "\t<imageEO type=\"spatialRessection\" image_key=\"" << Conversion::intToString(i) << "\">\n";
		//fotoTriXml << "\t\t<parameters>\n";
		//fotoTriXml << "\t\t\t<Lb>\n";
		//fotoTriXml << lb << "\n</Lb>\n";
		fotoTriXml << "\t\t\t<Xa>\n";
		fotoTriXml << "\t\t\t\t<X0 uom=\"#m\">"<< Conversion::doubleToString(oe.get(i,4)) << "</X0>\n";
		fotoTriXml << "\t\t\t\t<Y0 uom=\"#m\">"<< Conversion::doubleToString(oe.get(i,5)) << "</Y0>\n";
		fotoTriXml << "\t\t\t\t<Z0 uom=\"#m\">"<< Conversion::doubleToString(oe.get(i,6)) << "</Z0>\n";
		fotoTriXml << "\t\t\t\t<phi uom=\"#rad\">"<< Conversion::doubleToString(oe.get(i,2)*M_PI/180) << "</phi>\n";
		fotoTriXml << "\t\t\t\t<omega uom=\"#rad\">"<< Conversion::doubleToString(oe.get(i,1)*M_PI/180) << "</omega>\n";
		fotoTriXml << "\t\t\t\t<kappa uom=\"#rad\">"<< Conversion::doubleToString(oe.get(i,3)*M_PI/180) << "</kappa>\n";
		fotoTriXml << "\t\t\t</Xa>\n";
		//fotoTriXml << "\t\t\t<L0>\n";
		//fotoTriXml << l0 << "\n</L0>\n";
		//fotoTriXml << "\t\t</parameters>\n";
		fotoTriXml << "</imageEO>\n";
	}

	fotoTriXml << "</exteriorOrientation>\n";

	//qDebug("Metodo chamado create varias OEs");
	return fotoTriXml.str();
}

string PTManager::getPhotoTriXml()
{
	stringstream fotoTriXml;
	fotoTriXml << "<phototriangulation>\n";

	fotoTriXml << "\t<iterations>"<< Conversion::intToString(pt->getTotalIterations()) <<"</iterations>\n";
	fotoTriXml << "\t<converged>"<< (pt->isConverged()? "true":"false")<<"</converged>\n";
	fotoTriXml << "\t<metricConvergency>"<< pt->getMetricConvergencyValue()<<"</metricConvergency>\n";
	fotoTriXml << "\t<rmse>" << pt->calculateRMSE() << "</rmse>\n";
	fotoTriXml << "\t<angularConvergency>"<< pt->getAngularConvergencyValue()<<"</angularConvergency>\n";

	fotoTriXml << getUsedPointsXml() << getUsedImagesXml();

	fotoTriXml << "</phototriangulation>\n";

	return fotoTriXml.str();

}

string PTManager::getUsedPointsXml()
{
	stringstream usedPoints;
	usedPoints << "\t<usedPoints>\n";
	for (int i=0;i<listSelectedPoints.size();i++)
	{
		Point *pnt=listSelectedPoints.at(i);
		usedPoints << "\t\t<pointKey>" <<	pnt->getId()	<<"</pointKey>\n";
	}
	usedPoints << "\t</usedPoints>\n";
	return usedPoints.str();
}

string PTManager::getUsedImagesXml()
{
	stringstream usedImages;
	usedImages << "\t<usedImages>\n";
	for (int i=0;i<listSelectedImages.size();i++)
	{
		Image *img=listSelectedImages.at(i);
		usedImages << "\t\t<imageKey>" <<	img->getId()	<<"</imageKey>\n";
	}
	usedImages << "\t</usedImages>\n";
	return usedImages.str();
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
	//Lo Lb Iteracoes convergencia sÃ£o comuns a todas as OES.
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
			//qDebug("File %s kappa0 = %.7f",imageFile.c_str(),flightDirection);
		}
}

void PTManager::setImageFlightDirection(int imageKey, double flightDirection)
{
    Image *img =efotoManager->getProject()->image(imageKey);
	if (img!=NULL)
		img->setFlightDirection(flightDirection);
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

double PTManager::getImageFlightDirection(int imageKey)
{
    Image *img =efotoManager->getProject()->image(imageKey);
	if (img!=NULL)
		return img->getFlightDirection();

	return 3*M_PI;
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
	QList<EPolygon *> epolygons;

	int hemiLatitude = getTerrainLatHemisphere();
	int zona=getTerrainZone();
	GeoSystem sys=getTerrainDatum();



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
	aux<<"<StyleMap id=\"checkingPoint\">\n<Pair>\n<key>normal</key>\n<styleUrl>#sn_CheckingPoint</styleUrl>\n</Pair>\n<Pair>\n<key>highlight</key>\n<styleUrl>#sh_CheckingPoint</styleUrl>\n</Pair>\n</StyleMap>\n";
	aux<<"<Style id=\"sn_CheckingPoint\">\n<IconStyle>\n<color>"<<colorNormalChecking<<"</color>\n<scale>1.0</scale>\n<Icon>\n<href>"<< checkingPointIcon <<"</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n</Style>\n";
	aux<<"<Style id=\"sh_CheckingPoint\">\n<IconStyle>\n<color>"<<colorHighLightChecking<<"</color>\n<scale>1.1</scale>\n<Icon>\n<href>"<< checkingPointIcon <<"</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n</Style>\n";

	Matrix oe=getMatrixOE();
	//oe.show();
	if (oe.getCols()>1 )
	{
		int B=0;
		int G=0;
		int R=0;
		for (int i=0;i<listAllImages.size();i++)
		{
			string icon= "http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png";
			switch (i%3)
			{
				case 0:	B=rand()%15;
				break;
				case 1: G=rand()%15;
				break;
				case 2: R=rand()%15;
				break;
				default :	B=rand()%15;
					G=rand()%15;
					R=rand()%15;
				break;
			}

			stringstream aux1;

			aux1 << hex << "ff"<< B << G << R << B << G << R;


			string lineColor =aux1.str();
			//qDebug("B: %d\tG: %d\tR: %d string: %s",B,G,R,lineColor.c_str());
			//string lineColor= "ffff0000"; //codigo hexadecimal alphaBGR

			string lineWidth= "2";
			aux<<"<StyleMap id=\"msn_ylw-pushpin" << Conversion::intToString(i) <<"\">\n<Pair>\n<key>normal</key>\n<styleUrl>#sn_ylw-pushpin"<<Conversion::intToString(i)<< "</styleUrl>\n</Pair>\n<Pair>\n<key>highlight</key>\n<styleUrl>#sh_ylw-pushpin"<<Conversion::intToString(i)<< "</styleUrl>\n</Pair>\n</StyleMap>\n";
			aux<<"<Style id=\"sn_ylw-pushpin"<<Conversion::intToString(i)<< "\">\n<IconStyle>\n<scale>1.3</scale>\n<Icon>\n<href>"<< icon <<"</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n" << "<LineStyle>\n<color>"<< lineColor <<"</color>\n<width>"<< lineWidth<<"</width>\n</LineStyle>\n<PolyStyle>\n<fill>0</fill>\n</PolyStyle>\n</Style>\n";
			aux<<"<Style id=\"sh_ylw-pushpin"<<Conversion::intToString(i)<< "\">\n<IconStyle>\n<scale>1.3</scale>\n<Icon>\n<href>"<< icon <<"</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n" << "<LineStyle>\n<color>"<< lineColor <<"</color>\n<width>"<< lineWidth<<"</width>\n</LineStyle>\n<PolyStyle>\n<fill>0</fill>\n</PolyStyle>\n</Style>\n";

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


			/*
			QList<QPointF *> pnts;
			pnts << new QPointF(E1,N1) <<  new QPointF(E2,N1) <<  new QPointF(E2,N2) << new QPointF(E1,N2);

			EPolygon *polygon = new EPolygon(pnts);
			polygon->setLabel(QString::fromStdString(img->getFilename()));
			epolygons << polygon;

			pnts.clear();*/
			Matrix temp1= ConvertionsSystems::utmToGeo(E1,N1,zona,hemiLatitude,sys);
			Matrix temp2= ConvertionsSystems::utmToGeo(E2,N2,zona,hemiLatitude,sys);


			double lat1,longi1,lat2,longi2;
			Matrix plh1,plh2;

			qDebug("old system: %s",sys.getSystemName().c_str());
			if (sys.getSystemName()!="WGS84")
			{
				double oldPhi1=temp1.get(1,1);
				double oldLambda1=temp1.get(1,2);
				double oldPhi2=temp2.get(1,1);
				double oldLambda2=temp2.get(1,2);

				plh1=ConvertionsSystems::convertSystemsSimplifiedMolodensky(sys,WGS84,oldPhi1,oldLambda1,0);
				plh2=ConvertionsSystems::convertSystemsSimplifiedMolodensky(sys,WGS84,oldPhi2,oldLambda2,0);

				lat1 = plh1.get(1,1)*180/M_PI;
				lat1 = (hemiLatitude =='S'? -lat1 : lat1);
				longi1 = plh1.get(1,2)*180/M_PI;

				lat2 = plh2.get(1,1)*180/M_PI;
				lat2 = (hemiLatitude =='S'? -lat2 : lat2);
				longi2 =plh2.get(1,2)*180/M_PI;
			}
			else
			{
				plh1=temp1;
				plh2=temp2;

				lat1 = plh1.get(1,1)*180/M_PI;
				lat1 = (hemiLatitude =='S'? -lat1 : lat1);
				longi1 = plh1.get(1,2)*180/M_PI;

				lat2 = plh2.get(1,1)*180/M_PI;
				lat2 = (hemiLatitude =='S'? -lat2 : lat2);
				longi2 =plh2.get(1,2)*180/M_PI;
			}

			stringstream coord;
			coord << Conversion::doubleToString(longi1) << "," <<Conversion::doubleToString(lat1) << ",0 ";
			coord << Conversion::doubleToString(longi2) << "," <<Conversion::doubleToString(lat1) << ",0 ";
			coord << Conversion::doubleToString(longi2) << "," <<Conversion::doubleToString(lat2) << ",0 ";
			coord << Conversion::doubleToString(longi1) << "," <<Conversion::doubleToString(lat2) << ",0 ";
			coord << Conversion::doubleToString(longi1) << "," <<Conversion::doubleToString(lat1) << ",0 ";
			string coordenadas=coord.str();

			aux << "<Placemark>\n";
			aux << "<name>"<< img->getFilename() << "</name>\n";
			aux << "<styleUrl>";
			aux << "#msn_ylw-pushpin" <<Conversion::intToString(i)<< "</styleUrl>\n";
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
	}

	/*
				double interX=0.0;
				double interY=0.0;
				for( int i=0; i<epolygons.size()-1;i++)
				{
								EPolygon *temp1=epolygons.at(i);
								for (int j=i+1; j<epolygons.size();j++)
								{
												EPolygon *temp2=epolygons.at(j);
												if (temp1->isIntercepted(*temp2,interX,interY))
												{
																qDebug("%s intercepts %s in %.3f  %.3f",temp1->getLabel().toStdString().c_str(),temp2->getLabel().toStdString().c_str(),interX,interY);
																//qDebug()<< temp1->getLabel()<< " intercepts "<< temp2->getLabel() << " in x = " << inter;
												}
								}
				}
*/
	string controlPoint="";
	string photogrammetricPoint="";
	string checkingPoint="";

	//qDebug("%c\t%d\t%s",latitude,zona,sys.getSystemName().c_str());
	for(int i=0;i<listAllPoints.size();i++)
	{
		Point *pnt=listAllPoints.at(i);
		//string pointType;
		if (pnt->getType() == Point::PHOTOGRAMMETRIC)
			photogrammetricPoint += pointToKml(pnt,zona,hemiLatitude,sys,"photogrammetricPoint");
		else if (pnt->getType() == Point::CONTROL)
			controlPoint += pointToKml(pnt,zona,hemiLatitude,sys,"controlPoint");
		else if (pnt->getType() == Point::CHECKING)
			checkingPoint += pointToKml(pnt,zona,hemiLatitude,sys, "checkingPoint");
	}

	aux << "<Folder>\n";
    aux << "<name>" << EDomElement(efotoManager->getProject()->getXml("projectHeader")).elementByTagName("name").toString() << "</name>\n";

	aux << "<Folder>\n";
	aux << "<name>" << "Control Points"<< "</name>\n";
	aux << controlPoint;
	aux << "</Folder>\n";

	aux << "<Folder>\n";
	aux << "<name>" << "Photogrammetric Points"<< "</name>\n";
	aux << photogrammetricPoint;
	aux << "</Folder>\n";

	aux << "<Folder>\n";
	aux << "<name>" << "Checking Points"<< "</name>\n";
	aux << checkingPoint;
	aux << "</Folder>\n";


	aux << "</Folder>\n";
	aux << "</Document>\n</kml>";

	EDomElement xmlgoogle(aux.str());
	return xmlgoogle.indent('\t').getContent();

}

//string PTManager::pointToKml(Point *pnt, int zona,GeoSystem sys ,char hemiLatitude,string pointType)
string PTManager::pointToKml(Point *pnt, int zona,int hemiLatitude ,GeoSystem sys,string pointType)
{
	stringstream pointString;
	double E=pnt->getObjectCoordinate().getX();
	double N=pnt->getObjectCoordinate().getY();
	double H=pnt->getObjectCoordinate().getZ();

	Matrix temp=ConvertionsSystems::utmToGeo(E,N,zona,hemiLatitude,sys);
	double lat,longi;
	Matrix plh;

	if (sys.getSystemName()!="WGS84")
	{
		double oldPhi=temp.get(1,1);
		double oldLambda=temp.get(1,2);
		plh=ConvertionsSystems::convertSystemsSimplifiedMolodensky(sys,WGS84,oldPhi,oldLambda,H);
		lat = plh.get(1,1)*180/M_PI;
		lat = (hemiLatitude =='S'? -lat : lat);
		longi = plh.get(1,2)*180/M_PI;
	}
	else
	{
		plh=temp;
		lat = plh.get(1,1)*180/M_PI;
		lat = (hemiLatitude =='S'? -lat : lat);
		longi = plh.get(1,2)*180/M_PI;
	}
	/*
				double lat=plh.get(1,1)*180/M_PI;
				double longi=plh.get(1,2)*180/M_PI;*/
	H=plh.get(1,3);

	//lat = (hemiLatitude=='S' ? -lat:lat);

	pointString << "<Placemark>\n";
	pointString << "<name>" << pnt->getPointId() << "</name>\n";
	pointString << "<description>" << pnt->getDescription() << "</description>\n";
	pointString << "<LookAt>\n";
	pointString << "<longitude>";
	pointString << Conversion::doubleToString(longi);
	pointString << "</longitude>\n";
	pointString <<  "<latitude>";
	pointString << Conversion::doubleToString(lat);
	pointString << "</latitude>\n";
	pointString << "<altitude>";
	pointString << Conversion::doubleToString(H)<<"</altitude>\n";
	pointString << "<heading>0.01795058696543714</heading>\n";
	pointString << "<tilt>0</tilt>\n";
	pointString << "<range>1463.920597456832</range>\n";
	pointString << "<altitudeMode>relativeToGround</altitudeMode>\n";
	pointString << "<gx:altitudeMode>relativeToSeaFloor</gx:altitudeMode>\n";
	pointString << "</LookAt>\n";
	pointString << "<styleUrl>#" << pointType <<"</styleUrl>\n";
	pointString << "<Point>\n";
	pointString << "<altitudeMode>clampToGround</altitudeMode>\n";
	pointString << "<gx:altitudeMode>clampToSeaFloor</gx:altitudeMode>\n";
	pointString << "<coordinates>";
	pointString << Conversion::doubleToString(longi);
	pointString << ",";
	pointString << Conversion::doubleToString(lat);
	pointString << ",";
	pointString << Conversion::doubleToString(H) <<"</coordinates>\n";
	pointString << "</Point>\n";
	pointString << "</Placemark>\n";

	return pointString.str();

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
        Point *pnt=efotoManager->getProject()->point(spareENH.getInt(i+1,4)); // rever Aqui eu posso precisar escrever código que de roolback nos pontos

		double x=spareENH.get(i+1,1);
		double y=spareENH.get(i+1,2);
		double z=spareENH.get(i+1,3);

		pnt->getObjectCoordinate().setX(x);
		pnt->getObjectCoordinate().setY(y);
		pnt->getObjectCoordinate().setZ(z);
	}

}

// Retorna a posiÃ§ao de 'img' no deque de imagens selecionadas, se nao estiver retorna -1
int PTManager::whereInSelectedImages(Image *img)
{
	for (int i=0;i<listSelectedImages.size();i++)
	{
		if (listSelectedImages.at(i)==img)
			return i;
	}
	return -1;
}

// Retorna a posiÃ§ao de 'img' no deque de imagens, se nao estiver retorna -1
int PTManager::whereInImages(Image *img)
{
	for (int i=0;i<listAllImages.size();i++)
	{
		if (listAllImages.at(i)==img)
			return i;
	}
	return -1;
}

deque<string> PTManager::getPointsWithLesserThanOverlap(int overlap)
{
	deque<string> ids;
	int numPoints=listAllPoints.size();
	for (int i=0;i<numPoints;i++)
	{
		Point * pnt=listAllPoints.at(i);
		if(pnt->getType() == Point::PHOTOGRAMMETRIC)
			if (getImagesAppearances(pnt->getId()).size() < overlap)
				ids.push_back(pnt->getPointId());
	}
	return ids;
}

int PTManager::createNewPoint()
{
	string text = "";

    int currentItemId = efotoManager->getProject()->getFreePointId();
	text += "<point key=\"" + Conversion::intToString(currentItemId) + "\" type=\"photogrammetric\">\n";
	text += "<pointId>PF" + Conversion::intToString(currentItemId) + "</pointId>\n";
	text += "<description>Photogrammetric point created in Phototriangulation</description>\n";
	text += "<spatialCoordinates uom=\"#m\">\n";
	text += "<gml:pos>0.0 0.0 0.0</gml:pos>\n";
	text += "<sigma>Not Available</sigma>\n";
	text += "</spatialCoordinates>\n";
	text += "</point>";

    efotoManager->getProject()->addPoint(text);
    listAllPoints.push_back(efotoManager->getProject()->point(currentItemId));

	return currentItemId;

}
void PTManager::connectPointInImage(int pointKey, int imageKey)
{
    Point* point = efotoManager->getProject()->point(pointKey);
    Image* image = efotoManager->getProject()->image(imageKey);
	point->putImage(image);
	image->putPoint(point);
	image->sortPoints();

}

bool PTManager::allKappaSet()
{
	int nImages= listAllImages.size();
	for (int i=0;i<nImages;i++)
	{
		Image *img=listAllImages.at(i);
		if (img->getFlightDirection()>=2.5*M_PI)
			return false;
	}
	return true;
}

deque<string> PTManager::getImagesKappaSet()
{
	deque<string> list;
	int nImages= listAllImages.size();
	for (int i=0;i<nImages;i++)
	{
		Image *img=listAllImages.at(i);
		if (img->getFlightDirection()<2.5*M_PI && img->getFlightDirection()>=0)
			list.push_back(img->getFilename());
	}
	return list;
}



Matrix PTManager::getDigitalCoordinate(int imageKey, int pointKey)
{
	Matrix result(1,2);
    Point *pnt=efotoManager->getProject()->point(pointKey);
	ImageSpaceCoordinate coord=pnt->getImageCoordinate(imageKey);
	result.set(1,1,coord.getCol());
	result.set(1,2,coord.getLin());
	return result;
}

/** Em teste de sort dos pontos fotogrametricos segundo Francisco,TFC.
*/
/*
void PTManager::photogrammetricSort()
{
 pt->calculateInicialsValues();
 Matrix oe=pt->getMatrixInicialValues();
 for (int i=0;i<listAllPoints.size();i++)
 {
  Point *pnt=listAllPoints.at(i);
  Image *img=pnt->getImageAt(0);
  ImageSpaceCoordinate coord=pnt->getDigitalCoordinate(img->getId());
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
/*
 Matrix result(1,2);
 result.set(1,1,E1);
 result.set(1,2,N1);
 //qDebug("imagem 1 ponto 1, col: %d \tlin:%d",col,row);
 result.show('f',4,"Matrix EN");
 return result;
}
*/

bool PTManager::hasAllImagesInitialValues()
{
	for (int i=0;i<listAllImages.size();i++)
	{
		Image *img=listAllImages.at(i);
		if(!img->isInsAvailable())
			return false;
		if(!img->isGnssAvailable())
			return false;
		if(img->getInsType()!="Initial")
			return false;
		if(img->getGnssType()!="Initial")
			return false;
	}
	return true;
}

double PTManager::getPhiTerrain()
{
	Dms lat(myTerrain->getCentralCoordLat());
    EDomElement coordinates(efotoManager->getProject()->getXml("terrain"));
	coordinates=coordinates.elementByTagName("Lat");
	if (getTerrainLatHemisphere()<0)
		lat.setSignal(true);

	//printf("phi0 : %s\tDMS: %s\n",myterrain->getCentralCoordLat().c_str(),lat.toString(5).c_str());
	return lat.dmsToRadiano();
}

double PTManager::getLambdaTerrain()
{
	Dms longi(myTerrain->getCentralCoordLong());
	//printf("long0 : %s\tDMS: %s\n\n",myterrain->getCentralCoordLong().c_str(),longi.toString(5).c_str());
    EDomElement coordinates(efotoManager->getProject()->getXml("terrain"));
	coordinates=coordinates.elementByTagName("Long");
	if (getTerrainLongHemisphere()<0)//coordinates.attribute("direction")=="W")
		longi.setSignal(true);

	return longi.dmsToRadiano();
}

double PTManager::getAltitudeMedia()
{
	return myTerrain->getMeanAltitude();
}

void PTManager::convertToNunes(deque<Point*> points, GeoSystem sys, int hemi, int zona)
{
	int rows=points.size();
	for (int i=0;i<rows;i++)
	{      //N0=-5.85
		Point *pnt=points.at(i);
		ObjectSpaceCoordinate temp = pnt->getObjectCoordinate();
		double E = temp.getX();
		double N = temp.getY();
		double H = temp.getZ();
		//qDebug("phi %.5f\tlambda%.5f",getPhiTerrain(),getLambdaTerrain());
		Matrix gc=ConvertionsSystems::utmToNunes(E,N,H,zona,hemi,getPhiTerrain(),getLambdaTerrain(),getAltitudeMedia(),sys);
		temp.setX(gc.get(1,1));
		temp.setY(gc.get(1,2));
		temp.setZ(gc.get(1,3));
		pnt->setObjectCoordinate(temp);
	}
}

void PTManager::convertToUTM(deque<Point*> points, GeoSystem sys)
{
	int rows=points.size();

	//double R=ConvertionsSystems::getNunesRaio(getPhiTerrain(),getLambdaTerrain(),sys);
	double phi0=getPhiTerrain();
	double lambda0=getLambdaTerrain();
	double h0=getAltitudeMedia();

	for (int i=0;i<rows;i++)
	{
		Point *pnt=points.at(i);
		ObjectSpaceCoordinate temp = pnt->getObjectCoordinate();
		double X = temp.getX();
		double Y = temp.getY();
		double Z = temp.getZ();

		Matrix gc=ConvertionsSystems::nunesToUtm(X,Y,Z,phi0,lambda0,h0,sys);
		temp.setX(gc.get(1,1));
		temp.setY(gc.get(1,2));
		temp.setZ(gc.get(1,3));
		pnt->setObjectCoordinate(temp);
	}

	// convertendo os parametros da OE
	//Matrix oes=getMatrixOE();

	for (int i=1; i<=AFP.getRows(); i++)
	{
		double X0 = AFP.get(i,4);
		double Y0 = AFP.get(i,5);
		double Z0 = AFP.get(i,6);
		Matrix temp2 = ConvertionsSystems::nunesToUtm(X0,Y0,Z0,phi0,lambda0,h0,sys);
		AFP.set(i,4,temp2.get(1,1));
		AFP.set(i,5,temp2.get(1,2));
		AFP.set(i,6,temp2.get(1,3));
	}

}

bool PTManager::hasEODone()
{
    EDomElement exteriorXml(efotoManager->getProject()->getXml("exteriorOrientation"));
	if(exteriorXml.hasTagName("imageEO"))
		return true;
	return false;
}

Matrix PTManager::eoParametersFromXml()
{
	EDomElement tempXa;
    EDomElement exteriorXml(efotoManager->getProject()->getXml("exteriorOrientation"));
	deque<EDomElement> oesXml=exteriorXml.elementsByTagName("imageEO");

	int imagekey;
	double x0,y0,z0,omega,phi,kappa;
	int numEO = oesXml.size();
	Matrix oesMatrix(numEO,7);

	for (int i=0; i<numEO; i++)
	{
		//temp.setContent(oesXml.at(i).getContent());
		imagekey=Conversion::stringToInt(oesXml.at(i).attribute("image_key"));
		tempXa.setContent(oesXml.at(i).elementByTagName("Xa").getContent());
		x0=Conversion::stringToDouble(tempXa.elementByTagName("X0").toString());
		y0=Conversion::stringToDouble(tempXa.elementByTagName("Y0").toString());
		z0=Conversion::stringToDouble(tempXa.elementByTagName("Z0").toString());
		omega=Conversion::stringToDouble(tempXa.elementByTagName("omega").toString())*180/M_PI;
		phi=Conversion::stringToDouble(tempXa.elementByTagName("phi").toString())*180/M_PI;
		kappa=Conversion::stringToDouble(tempXa.elementByTagName("kappa").toString())*180/M_PI;

		oesMatrix.set(i+1,1,imagekey);
		oesMatrix.set(i+1,2,omega);
		oesMatrix.set(i+1,3,phi);
		oesMatrix.set(i+1,4,kappa);
		oesMatrix.set(i+1,5,x0);
		oesMatrix.set(i+1,6,y0);
		oesMatrix.set(i+1,7,z0);
	}
	return oesMatrix;
}

double PTManager::getRMSE()
{
	return pt->calculateRMSE();
}

int PTManager::getPreviousTotalIterationsXml()
{
    EDomElement exteriorXml(efotoManager->getProject()->getXml("phototriangulation"));
	return Conversion::stringToInt(exteriorXml.elementByTagName("iterations").toString());
}

bool PTManager::getPreviousConvergedXML()
{
    EDomElement exteriorXml(efotoManager->getProject()->getXml("phototriangulation"));
	if (exteriorXml.elementByTagName("converged").toString()=="true")
		return true;
	return false;
}

double PTManager::getPreviousRmseXML()
{
    EDomElement exteriorXml(efotoManager->getProject()->getXml("phototriangulation"));
	return exteriorXml.elementByTagName("rmse").toDouble();
}

int PTManager::getTerrainLatHemisphere()
{
    EDomElement terrain = efotoManager->getProject()->getXml("terrain");
	int hemi=terrain.elementByTagName("Lat").attribute("direction")=="S" ? -1:1;
	return hemi;
}

int PTManager::getTerrainLongHemisphere()
{
    EDomElement terrain = efotoManager->getProject()->getXml("terrain");
	int hemi=terrain.elementByTagName("Long").attribute("direction")=="W" ? -1:1;
	return hemi;
}

int PTManager::getTerrainZone()
{
	return myTerrain->getUtmFuse();
}

GeoSystem PTManager::getTerrainDatum()
{
	return GeoSystem(myTerrain->getGRS());
}

void PTManager::setLocalTopocentricMode(bool status)
{
	localTopocentricMode = status;
}

bool PTManager::getLocalTopocentricMode()
{
	return localTopocentricMode;
}
} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
