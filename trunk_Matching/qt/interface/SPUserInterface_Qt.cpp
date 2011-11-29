#include "SPUserInterface_Qt.h"

#include <qdesktopwidget.h>
#include <qapplication.h>
#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qaction.h>
#include <qstring.h>

SPUserInterface_Qt* SPUserInterface_Qt::spInst = NULL;

SPUserInterface_Qt* SPUserInterface_Qt::instance(SPManager* manager)
{
        if (spInst = NULL)
	{
                delete spInst;
                spInst = NULL;
	}
        if (spInst == NULL)
	{
                spInst = new SPUserInterface_Qt(manager);
	}
        return spInst;
}

SPUserInterface_Qt::SPUserInterface_Qt(SPManager* manager, QWidget* parent, Qt::WindowFlags fl)
        : QWidget(parent, fl)
{	
        //setupUi(this);
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
        if (manager->interiorDone())
                actionView_report->setEnabled(true);
*/
        this->manager = manager;


        setWindowState(this->windowState());

        // Center window
        QDesktopWidget *desktop = QApplication::desktop();
        int Cx,Cy;
        QRect rect = geometry();
        Cx = (desktop->width() - rect.width())/2;
        Cy = (desktop->height() - rect.height())/2;
        move(Cx,Cy);

        allow_close = true;

	qApp->processEvents();
	init();
}

SPUserInterface_Qt::~SPUserInterface_Qt()
{
	// no need to delete child widgets, Qt does it all for us
}

void SPUserInterface_Qt::languageChange()
{
        //retranslateUi(this);
}

void SPUserInterface_Qt::init()
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

void SPUserInterface_Qt::closeEvent(QCloseEvent *e)
{
        if (!allow_close)
        {
            e->ignore();
            return;
        }

	LoadingScreen::instance().show();
	qApp->processEvents();
        //delete(myImageView);
	manager->returnProject();
        QWidget::closeEvent(e);
}

bool SPUserInterface_Qt::exec()
{
    show();
    LoadingScreen::instance().close();
    return true;
}
