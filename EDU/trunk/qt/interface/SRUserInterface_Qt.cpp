#include "SRUserInterface_Qt.h"
#include <QPushButton>

#include "ETableWidget.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

SRUserInterface_Qt* SRUserInterface_Qt::srInst = NULL;

SRUserInterface_Qt* SRUserInterface_Qt::instance(SRManager* manager)
{

	if (srInst != NULL)
	{
		delete(srInst);
		srInst = NULL;
	}
	if (srInst == NULL)
	{
		delete(srInst);
		srInst = new SRUserInterface_Qt(manager);
	}
	return srInst;
}

SRUserInterface_Qt::SRUserInterface_Qt(SRManager* manager, QWidget* parent, Qt::WindowFlags fl)
	: QMainWindow(parent, fl)
{
	setupUi(this);
	actionSet->setCheckable(true);
	actionUnset->setCheckable(true);
	actionMove->setCheckable(true);
	actionZoom->setCheckable(true);

	QActionGroup *group = new QActionGroup(this);
	group->addAction(actionSet);
	group->addAction(actionUnset);
	group->addAction(actionMove);
	group->addAction(actionZoom);
	actionSet->setVisible(false);
	actionInsert_Points->setVisible(false);
	actionMove->setVisible(false);
	actionZoom->setVisible(false);
	actionFit->setVisible(false);
	group->setExclusive(true);
	actionGrid->setCheckable(true);
	actionTable->setEnabled(false);
	actionFlight->setEnabled(false);
	actionSpatialRessection->setEnabled(false);
	table1->setEditTriggers(QAbstractItemView::NoEditTriggers);

	//Tem que rever esse primeiro connect... o calculate da SR não é direto, tem que abrir a tela pra escolher iterações e precisão.
	QObject::connect(actionSpatialRessection, SIGNAL(triggered()), this, SLOT(calculateSR()));
	//QObject::connect(actionSet, SIGNAL(triggered()), this, SLOT(activeSetMode()));
	//QObject::connect(actionUnset, SIGNAL(triggered()), this, SLOT(activeUnsetMode()));
	//QObject::connect(actionMove, SIGNAL(triggered()), this, SLOT(activePanMode()));
	//QObject::connect(actionZoom, SIGNAL(triggered()), this, SLOT(activeZoomMode()));
	//QObject::connect(actionFit, SIGNAL(triggered()), this, SLOT(fitView()));
	QObject::connect(actionFlight, SIGNAL(triggered()), this, SLOT(setFlight()));
	QObject::connect(actionTable, SIGNAL(triggered()), this, SLOT(viewReport()));
        QObject::connect(actionDone, SIGNAL(triggered()), this, SLOT(EOdone()));

	this->manager = manager;
	if (manager->exteriorDone())
		actionTable->setEnabled(true);

	flightAvailable = false;
	setWindowState(this->windowState() | Qt::WindowMaximized);
	qApp->processEvents();
	init();
}

SRUserInterface_Qt::~SRUserInterface_Qt()
{
	// no need to delete child widgets, Qt does it all for us
}


void SRUserInterface_Qt::languageChange()
{
	retranslateUi(this);
}

