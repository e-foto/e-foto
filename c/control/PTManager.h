/*Copyright 2002-2021 e-foto team (UERJ)
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

#ifndef PTMANAGER_H
#define PTMANAGER_H

#include "CommonDef.h"
#include "BundleAdjustment.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {


class PTUserInterface;
class EFotoManager;


class Point;
class Image;
class Sensor;
class Flight;
class Terrain;
class InteriorOrientation;

/**
  * \file PTManager.h
  * \class PTMannager
  * \brief Classe responsavel por gerenciar a interface e o calculo da
  * fototriangulação fornecendo metodos basicos a interface, e fazendo a
  * ligação entre o calculo e a interface
*/
class PTManager {
protected:
    EFotoManager* efotoManager;
    std::deque<Image*> listAllImages;
    std::deque<InteriorOrientation*> listOis;
    Sensor* mySensor;
    Flight* myFlight;
    Terrain* myTerrain;

    bool marksSaveState;
    bool started;
    bool status;
    bool localTopocentricMode;
    bool previousData;
    int maxIterations;
    double metricConvergency;
    double angularConvergency;
    PTUserInterface* myInterface;
    double flightScale;

    std::deque<Point*> listAllPoints;


    std::deque<Image*> listSelectedImages;
    std::deque<Point*> listSelectedPoints;

    BundleAdjustment* pt;
    Matrix ENH, spareENH, AFP, MVC, residuos;

public:
    /**
     * \brief Construtor completo da classe para gerenciamento total
     * \param newManager : Gerente geral do software e-foto
     * \param images : Lista com todos os objetos Image cadastrados no projeto atual
     * \param ois : Lista com todos os objetos InteriorOrientation, necessário para o
     * calculo da fototriangulação
     * \param sensor : Objeto Sensor, necessário para o calculo da fototriangulação
     * \param flight : Objeto Flight, necessário para o calculo da fototriangulação
     */
    explicit PTManager(EFotoManager* newManager, std::deque<Image*> images,
                       std::deque<InteriorOrientation*> ois, Sensor* sensor,
                       Flight* flight, Terrain*);

    /**
    * \brief Destrutor do objeto
    */
    ~PTManager();

    /**
    * \brief Metodo que retorna o nome de arquivo da imagem pelo seu respectiva key no projeto
    * \param imageKey : Key da imagem no projeto
    * \return string : Texto contendo o nome do arquivo da imagem
    */
    std::string getImagefile(int imageKey);

    /**
    * \brief Metodo que reune os parametros necessários para o calculo da fototriangulação
    * e o executa através da classe BundleAdjustment em seguida alterando os atributos
    * de classe com os resultados obtidos
    * \return bool : Informa se o calculo foi bem sucedido ou não;
    */
    bool calculatePT();

    /**
    * \brief Metodo que toma as medidas necessarias e dispara a execução da interface com
    * o usuário
    * \return bool : informa se a execuÃ§ao foi bem sucedida ou não
    */
    bool exec();

    /**
    * \brief Metodo que retorna o controle do software para o modulo de ProjectManager
    */
    void returnProject();

    /**
    * \brief Metodo que altera a matriz com os resultados das orientações exteriores
    * \param afp : Matriz com os parametros das orientações exteriores das imagens
    */
    void setMatrixAFP(Matrix afp);

    /**
    * \brief Metodo que retorna a matriz com os parametros das orientaçoes exteriores
    * \return Matrix : Matrix com os parametros, onde cada linha representa uma imagem e
    * a sequencia dos parametros e : omega, phi, kappa, X0, Y0, Z0
    * \attention Se o calculo da classe BundleAdjusment nao tiver sido disparado, o
    * retorna sera uma matriz 1x1, com o valor zero
    */
    Matrix getMatrixOE();

    /**
    * \brief Metodo que altera a matriz que corresponde aos valores ENH de todos os pontos
    * cadastrados no projeto, onde cada linha representa um ponto
    */
    void setENH();

    /**
    * \brief Metodo que retorna a matriz que corresponde aos valores ENH de todos os
    * pontos cadastrados no projeto, onde cada linha representa um ponto
    */
    Matrix getENH();

    /**
    * \brief Metodo que retorna a matriz dos residuos dos parametros das orientações
    * exteriores
    * \return Matrix : Matrix com os residuos parametros, onde cada linha representa
    * uma imagem e a sequencia dos parametros é :
    * delta omega,delta phi, delta kappa, delta X0, delta Y0, delta Z0
    */
    Matrix getMVC();

