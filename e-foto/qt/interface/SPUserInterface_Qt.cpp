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
#include "SPUserInterface_Qt.h"

#include "SPManager.h"
#include "StereoTools.h"
#include "StereoDisplay.h"
#include "TreeFeatures.h"
#include "LoadingScreen.h"
#include "ImageViewers.h"

#include <qdesktopwidget.h>
#include <qapplication.h>
#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qaction.h>
#include <qstring.h>

#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

SPUserInterface_Qt* SPUserInterface_Qt::spInst = NULL;

SPUserInterface_Qt* SPUserInterface_Qt::instance(SPManager* manager)
{
    if (spInst != NULL)
    {
        delete spInst;
        spInst = NULL;
    }
    //if (spInst == NULL)
    //{
    spInst = new SPUserInterface_Qt(manager);
    //}
    return spInst;
}

SPUserInterface_Qt::SPUserInterface_Qt(SPManager* manager, QWidget* parent, Qt::WindowFlags fl)
    : QMainWindow(parent, fl)
{
    setupUi(this);

    // Connections
    QObject::connect(comboBox_3, SIGNAL(currentIndexChanged(int)), this, SLOT(updateClass(int)));
    QObject::connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangePair(int)));
    QObject::connect(comboBox_5, SIGNAL(currentIndexChanged(int)), this, SLOT(setColorMaskLeft(int)));
    QObject::connect(comboBox_6, SIGNAL(currentIndexChanged(int)), this, SLOT(setColorMaskRight(int)));
    QObject::connect(comboBox_7, SIGNAL(currentIndexChanged(int)), this, SLOT(setReverseLensGlasses(int)));
    QObject::connect(comboBox_2, SIGNAL(currentIndexChanged(int)), this, SLOT(onStereoModeChanged(int)));

    QObject::connect(actionLoad, SIGNAL(triggered()), this, SLOT(onLoadButton()));
    QObject::connect(actionSave, SIGNAL(triggered()), this, SLOT(onSaveButton()));
    QObject::connect(actionExport, SIGNAL(triggered()), this, SLOT(onExportButton()));
    //QObject::connect(doneButton, SIGNAL(clicked()), this, SLOT(close()));

    QObject::connect(actionAdd, SIGNAL(triggered()), this, SLOT(onAddButton()));
    QObject::connect(actionRemove, SIGNAL(triggered()), this, SLOT(onRemoveButton()));
    QObject::connect(actionRemove_All, SIGNAL(triggered()), this, SLOT(onRemoveAllButton()));
    QObject::connect(actionEnd, SIGNAL(triggered()), this, SLOT(onCloseFeature()));

    QObject::connect(actionSelect, SIGNAL(triggered(bool)), this, SLOT(onSelPtButton()));
    QObject::connect(actionInsert, SIGNAL(triggered(bool)), this, SLOT(onAddPtButton()));
    QObject::connect(actionEdit, SIGNAL(triggered(bool)), this, SLOT(onEditPtButton()));
    QObject::connect(actionRemove_Point, SIGNAL(triggered()), this, SLOT(onRemovePtButton()));

    QObject::connect(treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(onFeatureListClicked(QModelIndex)));

    // Add color map
    // Anagliph colors: Red, Green, Blue, Cyan, Magenta, Yellow
    color_map[0][0] = 1; color_map[0][1] = 0; color_map[0][2] = 0;
    color_map[1][0] = 0; color_map[1][1] = 1; color_map[1][2] = 0;
    color_map[2][0] = 0; color_map[2][1] = 0; color_map[2][2] = 1;
    color_map[3][0] = 0; color_map[3][1] = 1; color_map[3][2] = 1;
    color_map[4][0] = 1; color_map[4][1] = 1; color_map[4][2] = 0;
    color_map[5][0] = 1; color_map[5][1] = 0; color_map[5][2] = 1;

    // Edit mode = None
    measure_mode = 0;

    this->manager = manager;
    setWindowState(this->windowState());

    showMaximized();

    updateClass(0);

    allow_close = true;

    tree = new TreeFeatures("");
    treeView->setModel(tree);

    lastDir = ".";

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
    viewer = new StereoViewer();
    viewer->blockOpen();
    viewer->blockSave();
    viewer->setFeatures(manager->getFeaturesLink());
    viewer->getMarker().setToOnlyEmitClickedMode();

    //viewerSeparated = new SeparatedStereoViewer();
    //viewerSeparated->blockOpen();
    //viewerSeparated->blockSave();
    //viewerSeparated->getLeftMarker().setToOnlyEmitClickedMode();
    //viewerSeparated->getRightMarker().setToOnlyEmitClickedMode();
    //viewerSeparated->getLeftDisplay()->setCurrentScene(viewer->getDisplay()->getCurrentScene()->getLeftScene());
    //viewerSeparated->getRightDisplay()->setCurrentScene(viewer->getDisplay()->getCurrentScene()->getRightScene());

    //QTabWidget* viewersTab = new QTabWidget();
    //viewersTab->addTab(viewer,"StereoViewer");
    //viewersTab->addTab(viewerSeparated,"SeparatedViewers");
    //setCentralWidget(viewersTab);
    setCentralWidget(viewer);

    connect(&viewer->getMarker(),SIGNAL(clicked(QPointF, QPointF)),this,SLOT(stereoClicked(QPointF,QPointF)));
    connect(&viewer->getMarker(),SIGNAL(mouseMoved(QPointF,QPointF)),this,SLOT(stereoMoved(QPointF,QPointF)));
    connect(viewer->getDisplay(),SIGNAL(resized(int,int)),this,SLOT(adjustFit(int,int)));

    /*
    viewer->addToolBar(Qt::LeftToolBarArea,featuresToolBar);
    viewer->addToolBar(Qt::LeftToolBarArea,pointsToolBar);
    viewer->addToolBar(Qt::LeftToolBarArea,filesToolBar);
    viewer->setCorner(Qt::BottomRightCorner,Qt::RightDockWidgetArea);
    viewer->addDockWidget(Qt::BottomDockWidgetArea, featureListDockWidget, Qt::Horizontal);
    viewer->addDockWidget(Qt::RightDockWidgetArea, generalOptionsDockWidget, Qt::Vertical);
    viewer->addDockWidget(Qt::RightDockWidgetArea, featureEditorDockWidget, Qt::Vertical);
    viewer->tabifyDockWidget(generalOptionsDockWidget, featureEditorDockWidget);
    statusBar()->setHidden(true);
    */

    viewer->addToolBar(Qt::RightToolBarArea,featuresToolBar);
    viewer->addToolBar(Qt::RightToolBarArea,pointsToolBar);
    viewer->addToolBar(Qt::RightToolBarArea,filesToolBar);
    actionAdd->setShortcut(QKeySequence(tr("Ctrl+A")));
    actionEdit->setShortcut(QKeySequence(tr("Ctrl+D")));
    actionSelect->setShortcut(QKeySequence(tr("Ctrl+S")));

    viewer->setCorner(Qt::BottomRightCorner,Qt::RightDockWidgetArea);
    viewer->addDockWidget(Qt::RightDockWidgetArea, featureListDockWidget, Qt::Vertical);
    viewer->addDockWidget(Qt::RightDockWidgetArea, generalOptionsDockWidget, Qt::Vertical);
    viewer->addDockWidget(Qt::RightDockWidgetArea, featureEditorDockWidget, Qt::Vertical);
    viewer->tabifyDockWidget(generalOptionsDockWidget, featureEditorDockWidget);
    statusBar()->setHidden(true);

    show();
    qApp->processEvents();
    changePair(1,2);
    featureEditorDockWidget->setFocus();
    viewer->getToolBar()->executeAction(viewer->getToolBar()->setMoveTool);
    QImage bg(32, 32, QImage::Format_RGBA8888);
    bg.fill(QColor(128,128,128));
    viewer->loadBackground(bg);
    viewer->getDisplay()->updateAll();

    LoadingScreen::instance().close();
    return true;
}

