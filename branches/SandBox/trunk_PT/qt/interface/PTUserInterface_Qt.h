#ifndef PTUSERINTERFACE_QT_H
#define PTUSERINTERFACE_QT_H

#include "ui_FotoTriForm.h"
#include "PTUserInterface.h"
//#include "ImageView.h"
#include "LoadingScreen.h"
#include "ETableWidget.h"
#include "WindowsSelectPage.h"
#include "StereoService.h"
#include "FlightDirectionForm.h"

class PointMark;

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

	int currentPointKey,leftImageKey,rightImageKey;
	bool insertionMode;

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

    void closeEvent(QCloseEvent *event);

private:
    void init();

public:
    static PTUserInterface_Qt* instance(PTManager* ptManager);
	void updateImageTable(QString image,string imageFilename, bool move=true);
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
	void setCurrentPointKey(int newPointKey);

        //void selectAllAppearances(int index);
        //int findKeyAppearances(QString table, QString searched);
        //void showImagesAppearances(int indexRow,int indexCol);

        void updateImagesList(QString imageFilename);
	void updatePointsTable();
	void selectAllAppearances(int pointKey);
        int findKeyAppearances(ETableWidget *table, int pointKey);
	void showImagesAppearances(int pointKey);


	void imageClicked(QPointF* pixel);
	void updateCoordinatesInfo(QPointF* pixel);
        void updateMark(MonoDisplay *display,int imageKey, int pointKey, QPointF pixel);

	void markAllpoints(MonoDisplay *display);
	void clearAllMarks(MonoDisplay *display);
	// Deixa o usuario entrar com o valor da linha e coluna na mao atualizando na interface Atualmente desabilitada
	void updatePoint(int, int, int);
	//void closingPTUserInterface();
	void exportToKml();

	void setMaxIteration(int iterations);
	void setMetricConvergencyValue(double value);
	void setAngularConvergencyValue(double value);

	void addPoint();
	//void insertPointIn(int imageKey);
	void toggleInsertPointMode(bool newInsertionMode);
        //int isPointIn(ETableWidget *table, int pointkey);

        void tableClicked(QTableWidgetItem* item);

protected slots:
	void undoMark();
	void putInStack(int oldCol,int oldLin,int pointKey, int imageKey, QString PointId);

	//virtual bool confirmToClose();
};

class PointMark
{
protected:
	QPointF *digitalCoordinate;
	QString pointId;
	int keyPoint;
	int keyImage;

public:
	PointMark(QPointF coord,int keypoint ,int keyimage,QString id="PointId");
	void setDigitalCoordinate(QPointF newCoord);
	void setPointId(QString newPointId);
	void setKeyPoint(int newKeyPoint);
	void setKeyImage(int newKeyImage);

	QPointF* getDigitalCoordinate();
	QString getPointId();
	int getKeyPoint();
	int getKeyImage();
	QString toString();

};


#endif // PTUSERINTERFACE_QT_H
