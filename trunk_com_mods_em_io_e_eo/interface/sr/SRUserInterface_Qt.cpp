#include "SRUserInterface_Qt.h"
#include <QPushButton>

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
    group->setExclusive(true);
    actionGrid->setCheckable(true);
    actionTable->setEnabled(false);
    actionFlight->setEnabled(false);
    actionSpatialRessection->setEnabled(false);
    table1->setEditTriggers(QAbstractItemView::NoEditTriggers);
	points = NULL;
	pointsOccurrenceList = NULL;
	table2PointsOccurrence = NULL;

    //Tem que rever esse primeiro connect... o calculate da SR não é direto, tem que abrir a tela pra escolher iterações e precisão.
    QObject::connect(actionSpatialRessection, SIGNAL(triggered()), this, SLOT(calculateSR()));
    QObject::connect(actionSet, SIGNAL(triggered()), this, SLOT(activeSetMode()));
    QObject::connect(actionUnset, SIGNAL(triggered()), this, SLOT(activeUnsetMode()));
    QObject::connect(actionMove, SIGNAL(triggered()), this, SLOT(activePanMode()));
    QObject::connect(actionZoom, SIGNAL(triggered()), this, SLOT(activeZoomMode()));
    QObject::connect(actionFit, SIGNAL(triggered()), this, SLOT(fitView()));
	QObject::connect(actionFlight, SIGNAL(triggered()), this, SLOT(setFlight()));
	QObject::connect(actionTable, SIGNAL(triggered()), this, SLOT(viewReport()));
	QObject::connect(actionInsert_Points, SIGNAL(triggered()), this, SLOT(activeInsertPoint()));

    this->manager = manager;
	if (manager->exteriorDone())
		actionTable->setEnabled(true);

    flightAvailable = false;
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

	//myImageView = new ImageView(QString(manager->getImageFile().c_str()), centralwidget);
	//myImageView->setFocusPolicy(Qt::NoFocus);
	myImageView = new ImageView(centralwidget);

    gridLayout->addWidget(myImageView, 0, 0, 1, 1);

    setCentralWidget(centralwidget);
	resize(1024,768);

	// Make some connections
	connect (myImageView, SIGNAL(mousePressed()), this, SLOT(informState()));
	connect (myImageView, SIGNAL(markClicked(QPoint)), this, SLOT(receivePoint(QPoint)));
	connect (myImageView, SIGNAL(flightDirectionClicked(QPoint)), this, SLOT(receiveFlightDirection(QPoint)));
	connect (myImageView, SIGNAL(changed()), this, SLOT(makeRepaint()));

	//this->showNormal();
	//myImageView->fitView();
}

void SRUserInterface_Qt::loadPointsTable()
{
	int numberOfPoints = manager->listImagePoints().size();
	if (points != NULL)
		delete points;
	points = new QStandardItemModel(numberOfPoints, 11);
	for (int row = 0; row < numberOfPoints; row++)
	{
		deque<string> pointData = manager->pointData(row);
		for (unsigned int col = 0; col < pointData.size(); col++)
		{
			QStandardItem* item = new QStandardItem(QString(pointData.at(col).c_str()));
			if (col == 1)
			{
				item->setCheckable(true);
			}
			points->setItem(row, col, item);
		}
		for (int col = pointData.size(); col < 10; col++)
		{
			QStandardItem* item = new QStandardItem(QString(""));
			points->setItem(row, col, item);
		}
		QStandardItem* item = new QStandardItem();
		points->setItem(row, 10, item);
		if (points->data(points->index(row,6)).toString() != "")
		{
			points->setData(points->index(row, 10), QVariant(true));
		}
		else
		{
			points->setData(points->index(row, 10), QVariant(false));
		}
	}
	points->setHeaderData(0, Qt::Horizontal, QVariant("Number"));
	points->setHeaderData(1, Qt::Horizontal, QVariant("Id"));
	points->setHeaderData(2, Qt::Horizontal, QVariant("Description"));
	points->setHeaderData(3, Qt::Horizontal, QVariant("E"));
	points->setHeaderData(4, Qt::Horizontal, QVariant("N"));
	points->setHeaderData(5, Qt::Horizontal, QVariant("H"));
	points->setHeaderData(6, Qt::Horizontal, QVariant("col"));
	points->setHeaderData(7, Qt::Horizontal, QVariant("lin"));
	points->setHeaderData(8, Qt::Horizontal, QVariant(QChar(0x03BE)));
	points->setHeaderData(9, Qt::Horizontal, QVariant(QChar(0x03B7)));
	points->setHeaderData(10, Qt::Horizontal, QVariant("Used"));
	table1->setModel(points);
	table1->setColumnHidden(0,true);
	table1->setColumnHidden(10,true);
	table1->selectRow(0);
	table1->setFocus();
}