void SRUserInterface_Qt::init()
{
	// Insert image into layout
	QWidget* centralwidget = new QWidget(this);

	QGridLayout* gridLayout = new QGridLayout(centralwidget);

	//oldImageView = new ImageView(centralwidget);
	imageView = new SingleViewer();
	imageView->blockOpen();
	imageView->blockSave();
	imageView->addToolBar(Qt::TopToolBarArea,toolBar);

	//gridLayout->addWidget(oldImageView, 0, 0, 1, 1);
	gridLayout->addWidget(imageView, 0, 0, 1, 1);
	setCentralWidget(centralwidget);

        flightDirectionForm = new FlightDirectionForm();
        flightDirectionForm->imagesFlightDirectionCombo->setHidden(true);
        flightDirectionForm->imageLabel->setHidden(true);
        connect(flightDirectionForm,SIGNAL(valuesFlightDirectionForm(QString,double)),this,SLOT(setFlightDirection(QString,double)));

	markOn = new Marker(SymbolsResource::getTriangle(Qt::green, Qt::transparent, QSize(24, 24), 2, true)); // Personalizando as marcas. Que no futuro eu quero melhorar para inserir uso de 2 ou 3 marcas de acordo com o tipo de ponto.
	markOff = new Marker(SymbolsResource::getTriangle(Qt::darkRed, Qt::transparent, QSize(24, 24), 2, true)); // Personalizando as marcas. Que no futuro eu quero melhorar para inserir uso de 2 ou 3 marcas de acordo com o tipo de ponto.
	//imageView->getMarker()->changeMarker(mark);

	// Make some connections
	imageView->getMarker()->setToOnlyEmitClickedMode();
	connect(imageView->getMarker(),SIGNAL(clicked(QPointF)), this, SLOT(receivePoint(QPointF)));
	//connect (oldImageView, SIGNAL(mousePressed()), this, SLOT(informState()));
	//connect (oldImageView, SIGNAL(markClicked(QPoint)), this, SLOT(receivePoint(QPoint)));
	//connect (oldImageView, SIGNAL(changed()), this, SLOT(makeRepaint()));

	//REVER
	//connect (oldImageView, SIGNAL(flightDirectionClicked(QPoint)), this, SLOT(receiveFlightDirection(QPoint)));

	//this->showNormal();
	//oldImageView->fitView();
}

void SRUserInterface_Qt::informState()
{
	//if (oldImageView->getViewMode() != 4)
		//oldImageView->selectPoint(QString::number(table1->currentIndex().row()).toStdString());
	//else
		//oldImageView->selectPoint("-1");
}

void SRUserInterface_Qt::receivePoint(QPointF p)
{
	if (p.x() < 0 || p.y() < 0)
		return;
    imageView->getMarker()->insertMark(p, table1->currentIndex().row()+1, points->data(points->index(table1->currentIndex().row(), 2)).toString(), markOn);

    points->setData(points->index(table1->currentIndex().row(), 7), QVariant(p.x()));
    points->setData(points->index(table1->currentIndex().row(), 8), QVariant(p.y()));
    points->setData(points->index(table1->currentIndex().row(), 9), QVariant(manager->pointToDetector(p.x(), p.y()).at(0)));
    points->setData(points->index(table1->currentIndex().row(), 10), QVariant(manager->pointToDetector(p.x(), p.y()).at(1)));
    points->setData(points->index(table1->currentIndex().row(), 11), QVariant(true));
    points->item(table1->currentIndex().row(),2)->setCheckState(Qt::Checked);

	measurePoint(points->data(points->index(table1->currentIndex().row(), 0)).toInt(),p.x(),p.y());

	table1->selectRow(table1->currentIndex().row() + 1);
	testActivateSR();
}

void SRUserInterface_Qt::setFlightDirection(QString imageFile, double kappa0)
{
        manager->flightDirection(kappa0);
	flightAvailable = true;
	actionSpatialRessection->setEnabled(true);
}

void SRUserInterface_Qt::updateSelection(QStandardItem *item)
{
    if (item->column() == 2)
	{
        if (points->data(points->index(item->row(),11)).toBool())
		{
			if (item->checkState() == Qt::Checked)
			{
				manager->selectPoint(points->data(points->index(item->row(),0)).toInt());
			}
			else
			{
				manager->unselectPoint(points->data(points->index(item->row(),0)).toInt());
			}
                        updateDisplayedPoints();
			makeRepaint();
			testActivateSR();
		}
		else
		{
			if (item->checkState() == Qt::Checked)
			{
				item->setCheckState(Qt::Unchecked);
			}
		}
	}
}

void SRUserInterface_Qt::updateAll()
{
    for (int i=0; i<points->rowCount(); i++)
    {
        if (points->item(i,2)->checkState() == Qt::Checked)
            manager->selectPoint(points->data(points->index(i,0)).toInt());
        else
            manager->unselectPoint(points->data(points->index(i,0)).toInt());
    }
    updateDisplayedPoints();
    makeRepaint();
    testActivateSR();
}

void SRUserInterface_Qt::makeRepaint()
{
	imageView->repaint();
	table1->repaint();
}

void SRUserInterface_Qt::activeSetMode()
{
	//oldImageView->setViewMode(1);
}

void SRUserInterface_Qt::activeUnsetMode()
{
	//oldImageView->setViewMode(4);
}

void SRUserInterface_Qt::activePanMode()
{
	//oldImageView->setViewMode(2);
}

