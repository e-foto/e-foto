#include "IOUserInterface_Qt.h"

#include <qapplication.h>
#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qaction.h>
#include <qstring.h>

//#include <iostream>

IOUserInterface_Qt* IOUserInterface_Qt::ioInst = NULL;

IOUserInterface_Qt* IOUserInterface_Qt::instance(IOManager* manager)
{
	if (ioInst = NULL)
	{
		delete ioInst;
		ioInst = NULL;
	}
	if (ioInst == NULL)
	{
		ioInst = new IOUserInterface_Qt(manager);
	}
	return ioInst;
}

IOUserInterface_Qt::IOUserInterface_Qt(IOManager* manager, QWidget* parent, Qt::WindowFlags fl)
	: QMainWindow(parent, fl)
{	
	setupUi(this);

	actionSet_mark->setCheckable(true);
	actionMove->setCheckable(true);
	actionZoom->setCheckable(true);
	QActionGroup *group = new QActionGroup(this);
	group->addAction(actionSet_mark);
	group->addAction(actionMove);
	group->addAction(actionZoom);
	group->setExclusive(true);
	actionActive_grid->setCheckable(true);
	actionView_report->setEnabled(false);
	actionInterior_orientation->setEnabled(false);
	table1->setEditTriggers(QAbstractItemView::NoEditTriggers);

	QObject::connect(actionInterior_orientation, SIGNAL(triggered()), this, SLOT(calculateIO()));
	QObject::connect(actionView_report, SIGNAL(triggered()), this, SLOT(viewReport()));
	QObject::connect(actionSet_mark, SIGNAL(triggered()), this, SLOT(activeSetMode()));
	QObject::connect(actionMove, SIGNAL(triggered()), this, SLOT(activePanMode()));
	QObject::connect(actionZoom, SIGNAL(triggered()), this, SLOT(activeZoomMode()));
	QObject::connect(actionFit_view, SIGNAL(triggered()), this, SLOT(fitView()));

	this->manager = manager;
	if (manager->interiorDone())
		actionView_report->setEnabled(true);

	init();
}

IOUserInterface_Qt::~IOUserInterface_Qt()
{
	// no need to delete child widgets, Qt does it all for us
}

void IOUserInterface_Qt::languageChange()
{
	retranslateUi(this);
}

void IOUserInterface_Qt::informState()
{
	myImageView->selectPoint(QString::number(table1->currentIndex().row()).toStdString());
}

void IOUserInterface_Qt::receiveMark(QPoint p)
{
	points->setData(points->index(table1->currentIndex().row(), 2), QVariant(p.x()));
	points->setData(points->index(table1->currentIndex().row(), 3), QVariant(p.y()));
	points->setData(points->index(table1->currentIndex().row(), 4), QVariant(true));
	measureMark(table1->currentIndex().row()+1,p.x(),p.y());
	table1->selectRow(table1->currentIndex().row() + 1);
	testActivateIO();
}

void IOUserInterface_Qt::makeRepaint()
{
	myImageView->repaint();
	table1->repaint();
}

void IOUserInterface_Qt::activeSetMode()
{
	myImageView->setViewMode(1);
}

void IOUserInterface_Qt::activePanMode()
{
	myImageView->setViewMode(2);
}

void IOUserInterface_Qt::activeZoomMode()
{
	myImageView->setViewMode(3);
}

void IOUserInterface_Qt::fitView()
{
	myImageView->fitView();
}

void IOUserInterface_Qt::init()
{
	// Insert image into layout
	QWidget* centralwidget = new QWidget(this);

	QGridLayout* gridLayout = new QGridLayout(centralwidget);

	//myImageView = new ImageView(QString(manager->getImageFile().c_str()), centralwidget);
	//myImageView->setFocusPolicy(Qt::NoFocus);
	myImageView = new ImageView(centralwidget);

	gridLayout->addWidget(myImageView, 0, 0, 1, 1);

	setCentralWidget(centralwidget);

	resize(1024,800);

	// Make some connections
	connect (myImageView, SIGNAL(mousePressed()), this, SLOT(informState()));
	connect (myImageView, SIGNAL(markClicked(QPoint)), this, SLOT(receiveMark(QPoint)));
	connect (myImageView, SIGNAL(changed()), this, SLOT(makeRepaint()));

	calculationMode = 0;

	//this->showNormal();
	//myImageView->fitView();
}

bool IOUserInterface_Qt::measureMark(int id, int col, int lin)
{
	return manager->measureMark(id, col, lin);
}

bool IOUserInterface_Qt::calculateIO()
{
	bool result = manager->calculateIO();
	viewReport();
	actionView_report->setEnabled(true);
	return result;
}

