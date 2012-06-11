#ifndef PTUSERINTERFACE_QT_H
#define PTUSERINTERFACE_QT_H

#include "ui_FotoTriForm.h"
#include "PTUserInterface.h"
//#include "ImageView.h"
#include "LoadingScreen.h"
#include "ETableWidget.h"
#include "WindowsSelectPage.h"
#include "ImageViewers.h"
#include "FlightDirectionForm.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class PointMark;


/**
* \file PTUserInterface_Qt.h
* \class PTUserInterface_Qt
* \brief Classe que gerencia a interface grafica com o usuario
* \copyright E-Foto group
* \author Paulo Andr� Batista Pupim
*/
class PTUserInterface_Qt: public QMainWindow, public Ui::PTMainWindow, public PTUserInterface
{
	Q_OBJECT

protected:
	static PTUserInterface_Qt* ptInst;
	PTUserInterface_Qt(PTManager* manager,QWidget* parent=0, Qt::WindowFlags fl = Qt::Window);
	~PTUserInterface_Qt();

	//StereoView *viewStereo;
    SeparatedStereoViewer* viewer;
    Marker* mark;
    Marker* selectedMark;
    Marker* photoMark;
    Marker* photoSelectedMark;

	QLabel *leftCoordinatesInfoLabel;
	QLabel *rightCoordinatesInfoLabel;

	ETableWidget *tableImageLeft;
	ETableWidget *tableImageRight;
	PTManager *ptManager;

	int currentPointKey,leftImageKey,rightImageKey;
	bool insertionMode;

	QList<int> markedImages;
	//void previsionMark(int pointKey,QPointF *point);

	string leftImageString, rightImageString;
	QStringList listAllImages;// contem os file name das imagens
	QStringList listAllPoints;// contem os ids dos pontos

	QStringList listImageLeft; // contem a lista das imagens que podem ser escolhidas para a leftView
	QStringList listImageRight;// contem a lista das imagens que podem ser escolhidas para a rightView

	QWidget *selectionView;
	WindowsSelectPage *selectionImagesView;
	WindowsSelectPage *selectionPointsView;

	FlightDirectionForm *flightDirectionForm;

	QList<PointMark*> undoStack;

private:
	/**
 * \brief Metodo que faz as primeiras configuraçoes da interface
 */
	void init();

protected:
	/**
 * \brief Metodo que reimplementa o evento do QT de fechar a janela
 * \param event : Objeto contendo informaçoes sobre o evento de fechamento de janela
 */
	void closeEvent(QCloseEvent *event);

public:
	/**
 * \brief Metodo que cria um ponteiro estatico para a interface
 * \param ptManager : Ponteiro para o gerente do modulo de fototriangulaçao
 */
	static PTUserInterface_Qt* instance(PTManager* ptManager);

	/**
 * \brief Metodo auxiliar que atualiza os items da tabela da imagem informada
 * \param image : string
 */
	void updateImageTable(ETableWidget *imageTable,string imageFilename, bool move=true);
	bool exec();

public slots:

	void makeTheSpell(); // Metodo da gambiarra inserida no construtor desta classe durante os testes de exibição de resultados gráficos (GraphicWorkAround).
	deque<Matrix*> getImageMatrixes(); // (GraphicWorkAround).
	Matrix* getImageMatrix(QImage img); // (GraphicWorkAround).

	/**
 * \brief Metodo que mostra um widget com as informaçoes principais do resultado da fototriangulaçao
 */
	void viewReport();

	/**
 * \brief Metodo que mostra um widget com algumas opçoes para rodar o calculo da fotoTriangulaçao
 */
	void showSelectionWindow();

	/**
 * \brief Metodo para salvar as marcas no xml
 */
	void saveMarks();

	/**
 * \brief Metodo para salvar as marcas e os resultados calculados pelo calculo da fototriangulaçao
 */
	void acceptResults();

