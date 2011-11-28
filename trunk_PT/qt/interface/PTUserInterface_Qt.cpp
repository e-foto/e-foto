#include "PTUserInterface_Qt.h"
#include "WindowsSelectPage.h"

#include <qapplication.h>
#include <QtGui>
#include <QMessageBox>
#include "ConvertionsSystems.h"

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
		delete(ptInst);
		ptInst = new PTUserInterface_Qt(ptManager);
	}
	return ptInst;
}

PTUserInterface_Qt::PTUserInterface_Qt(PTManager *manager, QWidget *parent, Qt::WindowFlags fl)
	:QMainWindow(parent, fl)
{
	setupUi(this);
	currentZoomLevelDetail=4;
	ptManager = manager;
	saveMarksButton->setDisabled(true);
	StereoService ss;
	leftView= ss.getMonoView(this,"");
	rightView= ss.getMonoView(this,"");

	leftDisplay = ss.getMonoDisplay(this,leftView);
	rightDisplay = ss.getMonoDisplay(this,rightView);

	DetailDisplay* ldd = leftDisplay->getDetailDisplay();
	DetailDisplay* rdd = rightDisplay->getDetailDisplay();

	OverviewDisplay* lod = leftDisplay->getOverviewDisplay();
	OverviewDisplay* rod = rightDisplay->getOverviewDisplay();

	lod->setMaximumSize(200,200);
	rod->setMaximumSize(200,200);
	ldd->setMaximumHeight(200);
	rdd->setMaximumHeight(200);

	leftCoordinatesInfoLabel = new QLabel();
	rightCoordinatesInfoLabel= new QLabel();

	leftCoordinatesInfoLabel->setMaximumHeight(13);
	rightCoordinatesInfoLabel->setMaximumHeight(13);

	topLeftHorizontalLayout->addWidget(ldd);
	topLeftHorizontalLayout->addWidget(lod);
	leftVerticalLayout->addWidget(leftDisplay);
	leftVerticalLayout->addWidget(leftCoordinatesInfoLabel);

	topRightHorizontalLayout->addWidget(rod);
	topRightHorizontalLayout->addWidget(rdd);
	rightVerticalLayout->addWidget(rightDisplay);
	rightVerticalLayout->addWidget(rightCoordinatesInfoLabel);

	setWindowTitle("E-foto - Phototriangulation");

    connect(actionMove,SIGNAL(triggered()),this,SLOT(activePanMode()));
	//connect(actionZoom,SIGNAL(triggered()),this,SLOT(activeZoomMode()));
    connect(actionFit_View,SIGNAL(triggered()),this,SLOT(fitView()));
    connect(actionView_Report, SIGNAL(triggered()), this, SLOT(viewReport()));
	//connect(actionFoto_Tri,SIGNAL(triggered()), this, SLOT(showSelectionWindow()));
	connect(actionCalculateFotoTri,SIGNAL(triggered()),this,SLOT(showSelectionWindow()));
	connect(calculateFotoTriToolButton,SIGNAL(clicked()),this,SLOT(showSelectionWindow()));
	connect(zoomToolButton,SIGNAL(clicked()),this,SLOT(zoomDetail()));
	connect(saveMarksButton,SIGNAL(clicked()),this,SLOT(saveMarks()));
	connect(flightDirectionToolButton,SIGNAL(clicked()),this,SLOT(openImagesFlightDirectionForm()));
	connect(markToolButton,SIGNAL(clicked()),this,SLOT(addPoint()));
	connect(insertPointInButton,SIGNAL(clicked(bool)),this,SLOT(toggleInsertPointMode(bool)));

	connect(leftDisplay,SIGNAL(mouseClicked(QPointF*)),this,SLOT(imageClicked(QPointF*)));
	connect(rightDisplay,SIGNAL(mouseClicked(QPointF*)),this,SLOT(imageClicked(QPointF*)));
	connect(leftDisplay,SIGNAL(mousePositionChanged(QPointF*)),this,SLOT(updateCoordinatesInfo(QPointF*)));
	connect(rightDisplay,SIGNAL(mousePositionChanged(QPointF*)),this,SLOT(updateCoordinatesInfo(QPointF*)));

	connect(exportToKmlButton,SIGNAL(clicked()),this,SLOT(exportToKml()));

	QShortcut* undoShortcut = new QShortcut(QKeySequence(tr("Ctrl+Z", "Undo")),this);
	connect(undoShortcut, SIGNAL(activated()), this, SLOT(undoMark()));

	//setWindowState(this->windowState() | Qt::WindowMaximized);
	actionMove->setChecked(true);
	actionView_Report->setEnabled(false);
	insertionMode=false;

	actionCalculateFotoTri->setEnabled(false);
	calculateFotoTriToolButton->setEnabled(false);

	qApp->processEvents();
	//qDebug("Construtor");
	init();

}

