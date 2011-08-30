#ifndef PTUSERINTERFACE_QT_H
#define PTUSERINTERFACE_QT_H

#include "ui_FotoTriForm.h"
#include "PTUserInterface.h"
#include "ImageView.h"
#include "LoadingScreen.h"
#include "ETableWidget.h"
#include "WindowsSelectPage.h"

class PTUserInterface_Qt: public QMainWindow, public Ui::PTMainWindow, public PTUserInterface
{
	Q_OBJECT

protected:
    static PTUserInterface_Qt* ptInst;
    PTUserInterface_Qt(PTManager* manager,QWidget* parent=0, Qt::WindowFlags fl = Qt::Window);
    ~PTUserInterface_Qt();

	ETableWidget *tableImageLeft;
	ETableWidget *tableImageRight;
    PTManager *ptManager;
    void closeEvent(QCloseEvent *event);
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
	void updateImageTable(QString image,QString imageFilename);
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

	void updateImagesList(QString);
	void selectAllAppearances(int index);
	int findKeyAppearances(QString table, QString searched);
};


#endif // PTUSERINTERFACE_QT_H
