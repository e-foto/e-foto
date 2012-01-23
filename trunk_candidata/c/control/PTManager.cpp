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

Matrix PTManager::getFotoIndice(deque<Matrix*> imgs, deque<Matrix> IOs, deque<Matrix> EOs, int width, int height, Matrix& dim) //(GraphicWorkAround)
{
	// Note-se que o resultado deste método vai mostrar claramente a interseção entre as imagens, mas ele se aplica apenas para feedback com o usuário.
	// Ele não serve para definirmos a exata sobreposição de imagens no domínio do espaço objeto.
	// Esta é uma primeira abordagem com apenas uma imagem... num futuro poderemos pensar em emitir todas as imagens, mas com as suas devidas reprojeções para o fotoindice.

	// Passo 1: Define o plano médio do terreno como Z fixo para todo o fotoindice
	double z = efotoManager->instanceTerrain()->getMeanAltitude();

	// Passo 2: Calcula as coordenadas x,y dos quatro cantos de todas as imagens usando o Raio Projetivo e as IOs e EOs informadas. Xmax, Xmin, Ymax e Ymin precisam ser guardados.

	// Passo 3: Computa uma IO fictícia que leva o BoundingBox definido por ((Xmin, Ymin), (Xmax, Ymax)) nos pixels da imagem definida por (width, height)

	// Passo 4: Gerar duas matrizes com width e height. A primeira vai acumular o valor radiométrico das imagens e a segunda vai acumular quantas contribuições de cor cada pixel recebeu.
	// Aqui existe um problema técnico:
	// Precisamos definir junto do Marcelo um padrão sobre transpor ou não os pixels da imagem quando vertermos QImage em Matrix e isso define se width é coluna ou linha e para  height se é linha ou coluna.

	// Passo 5: Para cada pixel de cada imagem computar onde ele foi projetado sobre a imagem do fotoindice e acumular esse valor na imagem de acumulação.
	// A imagem de contribuições vai ter o pixel equivalente somado de um cada vez que um pixel de uma imagem for projetado.

	// Passo 6: Dividir cada pixel da primeira imagem gerada para o fotoindice pelo pixel correspondente na imagem de contribuições. É preciso observar e evitar as divisões por zero.
	// Onde a divisão por zero estiver marcada atualizem o valor a imagem final para -1. Isso significará pixel nulo (transparente) na imagem final a ser gerada pelo visualizador.
	// Depois da divisão de todos os pixels na primeira imagem teremos o fotoindice de nossa primeira abordagem.

	// Passo 7: Precisamos alimentar a matrix dim que receberá o Xmin, o valor (Xmax-Xmin)/(width (ou height, que precisa ainda ser decidido)), o Ymin e o valor (Ymax-Ymin)/(height (ou width, também por definir))
	// Basta então, retornarmos o fotoindice.

	// Obs.: Essa primeira abordagem deve gerar imagens com vultos nas sobreposições e a segunda imagem, a de contruições, não necessáriamente revela as sobreposições da maneira como esta feito.
	// Contudo podemos ter uma terceira e uma quarta matrizes para registrar essa sobreposição real (sobre o domínio do espaço imagem). Nesse modo, a quarta matrix é binária. Ou uma imagem desenhou um pixel ou não. E ao final de cada imagem (do projeto) processada soma-se os valores da quarta matrix à terceira e em seguida zera-se a quarta matrix. O resultado disto é que no final o terceira matrix vai dizer quantas imagens contribuiram para cada pixel.



	// Vamos ver a abordagem 2 só para entender o que mudaria. Até o passo 3 seria tudo igual.
	// O passo 4 seria gerar dois deques de matrizes, onde cada imagem entrada vai gerar duas matrizes uma de acumulo no primeiro deque e uma de contribuições no segundo deque.
	// Os passos 5 e 6 vão gerar então novas imagens que não são as melhores aproximaçôes lineares para cada imagem e sua reprojeção, mas passam perto.
	// Neste modo o passo 7 ainda é identico ao anterior, pois geramos todas as camadas do fotoindice com a mesma resolução.
	// Bastaria então retornar o deque.
	// A melhoria desta abordagem é não misturarmos as imagens e podermos ver cada imagem do fotoindice isoladamente no visualizador. Isso consome ainda mais memória, mas só.

	// Quanto a questão de projetarmos os pixels das imagens originais para o fotoindice e não buscarmos os pixels do fotoindice entre todas as imagens, essa é uma decisão que afeta principalmente o tempo de execução da montagem.
	// A escolha feita é, hoje, a mais rápida, mas podemos ponderar por fazer as projeções no sentido oposto num outro momento.

	// Quanto a aproximação linear para a radiometria dos pixels, ela pode ser feita nos dois sentidos, mas é algo a ser deixado para depois.
	// No primeiro caso abordado ele seria simples de fazer: é só considerar o inverso da distância dos centros (de cada pixel projetado para o centro do pixel de destino) ao realizar a acumulação. Isso eu esclareço depois com calma.
	//Outro modo é a tradicional interpolação bilinear no caso de fazermos o trabalho no sentido do fotoindice para as imagens.

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
	pt= new BundleAdjustment(listSelectedImages,listSelectedPoints);
	pt->setMaxNumberIterations(maxIterations);
	pt->setMetricConvergencyValue(metricConvergency);
	pt->setAngularConvergencyValue(angularConvergency);

	//photogrammetricSort(listSelectedPoints);

	if (pt->isPossibleCalculate())
	{
		if (hasAllImagesInitialValues())
		{
			Matrix matInitialValues(listAllImages.size(),6);
			for (int i=0;i<listAllImages.size();i++)
			{
				Image *img=listAllImages.at(i);
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
	//	points.show('f',4);
	ENH=points;
	spareENH=ENH;
	spareENH.putMatrix(pointKeys,1,ENH.getCols()+1);
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
				string data = allPoints.at(i).elementByTagAtt("imageCoordinates", "image_key",Conversion::intToString(listAllImages.at(j)->getId())).getContent();
				//qDebug("%s\n",data.c_str());
				if (data != "")
				{
					Point* pointToInsert = efotoManager->instancePoint(Conversion::stringToInt(allPoints.at(i).attribute("key")));
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
		Point* point= efotoManager->instancePoint(Conversion::stringToInt(allPoints.at(i).attribute("key")));
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

string PTManager::getPointId(int pointKey)
{
	return efotoManager->instancePoint(pointKey)->getPointId();
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
			if (pnt->is("PhotogrammetricPoint") && appearances < 2)
				continue;
			if(!pnt->is("CheckingPoint") || (pnt->is("CheckingPoint") && cond!="noCheckingPoint"))
				list.push_back(pnt->getPointId());
		}
		return list;
	}else if (imageFileName =="" && cond=="")
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
				DigitalImageSpaceCoordinate coord=temp->getPointAt(j)->getDigitalCoordinate(temp->getId());
				//qDebug("%s from %s",temp->getPointAt(j)->getPointId().c_str() , imageFileName.c_str());
				result.setInt(j+1,1,coord.getCol());
				result.setInt(j+1,2,coord.getLin());
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
	string points="<points>\n";
	for (int i=0; i<listAllPoints.size(); i++)
		points += listAllPoints.at(i)->xmlGetData().c_str();
	points+="</points>\n";

	EDomElement newXml(efotoManager->xmlGetData());
	newXml.replaceChildByTagName("points",points);
	efotoManager->xmlSetData(newXml.getContent());

	//qDebug("NEWXML:\n%s",newXml.elementByTagName("points").getContent().c_str());
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
		//fotoTriXml << "\t<imageEO type=\"photoTriangulation\" image_key=\"" << Conversion::intToString(i) << "\">\n";
		fotoTriXml << "\t<imageEO type=\"spatialRessection\" image_key=\"" << Conversion::intToString(i) << "\">\n";
		fotoTriXml << "\t\t<iterations>"<< pt->getTotalIterations() <<"</iterations>\n";
		fotoTriXml << "\t\t<converged>"<< (pt->isConverged()? "true":"false")<<"</converged>\n";
		fotoTriXml << "\t\t<parameters>\n";
		fotoTriXml << "\t\t\t<Lb>\n";
		fotoTriXml << lb << "\n</Lb>\n";
		fotoTriXml << "\t\t\t<Xa>\n";
		fotoTriXml << "\t\t\t\t<X0 uom=\"#m\">"<< Conversion::doubleToString(oe.get(i,4)) << "</X0>\n";
		fotoTriXml << "\t\t\t\t<Y0 uom=\"#m\">"<< Conversion::doubleToString(oe.get(i,5)) << "</Y0>\n";
		fotoTriXml << "\t\t\t\t<Z0 uom=\"#m\">"<< Conversion::doubleToString(oe.get(i,6)) << "</Z0>\n";
		fotoTriXml << "\t\t\t\t<phi uom=\"#m\">"<< Conversion::doubleToString(oe.get(i,2)) << "</phi>\n";
		fotoTriXml << "\t\t\t\t<omega uom=\"#m\">"<< Conversion::doubleToString(oe.get(i,1)) << "</omega>\n";
		fotoTriXml << "\t\t\t\t<kappa uom=\"#m\">"<< Conversion::doubleToString(oe.get(i,3)) << "</kappa>\n";
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
			//qDebug("File %s kappa0 = %.7f",imageFile.c_str(),flightDirection);
		}
}

void PTManager::setImageFlightDirection(int imageKey, double flightDirection)
{
	Image *img =efotoManager->instanceImage(imageKey);
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
	Image *img =efotoManager->instanceImage(imageKey);
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


	EDomElement terrain = efotoManager->getXml("terrain");
	int hemiLatitude = terrain.elementByTagName("Lat").attribute("direction")=="S" ? -1:1;
	int zona=Conversion::stringToInt(terrain.elementByTagName("utmFuse").toString());
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

			//QPolygonF
			//EPolygon *pol1= new EPolygon();
			//pol1 << QPointF(E1,N1) << QPointF(E2,N1) << QPointF(E2,N2) << QPointF(E1,N2);

			QList<QPointF *> pnts;
			pnts << new QPointF(E1,N1) <<  new QPointF(E2,N1) <<  new QPointF(E2,N2) << new QPointF(E1,N2);

			EPolygon *polygon = new EPolygon(pnts);
			polygon->setLabel(QString::fromStdString(img->getFilename()));
			epolygons << polygon;

			pnts.clear();
			Matrix plh1= ConvertionsSystems::utmToGeo(E1,N1,zona,hemiLatitude,GeoSystem());
			Matrix plh2= ConvertionsSystems::utmToGeo(E2,N2,zona,hemiLatitude,GeoSystem());


			double lat1 = plh1.get(1,1)*180/M_PI;
			lat1 = (hemiLatitude =='S'? -lat1 : lat1);
			double longi1 = plh1.get(1,2)*180/M_PI;

			double lat2 = plh2.get(1,1)*180/M_PI;
			lat2 = (hemiLatitude =='S'? -lat2 : lat2);
			double longi2 =plh2.get(1,2)*180/M_PI;

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

	string controlPoint="";
	string photogrammetricPoint="";
	string checkingPoint="";

	//qDebug("%c\t%d\t%s",latitude,zona,sys.getSystemName().c_str());
	for(int i=0;i<listAllPoints.size();i++)
	{
		Point *pnt=listAllPoints.at(i);
		//string pointType;
		if (pnt->is("PhotogrammetricPoint"))
			photogrammetricPoint += pointToKml(pnt,zona,hemiLatitude,sys,"photogrammetricPoint");
		else if (pnt->is("ControlPoint"))
			controlPoint += pointToKml(pnt,zona,hemiLatitude,sys,"controlPoint");
		else if (pnt->is("CheckingPoint"))
			checkingPoint += pointToKml(pnt,zona,hemiLatitude,sys, "checkingPoint");
	}

	aux << "<Folder>\n";
	aux << "<name>" << EDomElement(efotoManager->getXml("projectHeader")).elementByTagName("name").toString() << "</name>\n";

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

	Matrix plh=ConvertionsSystems::utmToGeo(E,N,zona,hemiLatitude,sys);

	double lat=plh.get(1,1)*180/M_PI;
	double longi=plh.get(1,2)*180/M_PI;

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
		Point *pnt=efotoManager->instancePoint(spareENH.getInt(i+1,4));

		double x=spareENH.get(i+1,1);
		double y=spareENH.get(i+1,2);
		double z=spareENH.get(i+1,3);

		pnt->getObjectCoordinate().setX(x);
		pnt->getObjectCoordinate().setY(y);
		pnt->getObjectCoordinate().setZ(z);
	}

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

deque<string> PTManager::getPointsWithLesserThanOverlap(int overlap)
{
	deque<string> ids;
	int numPoints=listAllPoints.size();
	for (int i=0;i<numPoints;i++)
	{
		Point * pnt=listAllPoints.at(i);
		if(pnt->is("PhotogrammetricPoint"))
			if (getImagesAppearances(pnt->getId()).size() < overlap)
				ids.push_back(pnt->getPointId());
	}
	return ids;
}

int PTManager::createNewPoint()
{
	string text = "";

	int currentItemId = efotoManager->getFreePointId();
	text += "<point key=\"" + Conversion::intToString(currentItemId) + "\" type=\"photogrammetric\">\n";
	text += "<pointId>PF" + Conversion::intToString(currentItemId) + "</pointId>\n";
	text += "<description>Photogrammetric point created in Phototriangulation</description>\n";
	text += "<spatialCoordinates uom=\"#m\">\n";
	text += "<gml:pos>0.0 0.0 0.0</gml:pos>\n";
	text += "<sigma>Not Available</sigma>\n";
	text += "</spatialCoordinates>\n";
	text += "</point>";

	EDomElement xml(efotoManager->xmlGetData());
	xml.addChildAtTagName("points",text);

	efotoManager->xmlSetData(xml.getContent());
	listAllPoints.push_back(efotoManager->instancePoint(currentItemId));

	return currentItemId;

}
void PTManager::connectPointInImage(int pointKey, int imageKey)
{
	Point* point = efotoManager->instancePoint(pointKey);
	Image* image = efotoManager->instanceImage(imageKey);
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

Matrix PTManager::getDigitalCoordinate(int imageKey, int pointKey)
{
	Matrix result(1,2);
	Point *pnt=efotoManager->instancePoint(pointKey);
	DigitalImageSpaceCoordinate coord=pnt->getDigitalCoordinate(imageKey);
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

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
