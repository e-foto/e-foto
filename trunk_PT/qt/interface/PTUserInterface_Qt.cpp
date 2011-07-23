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

        //setWindowState(this->windowState() | Qt::WindowMaximized);
	actionMove->setChecked(true);
	actionView_Report->setEnabled(false);

	qApp->processEvents();
	init();
}

PTUserInterface_Qt::~PTUserInterface_Qt()
{

}
void PTUserInterface_Qt::init()
{
	QWidget* areaImageLeftWidget = new QWidget();
   //     QWidget* areaImageRightWidget = new QWidget();

        //QLabel* areaImageLeftWidget = new QLabel("imagem esquerda");
        //QLabel* areaImageRightWidget = new QLabel("imagem direita");

	QGridLayout* gridLayout = new QGridLayout();

	myImageLeftView = new ImageView(areaImageLeftWidget);
        //myImageRightView = new ImageView(areaImageRightWidget);

        tableImageLeft = new TableIOEOWidget();
 //   tableImageRight = new TableIOEOWidget();

	QVBoxLayout *verticalLeftLayout= new QVBoxLayout();
	verticalLeftLayout->addWidget(myImageLeftView,Qt::AlignCenter);
	verticalLeftLayout->addWidget(tableImageLeft);

        /*
	QVBoxLayout *verticalRightLayout= new QVBoxLayout();
	verticalRightLayout->addWidget(myImageRightView,Qt::AlignCenter);
	verticalRightLayout->addWidget(tableImageRight);
*/
	gridLayout->addLayout(verticalLeftLayout,0,0,1,1);
//	gridLayout->addLayout(verticalRightLayout,0,1,1,1);

	centralwidget->setLayout(gridLayout);

       // connect(myImageLeftView, SIGNAL(mousePressed()), this, SLOT(informState()));
       //connect(myImageLeftView, SIGNAL(markClicked(QPoint)), this, SLOT(receiveMark(QPoint)));
	connect(myImageLeftView, SIGNAL(changed()), this, SLOT(makeRepaint()));

        //connect(myImageRightView, SIGNAL(mousePressed()), this, SLOT(informState()));
        //connect(myImageRightView, SIGNAL(markClicked(QPoint)), this, SLOT(receiveMark(QPoint)));
      //  connect(myImageRightView, SIGNAL(changed()), this, SLOT(makeRepaint()));



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
        //myImageLeftView->loadImage(QString(ptManager->getImagefile(0).c_str()));

	return true;
}

void PTUserInterface_Qt::makeRepaint()
{
   // if (sender()->objectName()==myImageLeftView->objectName())
    //{
        myImageLeftView->repaint();
      /*  tableImageLeft->repaint();
    //}
    //if (sender()->objectName()==myImageRightView->objectName())
    //{
        myImageRightView->repaint();
        tableImageRight->repaint();
    //}*/
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
	//qDebug("Vendo Report");
    QWidget *afpView = new QWidget();
    QHBoxLayout *horizontalLayout= new QHBoxLayout();
    TableIOEOWidget *afpTable=  new TableIOEOWidget(ptManager->getMatrixAFP(),'f',4);
    /*Matar isso tão cedo quanto possivel
    QWidget *a1View = new QWidget();
    TableIOEOWidget *A1Table=  new TableIOEOWidget(ptManager->getMatrixA1(),'f',6);
    QHBoxLayout *a1horizontalLayout= new QHBoxLayout();


    QWidget *a2View = new QWidget();
    TableIOEOWidget *A2Table=  new TableIOEOWidget(ptManager->getMatrixA2(),'f',6);
    QHBoxLayout *a2horizontalLayout= new QHBoxLayout();


    QWidget *l0View = new QWidget();
    TableIOEOWidget *L0Table=  new TableIOEOWidget(ptManager->getMatrixL0(),'f',6);
    QHBoxLayout *l0horizontalLayout= new QHBoxLayout();

    QWidget *lbView = new QWidget();
    TableIOEOWidget *LbTable=  new TableIOEOWidget(ptManager->getMatrixLb(),'f',6);
    QHBoxLayout *lbhorizontalLayout= new QHBoxLayout();

    QWidget *n11View = new QWidget();
    TableIOEOWidget *N11Table=  new TableIOEOWidget(ptManager->getMatrixN11(),'f',6);
    QHBoxLayout *n11horizontalLayout= new QHBoxLayout();


    QWidget *n12View = new QWidget();
    TableIOEOWidget *N12Table=  new TableIOEOWidget(ptManager->getMatrixN12(),'f',6);
    QHBoxLayout *n12horizontalLayout= new QHBoxLayout();


    QWidget *n22View = new QWidget();
    TableIOEOWidget *N22Table=  new TableIOEOWidget(ptManager->getMatrixN22(),'f',6);
    QHBoxLayout *n22horizontalLayout= new QHBoxLayout();



a1horizontalLayout->addWidget(A1Table);
a2horizontalLayout->addWidget(A2Table);
l0horizontalLayout->addWidget(L0Table);
lbhorizontalLayout->addWidget(LbTable);
n11horizontalLayout->addWidget(N11Table);
n22horizontalLayout->addWidget(N22Table);



    a1View->setLayout(a1horizontalLayout);
    a2View->setLayout(a2horizontalLayout);
    l0View->setLayout(l0horizontalLayout);
    lbView->setLayout(lbhorizontalLayout);


    a1View->show();
    a2View->show();
    l0View->show();
    lbView->show();
*/
    horizontalLayout->addWidget(afpTable);
    afpView->setLayout(horizontalLayout);
    afpView->show();
}

bool PTUserInterface_Qt::calculatePT()
{
	//qDebug("Calculando FotoTri");

	bool result = ptManager->calculatePT();
    viewReport();
	actionView_Report->setEnabled(true);
    return result;

}