	/**
 * \brief Metodo que dispara o calculo da fototriangulaçao
 * \return bool : Informa se o calculo foi bem sucedido ou nao
 */
	bool calculatePT();

	/**
 * \brief Metodo que abre um widget parao usuario informar a direçao de voo de cada imagem
 */
	void openImagesFlightDirectionForm();

	/**
 * \brief Metodo que altera o valor da direçao de voo da imagem
 * \param imageFile : Nome do arquivo de imagem
 * \param kappa0 : Valor do angulo, em radianos, da direçao de voo
 */
	void setFlightDirection(QString imageFile, double kappa0);

	/**
 * \brief Metodo para alterar a key do ponto atual
 * \param newPointKey : Nova key do ponto
 */
	void setCurrentPointKey(int newPointKey);

	/**
 * \brief Metodo que atualiza a tabela das imagens e garante que a mesma imagem não possa ser selecionada nos dois displays
 * \param imageFilename : Texto contendo o nome do arquivo de imagem
 */
	void updateImagesList(QString imageFilename);

	/**
 * \brief Metodo que atualiza os valores da tabela de pontos
 */
	void updatePointsTable();

	/**
 * \brief Metodo que seleciona todas as linhas de todas as tabelas em que o ponto parece
 * \param pointKey : Key do ponto
 */
	void selectAllAppearances(int pointKey);

	/**
 * \brief Metodo que retorna o numero da linha em que o ponto aparece na tabela, se o ponto nao estiver na tabela retorna -1
 * \param table : Objeto tabela em que sera feita a busca
 * \param pointKey : Key do ponto procurado
 * \return int : Valor da linha da tabela
 */
	int findKeyAppearances(ETableWidget *table, int pointKey);

	/**
 * \brief Metodo que altera a label e o widget que contem os nomes das imagens em que o ponto aparece
 * \param pointKey: Key do ponto
 */
	void showImagesAppearances(int pointKey);

	/**
 * \brief Metodo que cuida dos procedimentos necessarios para inserçao ou update de um ponto na imagem
 * \param pixel : Objeto que contem as informaçoes de linha e coluna do pixel
 */
	void imageClicked(QPointF pixel);

	/**
 * \brief Metodo que atualiza os valores das coordenadas digitais dos objetos da classe Point
 * \param pixel : Objeto com as informaçoes de linha e coluna do ponto
 */
	void updateCoordinatesInfo(QPointF* pixel);

	/**
 * \brief Metodo que reponsavel por atualizar no display a posi��o da marca de um ponto
 * \param display : Objeto display a ser atualizado
 * \param imageKey: Key da imagem contida no display
 * \param pointKey: Key do ponto a ter sua marca atualizada
 * \param   pixel : Objeto contendo a informaçao sobre a nova marcaçao do ponto no display
 */
	void updateMark(SingleDisplay *display,int imageKey, int pointKey, QPointF pixel);

	/**
 * \brief Metodo que coloca todas as marcaçoes de ponto no display
 * \param display : Display em que as marcas serao colocadas
 */
	void markAllpoints(SingleDisplay *display);
	void clearAllMarks(SingleDisplay *display);

	/**
 * \brief Metodo que exporta os pontos e croqui do bloco para um arquivo xml que o GoogleEarth interprete
 */
	void exportToKml();

	/**
 * \brief Metodo que altera o numero maximo de iteraçoes para o calculo da fototriangulaçao
 * \param iterations : Numero maximo de iteraçoes
 */
	void setMaxIteration(int iterations);

	/**
 * \brief Metodo que altera o valor da convergencia dos parametros X0 Y0 e Z0 de cada imagem.
 * \param value : Valor da convergencia em metros
 */
	void setMetricConvergencyValue(double value);

	/**
 * \brief Metodo que altera o valor da convergencia dos parametros omega phi e kappa de cada imagem.
 * \param value : Valor da convergencia em radianos
 */
	void setAngularConvergencyValue(double value);

	/**
 * \brief Metodo para adicionar um ponto fotogrametrico no projeto
 */
	void addPoint();

