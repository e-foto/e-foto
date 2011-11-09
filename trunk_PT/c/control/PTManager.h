#ifndef PTMANAGER_H
#define PTMANAGER_H

#include "EObject.h"
#include "BundleAdjustment.h"
#include "Matrix.h"
#include "ConvertionsSystems.h"
#include <QStringList>
#include <deque>


class PTUserInterface;
class EFotoManager;


class Point;
class Image;
class Sensor;
class Flight;
class InteriorOrientation;

/**
  * \file PTManager.h
  * \class PTMannager
  * \brief Classe responsavel por gerenciar a interface e o calculo da fototriangulação fornecendo metodos basicos a interface, e fazendo a ligação entre o calculo e a interface
  * \author Paulo André Batista Pupim
*/
class PTManager
{
protected:
	bool started;
	bool status;
	bool marksSaveState;
	bool previousData;

	int maxIterations;
	double metricConvergency;
	double angularConvergency;

	EFotoManager* efotoManager;
	PTUserInterface* myInterface;

	Sensor *mySensor;
	Flight *myFlight;
	double flightScale;
	/***/
	deque<Image*> listAllImages;
	deque<Point*> listAllPoints;
	deque<InteriorOrientation*> listOis;

	deque<Image*> listSelectedImages;
	deque<Point*> listSelectedPoints;

	BundleAdjustment* pt;
	Matrix ENH,spareENH,AFP,MVC,residuos;

public:
    /**
    * \brief Construtor basico da classe
    * \attention Não use-o em sua totalidade
    */
    PTManager();

    /**
    * \brief Construtor completo da classe para gerenciamento total
    * \param newManager : Gerente geral do software e-foto
    * \param images : Lista com todos os objetos Image cadastrados no projeto atual
    * \param ois : Lista com todos os objetos InteriorOrientation, necessário para o calculo da fototriangulação
    * \param sensor : Objeto Sensor, necessário para o calculo da fototriangulação
    * \param flight : Objeto Flight, necessário para o calculo da fototriangulação
    */
    PTManager(EFotoManager* newManager, deque<Image*> images, deque<InteriorOrientation*> ois, Sensor* sensor,Flight* flight);

    /**
     * \brief Destrutor do objeto
    */
    ~PTManager();

    /**
    * \brief Metodo que retorna o nome de arquivo da imagem pelo seu respectiva key no projeto
    * \param imageId : Key da imagem no projeto
    * \return string : Texto contendo o nome do arquivo da imagem
    */
    string getImagefile(int imageId);

    /**
    * \brief Metodo que altera a interface com o usuário
    * \param newInterface : Nova intefrace com o usuário
    * \attention Atualmente só existe a interface com o usuário utilizando o QT4
    */
    void setInterface(PTUserInterface* newInterface);

    /**
    * \brief Metodo que retorna um ponteiro para a interface com o usuário atual
    * \return PTUserInterface : interface com o usuário
    */
    PTUserInterface* getInterface();

    /**
    * \brief Metodo que reune os parametros necessários para o calculo da fototriangulação e o executa através da classe BundleAdjustment em seguida alterando os atributos de classe com os resultados obtidos
    * \return bool : Informa se o calculo foi bem sucedido ou não;
    */
    bool calculatePT();

    /**
    * \brief Metodo que toma as medidas necessarias e dispara a execução da interface com o usuário
    * \return bool : informa se a execuçao foi bem sucedida ou não
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
    * \brief Metodo que retorna a matriz com os parametros das orientações exteriores
    * \return Matrix : Matrix com os parametros, onde cada linha representa uma imagem e a sequencia dos parametros é : omega, phi, kappa, X0, Y0, Z0
    */
    Matrix getMatrixOE();

    /**
    * \brief Metodo que altera a matriz que corresponde aos valores ENH de todos os pontos cadastrados no projeto, onde cada linha representa um ponto
    */
    void setENH();

    /**
    * \brief Metodo que retorna a matriz que corresponde aos valores ENH de todos os pontos cadastrados no projeto, onde cada linha representa um ponto
    */
    Matrix getENH();

