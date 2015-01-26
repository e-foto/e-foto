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
#include "OrthoUserInterface_Qt.h"

#include "DemFeatures.h"
#include "ImageViewers.h"
#include "SingleTools.h"
#include "SingleDisplay.h"

#include <qdesktopwidget.h>
#include <qapplication.h>
#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qaction.h>
#include <qstring.h>
#include "Orthorectification.h"
#include "OrthoManager.h"
#include <math.h>

#include <QCloseEvent>

#include <iomanip>
#include <fstream>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

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
        QObject::connect(loadButton, SIGNAL(clicked()), this, SLOT(onLoadOrthoClicked()));
    QObject::connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(onShowImageChanged(int)));
        QObject::connect(orthoQualityButton, SIGNAL(clicked()), this, SLOT(onOrthoQualityButtonClicked()));

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

        ortho_qual_form = NULL;

        lastDir = ".";

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
        QString filename = QFileDialog::getOpenFileName(this, tr("Open DEM file"), lastDir, tr("DEM (*.dsm);; Text file (*.txt);; All files (*.*)")) ;
    // if no file name written, return
    if (filename=="")
        return;

    // Save last dir
    int i=filename.lastIndexOf("/");
        lastDir = filename.left(i);

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

void OrthoUserInterface_Qt::onLoadOrthoClicked()
{
        // File open dialog
        QString filename = QFileDialog::getOpenFileName(this, tr("Open Ortho-image file"), lastDir, tr("EOI (*.eoi);; All files (*.*)")) ;
        // if no file name written, return
        if (filename=="")
                return;

        // Save last dir
        int i=filename.lastIndexOf("/");
        lastDir = filename.left(i);

        // Add file to line edit
        lineEdit->setText(filename);

        // Load Ortho
        manager->loadOrtho((char *)lineEdit->text().toStdString().c_str());

        orthoQualityButton->setEnabled(true);
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
        QString filename = QFileDialog::getSaveFileName(this, tr("Save Orthoimage"), lastDir, tr("E-FOTO Orthoimage (*.eoi);; All files (*.*)")) ;
    // if no file name written, return
    if (filename=="")
        return;

    // Save last dir
    int i=filename.lastIndexOf("/");
        lastDir = filename.left(i);

    disableOptions();
    setAllowClose(false);
    manager->setInterMethod(comboBox4->currentIndex());
    // To do: solve unused field (comboBox3) of user interface
    manager->orthoRectification((char *)filename.toStdString().c_str()/*,comboBox3->currentIndex()*/, comboBox->currentIndex(), doubleSpinBox1->value(), doubleSpinBox2->value());
    setAllowClose(true);
    enableOptions();
    setCurrentWork("Done");
        orthoQualityButton->setEnabled(true);
}

void OrthoUserInterface_Qt::onOrthoQualityButtonClicked()
{
    // Open Ortho-image Quality Editor

    ortho_qual_form = new OrthoQualityUserInterface_Qt(manager, this);

    Matrix * img = manager->getOrtho()->getOrthoImage();
    double Xi, Yi, Xf, Yf, res_x, res_y;
    manager->getOrtho()->getOrthoParametersA(Xi, Yi, Xf, Yf, res_x, res_y);
    ortho_qual_form->showImage2D(img, Xi, res_x, Yi, res_y, false);

    this->setHidden(true);
    connect(ortho_qual_form,SIGNAL(closed(bool)),this,SLOT(onCloseOrthoQualityForm()));
    ortho_qual_form->showMaximized();
}

void OrthoUserInterface_Qt::onCloseOrthoQualityForm()
{
    if (ortho_qual_form)
        delete ortho_qual_form;

    show();
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
        loadButton->setEnabled(false);
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
        loadButton->setEnabled(true);
}

void OrthoUserInterface_Qt::setProgress(int progress)
{
    if (progress < 0) progress = 0;
    if (progress > 100) progress = 100;

    progressBar->setValue(progress);
    qApp->processEvents();
}

