#ifndef PTUSERINTERFACE_QT_H
#define PTUSERINTERFACE_QT_H
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
#include "ui_FotoTriForm.h"
#include "PTUserInterface.h"

class WindowsSelectPage;
class QCheckBox;

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class PointMark;
class Marker;
class SeparatedViewer;
class SeparatedStereoViewer;
class SingleDisplay;
class FlightDirectionForm;



/**
* \file PTUserInterface_Qt.h
* \class PTUserInterface_Qt
* \brief Classe que gerencia a interface grafica com o usuario
* \copyright E-Foto group
* \author Paulo André Batista Pupim
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

	unsigned int currentPointKey,leftImageKey,rightImageKey;
	bool insertionMode;

	QList<int> markedImages;
	//void previsionMark(int pointKey,QPointF *point);

	std::string leftImageString, rightImageString;
	QStringList listAllImages;// contem os file name das imagens
	QStringList listAllPoints;// contem os ids dos pontos

	QStringList listImageLeft; // contem a lista das imagens que podem ser escolhidas para a leftView
	QStringList listImageRight;// contem a lista das imagens que podem ser escolhidas para a rightView

	QWidget *selectionView;
	WindowsSelectPage *selectionImagesView;
	WindowsSelectPage *selectionPointsView;

	FlightDirectionForm *flightDirectionForm;

	QList<PointMark*> undoStack;

	QCheckBox *geodesicCheckBox;
	QCheckBox *topocentricCheckBox;
	std::string fileExport;
	int saveFtReport(char *filename);

private:
	/**
 * \brief Metodo que faz as primeiras configuraÃ§oes da interface
 */
	void init();

protected:
	/**
 * \brief Metodo que reimplementa o evento do QT de fechar a janela
 * \param event : Objeto contendo informaÃ§oes sobre o evento de fechamento de janela
 */
	void closeEvent(QCloseEvent *event);

public:
	/**
 * \brief Metodo que cria um ponteiro estatico para a interface
 * \param ptManager : Ponteiro para o gerente do modulo de fototriangulaÃ§ao
 */
	static PTUserInterface_Qt* instance(PTManager* ptManager);

	/**
 * \brief Metodo auxiliar que atualiza os items da tabela da imagem informada
 * \param image : string
 */
	void updateImageTable(ETableWidget *imageTable,std::string imageFilename, bool move=true);
	bool exec();

