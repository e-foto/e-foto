#ifndef PTUSERINTERFACE_QT_H
#define PTUSERINTERFACE_QT_H

#include "ui_FotoTriForm.h"
#include "PTUserInterface.h"
#include "ImageView.h"
#include "LoadingScreen.h"
#include "TableIOEOWidget.h"

class PTUserInterface_Qt: public QMainWindow, public Ui::PTMainWindow, public PTUserInterface
{
	Q_OBJECT

protected:
    static PTUserInterface_Qt* ptInst;
    PTUserInterface_Qt(PTManager* manager,QWidget* parent=0, Qt::WindowFlags fl = Qt::Window);
    ~PTUserInterface_Qt();

    ImageView *myImageLeftView;
    ImageView *myImageRightView;
    TableIOEOWidget *tableImageLeft;
    TableIOEOWidget *tableImageRight;
    PTManager *ptManager;
    void closeEvent(QCloseEvent *event);

private:
	void init();

public:
    static PTUserInterface_Qt* instance(PTManager* ptManager);
	bool exec();

public slots:
    void activePanMode();
    void activeZoomMode();
    void fitView();
    void viewReport();
    void calculatePT();
    void makeRepaint();

};


#endif // PTUSERINTERFACE_QT_H
