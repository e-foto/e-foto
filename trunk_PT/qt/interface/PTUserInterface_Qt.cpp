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

        Matrix exhibition=ptManager->getENH();
        QStringList headerlabels;
        headerlabels<<"E"<<"N"<<"H"<<"Coluna"<<"Linha";
        Matrix col=ptManager->getCol().transpose();
        Matrix lin=ptManager->getLin().transpose();

        /*
        for (int i=0;i<exhibition.getRows();i++)
        {
            for (int j=0;j<exhi)
            Matrix temp(ptManager->getColLin(i,j);
            exhibition.putMatrix()
        }
        */
        //exhibition.putMatrix(ptManager->getLin().transpose(),1,exhibition.getCols()+1);


        tableImageLeft->setTableData(exhibition,'f',3);
        tableImageLeft->setHorizontalHeaderLabels(headerlabels);
        tableImageLeft->resizeTable();
        //for(int i=0;)
        //tableImageLeft->setVerticalHeadersLabels();

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
    QStringList afpHeaderLabels;
    // omega, phi, kappa, X0, Y0, Z0;
    afpHeaderLabels<< QString::fromUtf8("ω")<<QString::fromUtf8("φ")<<QString::fromUtf8("κ")<<"X0"<<"Y0"<<"Z0";


    QWidget *afpView = new QWidget();
    QHBoxLayout *horizontalLayout= new QHBoxLayout();
    TableIOEOWidget *afpTable=  new TableIOEOWidget(ptManager->getMatrixAFP(),'f',5);
    afpTable->setHorizontalHeaderLabels(afpHeaderLabels);
    TableIOEOWidget *L0Table = new TableIOEOWidget(ptManager->getMatrixL0(),'f',5);
    L0Table->setHorizontalHeaderLabels(QStringList("L0"));
    TableIOEOWidget *LbTable = new TableIOEOWidget(ptManager->getMatrixLb(),'f',5);
    LbTable->setHorizontalHeaderLabels(QStringList("Lb"));
    TableIOEOWidget *residuosTable = new TableIOEOWidget(ptManager->getResiduos(),'f',8);
    residuosTable->setHorizontalHeaderLabels(QStringList("Residuos"));

    horizontalLayout->addWidget(afpTable);
    horizontalLayout->addWidget(L0Table);
    horizontalLayout->addWidget(LbTable);
    horizontalLayout->addWidget(residuosTable);
    horizontalLayout->setStretchFactor(afpTable,3);
    horizontalLayout->setStretchFactor(L0Table,1);
    horizontalLayout->setStretchFactor(LbTable,1);
    horizontalLayout->setStretchFactor(residuosTable,1);
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