	/**
 * \brief Metodo que ativa ou desativa o modo de inserçao de pontos em uma imagem clicada
 * \param insertionMode : Booleano para ativar ou desativar o modo de inserçao
 */
	void toggleInsertPointMode(bool insertionMode);
	//int isPointIn(ETableWidget *table, int pointkey);

	/**
 * \brief Metodo responsavel por tomar as providencias quando um usario clica em algum item da tabela
 * \param item : item da tabela clicado
 */
	void tableClicked(QTableWidgetItem* item);

/**
* \brief Metodo que mostra, coloca em um widget, o resultado de calculo de OE presente no corrente Xml
*/
	void showReportXml();

	void FlightFormClosed(QList<int> list);
		// Deixa o usuario entrar com o valor da linha e coluna na mao atualizando na interface Atualmente desabilitada
	//void updatePoint(int, int, double);
protected slots:

	/**
 * \brief Metodo que responsavel por desfazes a ultima maracaçao do usuario
 */
	void undoMark();

	/**
 * \brief Metodo que empilha as marcaçoes para que possam ser recuperado pelo undo
 * \param oldCol   : Valor da coluna do ponto
 * \param oldLin   : Valor da linha do ponto
 * \param pointKey : Key do ponto
 * \param imageKey : Key da imagem em o ponto esta
 * \param pointId  : texto contendo o id do ponto
 */
	void putInStack(int oldCol,int oldLin,int pointKey, int imageKey, QString pointId);

	/**
	* \brief Método que substitui todos os "-1" para -- indicando que esse ponto ainda não foi marcado na imagem
	* \param imageTable: Tabela com valores a serem substituidos
	*/
	void replaceTo__(ETableWidget *imageTable);
	//void openImagesFlightDirectionForm();
	//virtual bool confirmToClose();
};

/**
 * \file PTUserInterface_Qt.h
 * \class PointMark
 * \brief Classe auxiliar os comandos de desfazer marcas
 */
class PointMark
{
protected:
	QPointF *digitalCoordinate;
	QString pointId;
	int keyPoint;
	int keyImage;

public:
	/**
 * \brief Construtor basico da classe
 * \param coord : Objeto contendo a coordenada do ponto
 * \param keypoint : Key do ponto
 * \param keyImage : Key da imagem
 * \param id : texto contendo o id do ponto
 */
	PointMark(QPointF coord,int keypoint ,int keyImage,QString id="PointId");

	/**
 * \brief Metodo para alterar a coordenada digital do ponto
 * \param newCoord : Objeto contendo a nova coordenada
 */
	void setDigitalCoordinate(QPointF newCoord);

	/**
 * \brief Metodo para alterar o id do ponto
 * \param newPointId : Texto contendo o novo Id do ponto
 */
	void setPointId(QString newPointId);

	/**
 * \brief Metodo para alterar a key do ponto
 * \param newKeyPoint : Valor da nova key do ponto
 */
	void setKeyPoint(int newKeyPoint);

	/**
 * \brief Metodo para alterar a key da imagem
 * \param newKeyImage : Valor do nova key da imagem
 */
	void setKeyImage(int newKeyImage);

	/**
 * \brief Metodo para retornar a coordenada digital
 * \return QPointF : Obejto contendo informaçoes sobre as coordenadas digitais
 */
	QPointF* getDigitalCoordinate();

	/**
 * \brief Metodo para retornar o id do ponto
 * \return QString : Texto contendo o id do ponto
 */
	QString getPointId();

	/**
 * \brief Metodo para retornar a key do ponto
 * \return int : Key do ponto
 */
	int getKeyPoint();

	/**
 * \brief Metodo para retornar a key do ponto
 * \return int : Key da imagem
 */
	int getKeyImage();

	/**
 * \brief Metodo que retorna um texto contendo os valores dos atributos do objeto
 * \return QString : Texto informando os valores dos atributos
 */
	QString toString();

};


} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // PTUSERINTERFACE_QT_H