bool IOUserInterface_Qt::viewReport()
{
	deque<string> myValues = manager->makeReport();

	windowReport = new QWidget();
	QVBoxLayout *myLayout = new QVBoxLayout();
	QTabWidget *myTab = new QTabWidget();

	vector<string> myXa;
	myXa.push_back(myValues.at(0));
	MatrixModel* myXaModel = new MatrixModel(myXa);
	MatrixView* myXaView = new MatrixView(windowReport, myXaModel);
	myTab->addTab(myXaView, QString::fromUtf8("Xa"));
	vector<string> myLa;
	myLa.push_back(myValues.at(1));
	MatrixModel* myLaModel = new MatrixModel(myLa);
	MatrixView* myLaView = new MatrixView(windowReport, myLaModel);
	myTab->addTab(myLaView, QString::fromUtf8("La"));
	QLabel* myLabel = new QLabel(myValues.at(2).c_str());
	myTab->addTab(myLabel, QString::fromUtf8("sigma0^2"));
	vector<string> myV;
	myV.push_back(myValues.at(3));
	MatrixModel* myVModel = new MatrixModel(myV);
	MatrixView* myVView = new MatrixView(windowReport, myVModel);
	myTab->addTab(myVView, QString::fromUtf8("V"));
	vector<string> mySXa;
	mySXa.push_back(myValues.at(4));
	MatrixModel* mySXaModel = new MatrixModel(mySXa);
	MatrixView* mySXaView = new MatrixView(windowReport, mySXaModel);
	myTab->addTab(mySXaView, QString::fromUtf8("SigmaXa"));
	vector<string> mySLa;
	mySLa.push_back(myValues.at(5));
	MatrixModel* mySLaModel = new MatrixModel(mySLa);
	MatrixView* mySLaView = new MatrixView(windowReport, mySLaModel);
	myTab->addTab(mySLaView, QString::fromUtf8("SigmaLa"));

	QPushButton *acceptButton = new QPushButton("&Accept", this);
	QObject::connect(acceptButton, SIGNAL(clicked()), this, SLOT(acceptIO()));

	myLayout->addWidget(myTab);
	myLayout->addWidget(acceptButton);

	windowReport->setLayout(myLayout);
	windowReport->setWindowModality(Qt::ApplicationModal);
	windowReport->setWindowTitle("Interior Orientation Report");
	windowReport->show();

	return true;
}

void IOUserInterface_Qt::testActivateIO()
{
	if (calculationMode == 1 || calculationMode == 2)
	{
		if(manager->countMarks() == manager->getTotalMarks())
			actionInterior_orientation->setEnabled(true);
		else
			actionInterior_orientation->setEnabled(false);
	}
}

void IOUserInterface_Qt::acceptIO()
{
	manager->acceptIO();
	windowReport->close();
}

void IOUserInterface_Qt::closeEvent(QCloseEvent *e)
{
	LoadingScreen::instance().show();
	qApp->processEvents();
	delete(myImageView);
	manager->returnProject();
	QMainWindow::closeEvent(e);
}

