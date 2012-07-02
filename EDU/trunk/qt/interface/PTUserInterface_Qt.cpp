#include "PTUserInterface_Qt.h"
#include "WindowsSelectPage.h"

#include <qapplication.h>
#include <QtGui>
#include <QMessageBox>
#include "ConvertionsSystems.h"


namespace br {
namespace uerj {
namespace eng {
namespace efoto {

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
	leftImageTableWidget;
	setupUi(this);
	ptManager = manager;
	saveMarksButton->setDisabled(true);

	viewer = new SeparatedStereoViewer(this); // Inserção do novo display
	viewer->blockOpen();
	viewer->blockSave();
	viewerLayout->addWidget(viewer);
	QToolBar* controlTool = new QToolBar("Control Tools");
	controlTool->addWidget(markToolButton);
	controlTool->addWidget(flightDirectionToolButton);
	controlTool->addWidget(calculateFotoTriToolButton);
	controlTool->addWidget(saveMarksButton);
	controlTool->addWidget(viewReportToolButton);
	controlTool->addWidget(exportToKmlButton);
    //controlTool->addWidget(insertPointInButton);
    /*viewer->*/addToolBar(Qt::LeftToolBarArea,controlTool);
    addToolBar(Qt::LeftToolBarArea,viewer->getToolBar());
    toolsDockWidget->setHidden(true);
    mark = new Marker(SymbolsResource::getTriangle(QColor(255,255,0,255), QColor(Qt::transparent),QSize(24,24), 2, true));
    selectedMark = new Marker(SymbolsResource::getTriangle(QColor(0,255,0,255), QColor(Qt::transparent),QSize(24,24), 2, true));
    photoMark = new Marker(SymbolsResource::getSquare(QColor(255,255,0,255), QColor(Qt::transparent),QSize(19,19), 2, true));
    photoSelectedMark = new Marker(SymbolsResource::getSquare(QColor(0,255,0,255), QColor(Qt::transparent),QSize(19,19), 2, true));

    // Escondendo a opção de ligar ou desligar o modo de vinculação de ponto a uma imagem.
    insertPointInButton->setHidden(true);
    insertionMode=true;

	viewer->getLeftMarker().setToOnlyEmitClickedMode(); // Pluges para que o novo display funcione
	viewer->getRightMarker().setToOnlyEmitClickedMode();
	connect(&viewer->getLeftMarker(),SIGNAL(clicked(QPointF)),this,SLOT(imageClicked(QPointF)));
	connect(&viewer->getRightMarker(),SIGNAL(clicked(QPointF)),this,SLOT(imageClicked(QPointF)));

    //Marker mark(SymbolsResource::getX(Qt::yellow, QSize(24, 24),2)); // Personalizando as marcas. Que no futuro eu quero melhorar para inserir uso de 2 ou 3 marcas de acordo com o tipo de ponto.
    //viewer->getLeftMarker().changeMarker(mark);
    //viewer->getRightMarker().changeMarker(mark);

    // Isso permanece aqui para permitir testar uma visualização de resultados ainda em fase de montagem e testes. Isso foi denominado (GraphicWorkAround) para facilitar encontrar as mudanças ou adições no código.
    //SeparatedStereoToolsBar* tool = viewer->getToolBar();
    //QAction* showFotoIndice = new QAction("Results",tool);
    //showFotoIndice->setToolTip("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Show Results</span></p></body></html>");
    //tool->addSeparator();
    //tool->addAction(showFotoIndice);
    //connect(showFotoIndice, SIGNAL(triggered()), this, SLOT(makeTheSpell()));

	setWindowTitle("E-foto - Phototriangulation");

	connect(actionView_Report, SIGNAL(triggered()), this, SLOT(viewReport()));
	connect(actionCalculateFotoTri,SIGNAL(triggered()),this,SLOT(showSelectionWindow()));
	connect(calculateFotoTriToolButton,SIGNAL(clicked()),this,SLOT(showSelectionWindow()));
	connect(saveMarksButton,SIGNAL(clicked()),this,SLOT(saveMarks()));
	connect(flightDirectionToolButton,SIGNAL(clicked()),this,SLOT(openImagesFlightDirectionForm()));
	connect(markToolButton,SIGNAL(clicked()),this,SLOT(addPoint()));
	connect(insertPointInButton,SIGNAL(clicked(bool)),this,SLOT(toggleInsertPointMode(bool)));
	connect(viewReportToolButton,SIGNAL(clicked(bool)),this,SLOT(showReportXml()));
	//connect(leftDisplay,SIGNAL(mousePositionChanged(QPointF*)),this,SLOT(updateCoordinatesInfo(QPointF*)));
	//connect(rightDisplay,SIGNAL(mousePositionChanged(QPointF*)),this,SLOT(updateCoordinatesInfo(QPointF*)));
	connect(exportToKmlButton,SIGNAL(clicked()),this,SLOT(exportToKml()));

	QShortcut* undoShortcut = new QShortcut(QKeySequence(tr("Ctrl+Z", "Undo")),this);
	connect(undoShortcut, SIGNAL(activated()), this, SLOT(undoMark()));

	actionMove->setChecked(true);

	if( ptManager->hasEODone())
		viewReportToolButton->setEnabled(true);
	else
		viewReportToolButton->setEnabled(false);

    //insertionMode=false;
	bool activeCalculate=ptManager->hasAllImagesInitialValues();
	actionCalculateFotoTri->setEnabled(activeCalculate);
	calculateFotoTriToolButton->setEnabled(activeCalculate);
	imagesPointTreeWidget->setColumnHidden(3,true);

	setWindowState(this->windowState() | Qt::WindowMaximized);