void SPUserInterface_Qt::updateData()
{
    updateTable();
    viewer->getDisplay()->updateAll();
    //viewerSeparated->update();
}

void SPUserInterface_Qt::updateTable()
{
    QString txt = QString::fromLocal8Bit(manager->getFeaturesList().c_str());
    TreeFeatures* newTree = new TreeFeatures(txt);
    treeView->setModel(newTree);
    delete tree;
    tree = newTree;

    int feat_id, pt_id;
    manager->getSelected(feat_id, pt_id);
    if (feat_id == -1) feat_id++;
    if (pt_id == -1) pt_id++;
    treeView->setCurrentIndex(tree->index(pt_id-1, 0, tree->index(feat_id-1, 0)));
    treeView->setExpanded(tree->index(feat_id-1, 0),true);
    treeView->setFocus();
}

void SPUserInterface_Qt::updateClass(int feat_type)
{
    comboBox_4->clear();

    // Point
    if (feat_type == 0)
    {
        comboBox_4->addItem("Point");
    }

    // Line
    if (feat_type == 1)
    {
        comboBox_4->addItem("Paved street");
        comboBox_4->addItem("Unpaved street");
        comboBox_4->addItem("Trail");
        comboBox_4->addItem("Railway");
        comboBox_4->addItem("River");
        comboBox_4->addItem("Bridge");
        comboBox_4->addItem("Undefined");
    }

    // Polygon
    if (feat_type == 2)
    {
        comboBox_4->addItem("House");
        comboBox_4->addItem("Building");
        comboBox_4->addItem("Industrial");
        comboBox_4->addItem("Club");
        comboBox_4->addItem("Station");
        comboBox_4->addItem("Wasteland");
        comboBox_4->addItem("Square");
        comboBox_4->addItem("Park");
        comboBox_4->addItem("Forest");
        comboBox_4->addItem("Lagoon");
        comboBox_4->addItem("Pool");
        comboBox_4->addItem("Undefined");
    }
}

