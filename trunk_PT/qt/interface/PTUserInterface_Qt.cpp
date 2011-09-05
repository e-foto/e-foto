#include "PTUserInterface_Qt.h"
#include "WindowsSelectPage.h"

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
		delete(ptInst);
		ptInst = new PTUserInterface_Qt(ptManager);
	}
	return ptInst;
}

PTUserInterface_Qt::PTUserInterface_Qt(PTManager *manager, QWidget *parent, Qt::WindowFlags fl)
	:QMainWindow(parent, fl)
{
	setupUi(this);
    ptManager = manager;

	setWindowTitle("E-foto - Phototriangulation");

    connect(actionMove,SIGNAL(triggered()),this,SLOT(activePanMode()));
    connect(actionZoom,SIGNAL(triggered()),this,SLOT(activeZoomMode()));
    connect(actionFit_View,SIGNAL(triggered()),this,SLOT(fitView()));
    connect(actionView_Report, SIGNAL(triggered()), this, SLOT(viewReport()));
	connect(actionFoto_Tri,SIGNAL(triggered()), this, SLOT(showSelectionWindow()));
	connect(actionCalculateFotoTri,SIGNAL(triggered()),this,SLOT(showSelectionWindow()));

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

	leftImageString = images.at(0);
	rightImageString= images.at(1);


	leftImageComboBox->addItems(listImageLeft);
	rightImageComboBox->addItems(listImageRight);

	leftImageTableWidget->setColumnHidden(3,true);
	pointsTableWidget->setColumnHidden(5,true);
	rightImageTableWidget->setColumnHidden(3,true);

	/***/
	updateImageTable("leftImage",leftImageString);
/*
	Matrix pointsMatrix=ptManager->getENH();
	for (int i=0;i<ids.size();i++)
	{
		idPoints << QString(ids.at(i).c_str());
		typePoints << QString(types.at(i).c_str());
		keysPoints << QString(keys.at(i).c_str());
	}
	pointsTableWidget->putInColumn(idPoints,1);
	pointsTableWidget->putInColumn(typePoints,2);
	pointsTableWidget->putIn(pointsMatrix,0,2,'f',3);
	pointsTableWidget->putInColumn(keysPoints,6);
	pointsTableWidget->setHorizontalHeaderLabels(headerLabelsPoints);
	pointsTableWidget->resizeTable();
*/
	updateImageTable("rightImage",rightImageString);
/***/



	connect(leftImageComboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(updateImagesList(QString)));
	connect(rightImageComboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(updateImagesList(QString)));

	connect(leftImageTableWidget,SIGNAL(cellClicked(int,int)),this, SLOT(selectAllAppearances(int)));
	connect(rightImageTableWidget,SIGNAL(cellClicked(int,int)),this, SLOT(selectAllAppearances(int)));
	connect(pointsTableWidget,SIGNAL(cellClicked(int,int)),this, SLOT(selectAllAppearances(int)));
	
}

void PTUserInterface_Qt::closeEvent(QCloseEvent *event)
{
	LoadingScreen::instance().show();
	qApp->processEvents();
        //delete(myImageRightView);
		//delete(myImageLeftView);

	listAllImages.clear();
	listAllPoints.clear();
	listImageLeft.clear();
	listImageRight.clear();

	//delete selectionImagesView;
	//delete selectionPointsView;

	ptManager->returnProject();
	QMainWindow::closeEvent(event);
}