PTUserInterface_Qt::~PTUserInterface_Qt()
{

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
	for (int i=0;i<images.size();i++)
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
//	leftImageTableWidget->setDisabled();
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

	connect(leftImageTableWidget,SIGNAL(validatedItem(int,int,int)),this,SLOT(updatePoint(int,int,int)));
	connect(rightImageTableWidget,SIGNAL(validatedItem(int,int,int)),this,SLOT(updatePoint(int,int,int)));

	/*Permite ediÃ§ao de coordenada via tabela*/
	//leftImageTableWidget->setType(1,"QSpinBox");
	//leftImageTableWidget->setType(2,"QSpinBox");

	//rightImageTableWidget->setType(1,"QSpinBox");
	//rightImageTableWidget->setType(2,"QSpinBox");

	// Se tiver sido calculado antes habilita o botão
	viewReportToolButton->setEnabled(ptManager->hasPreviousData());

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
	leftView->loadImage(QString::fromStdString(ptManager->getFilePath(leftImageString)));

	/** carregar imagem da direita*/
	rightView->loadImage(QString::fromStdString(ptManager->getFilePath(rightImageString)));
	mark.load("../X16x16.png");

	updateImageTable(leftImageTableWidget, leftImageString);
	updatePointsTable();
	pointsTableWidget->resizeTable();
	updateImageTable(rightImageTableWidget, rightImageString);

	markAllpoints(leftDisplay);
	markAllpoints(rightDisplay);

	setCurrentPointKey(pointsTableWidget->item(0,5)->text().toInt(&ok));
	this->show();
	LoadingScreen::instance().close();
	qApp->processEvents();

	return true;
}

void PTUserInterface_Qt::makeRepaint()
{
   // if (sender()->objectName()==myImageLeftView->objectName())
    //{
		//myImageLeftView->repaint();
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
		//myImageLeftView->setViewMode(2);
}

void PTUserInterface_Qt::zoomDetail()
{
	if(currentZoomLevelDetail!=64)
		currentZoomLevelDetail*=2;
	else
		currentZoomLevelDetail=1;

	leftDisplay->setDetailZoom(currentZoomLevelDetail);
	rightDisplay->setDetailZoom(currentZoomLevelDetail);
}

void PTUserInterface_Qt::fitView()
{

		//myImageLeftView->fitView();
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

	QLabel *iterations = new QLabel(iter);
	QLabel *converged;
	if (ptManager->getBundleAdjustment()->isConverged())
		converged = new QLabel(tr("Converged: yes"));
	else
		converged = new QLabel(tr("Converged: no"));

	QHBoxLayout *infoLayout= new QHBoxLayout();
	infoLayout->addWidget(iterations);
	infoLayout->addWidget(converged);


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
	buttonsLayout->addWidget(acceptButton);
	buttonsLayout->addWidget(discardButton);

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

	resultView->setWindowModality(Qt::ApplicationModal);
}

bool PTUserInterface_Qt::calculatePT()
{

	ptManager->selectImages(selectionImagesView->getSelectedItens());
	ptManager->selectPoints(selectionPointsView->getSelectedItens());

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
		ids.chop(1);
		QMessageBox::warning(selectionView,"Point taked out of evaluate",QString("Points with less than 2 overlaps:\n%1").arg(ids));
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
		leftView->loadImage(QString::fromStdString(ptManager->getFilePath(leftImageString)));
		leftDisplay->update();
		updateImageTable(leftImageTableWidget,leftImageString);
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
		rightView->loadImage(QString::fromStdString(ptManager->getFilePath(rightImageString)));
		rightDisplay->update();
		updateImageTable(rightImageTableWidget,rightImageString);
	}
}