    /**
    * \brief Metodo que retorna as coordenadas digitais dos pontos contidos na
    * imagem especificada
    * \param imageFilename : nome do arquivo de imagem
    * \return Matrix : Matriz com as coordenadas digitais da imagem onde cada
    * linha é a cooordenada de um ponto na sequencia: coluna , linha
    * \attention Esse metodo pode falhar se o projeto tiver duas imagens em diretorios
    * diferentes com o mesmo nome de arquivo.
    */
    Matrix getColLin(std::string imageFilename);

    /**
    * \overload getColLin
    * \brief Metodo que retorna as coordenadas digitais dos pontos contidos na
    * imagem especificada
    * \param imageKey : key da imagem no projeto
    * \return Matrix : Matriz com as coordenadas digitais da imagem onde cada linha
    * é a cooordenada de um ponto na sequencia: coluna , linha
    * \attention Esse metodo é totalmente seguro, pois é baseado na key da imagem que é
    * unica independente do caminho
    */
    Matrix getColLin(int imageKey);

    /**
    * \brief Metodo que faz as ligações entre todas as imagens e seus respectivos pontos,
    * ou seja, vincula ponto a imagem e vice-versa
    * \return bool : Informa se a operaÃ§ao foi bem sucedida ou não
    */
    bool connectImagePoints();

    /**
    * \brief Metodo que altera a lista de todos pontos no projeto
    * \return
    */
    void setListPoint();

    /**
    * \brief Metodo auxiliar que retorna uma lista de key dos pontos contidos na imagem
    * especificada
    * \param imageFileName: Nome do arquivo de imagem, se uma string vazia for passada,
    * o retorno será uma lista com as key de todos os pontos no projeto
    * \return std::deque<string> : Lista com as keys dos pontos
    */
    std::deque<std::string> getStringKeysPoints(std::string imageFileName = "");

    /**
    * \brief Metodo auxiliar que retorna uma lista com os tipos dos pontos contidos na
    * imagem especificada
    * \param imageFileName: Nome do arquivo de imagem, se uma string vazia for passada,
    * o retorno será uma lista com os tipos de todos os pontos no projeto
    * \return std::deque<string> : Lista com os tipos dos pontos
    */
    std::deque<std::string> getStringTypePoints(std::string imageFileName = "");

    /**
    * \brief Metodo auxiliar que retorna uma lista dos ids dos pontos contidos na imagem
    * especificada
    * \param imageFileName  : Nome do arquivo de imagem, se uma string vazia for passada,
    * o retorno será uma lista com os ids de todos os pontos dependendo do \param cond1
    * \param cond1 : Indica a condiÃ§ao de seleÃ§ao dos pontos. Atualmente só existe a
    * condição "noCheckingPoint" que exclui os ids dos pontos de checking
    * \return std::deque<string> : Lista com os ids dos pontos
    */
    std::deque<std::string> getStringIdPoints(std::string imageFileName = "",
            std::string cond1 = "");

    /**
    * \brief Metodo que retorna uma lista contendo todos os nomes dos arquivos das
    * imagens cadastradas
    * \return std::deque<string> : Lista com os nomes dos arquivos de imagens
    */
    std::deque<std::string> getStringImages();

    /**
    * \brief Metodo que altera a lista de pontos selecionados para a execução do
    * calculo da fototriangulação
    * \param selectedPointsList : lista com os ids dos pontos selecionados
    */
    void selectPoints(std::deque<std::string> selectedPointsList);

    /**
    * \brief Metodo que altera a lista de imagens selecionadas para a execução do
    * calculo da fototriangulação
    * \param selectedImagesList : lista com os nomes dos arquivos de imagens selecionadas
    */
    void selectImages(std::deque<std::string> selectedImagesList);

    // metodo para dar um update nas coordenadas digitais do pont
    /**
    * \brief Metodo que atualiza as coordenadas digitais do ponto na imagem especificada
    * \param imageId : key da imagem em que se deseja atualizar as coordenadas
    * \param pointKey : key do ponto
    * \param col : Novo valor de coluna do ponto
    * \param lin : Novo valor de linha do ponto
    */
    void updateDigitalCoordinatesPoint(int imageId, int pointKey, double col,
                                       double lin);