    /**
    * \brief Metodo que retorna a matriz dos residuos dos parametros das orientações exteriores
    * \return Matrix : Matrix com os residuos parametros, onde cada linha representa uma imagem e a sequencia dos parametros é : delta omega,delta phi, delta kappa, delta X0, delta Y0, delta Z0
    */
    Matrix getMVC();

    /**
    * \brief Metodo que retorna as coordenadas digitais dos pontos contidos na imagem especificada
    * \param imageFilename : nome do arquivo de imagem
    * \return Matrix : Matriz com as coordenadas digitais da imagem onde cada linha é a cooordenada de um ponto na sequencia: coluna , linha
    * \attention Esse metodo pode falhar se o projeto tiver duas imagens em diretorios diferentes com o mesmo nome de arquivo.
    */
    Matrix getColLin(string imageFilename);

    /**
    * \overload getColLin
    * \brief Metodo que retorna as coordenadas digitais dos pontos contidos na imagem especificada
    * \param imageKey : key da imagem no projeto
    * \return Matrix : Matriz com as coordenadas digitais da imagem onde cada linha é a cooordenada de um ponto na sequencia: coluna , linha
    * \attention Esse metodo é totalmente seguro, pois é baseado na key da imagem que é unica independente do caminho
    */
    Matrix getColLin(int imageKey);

    /**
    * \brief Metodo que faz as ligações entre todas as imagens e seus respectivos pontos, ou seja, vincula ponto a imagem e vice-versa
    * \return bool : Informa se a operaçao foi bem sucedida ou não
    */
    bool connectImagePoints();

    /**
    * \brief Metodo que altera a lista de todos pontos no projeto
    * \return
    */
    void setListPoint();

    /**
    * \brief Metodo auxiliar que retorna uma lista de key dos pontos contidos na imagem especificada
    * \param imageFileName: Nome do arquivo de imagem, se uma string vazia for passada, o retorno será uma lista com as key de todos os pontos no projeto
    * \return deque<string> : Lista com as keys dos pontos
    */
    deque<string> getStringKeysPoints(string imageFileName="");

    /**
    * \brief Metodo auxiliar que retorna uma lista com os tipos dos pontos contidos na imagem especificada
    * \param imageFileName: Nome do arquivo de imagem, se uma string vazia for passada, o retorno será uma lista com os tipos de todos os pontos no projeto
    * \return deque<string> : Lista com os tipos dos pontos
    */
    deque<string> getStringTypePoints(string imageFileName="");

    /**
    * \brief Metodo auxiliar que retorna uma lista dos ids dos pontos contidos na imagem especificada
    * \param imageFileName  : Nome do arquivo de imagem, se uma string vazia for passada, o retorno será uma lista com os ids de todos os pontos dependendo do \param cond1
    * \param cond1 : Indica a condiçao de seleçao dos pontos. Atualmente só existe a condição "noCheckingPoint" que exclui os ids dos pontos de checking
    * \return deque<string> : Lista com os ids dos pontos
    */
    deque<string> getStringIdPoints(string imageFileName="",string cond1="");

    /**
    * \brief Metodo que retorna uma lista contendo todos os nomes dos arquivos das imagens cadastradas
    * \return deque<string> : Lista com os nomes dos arquivos de imagens
    */
    deque<string> getStringImages();

    /**
    * \brief Metodo que altera a lista de pontos selecionados para a execução do calculo da fototriangulação
    * \param selectedPointsList : lista com os ids dos pontos selecionados
    */
    void selectPoints(deque<string> selectedPointsList);

    /**
    * \brief Metodo que altera a lista de imagens selecionadas para a execução do calculo da fototriangulação
    * \param selectedImagesList : lista com os nomes dos arquivos de imagens selecionadas
    */
    void selectImages(deque<string> selectedImagesList);

    // metodo para dar um update nas coordenadas digitais do pont
    /**
    * \brief Metodo que atualiza as coordenadas digitais do ponto na imagem especificada
    * \param imageId : key da imagem em que se deseja atualizar as coordenadas
    * \param pointKey : key do ponto
    * \param col : Novo valor de coluna do ponto
    * \param lin : Novo valor de linha do ponto
    */
    void updateDigitalCoordinatesPoint(int imageId,int pointKey,int col, int lin);