/*
 *  Button clicks
 **/

void SPUserInterface_Qt::onStereoModeChanged(int option)
{
    auto display = viewer->getDisplay();
    if (comboBox_2->isEnabled() && !display->setStereoMode((bool)option))
    {
        // The default message has been preserved as a comment until it is
        // possible to complete the maintenance of this feature.
        // QMessageBox::critical(this,tr("Error"),tr("Your system does not "
        //                                        "support polarized stereo."));
        QMessageBox::critical(this,tr("Error"),tr("This feature is under "
                                                  "maintenance, check the "
                                                  "setStereoMode method of "
                                                  "GLDisplay.cpp"));
        comboBox_2->setEnabled(false);
        comboBox_2->setCurrentIndex(0);
    }
}

void SPUserInterface_Qt::onLoadButton()
{
    // File open dialog
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Features file"), lastDir, tr("Stereoplotter features file (*.spf);; All files (*.*)")) ;

    // if no file name written, return
    if (filename=="")
        return;

    // Save last dir
    int i=filename.lastIndexOf("/");
    lastDir = filename.left(i);

    // Ask if create new or append
    bool append = false;

    if (manager->getNumFeatures() > 0)
        append = QMessageBox::question(this, "Open features", "Clear or append featues ?","Clear","Append");

    // Load DEM
    bool sp_load_flag = manager->loadFeatures((char *)filename.toLocal8Bit().constData(), append);

    // Report error
    if (!sp_load_flag)
    {
        QMessageBox::critical(this,"Error","Invalid features file format.");
        return;
    }

    manager->updateProjections();
    updateData();
    refreshFeatureSelectedData();
}

