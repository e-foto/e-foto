/*Copyright 2002-2014 e-foto team (UERJ)
  This file is part of e-foto.

    e-foto is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    e-foto is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with e-foto.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef WIN32
#define _USE_MATH_DEFINES // for C++
#include <cmath>
#endif
#ifdef unix
#include <math.h>
#endif

#include "PTManager.h"
#include "EFotoManager.h"
#include "PTUserInterface_Qt.h"
#include "ConvertionsSystems.h"

#include "PhotoTriReport.h"

#include "EDom.h"
#include "Point.h"
#include "Image.h"
#include "InteriorOrientation.h"
#include "SensorWithFiducialMarks.h"
#include "Dms.h"
#include "Flight.h"
#include "Terrain.h"
#include "ProjectiveRay.h"
#include "PositionMatrix.h"
#include "ImageSpaceCoordinate.h"
#include "ExteriorOrientation.h"

#include <stdlib.h>
#include <sstream>

#include "EPolygon.h"

#define NUNES

namespace br {
namespace uerj {
namespace eng {
namespace efoto {


PTManager::PTManager(EFotoManager* newManager,
                     std::deque<Image*>images,
                     std::deque<InteriorOrientation*> ois,
                     Sensor* sensor, Flight* flight,
                     Terrain* terrain)
    : efotoManager(newManager),
      myInterface{nullptr},
      listAllImages(images),
      listOis(ois),
      mySensor(sensor),
      myFlight(flight),
      myTerrain(terrain),
      marksSaveState(true),
      started(false),
      status(false),
      localTopocentricMode(false),
      previousData(false),
      maxIterations(6),
      metricConvergency(0.001),
      angularConvergency(0.001),
      pt{nullptr}
{
    setListPoint();//lista todos os pontos
    setENH();
    flightScale = myFlight->getScaleDen();
}

PTManager::~PTManager()
{
}

/* to implement
Matrix PTManager::getFotoIndice(deque<Matrix*> imgs, deque<Matrix> IOs, deque<Matrix> EOs, int width, int height, Matrix& dim) //(GraphicWorkAround)
{
    // Note-se que o resultado deste mÃ©todo vai mostrar claramente a interseÃ§Ã£o entre as imagens, mas ele se aplica apenas para feedback com o usuÃ¡rio.
    // Ele nÃ£o serve para definirmos a exata sobreposiÃ§Ã£o de imagens no domÃ­nio do espaÃ§o objeto.
    // Esta Ã© uma primeira abordagem com apenas uma imagem... num futuro poderemos pensar em emitir todas as imagens, mas com as suas devidas reprojeÃ§Ãµes para o fotoindice.

    // Passo 1: Define o plano mÃ©dio do terreno como Z fixo para todo o fotoindice
    double z = efotoManager->instanceTerrain()->getMeanAltitude();

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
*/

bool PTManager::exec()
{
    if (efotoManager != NULL && mySensor != NULL
            && /*myFlight != NULL &&*/ listAllImages.size() > 1 && listOis.size() > 1) {
        qApp->processEvents();

        if (efotoManager->getInterfaceType().compare("Qt") == 0) {
            myInterface = PTUserInterface_Qt::instance(this);
        }

        for (size_t i = 0; i < listAllImages.size(); i++) {
            mySensor->putImage(listAllImages.at(i));
            listAllImages.at(i)->setSensor(mySensor);
            listAllImages.at(i)->setFlight(myFlight);
            listAllImages.at(i)->setIO(listOis.at(i));
            listOis.at(i)->setImage(listAllImages.at(i));
        }

        connectImagePoints();
        started = true;

        if (myInterface != NULL) {
            myInterface->exec();
        }
    }

    return status = true;
}

void PTManager::returnProject()
{
    efotoManager->reloadProject();
}

std::string PTManager::getImagefile(int imageKey)
{
    Image* img = efotoManager->instanceImage(imageKey);
    return img->getFilename();
}


bool PTManager::calculatePT()
{
    sortPointsSelected();
    pt = new BundleAdjustment(listSelectedImages, listSelectedPoints);

    if (localTopocentricMode) {
        convertToNunes(listSelectedPoints, getTerrainDatum(), getTerrainLatHemisphere(),
                       getTerrainZone());
    }

    pt->setMaxNumberIterations(maxIterations);
    pt->setMetricConvergencyValue(metricConvergency);
    pt->setAngularConvergencyValue(angularConvergency * M_PI / 180);

    if (pt->isPossibleCalculate()) {
        if (hasAllImagesInitialValues()) {
            Matrix matInitialValues(listSelectedImages.size(), 6);

            for (size_t i = 0; i < listSelectedImages.size(); i++) {
                Image* img = listSelectedImages.at(i);

                if(img->isInsAvailable() && img->getInsType() == "Initial") {
                    matInitialValues.set(i + 1, 1, img->getInsOmega());
                    matInitialValues.set(i + 1, 2, img->getInsPhi());
                    matInitialValues.set(i + 1, 3, img->getInsKappa());
                }

                if(img->isGnssAvailable() && img->getGnssType() == "Initial") {
                    matInitialValues.set(i + 1, 4, img->getGnssX0());
                    matInitialValues.set(i + 1, 5, img->getGnssY0());
                    matInitialValues.set(i + 1, 6, img->getGnssZ0());
                }
            }

            pt->setUserInitialValues(matInitialValues);
        }

        bool result = pt->calculate(false);
        setMatrixAFP(pt->getAFP());

        if (localTopocentricMode) {
            convertToUTM(listSelectedPoints, getTerrainDatum());
        }

        residuos = pt->getMatRes();
        return result;
    } else {
        return false;
    }
}

void PTManager::setMatrixAFP(Matrix afp)
{
    Matrix result(afp.getRows(), afp.getCols());

    for (size_t i = 1; i <= afp.getRows(); i++) {
        result.set(i, 1, Dms::radianoToDegreeDecimal(afp.get(i, 1)));
        result.set(i, 2, Dms::radianoToDegreeDecimal(afp.get(i, 2)));
        double kappa0 = Dms::radianoToDegreeDecimal(afp.get(i, 3));
        result.set(i, 3, kappa0 > 180.0 ? kappa0 - 360 : kappa0 );
        result.set(i, 4, afp.get(i, 4));
        result.set(i, 5, afp.get(i, 5));
        result.set(i, 6, afp.get(i, 6));
    }

    AFP = result;
}

Matrix PTManager::getMatrixOE() const
{
    if (pt == NULL) {
        return Matrix(0, 0);
    }

    return AFP;
}

void PTManager::setENH()
{
    Matrix points(listAllPoints.size(), 3);
    Matrix pointKeys(listAllPoints.size(), 1);

    for (size_t i = 0; i < listAllPoints.size(); i++) {
        Point* p = listAllPoints.at(i);
        double E = p->getObjectCoordinate().getX();
        double N = p->getObjectCoordinate().getY();
        double H = p->getObjectCoordinate().getZ();
        pointKeys.set(i + 1, 1, p->getId());
        points.set(i + 1, 1, E);
        points.set(i + 1, 2, N);
        points.set(i + 1, 3, H);
    }

    //points.show('f',4);
    ENH = points;
    spareENH = ENH;
    spareENH.putMatrix(pointKeys, 1, ENH.getCols() + 1);
}