	qApp->processEvents();
	//qDebug("Construtor");
	init();

}

PTUserInterface_Qt::~PTUserInterface_Qt()
{

}

void PTUserInterface_Qt::makeTheSpell() // (GraphicWorkAround)
{
	SingleViewer* graphicResults = new SingleViewer(0);

	// Passo 1: Para cada imagem do projeto com uma OE, carregue a imagem e converta em matrix gerando um deque de matrizes
	deque<Matrix*> imgs = getImageMatrixes();
	//deque<Matrix> IOs = getImageIOs();
	//deque<Matrix> EOs = getImageEOs();


	// Passo 2: Repasse ao Manager o deque de imagens e um deque com as IOs e EOs atualizadas (somente Xa). Assinatura do método no manager: Matrix getFotoIndice(deque<Matrix*> imgs, deque<Matrix> IOs, deque<Matrix> EOs, int width, int height, Matrix& dim);
	// Passo 3: Pegue a matrix resultante e carregue-a no visualizador.
	//Matrix dim;
	//graphicResults->loadImage(manager->getFotoIndice( imgs, IOs, EOs, 3000, 1000, dim));
	for (int i = 0; i < imgs.size(); i++)
		delete(imgs.at(i));

	// Passo 4: Dê a métrica correta ao visualizador usando o resumo das dimensões da imagem de fotoindice
	//graphicResults->setOrtoImageMode(dim.get(1,1) ,dim.get(2,1) ,dim.get(3,1) ,dim.get(4,1));

	graphicResults->show();
}

deque<Matrix*> PTUserInterface_Qt::getImageMatrixes() // (GraphicWorkAround).
{
	deque<Matrix*> result;

	// Para cada imagem do projeto abra a imagem com um QImage e faça: Matrix* getImageMatrix(QImage img)
	// Dai é só guardar cada ponteiro no deque e retornar.
}

Matrix* PTUserInterface_Qt::getImageMatrix(QImage img) // (GraphicWorkAround). // Aqui eu considero que isso vai apenas gerar imagens em tons de cinza para o fotoindice. Caso desejem fazer o fotoindice colorido, devido as restrições da classe matrix vamos ter de fazer um foto indice por canal e depois juntar os canais no final do processamento dos fotoindices.
{
	// Transforma a QImage em matrix num oposto direto ao que o Marcelo vem fazendo em suas classes.
	Matrix* mat = new Matrix(img.width(), img.height()); // Note que coluna vira linha e vice-versa.
	double pixel;

	// Convert QImage to Matrix
	for (int i = 0; i <= img.width(); i++)
	{
		for (int j = 0; j <= img.height(); j++)
		{
			// Isso está transpondo a imagem ao construir a matrix, ou seja, (x,y) vira (i,j). O valor em tons de cinza vai ser redimensionado para o espaço normalizado, entre 0 e 1.
			int c = qGray(img.pixel(i, j));
			pixel = (double)c/255.0;
			mat->set(i, j, pixel);
		}
	}
	return mat;
}

void PTUserInterface_Qt::init()
{
	//qDebug("INIT");
	deque<string> images =ptManager->getStringImages();
	deque<string> points =ptManager->getStringIdPoints();
	for (int i=0;i<images.size();i++)
	{
		QString img=images.at(i).c_str();
		listAllImages << img;
		if (i!=1)
			listImageLeft << img;
		if (i!=0)
			listImageRight << img;
	}
	//Esse for esta causando crashs no programa... Verfificar e explicar o porquê.
	for (int i=0;i<points.size();i++)
		listAllPoints << QString(points.at(i).c_str());

	currentPointKey =-1;
	leftImageString = images.at(0);
	rightImageString= images.at(1);
	leftImageKey = ptManager->getImageId(leftImageString);
	rightImageKey = ptManager->getImageId(rightImageString);

	leftImageComboBox->addItems(listImageLeft);
	rightImageComboBox->addItems(listImageRight);
	//QTableWidget
	//Esconde as colunas das keys
	leftImageTableWidget->setColumnHidden(3,true);
	pointsTableWidget->setColumnHidden(5,true);
	rightImageTableWidget->setColumnHidden(3,true);

	leftImageTableWidget->verticalHeader()->hide();
	rightImageTableWidget->verticalHeader()->hide();
	pointsTableWidget->verticalHeader()->hide();

	connect(leftImageComboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(updateImagesList(QString)));
	connect(rightImageComboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(updateImagesList(QString)));

	connect(leftImageTableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this, SLOT( tableClicked(QTableWidgetItem*) ));
	connect(rightImageTableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this, SLOT( tableClicked(QTableWidgetItem*) ));
	connect(pointsTableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this, SLOT( tableClicked(QTableWidgetItem*) ));

	//connect(leftImageTableWidget,SIGNAL(validatedItem(int,int,double)),this,SLOT(updatePoint(int,int,double)));
	//connect(rightImageTableWidget,SIGNAL(validatedItem(int,int,double)),this,SLOT(updatePoint(int,int,double)));

	/*Permite ediÃ§ao de coordenada via tabela*/
	//leftImageTableWidget->setType(1,"QSpinBox");
	//leftImageTableWidget->setType(2,"QSpinBox");

	//rightImageTableWidget->setType(1,"QSpinBox");
	//rightImageTableWidget->setType(2,"QSpinBox");

	// Se tiver sido calculado antes habilita o botão
	viewReportToolButton->setEnabled(ptManager->hasEODone());

	//	connect(new QKeySequence())
}

void PTUserInterface_Qt::closeEvent(QCloseEvent *event)
{
	if (!ptManager->getMarksSavedState())
	{
		QMessageBox::StandardButton reply;
		reply=QMessageBox::question(this,tr("Warning: exiting with unsaved marks"), tr("Do you want to save changes?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		if (reply == QMessageBox ::Yes )
		{
			saveMarks();
		}
		else if (reply == QMessageBox::Cancel)
		{
			event->ignore();
			return;
			//QMainWindow::closeEvent(event);
		}

	}

	LoadingScreen::instance().show();
	qApp->processEvents();

	listAllImages.clear();
	listAllPoints.clear();
	listImageLeft.clear();
	listImageRight.clear();

	ptManager->returnProject();
	QMainWindow::closeEvent(event);
}

bool PTUserInterface_Qt::exec()
{
	bool ok;
	//qDebug("EXEC");
	//	QStringList headerLabelsPoints,idPoints,typePoints,keysPoints;//,leftImageIdPoints, rightImageIdPoints;
	/*
 deque<string>  ids  = ptManager->getStringIdPoints();
 deque<string> types = ptManager->getStringTypePoints();
 deque<string> keys  = ptManager->getStringKeysPoints();
 headerLabelsPoints<<"Id"<<"Type"<<"E"<<"N"<<"H";*/

	/** carregar imagem da esquerda*/
	viewer->loadLeftImage(QString::fromStdString(ptManager->getFilePath(leftImageString)));
	viewer->getLeftDisplay()->getCurrentScene()->scaleTo(1);

	/** carregar imagem da direita*/
	viewer->loadRightImage(QString::fromStdString(ptManager->getFilePath(rightImageString)));
	viewer->getRightDisplay()->getCurrentScene()->scaleTo(1);

	updateImageTable(leftImageTableWidget, leftImageString);
	updatePointsTable();
	//pointsTableWidget->resizeTable();
	updateImageTable(rightImageTableWidget, rightImageString);

	markAllpoints(viewer->getLeftDisplay());
	markAllpoints(viewer->getRightDisplay());

	setCurrentPointKey(pointsTableWidget->item(0,5)->text().toInt(&ok));
	this->show();
	LoadingScreen::instance().close();
	qApp->processEvents();

	return true;
}

void PTUserInterface_Qt::viewReport()
{
	QWidget *resultView = new QWidget();
	resultView->setGeometry(resultView->x()+50,resultView->y()+50,1200,400);
	QHBoxLayout *horizontalLayout= new QHBoxLayout();
	//qDebug("Vendo Report");
	QStringList oeHeaderLabels;
	//omega, phi, kappa, X0, Y0, Z0;ÏÏÎº// ctrl+shift+u depois omega=03c9, phi=03c6	kappa=03ba
	oeHeaderLabels<< "Image Id"<< QString::fromUtf8("ω")<<QString::fromUtf8("φ")<<QString::fromUtf8("κ")<<"X0"<<"Y0"<<"Z0"<<QString::fromUtf8("δω") << QString::fromUtf8("δφ") << QString::fromUtf8("δκ") << QString::fromUtf8("δX0") << QString::fromUtf8("δY0") << QString::fromUtf8("δZ0");


	QString iter="Iterations: ";
	iter+=QString::number(ptManager->getBundleAdjustment()->getTotalIterations());

	QString rmse="RMSE: ";
	rmse+=QString::number(ptManager->getRMSE(),'f',4);

	QLabel *iterations = new QLabel(iter);
	QLabel *converged;
	QLabel *rmseLabel = new QLabel(rmse+" m");
	if (ptManager->getBundleAdjustment()->isConverged())
		converged = new QLabel(tr("Converged: yes"));
	else
		converged = new QLabel(tr("Converged: no"));

	QHBoxLayout *infoLayout= new QHBoxLayout();
	infoLayout->addWidget(iterations);
	infoLayout->addWidget(converged);
	infoLayout->addWidget(rmseLabel);

	/**///tabela das OEs
	QVBoxLayout *oeLayout= new QVBoxLayout();
	ETableWidget *oeTable=  new ETableWidget();
	QStringList imagesSelected;
	deque<string> images=selectionImagesView->getSelectedItens();
	for (int i=0;i<images.size();i++)
		imagesSelected << QString(images.at(i).c_str());
	oeTable->setColumnCount(13);

	oeTable->putInColumn(imagesSelected,0);
	oeTable->putIn(ptManager->getMatrixOE(),0,1,'f',4);
	oeTable->putIn(ptManager->getMVC(),0,7,'f',5);
	oeTable->setHorizontalHeaderLabels(oeHeaderLabels);
	oeTable->setSortingEnabled(true);

	oeLayout->addLayout(infoLayout);
	oeLayout->addWidget(oeTable);
	/*
 QVBoxLayout *mvcLayout= new QVBoxLayout();
 QLabel *mvcLabel= new QLabel("<font size=5>MVC");
 mvcLabel->setTextFormat(Qt::RichText);
 mvcLabel->setAlignment(Qt::AlignHCenter);
 ETableWidget *mvcTable = new ETableWidget(ptManager->getMVC(),'f',10);
 mvcLayout->addWidget(mvcLabel);
 mvcLayout->addWidget(mvcTable);
 oeLayout->addLayout(mvcLayout);
 horizontalLayout->addLayout(oeLayout);
*/
	/**/
	horizontalLayout->setStretchFactor(oeLayout,13);
	/**///tabela dos pontos fotogrametricos

	deque<string>  ids  = ptManager->getSelectedPointIdPhotogrammetric();
	if (ids.size()!=0)
	{
		QVBoxLayout *phtgLayout= new QVBoxLayout();
		QLabel *phtgLabel= new QLabel("<font size=5>Photogrammetric Points");
		phtgLabel->setTextFormat(Qt::RichText);
		phtgLabel->setAlignment(Qt::AlignHCenter);
		ETableWidget *photogrammetricTable = new ETableWidget();
		QStringList headerLabelsPoints,idsPhotogrammetric;//,leftImageIdPoints, rightImageIdPoints;
		headerLabelsPoints<<"Id"<<"E"<<"N"<<"H"<< QString::fromUtf8("δE") << QString::fromUtf8("δN") << QString::fromUtf8("δH");
		Matrix pointsPhotogrametricMatrix=ptManager->getPhotogrammetricENH();
		Matrix pointsResiduosPhotogrametricMatrix=ptManager->getResiduoPhotogrammetric();

		for (int i=0;i<ids.size();i++)
			idsPhotogrammetric << QString(ids.at(i).c_str());
		photogrammetricTable->setColumnCount(7);
		photogrammetricTable->putInColumn(idsPhotogrammetric,0);
		photogrammetricTable->putIn(pointsPhotogrametricMatrix,0,1,'f',4);
		photogrammetricTable->putIn(pointsResiduosPhotogrametricMatrix,0,4,'f',5);
		photogrammetricTable->setHorizontalHeaderLabels(headerLabelsPoints);
		photogrammetricTable->resizeTable();
		photogrammetricTable->setSortingEnabled(true);
		phtgLayout->addWidget(phtgLabel);
		phtgLayout->addWidget(photogrammetricTable);
		//horizontalLayout->addLayout(phtgLayout);
		oeLayout->addLayout(phtgLayout);
		//horizontalLayout->setStretchFactor(phtgLayout,7);
	}
	/**/

	QHBoxLayout *buttonsLayout= new QHBoxLayout();
	QPushButton *acceptButton= new QPushButton(tr("Accept"));
	QPushButton *discardButton= new QPushButton(tr("Discard"));
	QPushButton *exportTxtButton= new QPushButton(tr("Export values"));
	buttonsLayout->addWidget(acceptButton);
	buttonsLayout->addWidget(discardButton);
	buttonsLayout->addWidget(exportTxtButton);

	QVBoxLayout *reportLayout= new QVBoxLayout;
	//reportLayout->addLayout(infoLayout);
	//reportLayout->addLayout(horizontalLayout);
	reportLayout->addLayout(oeLayout);
	reportLayout->addLayout(buttonsLayout);

	resultView->setLayout(reportLayout);
	resultView->show();

	connect(acceptButton,SIGNAL(clicked()),this,SLOT(acceptResults()));
	connect(acceptButton,SIGNAL(clicked()),resultView,SLOT(close()));
	connect(discardButton, SIGNAL(clicked()),resultView,SLOT(close()));
    connect(exportTxtButton,SIGNAL(clicked()),this,SLOT(exportCoordinatesTxt()));
	resultView->setWindowModality(Qt::ApplicationModal);
}

void PTUserInterface_Qt::showReportXml()
{
	Matrix oesXml=ptManager->eoParametersFromXml();
	QDockWidget *dockResultView = new QDockWidget("EO Parameters",this);
	QWidget *resultView = new QWidget();

	dockResultView->setWidget(resultView);
	QHBoxLayout *horizontalLayout= new QHBoxLayout();
	//qDebug("Vendo Report");
	QStringList oeHeaderLabels;
	//omega, phi, kappa, X0, Y0, Z0;ÏÏÎº// ctrl+shift+u depois omega=03c9, phi=03c6	kappa=03ba
    oeHeaderLabels<< "Image Id"<< QString::fromUtf8("ω")<<QString::fromUtf8("φ")<<QString::fromUtf8("κ")<<"X0"<<"Y0"<<"Z0";

	QString iter="Iterations: ";
	iter+=QString::number(ptManager->getPreviousTotalIterationsXml());

	QLabel *iterations = new QLabel(iter);
	QLabel *converged;
	if (ptManager->getPreviousConvergedXML())
		converged = new QLabel(tr("Converged: yes"));
	else
		converged = new QLabel(tr("Converged: no"));

	QString rmse="RMSE: ";
	rmse+=QString::number(ptManager->getPreviousRmseXML());
	QLabel *rmseLabel = new QLabel(rmse +" m");

	QHBoxLayout *infoLayout= new QHBoxLayout();
	infoLayout->addWidget(iterations);
	infoLayout->addWidget(converged);
	infoLayout->addWidget(rmseLabel);

	QStringList imagesSelected;
	//oesXml.show('f',3,"oesXml");
	for (int i=0; i<oesXml.getRows();i++)
	{
		//qDebug("imageID: %d\n",oesXml.getInt(i+1,1));
		imagesSelected << QString::fromStdString(ptManager->getImagefile(oesXml.getInt(i+1,1)).c_str());
	}
	oesXml=oesXml.sel(1,oesXml.getRows(),2,7);
	ETableWidget *table= new ETableWidget();
	table->setColumnCount(7);
	table->putInColumn(imagesSelected,0);
	table->putIn(oesXml,0,1,'f',4);
	table->setHorizontalHeaderLabels(oeHeaderLabels);
	table->resizeTable();
	table->setSortingEnabled(true);
	horizontalLayout->addWidget(table);
	horizontalLayout->setStretchFactor(table,7);

	QVBoxLayout *reportLayout= new QVBoxLayout;
	reportLayout->addLayout(infoLayout);
	reportLayout->addLayout(horizontalLayout);

	//reportLayout->setStretchFactor(horizontalLayout,7);
	resultView->setLayout(reportLayout);
	// int tableHeight = table->rowHeight(0)*table->rowCount()+100 > 600 ? 600 : table->rowHeight(0)*table->rowCount()+100 ;
	//resultView->setMinimumSize(resultView->width()+50,tableHeight);

	dockResultView->setFloating(true);
	dockResultView->show();
	resultView->show();
}

bool PTUserInterface_Qt::calculatePT()
{

	ptManager->selectImages(selectionImagesView->getSelectedItens());
	ptManager->selectPoints(selectionPointsView->getSelectedItens());

	//int answer=QMessageBox::Question(this,tr("Calculating PhotoTriangulation"),tr("This may take awhile.\nPlease wait window with results appears"));

	int answer = QMessageBox::question(this,tr("Calculating PhotoTriangulation"),tr("Do you want to calculate in Local Topocentric Mode?\nThis mode converts all control points coordinates in local topocentric coordinates then calculate photo triangulation.\n\n\nThis may take awhile.\nPlease wait until window with results appears"),QMessageBox::Yes, QMessageBox::No);
	if (answer== QMessageBox::Yes)
		ptManager->setLocalTopocentricMode(true);
	else
		ptManager->setLocalTopocentricMode(false);


	bool result = ptManager->calculatePT();
	if (result)
	{
		viewReport();
		actionView_Report->setEnabled(true);
		selectionView->close();
	}
	if (!ptManager->getBundleAdjustment()->isPossibleCalculate())
	{
		QMessageBox::information(this,tr("Impossible Calculate PhotoTriangulation"),tr("There's no sufficient points to calculate Phototriangulation,\ntry put more Control Points or Photogrammetric(Tie) Points "));
	}

	return result;
}

void PTUserInterface_Qt::showSelectionWindow()
{
	selectionView= new QWidget();

	deque<string> idsOut = ptManager->getPointsWithLesserThanOverlap(2);
	if (idsOut.size()>0)
	{
		QString ids="";
		for (int i=0;i<idsOut.size() ; i++)
		{
			ids+=QString::fromStdString(idsOut.at(i));
			ids+=", ";
		}
		ids.chop(2);
		ids+=".";
		QMessageBox::warning(selectionView,"Point taked out of evaluation",QString("Points with less than 2 overlaps:\n%1").arg(ids));
	}


	//if (selectionImagesView==NULL || selectionPointsView==NULL)
	{
		//qDebug("Ponteiros nulos");
		selectionImagesView= new WindowsSelectPage(tr("Images available"),tr("Images selected"));
		selectionPointsView= new WindowsSelectPage(tr("Points available"),tr("Points selected"));
	}

	QStringList listImages;
	deque<string> lista;
	lista=ptManager->getStringImages();
	int sizeList=lista.size();
	for (int i=0;i<sizeList;i++)
		listImages << QString(lista.at(i).c_str());
	selectionImagesView->setInitialList(listImages);
	selectionImagesView->addAllItems();

	QStringList listPoints;
	lista=ptManager->getStringIdPoints("","noCheckingPoint");
	sizeList=lista.size();
	for (int i=0;i<sizeList;i++)
		listPoints << QString(lista.at(i).c_str());
	selectionPointsView->setInitialList(listPoints);
	selectionPointsView->addAllItems();

	QHBoxLayout *buttonsLayout= new QHBoxLayout();
	QPushButton *runButton= new QPushButton(tr("Run"));
	QPushButton *cancelButton= new QPushButton(tr("Cancel"));

	buttonsLayout->addWidget(runButton);
	buttonsLayout->addWidget(cancelButton);
	QLabel *iterationsLabel= new QLabel("Maximun iterations");
	QSpinBox *iterationsBox = new QSpinBox();
	iterationsBox->setValue(ptManager->getMaxIteration());
	iterationsBox->setRange(1,99);
	iterationsBox->setAccelerated(true);
	iterationsBox->setAlignment(Qt::AlignCenter);

	QLabel *metricConvergencyLabel= new QLabel("Metric Convergency");
	QDoubleSpinBox *metricConvergencyBox= new QDoubleSpinBox();
	metricConvergencyBox->setDecimals(5);
	metricConvergencyBox->setRange(0.00001,1.0);
	metricConvergencyBox->setSingleStep(0.00001);
	metricConvergencyBox->setValue(ptManager->getMetricConvergencyValue());
	metricConvergencyBox->setAccelerated(true);
	metricConvergencyBox->setAlignment(Qt::AlignCenter);

	QLabel *angularConvergencyLabel= new QLabel("Angular Convergency");
	QDoubleSpinBox *angularConvergencyBox= new QDoubleSpinBox();
	angularConvergencyBox->setDecimals(5);
	angularConvergencyBox->setRange(0.00001,1.0);
	angularConvergencyBox->setSingleStep(0.00001);
	angularConvergencyBox->setValue(ptManager->getAngularConvergencyValue());
	angularConvergencyBox->setAccelerated(true);
	angularConvergencyBox->setAlignment(Qt::AlignCenter);

	QHBoxLayout *parametersLayout= new QHBoxLayout();
	parametersLayout->addStretch();
	parametersLayout->addWidget(iterationsLabel);
	parametersLayout->addWidget(iterationsBox);
	parametersLayout->addStretch();
	parametersLayout->addWidget(metricConvergencyLabel);
	parametersLayout->addWidget(metricConvergencyBox);
	parametersLayout->addWidget(angularConvergencyLabel);
	parametersLayout->addWidget(angularConvergencyBox);
	parametersLayout->addStretch();

	connect(iterationsBox,SIGNAL(valueChanged(int)),this,SLOT(setMaxIteration(int)));
	connect(metricConvergencyBox,SIGNAL(valueChanged(double)),this,SLOT(setMetricConvergencyValue(double)));
	connect(angularConvergencyBox,SIGNAL(valueChanged(double)),this,SLOT(setAngularConvergencyValue(double)));

	QVBoxLayout *layout= new QVBoxLayout(this);
	layout->addWidget(selectionImagesView);
	layout->addWidget(selectionPointsView);
	layout->addLayout(parametersLayout);
	layout->addLayout(buttonsLayout);

	selectionView->setLayout(layout);

	connect(runButton,SIGNAL(clicked()),this,SLOT(calculatePT()));
	connect(cancelButton,SIGNAL(clicked()),selectionView,SLOT(close()));

	selectionView->setWindowModality(Qt::ApplicationModal);
	selectionView->show();
}

// Atualiza a tabela das imagens e garante que a mesma imagem não apareça nas duas comboBoxes
void PTUserInterface_Qt::updateImagesList(QString imageFilename)
{
	int index=0;
	for(int i=0;i<listAllImages.size();i++)
		if(listAllImages.at(i)==imageFilename)
			index=i;
	QStringList temp=listAllImages;
	temp.removeAt(index);
	//qDebug()<<"chamou updateImagesList: "<< imageFilename;

	if(sender()==leftImageComboBox)
	{
		leftImageString=imageFilename.toStdString().c_str();
		leftImageKey = ptManager->getImageId(leftImageString);
		QString currentRightImage=rightImageComboBox->currentText();
		rightImageComboBox->blockSignals(true);
		rightImageComboBox->clear();
		listImageRight=temp;
		rightImageComboBox->addItems(listImageRight);
		rightImageComboBox->setCurrentIndex(rightImageComboBox->findText(currentRightImage,Qt::MatchExactly));
		rightImageComboBox->blockSignals(false);
        viewer->loadLeftImage(QString::fromStdString(ptManager->getFilePath(leftImageString)));
		updateImageTable(leftImageTableWidget,leftImageString);
        clearAllMarks(viewer->getLeftDisplay()); markAllpoints(viewer->getLeftDisplay());
		viewer->getLeftDisplay()->updateAll();
	}
	else if(sender()==rightImageComboBox)
	{
		rightImageString = imageFilename.toStdString().c_str();
		rightImageKey = ptManager->getImageId(rightImageString);
		QString currentLefttImage=leftImageComboBox->currentText();
		leftImageComboBox->blockSignals(true);
		leftImageComboBox->clear();
		listImageLeft=temp;
		leftImageComboBox->addItems(listImageLeft);
		leftImageComboBox->setCurrentIndex(leftImageComboBox->findText(currentLefttImage,Qt::MatchExactly));
		leftImageComboBox->blockSignals(false);
        viewer->loadRightImage(QString::fromStdString(ptManager->getFilePath(rightImageString)));
		updateImageTable(rightImageTableWidget,rightImageString);
        clearAllMarks(viewer->getRightDisplay()); markAllpoints(viewer->getRightDisplay());
		viewer->getRightDisplay()->updateAll();
	}
}

//Atualiza a tabela de imagens
void PTUserInterface_Qt::updateImageTable(ETableWidget *imageTable, string imageFilename, bool move)
{
	bool ok;
	QStringList idImagesPoints, keysImagePoints;
	deque<string> imagesPoints = ptManager->getStringIdPoints(imageFilename);
	deque<string> keysPoints = ptManager->getStringKeysPoints(imageFilename);
	for (int i=0;i<imagesPoints.size();i++)
	{
		idImagesPoints << QString(imagesPoints.at(i).c_str());
		keysImagePoints << QString(keysPoints.at(i).c_str());
	}
	Matrix imageColLin= ptManager->getColLin(imageFilename);
	//qDebug()<< "keys " <<imageFilename.c_str() << " : "<<keysImagePoints;
	//if(imageTable=="leftImage")
	//{
	imageTable->setSortingEnabled(false);
	imageTable->clearContents();
	imageTable->setRowCount(0);
	imageTable->putInColumn(idImagesPoints,0);
	imageTable->putIn(imageColLin,0,1,'f',3);//,"QSpinBox",true,0,dim.getInt(1,1),dim.getInt(1,2));
	imageTable->putInColumn(keysImagePoints,3);

	replaceTo__(imageTable);
	int pos=findKeyAppearances(imageTable,currentPointKey);
	if (pos >=0 && imageTable->item(pos,1)->text()!="--" && imageTable->item(pos,2)->text()!="--")
	{
		imageTable->selectRow(pos);

		double col =imageTable->item(pos,1)->text().toDouble(&ok);
		double lin =imageTable->item(pos,2)->text().toDouble(&ok);
		QPointF pixel(col,lin);

		//qDebug("left image coord %dx%d",col,lin);
		if (imageTable==leftImageTableWidget)
		{
            //clearAllMarks(viewer->getLeftDisplay());
            //markAllpoints(viewer->getLeftDisplay());
			//qDebug("mark all left");
			if (move)
			{
				SingleScene* scene = (SingleScene*)viewer->getLeftDisplay()->getCurrentScene();
				scene->moveTo(pixel);
				scene->setDetailedPoint(pixel);
			}
			viewer->getLeftDisplay()->update();
		}
		else
		{
            //clearAllMarks(viewer->getRightDisplay());
            //markAllpoints(viewer->getRightDisplay());
			//qDebug("mark all right");
			if (move)
			{
				SingleScene* scene = (SingleScene*)viewer->getRightDisplay()->getCurrentScene();
				scene->moveTo(pixel);
				scene->setDetailedPoint(pixel);
			}
			viewer->getRightDisplay()->update();
		}
	}
	imageTable->setSortingEnabled(true);

}

void PTUserInterface_Qt::updatePointsTable()
{
	QStringList headerLabelsPoints,idPoints,typePoints,keysPoints;
	deque<string>  ids  = ptManager->getStringIdPoints();
	deque<string> types = ptManager->getStringTypePoints();
	deque<string> keys  = ptManager->getStringKeysPoints();
	headerLabelsPoints<<"Id"<<"Type"<<"E"<<"N"<<"H";

	Matrix pointsMatrix=ptManager->getENH();
	for (int i=0;i<ids.size();i++)
	{
		idPoints << QString(ids.at(i).c_str());
		typePoints << QString(types.at(i).c_str());
		keysPoints << QString(keys.at(i).c_str());
	}


	//pointsMatrix.show('f',4,"MatrixENH");
	pointsTableWidget->putInColumn(idPoints,0);
	pointsTableWidget->putInColumn(typePoints,1);
	pointsTableWidget->putIn(pointsMatrix,0,2,'f',3);
	pointsTableWidget->putInColumn(keysPoints,5);
	pointsTableWidget->setHorizontalHeaderLabels(headerLabelsPoints);
	//pointsTableWidget->resizeTable();
}

void PTUserInterface_Qt::selectAllAppearances(int pointKey)
{
	int indexLeftSearched,indexRightSearched, indexPointsSearched;

	indexLeftSearched = findKeyAppearances(leftImageTableWidget,pointKey);
	indexPointsSearched = findKeyAppearances(pointsTableWidget, pointKey);
	indexRightSearched = findKeyAppearances(rightImageTableWidget, pointKey);

	leftImageTableWidget->clearSelection();
	pointsTableWidget->clearSelection();
	rightImageTableWidget->clearSelection();

	if (indexLeftSearched>=0)
	{
		leftImageTableWidget->selectRow(indexLeftSearched);
		leftImageTableWidget->setRangeSelected(QTableWidgetSelectionRange(indexLeftSearched,3,indexLeftSearched,3),false);
	}
	if (indexPointsSearched>=0)
	{
		pointsTableWidget->selectRow(indexPointsSearched);
		pointsTableWidget->setRangeSelected(QTableWidgetSelectionRange(indexPointsSearched,5,indexPointsSearched,5),false);
	}
	if (indexRightSearched>=0)
	{
		rightImageTableWidget->selectRow(indexRightSearched);
		rightImageTableWidget->setRangeSelected(QTableWidgetSelectionRange(indexRightSearched,3,indexRightSearched,3),false);
	}

}

// Se encontrar retorna o indice na tabela da key procurada senão retorna -1
int PTUserInterface_Qt::findKeyAppearances(ETableWidget *table, int searchedPointKey)
{
	bool ok;
	int rows,key;
	if (table == leftImageTableWidget)
	{
		rows=leftImageTableWidget->rowCount();
		for (int i=0;i<rows;i++)
		{
			key=leftImageTableWidget->item(i,3)->text().toInt(&ok);
			if(key==searchedPointKey)
				return i;
		}
		return -1;
	}else if (table == pointsTableWidget)
	{
		rows=pointsTableWidget->rowCount();
		for (int i=0;i<rows;i++)
		{
			key=pointsTableWidget->item(i,5)->text().toInt(&ok);
			if(key==searchedPointKey)
				return i;
		}
		return -1;
	}else if (table == rightImageTableWidget)
	{
		rows=rightImageTableWidget->rowCount();
		for (int i=0;i<rows;i++)
		{
			key=rightImageTableWidget->item(i,3)->text().toInt(&ok);
			if(key==searchedPointKey)
				return i;
		}
		return -1;
	}
}

void PTUserInterface_Qt::updateMark(SingleDisplay *display, int imageKey, int pointKey, QPointF pixel)
{
	bool ok;
	double col=pixel.x();
	double lin=pixel.y();

	if (col<0 || lin<0 || currentPointKey<0)
		return;

	if (display == viewer->getLeftDisplay())
	{
		int pos=findKeyAppearances(leftImageTableWidget, pointKey);
		SingleScene* scene = (SingleScene*)viewer->getLeftDisplay()->getCurrentScene();
		if(pos<0 || col>scene->imageSize().width() || lin>scene->imageSize().height())
			return;
		saveMarksButton->setEnabled(true);
		ptManager->setMarksSavedState(false);
		double oldCol=leftImageTableWidget->item(pos,1)->text().toDouble(&ok);
		double oldLin=leftImageTableWidget->item(pos,2)->text().toDouble(&ok);
		putInStack(oldCol,oldLin,pointKey,imageKey,leftImageTableWidget->item(pos,0)->text());
		leftImageTableWidget->item(pos,1)->setText(QString::number(col, 'f', 2));
		leftImageTableWidget->item(pos,2)->setText(QString::number(lin, 'f', 2));
		QString pointId=leftImageTableWidget->item(pos,0)->text();
		//scene->geometry()->removePoint(pointId); //scene->geometry()->addPoint(pixel,leftImageTableWidget->item(pos,0)->text(),&mark);
		//qDebug("A %d",pointKey);
        Marker* pointMark;
        int pointsTableIndex = findKeyAppearances(pointsTableWidget,pointKey);
        if (pointsTableWidget->item(pointsTableIndex,1)->text() == "Control")
            pointMark = selectedMark;
        else
            pointMark = photoSelectedMark;
        clearAllMarks(viewer->getLeftDisplay());
        markAllpoints(viewer->getLeftDisplay());
        viewer->getLeftMarker().insertMark(pixel, pointKey, leftImageTableWidget->item(pos,0)->text(),pointMark);
		viewer->getLeftDisplay()->update();

				/*
		if(leftImageTableWidget->getItemSpinBox()!=NULL)
		{
			int colTable=leftImageTableWidget->getCurrentSpinBoxColumn();
			if (colTable==1)
				leftImageTableWidget->getItemSpinBox()->setValue(col);
			else if (colTable==2)
				leftImageTableWidget->getItemSpinBox()->setValue(lin);
		}
				*/
	}else if(display == viewer->getRightDisplay())
	{
		int pos=findKeyAppearances(rightImageTableWidget, pointKey);
		SingleScene* scene = (SingleScene*)viewer->getRightDisplay()->getCurrentScene();
		if(pos<0 || col>scene->imageSize().width() || lin>scene->imageSize().height())
			return;
		saveMarksButton->setEnabled(true);
		ptManager->setMarksSavedState(false);
		double oldCol=rightImageTableWidget->item(pos,1)->text().toDouble(&ok);
		double oldLin=rightImageTableWidget->item(pos,2)->text().toDouble(&ok);
		putInStack(oldCol,oldLin,pointKey,imageKey,rightImageTableWidget->item(pos,0)->text());
		rightImageTableWidget->item(pos,1)->setText(QString::number(col, 'f', 2));
		rightImageTableWidget->item(pos,2)->setText(QString::number(lin, 'f', 2));
		QString pointId=rightImageTableWidget->item(pos,0)->text();
		//scene->geometry()->removePoint(pointId); //scene->geometry()->addPoint(pixel,rightImageTableWidget->item(pos,0)->text(),&mark);
		//qDebug("B %d",pointKey);
        Marker* pointMark;
        int pointsTableIndex = findKeyAppearances(pointsTableWidget,pointKey);
        if (pointsTableWidget->item(pointsTableIndex,1)->text() == "Control")
            pointMark = selectedMark;
        else
            pointMark = photoSelectedMark;
        clearAllMarks(viewer->getRightDisplay());
        markAllpoints(viewer->getRightDisplay());
        viewer->getRightMarker().insertMark(pixel, pointKey,rightImageTableWidget->item(pos,0)->text(),pointMark);
		viewer->getRightDisplay()->update();

				/*
		if(rightImageTableWidget->getItemSpinBox()!=NULL)
		{
			int colTable=rightImageTableWidget->getCurrentSpinBoxColumn();
			if (colTable==1)
				rightImageTableWidget->getItemSpinBox()->setValue(col);
			else if (colTable==2)
				rightImageTableWidget->getItemSpinBox()->setValue(lin);
		}
				*/
	}
	ptManager->updateDigitalCoordinatesPoint(imageKey,pointKey,col,lin );
}

void PTUserInterface_Qt::updateCoordinatesInfo(QPointF *pixel)
{
	if (pixel==NULL)
		return;
	int col = pixel->x();
	int lin = pixel->y();
}

void PTUserInterface_Qt::imageClicked(QPointF pixel)
{
	if (sender() == &viewer->getLeftMarker())
	{
		if (insertionMode)
		{
			if(findKeyAppearances(leftImageTableWidget,currentPointKey)==-1)// Verificar se point nao esta na tabela da direita pelas pointkey
			{
				ptManager->connectPointInImage(currentPointKey,leftImageKey);
				updateImageTable(leftImageTableWidget,leftImageString,false);
				//conecte o ponto corrente na imagem da direita
			}
		}
		updateMark(viewer->getLeftDisplay(),ptManager->getImageId(leftImageString),currentPointKey,pixel);
		//leftView->moveTo(*pixel);
	}
	else if (sender() == &viewer->getRightMarker())
	{
		if (insertionMode)
		{

			if(findKeyAppearances(rightImageTableWidget,currentPointKey)==-1)// Verificar se point nao esta na tabela da esquerda pelas pointkey
			{
				//conecte o ponto corrente na imagem da esquerda
				ptManager->connectPointInImage(currentPointKey,rightImageKey);
				updateImageTable(rightImageTableWidget,rightImageString,false);
			}
		}
		updateMark(viewer->getRightDisplay(),ptManager->getImageId(rightImageString),currentPointKey,pixel);
		//rightView->moveTo(*pixel);
		//previsionMark(currentPointKey,pixel);
	}
	showImagesAppearances(currentPointKey);
}


/* Deixa o usuario entrar com o valor da linha e coluna na mao atualizando na interface. Atualmente abilitada
   */
   /*
void PTUserInterface_Qt::updatePoint(int tableRow,int tableCol, double value)
{
	bool ok;
	double col=-1;
	double lin=-1;
	//int rowPoint=item->row();
	int pointKey;
	QTableWidgetItem *item;

	if (sender()==leftImageTableWidget)
	{
		int imageKey=ptManager->getImageId(leftImageString);
		item=leftImageTableWidget->item(tableRow,tableCol);
		pointKey=leftImageTableWidget->item(tableRow,3)->text().toInt(&ok);
		// Se os valores forem iguais então não houve alteração e não há o que ser alterado
		//if (leftImageTableWidget->getPreviousValue()==value)
		//              return;
		SingleScene* scene = (SingleScene*)viewer->getLeftDisplay()->getCurrentScene();
		if(tableCol==1)
		{
			if (value > scene->imageSize().width())
			{
				value=leftImageTableWidget->getPreviousValue();
				leftImageTableWidget->item(tableRow,tableCol)->setText(QString::number(value));
			}
			col=value;
			lin=leftImageTableWidget->item(tableRow,2)->text().toDouble(&ok);
			//leftImageTableWidget->avaliateType(tableRow,2);
		}
		else if(tableCol==2)
		{
			if (value > scene->imageSize().height())
			{
				value=leftImageTableWidget->getPreviousValue();
				leftImageTableWidget->item(tableRow,tableCol)->setText(QString::number(value));
			}
			lin=value;//item->text().toDouble(&ok);
			col=leftImageTableWidget->item(tableRow,1)->text().toDouble(&ok);
			//leftImageTableWidget->avaliateType(tableRow+1 == leftImageTableWidget->rowCount() ? tableRow:tableRow+1,1);
		}
		//qDebug("updateLeftimage %dx%d",col,lin);
		updateMark(viewer->getLeftDisplay(),imageKey,pointKey,QPointF(col,lin));
		viewer->getLeftDisplay()->getCurrentScene()->moveTo(QPointF(col,lin));
		viewer->getLeftDisplay()->update();
		//ptManager->updateDigitalCoordinatesPoint(imageKey,pointKey,col,lin);
		//qDebug("col %d lin %d",col,lin);
	} else if (sender()==rightImageTableWidget)
	{
		int imageKey=ptManager->getImageId(rightImageString);
		item=rightImageTableWidget->item(tableRow,tableCol);
		pointKey=rightImageTableWidget->item(tableRow,3)->text().toInt(&ok);
		// Se os valores forem iguais então não houve alteração e não há o que ser alterado
		//if (rightImageTableWidget->getPreviousValue()==value)
		//                return;
		SingleScene* scene = (SingleScene*)viewer->getRightDisplay()->getCurrentScene();
		if(tableCol==1)
		{
			if (value > scene->imageSize().width())
			{
				value=rightImageTableWidget->getPreviousValue();
				rightImageTableWidget->item(tableRow,tableCol)->setText(QString::number(value));
			}
			col=value;//item->text().toDouble(&ok);
			lin=rightImageTableWidget->item(tableRow,2)->text().toDouble(&ok);
			//rightImageTableWidget->avaliateType(tableRow,2);
		}
		else if(tableCol==2)
		{
			if (value > scene->imageSize().height())
			{
				value=rightImageTableWidget->getPreviousValue();
				rightImageTableWidget->item(tableRow,tableCol)->setText(QString::number(value));
			}
			lin=value;//item->text().toDouble(&ok);
			col=rightImageTableWidget->item(tableRow,1)->text().toDouble(&ok);
			//rightImageTableWidget->avaliateType( tableRow+1 == rightImageTableWidget->rowCount() ? tableRow:tableRow+1,1);
		}
		updateMark(viewer->getRightDisplay(),imageKey,pointKey,QPointF(col,lin));
		viewer->getRightDisplay()->getCurrentScene()->moveTo(QPointF(col,lin));
		viewer->getRightDisplay()->update();
		//ptManager->updateDigitalCoordinatesPoint(imageKey,pointKey,col,lin);
		//qDebug("col %d lin %d",col,lin);
	}

}
*/
void PTUserInterface_Qt::acceptResults()
{
	ptManager->saveResults();
	ptManager->setENH();
	updatePointsTable();
	if( ptManager->hasEODone())
		viewReportToolButton->setEnabled(true);
	else
		viewReportToolButton->setEnabled(false);
}

void PTUserInterface_Qt::markAllpoints(SingleDisplay *display)
{
	double col,lin;
	bool ok;

	if (display == viewer->getLeftDisplay())
	{
		int pnts=leftImageTableWidget->rowCount();
		for(int i=0;i<pnts;i++)
		{
			col=leftImageTableWidget->item(i,1)->text().toDouble(&ok);
			lin=leftImageTableWidget->item(i,2)->text().toDouble(&ok);
			//colocar as marcas no display
			int pos=findKeyAppearances(leftImageTableWidget, leftImageTableWidget->item(i,3)->text().toInt(&ok));
			if (pos >=0 && (leftImageTableWidget->item(i,1)->text()!="--" && leftImageTableWidget->item(i,2)->text()!="--"))
			{
				//qDebug("left lin: %.3f  col: %.3f",lin,col);
				QPointF pixel(col,lin);
				//viewer->getLeftDisplay()->getCurrentScene()->geometry()->addPoint(pixel,leftImageTableWidget->item(pos,0)->text(),&mark);
				int pointkey = leftImageTableWidget->item(pos,3)->text().toInt();
				//qDebug("C %d", pointkey);
                Marker* pointMark;
                int pointsTableIndex = findKeyAppearances(pointsTableWidget,pointkey);
                if (pointsTableWidget->item(pointsTableIndex,1)->text() == "Control")
                    pointMark = mark;
                else
                    pointMark = photoMark;
                viewer->getLeftMarker().insertMark(pixel, pointkey,leftImageTableWidget->item(pos,0)->text(),pointMark);
				viewer->getLeftDisplay()->update();
			}
		}
	}
	else if (display == viewer->getRightDisplay())
	{
		int pnts=rightImageTableWidget->rowCount();
		for(int i=0;i<pnts;i++)
		{
			col=rightImageTableWidget->item(i,1)->text().toDouble(&ok);
			lin=rightImageTableWidget->item(i,2)->text().toDouble(&ok);
			//colocar as marcas no display
			int pos=findKeyAppearances(rightImageTableWidget, rightImageTableWidget->item(i,3)->text().toInt(&ok));
			if (pos >=0 && (rightImageTableWidget->item(i,1)->text()!="--" && rightImageTableWidget->item(i,2)->text()!="--"))
			{
				//qDebug("right lin: %.3f  col: %.3f",lin,col);
				QPointF pixel(col,lin);
				//viewer->getRightDisplay()->getCurrentScene()->geometry()->addPoint(pixel,rightImageTableWidget->item(pos,0)->text(),&mark);
				int pointkey = rightImageTableWidget->item(pos,3)->text().toInt();
				//qDebug("D %d",pointkey);
                Marker* pointMark;
                int pointsTableIndex = findKeyAppearances(pointsTableWidget,pointkey);
                if (pointsTableWidget->item(pointsTableIndex,1)->text() == "Control")
                    pointMark = mark;
                else
                    pointMark = photoMark;
                viewer->getRightMarker().insertMark(pixel, pointkey, rightImageTableWidget->item(pos,0)->text(),pointMark);
                viewer->getRightDisplay()->update();
			}
		}
	}
}

void PTUserInterface_Qt::clearAllMarks(SingleDisplay *display)
{
	display->getCurrentScene()->geometry()->clear();
	display->update();
}

void PTUserInterface_Qt::showImagesAppearances(int pointKey)
{
	imagesPointTreeWidget->clear();
	deque<string> appearances=ptManager->getImagesAppearances(pointKey);

	if (appearances.size()==0)
        return pointIdLabel->setText(QString("Point %1").arg(QString::fromStdString(ptManager->getPointId(pointKey))));

	QList<QTreeWidgetItem*> treelist;
	for(int i=0;i<appearances.size();i++)
	{
		QString imageKey= QString::number(ptManager->getImageId(appearances.at(i)));
		Matrix coord=ptManager->getDigitalCoordinate(ptManager->getImageId(appearances.at(i)),pointKey);
		QString linStr=QString::number(coord.get(1,1),'f',3);
		QString colStr=QString::number(coord.get(1,2),'f',3);
		if(coord.get(1,1)==-1 && coord.get(1,2)==-1)
		{
			linStr="--";
			colStr="--";
		}
		QStringList stringList;
		//stringList << QString::fromStdString(appearances.at(i)) << QString::number(coord.get(1,1))<< QString::number(coord.get(1,2)) << imageKey;
		stringList << QString::fromStdString(appearances.at(i)) << linStr<< colStr << imageKey;
		QTreeWidgetItem *item=new QTreeWidgetItem(stringList);
		item->setTextAlignment(0,Qt::AlignCenter);
		item->setTextAlignment(1,Qt::AlignCenter);
		item->setTextAlignment(2,Qt::AlignCenter);
		treelist.push_back(item);
	}

	imagesPointTreeWidget->addTopLevelItems(treelist);
	imagesPointTreeWidget->resizeColumnToContents(1);
	imagesPointTreeWidget->resizeColumnToContents(0);
	imagesPointTreeWidget->resizeColumnToContents(2);

    pointIdLabel->setText(QString("Point %1").arg(QString::fromStdString(ptManager->getPointId(pointKey))));
}

void PTUserInterface_Qt::saveMarks()
{
	ptManager->saveMarks();
	ptManager->setMarksSavedState(true);
	saveMarksButton->setEnabled(false);
}

void PTUserInterface_Qt::openImagesFlightDirectionForm()
{
	flightDirectionForm = new FlightDirectionForm(listAllImages,markedImages);
	flightDirectionForm->setPassMode(true);

	connect(flightDirectionForm,SIGNAL(valuesFlightDirectionForm(QString,double)),this,SLOT(setFlightDirection(QString,double)));
	connect(flightDirectionForm,SIGNAL(markedImagesList(QList<int>,QStringList)),this,SLOT(FlightFormClosed(QList<int>)));
	flightDirectionForm->setGeometry((this->x()+this->width())/2,(this->y()+this->height())/2,flightDirectionForm->width(),flightDirectionForm->height());
	flightDirectionForm->show();
}

void PTUserInterface_Qt::setFlightDirection(QString imageFile, double kappa0)
{
	int imageKey = ptManager->getImageId(imageFile.toStdString().c_str());
	ptManager->setImageFlightDirection(imageKey, kappa0);
}

void PTUserInterface_Qt::FlightFormClosed(QList<int> list)
{
	markedImages=list;
	if (markedImages.size()==listAllImages.size())
	{
		actionCalculateFotoTri->setEnabled(true);
		calculateFotoTriToolButton->setEnabled(true);
		//flightDirectionForm->close();
	}
}

void PTUserInterface_Qt::exportCoordinates()
{
	QWidget *chooseSystem=new QWidget();

	QHBoxLayout *hl1=new QHBoxLayout();
	QHBoxLayout *hl2=new QHBoxLayout();
	QVBoxLayout *vl1= new QVBoxLayout();

	QLabel *lbl1= new QLabel("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\"><html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">p, li { white-space: pre-wrap; }</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\"><p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Choose system(s) to export</p></body></html>");
	QLabel *lbl2= new QLabel("File: ");
    geodesicCheckBox= new QCheckBox("Geodesic",this);
    topocentricCheckBox= new QCheckBox("Local Topocentric",this);
	QLineEdit *fileLineEdit = new QLineEdit(this);
    QToolButton *fileChooseButton= new QToolButton(this);
    fileChooseButton->setText("...");
	fileChooseButton->setToolTip("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\"><html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">p, li { white-space: pre-wrap; }</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\"><p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#000000;\">Choose file and path to txt flie</span></p></body></html>");
	QPushButton *exportButton =  new QPushButton("Export");

	hl1->addWidget(geodesicCheckBox);
	hl1->addWidget(topocentricCheckBox);
	hl2->addWidget(lbl2);
	hl2->addWidget(fileLineEdit);
	hl2->addWidget(fileChooseButton);
	vl1->addWidget(lbl1);
	vl1->addLayout(hl1);
	vl1->addLayout(hl2);
    vl1->addWidget(exportButton);
	chooseSystem->setLayout(vl1);
	fileLineEdit->setReadOnly(true);
	chooseSystem->show();

	qDebug() << "Entrou ";
    connect(exportButton,SIGNAL(clicked()),this,SLOT(exportCoordinatesTxt()));


}


void PTUserInterface_Qt::exportCoordinatesTxt()
{

    QString fileExport= QFileDialog::getSaveFileName(this,"Save file",".","*.txt");
    if(!fileExport.endsWith(".txt"))
        fileExport.append(".txt");

    QFile *exportTxt=new QFile(fileExport);
    exportTxt->setFileName(fileExport);
    exportTxt->open(QIODevice::WriteOnly);

    stringstream coordinates;

    coordinates << ptManager->getCoordinatesGeodesic();
    coordinates << ptManager->getCoordinatesTopocentric();

    exportTxt->write(coordinates.str().data());
    exportTxt->close();
}


void PTUserInterface_Qt::exportToKml()
{

	QString fileName=QFileDialog::getSaveFileName(this,"Save File",QDir::currentPath(),"*.kml");
	if (fileName=="")
		return;

	QFile *exported= new QFile(fileName);
	QFileInfo file(*exported);
	if (!file.fileName().endsWith(".kml"))
		exported->setFileName(fileName.append(".kml"));

	exported->open(QIODevice::WriteOnly);
	exported->write(ptManager->exportBlockTokml(exported->fileName().toStdString().c_str()).c_str());
	exported->close();
}

void PTUserInterface_Qt::tableClicked(QTableWidgetItem* item)
{
	bool ok;
	ETableWidget * table = (ETableWidget*)item->tableWidget();
	int tableRow=item->row();

	double leftCol,leftLin,rightCol,rightLin;

    clearAllMarks(viewer->getLeftDisplay());
    markAllpoints(viewer->getLeftDisplay());
    clearAllMarks(viewer->getRightDisplay());
    markAllpoints(viewer->getRightDisplay());

	if (table==leftImageTableWidget)
    {
		setCurrentPointKey(leftImageTableWidget->item(tableRow,3)->text().toInt(&ok));
        Marker* pointMark;
        int pointsTableIndex = findKeyAppearances(pointsTableWidget,currentPointKey);
        if (pointsTableWidget->item(pointsTableIndex,1)->text() == "Control")
            pointMark = selectedMark;
        else
            pointMark = photoSelectedMark;
		leftCol=leftImageTableWidget->item(tableRow,1)->text().toDouble(&ok);
		leftLin=leftImageTableWidget->item(tableRow,2)->text().toDouble(&ok);
		if (leftImageTableWidget->item(tableRow,1)->text()!="--" && leftImageTableWidget->item(tableRow,2)->text()!="--")
		{
			SingleScene* scene = (SingleScene*)viewer->getLeftDisplay()->getCurrentScene();
            scene->moveTo(QPointF(leftCol,leftLin));
            scene->geometry()->updatePoint(QPointF(leftCol,leftLin),currentPointKey,leftImageTableWidget->item(tableRow,0)->text(),pointMark);
			scene->setDetailedPoint(QPointF(leftCol,leftLin));
			viewer->getLeftDisplay()->update();
		}
    }
	else if (table==rightImageTableWidget)
    {
		setCurrentPointKey(rightImageTableWidget->item(tableRow,3)->text().toInt(&ok));
        Marker* pointMark;
        int pointsTableIndex = findKeyAppearances(pointsTableWidget,currentPointKey);
        if (pointsTableWidget->item(pointsTableIndex,1)->text() == "Control")
            pointMark = selectedMark;
        else
            pointMark = photoSelectedMark;
		rightCol=rightImageTableWidget->item(tableRow,1)->text().toDouble(&ok);
		rightLin=rightImageTableWidget->item(tableRow,2)->text().toDouble(&ok);

		if (rightImageTableWidget->item(tableRow,1)->text()!="--" && rightImageTableWidget->item(tableRow,2)->text()!="--")
		{
			SingleScene* scene = (SingleScene*)viewer->getRightDisplay()->getCurrentScene();
			scene->moveTo(QPointF(rightCol,rightLin));
            scene->geometry()->updatePoint(QPointF(rightCol,rightLin),currentPointKey,rightImageTableWidget->item(tableRow,0)->text(),pointMark);
			scene->setDetailedPoint(QPointF(rightCol,rightLin));
			viewer->getRightDisplay()->update();
		}
	}
	else if (table==pointsTableWidget)
	{
        setCurrentPointKey(pointsTableWidget->item(tableRow,5)->text().toInt(&ok));
        Marker* pointMark;
        int pointsTableIndex = findKeyAppearances(pointsTableWidget,currentPointKey);
        if (pointsTableWidget->item(pointsTableIndex,1)->text() == "Control")
            pointMark = selectedMark;
        else
            pointMark = photoSelectedMark;
		int leftTableIndex = findKeyAppearances(leftImageTableWidget,currentPointKey);
		int rightTableIndex = findKeyAppearances(rightImageTableWidget,currentPointKey);
		if (leftTableIndex >=0)
        {
			leftCol=leftImageTableWidget->item(leftTableIndex,1)->text().toDouble(&ok);
			leftLin=leftImageTableWidget->item(leftTableIndex,2)->text().toDouble(&ok);
			if (leftImageTableWidget->item(leftTableIndex,1)->text()!="--" && leftImageTableWidget->item(leftTableIndex,2)->text()!="--")
			{
				SingleScene* scene = (SingleScene*)viewer->getLeftDisplay()->getCurrentScene();
				scene->moveTo(QPointF(leftCol,leftLin));
                scene->geometry()->updatePoint(QPointF(leftCol,leftLin),currentPointKey,leftImageTableWidget->item(leftTableIndex,0)->text(),pointMark);
				scene->setDetailedPoint(QPointF(leftCol,leftLin));
				viewer->getLeftDisplay()->update();
			}
		}
		if (rightTableIndex >=0)
        {
			rightCol=rightImageTableWidget->item(rightTableIndex,1)->text().toDouble(&ok);
			rightLin=rightImageTableWidget->item(rightTableIndex,2)->text().toDouble(&ok);
			if (rightImageTableWidget->item(rightTableIndex,1)->text()!="--" && rightImageTableWidget->item(rightTableIndex,2)->text()!="--")
			{
				SingleScene* scene = (SingleScene*)viewer->getRightDisplay()->getCurrentScene();
				scene->moveTo(QPointF(rightCol,rightLin));
                scene->geometry()->updatePoint(QPointF(rightCol,rightLin),currentPointKey,rightImageTableWidget->item(rightTableIndex,0)->text(),pointMark);
				scene->setDetailedPoint(QPointF(rightCol,rightLin));
				viewer->getRightDisplay()->update();
			}
		}
    }
}

void PTUserInterface_Qt::setMaxIteration(int iterations)
{
	ptManager->setMaxIteration(iterations);
}

void PTUserInterface_Qt::setMetricConvergencyValue(double value)
{
	ptManager->setMetricConvergencyValue(value);
}

void PTUserInterface_Qt::setAngularConvergencyValue(double value)
{
	ptManager->setAngularConvergencyValue(value);
}

void PTUserInterface_Qt::setCurrentPointKey(int newPointKey)
{
	currentPointKey = (newPointKey > 0 ? newPointKey : 1);
	selectAllAppearances(currentPointKey);
	showImagesAppearances(currentPointKey);

}

void PTUserInterface_Qt::addPoint()
{
	int idNewPoint = ptManager->createNewPoint();
	ptManager->setENH();
	ptManager->reloadPointsCoordinates();
	updatePointsTable();
	setCurrentPointKey(idNewPoint);
    clearAllMarks(viewer->getLeftDisplay());
    markAllpoints(viewer->getLeftDisplay());
    clearAllMarks(viewer->getRightDisplay());
    markAllpoints(viewer->getRightDisplay());
}

void PTUserInterface_Qt::toggleInsertPointMode(bool insertionMode)
{
	this->insertionMode=insertionMode;
	if (insertionMode)
        insertPointInButton->setStatusTip("Insertion mode ON");
	else
		insertPointInButton->setStatusTip("Insertion mode OFF");

}

void PTUserInterface_Qt::replaceTo__(ETableWidget *imageTable)
{
	int rows= imageTable->rowCount();
	int cols= imageTable->columnCount();
	for (int i=0;i<rows;i++)
	{
		for (int j=0;j<cols;j++)
		{
			if (imageTable->item(i,j)->text()=="-1.000")
				imageTable->item(i,j)->setText("--");
		}
	}
}

//Retorna a posicao do ponto na tabela se o ponto nao estiver retorna -1
/*
int PTUserInterface_Qt::findKeyAppearances(ETableWidget *table, int pointKey)
{
 int rows=table->rowCount();
 bool ok;
 for (int i=0;i<rows;i++)
 {
  int key=table->item(i,3)->text().toInt(&ok);
  if (key==pointKey)
   return i;
 }
 return -1;
}*/


void PTUserInterface_Qt::putInStack(int oldCol,int oldLin,int pointKey, int imageKey,QString pointID)
{
	QPointF tempPixel(oldCol,oldLin);
	PointMark *markPoint= new PointMark(tempPixel,pointKey,imageKey,pointID);
	undoStack.push_back(markPoint);

	//qDebug() << "Empilhando "<< markPoint->toString();
}

void PTUserInterface_Qt::undoMark()
{
	//qDebug("undo Chamado");
	if(!undoStack.isEmpty())
	{
		PointMark *mark=undoStack.last();
		QPointF *point=mark->getDigitalCoordinate();
		//qDebug() << "Desempilhando: "<< mark->toString();
		ptManager->updateDigitalCoordinatesPoint(mark->getKeyImage(),mark->getKeyPoint(),point->x(),point->y());

		if (currentPointKey==mark->getKeyPoint())
		{
			if (findKeyAppearances(leftImageTableWidget,mark->getKeyPoint())!=-1)
				updateImageTable(leftImageTableWidget,leftImageString);
			if (findKeyAppearances(rightImageTableWidget,mark->getKeyPoint())!=-1)
				updateImageTable(rightImageTableWidget,rightImageString);
		}
		else
		{
			if (findKeyAppearances(leftImageTableWidget,mark->getKeyPoint())!=-1)
				updateImageTable(leftImageTableWidget,leftImageString);
			if (findKeyAppearances(rightImageTableWidget,mark->getKeyPoint())!=-1)
				updateImageTable(rightImageTableWidget,rightImageString);
			setCurrentPointKey(mark->getKeyPoint());
		}

		undoStack.pop_back();
	}

}




PointMark::PointMark(QPointF coord, int keypoint,int imagekey,QString id)
{
	this->setDigitalCoordinate(coord);
	this->setPointId(id);
	this->setKeyPoint(keypoint);
	this->setKeyImage(imagekey);
}

void PointMark::setDigitalCoordinate(QPointF newCoord)
{
	digitalCoordinate = new QPointF(newCoord.x(),newCoord.y());
}

void PointMark::setKeyPoint(int newKeyPoint)
{
	keyPoint=  (newKeyPoint>0 ? newKeyPoint:1);
}

void PointMark::setPointId(QString newPointId)
{
	pointId= newPointId;
}

void PointMark::setKeyImage(int newKeyImage)
{
	keyImage = (newKeyImage > 0 ? newKeyImage : 1);
}

QPointF * PointMark::getDigitalCoordinate()
{
	return digitalCoordinate;
}

QString PointMark::getPointId()
{
	return pointId;
}

int PointMark::getKeyPoint()
{
	return keyPoint;
}

int PointMark::getKeyImage()
{
	return keyImage;
}

QString PointMark::toString()
{
	return QString(pointId + " " + QString::number(keyPoint)+ " from " +QString::number(keyImage)+ " with " +QString::number(digitalCoordinate->x())+ "x" +QString::number(digitalCoordinate->y()));
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