void SPUserInterface_Qt::onSaveButton()
{
    // File open dialog
    //
    QFileDialog salvar(this, tr("Save features file"), lastDir, tr("Stereoplotter features file (*.spf);; All files (*.*)"));
    salvar.setAcceptMode(QFileDialog::AcceptSave);
    salvar.setDefaultSuffix("spf");
    if(salvar.exec())
    {
        QString filename = salvar.selectedFiles()[0];

        // if no file name written, return
        if (filename.isEmpty())
            return;

        // Save last dir
        int i=filename.lastIndexOf("/");
        lastDir = filename.left(i);

        // Save Features
        //manager->saveFeatures((char *)filename.toStdString().c_str());
        manager->saveFeatures((char *)filename.toLocal8Bit().constData());
    }
}

void SPUserInterface_Qt::onExportButton()
{
    // File open dialog
    QFileDialog salvar(this, tr("Export features"), lastDir, tr("Text file (*.txt);; Shape file (*.shp)"));
    salvar.setAcceptMode(QFileDialog::AcceptSave);
    //salvar.setDefaultSuffix("txt");
    if(salvar.exec())
    {
        QString filename = salvar.selectedFiles()[0];
        // if no file name written, return
        if (filename.isEmpty())
            return;

        QString mode = salvar.selectedNameFilter();

        // Save last dir
        int i=filename.lastIndexOf("/");
        lastDir = filename.left(i);

        // Export features
        if (mode.contains("Text file (*.txt)"))
        {
            //manager->exportFeatures((char *)filename.toStdString().c_str(), 0);
            manager->exportFeatures((char *)filename.toLocal8Bit().constData(), 0);
        }
        else
        {
            //manager->exportFeatures((char *)filename.toStdString().c_str(), 1);
            manager->exportFeatures((char *)filename.toLocal8Bit().constData(), 1);
        }
    }
}

void SPUserInterface_Qt::onAddButton()
{
    AddDialog addDialog(manager, this);
    addDialog.setFixedWidth(300);
    //addDialog.move(featuresToolBar->pos().x()-300,featuresToolBar->pos().y());
    addDialog.move(QCursor::pos().x()-300,QCursor::pos().y());
    addDialog.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint);

    if (addDialog.exec() == QDialog::Accepted)
    {
        // Now, the feature is added by the addDialog form in accept method.
        //manager->addFeature(nameEdit->text().toStdString(), comboBox_3->currentIndex()+1, (comboBox_4->currentIndex()+1)%comboBox_4->count());

        updateData();

        int fid, pid;
        manager->getSelected(fid, pid);

        treeView->setCurrentIndex(treeView->model()->index(fid-1,0));

        if(actionInsert->isChecked())
            onAddPtButton();
        else
            actionInsert->trigger();
        refreshFeatureSelectedData();
    }
}

void SPUserInterface_Qt::onRemoveButton()
{
    manager->removeFeature();
    updateData();
    refreshFeatureSelectedData();
}

void SPUserInterface_Qt::onRemoveAllButton()
{
    if (QMessageBox::warning(this,"Erase all?","Erase all features?",QMessageBox::Yes,QMessageBox::No) == QMessageBox::No)
        return;

    manager->removeAllFeatures();
    updateData();
    refreshFeatureSelectedData();
}

void SPUserInterface_Qt::onAddPtButton()
{
    viewer->getToolBar()->changeMode(1);
    //viewerSeparated->getToolBar()->changeMode(1);
    if (actionEdit->isChecked())
        actionEdit->setChecked(false);

    if (actionSelect->isChecked())
        actionSelect->setChecked(false);

    measure_mode = actionInsert->isChecked();
}

void SPUserInterface_Qt::onEditPtButton()
{
    viewer->getToolBar()->changeMode(1);
    //viewerSeparated->getToolBar()->changeMode(1);
    if (actionInsert->isChecked())
        actionInsert->setChecked(false);

    if (actionSelect->isChecked())
        actionSelect->setChecked(false);

    actionEdit->isChecked() ? measure_mode = 2 : measure_mode = 0;
}

