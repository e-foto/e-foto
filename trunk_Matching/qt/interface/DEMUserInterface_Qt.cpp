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
        if (manager->interiorDone())
                actionView_report->setEnabled(true);
*/
        this->manager = manager;

        QObject::connect(doneButton, SIGNAL(clicked()), this, SLOT(close()));

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
	LoadingScreen::instance().show();
	qApp->processEvents();
        //delete(myImageView);
	manager->returnProject();
        QWidget::closeEvent(e);
}

bool DEMUserInterface_Qt::exec()
{
    show();
    LoadingScreen::instance().close();
    return true;
}

/*
 * Image dealing
 **/

Matrix * DEMUserInterface_Qt::loadImage(char *filename, double sample)
{
        int levels=256;

        QImage img;
        img.load(filename);

        int step = int(1.0/sample);
        int width = int(img.width()*sample);
        int height = int(img.height()*sample);
        int pixel;

        Matrix *I = new Matrix(height, width);

        for (unsigned int i=1; i<=height; i++)
        {
                for (unsigned int j=1; j<=width; j++)
                {
                        pixel = img.pixel((j-1)*step,(i-1)*step);
//			pixel = ((pixel >> 16) & 0xFF)*0.2989 + ((pixel >> 8) & 0xFF)*0.5870 + (pixel & 0xFF)*0.1140;
                        pixel = pixel & 0xFF;
                        I->set(i, j, pixel/double(levels-1));
                }
        }

        return I;
}

int DEMUserInterface_Qt::saveImage(char *filename, Matrix *I)
{
        int levels = 256;

        QImage img(I->getCols(), I->getRows(), QImage::Format_RGB32); // Qt4

        int pixel;
        for (unsigned int i=1; i<=img.height(); i++)
        {
                for (unsigned int j=1; j<=img.width(); j++)
                {
                        pixel = round(I->get(i,j)*double(levels-1));
                        pixel = (pixel << 16) + (pixel << 8) + pixel;
                        img.setPixel(j-1, i-1, pixel);
                }
        }

        img.save(filename,"BMP");

        return 1;
}
