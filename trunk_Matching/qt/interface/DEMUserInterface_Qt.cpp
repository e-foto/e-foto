#include "DEMUserInterface_Qt.h"

#include <qapplication.h>
#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qaction.h>
#include <qstring.h>


DEMUserInterface_Qt* DEMUserInterface_Qt::demInst = NULL;

DEMUserInterface_Qt* DEMUserInterface_Qt::instance(DEMManager* manager)
{
        if (demInst = NULL)
	{
                delete demInst;
                demInst = NULL;
	}
        if (demInst == NULL)
	{
                demInst = new DEMUserInterface_Qt(manager);
	}
        return demInst;
}

DEMUserInterface_Qt::DEMUserInterface_Qt(DEMManager* manager, QWidget* parent, Qt::WindowFlags fl)
        : QWidget(parent, fl)
{	
	setupUi(this);
/*
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
*/
        setWindowState(this->windowState());
	qApp->processEvents();
	init();
}

DEMUserInterface_Qt::~DEMUserInterface_Qt()
{
	// no need to delete child widgets, Qt does it all for us
}

void DEMUserInterface_Qt::languageChange()
{
	retranslateUi(this);
}

void DEMUserInterface_Qt::init()
{
    /*
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
*/
	//this->showNormal();
	//myImageView->fitView();
}

void DEMUserInterface_Qt::closeEvent(QCloseEvent *e)
{
    /*
	LoadingScreen::instance().show();
	qApp->processEvents();
	delete(myImageView);
	manager->returnProject();
	QMainWindow::closeEvent(e);
        */
}

bool DEMUserInterface_Qt::exec()
{
    show();
    LoadingScreen::instance().close();
    return true;
}
