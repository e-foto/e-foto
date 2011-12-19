#include "OrthoUserInterface_Qt.h"

#include <qdesktopwidget.h>
#include <qapplication.h>
#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qaction.h>
#include <qstring.h>

OrthoUserInterface_Qt* OrthoUserInterface_Qt::OrthoInst = NULL;

OrthoUserInterface_Qt* OrthoUserInterface_Qt::instance(OrthoManager* manager)
{
        if (OrthoInst = NULL)
	{
                delete OrthoInst;
                OrthoInst = NULL;
	}
        if (OrthoInst == NULL)
	{
                OrthoInst = new OrthoUserInterface_Qt(manager);
	}
        return OrthoInst;
}

OrthoUserInterface_Qt::OrthoUserInterface_Qt(OrthoManager* manager, QWidget* parent, Qt::WindowFlags fl)
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
        QObject::connect(abortButton, SIGNAL(clicked()), this, SLOT(onAbortClicked()));
        QObject::connect(orthoButton, SIGNAL(clicked()), this, SLOT(onOrthoClicked()));
        QObject::connect(loadDemButton, SIGNAL(clicked()), this, SLOT(onLoadDemClicked()));
        QObject::connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(onShowImageChanged(int)));

        setWindowState(this->windowState());

        // Set flags
        dem_load_flag = 0;

        // Center window
        QDesktopWidget *desktop = QApplication::desktop();
        int Cx,Cy;
        QRect rect = geometry();
        Cx = (desktop->width() - rect.width())/2;
        Cy = (desktop->height() - rect.height())/2;
        move(Cx,Cy);

        allow_close = true;
        onShowImageChanged(checkBox->isChecked());

	qApp->processEvents();
	init();
}

OrthoUserInterface_Qt::~OrthoUserInterface_Qt()
{
	// no need to delete child widgets, Qt does it all for us
}

void OrthoUserInterface_Qt::languageChange()
{
	retranslateUi(this);
}

void OrthoUserInterface_Qt::init()
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

void OrthoUserInterface_Qt::closeEvent(QCloseEvent *e)
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

bool OrthoUserInterface_Qt::exec()
{
    show();
    LoadingScreen::instance().close();
    return true;
}

void OrthoUserInterface_Qt::onAbortClicked()
{
    // Abort clicked
    manager->setFlagCancel();
    workLabel->setText("Canceled");
    progressBar->setValue(0);
    enableOptions();
}

void OrthoUserInterface_Qt::onLoadDemClicked()
{
    // File open dialog
    QString filename = QFileDialog::getOpenFileName(this, tr("Open DEM file"), ".", tr("DEM (*.dsm);; Text file (*.txt);; All files (*.*)")) ;
    // if no file name written, return
    if (filename=="")
            return;

    // Save last dir
    int i=filename.lastIndexOf("/");
    QDir dir(filename.left(i));
    dir.setCurrent(dir.absolutePath());

    // Add file to line edit
    lineEdit->setText(filename);

    // Load DEM
    dem_load_flag = manager->loadDemGrid((char *)lineEdit->text().toStdString().c_str(),comboBox2->currentIndex());

    // Report error
    if (!dem_load_flag)
    {
        QMessageBox::critical(this,"Error","Invalid DEM file format.");
        return;
    }
}

void OrthoUserInterface_Qt::onOrthoClicked()
{
    // Ortho clicked

    if (!dem_load_flag)
    {
        QMessageBox::critical(this,"Error","Please, load a DEM first.");
        return;
    }

    // Save dialog
    // File open dialog
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Orthoimage"), ".", tr("E-FOTO Orthoimage (*.eoi);; All files (*.*)")) ;
    // if no file name written, return
    if (filename=="")
            return;

    // Save last dir
    int i=filename.lastIndexOf("/");
    QDir dir(filename.left(i));
    dir.setCurrent(dir.absolutePath());

    disableOptions();
    setAllowClose(false);
    manager->setInterMethod(comboBox4->currentIndex());
    manager->orthoRectification((char *)filename.toStdString().c_str(),comboBox3->currentIndex(), comboBox->currentIndex(), doubleSpinBox1->value(), doubleSpinBox2->value());
    setAllowClose(true);
    enableOptions();
    setCurrentWork("Done");
}

void OrthoUserInterface_Qt::disableOptions()
{
    comboBox->setEnabled(false);
    comboBox2->setEnabled(false);
    lineEdit->setEnabled(false);
    doubleSpinBox1->setEnabled(false);
    doubleSpinBox2->setEnabled(false);
    orthoButton->setEnabled(false);
    doneButton->setEnabled(false);
}

void OrthoUserInterface_Qt::enableOptions()
{
    comboBox->setEnabled(true);
    comboBox2->setEnabled(true);
    lineEdit->setEnabled(true);
    doubleSpinBox1->setEnabled(true);
    doubleSpinBox2->setEnabled(true);
    orthoButton->setEnabled(true);
    doneButton->setEnabled(true);
}

void OrthoUserInterface_Qt::setProgress(int progress)
{
    if (progress < 0) progress = 0;
    if (progress > 100) progress = 100;

    progressBar->setValue(progress);
    qApp->processEvents();
}

void OrthoUserInterface_Qt::setCurrentWork(string msg)
{
    QString qmsg = QString::fromStdString(msg);
    workLabel->setText(qmsg);
}

void OrthoUserInterface_Qt::onShowImageChanged(int opt)
{
    manager->setShowImage(opt);
}

/*
 * Image dealing
 **/
#include <qdebug.h>
Matrix * OrthoUserInterface_Qt::loadImage(char *filename, double sample)
{
        int levels=256;

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
//	  		pixel = ((pixel >> 16) & 0xFF)*0.2989 + ((pixel >> 8) & 0xFF)*0.5870 + (pixel & 0xFF)*0.1140; // Color to gray 8-bit
//                        pixel = (((pixel >> 16) & 0xFF) + ((pixel >> 8) & 0xFF) + (pixel & 0xFF)) / 3; // Simple color to gray 8-bit
//                        pixel = pixel & 0xFF; // Gray 24-bit to 8-bit
//                        I->set(i, j, pixel/double(levels-1));
                        I->set(i, j, double(pixel&0xFFFFFF)/double(0xFFFFFF)); // Color 24-bit (RR GG BB) to 0-1
                }
                progressBar->setValue((100*i)/height);
        }

        return I;
}

int OrthoUserInterface_Qt::saveImage(char *filename, Matrix *I)
{
        int levels = 256;

        QImage img(I->getCols(), I->getRows(), QImage::Format_RGB32); // Qt4

        int pixel;
        for (unsigned int i=1; i<=img.height(); i++)
        {
                for (unsigned int j=1; j<=img.width(); j++)
                {
//                        pixel = round(I->get(i,j)*double(levels-1));
//                        pixel = (pixel << 16) + (pixel << 8) + pixel; // Gray to color 24-bit
                        pixel = int(I->get(i,j)*double(0xFFFFFF));

                        img.setPixel(j-1, i-1, pixel);
                }
        }

        img.save(filename,"BMP");

        return 1;
}

void OrthoUserInterface_Qt::showImage(Matrix* image, bool isGrayscale)
{
    SingleViewer* sv = new SingleViewer(this);
    sv->loadImage(image, isGrayscale);
    sv->blockOpen();
    sv->blockMark();
    sv->show();
}

void OrthoUserInterface_Qt::showErrorMessage(QString msg)
{
    QMessageBox::critical(this, "Error",msg);
    doneButton->click();
}
