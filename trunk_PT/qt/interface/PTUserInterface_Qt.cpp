#include "PTUserInterface_Qt.h"

#include <qapplication.h>
#include <QtGui>

PTUserInterface_Qt* PTUserInterface_Qt::ptInst = NULL;

PTUserInterface_Qt* PTUserInterface_Qt::instance(PTManager *ptManager)
{
    if (ptInst != NULL)
	{
        delete ptInst;
        ptInst = NULL;
	}
    if (ptInst == NULL)
	{
                ptInst = new PTUserInterface_Qt(ptManager);
	}
    return ptInst;
}

PTUserInterface_Qt::PTUserInterface_Qt(PTManager *manager, QWidget *parent, Qt::WindowFlags fl)
	:QMainWindow(parent, fl)
{
	setupUi(this);
    ptManager = manager;

    setWindowTitle("E-foto - Foto Triangulation");

    connect(actionMove,SIGNAL(triggered()),this,SLOT(activePanMode()));
    connect(actionZoom,SIGNAL(triggered()),this,SLOT(activeZoomMode()));
    connect(actionFit_View,SIGNAL(triggered()),this,SLOT(fitView()));
    connect(actionView_Report, SIGNAL(triggered()), this, SLOT(viewReport()));
    connect(actionFoto_Tri,SIGNAL(triggered()), this, SLOT(calculatePT()));

	setWindowState(this->windowState() | Qt::WindowMaximized);
	qApp->processEvents();
	init();
}

PTUserInterface_Qt::~PTUserInterface_Qt()
{

}
void PTUserInterface_Qt::init()
{
	QWidget* areaImageLeftWidget = new QWidget();
        QWidget* areaImageRightWidget = new QWidget();

        //QLabel* areaImageLeftWidget = new QLabel("imagem esquerda");
        //QLabel* areaImageRightWidget = new QLabel("imagem direita");

	QGridLayout* gridLayout = new QGridLayout();

	myImageLeftView = new ImageView(areaImageLeftWidget);
        myImageRightView = new ImageView(areaImageRightWidget);

    tableImageLeft = new TableIOEOWidget();
    tableImageRight = new TableIOEOWidget();

	QVBoxLayout *verticalLeftLayout= new QVBoxLayout();
	verticalLeftLayout->addWidget(myImageLeftView,Qt::AlignCenter);
	verticalLeftLayout->addWidget(tableImageLeft);

	QVBoxLayout *verticalRightLayout= new QVBoxLayout();
	verticalRightLayout->addWidget(myImageRightView,Qt::AlignCenter);
	verticalRightLayout->addWidget(tableImageRight);

	gridLayout->addLayout(verticalLeftLayout,0,0,1,1);
	gridLayout->addLayout(verticalRightLayout,0,1,1,1);

	centralwidget->setLayout(gridLayout);

       // connect(myImageLeftView, SIGNAL(mousePressed()), this, SLOT(informState()));
       //connect(myImageLeftView, SIGNAL(markClicked(QPoint)), this, SLOT(receiveMark(QPoint)));
	connect(myImageLeftView, SIGNAL(changed()), this, SLOT(makeRepaint()));

        //connect(myImageRightView, SIGNAL(mousePressed()), this, SLOT(informState()));
        //connect(myImageRightView, SIGNAL(markClicked(QPoint)), this, SLOT(receiveMark(QPoint)));
        connect(myImageRightView, SIGNAL(changed()), this, SLOT(makeRepaint()));



}

void PTUserInterface_Qt::closeEvent(QCloseEvent *event)
{
	LoadingScreen::instance().show();
	qApp->processEvents();
        //delete(myImageRightView);
        delete(myImageLeftView);

        ptManager->returnProject();
	QMainWindow::closeEvent(event);
}

bool PTUserInterface_Qt::exec()
{

	this->show();

	LoadingScreen::instance().close();
	qApp->processEvents();
        myImageLeftView->loadImage(QString(ptManager->getImagefile(0).c_str()));

	return true;
}

void PTUserInterface_Qt::makeRepaint()
{
    if (sender()->objectName()==myImageLeftView->objectName())
    {
        myImageLeftView->repaint();
        tableImageLeft->repaint();
    }
    if (sender()->objectName()==myImageRightView->objectName())
    {
        myImageRightView->repaint();
        tableImageRight->repaint();
    }
}
/*
void PTUserInterface_Qt::activeSetMode()
{
        myImageLeftView->setViewMode(1);
}
*/
void PTUserInterface_Qt::activePanMode()
{
        myImageLeftView->setViewMode(2);
}

void PTUserInterface_Qt::activeZoomMode()
{
        myImageLeftView->setViewMode(3);
}

void PTUserInterface_Qt::fitView()
{
        myImageLeftView->fitView();
}

void PTUserInterface_Qt::viewReport()
{
    qDebug("Vendo Report");
}

void PTUserInterface_Qt::calculatePT()
{
    qDebug("Calculando FotoTri");
}