void OrthoUserInterface_Qt::setCurrentWork(std::string msg)
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
void OrthoUserInterface_Qt::loadImage(Matrix & I, char *filename, double sample)
{
    int levels=256;

    QImage img;
    img.load(filename);

    int step = int(1.0/sample);
    int width = int(img.width()*sample);
    int height = int(img.height()*sample);
    int pixel;

        // Resize Matrix
        I.resize(height, width);

    progressBar->setValue(0);
    for (int i=1; i<=height; i++)
    {
        for (int j=1; j<=width; j++)
        {
            pixel = img.pixel((j-1)*step,(i-1)*step);
            //	  		pixel = ((pixel >> 16) & 0xFF)*0.2989 + ((pixel >> 8) & 0xFF)*0.5870 + (pixel & 0xFF)*0.1140; // Color to gray 8-bit
            //                        pixel = (((pixel >> 16) & 0xFF) + ((pixel >> 8) & 0xFF) + (pixel & 0xFF)) / 3; // Simple color to gray 8-bit
            //                        pixel = pixel & 0xFF; // Gray 24-bit to 8-bit
            //                        I->set(i, j, pixel/double(levels-1));
                        I.set(i, j, double(pixel&0xFFFFFF)/double(0xFFFFFF)); // Color 24-bit (RR GG BB) to 0-1
        }
        progressBar->setValue((100*i)/height);
    }
}

int OrthoUserInterface_Qt::saveImage(char *filename, Matrix *I)
{
    int levels = 256;

    QImage img(I->getCols(), I->getRows(), QImage::Format_RGB32); // Qt4

    int pixel;
    for (int i=1; i<=img.height(); i++)
    {
        for (int j=1; j<=img.width(); j++)
        {
            //                        pixel = round(I->get(i,j)*double(levels-1));
            //                        pixel = (pixel << 16) + (pixel << 8) + pixel; // Gray to color 24-bit
            pixel = int(I->get(i,j)*double(0xFFFFFF));

            img.setPixel(j-1, i-1, pixel);
        }
    }

    // Expanção do XML
    manager->addOrthoToXML2(std::string(filename));

    img.save(filename,"BMP");

    return 1;
}

void OrthoUserInterface_Qt::showImage2D(Matrix* image, double xi, double dx, double yi, double dy, bool isGrayscale)
{
        SingleViewer* sv = new SingleViewer(this);
        sv->setOrtoImageMode(xi, dx, yi, dy);
        sv->loadImage(image, isGrayscale);
        sv->blockOpen();
        sv->blockMark();
        sv->show();
}