void SRUserInterface_Qt::informState()
{
	if (myImageView->getViewMode() != 4)
		myImageView->selectPoint(QString::number(table1->currentIndex().row()).toStdString());
	else
		myImageView->selectPoint("-1");
}

void SRUserInterface_Qt::receivePoint(QPoint p)
{
    points->setData(points->index(table1->currentIndex().row(), 6), QVariant(p.x()));
    points->setData(points->index(table1->currentIndex().row(), 7), QVariant(p.y()));
    points->setData(points->index(table1->currentIndex().row(), 8), QVariant(manager->pointToAnalog(p.x(), p.y()).at(0)));
    points->setData(points->index(table1->currentIndex().row(), 9), QVariant(manager->pointToAnalog(p.x(), p.y()).at(1)));
    points->setData(points->index(table1->currentIndex().row(), 10), QVariant(true));
    points->item(table1->currentIndex().row(),1)->setCheckState(Qt::Checked);
	measurePoint(points->data(points->index(table1->currentIndex().row(), 0)).toInt(),p.x(),p.y());
    table1->selectRow(table1->currentIndex().row() + 1);
    testActivateSR();
}

void SRUserInterface_Qt::receiveFlightDirection(QPoint p)
{
    myImageView->drawFlightDirection(p.x(),p.y());
    manager->flightDirection(p.x(),p.y());
    flightAvailable = true;

    actionSpatialRessection->setEnabled(true);
    toolBar->setEnabled(true);
	dockWidget->setEnabled(true);
}