Matrix PTManager::getENH() const
{
    return ENH;
}

bool PTManager::connectImagePoints()
{
    if (!(started)) {
        qApp->processEvents();
        EDomElement xmlPoints(efotoManager->getXml("points"));
        std::deque<EDomElement> allPoints = xmlPoints.elementsByTagName("point");

        for (unsigned int j = 0; j < listAllImages.size(); j++) {
            for (unsigned int i = 0; i < allPoints.size(); i++) {
                std::string data = allPoints.at(i).elementByTagAtt("imageCoordinates",
                                   "image_key", Conversion::intToString(listAllImages.at(
                                               j)->getId())).getContent();

                if (data != "") {
                    Point* pointToInsert = efotoManager->instancePoint(Conversion::stringToInt(
                                               allPoints.at(i).attribute("key")));

                    if (pointToInsert != NULL) {
                        listAllImages.at(j)->putPoint(pointToInsert);
                        pointToInsert->putImage(listAllImages.at(j));//novo em teste:06/08/2011
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
    std::deque<EDomElement> allPoints = xmlPoints.elementsByTagName("point");

    for(size_t i = 0; i < allPoints.size(); i++) {
        Point* point = efotoManager->instancePoint(Conversion::stringToInt(allPoints.at(
                           i).attribute("key")));

        if (point != NULL) { // && !point->getType() == Point::CHECKING)
            listAllPoints.push_back(point);
        }
    }
}
// retorna uma lista com os nomes das imagens
std::deque<std::string> PTManager::getStringImages()
{
    std::deque<std::string> list;
    size_t numImages = listAllImages.size();

    for (size_t i = 0; i < numImages; i++) {
        list.push_back(listAllImages.at(i)->getFilename());
    }

    return list;
}

std::string PTManager::getFilePath(std::string fileName)
{
    size_t numImages = listAllImages.size();

    for (size_t i = 0; i < numImages; i++)
        if(listAllImages.at(i)->getFilename() == fileName) {
            std::string file = listAllImages.at(i)->getFilepath() + "/" + fileName;
            return file;
        }

    return "";
}

std::string PTManager::getFilePath(int imageKey)
{
    size_t numImages = listAllImages.size();

    for (size_t i = 0; i < numImages; i++)
        if(listAllImages.at(i)->getId() == imageKey) {
            std::string file = listAllImages.at(i)->getFilepath() + "/" + listAllImages.at(
                                   i)->getFilename();
            return file;
        }

    return "";
}


std::deque<std::string> PTManager::getStringTypePoints(std::string
        imageFileName)
{
    std::deque<std::string> list;

    if (imageFileName == "") {
        size_t numPnts = listAllPoints.size();

        for (size_t i = 0; i < numPnts; i++) {
            if (listAllPoints.at(i) && listAllPoints.at(i)->getType() == Point::CONTROL) {
                list.push_back("Control");
            }

            if (listAllPoints.at(i)
                    && listAllPoints.at(i)->getType() == Point::PHOTOGRAMMETRIC) {
                list.push_back("Photogrammetric");
            }

            if (listAllPoints.at(i) && listAllPoints.at(i)->getType() == Point::CHECKING) {
                list.push_back("Checking");
            }

            if (listAllPoints.at(i) && listAllPoints.at(i)->getType() == Point::UNKNOWN) {
                list.push_back("Unknown");
            }
        }

        return list;
    } else {
        size_t numImages = listAllImages.size();

        for (size_t i = 0; i < numImages; i++) {
            if(listAllImages.at(i)->getFilename() == imageFileName) {
                Image* temp = listAllImages.at(i);
                size_t numpnts = temp->countPoints();

                for (size_t j = 0; j < numpnts; j++) {
                    if (temp->getPointAt(j)
                            && temp->getPointAt(j)->getType() == Point::CONTROL) {
                        list.push_back("Control");
                    }

                    if (temp->getPointAt(j)
                            && temp->getPointAt(j)->getType() == Point::PHOTOGRAMMETRIC) {
                        list.push_back("Photogrammetric");
                    }

                    if (temp->getPointAt(j)
                            && temp->getPointAt(j)->getType() == Point::CHECKING) {
                        list.push_back("Checking");
                    }

                    if (temp->getPointAt(j)
                            && temp->getPointAt(j)->getType() == Point::UNKNOWN) {
                        list.push_back("Unknown");
                    }
                }

                return list;
            }
        }
    }

    throw "Image file problem";
}

// preenche a lista de imagens selecionadas(listSelectedImages) atraves de um deque de strings com os caminhos das imagens
void PTManager::selectImages(std::deque<std::string> selectedImagesList)
{
    if(listSelectedImages.size() != 0) {
        listSelectedImages.clear();
    }

    size_t numImgs = listAllImages.size();
    size_t  numSelectedImgs = selectedImagesList.size();

    for(size_t i = 0; i < numSelectedImgs; i++)
        for (size_t j = 0; j < numImgs; j++)
            if (listAllImages.at(j)->getFilename() == selectedImagesList.at(i)) {
                listSelectedImages.push_back(listAllImages.at(j));
            }

    //listSelectedImages=listImages;
}
// preenche a lista de pontos selecionados(listSelectedPoints) atraves de um deque de strings com Ids dos pontos
void PTManager::selectPoints(std::deque<std::string> selectedPointsList)
{
    if(listSelectedPoints.size() != 0) {
        listSelectedPoints.clear();
    }

    size_t numPnts = listAllPoints.size();
    size_t  numSelectedPnts = selectedPointsList.size();

    for(size_t i = 0; i < numSelectedPnts; i++)
        for (size_t j = 0; j < numPnts; j++)
            if (listAllPoints.at(j)->getPointId() == selectedPointsList.at(i)
                    && listAllPoints.at(j)->getType() != Point::CHECKING) {
                listSelectedPoints.push_back(listAllPoints.at(j));
            }
}

BundleAdjustment* PTManager::getBundleAdjustment()
{
    return pt;
}

std::string PTManager::getPointId(int pointKey)
{
    return efotoManager->instancePoint(pointKey)->getPointId();
}


// retorna uma lista com os ids dos pontos
std::deque<std::string> PTManager::getStringIdPoints(std::string imageFileName,
        std::string cond)
{
    std::deque<std::string> list;

    if (imageFileName == "" && cond != "") {
        size_t numPnts = listAllPoints.size();

        for (size_t i = 0; i < numPnts; i++) {
            Point* pnt = listAllPoints.at(i);
            int appearances = getImagesAppearances(pnt->getId()).size();

            if (pnt->getType() == Point::PHOTOGRAMMETRIC && appearances < 2) {
                continue;
            }

            if(pnt->getType() != Point::CHECKING || cond != "noCheckingPoint") {
                list.push_back(pnt->getPointId());
            }
        }

        return list;
    } else if (imageFileName == "" && cond == "") {
        int numPnts = listAllPoints.size();

        for (int i = 0; i < numPnts; i++) {
            Point* pnt = listAllPoints.at(i);

            if(pnt->getType() != Point::CHECKING || cond != "noCheckingPoint") {
                list.push_back(pnt->getPointId());
            }
        }

        return list;
    } else {
        int numImages = listAllImages.size();

        for (int i = 0; i < numImages; i++) {
            if(listAllImages.at(i)->getFilename() == imageFileName) {
                Image* temp = listAllImages.at(i);
                int numpnts = temp->countPoints();

                for (int j = 0; j < numpnts; j++) {
                    list.push_back(temp->getPointAt(j)->getPointId());
                }

                return list;
            }
        }
    }

    return list;
}

std::deque<std::string> PTManager::getStringKeysPoints(std::string
        imageFileName)
{
    std::deque<std::string> list;

    if (imageFileName == "") {
        int numPnts = listAllPoints.size();

        for (int i = 0; i < numPnts; i++) {
            Point* pnt = listAllPoints.at(i);
            list.push_back(Conversion::intToString(pnt->getId()));
        }

        return list;
    } else {
        int numImages = listAllImages.size();

        for (int i = 0; i < numImages; i++) {
            if(listAllImages.at(i)->getFilename() == imageFileName) {
                Image* temp = listAllImages.at(i);
                int numpnts = temp->countPoints();

                for (int j = 0; j < numpnts; j++) {
                    list.push_back(Conversion::intToString(temp->getPointAt(j)->getId()));
                }

                return list;
            }
        }
    }

    throw "Image file error";
}

Matrix PTManager::getColLin(std::string imageFilename)
{
    int numImages = listAllImages.size();

    for (int i = 0; i < numImages; i++) {
        if(listAllImages.at(i)->getFilename() == imageFilename) {
            Image* temp = listAllImages.at(i);
            int numpnts = temp->countPoints();
            Matrix result(numpnts, 2);

            for (int j = 0; j < numpnts; j++) {
                ImageSpaceCoordinate coord = temp->getPointAt(j)->getImageCoordinate(
                                                 temp->getId());
                result.set(j + 1, 1, coord.getCol());
                result.set(j + 1, 2, coord.getLin());
            }

            return result;
        }
    }

    throw "image File do not found";
}

Matrix PTManager::getColLin(int imageKey)
{
    size_t numImages = listAllImages.size();

    for (size_t i = 0; i < numImages; i++) {
        if(listAllImages.at(i)->getId() == imageKey) {
            Image* temp = listAllImages.at(i);
            int numpnts = temp->countPoints();
            Matrix result(numpnts, 2);

            for (int j = 0; j < numpnts; j++) {
                ImageSpaceCoordinate coord = temp->getPointAt(j)->getImageCoordinate(
                                                 temp->getId());
                result.set(j + 1, 1, coord.getCol());
                result.set(j + 1, 2, coord.getLin());
            }

            return result;
        }
    }

    throw "imageKey do not found";
}


//Faz um update das coordenadas digitais do ponto 'pointKey' na imagem 'imageKey'
void PTManager::updateDigitalCoordinatesPoint(int imageId, int pointKey,
        double col, double lin)
{
    Point* pnt = efotoManager->instancePoint(pointKey);
    pnt->deleteImageCoordinate(imageId);
    ImageSpaceCoordinate temp = ImageSpaceCoordinate(imageId, col, lin);
    pnt->putImageCoordinate(temp);
}

// Procura a key da imagem pelo nome do arquivo senao encontrar retorna -1
int PTManager::getImageId(std::string imageFilename)
{
    int numImages = listAllImages.size();

    for (int i = 0; i < numImages; i++)
        if(listAllImages.at(i)->getFilename() == imageFilename) {
            return listAllImages.at(i)->getId();
        }

    return -1;
}

void PTManager::sortPointsSelected()
{
    std::deque<Point*>listCtrl;
    size_t numSelectedPoints = listSelectedPoints.size();

    for(size_t i = 0; i < numSelectedPoints ; i++) {
        Point* pnt = listSelectedPoints.at(i);

        if (pnt->getType() == Point::CONTROL) {
            listCtrl.push_front(pnt);
        } else if (pnt->getType() == Point::PHOTOGRAMMETRIC) {
            listCtrl.push_back(pnt);
        }
    }

    listSelectedPoints = listCtrl;
}

// retorna o ENH dos pontos Fotogrammetricos
Matrix PTManager::getPhotogrammetricENH()
{
    size_t points = listSelectedPoints.size();
    Matrix enh(0, 3);

    for (size_t i = 0; i < points; i++) {
        Point* pnt = listSelectedPoints.at(i);

        if (pnt->getType() == Point::PHOTOGRAMMETRIC) {
            ObjectSpaceCoordinate coor = pnt->getObjectCoordinate();
            Matrix temp(1, 3);
            temp.set(1, 1, coor.getX());
            temp.set(1, 2, coor.getY());
            temp.set(1, 3, coor.getZ());
            enh = enh | temp;
        }
    }

    return enh;
}

Matrix PTManager::getResiduoPhotogrammetric()
{
    size_t points = listSelectedPoints.size();
    Matrix residuos(0, 3);

    for (size_t i = 0; i < points; i++) {
        Point* pnt = listSelectedPoints.at(i);

        if (pnt->getType() == Point::PHOTOGRAMMETRIC) {
            Matrix temp = pt->getResiduo(pnt);
            residuos = residuos | temp;
        }
    }

    return residuos;
}

std::deque<std::string> PTManager::getSelectedPointIdPhotogrammetric()
{
    size_t points = listSelectedPoints.size();
    std::deque<std::string> selected;

    for (size_t i = 0; i < points; i++) {
        Point* pnt = listSelectedPoints.at(i);

        if (pnt->getType() == Point::PHOTOGRAMMETRIC) {
            selected.push_back(pnt->getPointId());
        }
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
std::deque<std::string> PTManager::getImagesAppearances(int pointKey)
{
    std::deque<std::string> appearance;
    Point* pnt = efotoManager->instancePoint(pointKey);
    int numImages = pnt->countImages();

    for(int i = 0; i < numImages; i++) {
        appearance.push_back(pnt->getImageAt(i)->getFilename());
    }

    return appearance;
}

void PTManager::saveMarks()
{
    //qDebug("Chamado metodo para salvar as marcas");
    std::string points = "<points>\n";

    for (size_t i = 0; i < listAllPoints.size(); i++) {
        points += listAllPoints.at(i)->xmlGetData().c_str();
    }

    points += "</points>\n";
    EDomElement newXml(efotoManager->xmlGetData());
    newXml.replaceChildByTagName("points", points);
    efotoManager->xmlSetData(newXml.getContent());
    efotoManager->saveEPP();
    //qDebug("NEWXML:\n%s",newXml.elementByTagName("points").getContent().c_str());
}

void PTManager::saveImages()
{
    std::string images = "<images>\n";

    for (size_t i = 0; i < listAllImages.size(); i++) {
        images += listAllImages.at(i)->xmlGetData().c_str();
    }

    images += "</images>\n";
    EDomElement newXml(efotoManager->xmlGetData());
    newXml.replaceChildByTagName("images", images);
    efotoManager->xmlSetData(newXml.getContent());
}

void PTManager::saveBundleAdjustment()
{
    EDomElement newExteriorOrientationXml(createOESXml());
    EDomElement oldExteriorOrientationXml(
        efotoManager->getXml("exteriorOrientation"));
    std::deque<EDomElement> oldEos =
        oldExteriorOrientationXml.elementsByTagName("imageEO");
    EDomElement oldEpp(efotoManager->xmlGetData());

    if (oldEpp.hasTagName("phototriangulation")) {
        oldEpp.replaceChildByTagName("phototriangulation", getPhotoTriXml());
    } else {
        oldEpp.addChildAtTagName("efotoPhotogrammetricProject", getPhotoTriXml());
    }

    EDomElement temp;

    for (size_t i = 0; i < oldEos.size() ; i++) {
        temp.setContent(oldEos.at(i).getContent());
        std::string imageKey = temp.attribute("image_key");

        if (newExteriorOrientationXml.elementByTagAtt("imageEO", "image_key",
                imageKey).getContent() == ""  && temp.attribute("type") == "spatialResection") {
            newExteriorOrientationXml.addChildAtTagName("exteriorOrientation",
                    temp.getContent());
        }
    }

    /**/
    /**/
    oldEpp.replaceChildByTagName("exteriorOrientation",
                                 newExteriorOrientationXml.getContent());
    efotoManager->xmlSetData(oldEpp.getContent());
}

std::string PTManager::createOESXml()
{
    std::stringstream fotoTriXml;
    //codigo de criacao da xml da bundle(multiplas tags de Orientacao Exterior)
    Matrix oe = AFP;
    fotoTriXml << "<exteriorOrientation>\n";

    for (size_t i = 1; i <= oe.getRows(); i++) {
        Image* img = listSelectedImages.at(i - 1);
        fotoTriXml << "\t<imageEO type=\"photoTriangulation\" image_key=\"" <<
                   Conversion::intToString(img->getId()) << "\">\n";
        fotoTriXml << "\t\t\t<Xa>\n";
        fotoTriXml << "\t\t\t\t<X0 uom=\"#m\">" << Conversion::doubleToString(oe.get(i,
                   4)) << "</X0>\n";
        fotoTriXml << "\t\t\t\t<Y0 uom=\"#m\">" << Conversion::doubleToString(oe.get(i,
                   5)) << "</Y0>\n";
        fotoTriXml << "\t\t\t\t<Z0 uom=\"#m\">" << Conversion::doubleToString(oe.get(i,
                   6)) << "</Z0>\n";
        fotoTriXml << "\t\t\t\t<phi uom=\"#rad\">" << Conversion::doubleToString(oe.get(
                       i, 2)*M_PI / 180) << "</phi>\n";
        fotoTriXml << "\t\t\t\t<omega uom=\"#rad\">" << Conversion::doubleToString(
                       oe.get(i, 1)*M_PI / 180) << "</omega>\n";
        fotoTriXml << "\t\t\t\t<kappa uom=\"#rad\">" << Conversion::doubleToString(
                       oe.get(i, 3)*M_PI / 180) << "</kappa>\n";
        fotoTriXml << "\t\t\t</Xa>\n";
        fotoTriXml << "</imageEO>\n";
    }

    fotoTriXml << "</exteriorOrientation>\n";
    return fotoTriXml.str();
}

std::string PTManager::getPhotoTriXml()
{
    std::stringstream fotoTriXml;
    fotoTriXml << "<phototriangulation>\n";
    fotoTriXml << "\t<iterations>" << Conversion::intToString(
                   pt->getTotalIterations()) << "</iterations>\n";
    fotoTriXml << "\t<converged>" << (pt->isConverged() ? "true" : "false") <<
               "</converged>\n";
    fotoTriXml << "\t<metricConvergency uom=\"#m\">" <<
               pt->getMetricConvergencyValue() << "</metricConvergency>\n";
    fotoTriXml << "\t<angularConvergency uom=\"#rad\">" <<
               pt->getAngularConvergencyValue() << "</angularConvergency>\n";
    fotoTriXml << "\t<rmse>" << pt->calculateRMSE() << "</rmse>\n";
    fotoTriXml << getUsedPointsXml() << getUsedImagesXml();
    fotoTriXml << "</phototriangulation>\n";
    return fotoTriXml.str();
}

std::string PTManager::getUsedPointsXml()
{
    std::stringstream usedPoints;
    usedPoints << "\t<usedPoints>\n";

    for (size_t i = 0; i < listSelectedPoints.size(); i++) {
        Point* pnt = listSelectedPoints.at(i);
        usedPoints << "\t\t<pointKey>" <<   pnt->getId()    << "</pointKey>\n";
    }

    usedPoints << "\t</usedPoints>\n";
    return usedPoints.str();
}

std::string PTManager::getUsedImagesXml()
{
    std::stringstream usedImages;
    usedImages << "\t<usedImages>\n";

    for (size_t i = 0; i < listSelectedImages.size(); i++) {
        Image* img = listSelectedImages.at(i);
        usedImages << "\t\t<imageKey>" <<   img->getId()    << "</imageKey>\n";
    }

    usedImages << "\t</usedImages>\n";
    return usedImages.str();
}

void PTManager::setMarksSavedState(bool marksState)
{
    marksSaveState = marksState;
    efotoManager->setSavedState(marksState);
}

bool PTManager::getMarksSavedState() const
{
    return marksSaveState;
}

void PTManager::setImageFlightDirection(std::string imageFile,
                                        double flightDirection)
{
    for (size_t i = 0; i < listAllImages.size(); i++)
        if(listAllImages.at(i)->getFilename() == imageFile) {
            listAllImages.at(i)->setFlightDirection(flightDirection);
        }
}

void PTManager::setImageFlightDirection(int imageKey, double flightDirection)
{
    Image* img = efotoManager->instanceImage(imageKey);

    if (img != NULL) {
        img->setFlightDirection(flightDirection);
    }
}

double PTManager::getLongitudinalOverlap(std::string imageFile)
{
    for (size_t i = 0; i < listAllImages.size(); i++)
        if(listAllImages.at(i)->getFilename() == imageFile) {
            return listAllImages.at(i)->getFlight()->getLongitudinalOverlap();
        }

    return -1;
}

std::string PTManager::exportBlockTokml(std::string fileName, bool fromXML)
{
    //QList<EPolygon *> epolygons;
    int hemiLatitude = getTerrainLatHemisphere();
    int zona = getTerrainZone();
    GeoSystem sys = getTerrainDatum();
    std::stringstream aux;
    aux << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<kml xmlns=\"http://www.opengis.net/kml/2.2\" xmlns:gx=\"http://www.google.com/kml/ext/2.2\" xmlns:kml=\"http://www.opengis.net/kml/2.2\" xmlns:atom=\"http://www.w3.org/2005/Atom\">\n";
    aux << "<Document>\n<name>" << fileName << "</name>\n";
    std::string controlPointIcon         =
        "http://maps.google.com/mapfiles/kml/paddle/wht-blank.png";
    std::string photogrammetricPointIcon =
        "http://maps.google.com/mapfiles/kml/shapes/placemark_circle.png";
    std::string checkingPointIcon        =
        "http://maps.google.com/mapfiles/kml/paddle/grn-blank.png";
    std::string colorNormalControl = "ff00ff00";
    std::string colorNormalPhotogrammetric = "ff00ffff";
    std::string colorNormalChecking = "ff12f580" ; //18--245-128
    std::string colorHighLightControl = "ff0000ff";
    std::string colorHighLightPhotogrammetric = "ff0000ff";
    std::string colorHighLightChecking = "ff12f580";
    // Style para pontos de controle
    aux << "<StyleMap id=\"controlPoint\">\n<Pair>\n<key>normal</key>\n<styleUrl>#sn_ControlPoint</styleUrl>\n</Pair>\n<Pair>\n<key>highlight</key>\n<styleUrl>#sh_ControlPoint</styleUrl>\n</Pair>\n</StyleMap>\n";
    aux << "<Style id=\"sn_ControlPoint\">\n<IconStyle>\n<color>" <<
        colorNormalControl << "</color>\n<scale>1.0</scale>\n<Icon>\n<href>" <<
        controlPointIcon <<
        "</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n</Style>\n";
    aux << "<Style id=\"sh_ControlPoint\">\n<IconStyle>\n<color>" <<
        colorHighLightControl << "</color>\n<scale>1.1</scale>\n<Icon>\n<href>" <<
        controlPointIcon <<
        "</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n</Style>\n";
    // Style para pontos fotogrametricos
    aux << "<StyleMap id=\"photogrammetricPoint\">\n<Pair>\n<key>normal</key>\n<styleUrl>#sn_PhotogrammetricPoint</styleUrl>\n</Pair>\n<Pair>\n<key>highlight</key>\n<styleUrl>#sh_PhotogrammetricPoint</styleUrl>\n</Pair>\n</StyleMap>\n";
    aux << "<Style id=\"sn_PhotogrammetricPoint\">\n<IconStyle>\n<color>" <<
        colorNormalPhotogrammetric << "</color>\n<scale>1.0</scale>\n<Icon>\n<href>" <<
        photogrammetricPointIcon <<
        "</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n</Style>\n";
    aux << "<Style id=\"sh_PhotogrammetricPoint\">\n<IconStyle>\n<color>" <<
        colorHighLightPhotogrammetric << "</color>\n<scale>1.1</scale>\n<Icon>\n<href>"
        << photogrammetricPointIcon <<
        "</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n</Style>\n";
    // Style para pontos de checking
    aux << "<StyleMap id=\"checkingPoint\">\n<Pair>\n<key>normal</key>\n<styleUrl>#sn_CheckingPoint</styleUrl>\n</Pair>\n<Pair>\n<key>highlight</key>\n<styleUrl>#sh_CheckingPoint</styleUrl>\n</Pair>\n</StyleMap>\n";
    aux << "<Style id=\"sn_CheckingPoint\">\n<IconStyle>\n<color>" <<
        colorNormalChecking << "</color>\n<scale>1.0</scale>\n<Icon>\n<href>" <<
        checkingPointIcon <<
        "</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n</Style>\n";
    aux << "<Style id=\"sh_CheckingPoint\">\n<IconStyle>\n<color>" <<
        colorHighLightChecking << "</color>\n<scale>1.1</scale>\n<Icon>\n<href>" <<
        checkingPointIcon <<
        "</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n</Style>\n";

    if (fromXML) {
        AFP = eoParametersFromXml(false);
        //Conectar imagens e respectivos sensores
    }

    Matrix oe = getMatrixOE();

    if (oe.getCols() > 1 ) {
        int B = 0;
        int G = 0;
        int R = 0;

        for (size_t i = 0; i < listAllImages.size(); i++) {
            std::string icon =
                "http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png";

            switch (i % 3) {
            case 0:
                B = rand() % 15;
                break;

            case 1:
                G = rand() % 15;
                break;

            case 2:
                R = rand() % 15;
                break;

            default :
                B = rand() % 15;
                G = rand() % 15;
                R = rand() % 15;
                break;
            }

            std::stringstream aux1;
            aux1 << std::hex << "ff" << B << G << R << B << G << R;
            std::string lineColor = aux1.str();
            std::string lineWidth = "2";
            aux << "<StyleMap id=\"msn_ylw-pushpin" << Conversion::intToString(
                    i) << "\">\n<Pair>\n<key>normal</key>\n<styleUrl>#sn_ylw-pushpin" <<
                Conversion::intToString(i) <<
                "</styleUrl>\n</Pair>\n<Pair>\n<key>highlight</key>\n<styleUrl>#sh_ylw-pushpin"
                << Conversion::intToString(i) << "</styleUrl>\n</Pair>\n</StyleMap>\n";
            aux << "<Style id=\"sn_ylw-pushpin" << Conversion::intToString(
                    i) << "\">\n<IconStyle>\n<scale>1.3</scale>\n<Icon>\n<href>" << icon <<
                "</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n"
                << "<LineStyle>\n<color>" << lineColor << "</color>\n<width>" << lineWidth <<
                "</width>\n</LineStyle>\n<PolyStyle>\n<fill>0</fill>\n</PolyStyle>\n</Style>\n";
            aux << "<Style id=\"sh_ylw-pushpin" << Conversion::intToString(
                    i) << "\">\n<IconStyle>\n<scale>1.3</scale>\n<Icon>\n<href>" << icon <<
                "</href>\n</Icon>\n<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n</IconStyle>\n"
                << "<LineStyle>\n<color>" << lineColor << "</color>\n<width>" << lineWidth <<
                "</width>\n</LineStyle>\n<PolyStyle>\n<fill>0</fill>\n</PolyStyle>\n</Style>\n";
            Image* img = listAllImages.at(i);
            int width = img->getWidth();
            int height = img->getHeight();
            double Z0 = oe.get(i + 1, 6);
            double c = img->getSensor()->getFocalDistance();
            double P1x = pt->digitalToAnalog(listOis.at(i), height / 2, 0).get(1, 1);
            double PPx = img->getSensor()->getPrincipalPointCoordinates().getXi();
            double PPy = img->getSensor()->getPrincipalPointCoordinates().getEta();
            double P2y = pt->digitalToAnalog(listOis.at(i), 0, width / 2).get(1, 2);
            double deltaE = Z0 * (P1x - PPx) / c;
            double deltaN = Z0 * (P2y - PPy) / c;
            double E1 = oe.get(i + 1, 4) - deltaE;
            double N1 = oe.get(i + 1, 5) + deltaN;
            double E2 = oe.get(i + 1, 4) + deltaE;
            double N2 = oe.get(i + 1, 5) - deltaN;
            Matrix temp1 = ConvertionsSystems::utmToGeo(E1, N1, zona, hemiLatitude, sys);
            Matrix temp2 = ConvertionsSystems::utmToGeo(E2, N2, zona, hemiLatitude, sys);
            double lat1, longi1, lat2, longi2;
            Matrix plh1, plh2;
            qDebug("old system: %s", sys.getSystemName().c_str());

            if (sys.getSystemName() != "WGS84") {
                double oldPhi1 = temp1.get(1, 1);
                double oldLambda1 = temp1.get(1, 2);
                double oldPhi2 = temp2.get(1, 1);
                double oldLambda2 = temp2.get(1, 2);
                GeoSystem newGeoSys(WGS84);
                plh1 = ConvertionsSystems::convertSystemsSimplifiedMolodensky(sys, newGeoSys,
                        oldPhi1, oldLambda1, 0);
                plh2 = ConvertionsSystems::convertSystemsSimplifiedMolodensky(sys, newGeoSys,
                        oldPhi2, oldLambda2, 0);
                lat1 = plh1.get(1, 1) * 180 / M_PI;
                lat1 = (hemiLatitude == 'S' ? -lat1 : lat1);
                longi1 = plh1.get(1, 2) * 180 / M_PI;
                lat2 = plh2.get(1, 1) * 180 / M_PI;
                lat2 = (hemiLatitude == 'S' ? -lat2 : lat2);
                longi2 = plh2.get(1, 2) * 180 / M_PI;
            } else {
                plh1 = temp1;
                plh2 = temp2;
                lat1 = plh1.get(1, 1) * 180 / M_PI;
                lat1 = (hemiLatitude == 'S' ? -lat1 : lat1);
                longi1 = plh1.get(1, 2) * 180 / M_PI;
                lat2 = plh2.get(1, 1) * 180 / M_PI;
                lat2 = (hemiLatitude == 'S' ? -lat2 : lat2);
                longi2 = plh2.get(1, 2) * 180 / M_PI;
            }

            std::stringstream coord;
            coord << Conversion::doubleToString(longi1) << "," <<
                  Conversion::doubleToString(lat1) << ",0 ";
            coord << Conversion::doubleToString(longi2) << "," <<
                  Conversion::doubleToString(lat1) << ",0 ";
            coord << Conversion::doubleToString(longi2) << "," <<
                  Conversion::doubleToString(lat2) << ",0 ";
            coord << Conversion::doubleToString(longi1) << "," <<
                  Conversion::doubleToString(lat2) << ",0 ";
            coord << Conversion::doubleToString(longi1) << "," <<
                  Conversion::doubleToString(lat1) << ",0 ";
            std::string coordenadas = coord.str();
            aux << "<Placemark>\n";
            aux << "<name>" << img->getFilename() << "</name>\n";
            aux << "<styleUrl>";
            aux << "#msn_ylw-pushpin" << Conversion::intToString(i) << "</styleUrl>\n";
            aux << "<LineString>\n";
            aux << "<tessellate>1</tessellate>\n";
            aux << "<coordinates>\n";
            aux << coordenadas << "\n";
            aux << "</coordinates>\n";
            aux << "</LineString>\n";
            aux << "</Placemark>\n";
        }
    }

    std::string controlPoint = "";
    std::string photogrammetricPoint = "";
    std::string checkingPoint = "";

    for(size_t i = 0; i < listAllPoints.size(); i++) {
        Point* pnt = listAllPoints.at(i);

        //string pointType;
        if (pnt->getType() == Point::PHOTOGRAMMETRIC) {
            photogrammetricPoint += pointToKml(pnt, zona, hemiLatitude, sys,
                                               "photogrammetricPoint");
        } else if (pnt->getType() == Point::CONTROL) {
            controlPoint += pointToKml(pnt, zona, hemiLatitude, sys, "controlPoint");
        } else if (pnt->getType() == Point::CHECKING) {
            checkingPoint += pointToKml(pnt, zona, hemiLatitude, sys, "checkingPoint");
        }
    }

    aux << "<Folder>\n";
    aux << "<name>" << EDomElement(
            efotoManager->getXml("projectHeader")).elementByTagName("name").toString() <<
        "</name>\n";
    aux << "<Folder>\n";
    aux << "<name>" << "Control Points" << "</name>\n";
    aux << controlPoint;
    aux << "</Folder>\n";
    aux << "<Folder>\n";
    aux << "<name>" << "Photogrammetric Points" << "</name>\n";
    aux << photogrammetricPoint;
    aux << "</Folder>\n";
    aux << "<Folder>\n";
    aux << "<name>" << "Checking Points" << "</name>\n";
    aux << checkingPoint;
    aux << "</Folder>\n";
    aux << "</Folder>\n";
    aux << "</Document>\n</kml>";
    EDomElement xmlgoogle(aux.str());
    return xmlgoogle.indent('\t').getContent();
}

std::string PTManager::pointToKml(Point* pnt,
                                  int zona,
                                  int hemiLatitude,
                                  GeoSystem sys,
                                  std::string pointType)
{
    std::stringstream pointString;
    const auto E = pnt->getObjectCoordinate().getX();
    const auto  N = pnt->getObjectCoordinate().getY();
    double H = pnt->getObjectCoordinate().getZ();
    Matrix temp = ConvertionsSystems::utmToGeo(E, N, zona, hemiLatitude, sys);
    double lat, longi;
    Matrix plh;

    if (sys.getSystemName() != "WGS84") {
        double oldPhi = temp.get(1, 1);
        double oldLambda = temp.get(1, 2);
        GeoSystem newGeoSys(WGS84);
        plh = ConvertionsSystems::convertSystemsSimplifiedMolodensky(sys, newGeoSys,
                oldPhi, oldLambda, H);
        lat = plh.get(1, 1) * 180 / M_PI;
        lat = (hemiLatitude == 'S' ? -lat : lat);
        longi = plh.get(1, 2) * 180 / M_PI;
    } else {
        plh = temp;
        lat = plh.get(1, 1) * 180 / M_PI;
        lat = (hemiLatitude == 'S' ? -lat : lat);
        longi = plh.get(1, 2) * 180 / M_PI;
    }

    H = plh.get(1, 3);
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
    pointString << Conversion::doubleToString(H) << "</altitude>\n";
    pointString << "<heading>0.01795058696543714</heading>\n";
    pointString << "<tilt>0</tilt>\n";
    pointString << "<range>1463.920597456832</range>\n";
    pointString << "<altitudeMode>relativeToGround</altitudeMode>\n";
    pointString << "<gx:altitudeMode>relativeToSeaFloor</gx:altitudeMode>\n";
    pointString << "</LookAt>\n";
    pointString << "<styleUrl>#" << pointType << "</styleUrl>\n";
    pointString << "<Point>\n";
    pointString << "<altitudeMode>clampToGround</altitudeMode>\n";
    pointString << "<gx:altitudeMode>clampToSeaFloor</gx:altitudeMode>\n";
    pointString << "<coordinates>";
    pointString << Conversion::doubleToString(longi);
    pointString << ",";
    pointString << Conversion::doubleToString(lat);
    pointString << ",";
    pointString << Conversion::doubleToString(H) << "</coordinates>\n";
    pointString << "</Point>\n";
    pointString << "</Placemark>\n";
    return pointString.str();
}


void PTManager::setMetricConvergencyValue(double value)
{
    metricConvergency = value;
}

double PTManager::getMetricConvergencyValue() const
{
    return metricConvergency;
}

void PTManager::setAngularConvergencyValue(double value)
{
    angularConvergency = value;
}

double PTManager::getAngularConvergencyValue() const
{
    return angularConvergency;
}

void PTManager::setMaxIteration(int iterations)
{
    maxIterations = iterations;
}

int PTManager::getMaxIteration() const
{
    return maxIterations;
}

void PTManager::reloadPointsCoordinates()
{
    int numPoints = listAllPoints.size();

    for (int i = 0; i < numPoints; i++) {
        Point* pnt = efotoManager->instancePoint(spareENH.getInt(i + 1, 4));
        double x = spareENH.get(i + 1, 1);
        double y = spareENH.get(i + 1, 2);
        double z = spareENH.get(i + 1, 3);
        pnt->getObjectCoordinate().setX(x);
        pnt->getObjectCoordinate().setY(y);
        pnt->getObjectCoordinate().setZ(z);
    }
}

std::deque<std::string> PTManager::getPointsWithLesserThanOverlap(
    size_t overlap)
{
    std::deque<std::string> ids;
    size_t numPoints = listAllPoints.size();

    for (size_t i = 0; i < numPoints; i++) {
        Point* pnt = listAllPoints.at(i);

        if(pnt->getType() == Point::PHOTOGRAMMETRIC)
            if (getImagesAppearances(pnt->getId()).size() < overlap) {
                ids.push_back(pnt->getPointId());
            }
    }

    return ids;
}

int PTManager::createNewPoint()
{
    std::string text = "";
    int currentItemId = efotoManager->getFreePointId();
    text += "<point key=\"" + Conversion::intToString(currentItemId) +
            "\" type=\"photogrammetric\">\n";
    text += "<pointId>PF" + Conversion::intToString(currentItemId) + "</pointId>\n";
    text += "<description>Photogrammetric point created in Phototriangulation</description>\n";
    text += "<spatialCoordinates uom=\"#m\">\n";
    text += "<gml:pos>0.0 0.0 0.0</gml:pos>\n";
    text += "<sigma>Not Available</sigma>\n";
    text += "</spatialCoordinates>\n";
    text += "</point>";
    EDomElement xml(efotoManager->xmlGetData());
    xml.addChildAtTagName("points", text);
    efotoManager->xmlSetData(xml.getContent());
    listAllPoints.push_back(efotoManager->instancePoint(currentItemId));
    return currentItemId;
}

void PTManager::runAutoMeasurement()
{
    bool notChanged = true;
    size_t nImgs = listAllImages.size();

    for (size_t i = 0; nImgs > i; i++) {
        Image* img = listAllImages.at(i);

        if (img->getGnssType() == "Initial" || img->getGnssType() == "Fixed")
            if (img->getInsType() == "Initial" || img->getInsType() == "Fixed") {
                Matrix Xa(6, 1);
                Xa.set(1, 1, img->getGnssX0());
                Xa.set(2, 1, img->getGnssY0());
                Xa.set(3, 1, img->getGnssZ0());
                Xa.set(4, 1, img->getInsPhi());
                Xa.set(5, 1, img->getInsOmega());
                Xa.set(6, 1, img->getInsKappa());
                ExteriorOrientation newEO;
                newEO.setXa(Xa);
                img->setEO(&newEO);
                ProjectiveRay pr(img);
                size_t nPnts = listAllPoints.size();

                for (size_t j = 0; nPnts > j; j++) {
                    Point* pnt = listAllPoints.at(j);

                    if (!pnt->hasImageCoordinate(img->getId())) {
                        ImageSpaceCoordinate pos = pr.objectToImage(pnt->getObjectCoordinate(), false);

                        if (pos.getCol() > 0.1 && pos.getCol() < img->getWidth() && pos.getLin() > 0.1
                                && pos.getLin() < img->getHeight()) {
                            connectPointInImage(pnt->getId(), img->getId());
                            pnt->putImageCoordinate(pos);
                            notChanged = false;
                        }
                    }
                }
            }
    }

    setMarksSavedState(notChanged);
}

void PTManager::disconnectPointAndImage(int pointKey, int imageKey)
{
    Image* image = efotoManager->image(imageKey);

    if (image != NULL) {
        Point* point = image->getPoint(pointKey);

        if (point != NULL) {
            point->removeImage(imageKey);
            point->deleteImageCoordinate(imageKey);
            image->removePoint(pointKey);
            setMarksSavedState(false);
        }
    }
}

void PTManager::connectPointInImage(int pointKey, int imageKey)
{
    Point* point = efotoManager->instancePoint(pointKey);
    Image* image = efotoManager->instanceImage(imageKey);
    point->putImage(image);
    image->putPoint(point);
    image->sortPoints();
}

std::string PTManager::getCoordinatesGeodesic()
{
    size_t rows = listAllPoints.size();
    std::string result;
    result += "\t\t\tCoordinates in geodesic\n";
    result += "PointId\t\tPhi\t\t\tLambda\n";
    int hemi = getTerrainLatHemisphere();
    int zone = getTerrainZone();
    GeoSystem sys = getTerrainDatum();

    for (size_t i = 0; i < rows; i++) {
        Point* pnt = listAllPoints.at(i);
        ObjectSpaceCoordinate temp = pnt->getObjectCoordinate();
        double E = temp.getX();
        double N = temp.getY();
        Matrix gc = ConvertionsSystems::utmToGeo(E, N, zone, hemi, sys);
        result += pnt->getPointId();
        result += "\t\t";
        result += Conversion::doubleToString(gc.get(1, 1) * 180 / M_PI, 5);
        result += "\t\t";
        result += Conversion::doubleToString(gc.get(1, 2) * 180 / M_PI, 5);
        result += "\n";
    }

    return result;
}

std::string PTManager::getCoordinatesTopocentric()
{
    int rows = listAllPoints.size();
    std::string result;
    result += "\t\t\tCoordinates in Local Topocentric\n";
    result += "PointId\t\tE\t\t\t\tN\t\t\t\tH\n";
    int hemi = getTerrainLatHemisphere();
    int zone = getTerrainZone();
    double latterrain = getPhiTerrain();
    double longterrain = getLambdaTerrain();
    double h0 = getAltitudeMedia();
    GeoSystem sys = getTerrainDatum();

    for (int i = 0; i < rows; i++) {
        Point* pnt = listAllPoints.at(i);
        ObjectSpaceCoordinate temp = pnt->getObjectCoordinate();
        double E = temp.getX();
        double N = temp.getY();
        double H = temp.getZ();
        Matrix gc = ConvertionsSystems::utmToNunes(E, N, H, zone, hemi, latterrain,
                    longterrain, h0, sys);
        result += pnt->getPointId();
        result += "\t\t";
        result += Conversion::doubleToString(gc.get(1, 1), 3);
        result += "\t\t\t";
        result += Conversion::doubleToString(gc.get(1, 2), 3);
        result += "\t\t\t";
        result += Conversion::doubleToString(gc.get(1, 3), 3);
        result += "\n";
    }

    return result;
}



Matrix PTManager::getDigitalCoordinate(int imageKey, int pointKey)
{
    Matrix result(1, 2);
    Point* pnt = efotoManager->instancePoint(pointKey);
    ImageSpaceCoordinate coord = pnt->getImageCoordinate(imageKey);
    result.set(1, 1, coord.getCol());
    result.set(1, 2, coord.getLin());
    return result;
}

bool PTManager::hasAllImagesInitialValues()
{
    for (size_t i = 0; i < listAllImages.size(); i++) {
        Image* img = listAllImages.at(i);

        if(!img->isInsAvailable()) {
            return false;
        }

        if(!img->isGnssAvailable()) {
            return false;
        }

        if(img->getInsType() != "Initial") {
            return false;
        }

        if(img->getGnssType() != "Initial") {
            return false;
        }
    }

    return true;
}

double PTManager::getPhiTerrain()
{
    Dms lat(myTerrain->getCentralCoordLat());
    EDomElement coordinates(efotoManager->getXml("terrain"));
    coordinates = coordinates.elementByTagName("Lat");

    if (getTerrainLatHemisphere() < 0) {
        lat.setSignal(true);
    }

    return lat.dmsToRadiano();
}

double PTManager::getLambdaTerrain()
{
    Dms longi(myTerrain->getCentralCoordLong());
    EDomElement coordinates(efotoManager->getXml("terrain"));
    coordinates = coordinates.elementByTagName("Long");

    if (getTerrainLongHemisphere() < 0) { //coordinates.attribute("direction")=="W")
        longi.setSignal(true);
    }

    return longi.dmsToRadiano();
}

double PTManager::getAltitudeMedia() const
{
    return myTerrain->getMeanAltitude();
}

void PTManager::convertToNunes(std::deque<Point*> points, GeoSystem sys,
                               int hemi, int zona)
{
    int rows = points.size();

    for (int i = 0; i < rows; i++) {
        //N0=-5.85
        Point* pnt = points.at(i);
        ObjectSpaceCoordinate temp = pnt->getObjectCoordinate();
        double E = temp.getX();
        double N = temp.getY();
        double H = temp.getZ();
        Matrix gc = ConvertionsSystems::utmToNunes(E, N, H, zona, hemi, getPhiTerrain(),
                    getLambdaTerrain(), getAltitudeMedia(), sys);
        temp.setX(gc.get(1, 1));
        temp.setY(gc.get(1, 2));
        temp.setZ(gc.get(1, 3));
        pnt->setObjectCoordinate(temp);
    }
}

void PTManager::convertToUTM(std::deque<Point*> points, GeoSystem sys)
{
    int rows = points.size();
    double phi0 = getPhiTerrain();
    double lambda0 = getLambdaTerrain();
    double h0 = getAltitudeMedia();

    for (int i = 0; i < rows; i++) {
        Point* pnt = points.at(i);
        ObjectSpaceCoordinate temp = pnt->getObjectCoordinate();
        double X = temp.getX();
        double Y = temp.getY();
        double Z = temp.getZ();
        Matrix gc = ConvertionsSystems::nunesToUtm(X, Y, Z, phi0, lambda0, h0, sys);
        temp.setX(gc.get(1, 1));
        temp.setY(gc.get(1, 2));
        temp.setZ(gc.get(1, 3));
        pnt->setObjectCoordinate(temp);
    }

    // convertendo os parametros da OE
    //Matrix oes=getMatrixOE();

    for (size_t i = 1; i <= AFP.getRows(); i++) {
        double X0 = AFP.get(i, 4);
        double Y0 = AFP.get(i, 5);
        double Z0 = AFP.get(i, 6);
        Matrix temp2 = ConvertionsSystems::nunesToUtm(X0, Y0, Z0, phi0, lambda0, h0,
                       sys);
        AFP.set(i, 4, temp2.get(1, 1));
        AFP.set(i, 5, temp2.get(1, 2));
        AFP.set(i, 6, temp2.get(1, 3));
    }
}

bool PTManager::hasEODone()
{
    EDomElement phototriXml(efotoManager->getXml("phototriangulation"));
    return phototriXml.elementByTagName("converged").toBool();
}

Matrix PTManager::eoParametersFromXml(bool withIds)
{
    EDomElement tempXa;
    EDomElement exteriorXml(efotoManager->getXml("exteriorOrientation"));
    std::deque<EDomElement> oesXml = exteriorXml.elementsByTagName("imageEO");
    int numEO = oesXml.size();
    Matrix oesMatrix;

    if (withIds) {
        oesMatrix.resize(numEO, 7);
    } else {
        oesMatrix.resize(numEO, 6);
    }

    for (int i = 0; i < numEO; i++) {
        int imagekey = Conversion::stringToInt(oesXml.at(i).attribute("image_key"));
        tempXa.setContent(oesXml.at(i).elementByTagName("Xa").getContent());
        double x0 = Conversion::stringToDouble(
                        tempXa.elementByTagName("X0").toString());
        double y0 = Conversion::stringToDouble(
                        tempXa.elementByTagName("Y0").toString());
        double z0 = Conversion::stringToDouble(
                        tempXa.elementByTagName("Z0").toString());
        double omega = Conversion::stringToDouble(
                           tempXa.elementByTagName("omega").toString()) * 180 / M_PI;
        double phi = Conversion::stringToDouble(
                         tempXa.elementByTagName("phi").toString()) * 180 / M_PI;
        double kappa = Conversion::stringToDouble(
                           tempXa.elementByTagName("kappa").toString()) * 180 / M_PI;

        if (withIds) {
            oesMatrix.set(i + 1, 1, imagekey);
            oesMatrix.set(i + 1, 2, omega);
            oesMatrix.set(i + 1, 3, phi);
            oesMatrix.set(i + 1, 4, kappa);
            oesMatrix.set(i + 1, 5, x0);
            oesMatrix.set(i + 1, 6, y0);
            oesMatrix.set(i + 1, 7, z0);
        } else {
            oesMatrix.set(i + 1, 1, omega);
            oesMatrix.set(i + 1, 2, phi);
            oesMatrix.set(i + 1, 3, kappa);
            oesMatrix.set(i + 1, 4, x0);
            oesMatrix.set(i + 1, 5, y0);
            oesMatrix.set(i + 1, 6, z0);
        }
    }

    return oesMatrix;
}

double PTManager::getRMSE()
{
    return pt->calculateRMSE();
}

int PTManager::getPreviousTotalIterationsXml()
{
    EDomElement exteriorXml(efotoManager->getXml("phototriangulation"));
    return Conversion::stringToInt(
               exteriorXml.elementByTagName("iterations").toString());
}

bool PTManager::getPreviousConvergedXML()
{
    EDomElement exteriorXml(efotoManager->getXml("phototriangulation"));

    if (exteriorXml.elementByTagName("converged").toString() == "true") {
        return true;
    }

    return false;
}

double PTManager::getPreviousRmseXML()
{
    EDomElement exteriorXml(efotoManager->getXml("phototriangulation"));
    return exteriorXml.elementByTagName("rmse").toDouble();
}

int PTManager::getTerrainLatHemisphere()
{
    EDomElement terrain(efotoManager->getXml("terrain"));
    if (terrain.elementByTagName("Lat").attribute("direction") == "S")
        return -1;

    return 1;
}

int PTManager::getTerrainLongHemisphere()
{
    EDomElement terrain(efotoManager->getXml("terrain"));
    if (terrain.elementByTagName("Long").attribute("direction") == "W")
        return -1;

    return 1;
}

int PTManager::getTerrainZone() const
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

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