//Atualiza a tabela de imagens
void PTUserInterface_Qt::updateImageTable(ETableWidget *imageTable, string imageFilename, bool move)
{
	/*
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
	//qDebug()<< "keys " <<imageFilename << " : "<<keysImagePoints;
	if(imageTable=="leftImage")
	{
		leftImageTableWidget->setSortingEnabled(false);
		leftImageTableWidget->clearContents();
		leftImageTableWidget->setRowCount(0);
		leftImageTableWidget->putInColumn(idImagesPoints,0);
		leftImageTableWidget->putIn(imageColLin,0,1,'f',0);//,"QSpinBox",true,0,dim.getInt(1,1),dim.getInt(1,2));
		leftImageTableWidget->putInColumn(keysImagePoints,3);
		int pos=findKeyAppearances(leftImageTableWidget,currentPointKey);
		leftImageTableWidget->selectRow(pos);


		clearAllMarks(leftDisplay);
		markAllpoints(leftDisplay);

		if (pos >=0)
		{
			int col =leftImageTableWidget->item(pos,1)->text().toInt(&ok);
			int lin =leftImageTableWidget->item(pos,2)->text().toInt(&ok);
			//qDebug("left image coord %dx%d",col,lin);
			QPointF pixel(col,lin);
			if (move)
				leftView->moveTo(pixel);
			leftDisplay->update();
		}
		leftImageTableWidget->setSortingEnabled(true);

	}else if (image == "rightImage")
	{
		//qDebug()<<"chamou updateImageTable: "<< image << " : " << imageFilename;
		rightImageTableWidget->setSortingEnabled(false);
		rightImageTableWidget->clearContents();
		rightImageTableWidget->setRowCount(0);
		rightImageTableWidget->putInColumn(idImagesPoints,0);
		rightImageTableWidget->putIn(imageColLin,0,1,'f',0);//,"QSpinBox",false,0,);
		rightImageTableWidget->putInColumn(keysImagePoints,3);
		int pos=findKeyAppearances(rightImageTableWidget,currentPointKey);
		rightImageTableWidget->selectRow(pos);


		clearAllMarks(rightDisplay);
		markAllpoints(rightDisplay);

		if (pos >=0)
		{
			int col =rightImageTableWidget->item(pos,1)->text().toInt(&ok);
			int lin =rightImageTableWidget->item(pos,2)->text().toInt(&ok);
			//qDebug("right image coord %dx%d",col,lin);
			QPointF pixel(col,lin);
			if(move)
				rightView->moveTo(pixel);
			rightDisplay->update();
		}
		rightImageTableWidget->setSortingEnabled(true);
	}
	*/

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
	//qDebug()<< "keys " <<imageFilename << " : "<<keysImagePoints;
	//if(imageTable=="leftImage")
	//{
		imageTable->setSortingEnabled(false);
		imageTable->clearContents();
		imageTable->setRowCount(0);
		imageTable->putInColumn(idImagesPoints,0);
		imageTable->putIn(imageColLin,0,1,'f',0);//,"QSpinBox",true,0,dim.getInt(1,1),dim.getInt(1,2));
		imageTable->putInColumn(keysImagePoints,3);

		int pos=findKeyAppearances(imageTable,currentPointKey);
		if (pos >=0)
		{
			imageTable->selectRow(pos);

			int col =imageTable->item(pos,1)->text().toInt(&ok);
			int lin =imageTable->item(pos,2)->text().toInt(&ok);
			QPointF pixel(col,lin);

			//qDebug("left image coord %dx%d",col,lin);
			if (imageTable==leftImageTableWidget)
			{
				clearAllMarks(leftDisplay);
				markAllpoints(leftDisplay);
				//qDebug("mark all left");
				if (move)
					leftView->moveTo(pixel);
				leftDisplay->update();
			}
			else
			{
				clearAllMarks(rightDisplay);
				markAllpoints(rightDisplay);
				//qDebug("mark all right");
				if (move)
					rightView->moveTo(pixel);
				rightDisplay->update();
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
	pointsTableWidget->putInColumn(idPoints,0);
	pointsTableWidget->putInColumn(typePoints,1);
	pointsTableWidget->putIn(pointsMatrix,0,2,'f',3);
	pointsTableWidget->putInColumn(keysPoints,5);
	pointsTableWidget->setHorizontalHeaderLabels(headerLabelsPoints);
	//pointsTableWidget->resizeTable();
}
//Seleciona em todas as tabelas onde o ponto aparece
/*
void PTUserInterface_Qt::selectAllAppearances(int tableRow)
{
	bool ok;
	//qDebug("linha clicada: %d",tableRow);
	int indexLeftSearched,indexRightSearched, indexPointsSearched;
	ETableWidget *table;
	if (sender()==NULL)
		table=pointsTableWidget;
	else
		table=(ETableWidget*)sender();

	if (table==leftImageTableWidget)
	{
		QString searched=leftImageTableWidget->item(tableRow,3)->text();
		currentPointKey=searched.toInt(&ok);
		indexLeftSearched = tableRow;
		indexPointsSearched = findKeyAppearances("pointsTable", searched);
		indexRightSearched = findKeyAppearances("rightImageTable", searched);
		int col=leftImageTableWidget->item(tableRow,1)->text().toInt(&ok);
		int lin=leftImageTableWidget->item(tableRow,2)->text().toInt(&ok);
		leftView->moveTo(QPointF(col,lin));
		leftDisplay->update();
	}
	else if (table==pointsTableWidget)
	{
		QString searched=pointsTableWidget->item(tableRow,5)->text();
		currentPointKey=searched.toInt(&ok);
		indexLeftSearched=findKeyAppearances("leftImageTable",searched);
		indexPointsSearched=tableRow;
		indexRightSearched = findKeyAppearances("rightImageTable", searched);

		if (indexLeftSearched>=0)
		{
			int leftCol=leftImageTableWidget->item(indexLeftSearched,1)->text().toInt(&ok);
			int leftLin=leftImageTableWidget->item(indexLeftSearched,2)->text().toInt(&ok);
			leftView->moveTo(QPointF(leftCol,leftLin));
			leftDisplay->update();
		}
		if (indexRightSearched>=0)
		{
			int rightCol=rightImageTableWidget->item(indexRightSearched,1)->text().toInt(&ok);
			int rightLin=rightImageTableWidget->item(indexRightSearched,2)->text().toInt(&ok);
			rightView->moveTo(QPointF(rightCol,rightLin));
			rightDisplay->update();
		}

	}
	else if (table==rightImageTableWidget)
	{
		QString searched=rightImageTableWidget->item(tableRow,3)->text();
		currentPointKey=searched.toInt(&ok);
		indexLeftSearched=findKeyAppearances("leftImageTable",searched);
		indexPointsSearched = findKeyAppearances("pointsTable", searched);
		indexRightSearched =tableRow;
		int col=rightImageTableWidget->item(tableRow,1)->text().toInt(&ok);
		int lin=rightImageTableWidget->item(tableRow,2)->text().toInt(&ok);
		rightView->moveTo(QPointF(col,lin));
		rightDisplay->update();
	}
	//Limpando seleção
	leftImageTableWidget->clearSelection();
	pointsTableWidget->clearSelection();
	rightImageTableWidget->clearSelection();

	if (indexLeftSearched>=0)
	{
		leftImageTableWidget->selectRow(indexLeftSearched);
		leftImageTableWidget->setRangeSelected(QTableWidgetSelectionRange(indexLeftSearched,3,indexLeftSearched,3),false);
		//leftImageTableWidget->setRangeSelected(QTableWidgetSelectionRange(indexLeftSearched,0,indexLeftSearched,2),true);
	}
	if (indexPointsSearched>=0)
	{
		pointsTableWidget->selectRow(indexPointsSearched);
		pointsTableWidget->setRangeSelected(QTableWidgetSelectionRange(indexPointsSearched,5,indexPointsSearched,5),false);
		//pointsTableWidget->setRangeSelected(QTableWidgetSelectionRange(indexPointsSearched,0,indexPointsSearched,4),true);
	}
	if (indexRightSearched>=0)
	{
		rightImageTableWidget->selectRow(indexRightSearched);
		rightImageTableWidget->setRangeSelected(QTableWidgetSelectionRange(indexRightSearched,3,indexRightSearched,3),false);
		//rightImageTableWidget->setRangeSelected(QTableWidgetSelectionRange(indexRightSearched,0,indexRightSearched,2),true);
	}

}
*/
/*
// Se encontrar retorna o indice na tabela da key procurada senão retorna -1
int PTUserInterface_Qt::findKeyAppearances(QString table, QString searched)
{

        if (table == "leftImageTable")
        {
                for (int i=0;i<leftImageTableWidget->rowCount();i++)
                        if(leftImageTableWidget->item(i,3)->text()==searched)
                                return i;
                return -1;
        }else if (table == "pointsTable")
        {
                for (int i=0;i<pointsTableWidget->rowCount();i++)
                        if(pointsTableWidget->item(i,5)->text()==searched)
                                return i;
                return -1;
        }else if (table == "rightImageTable")
        {
                for (int i=0;i<rightImageTableWidget->rowCount();i++)
                        if(rightImageTableWidget->item(i,3)->text()==searched)
                                return i;
                return -1;
        }
}
*/

// Informa em que imagens o ponto contido na linha indexRow e coluna IndexCol aparece.
/*
void PTUserInterface_Qt::showImagesAppearances(int indexRow, int indexCol)
{
        /*
        if(indexCol==0)
        {
                int keyPoint=pointsTableWidget->item(indexRow,5)->text().toInt(&ok);
                deque<string> appearances=ptManager->getImagesAppearances(keyPoint);
                QWidget *temp= new QWidget();
                QVBoxLayout *layout=new QVBoxLayout();
                QTreeWidget *tree=new QTreeWidget();

                QList<QTreeWidgetItem*> imagesFileName;
                QStringList lista;
                for(int i=0;i<appearances.size();i++)
                {
                        lista.append(QString::fromStdString(appearances.at(i)));
                        QTreeWidgetItem *item=new QTreeWidgetItem(lista);
                        imagesFileName.append(item);
                        lista.clear();
                }

                //qDebug()<<lista;

                //item->
                tree->setHeaderLabel(tr("PointId: %1").arg(pointsTableWidget->item(indexRow,0)->text()));
                tree->addTopLevelItems(imagesFileName);

                //tree->insertTopLevelItems(imagesFileName);
                layout->addWidget(tree);
                temp->setLayout(layout);
                temp->show();
                //connect(temp,SIGNAL(),temp,SLOT(close()));
                //
        }*/
    /*
        bool ok;
        ETableWidget *table;
        int keyPoint;
        if (sender()==NULL)
                table=pointsTableWidget;
        else
                table=(ETableWidget*)sender();

        if (table==leftImageTableWidget)
        {
        //	pointIdLabel->setText(QString("Point %1 is in images").arg(leftImageTableWidget->item(indexRow,0)->text()));
                keyPoint=leftImageTableWidget->item(indexRow,3)->text().toInt(&ok);
        }
        else if (table==pointsTableWidget)
        {
                //pointIdLabel->setText(QString("Point %1 is in images").arg(pointsTableWidget->item(indexRow,0)->text()));
                keyPoint=pointsTableWidget->item(indexRow,5)->text().toInt(&ok);
        }
        else if (table==rightImageTableWidget)
        {
                //pointIdLabel->setText(QString("Point %1 is in images").arg(rightImageTableWidget->item(indexRow,0)->text()));
                keyPoint=rightImageTableWidget->item(indexRow,3)->text().toInt(&ok);
        }

        deque<string> appearances=ptManager->getImagesAppearances(keyPoint);
        imagesPointListWidget->clear();

        QStringList lista;
        for(int i=0;i<appearances.size();i++)
                lista.append(QString::fromStdString(appearances.at(i)));
        imagesPointListWidget->addItems(lista);
        if (table==leftImageTableWidget)
                pointIdLabel->setText(QString("Point %1 is in images").arg(leftImageTableWidget->item(indexRow,0)->text()));
        else if (table==pointsTableWidget)
                pointIdLabel->setText(QString("Point %1 is in images").arg(pointsTableWidget->item(indexRow,0)->text()));
        else if (table==rightImageTableWidget)
                pointIdLabel->setText(QString("Point %1 is in images").arg(rightImageTableWidget->item(indexRow,0)->text()));

}
*/

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

void PTUserInterface_Qt::updateMark(MonoDisplay *display, int imageKey, int pointKey, QPointF pixel)
{
	bool ok;
	int col=pixel.x();
	int lin=pixel.y();

	if (col<0 || lin<0 || currentPointKey<0)
		return;

        if (display==leftDisplay)
	{
                int pos=findKeyAppearances(leftImageTableWidget, pointKey);
		if(pos<0 || col>leftView->imageSize().width() || lin>leftView->imageSize().height())
			return;
		saveMarksButton->setEnabled(true);
		ptManager->setMarksSavedState(false);
		int oldCol=leftImageTableWidget->item(pos,1)->text().toInt(&ok);
		int oldLin=leftImageTableWidget->item(pos,2)->text().toInt(&ok);
		putInStack(oldCol,oldLin,pointKey,imageKey,leftImageTableWidget->item(pos,0)->text());
		leftImageTableWidget->item(pos,1)->setText(QString::number(col));
		leftImageTableWidget->item(pos,2)->setText(QString::number(lin));
		QString pointId=leftImageTableWidget->item(pos,0)->text();
		leftView->geometries()->removePoint(pointId);
		leftView->geometries()->addPoint(pixel,leftImageTableWidget->item(pos,0)->text(),&mark);
		leftDisplay->update();

		if(leftImageTableWidget->getItemSpinBox()!=NULL)
		{
			int colTable=leftImageTableWidget->getCurrentSpinBoxColumn();
			if (colTable==1)
				leftImageTableWidget->getItemSpinBox()->setValue(col);
			else if (colTable==2)
				leftImageTableWidget->getItemSpinBox()->setValue(lin);
		}
        }else if(display == rightDisplay)
	{
                int pos=findKeyAppearances(rightImageTableWidget, pointKey);
		if(pos<0 || col>rightView->imageSize().width() || lin>rightView->imageSize().height())
			return;
		saveMarksButton->setEnabled(true);
		ptManager->setMarksSavedState(false);
		int oldCol=rightImageTableWidget->item(pos,1)->text().toInt(&ok);
		int oldLin=rightImageTableWidget->item(pos,2)->text().toInt(&ok);
		putInStack(oldCol,oldLin,pointKey,imageKey,rightImageTableWidget->item(pos,0)->text());
		rightImageTableWidget->item(pos,1)->setText(QString::number(col));
		rightImageTableWidget->item(pos,2)->setText(QString::number(lin));
		QString pointId=rightImageTableWidget->item(pos,0)->text();
		rightView->geometries()->removePoint(pointId);
		rightView->geometries()->addPoint(pixel,rightImageTableWidget->item(pos,0)->text(),&mark);
		rightDisplay->update();

		if(rightImageTableWidget->getItemSpinBox()!=NULL)
		{
			int colTable=rightImageTableWidget->getCurrentSpinBoxColumn();
			if (colTable==1)
				rightImageTableWidget->getItemSpinBox()->setValue(col);
			else if (colTable==2)
				rightImageTableWidget->getItemSpinBox()->setValue(lin);
		}
	}
	ptManager->updateDigitalCoordinatesPoint(imageKey,pointKey,col,lin );
}

void PTUserInterface_Qt::updateCoordinatesInfo(QPointF *pixel)
{
	if (pixel==NULL)
		return;
	int col = pixel->x();
	int lin = pixel->y();

	if (sender()==leftDisplay)
	{
		if (col<0 || lin <0 || col>leftView->imageSize().width() || lin>leftView->imageSize().height())
			leftCoordinatesInfoLabel->setText(tr("Out of image's borders"));
		else
			leftCoordinatesInfoLabel->setText(QString::number(col) + "x" + QString::number(lin));
	}
	else if (sender()==rightDisplay)
	{
		if (col<0 || lin<0 || col>rightView->imageSize().width() || lin>rightView->imageSize().height())
			rightCoordinatesInfoLabel->setText(tr("Out of image's borders"));
		else
			rightCoordinatesInfoLabel->setText(QString::number(col) + "x" + QString::number(lin));
	}
}

void PTUserInterface_Qt::imageClicked(QPointF *pixel)
{
	if (sender()==leftDisplay)
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
		updateMark(leftDisplay,ptManager->getImageId(leftImageString),currentPointKey,*pixel);
		//leftView->moveTo(*pixel);
	}
	else if (sender()==rightDisplay)
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
		updateMark(rightDisplay,ptManager->getImageId(rightImageString),currentPointKey,*pixel);
		//rightView->moveTo(*pixel);
		//previsionMark(currentPointKey,pixel);
	}
}