void SPUserInterface_Qt::onSelPtButton()
{
    viewer->getToolBar()->changeMode(1);
    //viewerSeparated->getToolBar()->changeMode(1);
    if (actionInsert->isChecked())
        actionInsert->setChecked(false);

    if (actionEdit->isChecked())
        actionEdit->setChecked(false);

    actionSelect->isChecked() ? measure_mode = 3 : measure_mode = 0;
}

void SPUserInterface_Qt::onRemovePtButton()
{
    manager->removePoint();
    updateData();
    refreshFeatureSelectedData();
}

void SPUserInterface_Qt::onFeatureListClicked(QModelIndex index)
{
    QModelIndex p_index = index.parent();
    int feat_id, pt_id;

    if (p_index.row() < 0)
    {
        feat_id = index.row() + 1;
        pt_id = -1;
    }
    else
    {
        feat_id = p_index.row() + 1;
        pt_id = index.row() + 1;
    }

    manager->setSelected(feat_id, pt_id);

    refreshFeatureSelectedData();

    viewer->getDisplay()->updateAll();
}

void SPUserInterface_Qt::refreshFeatureSelectedData()
{
    // Get Feature data
    std::string fname;
    int sel_feat, fclass, ftype, no_points;
    double perimeter, area;

    manager->getSelectedFeatureData(sel_feat, fname, ftype, fclass, no_points, perimeter, area);

    // If no feature selected
    if (sel_feat < 0)
    {
        featureIdLabel->setText("-");
        noPointsLabel->setText("-");
        perimeterLabel->setText("-");
        areaLabel->setText("-");
        return;
    }

    // If has feature selected
    featureIdLabel->setText(QString::number(sel_feat));
    nameEdit->setText(QString::fromLocal8Bit(fname.c_str()));
    comboBox_3->setCurrentIndex(ftype-1);
    comboBox_4->setCurrentIndex(fclass-1>=0? fclass-1 : comboBox_4->count()-1);
    noPointsLabel->setText(QString::number(no_points));
    perimeterLabel->setText(QString::number(perimeter,'f',2) + " meters");
    areaLabel->setText(QString::number(area,'f',2) + " square meters");
}

void SPUserInterface_Qt::addImagePair(char *item)
{
    QString text = QString::fromLatin1(item);
    comboBox->addItem(text);
}

void SPUserInterface_Qt::onCloseFeature()
{
    manager->setSelected(-1,-1);
    treeView->clearSelection();
    updateData();
    refreshFeatureSelectedData();
    if(actionSelect->isChecked())
        onSelPtButton();
    else
        actionSelect->trigger();
}

void SPUserInterface_Qt::stereoClicked(QPointF lPos, QPointF rPos)
{
    if (measure_mode == 0)
        return;

    int fid, pid;
    manager->getSelected(fid, pid);

    if (measure_mode != 3)
    {
        if (fid < 1 || (pid < 1 && measure_mode != 1))
            return;
    }

    double lx = lPos.x(), ly = lPos.y(), rx = rPos.x(), ry = rPos.y(), X , Y, Z;

    manager->computeIntersection(lx, ly, rx, ry, X, Y, Z);

    //
    // Check mode
    //

    // Add pt
    if (measure_mode == 1)
    {
        pid++;
        if (pid < 1)
            pid = 1;
        manager->addPoint(fid, pid, lx, ly, rx, ry, X, Y, Z);
        updateData();
    }

    // Edit pt
    if (measure_mode == 2)
    {
        manager->updatePoint(fid, pid, lx, ly, rx, ry, X, Y, Z);
        updateData();
    }

    // Select feature
    if (measure_mode == 3)
    {
        manager->setSelectedXYZ(X,Y,Z);
        manager->getSelected(fid, pid);
        if (fid == -1) fid++;
        if (pid == -1) pid++;
        treeView->setCurrentIndex(tree->index(pid-1, 0, tree->index(fid-1, 0)));
        treeView->setExpanded(tree->index(fid-1, 0),true);
        //onFeatureSelected();
        treeView->setFocus();
    }

    refreshFeatureSelectedData();
}