    /**
    * \brief Metodo que retorna a key de uma imagem
    * \param imageFilename : Nome do arquivo de imagem
    * \return int : key da imagem
    */
    int getImageId(std::string imageFilename);

    /**
    * \brief Metodo que retorna o Id de um ponto
    * \param pointKey: Key do ponto no projeto
    * \return std::string : id do ponto
    */
    std::string getPointId(int pointKey);

    /**
    * \brief Metodo que retorna o objeto usado para o calculo da fototriangulação
    * \return BundleAdjustment : Objeto usado para calcular a fototriangulação
    * \attention Não use esse metodo antes de disparar o metodo de calculo
    */
    BundleAdjustment* getBundleAdjustment();

    /**
    * \brief Metodo usado para organizar a lista de pontos selecionados, de forma que todos os pontos de controle sempre fiquem antes dos pontos fotogramétricos
    */
    void sortPointsSelected();

    /**
    * \brief Metodo auxiliar para retornar as coordenadas ENH apenas dos pontos fotogramétricos que estiverem na lista de pontos selecionados
    * \return Matrix : Matriz com as coordenadas dos pontos fotogramétricos
    */
    Matrix getPhotogrammetricENH();

    /**
    * \brief Metodo que retorna os residuos das coordenadas ENH dos pontos fotogramétricos selecionados
    * \return Matrix : Matriz com os residuos das coordenadas ENH
    */
    Matrix getResiduoPhotogrammetric();

    /**
    * \brief Metodo auxiliar que retorna uma lista com os ids dos pontos fotogramétricos selecionados
    * \return std::deque<std::string> : Lista com os ids dos pontos fotogramétricos
    */
    std::deque<std::string> getSelectedPointIdPhotogrammetric();

    /**
    * \brief Metodo que retorna o caminho do arquivo da imagem especificado
    * \param fileName : Nome do arquivo de imagem
    * \return std::string : Caminho do arquivo da imagem SEM o nome do arquivo de imagem
    * \attention Esse metodo pode falhar se o projeto tiver duas imagens em diretorios diferentes com o mesmo nome de arquivo.
    */
    std::string getFilePath(std::string fileName);

    /**
    * \overload getFilePath
    * \brief Metodo que retorna o caminho do arquivo da imagem especificado
    * \param imageKey : key da imagem no projeto
    * \return std::string : Caminho do arquivo da imagem SEM o nome do arquivo de imagem
    * \attention Esse metodo é totalmente seguro, pois é baseado na key da imagem que é unica.
    */
    std::string getFilePath(int imageKey);

    /**
    * \brief Metodo responsavel por salvar, no xml corrente, tanto as marcaÃ§oes de linha e coluna como os resultados da fototriangulaÃ§ao
    */
    void saveResults();

    /**
    * \brief Metodo responsavel por salvar, no xml corrente, os resulados da fototriangulaÃ§ao
    */
    void saveBundleAdjustment();

    /**
    * \brief Metodo responsavel por salvar, no xml corrente, as maraÃ§oes de linha e coluna
    */
    void saveMarks();

    /**
    * \brief Metodo responsavel por salvar, no xml corrente, as imagens
    */
    void saveImages();

    /**
    * \brief Metodo para alterar o estado atual das marcas, isto e, se houve mudanÃ§a nos valores de linha e coluna de algum ponto
    * \param marksState : Informa se houve mudanÃ§as nas marcas
    */
    void setMarksSavedState(bool marksState);

    /**
    * \brief Metodo para retornar o estado atual das marcas
    * \return bool: Indica se houve mudanÃ§as
    */
    bool getMarksSavedState();

    /**
    * \brief Cria um texto em xml contendo tudo que for pertinente a fototriangulaÃ§ao
    * \return std::string : Texto em xml com os resultados entre outros valores da fototriangulaÃ§ao
    */
    std::string createOESXml();

    /**
    * \brief Metodo que retorna uma lista dos nomes dos arquivos de imagens em que o referido ponto aparece
    * \param pointKey : key do ponto desejado
    * \return std::deque<std::string> : Lista com os nomes dos arquivos de imagens
    */
    std::deque<std::string> getImagesAppearances(int pointKey);