/* Deixa o usuario entrar com o valor da linha e coluna na mao atualizando na interface. Atualmente abilitada
   */
void PTUserInterface_Qt::updatePoint(int tableRow,int tableCol, int value)
{
	bool ok;
	int col=-1;
	int lin=-1;
	//int rowPoint=item->row();
	int pointKey;
	QTableWidgetItem *item;

	if (sender()==leftImageTableWidget)
	{
		int imageKey=ptManager->getImageId(leftImageString);
		item=leftImageTableWidget->item(tableRow,tableCol);
		pointKey=leftImageTableWidget->item(tableRow,3)->text().toInt(&ok);
		// Se os valores forem iguais então não houve alteração e não há o que ser alterado
		if ((int)leftImageTableWidget->getPreviousValue()==value)
			return;
		if(tableCol==1)
		{
			if (value>leftView->imageSize().width())
			{
				value=leftImageTableWidget->getPreviousValue();
				leftImageTableWidget->item(tableRow,tableCol)->setText(QString::number(value));
			}
			col=value;
			lin=leftImageTableWidget->item(tableRow,2)->text().toInt(&ok);
			//leftImageTableWidget->avaliateType(tableRow,2);
		}
		else if(tableCol==2)
		{
			if (value>leftView->imageSize().height())
			{
				value=leftImageTableWidget->getPreviousValue();
				leftImageTableWidget->item(tableRow,tableCol)->setText(QString::number(value));
			}
			lin=value;//item->text().toInt(&ok);
			col=leftImageTableWidget->item(tableRow,1)->text().toInt(&ok);
			//leftImageTableWidget->avaliateType(tableRow+1 == leftImageTableWidget->rowCount() ? tableRow:tableRow+1,1);
		}
		//qDebug("updateLeftimage %dx%d",col,lin);
                updateMark(leftDisplay,imageKey,pointKey,QPointF(col,lin));
		leftView->moveTo(QPointF(col,lin));
		leftDisplay->update();
		//ptManager->updateDigitalCoordinatesPoint(imageKey,pointKey,col,lin);
		//qDebug("col %d lin %d",col,lin);
	} else if (sender()==rightImageTableWidget)
	{
		int imageKey=ptManager->getImageId(rightImageString);
		item=rightImageTableWidget->item(tableRow,tableCol);
		pointKey=rightImageTableWidget->item(tableRow,3)->text().toInt(&ok);
		// Se os valores forem iguais então não houve alteração e não há o que ser alterado
		if ((int)rightImageTableWidget->getPreviousValue()==value)
			return;
		if(tableCol==1)
		{
			if (value>rightView->imageSize().width())
			{
				value=rightImageTableWidget->getPreviousValue();
				rightImageTableWidget->item(tableRow,tableCol)->setText(QString::number(value));
			}
			col=value;//item->text().toInt(&ok);
			lin=rightImageTableWidget->item(tableRow,2)->text().toInt(&ok);
			//rightImageTableWidget->avaliateType(tableRow,2);
		}
		else if(tableCol==2)
		{
			if (value>rightView->imageSize().height())
			{
				value=rightImageTableWidget->getPreviousValue();
				rightImageTableWidget->item(tableRow,tableCol)->setText(QString::number(value));
			}
			lin=value;//item->text().toInt(&ok);
			col=rightImageTableWidget->item(tableRow,1)->text().toInt(&ok);
			//rightImageTableWidget->avaliateType( tableRow+1 == rightImageTableWidget->rowCount() ? tableRow:tableRow+1,1);
		}
                updateMark(rightDisplay,imageKey,pointKey,QPointF(col,lin));
		rightView->moveTo(QPointF(col,lin));
		rightDisplay->update();
		//ptManager->updateDigitalCoordinatesPoint(imageKey,pointKey,col,lin);
		//qDebug("col %d lin %d",col,lin);
	}

}