void SRUserInterface_Qt::actualizeSelection(QStandardItem *item)
{
    if (item->column() == 1)
    {
        if (points->data(points->index(item->row(),10)).toBool())
        {
            if (item->checkState() == Qt::Checked)
            {
                manager->selectPoint(points->data(points->index(item->row(),0)).toInt());
            }
            else
            {
                manager->unselectPoint(points->data(points->index(item->row(),0)).toInt());
            }
            myImageView->drawPoints(points,2);
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

void SRUserInterface_Qt::makeRepaint()
{
    myImageView->repaint();
    table1->repaint();
}

void SRUserInterface_Qt::activeSetMode()
{
    myImageView->setViewMode(1);
}

void SRUserInterface_Qt::activeUnsetMode()
{
    myImageView->setViewMode(4);
}

void SRUserInterface_Qt::activePanMode()
{
    myImageView->setViewMode(2);
}

void SRUserInterface_Qt::activeZoomMode()
{
	myImageView->setViewMode(3);
}

void SRUserInterface_Qt::activeInsertPoint()
{
	if (imagePoints.size() == 0)
	{
		allPoints = manager->listAllPoints();
		imagePoints = manager->listImagePoints();
	}
	QPushButton* confirmButton = new QPushButton(tr("confirm"));
	QPushButton* cancelButton = new QPushButton(tr("cancel"));
	QLabel* message = new QLabel(tr("Please check or uncheck points to insert or delete the occurrence of the same about this image"));
	if (table2PointsOccurrence != NULL)
		delete table2PointsOccurrence;
	table2PointsOccurrence = new QTableWidget;
	QVBoxLayout* layout = new QVBoxLayout;
	QHBoxLayout* hlayout = new QHBoxLayout;
	if (pointsOccurrenceList != NULL)
		delete pointsOccurrenceList;
	pointsOccurrenceList = new QWidget;

	table2PointsOccurrence->setRowCount(manager->listAllPoints().size());
	table2PointsOccurrence->setColumnCount(3);
	table2PointsOccurrence->setHorizontalHeaderLabels(QString("Key.Point Id.Description").split("."));
	table2PointsOccurrence->setColumnHidden(0,true);
	table2PointsOccurrence->horizontalHeader()->setStretchLastSection(true);
	int totalOfPoints = manager->listAllPoints().size();
	for (int i=0;i<totalOfPoints;i++)
	{
		QStringList point;
		bool inserted = false;
		if (i<imagePoints.size())
		{
			point = QString(imagePoints.at(i).c_str()).split(" ");
			inserted = true;
			for (int j=0;j<allPoints.size();j++)
			{
				if (point.at(0) == QString(allPoints.at(j).c_str()).split(" ").at(0))
				{
					allPoints.erase(allPoints.begin()+j);
					break;
				}
			}
		}
		else
		{
			point = QString(allPoints.at(i-imagePoints.size()).c_str()).split(" ");
		}
		QTableWidgetItem *temp0= new QTableWidgetItem(point.at(0));
		temp0->setTextAlignment(Qt::AlignCenter);
		temp0->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		table2PointsOccurrence->setItem(i,0,temp0);
		QTableWidgetItem *temp1= new QTableWidgetItem(point.at(1));
		temp1->setTextAlignment(Qt::AlignCenter);
		temp1->setCheckState(inserted ? Qt::Checked : Qt::Unchecked);
		temp1->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
		table2PointsOccurrence->setItem(i,1,temp1);
		point.removeFirst();
		point.removeFirst();
		QTableWidgetItem *temp2 = new QTableWidgetItem(point.join(" "));
		temp2->setTextAlignment(Qt::AlignCenter);
		temp2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		table2PointsOccurrence->setItem(i,2,temp2);
	}
	hlayout->addWidget(message);
	hlayout->addWidget(confirmButton);
	hlayout->addWidget(cancelButton);
	layout->addWidget(table2PointsOccurrence);
	layout->addLayout(hlayout);
	pointsOccurrenceList->setLayout(layout);
	pointsOccurrenceList->setWindowModality(Qt::ApplicationModal);
	connect(cancelButton,SIGNAL(clicked()),pointsOccurrenceList,SLOT(close()));
	connect(confirmButton,SIGNAL(clicked()),this,SLOT(insertPoint()));
	pointsOccurrenceList->show();
}

void SRUserInterface_Qt::fitView()
{
    myImageView->fitView();
}

void SRUserInterface_Qt::insertPoint()
{
	int otherPointsCount = allPoints.size();
	int imagePointsCount = imagePoints.size();
	imagePoints.clear();
	allPoints.clear();
	for (int i = 0; i<imagePointsCount; i++)
	{
		if (table2PointsOccurrence->item(i,1)->checkState() == Qt::Unchecked)
		{
			bool ok;
			int key = table2PointsOccurrence->item(i,0)->text().toInt(&ok);
			if (ok)
			{
				manager->removePoint(key);
				manager->removePointFromImage(key);
			}
		}
	}
	for (int i = imagePointsCount; i<imagePointsCount+otherPointsCount; i++)
	{
		if (table2PointsOccurrence->item(i,1)->checkState() == Qt::Checked)
		{
			bool ok;
			int key = table2PointsOccurrence->item(i,0)->text().toInt(&ok);
			if (ok)
			{
				manager->insertPointOnImage(key);
			}
		}
	}
	loadPointsTable();
	myImageView->clearPoints();
	myImageView->createPoints(points,2);
	myImageView->drawPoints(points,2);
	pointsOccurrenceList->close();
}

bool SRUserInterface_Qt::measurePoint(int id, int col, int lin)
{
    return manager->measurePoint(id, col, lin);
}

bool SRUserInterface_Qt::calculateSR()
{
	int iterations; double gnssPrecision, insPrecision;
	iterations = QInputDialog::getInt(this,"Number of iterations","Set the number of iteration",10,1,50,1);
	gnssPrecision = QInputDialog::getDouble(this,"Precision","Set the center point precision",0.001,0.000001,10,6);
	insPrecision = QInputDialog::getDouble(this,"Precision","Set the atitude precision",0.001,0.000001,1,6);
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

    QWidget *XaView= new QWidget();
    QHBoxLayout *XaLayout= new QHBoxLayout();
    TableIOEOWidget *myXaView = new TableIOEOWidget();
    myXaView->setTableData(myValues.at(0));
    TableIOEOWidget *mySXaView = new TableIOEOWidget();
    mySXaView->setTableData(myValues.at(4));
    XaLayout->addWidget(myXaView);
    XaLayout->addWidget(mySXaView);
    XaView->setLayout(XaLayout);
    myTab->addTab(XaView, QString::fromUtf8("Xa"));
    connect(myXaView,SIGNAL(focusReceived()),mySXaView,SLOT(clearSelection()));
    connect(mySXaView,SIGNAL(focusReceived()),myXaView,SLOT(clearSelection()));

    QWidget *LbView = new QWidget();
    QHBoxLayout *LbLayout= new QHBoxLayout();
    TableIOEOWidget *myLbView = new TableIOEOWidget();
    myLbView->setTableData(myValues.at(1));
    TableIOEOWidget *mySLbView = new TableIOEOWidget();
    mySLbView->setTableData(myValues.at(5));
    LbLayout->addWidget(myLbView);
    LbLayout->addWidget(mySLbView);
    LbView->setLayout(LbLayout);
    myTab->addTab(LbView, QString::fromUtf8("Lb"));


    QWidget *sigmaView = new QWidget();
    QHBoxLayout *sigmaLayout= new QHBoxLayout();

    QString aux=QString::fromUtf8("<font size=12>σ<sub>0</sub><sup>2</sup>=V<sup>T</sup>PV");//σ² hexadecimal 03C3 0342//\sigma_{0}^{2} = frac{V^{T} * P * V}{n - m}\F
  /*QString aux2=QString::fromUtf8("<font size=12>n-m</font>");

    QLabel *auxLabel = new QLabel(aux);
    QLabel *aux2Label= new QLabel(aux2);
    QLine *linha= QLine();
    linha->setLine();
    */
    aux+="=";
    aux+=QString::fromStdString(myValues.at(2));
    aux+="</font>";

    QLabel* myValueLabel = new QLabel(aux);
    myValueLabel->setTextFormat(Qt::RichText);

    TableIOEOWidget *myVView = new TableIOEOWidget();
    myVView->setTableData(myValues.at(3));

    sigmaLayout->addWidget(myVView);
    //sigmaLayout->addWidget(sigma2);
    sigmaLayout->addWidget(myValueLabel);
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
    toolBar->setDisabled(true);
    dockWidget->setDisabled(true);
    myImageView->setViewMode(4);
	fitView();

    QMessageBox msgBox;
    msgBox.setText("Click in image to indicate the direction of flight");
    msgBox.exec();
}

void SRUserInterface_Qt::acceptSR()
{
    manager->acceptSR();
	windowReport->close();
}

bool SRUserInterface_Qt::exec()
{
	loadPointsTable();
    connect (points, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(actualizeSelection(QStandardItem*)));

    this->show();
	if (myImageView->loadImage(QString(manager->getImageFile().c_str())))
	{
		myImageView->createPoints(points,2);
		myImageView->drawPoints(points,2);
		myImageView->fitView();
	}
	makeRepaint();
	actionMove->trigger();

    if (qApp->exec())
        return false;
	delete(myImageView);
    return true;
}