    /**
    * \brief Metodo que altera o valor do angulo(kappa0) da direÃ§ao de voo na imagem
    * \param imagefile : Nome do arquivo de imagem
    * \param flightDirection : Valor da direÃ§ao de voo em radianos
    */
    void setImageFlightDirection(std::string imageFile, double flightDirection);

    /**
    * \overload setImageFlightDirection
    * \brief Metodo que altera o valor do angulo(kappa0) da direÃ§ao de voo na imagem
    * \param imageKey : Key da imagem
    * \param flightDirection : Valor da direÃ§ao de voo em radianos
    */
    void setImageFlightDirection(int imageKey, double flightDirection);

    /**
    * \brief Metodo que retorna uma string dos pontos e imagens que foram
    * processadas numa fototriangulaçao, num formato xml que o GoogleEarth interprete
    * \param fileName : Nome do arquivo em que o xml será salvo
    * \return string : xml(*.kml) dos pontos e imagens
    */
    std::string exportBlockTokml(std::string fileName, bool fromXML = false);

    /**
    * \brief Metodo auxiliar que retorna uma string correspondente a um ponto xml interpretavel pelo GoogleEarth
    * \param       pnt    : Objeto Point
    * \param      zona    : Zona na qual o ponto se encontra
    * \param hemiLatitude : Valor inteiro que representa o hemisfério SUL=-1 ou hemisfério NORTE=+1
    * \param      sys     : Sistema geodesico
    * \param   pointType  : Texto que informa que tipo de ponto é, controle fotogramétrico ou de checking
    * \return    std::string   : Texto com o xml correspondente a um lugar(placemark) no GoogleEarth
    */
    std::string pointToKml(Point* pnt, int zona, int hemiLatitude, GeoSystem sys,
                           std::string pointType);

    /**
    * \brief Metodo que altera o valor maximos de iterações de calculo da fototriangulação
    * \param iterations : Numero máximo de iterações
    */
    void setMaxIteration(int iterations);

    /**
    * \brief Metodo que retorna o valor máximo de iterações para o calculo da fototrianguulação
    * \return int : Número máximo de iterações
    */
    int getMaxIteration();

    /**
    * \brief Metodo que altera o valor da convergencia dos parametros X0 Y0 e Z0 de cada imagem.
    * \param value : Valor da convergencia em metros
    */
    void setMetricConvergencyValue(double value);

    /**
    * \brief Metodo que o valor da convergencia dos parametros X0 Y0 e Z0 de cada imagem.
    * \return double : Valor da convergencia dos parametros X0 Y0 e Z0 de cada imagem.
    */
    double getMetricConvergencyValue();

    /**
    * \brief Metodo que altera o valor da convergencia dos parametros omega phi e kappa de cada imagem.
    * \param value : Valor da convergencia em radianos
    */
    void setAngularConvergencyValue(double value);

    /**
    * \brief Metodo que altera o valor da convergencia dos parametros omega phi e kappa de cada imagem.
    * \return double : Valor da convergencia em radianos
    */
    double getAngularConvergencyValue();

    /**
    * \brief Metodo auxiliar que recarrega/reseta os valores das coordenadas ENH dos objetos Point
    */
    void reloadPointsCoordinates();

    /**
    * \brief Metodo auxiliar que retorna uma lista dos ids dos pontos fotogramétricos com uma sobreposiçao(numero de imagens em que o ponto aparece) menor que a especificada
    * \param overlap: Valor inteiro no qual o valor de superposiçõ es não alcance
    * \return std::deque<std::string> : lista com os Ids dos pontos
    */
    std::deque<std::string> getPointsWithLesserThanOverlap(size_t overlap);

    /**
    * \brief Metodo auxiliar que cria e instancia um novo objeto Point e adiciona ao xml do projeto
    * \return int : Retorna a key do ponto criado
    */
    int createNewPoint();

    /**
    * \brief Metodo que conecta vincula um ponto a uma imagem e vice-versa
    * \param pointKey: Key do ponto no projeto
    * \param imageKey: Key da imagem no projeto
    */
    void connectPointInImage(int pointKey, int imageKey);

    /**
    * \brief Metodo que desconecta um ponto e uma imagem (remove a medição do ponto)
    * \param pointKey: Key do ponto no projeto
    * \param imageKey: Key da imagem no projeto
    */
    void disconnectPointAndImage(int pointKey, int imageKey);

    /**
    * \brief Metodo que realiza medição automatica de pontos em imagens com GNSS e INS. Ele preserva mediçoes preexistentes.
    */
    void runAutoMeasurement();