bool PTUserInterface_Qt::exec()
{
	//Matrix col=ptManager->getCol().transpose();
	//Matrix lin=ptManager->getLin().transpose();
	QStringList headerLabelsPoints,idPoints,typePoints,keysPoints;//,leftImageIdPoints, rightImageIdPoints;
	deque<string>  ids  = ptManager->getStringIdPoints();
	deque<string> types = ptManager->getStringTypePoints();
	deque<string> keys  = ptManager->getStringKeysPoints();
	headerLabelsPoints<<"Id"<<"Type"<<"E"<<"N"<<"H";//<<"Column"<<"Row";


	updateImageTable("leftImage",leftImageString);

	Matrix pointsMatrix=ptManager->getENH();
	for (int i=0;i<ids.size();i++)
	{
		idPoints << QString(ids.at(i).c_str());
		typePoints << QString(types.at(i).c_str());
		keysPoints << QString(keys.at(i).c_str());
	}
	pointsTableWidget->putInColumn(idPoints,1);
	pointsTableWidget->putInColumn(typePoints,2);
	pointsTableWidget->putIn(pointsMatrix,0,2,'f',3);
	pointsTableWidget->putInColumn(keysPoints,6);
	pointsTableWidget->setHorizontalHeaderLabels(headerLabelsPoints);
	pointsTableWidget->resizeTable();

	updateImageTable("rightImage",rightImageString);

	connect(leftImageTableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(updatePoint(QTableWidgetItem*)));
	connect(rightImageTableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(updatePoint(QTableWidgetItem*)));
	
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

void PTUserInterface_Qt::activeZoomMode()
{
		//myImageLeftView->setViewMode(3);
}

void PTUserInterface_Qt::fitView()
{
		//myImageLeftView->fitView();
}

void PTUserInterface_Qt::viewReport()
{
	//qDebug("Vendo Report");
	QStringList afpHeaderLabels;
	//omega, phi, kappa, X0, Y0, Z0;ÏÏÎº// ctrl+shift+u depois omega=03c9, phi=03c6	kappa=03ba
	afpHeaderLabels<< QString::fromUtf8("ω")<<QString::fromUtf8("φ")<<QString::fromUtf8("κ")<<"X0"<<"Y0"<<"Z0";


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

	QWidget *afpView = new QWidget();
    QHBoxLayout *horizontalLayout= new QHBoxLayout();
	ETableWidget *afpTable=  new ETableWidget(ptManager->getMatrixAFP(),'f',5);
    afpTable->setHorizontalHeaderLabels(afpHeaderLabels);
	ETableWidget *residuosTable = new ETableWidget(ptManager->getResiduos(),'f',8);
	residuosTable->setHorizontalHeaderLabels(QStringList(tr("Residuos")));

	horizontalLayout->addWidget(afpTable);
	horizontalLayout->addWidget(residuosTable);
    horizontalLayout->setStretchFactor(afpTable,3);
    horizontalLayout->setStretchFactor(residuosTable,1);

	QHBoxLayout *buttonsLayout= new QHBoxLayout();
	QPushButton *acceptButton= new QPushButton(tr("Accept"));
	QPushButton *discardButton= new QPushButton(tr("Discard"));
	buttonsLayout->addWidget(acceptButton);
	buttonsLayout->addWidget(discardButton);

	QVBoxLayout *reportLayout= new QVBoxLayout;
	reportLayout->addLayout(infoLayout);
	reportLayout->addLayout(horizontalLayout);
	reportLayout->addLayout(buttonsLayout);

	afpView->setLayout(reportLayout);
    afpView->show();

	connect(acceptButton,SIGNAL(clicked()),this,SLOT(acceptResults()));
	connect(acceptButton,SIGNAL(clicked()),afpView,SLOT(close()));
	connect(discardButton, SIGNAL(clicked()),afpView,SLOT(close()));

	afpView->setWindowModality(Qt::ApplicationModal);
}

bool PTUserInterface_Qt::calculatePT()
{

	ptManager->selectImages(selectionImagesView->getSelectedItens());
	ptManager->selectPoints(selectionPointsView->getSelectedItens());

	bool result = ptManager->calculatePT();
	viewReport();
	actionView_Report->setEnabled(true);
	return result;

}

void PTUserInterface_Qt::showSelectionWindow()
{
	QWidget *selectionView= new QWidget();
	//if (selectionImagesView==NULL || selectionPointsView==NULL)
	{
		//qDebug("Ponteiros nulos");
		selectionImagesView= new WindowsSelectPage(tr("Images available"),tr("Images selected"));
		selectionPointsView= new WindowsSelectPage(tr("Points available"),tr("Points selected"));
	}
	//if ((selectionImagesView!=NULL || selectionPointsView!=NULL))
	{
		//qDebug("Ponteiros nÃ£o nulos");
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
	lista=ptManager->getStringIdPoints();
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

	QVBoxLayout *layout= new QVBoxLayout(this);
	layout->addWidget(selectionImagesView);
	layout->addWidget(selectionPointsView);
	layout->addLayout(buttonsLayout);

	selectionView->setLayout(layout);

	connect(runButton,SIGNAL(clicked()),this,SLOT(calculatePT()));
	connect(runButton,SIGNAL(clicked()),selectionView,SLOT(close()));
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

	if(sender()->objectName()==leftImageComboBox->objectName())
	{
		leftImageString=imageFilename.toStdString().c_str();
		//qDebug("leftImage: %s",imageFilename.toStdString().c_str());
		QString currentRightImage=rightImageComboBox->currentText();
		rightImageComboBox->blockSignals(true);
		rightImageComboBox->clear();
		listImageRight=temp;
		rightImageComboBox->addItems(listImageRight);
		rightImageComboBox->setCurrentIndex(rightImageComboBox->findText(currentRightImage,Qt::MatchExactly));
		rightImageComboBox->blockSignals(false);
		updateImageTable("leftImage",leftImageString);


	}
	else if(sender()->objectName()==rightImageComboBox->objectName())
	{
		rightImageString= imageFilename.toStdString().c_str();
		//qDebug("RightImage: %s",imageFilename.toStdString().c_str());

		QString currentLefttImage=leftImageComboBox->currentText();
		leftImageComboBox->blockSignals(true);
		leftImageComboBox->clear();
		listImageLeft=temp;
		leftImageComboBox->addItems(listImageLeft);
		leftImageComboBox->setCurrentIndex(leftImageComboBox->findText(currentLefttImage,Qt::MatchExactly));
		leftImageComboBox->blockSignals(false);
		updateImageTable("rightImage",rightImageString);
	}
}

void PTUserInterface_Qt::updateImageTable(QString image, string imageFilename)
{
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
	if(image=="leftImage")
	{
		//qDebug()<<"chamou updateImageTable: "<< image << " : " << imageFilename;
		leftImageTableWidget->clearContents();
		leftImageTableWidget->setRowCount(0);
		leftImageTableWidget->putInColumn(idImagesPoints,1);
		leftImageTableWidget->putIn(imageColLin,0,1,'f',0);
		leftImageTableWidget->putInColumn(keysImagePoints,4);
		leftImageTableWidget->selectRow(findKeyAppearances("leftImageTable",QString::number(currentPointKey)));
	}else if (image == "rightImage")
	{
		//qDebug()<<"chamou updateImageTable: "<< image << " : " << imageFilename;
		rightImageTableWidget->clearContents();
		rightImageTableWidget->setRowCount(0);
		rightImageTableWidget->putInColumn(idImagesPoints,1);
		rightImageTableWidget->putIn(imageColLin,0,1,'f',0);
		rightImageTableWidget->putInColumn(keysImagePoints,4);
		rightImageTableWidget->selectRow(findKeyAppearances("rightImageTable",QString::number(currentPointKey)));
	}
}
//Seleciona em todas as tabelas onde o ponto aparece
void PTUserInterface_Qt::selectAllAppearances(int tableRow)
{
	bool ok;
	int indexLeftSearched,indexRightSearched, indexPointsSearched;
	if (sender()->objectName()==leftImageTableWidget->objectName())
	{
		QString searched=leftImageTableWidget->item(tableRow,3)->text();
		currentPointKey=searched.toInt(&ok);
		indexLeftSearched = tableRow;
		indexPointsSearched = findKeyAppearances("pointsTable", searched);
		indexRightSearched = findKeyAppearances("rightImageTable", searched);
	}
	else if (sender()->objectName()==pointsTableWidget->objectName())
	{
		QString searched=pointsTableWidget->item(tableRow,5)->text();
		currentPointKey=searched.toInt(&ok);
		indexLeftSearched=findKeyAppearances("leftImageTable",searched);
		indexPointsSearched=tableRow;
		indexRightSearched = findKeyAppearances("rightImageTable", searched);
	}
	else if (sender()->objectName()==rightImageTableWidget->objectName())
	{
		QString searched=rightImageTableWidget->item(tableRow,3)->text();
		currentPointKey=searched.toInt(&ok);
		indexLeftSearched=findKeyAppearances("leftImageTable",searched);
		indexPointsSearched = findKeyAppearances("pointsTable", searched);
		indexRightSearched =tableRow;
	}
	//Limpando seleção
	leftImageTableWidget->clearSelection();
	pointsTableWidget->clearSelection();
	rightImageTableWidget->clearSelection();

	//qDebug()<<QApplication::focusWidget()->objectName();
	if (indexLeftSearched>=0)
	{
		//leftImageTableWidget->setFocus(Qt::OtherFocusReason);
		leftImageTableWidget->selectRow(indexLeftSearched);
		leftImageTableWidget->setRangeSelected(QTableWidgetSelectionRange(indexLeftSearched,3,indexLeftSearched,3),false);
		//leftImageTableWidget->setFocusPolicy(Qt::FocusPolicy);
	}
	if (indexPointsSearched>=0)
	{
		//pointsTableWidget->setFocus(Qt::OtherFocusReason);
		pointsTableWidget->selectRow(indexPointsSearched);
		pointsTableWidget->setRangeSelected(QTableWidgetSelectionRange(indexPointsSearched,5,indexPointsSearched,5),false);
	}
	if (indexRightSearched>=0)
	{
		//rightImageTableWidget->setFocus(Qt::OtherFocusReason);
		rightImageTableWidget->selectRow(indexRightSearched);
		rightImageTableWidget->setRangeSelected(QTableWidgetSelectionRange(indexRightSearched,3,indexRightSearched,3),false);
	}
	//((QTableWidget*)sender())->setFocus();
}
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

void PTUserInterface_Qt::updateMark(string image,int imageKey, int pointKey, int col, int lin)
{
	if (image=="leftImage")
	{
		int pos=findKeyAppearances("leftImageTable", QString::number(pointKey));
		leftImageTableWidget->item(pos,1)->setText(QString::number(col));
		leftImageTableWidget->item(pos,2)->setText(QString::number(lin));
		//imageKey=ptManager->getImageId(leftImage);
	}else if(image =="rightImage")
	{
		int pos=findKeyAppearances("rightImageTable", QString::number(pointKey));
		rightImageTableWidget->item(pos,1)->setText(QString::number(col));
		rightImageTableWidget->item(pos,2)->setText(QString::number(lin));
		//imageKey=ptManager->getImageId(rightImage);
	}
	ptManager->updateDigitalCoordinatesPoint(imageKey,pointKey,col,lin );
}

/* Deixa o usuario entrar com o valor da linha e coluna na mao atualizando na interface. Atualmente desabilitada
void PTUserInterface_Qt::updatePoint(QTableWidgetItem *item)
{
	bool ok;
	int col=-1;
	int lin=-1;
	int rowPoint=item->row();
	int pointKey=item->tableWidget()->item(rowPoint,3)->text().toInt(&ok);

	if (item->tableWidget()->objectName()==leftImageTableWidget->objectName())
	{
		int imageKey=ptManager->getImageId(leftImageString);
		if(item->column()==1)
		{
			col=item->text().toInt(&ok);
			lin=leftImageTableWidget->item(item->row(),2)->text().toInt(&ok);
		}
		else if(item->column()==2)
		{
			lin=item->text().toInt(&ok);
			col=leftImageTableWidget->item(item->row(),1)->text().toInt(&ok);
		}
		ptManager->updateDigitalCoordinatesPoint(imageKey,pointKey,col,lin);
		qDebug("col %d lin %d",col,lin);
	} else if (item->tableWidget()->objectName()==rightImageTableWidget->objectName())
	{
		int imageKey=ptManager->getImageId(rightImageString);
		if(item->column()==1)
		{
			col=item->text().toInt(&ok);
			lin=rightImageTableWidget->item(item->row(),2)->text().toInt(&ok);
		}
		else if(item->column()==2)
		{
			lin=item->text().toInt(&ok);
			col=rightImageTableWidget->item(item->row(),1)->text().toInt(&ok);
		}
		ptManager->updateDigitalCoordinatesPoint(imageKey,pointKey,col,lin);
		qDebug("col %d lin %d",col,lin);
	}

}
*/

void PTUserInterface_Qt::acceptResults()
{
	// salvar resultados no xml
}