void SRUserInterface_Qt::activeZoomMode()
{
	//oldImageView->setViewMode(3);
}

void SRUserInterface_Qt::fitView()
{
	//oldImageView->fitView();
}

bool SRUserInterface_Qt::measurePoint(int id, double col, double lin)
{
	return manager->measurePoint(id, col, lin);
}

void SRUserInterface_Qt::closeEvent(QCloseEvent *e)
{
	LoadingScreen::instance().show();
	qApp->processEvents();
	delete(imageView);
	delete(markOn);
	delete(markOff);
	manager->returnProject();
	QMainWindow::closeEvent(e);
}

bool SRUserInterface_Qt::calculateSR()
{
	int iterations; double gnssPrecision, insPrecision;
        iterations = QInputDialog::getInt(this,"Max. of iterations","Set max. number of iterations",10,1,50,1);
	gnssPrecision = QInputDialog::getDouble(this,"Precision","Set the precision for camera coordinates (meters)",0.001,0.000001,10,6);
	insPrecision = QInputDialog::getDouble(this,"Precision","Set the precision for attitude angles (radians)",0.001,0.000001,1,6);
	bool result = manager->calculateSR(iterations, gnssPrecision, insPrecision);
	viewReport();
	actionTable->setEnabled(true);
	return result;
}