void SPUserInterface_Qt::stereoMoved(QPointF lPos, QPointF rPos)
{
    double lx = lPos.x(), ly = lPos.y(), rx = rPos.x(), ry = rPos.y(), X , Y, Z;
    manager->computeIntersection(lx, ly, rx, ry, X, Y, Z);
    viewer->getToolBar()->actualizeStereoInfoLabel(X, Y, Z);
}

void SPUserInterface_Qt::centerImages(ObjectSpaceCoordinate coord, double zoom)
{
    ImageSpaceCoordinate centerLeft = manager->getLeftPoint(coord);
    ImageSpaceCoordinate centerRight = manager->getRightPoint(coord);
    QPointF atL(centerLeft.getPosition().get(1), centerLeft.getPosition().get(2));
    QPointF atR(centerRight.getPosition().get(1), centerRight.getPosition().get(2));
    viewer->getDisplay()->getCurrentScene()->getLeftScene()->scaleTo(zoom);
    viewer->getDisplay()->getCurrentScene()->getRightScene()->scaleTo(zoom);
    viewer->getDisplay()->getCurrentScene()->getLeftScene()->moveTo(atL);
    viewer->getDisplay()->getCurrentScene()->getRightScene()->moveTo(atR);

    //viewer->getMarker().addMark(atL, atR, 1, "center");

    viewer->update();

    //QMessageBox* msg = new QMessageBox(this);
    //msg->setText(QString::number(atL.x())+QString("x")+QString::number(atL.y())+QString(" ")+QString::number(atR.x())+QString("x")+QString::number(atR.y()));
    //msg->setText(QString::number(coord.getPosition().get(1))+QString("x")+QString::number(coord.getPosition().get(2))+QString("x")+QString::number(coord.getPosition().get(3)));
    //msg->show();
}

void SPUserInterface_Qt::changePair(int leftKey, int rightKey)
{
    viewer->loadLeftImage(QString::fromLocal8Bit(manager->getFullImagePath(leftKey).c_str()));
    viewer->loadRightImage(QString::fromLocal8Bit(manager->getFullImagePath(rightKey).c_str()));
    viewer->update();
}

void SPUserInterface_Qt::onChangePair(int pos)
{
    int lk, rk;
    manager->changePair(pos, lk, rk);
    changePair(lk, rk);
    centerImages(manager->getCentralPoint(), 1.0);
    viewer->getDisplay()->adjustFit(QPointF(manager->getLeftPoint(manager->getBoundingBoxCenter()).getPosition().get(1), manager->getLeftPoint(manager->getBoundingBoxCenter()).getPosition().get(2)),QPointF(manager->getRightPoint(manager->getBoundingBoxCenter()).getPosition().get(1),manager->getRightPoint(manager->getBoundingBoxCenter()).getPosition().get(2)),manager->getBoundingBoxIdealZoom(viewer->getDisplay()->width(),viewer->getDisplay()->height()));
    updateData();
}

void SPUserInterface_Qt::adjustFit(int width, int height)
{
    viewer->getDisplay()->adjustFit(QPointF(manager->getLeftPoint(manager->getBoundingBoxCenter()).getPosition().get(1), manager->getLeftPoint(manager->getBoundingBoxCenter()).getPosition().get(2)),QPointF(manager->getRightPoint(manager->getBoundingBoxCenter()).getPosition().get(1),manager->getRightPoint(manager->getBoundingBoxCenter()).getPosition().get(2)),manager->getBoundingBoxIdealZoom(width,height));
}

void SPUserInterface_Qt::showErrorMessage(QString msg)
{
    QMessageBox::critical(this, "Error",msg);
    close();
}

//
// Anagliph colors and reversion
//