    /**
    * \brief Metodo que retorna a key de uma imagem
    * \param imageFilename : Nome do arquivo de imagem
    * \return int : key da imagem
    */
    int getImageId(string imageFilename);

    /**
    * \brief Metodo que retorna o Id de um ponto
    * \param pointKey: Key do ponto no projeto
    * \return string : id do ponto
    */
    string getPointId(int pointKey);

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
    * \return deque<string> : Lista com os ids dos pontos fotogramétricos
    */
    deque<string> getSelectedPointIdPhotogrammetric();

    /**
    * \brief Metodo que retorna o caminho do arquivo da imagem especificado
    * \param fileName : Nome do arquivo de imagem
    * \return string : Caminho do arquivo da imagem SEM o nome do arquivo de imagem
    * \attention Esse metodo pode falhar se o projeto tiver duas imagens em diretorios diferentes com o mesmo nome de arquivo.
    */
    string getFilePath(string fileName);

    /**
    * \overload getFilePath
    * \brief Metodo que retorna o caminho do arquivo da imagem especificado
    * \param imageKey : key da imagem no projeto
    * \return string : Caminho do arquivo da imagem SEM o nome do arquivo de imagem
    * \attention Esse metodo é totalmente seguro, pois é baseado na key da imagem que é unica.
    */
    string getFilePath(int imageKey);

	/**
	* \brief Metodo responsavel por salvar, no xml corrente, tanto as marcaçoes de linha e coluna como os resultados da fototriangulaçao
	*/
    void saveResults();

	/**
	* \brief Metodo responsavel por salvar, no xml corrente, os resulados da fototriangulaçao
	*/
    void saveBundleAdjustment();

	/**
	* \brief Metodo responsavel por salvar, no xml corrente, as maraçoes de linha e coluna
	*/
    void saveMarks();

	/**
	* \brief Metodo para alterar o estado atual das marcas, isto e, se houve mudança nos valores de linha e coluna de algum ponto
	* \param marksState : Informa se houve mudanças nas marcas
	*/
    void setMarksSavedState(bool marksState);

	/**
	* \brief Metodo para retornar o estado atual das marcas
	* \return bool: Indica se houve mudanças
	*/
    bool getMarksSavedState();

	/**
	* \brief Cria um texto em xml contendo tudo que for pertinente a fototriangulaçao
	* \return string : Texto em xml com os resultados entre outros valores da fototriangulaçao
	*/
	string createBundleAdjustmentXml();

	/**
	* \brief Metodo que retorna uma lista dos nomes dos arquivos de imagens em que o referido ponto aparece
	* \param pointKey : key do ponto desejado
	* \return deque<String> : Lista com os nomes dos arquivos de imagens
	*/
    deque<string> getImagesAppearances(int pointKey);

	/**
	* \brief Carrega os dados da fototriangulaçao, se houver, feita anteriormente
	* \param fotoTriData : Texto xml contendo os dados da fototriangulaçao
	* \attention Ainda em desenvolvimento
	*/
	void loadFotoTriData(string fotoTriData);

	/**
	* \brief Metodo que checa se foi feita uma fototriangulaçao anteriormente
	* \return bool : Informa se uma fototriangulaçao foi feita anteriormente
	*/
	bool hasPreviousData();

	/**
	* \brief Metodo que altera o valor do angulo(kappa0) da direçao de voo na imagem
	* \param imagefile : Nome do arquivo de imagem
	* \param flightDirection : Valor da direçao de voo em radianos
	*/
    void setImageFlightDirection(string imageFile,double flightDirection);

	/**
	* \overload setImageFlightDirection
	* \brief Metodo que altera o valor do angulo(kappa0) da direçao de voo na imagem
	* \param imageKey : Key da imagem
	* \param flightDirection : Valor da direçao de voo em radianos
	*/
	void setImageFlightDirection(int imageKey,double flightDirection);

	/**
	* \brief Metodo que retorna o valor da superposiçao longitudinal do voo ao qual a imagem pertence.
	* \param imagefile : Nome do arquivo da imagem
	* \return double : valor da superposiçao em porcentagem
	*/
    double getLongitudinalOverlap(string imageFile);