bool SRUserInterface_Qt::viewReport()
{
	deque<string> myValues = manager->makeReport();

	windowReport = new QWidget();
	QVBoxLayout *myLayout = new QVBoxLayout();
	QHBoxLayout* upperLayout = new QHBoxLayout();
	QTabWidget *myTab = new QTabWidget();

	/* novo*/
	QLabel *xa= new QLabel("<font size=5>Xa");
	xa->setTextFormat(Qt::RichText);
	xa->setAlignment(Qt::AlignHCenter);

	QLabel *mvcxa= new QLabel("<font size=5>MVC(Xa)");
	mvcxa->setTextFormat(Qt::RichText);
	mvcxa->setAlignment(Qt::AlignHCenter);
	/**/

	QWidget *XaView= new QWidget();
	QHBoxLayout *XaLayout= new QHBoxLayout(); // layout contendo as tabelas
	/* novo */
	QVBoxLayout *xalabelLayout= new QVBoxLayout();// layout contendo o label e a tabela de Xa
	QVBoxLayout *mvclabelLayout= new QVBoxLayout();// layout contendo o label e a tabela de mvc(Xa)
	/**/

	ETableWidget *myXaView = new ETableWidget();

	QStringList listaXa;
	listaXa<<"X0"<<"Y0"<<"Z0"<<QString::fromUtf8("φ")<<QString::fromUtf8("ω")<<QString::fromUtf8("κ");
	myXaView->setRowCount(listaXa.size());
	myXaView->setVerticalHeaderLabels(listaXa);
	myXaView->setTableData(myValues.at(0),'f',5);

	/* novo */
	xalabelLayout->addWidget(xa);
	xalabelLayout->addWidget(myXaView);
	/**/

	ETableWidget *mySXaView = new ETableWidget();
	mySXaView->setTableData(myValues.at(4),'f',5);
	/* novo */
	mvclabelLayout->addWidget(mvcxa);
	mvclabelLayout->addWidget(mySXaView);
	/* novo */
	XaLayout->addLayout(xalabelLayout);
	XaLayout->addLayout(mvclabelLayout);

	XaView->setLayout(XaLayout); // noivo
	myTab->addTab(XaView, QString::fromUtf8("Xa"));
	connect(myXaView,SIGNAL(focusReceived()),mySXaView,SLOT(clearSelection()));
	connect(mySXaView,SIGNAL(focusReceived()),myXaView,SLOT(clearSelection()));

	// Aba La

	QWidget *LaView = new QWidget();
	QHBoxLayout *LaLayout= new QHBoxLayout();

	/* novo*/
	QLabel *la= new QLabel("<font size=5>La");
	la->setTextFormat(Qt::RichText);
	la->setAlignment(Qt::AlignHCenter);

	QLabel *mvcLa= new QLabel("<font size=5>MVC(La)");
	mvcLa->setTextFormat(Qt::RichText);
	mvcLa->setAlignment(Qt::AlignHCenter);
	/**/
	QVBoxLayout *laLabelLayout= new QVBoxLayout();// layout contendo o label e a tabela de Xa
	QVBoxLayout *mvcLaLabelLayout= new QVBoxLayout();// layout contendo o label e a tabela de mvc(Xa)


	ETableWidget *myLaView = new ETableWidget();
	myLaView->setTableData(myValues.at(1),'f',5);

	laLabelLayout->addWidget(la);
	laLabelLayout->addWidget(myLaView);

	ETableWidget *mySLaView = new ETableWidget();
	mySLaView->setTableData(myValues.at(5),'f',5);

	mvcLaLabelLayout->addWidget(mvcLa);
	mvcLaLabelLayout->addWidget(mySLaView);

	LaLayout->addLayout(laLabelLayout);
	LaLayout->addLayout(mvcLaLabelLayout);

	LaView->setLayout(LaLayout);
	if (myLaView->rowCount() !=0)
		myTab->addTab(LaView, QString::fromUtf8("La"));


	// Aba V dos Residuos

	QWidget *sigmaView = new QWidget();
	QHBoxLayout *sigmaLayout= new QHBoxLayout();
	QVBoxLayout *formulasLayout= new QVBoxLayout();

	QString aux1=QString::fromUtf8("<font size=5>σ<sub>0</sub><sup>2</sup>=V<sup>T</sup>PV/(n-m)");//σ² hexadecimal 03C3 0342//\sigma_{0}^{2} = frac{V^{T} * P * V}{n - m}\F
	aux1+="=";
	aux1+=QString::number(Conversion::stringToDouble(myValues.at(2)),'f',6);
    aux1+=QString::fromUtf8(" mm<sup>2</sup></font>");
	QLabel* myValueLabel1 = new QLabel(aux1);
	myValueLabel1->setTextFormat(Qt::RichText);

	QString aux2=QString::fromUtf8("<font size=5>σ<sub>0</sub>");//σ² hexadecimal 03C3 0342//\sigma_{0}^{2} = frac{V^{T} * P * V}{n - m}\F
	aux2+="=";
	aux2+=QString::number(sqrt(Conversion::stringToDouble(myValues.at(2))),'f',6);
    aux2+=" mm</font>";
	QLabel* myValueLabel2 = new QLabel(aux2);
	myValueLabel2->setTextFormat(Qt::RichText);

	formulasLayout->addWidget(myValueLabel1);
	formulasLayout->addWidget(myValueLabel2);

	ETableWidget *myVView = new ETableWidget();
	myVView->setTableData(myValues.at(3),'f',5);

	sigmaLayout->addWidget(myVView);
	sigmaLayout->addLayout(formulasLayout);

	sigmaLayout->setSpacing(QHBoxLayout::SetMinimumSize);
	sigmaView->setLayout(sigmaLayout);
	myTab->addTab(sigmaView, QString::fromUtf8("V"));



	QString itString("Iterations: ");
	itString += myValues.at(6).c_str();
	QLabel* itLabel = new QLabel(itString);
	QString conString("Converged: ");
	conString += myValues.at(7).c_str();
	QLabel* conLabel = new QLabel(conString);
	upperLayout->addWidget(itLabel);
	upperLayout->addStretch();
	upperLayout->addWidget(conLabel);

	QPushButton *acceptButton = new QPushButton("&Accept", this);
	QObject::connect(acceptButton, SIGNAL(clicked()), this, SLOT(acceptSR()));

	myLayout->addLayout(upperLayout);
	myLayout->addWidget(myTab);
	myLayout->addWidget(acceptButton);

	windowReport->setLayout(myLayout);
	windowReport->setWindowModality(Qt::ApplicationModal);
	windowReport->setWindowTitle("Exterior Orientation Report");
	windowReport->show();

	return true;
}

void SRUserInterface_Qt::testActivateSR()
{
	if(manager->countSelectedPoints() >= 4)
	{
		actionFlight->setEnabled(true);
		if (flightAvailable)
		{
			actionSpatialRessection->setEnabled(true);
		}
	}
	else
	{
		actionFlight->setEnabled(false);
		actionSpatialRessection->setEnabled(false);
	}
}

void SRUserInterface_Qt::setFlight()
{
	flightDirectionForm->setGeometry((this->x()+this->width())/2,(this->y()+this->height())/2,flightDirectionForm->width(),flightDirectionForm->height());
	flightDirectionForm->show();
}