void OrthoUserInterface_Qt::showImage3D(Matrix* image, double xi, double dx, double yi, double dy, double zi, double dz, bool isGrayscale)
{
        SingleViewer* sv = new SingleViewer(this);
        sv->setElevationImageMode(xi, dx, yi, dy, zi, dz);
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

/*******************************************************************************************************
 *
 *                      Ortho-quality class
 *
 *******************************************************************************************************/

OrthoQualityUserInterface_Qt::OrthoQualityUserInterface_Qt(OrthoManager *manager, QWidget *parent) : QMainWindow(parent)
{
        setupUi(this);
        this->manager = manager;

        viewer = new SingleViewer(0);
        viewer->blockOpen();
        viewer->blockSave();
        viewer->getMarker()->setToOnlyEmitClickedMode();

        tableWidget->setRowCount(0);

        connect(viewer->getMarker(),SIGNAL(clicked(QPointF)),this,SLOT(imageClicked(QPointF)));
        connect(loadButton,SIGNAL(clicked()),this,SLOT(loadPoints()));
        connect(saveButton,SIGNAL(clicked()),this,SLOT(saveQuality()));
        connect(doneButton,SIGNAL(clicked()),this,SLOT(close()));
        connect(deleteButton,SIGNAL(clicked()),this,SLOT(onDeletePoint()));
        connect(calculateButton,SIGNAL(clicked()),this,SLOT(calculateAll()));
        connect(checkBox,SIGNAL(stateChanged(int)),this,SLOT(onCheckBoxChanged(int)));
        connect(tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(onTableClicked(int,int)));

        setCentralWidget(viewer);

        // Create image marks
        mark_ortho = new Marker(SymbolsResource::getCross(Qt::yellow, QSize(24, 24),2));
        mark_gnd = new Marker(SymbolsResource::getCross(Qt::red, QSize(24, 24),2));
        mark_empty = new Marker(SymbolsResource::getText(""));

        lastDir = ".";
}

void OrthoQualityUserInterface_Qt::closeEvent(QCloseEvent *)
{
        emit closed(true);
}

void OrthoQualityUserInterface_Qt::showImage2D(Matrix* image, double xi, double dx, double yi, double dy, bool isGrayscale)
{
        viewer->setOrtoImageMode(xi, dx, yi, dy);
        viewer->loadImage(image, isGrayscale);
        viewer->blockOpen();
}

void OrthoQualityUserInterface_Qt::imageClicked(QPointF p)
{
        int sel_row = tableWidget->currentRow();
        int num_points = tableWidget->rowCount() - 2;

        if ((sel_row < 0) || (sel_row >= num_points))
        {
                QMessageBox::warning(this,"Warning","There is no selected point to measure");
                return;
        }

        // Calculate X and Y
        double X, Y, col, row;
        col = p.x() + 1.0;
        row = manager->getOrtho()->getHeight() - p.y(); // Convert matrix to image coordinate system
        row++;

        manager->getOrtho()->getXYAt(col, row, X, Y);

        // Update table
        setTableAt(sel_row,2,X);
        setTableAt(sel_row,3,Y);

        // Update marks
        updateMarks();

        viewer->update();
}

void OrthoQualityUserInterface_Qt::onTableClicked(int row, int col)
{
        int num_points = tableWidget->rowCount() - 2;

        if (row >= num_points)
            return;

        // Center coordinates
        double X, Y, rr, cc;

        X = getDoubleTableAt(row,0);
        Y = getDoubleTableAt(row,1);

        manager->getOrtho()->getColRowAt(X, Y, cc, rr);

        cc--; // Images coordinates ranges from 0
        rr--;
        rr = manager->getOrtho()->getHeight() - rr; // Convert matrix to image coordinate system

        viewer->getDisplay()->getCurrentScene()->moveTo(QPointF(cc,rr));
        viewer->update();
}

void OrthoQualityUserInterface_Qt::updateMarks()
{
    double col, row, X, Y;
    QPointF p;
    int no_points = tableWidget->rowCount()-2, key=1;

    // Clear marks
    viewer->getDisplay()->getCurrentScene()->geometry()->clear();

    for(int i=0; i<no_points; i++)
    {
        // Reference points

        // Calculate col and row - ranges from 1-N
        X = getDoubleTableAt(i,0);
        Y = getDoubleTableAt(i,1);
        manager->getOrtho()->getColRowAt(X, Y, col, row);
        col--; // Images coordinates ranges from 0
        row--;
        row = manager->getOrtho()->getHeight() - row; // Convert matrix to image coordinate system
        p.setX(col);
        p.setY(row);

        // Add mark
        if (checkBox->isChecked())
            viewer->getMarker()->addMark(p, key, QString::number(i+1), mark_gnd);
        else
            viewer->getMarker()->addMark(p, key, "", mark_empty);

        // Ortho-image points

        X = getDoubleTableAt(i,2);
        Y = getDoubleTableAt(i,3);
        manager->getOrtho()->getColRowAt(X, Y, col, row);
        col--; // Images coordinates ranges from 0
        row--;
        row = manager->getOrtho()->getHeight() - row; // Convert matrix to image coordinate system
        p.setX(col);
        p.setY(row);

        // Add mark
        if ((fabs(X-0.0)<0.00000000001) && (fabs(Y-0.0)<0.00000000001))
            viewer->getMarker()->addMark(p, key+1, "", mark_empty);
        else
            viewer->getMarker()->addMark(p, key+1, QString::number(i+1), mark_ortho);

        key+=2;
    }

    viewer->update();
}

void OrthoQualityUserInterface_Qt::onCheckBoxChanged(int state)
{
    updateMarks();
}

void OrthoQualityUserInterface_Qt::onDeletePoint()
{
    int sel_row = tableWidget->currentRow();
    tableWidget->removeRow(sel_row);
    updateMarks();
}

void OrthoQualityUserInterface_Qt::saveQuality()
{
        QString filename;

        filename = QFileDialog::getSaveFileName(this, tr("Save ortho-image quality report"), lastDir, tr("Text file (*.txt);; All files (*.*)"));
        if (filename=="")
                return;

        // Save last dir
        int i=filename.lastIndexOf("/");
        lastDir = filename.left(i);

        std::ofstream outfile((char *)filename.toStdString().c_str());

        if (outfile.fail())
                return;

        outfile << std::setprecision(5);

        outfile << "Terrain X\tTerrain Y\tOrtho-image X\tOrtho-image Y\tX error\tY Error\tPlanimetric Error\n";

        // Save table (may be imported or pasted on Excel or Calc)
        for (int i=0; i<tableWidget->rowCount(); i++)
        {
            outfile << getTableAt(i,0) << "\t" << getTableAt(i,1) << "\t" << getTableAt(i,2) << "\t" << getTableAt(i,3) << "\t" << getTableAt(i,4) << "\t" << getTableAt(i,5) << "\t" << getTableAt(i,6) << "\n";
        }

        outfile.close();
}

void OrthoQualityUserInterface_Qt::loadPoints()
{
    QString filename;

    filename = QFileDialog::getOpenFileName(this, tr("Load points for testing quality"), lastDir, tr("Points (*.txt);; All files (*.*)"));
    if (filename=="")
            return;

    // Save last dir
    int i=filename.lastIndexOf("/");
    lastDir = filename.left(i);

    // Check open option
    switch (comboBox1->currentIndex())
    {
        case 0: loadPointsFromTxt((char *)filename.toStdString().c_str()); break;
        case 1: loadPointsFromSP((char *)filename.toStdString().c_str()); break;
        case 2: loadPointsFromQuality((char *)filename.toStdString().c_str()); break;
    }

    updateMarks();
}

int OrthoQualityUserInterface_Qt::loadPointsFromSP(char *filename)
{
    // Use Dem feature in order to get points
    DemFeatures sp_points;
    if (!sp_points.loadFeatures(filename/*, 0*/, false))
        return 0;

    // Clear table
    tableWidget->setRowCount(0);
    QTableWidgetItem *newItem;
    int tab_pos = 0;

    // Copy XYZ to table
    for (int i = 0; i < sp_points.getNumFeatures(); i++)
    {
        DemFeature* df = sp_points.getFeatureLink(i+1);

        for (unsigned int j = 0; j < df->points.size(); j++)
        {
            // Add new table items
            tableWidget->insertRow(tab_pos);
            for (int k=0; k<7; k++)
            {
                switch (k)
                {
                    case 0 : newItem = new QTableWidgetItem(QString::number(df->points.at(j).X,'f',5)); break;
                    case 1 : newItem = new QTableWidgetItem(QString::number(df->points.at(j).Y,'f',5)); break;
                    default : newItem = new QTableWidgetItem(""); break;
                }
                newItem->setTextAlignment(Qt::AlignCenter);
                tableWidget->setItem(tab_pos, k, newItem);
            }
            tab_pos++;
        }
    }

    addTableEnding(tab_pos);

    return 1;
}

int OrthoQualityUserInterface_Qt::loadPointsFromTxt(char *filename)
{
    // Use text in order to get points
    // Tex file format:
    // X1 Y1 Z1
    // X2 Y2 Z2
    // ...

    std::ifstream arq(filename);

    if (arq.fail())
            return 0;

    double X=0.0, Y, Z;
    tableWidget->setRowCount(0);
    QTableWidgetItem *newItem;
    int tab_pos = 0;

    while (!arq.fail())
    {
        arq >> X >> Y >> Z;

        if (fabs(X - 0.0) < 0.0000000001)
            break;

        // Add new table items
        tableWidget->insertRow(tab_pos);
        for (int k=0; k<7; k++)
        {
            switch (k)
            {
                case 0 : newItem = new QTableWidgetItem(QString::number(X,'f',5)); break;
                case 1 : newItem = new QTableWidgetItem(QString::number(Y,'f',5)); break;
                default : newItem = new QTableWidgetItem(""); break;
            }
            newItem->setTextAlignment(Qt::AlignCenter);
            tableWidget->setItem(tab_pos, k, newItem);
        }

        X = 0.0; // Clear variable X
        tab_pos++;
    }

    arq.close();

    addTableEnding(tab_pos);

    return 1;
}

int OrthoQualityUserInterface_Qt::loadPointsFromQuality(char *filename)
{
    // Open ortho-imae quality file

    std::ifstream arq(filename);

    if (arq.fail())
            return 0;

    double RX, RY, OX, OY, dX, dY, pm;
    tableWidget->setRowCount(0);
    QTableWidgetItem *newItem;
    int tab_pos = 0;

    // Skip first line (header)
    std::string tag;
    getline(arq,tag);
    tag = tag.substr(0,9);
    if (tag.compare("Terrain X") != 0)
    {
        QMessageBox::critical(this,"Error","Error: this file is not a ortho-image quality file.");
        return 0;
    }

    while (!arq.fail())
    {
        arq >> RX >> RY >> OX >> OY >> dX >> dY >> pm;

        // Add new table items
        tableWidget->insertRow(tab_pos);
        for (int k=0; k<7; k++)
        {
            switch (k)
            {
                case 0 : newItem = new QTableWidgetItem(QString::number(RX,'f',5)); break;
                case 1 : newItem = new QTableWidgetItem(QString::number(RY,'f',5)); break;
                case 2 : newItem = new QTableWidgetItem(QString::number(OX,'f',5)); break;
                case 3 : newItem = new QTableWidgetItem(QString::number(OY,'f',5)); break;
                default : newItem = new QTableWidgetItem(""); break;
            }
            newItem->setTextAlignment(Qt::AlignCenter);
            tableWidget->setItem(tab_pos, k, newItem);
        }

        tab_pos++;
    }

    arq.close();

    // Remove last 2 rows (avg and std)
    tableWidget->removeRow(tableWidget->rowCount()-1);

    tab_pos = tableWidget->rowCount();
    addTableEnding(tab_pos);

    calculateAll();

    return 1;
}

void OrthoQualityUserInterface_Qt::addTableEnding(int tab_pos)
{
    QTableWidgetItem *newItem;

    // Add average and standard deviation rows
    tableWidget->insertRow(tab_pos);
    newItem = new QTableWidgetItem("Average");
    tableWidget->setItem(tab_pos, 3, newItem);
    tableWidget->insertRow(tab_pos+1);
    newItem = new QTableWidgetItem("Std dev");
    tableWidget->setItem(tab_pos+1, 3, newItem);

    // Add empty values
    for (int i=0; i<3; i++)
    {
        newItem = new QTableWidgetItem("");
        tableWidget->setItem(tab_pos, i, newItem);
        newItem = new QTableWidgetItem("");
        tableWidget->setItem(tab_pos+1, i, newItem);
    }

    // Add initial values
    for (int i=4; i<7; i++)
    {
        newItem = new QTableWidgetItem("0.0");
        newItem->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(tab_pos, i, newItem);
        newItem = new QTableWidgetItem("0.0");
        newItem->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(tab_pos+1, i, newItem);
    }

}

std::string OrthoQualityUserInterface_Qt::getTableAt(int row, int col)
{
    QTableWidgetItem *selItem;
    selItem = tableWidget->item(row, col);
    return selItem->text().toStdString();
}

double OrthoQualityUserInterface_Qt::getDoubleTableAt(int row, int col)
{
    QTableWidgetItem *selItem;
    selItem = tableWidget->item(row, col);
    return selItem->text().toDouble();
}

void OrthoQualityUserInterface_Qt::setTableAt(int row, int col, double value)
{
    QTableWidgetItem *selItem;
    selItem = tableWidget->item(row, col);
    selItem->setText(QString::number(value,'f',5));
}

void OrthoQualityUserInterface_Qt::calculateAll()
{
    int no_points = tableWidget->rowCount() - 2; // Discard average and std lines from table
    double avg[3] = {0.0, 0.0, 0.0}, stddev[3] = {0.0, 0.0, 0.0};
    double dX, dY, planerr;

    if (no_points < 1)
        return;

    // Calculate errors
    for (int i=0; i<no_points; i++)
    {
        dX = getDoubleTableAt(i,2) - getDoubleTableAt(i,0);
        dY = getDoubleTableAt(i,3) - getDoubleTableAt(i,1);
        planerr = sqrt(dX*dX + dY*dY);
        setTableAt(i,4,dX);
        setTableAt(i,5,dY);
        setTableAt(i,6,planerr);
    }

    // Calculate average based on errors
    for (int i=0; i<no_points; i++)
    {
        avg[0] += getDoubleTableAt(i,4);
        avg[1] += getDoubleTableAt(i,5);
        avg[2] += getDoubleTableAt(i,6);
    }
    avg[0] /= double(no_points);
    avg[1] /= double(no_points);
    avg[2] /= double(no_points);

    // Calculate standard deviation based on errors
    for (int i=0; i<no_points; i++)
    {
        stddev[0] += pow(getDoubleTableAt(i,4) - avg[0], 2);
        stddev[1] += pow(getDoubleTableAt(i,5) - avg[1], 2);
        stddev[2] += pow(getDoubleTableAt(i,6) - avg[2], 2);
    }
    stddev[0] = sqrt(stddev[0] / double(no_points));
    stddev[1] = sqrt(stddev[1] / double(no_points));
    stddev[2] = sqrt(stddev[2] / double(no_points));

    for (int i=0; i<3; i++)
    {
        setTableAt(no_points,4+i,avg[i]);
        setTableAt(no_points+1,4+i,stddev[i]);
    }
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
