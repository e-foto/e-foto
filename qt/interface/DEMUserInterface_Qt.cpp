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

#include "DEMUserInterface_Qt.h"

#include "Image.h"
#include "SingleDisplay.h"
#include "ImageViewers.h"
#include "LoadingScreen.h"

#include <QDesktopWidget>
#include <QFileDialog>
#include <QMessageBox>

#include <iostream>
#include <sstream>
#include <fstream>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

DEMUserInterface_Qt* DEMUserInterface_Qt::demInst = NULL;

DEMUserInterface_Qt* DEMUserInterface_Qt::instance(DEMManager* manager)
{
    if (demInst != NULL)
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
    this->manager = manager;

    QObject::connect(doneButton, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(demButton, SIGNAL(clicked()), this, SLOT(onDemExtractionClicked()));
    QObject::connect(saveButton2, SIGNAL(clicked()), this, SLOT(onDemSaveClicked()));
    QObject::connect(saveButton, SIGNAL(clicked()), this, SLOT(onDemGridSaveClicked()));
    QObject::connect(loadButton, SIGNAL(clicked()), this, SLOT(onDemLoadClicked()));
    QObject::connect(loadButton2, SIGNAL(clicked()), this, SLOT(onDemGridLoadClicked()));
    QObject::connect(interButton, SIGNAL(clicked()), this, SLOT(onDemGridClicked()));
    QObject::connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(onLSMCheckChanged(/*int*/)));
    QObject::connect(abortButton, SIGNAL(clicked()), this, SLOT(onAbortClicked()));
    QObject::connect(comboBox2_2, SIGNAL(currentIndexChanged(int)), this, SLOT(onGridAreaLimitsStateChanged(int)));
    QObject::connect(comboBox1, SIGNAL(currentIndexChanged(int)), this, SLOT(onInterStateChanged(int)));
    QObject::connect(checkBox_3, SIGNAL(stateChanged(int)), this, SLOT(onShowImageStateChanged(int)));
    QObject::connect(seedsButton, SIGNAL(clicked()), this, SLOT(onSeedsEditorClicked()));
    QObject::connect(stereoButton, SIGNAL(clicked()), this, SLOT(onStereoplotterClicked()));
    QObject::connect(comboBox4, SIGNAL(currentIndexChanged(int)), this, SLOT(onMatchingMethodChanged(int)));
    QObject::connect(loadPtsButton, SIGNAL(clicked()), this, SLOT(onLoadPtsButtonClicked()));
    QObject::connect(saveQButton, SIGNAL(clicked()), this, SLOT(onSavePtsButtonClicked()));
    QObject::connect(spinBox3, SIGNAL(valueChanged(int)), this, SLOT(onLSMTemplateSizeChanged(int)));
    QObject::connect(spinBox11, SIGNAL(valueChanged(int)), this, SLOT(onCorrTemplateSizeChanged(int)));
    QObject::connect(spinBox3_3, SIGNAL(valueChanged(int)), this, SLOT(onLSMTemplateSizeChanged(int)));
    QObject::connect(spinBox11_3, SIGNAL(valueChanged(int)), this, SLOT(onCorrTemplateSizeChanged(int)));
    QObject::connect(doubleSpinBox0, SIGNAL(valueChanged(double)), this, SLOT(onDownValueChanged()));

    setWindowState(this->windowState());
    sed = NULL;

    // Center window
    QDesktopWidget *desktop = QApplication::desktop();
    int Cx,Cy;
    QRect rect = geometry();
    Cx = (desktop->width() - rect.width())/2;
    Cy = (desktop->height() - rect.height())/2;
    move(Cx,Cy);

    // Set tab 0 as initial page
    tabWidget->setCurrentIndex(0);

    // Block some options
    saveButton->setEnabled(false);
    saveButton2->setEnabled(false);
    interButton->setEnabled(false);
    tabWidget->setTabEnabled(4, false);
    tabWidget->setTabEnabled(6, false);
    onMatchingMethodChanged(comboBox4->currentIndex());

    // Set initial states
    onGridAreaLimitsStateChanged(0);
    onInterStateChanged(0);
    manager->setShowImage(checkBox_3->isChecked());
    downReslabel->setText(QString::number(manager->calculateDemRes(doubleSpinBox0->value()),'f',2) + " meters");

    allow_close = true;

    comboBox0->setEnabled(false);
    demSource = 0;

    lastDir = ".";

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
    if (!allow_close)
    {
        e->ignore();
        return;
    }

    if (manager->isDemUnsaved())
    {
        if (QMessageBox::warning(this, "Unsaved work","A DSM was generated and not saved. Continue?", QMessageBox::Ok, QMessageBox::No) == QMessageBox::No)
        {
            e->ignore();
            return;
        }
    }

    if (manager->isGridUnsaved())
    {
        if (QMessageBox::warning(this, "Unsaved work","An interpolated DSM Grid was generated and not saved. Continue?", QMessageBox::Ok, QMessageBox::No) == QMessageBox::No)
        {
            e->ignore();
            return;
        }
    }

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

void DEMUserInterface_Qt::onAbortClicked()
{
    // Abort clicked
    manager->setCancel();
    enableOptions();
    setAllowClose(true);
}

void DEMUserInterface_Qt::onLSMTemplateSizeChanged(/*int ts*/)
{
    if (spinBox3->value() > spinBox3_3->value())
        spinBox3_3->setValue(spinBox3->value() + spinBox3_2->value());
}

void DEMUserInterface_Qt::onCorrTemplateSizeChanged(/*int ts*/)
{
    if (spinBox11->value() > spinBox11_3->value())
        spinBox11_3->setValue(spinBox11->value() + spinBox11_2->value());
}

void DEMUserInterface_Qt::onDownValueChanged(/*double value*/)
{
    downReslabel->setText(QString::number(manager->calculateDemRes(doubleSpinBox0->value()),'f',2) + " meters");
}

void DEMUserInterface_Qt::setMathcingHistogram(int *hist)
{
    histProgressBar1->setValue(hist[0]);
    histProgressBar2->setValue(hist[1]);
    histProgressBar3->setValue(hist[2]);
    histProgressBar4->setValue(hist[3]);
    histProgressBar5->setValue(hist[4]);
    histProgressBar6->setValue(hist[5]);
}

void DEMUserInterface_Qt::showImage2D(Matrix* image, double xi, double dx, double yi, double dy, bool isGrayscale)
{
    SingleViewer* sv = new SingleViewer(this);
    sv->setOrtoImageMode(xi, dx, yi, dy);
    sv->loadImage(image, isGrayscale);
    sv->blockOpen();
    sv->blockSave();
    sv->blockMark();
    sv->show();
}

void DEMUserInterface_Qt::showImage3D(Matrix* image, double xi, double dx, double yi, double dy, double zi, double dz, bool isGrayscale)
{
    SingleViewer* sv = new SingleViewer(this);
    sv->setElevationImageMode(xi, dx, yi, dy, zi, dz);
    sv->loadImage(image, isGrayscale);
    sv->blockOpen();
    sv->blockSave();
    sv->blockMark();
    sv->show();
}

void DEMUserInterface_Qt::disableOptions()
{
    tab->setDisabled(true);
    tab_2->setDisabled(true);
    tab_3->setDisabled(true);
    tab_4->setDisabled(true);
    tab_5->setDisabled(true);
    tab_7->setDisabled(true);
    groupBox_7->setDisabled(true);
    groupBox_11->setDisabled(true);
    groupBox_12->setDisabled(true);
    doneButton->setDisabled(true);
}

void DEMUserInterface_Qt::enableOptions()
{
    tab->setEnabled(true);
    tab_2->setEnabled(true);
    tab_3->setEnabled(true);
    tab_4->setEnabled(true);
    tab_5->setEnabled(true);
    tab_7->setEnabled(true);
    groupBox_7->setEnabled(true);
    groupBox_11->setEnabled(true);
    groupBox_12->setEnabled(true);
    doneButton->setEnabled(true);
}

void DEMUserInterface_Qt::addImagePair(char *item)
{
    QString text = QString::fromLatin1(item);
    comboBox5->addItem(text);
}

void DEMUserInterface_Qt::setStatus(char *txt)
{
    workLabel->setText(QString::fromLatin1(txt));
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
    autoMinZLabel->setText(QString::number(min,'f',5));
    autoMaxZLabel->setText(QString::number(max,'f',5));
}

void DEMUserInterface_Qt::setManualExtInfo(int npts, double min, double max)
{
    manualPointsLabel->setText(QString::number(npts));
    manualMinZLabel->setText(QString::number(min,'f',5));
    manualMaxZLabel->setText(QString::number(max,'f',5));
}

void DEMUserInterface_Qt::updateSeedsLabel(int nseeds)
{
    seedPointsLabel->setText(QString::number(nseeds));
}

void DEMUserInterface_Qt::setBoundingBox(double Xi, double Yi, double Xf, double Yf)
{
    XilineEdit->setText(QString::number(Xi,'f',5));
    YilineEdit->setText(QString::number(Yi,'f',5));
    XflineEdit->setText(QString::number(Xf,'f',5));
    YflineEdit->setText(QString::number(Yf,'f',5));
}

void DEMUserInterface_Qt::onDemSaveClicked()
{
    // File open dialog
    QString filter;
    QString suffix;

    if (comboBox8->currentIndex() == 0)
    {
        filter = tr("ASCII Full 2D + 3D (*.pix);; All files (*.*)");
        suffix = "pix";
    }
    else if (comboBox8->currentIndex() == 1)
    {
        filter = tr("ASCII 2D Points (*.txt);; All files (*.*)");
        suffix = "txt";
    }
    else if (comboBox8->currentIndex() == 2)
    {
        filter = tr("ASCII 2D Points (*.dat);; All files (*.*)");
        suffix = "dat";
    }
    else if (comboBox8->currentIndex() == 3)
    {
        filter = tr("Point Cloud (*.xyz);; All files (*.*)");
        suffix = "xyz";
    }
    else if (comboBox8->currentIndex() == 4)
    {
        filter = tr("Point Cloud (*.ixyz);; All files (*.*)");
        suffix = "ixyz";
    }
    else
    {
        filter = tr("Text file (*.txt);; All files (*.*)");
        suffix = "txt";
    }
    QFileDialog salvar(this,tr("Save file"),lastDir,filter);
    salvar.setAcceptMode(QFileDialog::AcceptSave);
    salvar.setDefaultSuffix(suffix);
    if(salvar.exec())
    {
        //QString filename = QFileDialog::getSaveFileName(this, tr("Open file"), lastDir, tr("DEM (*.pix);; Text file (*.txt);; All files (*.*)"));
        QString filename = salvar.selectedFiles()[0];
        if (filename.isEmpty())
            return;

        // Save last dir
        int i=filename.lastIndexOf("/");
        lastDir = filename.left(i);

        // Save DEM
        manager->saveDem((char *)filename.toLocal8Bit().constData(), comboBox8->currentIndex());

    }
}
void DEMUserInterface_Qt::onDemGridSaveClicked()
{
    // File save dialog
    QString filename;
    QString filter;
    QString suffix;

    if (comboBox9->currentIndex() == 0)
    {
        filter = tr("Binary DSM Grid (*.dsm);; All files (*.*)");
        suffix = "dsm";
    }
    else
    {
        filter = tr("ASCII DSM Grid (*.txt);; All files (*.*)");
        suffix = "txt";
    }
    QFileDialog salvar(this,tr("Open file"),lastDir,filter);
    salvar.setAcceptMode(QFileDialog::AcceptSave);
    salvar.setDefaultSuffix(suffix);
    if(salvar.exec())
    {
        filename = salvar.selectedFiles()[0];
        if (filename.isEmpty())
            return;

        // Save last dir
        int i=filename.lastIndexOf("/");
        lastDir = filename.left(i);

        // Save DEM
        manager->saveDemGrid((char *)filename.toLocal8Bit().constData(), comboBox9->currentIndex());
    }
}
void DEMUserInterface_Qt::onDemLoadClicked()
{
    // File open dialog
    QString filter;
    if (comboBox8->currentIndex() == 0)
    {
        filter = tr("ASCII Full 2D + 3D (*.pix);; All files (*.*)");
    }
    else if (comboBox8->currentIndex() == 1)
    {
        filter = tr("ASCII 2D Points (*.txt);; All files (*.*)");
    }
    else if (comboBox8->currentIndex() == 2)
    {
        filter = tr("ASCII 2D Points (*.dat);; All files (*.*)");
    }
    else if (comboBox8->currentIndex() == 3)
    {
        filter = tr("Point Cloud (*.xyz);; All files (*.*)");
    }
    else if (comboBox8->currentIndex() == 4)
    {
        filter = tr("Point Cloud (*.ixyz);; All files (*.*)");
    }
    else
    {
        filter = tr("All files (*.*)");
    }
    QString filename = QFileDialog::getOpenFileName(this, tr("Open DSM file"), lastDir, filter) ;
    // if no file name written, return
    if (filename=="")
        return;

    // Save last dir
    int i=filename.lastIndexOf("/");
    lastDir = filename.left(i);

    // Load DEM
    if (!manager->loadDem((char *)filename.toLocal8Bit().constData(), comboBox8->currentIndex()))
    {
        QMessageBox::critical(this,"Load error","Error while loading file. Check if file format option matches the file.");
        return;
    }

    enableAfterDEM(1);
}

void DEMUserInterface_Qt::onDemGridLoadClicked()
{
    // File open dialog
    QString filter;
    if (comboBox9->currentIndex() == 0)
    {
        filter = tr("Binary DSM Grid (*.dsm);; All files (*.*)");
    }
    else
    {
        filter = tr("ASCII DSM Grid (*.txt);; All files (*.*)");
    }
    QString filename = QFileDialog::getOpenFileName(this, tr("Open DSM Grid file"), lastDir, filter) ;
    // if no file name written, return
    if (filename=="")
        return;
    // Save last dir
    int i=filename.lastIndexOf("/");
    lastDir = filename.left(i);

    // Load DEM Grid
    if (!manager->loadDemGrid((char *)filename.toLocal8Bit().constData(), comboBox9->currentIndex()))
    {
        QMessageBox::critical(this,"Load error","Error while loading file. Check if file format option matches the file.");
        return;
    }

    // Enable quality test
    tabWidget->setTabEnabled(6,true);
}

void DEMUserInterface_Qt::enableAfterDEM(int sender)
{
    // Enable options
    saveButton2->setEnabled(true);
    interButton->setEnabled(true);
    tabWidget->setTabEnabled(4, true);

    demSource = demSource | sender;

    if (demSource < 3)
    {
        if (sender == 2)
            comboBox0->setCurrentIndex(1);
    }
    else
    {
        comboBox0->setEnabled(true);
        comboBox0->setCurrentIndex(2);
    }
}

void DEMUserInterface_Qt::enableAfterGrid()
{
    // Enable options
    saveButton->setEnabled(true);
    tabWidget->setTabEnabled(6,true);
}

void DEMUserInterface_Qt::onDemGridClicked()
{
    if (!checkBoundingBox())
        return;

    // Perform interpolation
    manager->interpolateGrid(/*comboBox0->currentIndex(), */ comboBox1->currentIndex(), comboBox2_2->currentIndex(), XilineEdit->text().toDouble(), YilineEdit->text().toDouble(), XflineEdit->text().toDouble(), YflineEdit->text().toDouble(), doubleSpinBox_8->value(), doubleSpinBox_9->value(), comboBox6->currentIndex(), doubleSpinBox15->value(), doubleSpinBox16->value(), comboBox7->currentIndex(), comboBox0->currentIndex());

    if (manager->cancelFlag())
        return;

    enableAfterGrid();
}

void DEMUserInterface_Qt::setElapsedTime(double t, int opt)
{
    int hms = int(t);
    int h = hms/3600;
    hms -= 3600 * h;
    int m = hms/60;
    hms -= 60 * m;
    double s = double(hms);
    s = s + (t-floor(t));

    QString str_time;
    QString h_str, m_str, s_str;

    (int(h) != 1) ? h_str = " hours, " : h_str = " hour, ";
    (int(m) != 1) ? m_str = " minutes, " : m_str = " minute, ";
    (int(s) != 1) ? s_str = " seconds." : s_str = " second.";

    if (h>0) str_time = QString::number(h) + h_str;
    if (h>0 || m>0) str_time += QString::number(m) + m_str;
    str_time += QString::number(s) + s_str;

    switch (opt)
    {
    case 1 : timeLabel->setText(str_time); break;
    default : matchingTimeLabel->setText(str_time);
    }
}

void DEMUserInterface_Qt::onLSMCheckChanged(int state)
{
    doubleSpinBox17->setEnabled(state);
}

void DEMUserInterface_Qt::onMatchingMethodChanged(int opt)
{
    tabWidget->setTabEnabled(3, opt==0); // Cross-correlation
    tabWidget->setTabEnabled(2, opt==1); // LSM
}

void DEMUserInterface_Qt::setGridData(double Xi, double Yi, double Xf, double Yf, double Zi, double Zf, double res_x, double res_y, int width, int height)
{
    XiLabel->setText(QString::number(Xi,'f',2));
    YiLabel->setText(QString::number(Yi,'f',2));
    XfLabel->setText(QString::number(Xf,'f',2));
    YfLabel->setText(QString::number(Yf,'f',2));
    gridMinZLabel->setText(QString::number(Zi,'f',2));
    gridMaxZLabel->setText(QString::number(Zf,'f',2));
    XiLabel_2->setText(QString::number(res_x,'f',2));
    YiLabel_2->setText(QString::number(res_y,'f',2));
    XiLabel_3->setText(QString::number(width));
    YiLabel_3->setText(QString::number(height));
}

int DEMUserInterface_Qt::checkBoundingBox()
{
    double Xi, Yi, Xf, Yf;

    Xi = XilineEdit->text().toDouble();
    Xf = XflineEdit->text().toDouble();
    Yi = YilineEdit->text().toDouble();
    Yf = YflineEdit->text().toDouble();

    if (Xi >= Xf || Yi >= Yf)
    {
        QMessageBox::critical(this,"Error","Error: grid bounding box has inconsistent values !");
        return 0;
    }

    return 1;
}

void DEMUserInterface_Qt::onInterStateChanged(int opt)
{
    bool ena1 = (opt == 1 || opt == 2);
    bool ena2 = (opt == 1 || opt == 0);

    groupBox_5->setEnabled(ena1);
    groupBox_4->setEnabled(ena2);
}

void DEMUserInterface_Qt::onGridAreaLimitsStateChanged(int opt)
{
    bool enable = (opt == 1);

    XilineEdit->setEnabled(enable);
    XflineEdit->setEnabled(enable);
    YilineEdit->setEnabled(enable);
    YflineEdit->setEnabled(enable);
    label_53->setEnabled(enable);
    label_54->setEnabled(enable);
    label_55->setEnabled(enable);
    label_56->setEnabled(enable);
}

void DEMUserInterface_Qt::onShowImageStateChanged(int opt)
{
    manager->setShowImage(opt);
}

void DEMUserInterface_Qt::onLoadPtsButtonClicked()
{
    // Define file filters
    QString filetype;
    if (comboBox->currentIndex() == 0)
        filetype = tr("Point cloud (*.xyz);; All files (*.*)");
    else
        filetype = tr("Stereoplotter features file(*.spf);; All files (*.*)");

    // File open dialog
    QString filename = QFileDialog::getOpenFileName(this, tr("Open DSM file"), lastDir, filetype) ;

    // if no file name written, return
    if (filename=="")
        return;

    // Save last dir
    int i=filename.lastIndexOf("/");
    lastDir = filename.left(i);

    textEdit->setText(QString::fromLocal8Bit(manager->getDemQuality((char *)filename.toLocal8Bit().constData(), comboBox->currentIndex()).c_str()));
}

void DEMUserInterface_Qt::onSavePtsButtonClicked()
{
    // File open dialog
    QFileDialog salvar(this,tr("Save DSM quality report"),lastDir,tr("Text file (*.txt);; All files (*.*)"));
    salvar.setAcceptMode(QFileDialog::AcceptSave);
    salvar.setDefaultSuffix("txt");
    if(salvar.exec())
    {
        QString filename = salvar.selectedFiles()[0];

        // if no file name written, return
        if (filename.isEmpty())
            return;

        // To do: implement tag demQuality on EPP file
        // manager->addDEMQualityToXML(filename.toStdString());

        // Save last dir
        int i=filename.lastIndexOf("/");
        lastDir = filename.left(i);

        std::ofstream outfile(filename.toLocal8Bit().constData());

        //outfile << textEdit->toPlainText().toStdString();
        outfile << textEdit->toPlainText().toLocal8Bit().constData();

        outfile.close();
    }
}
/*
 * Image matching
 **/

void DEMUserInterface_Qt::onDemExtractionClicked()
{
    manager->setStdParameters(spinBox3_2->value(), spinBox3_3->value(), spinBox11_2->value(), spinBox11_3->value(), checkBox_5->isChecked());
    //        manager->setEliminateBadPoints(checkBox_4->isChecked()); // Works for images individually
    manager->setAutoExtractionSettings(comboBox3->currentIndex(), comboBox4->currentIndex(), spinBox1->value(), doubleSpinBox0->value());
    manager->setLSMSettings(spinBox3->value(), spinBox4->value(), doubleSpinBox5->value(), doubleSpinBox6->value(), spinBox7->value(), doubleSpinBox8->value(), doubleSpinBox9->value(), doubleSpinBox10->value(), checkBox->isChecked(), doubleSpinBox17->value());
    manager->setNCCSettings(spinBox11->value(), spinBox12->value(), doubleSpinBox13->value(), doubleSpinBox14->value());
    int DEMflag = manager->extractDEM(comboBox5->currentIndex(), checkBox_2->checkState());

    if (!manager->cancelFlag() && DEMflag)
        enableAfterDEM(1);

    if (checkBox_4->isChecked())
        manager->eliminateBadPoints();
}

/*
 * Image dealing
 **/

void DEMUserInterface_Qt::loadImage(Matrix & I, char *filename, double sample)
{
    int levels=256;

    QImage img;
    img.load(filename);

    int step = int(1.0/sample);
    unsigned int width = int(img.width()*sample);
    unsigned int height = int(img.height()*sample);
    int pixel;

    // Resize Matrix
    I.resize(height, width);

    progressBar->setValue(0);
    for (unsigned int i=1; i<=height; i++)
    {
        for (unsigned int j=1; j<=width; j++)
        {
            pixel = img.pixel((j-1)*step,(i-1)*step);
            pixel = (((pixel >> 16) & 0xFF) + ((pixel >> 8) & 0xFF) + (pixel & 0xFF)) / 3;
            I.set(i, j, pixel/double(levels-1));
        }
        progressBar->setValue((100*i)/height);
    }
}

int DEMUserInterface_Qt::saveImage(char *filename, Matrix *I)
{
    int levels = 256;

    QImage img(I->getCols(), I->getRows(), QImage::Format_RGB32); // Qt4

    int pixel;
    for (int i=1; i<=img.height(); i++)
    {
        for (int j=1; j<=img.width(); j++)
        {
            pixel = round(I->get(i,j)*double(levels-1));
            pixel = (pixel << 16) + (pixel << 8) + pixel;
            img.setPixel(j-1, i-1, pixel);
        }
    }

    img.save(filename,"BMP");

    return 1;
}

/*
 * Call other interfaces
 */

void DEMUserInterface_Qt::onSeedsEditorClicked()
{
    sed = new SeedEditorUserInterface_Qt(manager, this);
    this->setHidden(true);
    connect(sed,SIGNAL(closed(bool)),this,SLOT(onSeedEditorClosed()));
    sed->showMaximized();
}

void DEMUserInterface_Qt::onSeedEditorClosed()
{
    if (sed)
        delete sed;
    show();
}

void DEMUserInterface_Qt::onStereoplotterClicked()
{
    // File open dialog
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Stereoplotter file"), lastDir, tr("Stereoplotter file (*.spf);; All files (*.*)")) ;
    // if no file name written, return
    if (filename=="")
        return;

    // Save last dir
    int i=filename.lastIndexOf("/");
    lastDir = filename.left(i);

    if (manager->loadDemFeature((char *) filename.toLocal8Bit().constData()))
        enableAfterDEM(2);
}

void DEMUserInterface_Qt::showFatalErrorMessage(QString msg, bool abort=true)
{
    QMessageBox::critical(this, "Error",msg);

    if (abort)
        doneButton->click();
}

/**************************
 *                        *
 *  Seeds Editor Manager  *
 *                        *
 **************************/

SeedEditorUserInterface_Qt::SeedEditorUserInterface_Qt(DEMManager *manager, QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    this->manager = manager;

    viewer = new SeparatedStereoViewer(0);
    viewer->blockOpen();
    viewer->blockSave();

    viewer->getLeftMarker().setToOnlyEmitClickedMode(); // Pluges para que o novo display funcione
    viewer->getRightMarker().setToOnlyEmitClickedMode();
    connect(&viewer->getLeftMarker(),SIGNAL(clicked(QPointF)),this,SLOT(imageClicked(QPointF)));
    connect(&viewer->getRightMarker(),SIGNAL(clicked(QPointF)),this,SLOT(imageClicked(QPointF)));
    connect(saveButton,SIGNAL(clicked()),this,SLOT(saveSeeds()));
    connect(loadButton,SIGNAL(clicked()),this,SLOT(loadSeeds()));
    connect(addButton,SIGNAL(clicked()),this,SLOT(onAddButtonClicked()));
    connect(removeButton,SIGNAL(clicked()),this,SLOT(onRemoveButtonClicked()));
    connect(okButton,SIGNAL(clicked()),this,SLOT(closeOk()));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(comboBox1,SIGNAL(currentIndexChanged(int)),this,SLOT(onComboBox1Changed()));
    connect(checkBox,SIGNAL(stateChanged(int)),this,SLOT(onCheckBoxChanged()));
    connect(tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(onTableClicked()));
    connect(tableWidget,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(onTableClicked()));

    Marker mark(SymbolsResource::getCross(Qt::yellow, QSize(24, 24),2)); // Create marks
    viewer->getLeftMarker().changeMarker(mark);
    viewer->getRightMarker().changeMarker(mark);

    setCentralWidget(viewer);

    // Create image marks
    mark_seeds = new Marker(SymbolsResource::getCross(Qt::yellow, QSize(24, 24),2));
    mark_pairs = new Marker(SymbolsResource::getCross(Qt::red, QSize(24, 24),2));
    mark_empty = new Marker(SymbolsResource::getText(""));

    // Copy seed and pair lists
    pw.setAllowClose(false);
    manager->getPointList(seeds,pairs);
    addPairs();
    updateData(/*0*/);

    // Empty list of selected seeds
    sel_seeds.clear();

    lastDir = ".";
}

void SeedEditorUserInterface_Qt::closeEvent(QCloseEvent *)
{
    emit closed(true);
}

int SeedEditorUserInterface_Qt::findKey(int seed_id)
{
    for (int i=0; i<tableWidget->rowCount(); i++)
    {
        if (tableWidget->item(i,0)->text().toInt() == seed_id)
            return no_pairs + (i+1);
    }

    return -1;
}

/*
 * Reveives only when viewer mark option is set
 */
void SeedEditorUserInterface_Qt::imageClicked(QPointF p)
{
    checkSelectedSeeds();

    if (sel_seeds.size() < 1)
    {
        QMessageBox::warning(this,"Warning","There is no seed pair selected to edit");
        return;
    }

    if (sel_seeds.size() > 1)
    {
        QMessageBox::warning(this,"Warning","There is are multiple seed pairs selected to edit");
        return;
    }

    // Edit and modify
    // Mark key ranges from 1-N
    int seed_id = tableWidget->item(sel_seeds.at(0),0)->text().toInt();
    int key = findKey(seed_id);
    int left_id, right_id;
    getImagesIds(left_id, right_id);
    QTableWidgetItem *item;

    if (sender() == &viewer->getLeftMarker())
    {
        viewer->getLeftMarker().insertMark(p, key, QString::number(seed_id), mark_seeds);
        item = tableWidget->item(sel_seeds.at(0),1);
        item->setText(QString::number(p.x()));
        item = tableWidget->item(sel_seeds.at(0),2);
        item->setText(QString::number(p.y()));
        seeds.get(seed_id)->left_x = double(p.x());
        seeds.get(seed_id)->left_y = double(p.y());
    }
    else
    {
        viewer->getRightMarker().insertMark(p, key, QString::number(seed_id), mark_seeds);
        item = tableWidget->item(sel_seeds.at(0),3);
        item->setText(QString::number(p.x()));
        item = tableWidget->item(sel_seeds.at(0),4);
        item->setText(QString::number(p.y()));
        seeds.get(seed_id)->right_x = double(p.x());
        seeds.get(seed_id)->right_y = double(p.y());
    }
}

/*
 * Control table events
 */
void SeedEditorUserInterface_Qt::onTableClicked(/*int row, int col*/)
{
    checkSelectedSeeds();

    if (sel_seeds.size() != 1)
        return;

    // Center coordinates
    double x1,y1, x2, y2;

    x1 = tableWidget->item(sel_seeds.at(0),1)->text().toDouble();
    y1 = tableWidget->item(sel_seeds.at(0),2)->text().toDouble();
    x2 = tableWidget->item(sel_seeds.at(0),3)->text().toDouble();
    y2 = tableWidget->item(sel_seeds.at(0),4)->text().toDouble();

    // Check if current seed pair was not measured
    if (int(x1) == 0 || int(y1) == 0 || int(x2) == 0 || int(y2) == 0 )
        return;

    viewer->getLeftDisplay()->getCurrentScene()->moveTo(QPointF(x1,y1));
    viewer->getRightDisplay()->getCurrentScene()->moveTo(QPointF(x2,y2));

    viewer->update();
}

void SeedEditorUserInterface_Qt::getImagesIds(int &left_id, int &right_id)
{
    int i = comboBox1->currentIndex();

    std::deque<int> listPairs = manager->getImagesPairs();
    std::deque<Image*> listAllImages = manager->getImages();

    // Decode
    int no_imgs = listAllImages.size();
    left_id = 1 + (listPairs.at(i) % no_imgs);
    right_id = 1 + (listPairs.at(i) / no_imgs);
}

void SeedEditorUserInterface_Qt::onAddButtonClicked()
{
    // Add new line
    int key = tableWidget->rowCount();
    QTableWidgetItem *newItem;

    tableWidget->insertRow(key);

    newItem = new QTableWidgetItem(tr("%1").arg(seeds.size()+1));
    tableWidget->setItem(key, 0, newItem);
    newItem = new QTableWidgetItem("");
    tableWidget->setItem(key, 1, newItem);
    newItem = new QTableWidgetItem("");
    tableWidget->setItem(key, 2, newItem);
    newItem = new QTableWidgetItem("");
    tableWidget->setItem(key, 3, newItem);
    newItem = new QTableWidgetItem("");
    tableWidget->setItem(key, 4, newItem);

    tableWidget->clearSelection();
    tableWidget->setCurrentItem(newItem);

    // Add new object to the seeds
    MatchingPoints mp;
    int left_id, right_id;
    getImagesIds(left_id, right_id);
    seeds.add(left_id, right_id, -1.0, -1.0, -1.0, -1.0, 0.0);

    // Add empty points to image pair
    viewer->getLeftMarker().insertMark(QPointF(-1,-1), key + 1, "", mark_empty);
    viewer->getRightMarker().insertMark(QPointF(-1,-1), key + 1, "", mark_empty);

    no_seeds++;
}

void SeedEditorUserInterface_Qt::onRemoveButtonClicked()
{
    checkSelectedSeeds();

    // No items selected
    if (sel_seeds.size() < 1)
    {
        QMessageBox::warning(this,"Warning","There is no seed pair selected to erase");
        return;
    }

    // Delete all seed marks
    // **** no_seeds = number of seeds for each pair ****
    int key = no_pairs + 1;
    for (int i=0; i<no_seeds; i++)
    {
        viewer->getLeftMarker().deleteMark(key);
        viewer->getRightMarker().deleteMark(key);
        key++;
    }

    // Start to erase
    int seed_id;
    for (int i=sel_seeds.size()-1; i>=0; i--)
    {
        seed_id = tableWidget->item(sel_seeds.at(i),0)->text().toInt();
        seeds.remove(seed_id);
    }

    // Update Seed Marks and Table
    addSeedsAndTable();

    viewer->update();
}

/*
 * This function creates a list with all indexes selected on the table
 * Returns: sel_seed vector updated
 */
void SeedEditorUserInterface_Qt::checkSelectedSeeds()
{
    QList<QTableWidgetItem *> selected;
    selected = tableWidget->selectedItems();
    int cols = tableWidget->columnCount();
    int no_pairs = selected.size()/cols;

    sel_seeds.clear();

    if (no_pairs < 1)
        return;

    // Qt table bug
    // Multiple selection from line 0, selected by shift key,
    // create a list full of 0s
    bool flag=false;
    if (no_pairs > 1)
    {
        if (selected.at(0)->row() == selected.at(1*cols)->row())
            flag = true;
    }

    // Add manually indexes, to fix this bug
    if (flag)
    {
        for (int i=0; i<no_pairs; i++)
            sel_seeds.push_back(i+selected.at(0)->row());
    }

    for (int i=0; i<no_pairs*cols; i+=cols)
        sel_seeds.push_back(selected.at(i)->row());

    // Sort selection seeds
    int aux;
    for (unsigned int i=0; i<sel_seeds.size()-1; i++)
    {
        for (unsigned int j=i+1; j<sel_seeds.size(); j++)
        {
            if (sel_seeds.at(i) > sel_seeds.at(j))
            {
                aux = sel_seeds.at(i);
                sel_seeds.at(i) = sel_seeds.at(j);
                sel_seeds.at(j) = aux;
            }
        }
    }
}

void SeedEditorUserInterface_Qt::onCheckBoxChanged(/*int state*/)
{
    updateMarks();
}

void SeedEditorUserInterface_Qt::closeOk()
{
    // Check if there are no unmeasured points
    if (seeds.hasEmptyPairs())
    {
        QMessageBox::critical(this,"Error","There are still unmeasured pairs");
        return;
    }

    // Copy new seed list to manager
    manager->overwriteSeedsList(seeds);

    close();
}

/*
 * Create pair list
 */
void SeedEditorUserInterface_Qt::addPairs()
{
    std::deque<int> listPairs = manager->getImagesPairs();
    std::deque<Image*> listAllImages = manager->getImages();

    // Add pairs to the interface
    int left_id, right_id;
    int no_imgs = listAllImages.size();
    std::string str_left, str_right;
    std::stringstream txt;

    for (unsigned int i=0; i<listPairs.size(); i++)
    {
        // Decode
        left_id = 1 + (listPairs.at(i) % no_imgs);
        right_id = 1 + (listPairs.at(i) / no_imgs);
        str_left = listAllImages.at(left_id-1)->getImageId();
        str_right = listAllImages.at(right_id-1)->getImageId();
        txt.str(""); // Clear stream
        txt << "Images " << str_left << " and " << str_right;
        comboBox1->addItem(QString::fromLatin1((char *)txt.str().c_str()));
    }
}

void SeedEditorUserInterface_Qt::saveSeeds()
{
    QFileDialog salvar(this,tr("Save seeds"),lastDir,tr("Seeds (*.txt);; All files (*.*)"));
    salvar.setAcceptMode(QFileDialog::AcceptSave);
    salvar.setDefaultSuffix("txt");
    if(salvar.exec())
    {
        QString filename = salvar.selectedFiles()[0];
        if (filename.isEmpty())
            return;

        // Save last dir
        int i=filename.lastIndexOf("/");
        lastDir = filename.left(i);

        // Save seeds
        seeds.save((char *)filename.toLocal8Bit().constData(), 0);
        // Expanção do XML
        //manager->addSeedsToXML(filename.toStdString());
        manager->addSeedsToXML(filename.toLocal8Bit().constData());
    }
}
void SeedEditorUserInterface_Qt::loadSeeds()
{
    QString filename;

    filename = QFileDialog::getOpenFileName(this, tr("Load seeds"), lastDir, tr("Seeds (*.txt);; All files (*.*)"));
    if (filename=="")
        return;

    // Save last dir
    int i=filename.lastIndexOf("/");
    lastDir = filename.left(i);

    // Load seeds
    seeds.load((char *)filename.toLocal8Bit().constData(), 0);

    // Update data
    updateData(/*comboBox1->currentIndex()*/);
}

void SeedEditorUserInterface_Qt::onComboBox1Changed(/*int index*/)
{
    updateData(/*index*/);
}

/*
 * Add matching points to the viewer
 */
void SeedEditorUserInterface_Qt::addMatchingPoints()
{
    MatchingPoints *mp;
    QPointF left_coord, right_coord;
    int left_id, right_id;
    getImagesIds(left_id,right_id);
    int total_pairs = pairs.size();
    no_pairs = 0;

    pw.setDescription("Loading matching points");
    pw.show();

    for (int i=0; i<total_pairs; i++)
    {
        pw.setProgress(100*(i+1)/total_pairs);
        qApp->processEvents();

        mp = pairs.get(i+1);

        if (mp->left_image_id != left_id || mp->right_image_id != right_id)
            continue;

        left_coord.setX(double(mp->left_x));
        left_coord.setY(double(mp->left_y));
        right_coord.setX(double(mp->right_x));
        right_coord.setY(double(mp->right_y));
        viewer->getLeftMarker().addMark(left_coord, no_pairs+1, "", mark_pairs);
        viewer->getRightMarker().addMark(right_coord, no_pairs+1, "", mark_pairs);

        no_pairs++;
    }

    pw.hide();

    pairsLabel->setText(QString::number(no_pairs)+"/"+QString::number(pairs.size()));
}

/*
 * Add seed points to the viewer
 * Update seeds table
 */
void SeedEditorUserInterface_Qt::addSeedsAndTable()
{
    MatchingPoints *mp;
    QPointF left_coord, right_coord;
    int left_id, right_id;
    getImagesIds(left_id,right_id);

    QTableWidgetItem *newItem;
    no_seeds=0;
    tableWidget->setRowCount(0);

    int key = no_pairs + 1;

    for (unsigned int i=0; i<seeds.size(); i++)
    {
        mp = seeds.get(i+1);

        if (mp->left_image_id != left_id || mp->right_image_id != right_id)
            continue;

        // Image points
        left_coord.setX(double(mp->left_x));
        left_coord.setY(double(mp->left_y));
        right_coord.setX(double(mp->right_x));
        right_coord.setY(double(mp->right_y));

        if (mp->left_x > 0 || mp->left_y > 0)
            viewer->getLeftMarker().addMark(left_coord, key, QString::number(i+1), mark_seeds);
        else
            viewer->getLeftMarker().addMark(left_coord, key, "", mark_empty);

        if (mp->right_x > 0 || mp->right_y > 0)
            viewer->getRightMarker().addMark(right_coord, key, QString::number(i+1), mark_seeds);
        else
            viewer->getRightMarker().addMark(right_coord, key, "", mark_empty);

        // Table
        tableWidget->insertRow(no_seeds);

        newItem = new QTableWidgetItem(tr("%1").arg(i+1));
        newItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        tableWidget->setItem(no_seeds, 0, newItem);

        // Write only if point was measured
        (mp->left_x > 0 || mp->left_y > 0) ? newItem = new QTableWidgetItem(tr("%1").arg(mp->left_x)) : newItem = new QTableWidgetItem("");
        newItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        tableWidget->setItem(no_seeds, 1, newItem);
        (mp->left_x > 0 || mp->left_y > 0) ? newItem = new QTableWidgetItem(tr("%1").arg(mp->left_y)) : newItem = new QTableWidgetItem("");
        newItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        tableWidget->setItem(no_seeds, 2, newItem);

        (mp->right_x > 0 || mp->right_y > 0) ? newItem = new QTableWidgetItem(tr("%1").arg(mp->right_x)) : newItem = new QTableWidgetItem("");
        newItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        tableWidget->setItem(no_seeds, 3, newItem);
        (mp->right_x > 0 || mp->right_y > 0) ? newItem = new QTableWidgetItem(tr("%1").arg(mp->right_y)) : newItem = new QTableWidgetItem("");
        newItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        tableWidget->setItem(no_seeds, 4, newItem);

        no_seeds++;
        key++;
    }

    seedsLabel->setText(QString::number(no_seeds)+"/"+QString::number(seeds.size())+"  ");
}

void SeedEditorUserInterface_Qt::updateMarks()
{
    //
    //  Clear marks
    //
    viewer->getLeftDisplay()->getCurrentScene()->geometry()->clear();
    viewer->getRightDisplay()->getCurrentScene()->geometry()->clear();

    //
    // Add matching points, if selected
    //
    no_pairs = 0;
    if (checkBox->isChecked() && pairs.size() > 0)
        addMatchingPoints();
    else
        pairsLabel->setText("0/0");

    //
    // Update table and seed points on image
    //
    no_seeds = 0;
    addSeedsAndTable();

    //
    // Update viewer
    //
    viewer->update();
}

void SeedEditorUserInterface_Qt::updateData(/*int i*/)
{
    //
    // Update current images
    //
    std::deque<Image*> listAllImages = manager->getImages();
    int left_id, right_id;
    getImagesIds(left_id, right_id);

    //
    // Update Images
    //
    std::string left_file = listAllImages.at(left_id-1)->getFilepath() + '/' + listAllImages.at(left_id-1)->getFilename();
    std::string right_file = listAllImages.at(right_id-1)->getFilepath() + '/' + listAllImages.at(right_id-1)->getFilename();

    //viewer->loadLeftImage(QString::fromStdString(left_file));
    viewer->loadLeftImage(QString::fromLocal8Bit(left_file.c_str()));
    //viewer->loadRightImage(QString::fromStdString(right_file));
    viewer->loadRightImage(QString::fromLocal8Bit(right_file.c_str()));

    //
    // Update marks
    //
    updateMarks();
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