    /**
    * \brief Metodo que retorna o valor de linha e coluna do ponto na imagem
    * \param pointKey : key do ponto
    * \param imageKey : key da imagem
    * \return Matrix : Matrix[1x2] contendo os valores coluna e linha do ponto na imagem
    */
    Matrix getDigitalCoordinate(int imageKey, int  pointKey);

    /**
    * \brief Metodo que checa se o usuario colocou para todas as imagens todos os parametros da orinentação exterior como valores iniciais
    * \return bool : Informa se todas as imagens tem valores iniciais para o calculo dos parametros de orientaçao exterior
    */
    bool hasAllImagesInitialValues();

    /**
    * \brief Metodo que retorna o valor da latitude, em radianos, do terreno informado pelo usuario no projectmanager
    * \return double : Latitude em radianos
    */
    double getPhiTerrain();

    /**
    * \brief Metodo que retorna o valor da longitude, em radianos, do terreno informado pelo usuario no projectmanager
    * \return double : Longitude em radianos
    */
    double getLambdaTerrain();

    /**
    * \brief Metodo que retorna o valor da altitude media, em metros, do terreno informado pelo usuario no projectmanager
    * \return double : Altitude media em metros
    */
    double getAltitudeMedia();


    GeoSystem getTerrainDatum();
    int getTerrainLatHemisphere();
    int getTerrainLongHemisphere();
    int getTerrainZone();


    /**
    * \brief Metodo que altera o sistema de coordenadas dos pontos para um sistema local topocentrico para ajuste da fototriangulacao
    * \param status: Ativa ou desativa o modo de calculo topocentrico
    */
    void setLocalTopocentricMode(bool status);

    /**
    * \brief Metodo que converte todos as coordenadas para geocentricas
    * \param points : lista dos pontos contendo as coordenadas
    * \param sys : Sistema geodesico no qual as coordenadas estão
    * \param hemi : inteiro informando em qual hemisferio estão as coordenadas;
    * i.e. -1=Sul +1=Norte
    * \param zona : Zona na qual as coordenadas estão
    * \return Matrix: Matriz com as coordenadas convertidas para geocentricas
    */
    void convertToNunes(std::deque<Point*> points,
                        GeoSystem sys,
                        int hemi,
                        int zona);

    /**
    * \brief Metodo que converte todos as coordenadas para UTM
    * \param points : lista dos pontos contendo as coordenadas
    * \param sys : Sistema geodesico no qual as coordenadas deverão estar
    * \return Matrix: Matriz com as coordenadas convertidas para UTM
    */
    void convertToUTM(std::deque<Point*> points, GeoSystem sys);

    /** \brief Metodo que checa se alguma orientacao exterior foi feita
    * \return bool : Informa se há ou nao orientacao exterior feita
    */
    bool hasEODone();

    /**
     * \brief Metodo que retorna uma matriz com os parametros da orientacao exterior, onde cada linha contem os parametros.
     * \return Matrix : Matriz com os parametros da orientacao exterior na seguinte ordem: image key, X0, Y0, Z0, omega, phi, kappa
    */
    Matrix eoParametersFromXml(bool withIds = true);

    /**
    * \brief Metodo que retorna o valor do RMSE do bloco
    * \return double: Valor do RMSE
    */
    double getRMSE();

    /**
    *	\brief Metodo que retorna o numero de iteracoes salvo no xml anteriormente
    *	\return int : Numero de iteracoes
    */
    int getPreviousTotalIterationsXml();

    /**
    *	\brief Metodo que informa se a fototriangulacao salva anteriormente convergiu ou nao
    *	\return bool : true: convergiu, false: nao convergiu
    */
    bool getPreviousConvergedXML();

    /**
    *	\brief Metodo que retorna o valor do RMSE em metros salvo no xml anteriormente
    *	\return double: Valor do RMSE em metros
    */
    double getPreviousRmseXML();

    /**
    *	\brief
    *	\return std::string
    */
    std::string getPhotoTriXml();

    /**
    *	\brief
    *	\return std::string
    */
    std::string getUsedPointsXml();

    /**
    *	\brief
    *	\return std::string
    */
    std::string getUsedImagesXml();

    std::string getCoordinatesGeodesic();
    std::string getCoordinatesTopocentric();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // PTMANAGER_H
