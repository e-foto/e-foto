/*Copyright 2002-2014 e-foto team (UERJ)
  This file is part of e-foto.

    e-foto is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    e-foto is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with e-foto.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "IOUserInterface_Qt.h"

#include "LoadingScreen.h"
#include "SingleTools.h"
#include "ImageViewers.h"

#include <QPushButton>

#include <qapplication.h>
#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qaction.h>
#include <qstring.h>
#include "IOManager.h"

#include "ETableWidget.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

//#include <iostream>

IOUserInterface_Qt* IOUserInterface_Qt::ioInst = NULL;

IOUserInterface_Qt* IOUserInterface_Qt::instance(IOManager* manager)
{
    if (ioInst != NULL)
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
	actionSet_mark->setVisible(false);
	actionMove->setVisible(false);
	actionZoom->setVisible(false);
	actionFit_view->setVisible(false);
	group->setExclusive(true);
	actionActive_grid->setCheckable(true);
	actionView_report->setEnabled(false);
	actionInterior_orientation->setEnabled(false);
	table1->setEditTriggers(QAbstractItemView::NoEditTriggers);

	QObject::connect(actionInterior_orientation, SIGNAL(triggered()), this, SLOT(calculateIO()));
	QObject::connect(actionView_report, SIGNAL(triggered()), this, SLOT(viewReport()));
        QObject::connect(actionDone, SIGNAL(triggered()), this, SLOT(OIdone()));
	//QObject::connect(actionSet_mark, SIGNAL(triggered()), this, SLOT(activeSetMode()));
	//QObject::connect(actionMove, SIGNAL(triggered()), this, SLOT(activePanMode()));
	//QObject::connect(actionZoom, SIGNAL(triggered()), this, SLOT(activeZoomMode()));
	//QObject::connect(actionFit_view, SIGNAL(triggered()), this, SLOT(fitView()));

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
}

void IOUserInterface_Qt::receiveMark(QPointF p)
{
	if (p.x() < 0 || p.y() < 0)
		return;
	imageView->getMarker()->insertMark(p, table1->currentIndex().row()+1, QString::number(table1->currentIndex().row()+1), mark);

	points->setData(points->index(table1->currentIndex().row(), 2), QString::number(p.x(),'f',2));
	points->setData(points->index(table1->currentIndex().row(), 3), QString::number(p.y(),'f',2));
	points->setData(points->index(table1->currentIndex().row(), 4), QVariant(true));

	measureMark(table1->currentIndex().row()+1,p.x(),p.y());

	table1->selectRow(table1->currentIndex().row() + 1);
	testActivateIO();
}

void IOUserInterface_Qt::makeRepaint()
{
	imageView->update();
	table1->repaint();
}

void IOUserInterface_Qt::activeSetMode()
{
	//oldImageView->setViewMode(1);
}

void IOUserInterface_Qt::activePanMode()
{
	//oldImageView->setViewMode(2);
}

void IOUserInterface_Qt::activeZoomMode()
{
	//oldImageView->setViewMode(3);
}

void IOUserInterface_Qt::fitView()
{
	//oldImageView->fitView();
}

void IOUserInterface_Qt::init()
{
	// Insert image into layout
	QWidget* centralwidget = new QWidget(this);
	QGridLayout* gridLayout = new QGridLayout(centralwidget);

	//oldImageView = new ImageView(centralwidget);
	imageView = new SingleViewer();
	imageView->blockOpen();
	imageView->blockSave();
	imageView->addToolBar(Qt::TopToolBarArea,toolBar);
	gridLayout->addWidget(imageView, 0, 0, 1, 1);
	setCentralWidget(centralwidget);

	mark = new Marker(SymbolsResource::getCross(Qt::green, QSize(24, 24), 1, true)); // Personalizando as marcas. Que no futuro eu quero melhorar para inserir uso de 2 ou 3 marcas de acordo com o tipo de ponto.
	//imageView->getMarker()->changeMarker(mark);

	//resize(1024,800);

	// Make some connections
	imageView->getMarker()->setToOnlyEmitClickedMode();
	connect(imageView->getMarker(),SIGNAL(clicked(QPointF)), this, SLOT(receiveMark(QPointF)));
	//connect (myImageView, SIGNAL(mousePressed()), this, SLOT(informState()));
	//connect (myImageView, SIGNAL(markClicked(QPoint)), this, SLOT(receiveMark(QPoint)));
	//connect (myImageView, SIGNAL(changed()), this, SLOT(makeRepaint()));

	calculationMode = 0;

	//this->showNormal();
	//myImageView->fitView();
}

bool IOUserInterface_Qt::measureMark(int id, double col, double lin)
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
    std::deque<std::string> myValues = manager->makeReport();

	windowReport = new QWidget();
	QVBoxLayout *myLayout = new QVBoxLayout();
	QTabWidget *myTab = new QTabWidget();

	QWidget *XaView= new QWidget();
	QHBoxLayout *XaLayout= new QHBoxLayout();
	ETableWidget *myXaView = new ETableWidget();

	/* novo*/
	QLabel *xa= new QLabel("<font size=5>Xa");
	xa->setTextFormat(Qt::RichText);
	xa->setAlignment(Qt::AlignHCenter);

	QLabel *mvcxa= new QLabel("<font size=5>MVC(Xa)");
	mvcxa->setTextFormat(Qt::RichText);
	mvcxa->setAlignment(Qt::AlignHCenter);
	/**/

	QStringList listaXa;
    listaXa << "a0" << "a1" << "a2" << "b0" << "b1" << "b2";
	myXaView->setRowCount(listaXa.size());
	myXaView->setVerticalHeaderLabels(listaXa);
	myXaView->setTableData(myValues.at(0),'f',5);

	ETableWidget *mySXaView = new ETableWidget();
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
	ETableWidget *myLaView = new ETableWidget();
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


	ETableWidget *mySLaView = new ETableWidget();
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

    QLabel* vLabel= new QLabel("<font size=5>V(mm)</font>");
	vLabel->setTextFormat(Qt::RichText);
	vLabel->setAlignment(Qt::AlignHCenter);
	QVBoxLayout *vTableLayout= new QVBoxLayout();
	ETableWidget *myVView = new ETableWidget();
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