void PTUserInterface_Qt::acceptResults()
{
	ptManager->saveResults();
}

void PTUserInterface_Qt::markAllpoints(MonoDisplay *display)
{
	int col,lin;
	bool ok;

	if (display==leftDisplay)
	{
		int pnts=leftImageTableWidget->rowCount();
		for(int i=0;i<pnts;i++)
		{
			col=leftImageTableWidget->item(i,1)->text().toInt(&ok);
			lin=leftImageTableWidget->item(i,2)->text().toInt(&ok);
			//colocar as marcas no display
			int pos=findKeyAppearances(leftImageTableWidget, leftImageTableWidget->item(i,3)->text().toInt(&ok));
			if (pos >=0)
			{
				QPointF pixel(col,lin);
				leftView->geometries()->addPoint(pixel,leftImageTableWidget->item(pos,0)->text(),&mark);
				leftDisplay->update();
			}
		}
	}
	else if (display==rightDisplay)
	{
		int pnts=rightImageTableWidget->rowCount();
		for(int i=0;i<pnts;i++)
		{
			col=rightImageTableWidget->item(i,1)->text().toInt(&ok);
			lin=rightImageTableWidget->item(i,2)->text().toInt(&ok);
			//colocar as marcas no display
			int pos=findKeyAppearances(rightImageTableWidget, rightImageTableWidget->item(i,3)->text().toInt(&ok));
			if (pos >=0)
			{
				QPointF pixel(col,lin);
				rightView->geometries()->addPoint(pixel,rightImageTableWidget->item(pos,0)->text(),&mark);
				rightDisplay->update();
			}
		}
	}
}