public slots:

	/* Methods into disuse:
	 *
	void makeTheSpell(); // Metodo da gambiarra inserida no construtor desta classe durante os testes de exibiÃ§Ã£o de resultados grÃ¡ficos (GraphicWorkAround).
	std::deque<Matrix*> getImageMatrixes(); // (GraphicWorkAround).
	Matrix* getImageMatrix(QImage img); // (GraphicWorkAround).
	*/
	void onReportButtonClicked();

	/**
 * \brief Method to close photo-triangulation
 */
	void FTdone();

	/**
 * \brief Metodo que mostra um widget com as informaÃ§oes principais do resultado da fototriangulaÃ§ao
 */
	void viewReport();

	/**
 * \brief Metodo que mostra um widget com algumas opÃ§oes para rodar o calculo da fotoTriangulaÃ§ao
 */
	void showSelectionWindow();

	/**
 * \brief Metodo para salvar as marcas no xml
 */
	void saveMarks();

	/**
 * \brief Metodo para salvar as marcas e os resultados calculados pelo calculo da fototriangulaÃ§ao
 */
	void acceptResults();

	/**
 * \brief Metodo que dispara o calculo da fototriangulaÃ§ao
 * \return bool : Informa se o calculo foi bem sucedido ou nao
 */
	bool calculatePT();

	/**
 * \brief Metodo que abre um widget parao usuario informar a direÃ§ao de voo de cada imagem
 */
	void openImagesFlightDirectionForm();

	/**
 * \brief Metodo que altera o valor da direÃ§ao de voo da imagem
 * \param imageFile : Nome do arquivo de imagem
 * \param kappa0 : Valor do angulo, em radianos, da direÃ§ao de voo
 */
	void setFlightDirection(QString imageFile, double kappa0);

	/**
 * \brief Metodo para alterar a key do ponto atual
 * \param newPointKey : Nova key do ponto
 */
	void setCurrentPointKey(int newPointKey);

	/**
 * \brief Metodo que atualiza a tabela das imagens e garante que a mesma imagem nÃ£o possa ser selecionada nos dois displays
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
 * \brief Metodo que cuida dos procedimentos necessarios para inserÃ§ao ou update de um ponto na imagem
 * \param pixel : Objeto que contem as informaÃ§oes de linha e coluna do pixel
 */
	void imageClicked(QPointF pixel);

	/* Method into disuse:
	 *
 * \brief Metodo que atualiza os valores das coordenadas digitais dos objetos da classe Point
 * \param pixel : Objeto com as informaÃ§oes de linha e coluna do ponto
	void updateCoordinatesInfo(QPointF* pixel);
	*/

	/**
 * \brief Metodo que reponsavel por atualizar no display a posição da marca de um ponto
 * \param display : Objeto display a ser atualizado
 * \param imageKey: Key da imagem contida no display
 * \param pointKey: Key do ponto a ter sua marca atualizada
 * \param   pixel : Objeto contendo a informaÃ§ao sobre a nova marcaÃ§ao do ponto no display
 */
	void updateMark(SingleDisplay *display,int imageKey, int pointKey, QPointF pixel);

	/**
 * \brief Metodo que coloca todas as marcaÃ§oes de ponto no display
 * \param display : Display em que as marcas serao colocadas
 */
	void markAllpoints(SingleDisplay *display);
	void clearAllMarks(SingleDisplay *display);

	/**
 * \brief Metodo que exporta os pontos e croqui do bloco para um arquivo xml que o GoogleEarth interprete
 * \param fromXML : indica se devem ser usadas a orientações exteriores armazenadas no XML
 */
	void exportToKml(bool fromXML = false);

	/**
 * \brief Metodo que altera o numero maximo de iteraÃ§oes para o calculo da fototriangulaÃ§ao
 * \param iterations : Numero maximo de iteraÃ§oes
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
 * \brief Metodo que ativa ou desativa o modo de inserÃ§ao de pontos em uma imagem clicada
 * \param insertionMode : Booleano para ativar ou desativar o modo de inserÃ§ao
 */
	void toggleInsertPointMode(bool insertionMode);
	//int isPointIn(ETableWidget *table, int pointkey);

	/**
 * \brief Metodo responsavel por tomar as providencias quando um usario clica em algum item da tabela
 * \param item : item da tabela clicado
 */
	void tableClicked(QTableWidgetItem* item);


	/**
 * \brief Slot responsavel por habilitar a remoção de medições
 */
	void measureClicked();

	/**
 * \brief Slot responsavel por comandar a medição automatica de pontos em imagens cujos parametros iniciais foram fornecidos
 */
	void autoMeasureClicked();

	/**
 * \brief Slot responsavel por providenciar a remoção de uma medição
 */
	void removeMeasurement();

	/**
 * \brief Slot responsavel por providenciar a remoção de um conjunto de medições
 */
	void removeAllMeasurements();

/**
* \brief Metodo que mostra, coloca em um widget, o resultado de calculo de OE presente no corrente Xml
*/
	void showReportXml();

	void FlightFormClosed(QList<int> list);
		// Deixa o usuario entrar com o valor da linha e coluna na mao atualizando na interface Atualmente desabilitada
	//void updatePoint(int, int, double);

	void exportCoordinatesTxt();

	void exportCoordinates();


protected slots:

	/**
 * \brief Metodo que responsavel por desfazes a ultima maracaÃ§ao do usuario
 */
	void undoMark();

	/**
 * \brief Metodo que empilha as marcaÃ§oes para que possam ser recuperado pelo undo
 * \param oldCol   : Valor da coluna do ponto
 * \param oldLin   : Valor da linha do ponto
 * \param pointKey : Key do ponto
 * \param imageKey : Key da imagem em o ponto esta
 * \param pointId  : texto contendo o id do ponto
 */
	void putInStack(int oldCol,int oldLin,int pointKey, int imageKey, QString pointId);

	/**
	* \brief MÃ©todo que substitui todos os "-1" para -- indicando que esse ponto ainda nÃ£o foi marcado na imagem
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
 * \return QPointF : Obejto contendo informaÃ§oes sobre as coordenadas digitais
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
	unsigned int getKeyPoint();

	/**
 * \brief Metodo para retornar a key do ponto
 * \return int : Key da imagem
 */
	unsigned int getKeyImage();

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
