#include "IOUserInterface_Qt.h"

#include <qapplication.h>
#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qaction.h>
#include <qstring.h>

#include "TableIOEOWidget.h"

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

	setWindowState(this->windowState() | Qt::WindowMaximized);
	qApp->processEvents();
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

	//resize(1024,800);

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

        QWidget *XaView= new QWidget();
        QHBoxLayout *XaLayout= new QHBoxLayout();
        TableIOEOWidget *myXaView = new TableIOEOWidget();

		/* novo*/
		QLabel *xa= new QLabel("<font size=5>Xa");
		xa->setTextFormat(Qt::RichText);
		xa->setAlignment(Qt::AlignHCenter);

		QLabel *mvcxa= new QLabel("<font size=5>MVC(Xa)");
		mvcxa->setTextFormat(Qt::RichText);
		mvcxa->setAlignment(Qt::AlignHCenter);
	/**/

		QStringList listaXa;
		listaXa << "A0" << "A1" << "A2" << "B0" << "B1" << "B2";
		myXaView->setRowCount(listaXa.size());
		myXaView->setVerticalHeaderLabels(listaXa);
		myXaView->setTableData(myValues.at(0),'f',5);

		TableIOEOWidget *mySXaView = new TableIOEOWidget();
		mySXaView->setTableData(myValues.at(4),'f',5);

		/* novo */
		QVBoxLayout *xalabelLayout= new QVBoxLayout();// layout contendo o label e a tabela de Xa
		QVBoxLayout *mvclabelLayout= new QVBoxLayout();// layout contendo o label e a tabela de mvc(Xa)
	/**/
		/* novo */
		xalabelLayout->addWidget(xa);
		xalabelLayout->addWidget(myXaView);
		/**/
		mvclabelLayout->addWidget(mvcxa);
		mvclabelLayout->addWidget(mySXaView);

		XaLayout->addLayout(xalabelLayout);
		XaLayout->addLayout(mvclabelLayout);

		//XaLayout->addWidget(mySXaView);
        XaView->setLayout(XaLayout);
        myTab->addTab(XaView, QString::fromUtf8("Xa"));
        connect(myXaView,SIGNAL(focusReceived()),mySXaView,SLOT(clearSelection()));
        connect(mySXaView,SIGNAL(focusReceived()),myXaView,SLOT(clearSelection()));

		//Aba La

		QWidget *LaView = new QWidget();
        QHBoxLayout *LaLayout= new QHBoxLayout();
        TableIOEOWidget *myLaView = new TableIOEOWidget();
		myLaView->setTableData(myValues.at(1),'f',5);

		/* novo */
		QVBoxLayout *laLabelLayout= new QVBoxLayout();// layout contendo o label e a tabela de La
		QVBoxLayout *mvcLaLabelLayout= new QVBoxLayout();// layout contendo o label e a tabela de mvc(La)
	/**/
		/* novo*/
		QLabel *la= new QLabel("<font size=5>La");
		la->setTextFormat(Qt::RichText);
		la->setAlignment(Qt::AlignHCenter);

		QLabel *mvcLa= new QLabel("<font size=5>MVC(La)");
		mvcLa->setTextFormat(Qt::RichText);
		mvcLa->setAlignment(Qt::AlignHCenter);


		TableIOEOWidget *mySLaView = new TableIOEOWidget();
		mySLaView->setTableData(myValues.at(5),'f',5);

		/* novo */
		laLabelLayout->addWidget(la);
		laLabelLayout->addWidget(myLaView);
		/**/

		mvcLaLabelLayout->addWidget(mvcLa);
		mvcLaLabelLayout->addWidget(mySLaView);

		LaLayout->addLayout(laLabelLayout);
		LaLayout->addLayout(mvcLaLabelLayout);

		LaView->setLayout(LaLayout);
		if (myLaView->rowCount() != 0)
			myTab->addTab(LaView, QString::fromUtf8("La"));


        QWidget *sigmaView = new QWidget();
        QHBoxLayout *sigmaLayout= new QHBoxLayout();
		QVBoxLayout *formulasLayout= new QVBoxLayout();

		QString aux1=QString::fromUtf8("<font size=5>σ<sub>0</sub><sup>2</sup>=V<sup>T</sup>PV/(n-m)");//σ² hexadecimal 03C3 0342//\sigma_{0}^{2} = frac{V^{T} * P * V}{n - m}\F

		aux1+="=";
		aux1+=QString::number(stringToDouble(myValues.at(2)),'f',6);
		aux1+="</font>";
		QLabel* myValueLabel1 = new QLabel(aux1);
		myValueLabel1->setTextFormat(Qt::RichText);

		QString aux2=QString::fromUtf8("<font size=5>σ<sub>0</sub>");//σ² hexadecimal 03C3 0342//\sigma_{0}^{2} = frac{V^{T} * P * V}{n - m}\F
		aux2+="=";
		aux2+=QString::number(sqrt(stringToDouble(myValues.at(2))),'f',6);
		aux2+="</font>";
		QLabel* myValueLabel2 = new QLabel(aux2);
		myValueLabel2->setTextFormat(Qt::RichText);

		QLabel* vLabel= new QLabel("<font size=5>V");
		vLabel->setTextFormat(Qt::RichText);
		vLabel->setAlignment(Qt::AlignHCenter);
		QVBoxLayout *vTableLayout= new QVBoxLayout();
        TableIOEOWidget *myVView = new TableIOEOWidget();
		myVView->setTableData(myValues.at(3),'f',5);
		vTableLayout->addWidget(vLabel);
		vTableLayout->addWidget(myVView);

		formulasLayout->addWidget(myValueLabel1);
		formulasLayout->addWidget(myValueLabel2);

		sigmaLayout->addLayout(vTableLayout);
		sigmaLayout->addLayout(formulasLayout);

        sigmaLayout->setSpacing(QHBoxLayout::SetMinimumSize);
        sigmaView->setLayout(sigmaLayout);
        myTab->addTab(sigmaView, QString::fromUtf8("V"));

        QPushButton *acceptButton = new QPushButton("&Accept", this);
	QObject::connect(acceptButton, SIGNAL(clicked()), this, SLOT(acceptIO()));

	myLayout->addWidget(myTab);
	myLayout->addWidget(acceptButton);

	windowReport->setLayout(myLayout);
	windowReport->setWindowModality(Qt::ApplicationModal);
	windowReport->setWindowTitle("Interior Orientation Report");
	windowReport->show();

        connect(myLaView,SIGNAL(focusReceived()),mySLaView,SLOT(clearSelection()));
        connect(mySLaView,SIGNAL(focusReceived()),myLaView,SLOT(clearSelection()));

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