void SPUserInterface_Qt::setReverseLensGlasses(int option)
{
    viewer->getDisplay()->setReverseLensGlasses((bool) option);
    updateData();
}

void SPUserInterface_Qt::setColorMaskLeft(int option)
{
    viewer->getDisplay()->setColorMaskLeft(color_map[option][0], color_map[option][1], color_map[option][2]);
    updateData();
}

void SPUserInterface_Qt::setColorMaskRight(int option)
{
    viewer->getDisplay()->setColorMaskRight(color_map[option][0], color_map[option][1], color_map[option][2]);
    updateData();
}


AddDialog::AddDialog(SPManager* spmanager, QWidget *parent) : QDialog(parent)
{
    manager = spmanager;

    nameLabel = new QLabel(tr("Name:"));
    typeLabel = new QLabel(tr("Feature type:"));
    classLabel = new QLabel(tr("Feature class:"));

    nameEdit = new QLineEdit;
    typeCombo = new QComboBox;
    classCombo = new QComboBox;

    nameLabel->setBuddy(nameEdit);
    typeLabel->setBuddy(typeCombo);
    classLabel->setBuddy(classCombo);

    cancelButton = new QPushButton(tr("Cancel"));
    okButton = new QPushButton(tr("Ok"));

    typeCombo->addItem("Point");
    typeCombo->addItem("Line");
    typeCombo->addItem("Polygon");
    classCombo->addItem("Point");
    typeCombo->setMinimumWidth(150);
    classCombo->setMinimumWidth(150);
    okButton->setDefault(true);
    okButton->setDisabled(true);

    QHBoxLayout *nameLayout = new QHBoxLayout;
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(nameEdit);

    QHBoxLayout *typeLayout = new QHBoxLayout;
    typeLayout->addWidget(typeLabel);
    typeLayout->addWidget(typeCombo);

    QHBoxLayout *classLayout = new QHBoxLayout;
    classLayout->addWidget(classLabel);
    classLayout->addWidget(classCombo);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(nameLayout);
    layout->addLayout(typeLayout);
    layout->addLayout(classLayout);
    layout->addLayout(buttonLayout);

    setLayout(layout);

    connect(nameEdit, SIGNAL(textChanged(QString)), this, SLOT(checkAcceptance(QString)));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(typeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateClassCombo(int)));

    nameEdit->setFocus();

    setWindowTitle(tr("Add feature"));
}

void AddDialog::updateClassCombo(int feat_type)
{
    classCombo->clear();

    // Point
    if (feat_type == 0)
    {
        classCombo->addItem("Point");
    }

    // Line
    if (feat_type == 1)
    {
        classCombo->addItem("Paved street");
        classCombo->addItem("Unpaved street");
        classCombo->addItem("Trail");
        classCombo->addItem("Railway");
        classCombo->addItem("River");
        classCombo->addItem("Bridge");
        classCombo->addItem("Undefined");
    }

    // Polygon
    if (feat_type == 2)
    {
        classCombo->addItem("House");
        classCombo->addItem("Building");
        classCombo->addItem("Industrial");
        classCombo->addItem("Club");
        classCombo->addItem("Station");
        classCombo->addItem("Wasteland");
        classCombo->addItem("Square");
        classCombo->addItem("Park");
        classCombo->addItem("Forest");
        classCombo->addItem("Lagoon");
        classCombo->addItem("Pool");
        classCombo->addItem("Undefined");
    }
}

void AddDialog::checkAcceptance(QString fname)
{
    okButton->setDisabled(fname.isEmpty());
}

void AddDialog::accept()
{
    //manager->addFeature(nameEdit->text().toStdString(), typeCombo->currentIndex()+1, (classCombo->currentIndex()+1)%classCombo->count());
    manager->addFeature(nameEdit->text().toLocal8Bit().constData(), typeCombo->currentIndex()+1, (classCombo->currentIndex()+1)%classCombo->count());
    done(QDialog::Accepted);
}

void AddDialog::reject()
{
    done(QDialog::Rejected);
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