bool IOUserInterface_Qt::exec()
{
	calculationMode = manager->getCalculationMode();
	if (calculationMode == 1)
	{
		PositionMatrix analogMarks = manager->getAnalogMarks();
		int numberOfMarks = analogMarks.getRows() / 2;

		points = new QStandardItemModel(numberOfMarks, 5);
		for (int row = 0; row < numberOfMarks; row++)
		{
			deque<string> markData = manager->markData(row);
			for (unsigned int col = 0; col < markData.size(); col++)
			{
				QStandardItem* item = new QStandardItem(QString(markData.at(col).c_str()));
				points->setItem(row, col, item);
			}
			for (int col = markData.size(); col < 4; col++)
			{
				QStandardItem* item = new QStandardItem(QString(""));
				points->setItem(row, col, item);
			}
			QStandardItem* item = new QStandardItem();
			points->setItem(row, 4, item);
			if (points->data(points->index(row,2)).toString() != "")
			{
				points->setData(points->index(row, 4), QVariant(true));
			}
			else
			{
				points->setData(points->index(row, 4), QVariant(false));
			}
		}
		points->setHeaderData(0, Qt::Horizontal, QVariant("X"));
		points->setHeaderData(1, Qt::Horizontal, QVariant("Y"));
		points->setHeaderData(2, Qt::Horizontal, QVariant("Col"));
		points->setHeaderData(3, Qt::Horizontal, QVariant("Row"));
		points->setHeaderData(4, Qt::Horizontal, QVariant("Used"));
		table1->setModel(points);
		table1->setColumnHidden(4,true);
		table1->selectRow(0);
		table1->setFocus();

		testActivateIO();

		this->show();
		LoadingScreen::instance().close();
		qApp->processEvents();
		//myImageView = new ImageView(centralwidget);
		if (myImageView->loadImage(QString(manager->getImageFile().c_str())))
		{
			myImageView->createPoints(points,1);
			myImageView->drawPoints(points,1);
			myImageView->fitView();
		}
		makeRepaint();
		actionMove->trigger();
	}
	if (calculationMode == 2)
	{
		PositionMatrix analogMarks = manager->getAnalogMarks();
		int numberOfMarks = analogMarks.getRows() / 2;

		points = new QStandardItemModel(numberOfMarks, 5);
		for (int row = 0; row < numberOfMarks; row++)
		{
			deque<string> markData = manager->markData(row);
			for (unsigned int col = 0; col < markData.size(); col++)
			{
				QStandardItem* item = new QStandardItem(QString(markData.at(col).c_str()));
				points->setItem(row, col, item);
			}
			for (int col = markData.size(); col < 4; col++)
			{
				QStandardItem* item = new QStandardItem(QString(""));
				points->setItem(row, col, item);
			}
			QStandardItem* item = new QStandardItem();
			points->setItem(row, 4, item);
			if (points->data(points->index(row,2)).toString() != "")
			{
				points->setData(points->index(row, 4), QVariant(true));
			}
			else
			{
				points->setData(points->index(row, 4), QVariant(false));
			}
		}
		points->setHeaderData(0, Qt::Horizontal, QVariant("X"));
		points->setHeaderData(1, Qt::Horizontal, QVariant("Y"));
		points->setHeaderData(2, Qt::Horizontal, QVariant("Col"));
		points->setHeaderData(3, Qt::Horizontal, QVariant("Row"));
		points->setHeaderData(4, Qt::Horizontal, QVariant("Used"));
		table1->setModel(points);
		table1->setColumnHidden(4,true);
		table1->selectRow(0);
		table1->setFocus();

		testActivateIO();

		receiveMark(QPoint(0,0));
		receiveMark(QPoint(manager->getFrameColumns(),0));
		receiveMark(QPoint(manager->getFrameColumns(),manager->getFrameRows()));
		receiveMark(QPoint(0,manager->getFrameRows()));

		this->show();
		LoadingScreen::instance().close();
		qApp->processEvents();
		//myImageView = new ImageView(centralwidget);
		if (myImageView->loadImage(QString(manager->getImageFile().c_str())))
		{
			myImageView->createPoints(points,1);
			myImageView->drawPoints(points,1);
			myImageView->fitView();
		}

		table1->selectRow(0);
		table1->setFocus();
		testActivateIO();
		makeRepaint();
		actionMove->trigger();
	}
	if (calculationMode == 3)
	{
		/*
		PositionMatrix analogMarks = manager->getAnalogMarks();
		int numberOfMarks = analogMarks.getRows() / 2;

		points = new QStandardItemModel(numberOfMarks, 5);
		for (int row = 0; row < numberOfMarks; row++)
		{
			deque<string> markData = manager->markData(row);
			for (unsigned int col = 0; col < markData.size(); col++)
			{
				QStandardItem* item = new QStandardItem(QString(markData.at(col).c_str()));
				points->setItem(row, col, item);
			}
			for (int col = markData.size(); col < 4; col++)
			{
				QStandardItem* item = new QStandardItem(QString(""));
				points->setItem(row, col, item);
			}
			QStandardItem* item = new QStandardItem();
			points->setItem(row, 4, item);
			if (points->data(points->index(row,2)).toString() != "")
			{
				points->setData(points->index(row, 4), QVariant(true));
			}
			else
			{
				points->setData(points->index(row, 4), QVariant(false));
			}
		}
		points->setHeaderData(0, Qt::Horizontal, QVariant("X"));
		points->setHeaderData(1, Qt::Horizontal, QVariant("Y"));
		points->setHeaderData(2, Qt::Horizontal, QVariant("Col"));
		points->setHeaderData(3, Qt::Horizontal, QVariant("Row"));
		points->setHeaderData(4, Qt::Horizontal, QVariant("Used"));
		table1->setModel(points);
		table1->setColumnHidden(4,true);
		table1->selectRow(0);
		table1->setFocus();
		*/
		actionSet_mark->setDisabled(true);
		removeDockWidget(dockWidget);
		actionInterior_orientation->setEnabled(true);

		this->show();
		LoadingScreen::instance().close();
		qApp->processEvents();
		//myImageView = new ImageView(centralwidget);
		if (myImageView->loadImage(QString(manager->getImageFile().c_str())))
		{
			//myImageView->createPoints(points,1);
			//myImageView->drawPoints(points,1);
			myImageView->fitView();
		}
		makeRepaint();
		actionMove->trigger();
	}

	//LoadingScreen::instance().close();

	//if (qApp->exec())
		//return false;
	//delete(myImageView); precisa de um stop
	return true;
}