void IOUserInterface_Qt::OIdone()
{
    close();
}

void IOUserInterface_Qt::closeEvent(QCloseEvent *e)
{
	LoadingScreen::instance().show();
	qApp->processEvents();
	delete(imageView);
	delete(mark);
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
            std::deque<std::string> markData = manager->markData(row);
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
		//if (oldImageView->loadImage(QString(manager->getImageFile().c_str())))
        //imageView->loadImage(QString(manager->getImageFile().c_str()));
        imageView->loadImage(QString::fromLocal8Bit(manager->getImageFile().c_str()));
        {
			//REVER
			//oldImageView->createPoints(points,1);
			//oldImageView->drawPoints(points,1);
			//oldImageView->fitView();
		}
		for (int row = 0; row < points->rowCount() ;row++)
		{
			if (points->item(row,2)->text().isEmpty())
				continue;
			QString pointName = QString::number(row+1);
			QPointF location(points->item(row,2)->text().toDouble(),points->item(row,3)->text().toDouble());
			imageView->getMarker()->insertMark(location, row+1, pointName, mark);
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
            std::deque<std::string> markData = manager->markData(row);
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

		receiveMark(QPointF(0,0));
		receiveMark(QPointF(manager->getFrameColumns(),0));
		receiveMark(QPointF(manager->getFrameColumns(),manager->getFrameRows()));
		receiveMark(QPointF(0,manager->getFrameRows()));

		this->show();
		LoadingScreen::instance().close();
		qApp->processEvents();
		//myImageView = new ImageView(centralwidget);
		//if (oldImageView->loadImage(QString(manager->getImageFile().c_str())))
        //imageView->loadImage(QString(manager->getImageFile().c_str()));
        imageView->loadImage(QString::fromLocal8Bit(manager->getImageFile().c_str()));
		{
			//REVER
			//oldImageView->createPoints(points,1);
			//oldImageView->drawPoints(points,1);
			//oldImageView->fitView();
		}
		for (int row = 0; row < points->rowCount() ;row++)
		{
			if (points->item(row,2)->text().isEmpty())
				continue;
			QString pointName = QString::number(row+1);
			QPointF location(points->item(row,2)->text().toDouble(),points->item(row,3)->text().toDouble());
			imageView->getMarker()->insertMark(location, row+1, pointName, mark);
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
   deque<std::string> markData = manager->markData(row);
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
		//if (oldImageView->loadImage(QString(manager->getImageFile().c_str())))
        //imageView->loadImage(QString(manager->getImageFile().c_str()));
        imageView->loadImage(QString::fromLocal8Bit(manager->getImageFile().c_str()));
        {
			//REVER
			//myImageView->createPoints(points,1);
			//myImageView->drawPoints(points,1);
			//oldImageView->fitView();
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

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