	/**
	* \overload getLongitudinalOverlap
	* \brief Metodo que retorna o valor da superposiçao longitudinal do voo ao qual a imagem pertence.
	* \param imageKey : Key da imagem
	* \return double : valor da superposiçao em porcentagem
	*/
	double getLongitudinalOverlap(int imageKey);

	/**
	* \brief Metodo que retorna o valor do angulo(kappa0) da direçao de voo
	* \param imageFile : Nome do arquivo de imagem
	* \return double : Valor do angulo da direçao de voo em radianos
	*/
	double getImageFlightDirection(string imageFile);

	/**
	* \brief Metodo que retorna o valor do angulo(kappa0) da direçao de voo
	* \param imageKey : Key da imagem
	* \return double : Valor do angulo da direçao de voo em radianos
	*/
	double getImageFlightDirection(int imageKey);

	/**
	* \brief Metodo que informa se o ponto esta disponivel
	* \param
	* \return
	*/
	bool isAvailablePoint(int imageId, int pointKey);

	/**
	* \brief Metodo que retorna o Id de um ponto
	* \param pointKey: Key do ponto no projeto
	* \return string : id do ponto
	*/
    string exportBlockTokml(string fileName);

	/**
	* \brief
	* \param
	* \return
	*/
    string pointToKml(Point *pnt, int zona,GeoSystem sys ,char hemiLatitude,string pointType);

	/**
	* \brief
	* \return
	*/
	double getFlightScale();

	/**
	* \brief
	* \param
	*/
    void setMaxIteration(int iterations);

	/**
	* \brief
	* \return
	*/
    int getMaxIteration();

	/**
	* \brief
	* \param
	*/
    void setMetricConvergencyValue(double value);

	/**
	* \brief
	* \return
	*/
	double getMetricConvergencyValue();

	/**
	* \brief
	* \param
	*/
    void setAngularConvergencyValue(double value);

	/**
	* \brief Metodo que retorna o Id de um ponto
	* \param pointKey: Key do ponto no projeto
	* \return string : id do ponto
	*/
	double getAngularConvergencyValue();

	/**
	* \brief Metodo que retorna o Id de um ponto
	* \param pointKey: Key do ponto no projeto
	* \return string : id do ponto
	*/
    void reloadPointsCoordinates();

	/**
	* \brief Metodo que retorna o Id de um ponto
	* \param pointKey: Key do ponto no projeto
	* \return string : id do ponto
	*/
    void photogrammetricSort();

    // Retorna a coordenada aproximada EN de um ponto na imagem, desde de que a Fototri tenha sido executada com sucesso.
	/**
	* \brief Metodo que retorna o Id de um ponto
	* \param pointKey: Key do ponto no projeto
	* \return string : id do ponto
	*/
	int whereInSelectedImages(Image *img);

	/**
	* \brief Metodo que retorna o Id de um ponto
	* \param pointKey: Key do ponto no projeto
	* \return string : id do ponto
	*/
	int whereInImages(Image *img);

	/**
	* \brief Metodo que retorna o Id de um ponto
	* \param pointKey: Key do ponto no projeto
	* \return string : id do ponto
	*/
	Matrix digitalToEN(Image *img,int col, int row, Matrix oe);

	/**
	* \brief Metodo que retorna o Id de um ponto
	* \param pointKey: Key do ponto no projeto
	* \return string : id do ponto
	*/
    Matrix getPointResiduo(int pointKey);

	/**
	* \brief Metodo que retorna o Id de um ponto
	* \param pointKey: Key do ponto no projeto
	* \return string : id do ponto
	*/
    deque<string> getPointsWithLesserThanOverlap(int overlap);

	/**
	* \brief Metodo que retorna o Id de um ponto
	* \param pointKey: Key do ponto no projeto
	* \return string : id do ponto
	*/
    void createNewPoint();

	/**
	* \brief Metodo que retorna o Id de um ponto
	* \param pointKey: Key do ponto no projeto
	* \return string : id do ponto
	*/
    void connectPointInImage(int pointKey, int imageKey);

};

#endif // PTMANAGER_H