void PTUserInterface_Qt::clearAllMarks(MonoDisplay *display)
{
	display->getCurrentView()->geometries()->clear();
	display->update();
}

void PTUserInterface_Qt::showImagesAppearances(int pointKey)
{
        deque<string> appearances=ptManager->getImagesAppearances(pointKey);
        imagesPointListWidget->clear();

        QStringList lista;
        for(int i=0;i<appearances.size();i++)
                lista.append(QString::fromStdString(appearances.at(i)));
        imagesPointListWidget->addItems(lista);

        pointIdLabel->setText(QString("Point %1 is in images").arg(QString::fromStdString(ptManager->getPointId(pointKey))));

}

void PTUserInterface_Qt::saveMarks()
{
	ptManager->saveMarks();
	ptManager->setMarksSavedState(true);
	saveMarksButton->setEnabled(false);
}

void PTUserInterface_Qt::openImagesFlightDirectionForm()
{
	flightDirectionForm= new FlightDirectionForm();
	flightDirectionForm->imagesFlightDirectionCombo->addItems(listAllImages);

	connect(flightDirectionForm,SIGNAL(valuesFlightDirectionForm(QString,double)),this,SLOT(setFlightDirection(QString,double)));

	flightDirectionForm->setGeometry((this->x()+this->width())/2,(this->y()+this->height())/2,flightDirectionForm->width(),flightDirectionForm->height());
	flightDirectionForm->show();
}

