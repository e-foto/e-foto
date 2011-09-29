#ifndef PTUSERINTERFACE_QT_H
#define PTUSERINTERFACE_QT_H

#include "ui_FotoTriForm.h"
#include "PTUserInterface.h"
#include "ImageView.h"
#include "LoadingScreen.h"
#include "ETableWidget.h"
#include "WindowsSelectPage.h"
#include "StereoService.h"
#include "FlightDirectionForm.h"

class PTUserInterface_Qt: public QMainWindow, public Ui::PTMainWindow, public PTUserInterface
{
	Q_OBJECT

protected:
    static PTUserInterface_Qt* ptInst;
    PTUserInterface_Qt(PTManager* manager,QWidget* parent=0, Qt::WindowFlags fl = Qt::Window);
    ~PTUserInterface_Qt();

	//StereoView *viewStereo;
	MonoDisplay *leftDisplay;
	MonoDisplay *rightDisplay;
	MonoView *leftView;
	MonoView *rightView;
	Marker mark;

	QLabel *leftCoordinatesInfoLabel;
	QLabel *rightCoordinatesInfoLabel;

	int currentZoomLevelDetail;

	ETableWidget *tableImageLeft;
	ETableWidget *tableImageRight;
	PTManager *ptManager;

	int currentPointKey;


	string leftImageString;
	string rightImageString;
	QStringList listAllImages;// contem os file name das imagens
	QStringList listAllPoints;// contem os ids dos pontos

	QStringList listImageLeft; // contem a lista das imagens que podem ser escolhidas para a leftView
	QStringList listImageRight;// contem a lista das imagens que podem ser escolhidas para a rightView

	QWidget *selectionView;
	WindowsSelectPage *selectionImagesView;
	WindowsSelectPage *selectionPointsView;

	FlightDirectionForm *flightDirectionForm;

    void closeEvent(QCloseEvent *event);




private:
    void init();

public:
    static PTUserInterface_Qt* instance(PTManager* ptManager);
	void updateImageTable(QString image,string imageFilename);
    bool exec();

public slots:
    void activePanMode();
	void zoomDetail();
    void fitView();
    void viewReport();
	void makeRepaint();
	void showSelectionWindow();
	void saveMarks();
	void acceptResults();
	bool calculatePT();
	void openImagesFlightDirectionForm();
	void setFlightDirection(QString imageFile, double kappa0);

	void updateImagesList(QString imageFilename);
	void selectAllAppearances(int index);
	void showImagesAppearances(int indexRow,int indexCol);
	int findKeyAppearances(QString table, QString searched);

	void imageClicked(QPointF* pixel);
	void updateCoordinatesInfo(QPointF* pixel);
	void updateMark(string image,int imageKey, int pointKey, QPointF pixel);

	void markAllpoints(MonoDisplay *display);
	void clearAllMarks(MonoDisplay *display);
	// Deixa o usuario entrar com o valor da linha e coluna na mao atualizando na interface Atualmente desabilitada
	void updatePoint(int, int, int);
	//void closingPTUserInterface();
	void exportToKml();

protected slots:
	//virtual bool confirmToClose();
};


#endif // PTUSERINTERFACE_QT_H
