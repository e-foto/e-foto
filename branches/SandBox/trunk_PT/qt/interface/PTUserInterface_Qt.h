#ifndef PTUSERINTERFACE_QT_H
#define PTUSERINTERFACE_QT_H

#include "ui_FotoTriForm.h"
#include "PTUserInterface.h"
#include "ImageView.h"
#include "LoadingScreen.h"
#include "ETableWidget.h"
#include "WindowsSelectPage.h"
#include "StereoService.h"

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

	QLabel *leftCoordinatesInfoLabel;
	QLabel *rightCoordinatesInfoLabel;

	ETableWidget *tableImageLeft;
	ETableWidget *tableImageRight;
    PTManager *ptManager;
    void closeEvent(QCloseEvent *event);

	int currentPointKey;

	string leftImageString;
	string rightImageString;
	QStringList listAllImages;// contem os file name das imagens
	QStringList listAllPoints;// contem os ids dos pontos

	QStringList listImageLeft; // contem a lista das imagens que podem ser escolhidas para a leftView
	QStringList listImageRight;// contem a lista das imagens que podem ser escolhidas para a rightView

	WindowsSelectPage *selectionImagesView;
	WindowsSelectPage *selectionPointsView;

private:
    void init();

public:
    static PTUserInterface_Qt* instance(PTManager* ptManager);
	void updateImageTable(QString image,string imageFilename);
    bool exec();

public slots:
    void activePanMode();
    void activeZoomMode();
    void fitView();
    void viewReport();
	void makeRepaint();
	void showSelectionWindow();
	void acceptResults();
	bool calculatePT();

	void updateImagesList(QString imageFilename);
	void selectAllAppearances(int index);
	int findKeyAppearances(QString table, QString searched);

	void imageClicked(QPointF* pixel);
	void updateCoordinatesInfo(QPointF* pixel);
	void updateMark(string image,int imageKey, int pointKey, int col, int lin);
	// Deixa o usuario entrar com o valor da linha e coluna na mao atualizando na interface Atualmente desabilitada
	//void updatePoint(QTableWidgetItem * item);
};


#endif // PTUSERINTERFACE_QT_H