void PTUserInterface_Qt::setFlightDirection(QString imageFile, double kappa0)
{
	int imageKey = ptManager->getImageId(imageFile.toStdString().c_str());
	ptManager->setImageFlightDirection(imageKey, kappa0);

	int currentIndex=flightDirectionForm->imagesFlightDirectionCombo->currentIndex();

	if (currentIndex==listAllImages.size()-1)
		flightDirectionForm->imagesFlightDirectionCombo->setCurrentIndex(listAllImages.size()-1);
	else
		flightDirectionForm->imagesFlightDirectionCombo->setCurrentIndex(currentIndex+1);

	if (ptManager->allKappaSet())
	{
		actionCalculateFotoTri->setEnabled(true);
		calculateFotoTriToolButton->setEnabled(true);
	}
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

    int leftCol,leftLin,rightCol,rightLin;

    if (table==leftImageTableWidget)
    {
        setCurrentPointKey(leftImageTableWidget->item(tableRow,3)->text().toInt(&ok));
        leftCol=leftImageTableWidget->item(tableRow,1)->text().toInt(&ok);
        leftLin=leftImageTableWidget->item(tableRow,2)->text().toInt(&ok);
        leftView->moveTo(QPointF(leftCol,leftLin));
        leftDisplay->update();
    }
    else if (table==rightImageTableWidget)
    {
        setCurrentPointKey(rightImageTableWidget->item(tableRow,3)->text().toInt(&ok));
        rightCol=rightImageTableWidget->item(tableRow,1)->text().toInt(&ok);
        rightLin=rightImageTableWidget->item(tableRow,2)->text().toInt(&ok);
        rightView->moveTo(QPointF(rightCol,rightLin));
        rightDisplay->update();

    }
    else if (table==pointsTableWidget)
    {
        setCurrentPointKey(pointsTableWidget->item(tableRow,5)->text().toInt(&ok));
        int leftTableIndex = findKeyAppearances(leftImageTableWidget,currentPointKey);
        int rightTableIndex = findKeyAppearances(rightImageTableWidget,currentPointKey);
        if (leftTableIndex >=0)
        {
            leftCol=leftImageTableWidget->item(leftTableIndex,1)->text().toInt(&ok);
            leftLin=leftImageTableWidget->item(leftTableIndex,2)->text().toInt(&ok);
            leftView->moveTo(QPointF(leftCol,leftLin));
            leftDisplay->update();
        }
        if (rightTableIndex >=0)
        {
            rightCol=rightImageTableWidget->item(rightTableIndex,1)->text().toInt(&ok);
            rightLin=rightImageTableWidget->item(rightTableIndex,2)->text().toInt(&ok);
            rightView->moveTo(QPointF(rightCol,rightLin));
            rightDisplay->update();
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
}

void PTUserInterface_Qt::toggleInsertPointMode(bool insertionMode)
{
	this->insertionMode=insertionMode;
	if (insertionMode)
		insertPointInButton->setStatusTip("Insertion mode ON");
	else
		insertPointInButton->setStatusTip("Insertion mode OFF");

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