void SRUserInterface_Qt::acceptSR()
{
	manager->acceptSR();
	windowReport->close();
}

void SRUserInterface_Qt::updateDisplayedPoints()
{
	for (int row = 0; row < points->rowCount() ;row++)
	{
        if (points->item(row,7)->text().isEmpty() || points->data(points->index(row, 1)).toString() == "false")
			continue;
        QString pointName = points->data(points->index(row, 2)).toString();
        QPointF location(points->item(row,7)->text().toDouble(),points->item(row,8)->text().toDouble());
        if (points->item(row,2)->checkState() == Qt::Checked)
			imageView->getMarker()->insertMark(location, row+1, pointName, markOn);
		else
			imageView->getMarker()->insertMark(location, row+1, pointName, markOff);
	}
}

void SRUserInterface_Qt::EOdone()
{
    close();
}

bool SRUserInterface_Qt::exec()
{
    //int numberOfPoints = manager->listImagePoints().size();
    int numberOfPoints = manager->listAllPoints().size();

    points = new QStandardItemModel(numberOfPoints, 12);
    points->setHeaderData(0, Qt::Horizontal, QVariant("Number"));
    points->setHeaderData(1, Qt::Horizontal, QVariant("isControl"));
    points->setHeaderData(2, Qt::Horizontal, QVariant("Id"));
    points->setHeaderData(3, Qt::Horizontal, QVariant("Description"));
    points->setHeaderData(4, Qt::Horizontal, QVariant("E"));
    points->setHeaderData(5, Qt::Horizontal, QVariant("N"));
    points->setHeaderData(6, Qt::Horizontal, QVariant("H"));
    points->setHeaderData(7, Qt::Horizontal, QVariant("col"));
    points->setHeaderData(8, Qt::Horizontal, QVariant("lin"));
    points->setHeaderData(9, Qt::Horizontal, QVariant(QChar(0x03BE)));
    points->setHeaderData(10, Qt::Horizontal, QVariant(QChar(0x03B7)));
    points->setHeaderData(11, Qt::Horizontal, QVariant("Used"));
    table1->setModel(points);
    table1->setColumnHidden(0,true);
    table1->setColumnHidden(1,true);
    table1->setColumnHidden(11,true);

    for (int row = 0; row < numberOfPoints; row++)
    {
        deque<string> pointData = manager->pointData(row);

        for (unsigned int col = 0; col < pointData.size(); col++)
        {
            QStandardItem* item = new QStandardItem(QString(pointData.at(col).c_str()));

            if (col == 2)
                item->setCheckable(true);

            points->setItem(row, col, item);
        }

        for (int col = pointData.size(); col < 11; col++)
        {
            QStandardItem* item = new QStandardItem(QString(""));
            points->setItem(row, col, item);
        }

        QStandardItem* item = new QStandardItem();
        points->setItem(row, 11, item);

        if (points->data(points->index(row,7)).toString() != "")
        {
            points->setData(points->index(row, 11), QVariant(true));
        }
        else
        {
            points->setData(points->index(row, 11), QVariant(false));
        }

        if (pointData.at(1) == "false")
            table1->setRowHidden(row,true);

        // Check if point was measured and check current row - fix by Marcelo Teixeira Silveira
        if ((!points->item(row, 7)->text().isEmpty()) && (!points->item(row, 8)->text().isEmpty()) && (!points->item(row, 9)->text().isEmpty()) && (!points->item(row, 10)->text().isEmpty()))
            points->item(row,2)->setCheckState(Qt::Checked);
    }

    table1->selectRow(0);
    table1->setFocus();

    connect (points, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(updateSelection(QStandardItem*)));

    this->show();
    LoadingScreen::instance().close();
    qApp->processEvents();
    //oldImageView = new ImageView(centralwidget);
    imageView->loadImage(QString(manager->getImageFile().c_str()));
    {
        //oldImageView->createPoints(points,2);
        //oldImageView->drawPoints(points,2);
        //oldImageView->fitView();
    }
    updateDisplayedPoints();
    updateAll();
    makeRepaint();
    actionMove->trigger();

    //LoadingScreen::instance().close();
    //if (qApp->exec())
    //return false;
    //delete(oldImageView);
    return true;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
