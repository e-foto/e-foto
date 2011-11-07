#include "DEMUserInterface_Qt.h"

#include <qdesktopwidget.h>
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
        QObject::connect(demButton, SIGNAL(clicked()), this, SLOT(onDemExtractionClicked()));
        QObject::connect(saveButton2, SIGNAL(clicked()), this, SLOT(onDemSaveClicked()));
        QObject::connect(saveButton, SIGNAL(clicked()), this, SLOT(onDemGridSaveClicked()));
        QObject::connect(loadButton, SIGNAL(clicked()), this, SLOT(onDemLoadClicked()));
        QObject::connect(interButton, SIGNAL(clicked()), this, SLOT(onDemGridClicked()));

        setWindowState(this->windowState());

        // Center window
        QDesktopWidget *desktop = QApplication::desktop();
        int Cx,Cy;
        QRect rect = geometry();
        Cx = (desktop->width() - rect.width())/2;
        Cy = (desktop->height() - rect.height())/2;
        move(Cx,Cy);

        // Set tab 0 as initial page
        tabWidget->setCurrentIndex(0);

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

void DEMUserInterface_Qt::addImagePair(char *item)
{
    QString text = QString::fromAscii(item);
    comboBox5->addItem(text);
}

void DEMUserInterface_Qt::setStatus(char *txt)
{
    workLabel->setText(QString::fromAscii(txt));
}

void DEMUserInterface_Qt::setProgress(int progress)
{
    if (progress < 0) progress = 0;
    if (progress > 100) progress = 100;

    progressBar->setValue(progress);
    qApp->processEvents();
}

void DEMUserInterface_Qt::setAutoExtInfo(int nseeds, int nmatch, double min, double max)
{
    seedPointsLabel->setText(QString::number(nseeds));
    automaticPointsLabel->setText(QString::number(nmatch));
    autoMinZLabel->setText(QString::number(min,'f',2));
    autoMaxZLabel->setText(QString::number(max,'f',2));
}

void DEMUserInterface_Qt::onDemSaveClicked()
{
    // File open dialog
    QString filename = QFileDialog::getSaveFileName(this, tr("Open file"), ".", tr("DEM (*.pix);; Text file (*.txt);; All files (*.*)"));
    if (filename=="")
        return;

    // Save last dir
    int i=filename.lastIndexOf("/");
    QDir dir(filename.left(i));
    dir.setCurrent(dir.absolutePath());

    // Save DEM
    manager->saveDem((char *)filename.toStdString().c_str(), comboBox8->currentIndex());
}

void DEMUserInterface_Qt::onDemGridSaveClicked()
{
    // File open dialog
    QString filename = QFileDialog::getSaveFileName(this, tr("Open file"), ".", tr("DEM Grid (*.dsm);; All files (*.*)"));
    if (filename=="")
        return;

    // Save last dir
    int i=filename.lastIndexOf("/");
    QDir dir(filename.left(i));
    dir.setCurrent(dir.absolutePath());

    // Save DEM
    manager->saveDem((char *)filename.toStdString().c_str(), comboBox9->currentIndex());
}

void DEMUserInterface_Qt::onDemLoadClicked()
{
    // File open dialog
    QString filename = QFileDialog::getOpenFileName(this, tr("Open DEM file"), ".", tr("DEM (*.pix);; Text file (*.txt);; All files (*.*)")) ;
    // if no file name written, return
    if (filename=="")
            return;

    // Save last dir
    int i=filename.lastIndexOf("/");
    QDir dir(filename.left(i));
    dir.setCurrent(dir.absolutePath());

    // Load DEM
    manager->loadDem((char *)filename.toStdString().c_str(), comboBox8->currentIndex());
}

void DEMUserInterface_Qt::onDemGridClicked()
{

}

/*
 * Image matching
 **/

void DEMUserInterface_Qt::onDemExtractionClicked()
{
    manager->setAutoExtractionSettings(comboBox3->currentIndex(), comboBox4->currentIndex(), spinBox1->value(), spinBox2->value(), doubleSpinBox0->value());
    manager->setLSMSettings(spinBox3->value(), spinBox4->value(), doubleSpinBox5->value(), doubleSpinBox6->value(), spinBox7->value(), doubleSpinBox8->value(), doubleSpinBox9->value(), doubleSpinBox10->value());
    manager->setNCCSettings(spinBox11->value(), spinBox12->value(), doubleSpinBox13->value(), doubleSpinBox14->value());
    manager->extractDEM(comboBox5->currentIndex());
}

/*
 * Image dealing
 **/

Matrix * DEMUserInterface_Qt::loadImage(char *filename, double sample)
{
        int levels=256;
        printf("filename: %s\n",filename);
        QImage img;
        img.load(filename);

        int step = int(1.0/sample);
        int width = int(img.width()*sample);
        int height = int(img.height()*sample);
        int pixel;

        Matrix *I = new Matrix(height, width);

        progressBar->setValue(0);
        for (unsigned int i=1; i<=height; i++)
        {
                for (unsigned int j=1; j<=width; j++)
                {
                        pixel = img.pixel((j-1)*step,(i-1)*step);
//			pixel = ((pixel >> 16) & 0xFF)*0.2989 + ((pixel >> 8) & 0xFF)*0.5870 + (pixel & 0xFF)*0.1140;
                        pixel = (((pixel >> 16) & 0xFF) + ((pixel >> 8) & 0xFF) + (pixel & 0xFF)) / 3;
//                        pixel = pixel & 0xFF;
                        I->set(i, j, pixel/double(levels-1));
                }
                progressBar->setValue((100*i)/height);
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
