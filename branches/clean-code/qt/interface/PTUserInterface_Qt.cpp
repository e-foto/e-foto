/*Copyright 2002-2018 e-foto team (UERJ)
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
#include "PTUserInterface_Qt.h"

#include "PTManager.h"
#include "WindowsSelectPage.h"
#include "ConvertionsSystems.h"
#include "SingleTools.h"
#include "SingleDisplay.h"
#include "LoadingScreen.h"
#include "WindowsSelectPage.h"
#include "ImageViewers.h"
#include "FlightDirectionForm.h"

#include <qapplication.h>
#include <QtGui>
#include <QMessageBox>
#include <QCheckBox>

#include <iomanip>
#include <sstream>
#include <fstream>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

PTUserInterface_Qt* PTUserInterface_Qt::ptInst = NULL;

PTUserInterface_Qt* PTUserInterface_Qt::instance(PTManager *ptManager)
{
    if (ptInst != NULL)
    {
        delete ptInst;
        ptInst = NULL;
    }
    if (ptInst == NULL)
    {
        ptInst = new PTUserInterface_Qt(ptManager);
    }
    return ptInst;
}

PTUserInterface_Qt::PTUserInterface_Qt(PTManager *manager, QWidget *parent, Qt::WindowFlags fl)
    :QMainWindow(parent, fl)
{

    setupUi(this);
    ptManager = manager;
    saveMarksButton->setDisabled(true);

    viewer = new SeparatedStereoViewer(this); // Inserção do novo display
    viewer->blockOpen();
    viewer->blockSave();
    viewerLayout->addWidget(viewer);
    QToolBar* controlTool = new QToolBar("Control Tools");
    controlTool->addWidget(markToolButton);
    controlTool->addWidget(calculateFotoTriToolButton);
    controlTool->addWidget(saveMarksButton);
    controlTool->addWidget(viewReportToolButton);
    controlTool->addWidget(exportToKmlButton);
    controlTool->addWidget(reportButton);
    controlTool->addWidget(doneButton);
    addToolBar(Qt::LeftToolBarArea,controlTool);
    addToolBar(Qt::LeftToolBarArea,viewer->getToolBar());
    toolsDockWidget->setHidden(true);
    mark = new Marker(SymbolsResource::getTriangle(QColor(255,255,0,255), QColor(Qt::transparent),QSize(24,24), 2, true));
    selectedMark = new Marker(SymbolsResource::getTriangle(QColor(0,255,0,255), QColor(Qt::transparent),QSize(24,24), 2, true));
    photoMark = new Marker(SymbolsResource::getSquare(QColor(255,255,0,255), QColor(Qt::transparent),QSize(19,19), 2, true));
    photoSelectedMark = new Marker(SymbolsResource::getSquare(QColor(0,255,0,255), QColor(Qt::transparent),QSize(19,19), 2, true));

    // Escondendo a opção de ligar ou desligar o modo de vinculação de ponto a uma imagem.
    insertPointInButton->setHidden(true);
    insertionMode=true;

    viewer->getLeftMarker().setToOnlyEmitClickedMode(); // Pluges para que o novo display funcione
    viewer->getRightMarker().setToOnlyEmitClickedMode();
    connect(&viewer->getLeftMarker(),SIGNAL(clicked(QPointF)),this,SLOT(imageClicked(QPointF)));
    connect(&viewer->getRightMarker(),SIGNAL(clicked(QPointF)),this,SLOT(imageClicked(QPointF)));

    setWindowTitle("E-foto - Phototriangulation");

    connect(actionView_Report, SIGNAL(triggered()), this, SLOT(viewReport()));
    connect(actionCalculateFotoTri,SIGNAL(triggered()),this,SLOT(showSelectionWindow()));
    connect(calculateFotoTriToolButton,SIGNAL(clicked()),this,SLOT(showSelectionWindow()));
    connect(saveMarksButton,SIGNAL(clicked()),this,SLOT(saveMarks()));
    connect(markToolButton,SIGNAL(clicked()),this,SLOT(addPoint()));
    connect(insertPointInButton,SIGNAL(clicked(bool)),this,SLOT(toggleInsertPointMode(bool)));
    connect(viewReportToolButton,SIGNAL(clicked(bool)),this,SLOT(showReportXml()));
    connect(reportButton,SIGNAL(clicked(bool)),this,SLOT(onReportButtonClicked()));
    connect(doneButton,SIGNAL(clicked(bool)),this,SLOT(FTdone()));
    connect(exportToKmlButton,SIGNAL(clicked()),this,SLOT(exportToKml()));
    actionCalculateFotoTri->setEnabled(true);
    calculateFotoTriToolButton->setEnabled(true);
    flightDirectionToolButton->setVisible(false);

    QShortcut* undoShortcut = new QShortcut(QKeySequence(tr("Ctrl+Z", "Undo")),this);
    connect(undoShortcut, SIGNAL(activated()), this, SLOT(undoMark()));

    actionMove->setChecked(true);

    if( ptManager->hasEODone())
        viewReportToolButton->setEnabled(true);
    else
        viewReportToolButton->setEnabled(false);

    // Problems with epp reading leads to identify EOs done, but broken pointers.
    // After solve the problem, remove next line and similar on acceptResults()
    reportButton->setEnabled(false);
    exportToKmlButton->setEnabled(false);

    imagesPointTreeWidget->setColumnHidden(3,true);
    imagesPointTreeWidget->setColumnHidden(4,true);

    setWindowState(this->windowState() | Qt::WindowMaximized);

    qApp->processEvents();
    init();
}

PTUserInterface_Qt::~PTUserInterface_Qt()
{

}

void PTUserInterface_Qt::init()
{
    std::deque<std::string> images =ptManager->getStringImages();
    std::deque<std::string> points =ptManager->getStringIdPoints();
    for (size_t i=0;i<images.size();i++)
    {
        QString img=images.at(i).c_str();
        listAllImages << img;
        if (i!=1)
            listImageLeft << img;
        if (i!=0)
            listImageRight << img;
    }
    //Esse for esta causando crashs no programa... Verfificar e explicar o porquê.
    for (size_t i=0;i<points.size();i++)
        listAllPoints << QString(points.at(i).c_str());

    currentPointKey =-1;
    leftImageString = images.at(0);
    rightImageString= images.at(1);
    leftImageKey = ptManager->getImageId(leftImageString);
    rightImageKey = ptManager->getImageId(rightImageString);

    leftImageComboBox->addItems(listImageLeft);
    rightImageComboBox->addItems(listImageRight);

    //Esconde as colunas das keys
    leftImageTableWidget->setColumnHidden(3,true);
    pointsTableWidget->setColumnHidden(5,true);
    rightImageTableWidget->setColumnHidden(3,true);

    leftImageTableWidget->verticalHeader()->hide();
    rightImageTableWidget->verticalHeader()->hide();
    pointsTableWidget->verticalHeader()->hide();

    connect(leftImageComboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(updateImagesList(QString)));
    connect(rightImageComboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(updateImagesList(QString)));

    connect(leftImageTableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this, SLOT( tableClicked(QTableWidgetItem*) ));
    connect(rightImageTableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this, SLOT( tableClicked(QTableWidgetItem*) ));
    connect(pointsTableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this, SLOT( tableClicked(QTableWidgetItem*) ));
    connect(imagesPointTreeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this, SLOT( measureClicked() ));
    connect(removeButton,SIGNAL(clicked()),this, SLOT( removeMeasurement() ));
    connect(removeAllButton,SIGNAL(clicked()),this, SLOT( removeAllMeasurements() ));
    connect(autoMeasurementButton,SIGNAL(clicked()),this, SLOT( autoMeasureClicked()));

    // Se tiver sido calculado antes habilita o botão
    viewReportToolButton->setEnabled(ptManager->hasEODone());
}

void PTUserInterface_Qt::closeEvent(QCloseEvent *event)
{
    if (!ptManager->getMarksSavedState())
    {
        QMessageBox::StandardButton reply;
        reply=QMessageBox::question(this,tr("Warning: exiting with unsaved marks"), tr("Do you want to save changes?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (reply == QMessageBox ::Yes )
        {
            saveMarks();
        }
        else if (reply == QMessageBox::Cancel)
        {
            event->ignore();
            return;
        }

    }

    LoadingScreen::instance().show();
    qApp->processEvents();

    listAllImages.clear();
    listAllPoints.clear();
    listImageLeft.clear();
    listImageRight.clear();

    ptManager->returnProject();
    QMainWindow::closeEvent(event);
}

bool PTUserInterface_Qt::exec()
{
    bool ok;

    /** carregar imagem da esquerda*/
    viewer->loadLeftImage(QString::fromLocal8Bit(ptManager->getFilePath(leftImageString).c_str()));
    viewer->getLeftDisplay()->getCurrentScene()->scaleTo(1);

    /** carregar imagem da direita*/
    viewer->loadRightImage(QString::fromLocal8Bit(ptManager->getFilePath(rightImageString).c_str()));
    viewer->getRightDisplay()->getCurrentScene()->scaleTo(1);

    updateImageTable(leftImageTableWidget, leftImageString);
    updatePointsTable();
    updateImageTable(rightImageTableWidget, rightImageString);

    markAllpoints(viewer->getLeftDisplay());
    markAllpoints(viewer->getRightDisplay());

    if (pointsTableWidget->item(0,5) != 0)
        setCurrentPointKey(pointsTableWidget->item(0,5)->text().toInt(&ok));
    this->show();
    LoadingScreen::instance().close();
    qApp->processEvents();

    // FlightDirectionDisabled!
    for (int i = 0; i < listAllImages.size(); i++)
        setFlightDirection(listAllImages.at(i),0);

    // AutoMeasurementDisabled!
    autoMeasurementButton->setVisible(false);

    return true;
}

void PTUserInterface_Qt::viewReport()
{
    QWidget *resultView = new QWidget();
    resultView->setGeometry(resultView->x()+50,resultView->y()+50,1200,400);
    QHBoxLayout *horizontalLayout= new QHBoxLayout();

    QHBoxLayout *buttonsLayout= new QHBoxLayout();
    QPushButton *acceptButton= new QPushButton(tr("Accept"));
    QPushButton *discardButton= new QPushButton(tr("Discard"));
    QPushButton *exportTxtButton= new QPushButton(tr("Export coordinates"));
    buttonsLayout->addWidget(acceptButton);
    buttonsLayout->addWidget(discardButton);
    buttonsLayout->addWidget(exportTxtButton);

    QStringList oeHeaderLabels;
    oeHeaderLabels<< "Image Id"<< QString::fromUtf8("ω")<<QString::fromUtf8("φ")<<QString::fromUtf8("κ")<<"X0"<<"Y0"<<"Z0"<<QString::fromUtf8("δω") << QString::fromUtf8("δφ") << QString::fromUtf8("δκ") << QString::fromUtf8("δX0") << QString::fromUtf8("δY0") << QString::fromUtf8("δZ0");

    QString iter="Iterations: ";
    iter+=QString::number(ptManager->getBundleAdjustment()->getTotalIterations());

    QString rmse="RMSE: ";
    rmse+=QString::number(ptManager->getRMSE(),'f',4);

    QLabel *iterations = new QLabel(iter);
    QLabel *converged;
    QLabel *rmseLabel = new QLabel(rmse+" m");
    QLabel *unitsLabel = new QLabel(QString::fromUtf8("(ω,φ,κ) in degrees and (X0,Y0,Z0) in meters"));

    if (ptManager->getBundleAdjustment()->isConverged())
        converged = new QLabel(tr("Converged: yes"));
    else
    {
        converged = new QLabel(tr("Converged: no"));
        acceptButton->setDisabled(true);
    }

    QHBoxLayout *infoLayout= new QHBoxLayout();
    infoLayout->addWidget(iterations);
    infoLayout->addWidget(converged);
    infoLayout->addWidget(rmseLabel);
    infoLayout->addWidget(unitsLabel);

    /**///tabela das OEs
    QVBoxLayout *oeLayout= new QVBoxLayout();
    ETableWidget *oeTable=  new ETableWidget();
    QStringList imagesSelected;
    std::deque<std::string> images=selectionImagesView->getSelectedItens();
    for (unsigned int i=0;i<images.size();i++)
        imagesSelected << QString(images.at(i).c_str());
    oeTable->setColumnCount(13);

    oeTable->putInColumn(imagesSelected,0);
    oeTable->putIn(ptManager->getMatrixOE(),0,1,'f',4);
    oeTable->putIn(ptManager->getMVC(),0,7,'f',5);
    oeTable->setHorizontalHeaderLabels(oeHeaderLabels);
    oeTable->setSortingEnabled(true);

    oeLayout->addLayout(infoLayout);
    oeLayout->addWidget(oeTable);
    horizontalLayout->setStretchFactor(oeLayout,13);
    /**///tabela dos pontos fotogrametricos

    std::deque<std::string>  ids  = ptManager->getSelectedPointIdPhotogrammetric();
    if (ids.size()!=0)
    {
        QVBoxLayout *phtgLayout= new QVBoxLayout();
        QLabel *phtgLabel= new QLabel("<font size=5>Photogrammetric Points");
        phtgLabel->setTextFormat(Qt::RichText);
        phtgLabel->setAlignment(Qt::AlignHCenter);
        ETableWidget *photogrammetricTable = new ETableWidget();
        QStringList headerLabelsPoints,idsPhotogrammetric;//,leftImageIdPoints, rightImageIdPoints;
        headerLabelsPoints<<"Id"<<"E"<<"N"<<"H"<< QString::fromUtf8("δE") << QString::fromUtf8("δN") << QString::fromUtf8("δH");
        Matrix pointsPhotogrametricMatrix=ptManager->getPhotogrammetricENH();
        Matrix pointsResiduosPhotogrametricMatrix=ptManager->getResiduoPhotogrammetric();

        for (size_t i=0;i<ids.size();i++)
            idsPhotogrammetric << QString(ids.at(i).c_str());
        photogrammetricTable->setColumnCount(7);
        photogrammetricTable->putInColumn(idsPhotogrammetric,0);
        photogrammetricTable->putIn(pointsPhotogrametricMatrix,0,1,'f',4);
        photogrammetricTable->putIn(pointsResiduosPhotogrametricMatrix,0,4,'f',5);
        photogrammetricTable->setHorizontalHeaderLabels(headerLabelsPoints);
        photogrammetricTable->resizeTable();
        photogrammetricTable->setSortingEnabled(true);
        phtgLayout->addWidget(phtgLabel);
        phtgLayout->addWidget(photogrammetricTable);
        oeLayout->addLayout(phtgLayout);
    }
    /**/

    QVBoxLayout *reportLayout= new QVBoxLayout;
    reportLayout->addLayout(oeLayout);
    reportLayout->addLayout(buttonsLayout);

    resultView->setLayout(reportLayout);

    connect(acceptButton,SIGNAL(clicked()),this,SLOT(acceptResults()));
    connect(acceptButton,SIGNAL(clicked()),resultView,SLOT(close()));
    connect(discardButton, SIGNAL(clicked()),resultView,SLOT(close()));
    connect(exportTxtButton,SIGNAL(clicked()),this,SLOT(exportCoordinatesTxt()));
    resultView->setWindowModality(Qt::ApplicationModal);

    exportTxtButton->setVisible(false);

    resultView->show();
}

void PTUserInterface_Qt::showReportXml()
{
    Matrix oesXml=ptManager->eoParametersFromXml();
    QDockWidget *dockResultView = new QDockWidget("EO Parameters",this);
    QWidget *resultView = new QWidget();

    dockResultView->setWidget(resultView);
    QHBoxLayout *horizontalLayout= new QHBoxLayout();
    QStringList oeHeaderLabels;
    oeHeaderLabels<< "Image Id"<< QString::fromUtf8("ω")<<QString::fromUtf8("φ")<<QString::fromUtf8("κ")<<"X0"<<"Y0"<<"Z0";

    QString iter="Iterations: ";
    iter+=QString::number(ptManager->getPreviousTotalIterationsXml());

    QLabel *iterations = new QLabel(iter);
    QLabel *converged;
    if (ptManager->getPreviousConvergedXML())
        converged = new QLabel(tr("Converged: yes"));
    else
        converged = new QLabel(tr("Converged: no"));

    QString rmse="RMSE: ";
    rmse+=QString::number(ptManager->getPreviousRmseXML());
    QLabel *rmseLabel = new QLabel(rmse +" m");
    QLabel *unitsLabel = new QLabel(QString::fromUtf8("(ω,φ,κ) in degrees and (X0,Y0,Z0) in meters"));

    QHBoxLayout *infoLayout= new QHBoxLayout();
    infoLayout->addWidget(iterations);
    infoLayout->addWidget(converged);
    infoLayout->addWidget(rmseLabel);
    infoLayout->addWidget(unitsLabel);

    QStringList imagesSelected;
    for (size_t i=0; i<oesXml.getRows();i++)
    {
        imagesSelected << QString::fromLocal8Bit(ptManager->getImagefile(oesXml.getInt(i+1,1)).c_str());
    }
    oesXml=oesXml.sel(1,oesXml.getRows(),2,7);
    ETableWidget *table= new ETableWidget();
    table->setColumnCount(7);
    table->putInColumn(imagesSelected,0);
    table->putIn(oesXml,0,1,'f',4);
    table->setHorizontalHeaderLabels(oeHeaderLabels);
    table->resizeTable();
    table->setSortingEnabled(true);
    horizontalLayout->addWidget(table);
    horizontalLayout->setStretchFactor(table,7);

    QVBoxLayout *reportLayout= new QVBoxLayout;
    reportLayout->addLayout(infoLayout);
    reportLayout->addLayout(horizontalLayout);

    resultView->setLayout(reportLayout);

    dockResultView->setFloating(true);
    dockResultView->show();
    resultView->show();
}

bool PTUserInterface_Qt::calculatePT()
{

    ptManager->selectImages(selectionImagesView->getSelectedItens());
    ptManager->selectPoints(selectionPointsView->getSelectedItens());

    int answer = QMessageBox::question(this,tr("Calculating PhotoTriangulation"),tr("Do you want to calculate in Local Topocentric Mode?\nThis mode converts all control points coordinates in local topocentric coordinates then calculate photo triangulation.\n\n\nThis may take awhile.\nPlease wait until window with results appears"),QMessageBox::Yes, QMessageBox::No);
    if (answer== QMessageBox::Yes)
        ptManager->setLocalTopocentricMode(true);
    else
        ptManager->setLocalTopocentricMode(false);


    bool result = ptManager->calculatePT();
    if (result)
    {
        viewReport();
        actionView_Report->setEnabled(true);
        selectionView->close();
    }
    if (!ptManager->getBundleAdjustment()->isPossibleCalculate())
    {
        QMessageBox::information(this,tr("Impossible Calculate PhotoTriangulation"),tr("There's no sufficient points to calculate Phototriangulation,\ntry put more Control Points or Photogrammetric(Tie) Points "));
    }

    return result;
}

void PTUserInterface_Qt::showSelectionWindow()
{
    selectionView= new QWidget();

    std::deque<std::string> idsOut = ptManager->getPointsWithLesserThanOverlap(2);
    if (idsOut.size()>0)
    {
        QString ids="";
        for (size_t i=0;i<idsOut.size() ; i++)
        {
            ids+=QString::fromLocal8Bit(idsOut.at(i).c_str());
            ids+=", ";
        }
        ids.chop(2);
        ids+=".";
        QMessageBox::warning(selectionView,"Point taked out of evaluation",QString("Points with less than 2 overlaps:\n%1").arg(ids));
    }


         selectionImagesView= new WindowsSelectPage(tr("Images available"),tr("Images selected"));
        selectionPointsView= new WindowsSelectPage(tr("Points available"),tr("Points selected"));


    QStringList listImages;
    std::deque<std::string> lista;
    lista=ptManager->getStringImages();
    int sizeList=lista.size();
    for (int i=0;i<sizeList;i++)
        listImages << QString(lista.at(i).c_str());
    selectionImagesView->setInitialList(listImages);
    selectionImagesView->addAllItems();

    QStringList listPoints;
    lista=ptManager->getStringIdPoints("","noCheckingPoint");
    sizeList=lista.size();
    for (int i=0;i<sizeList;i++)
        listPoints << QString(lista.at(i).c_str());
    selectionPointsView->setInitialList(listPoints);
    selectionPointsView->addAllItems();

    QHBoxLayout *buttonsLayout= new QHBoxLayout();
    QPushButton *runButton= new QPushButton(tr("Run"));
    QPushButton *cancelButton= new QPushButton(tr("Cancel"));

    buttonsLayout->addWidget(runButton);
    buttonsLayout->addWidget(cancelButton);
    QLabel *iterationsLabel= new QLabel("Maximun iterations");
    QSpinBox *iterationsBox = new QSpinBox();
    iterationsBox->setValue(ptManager->getMaxIteration());
    iterationsBox->setRange(1,99);
    iterationsBox->setAccelerated(true);
    iterationsBox->setAlignment(Qt::AlignCenter);

    QLabel *metricConvergencyLabel= new QLabel("Metric Convergency");
    QDoubleSpinBox *metricConvergencyBox= new QDoubleSpinBox();
    metricConvergencyBox->setDecimals(5);
    metricConvergencyBox->setRange(0.00001,1.0);
    metricConvergencyBox->setSingleStep(0.00001);
    metricConvergencyBox->setValue(ptManager->getMetricConvergencyValue());
    metricConvergencyBox->setAccelerated(true);
    metricConvergencyBox->setAlignment(Qt::AlignCenter);

    QLabel *angularConvergencyLabel= new QLabel("Angular Convergency");
    QDoubleSpinBox *angularConvergencyBox= new QDoubleSpinBox();
    angularConvergencyBox->setDecimals(5);
    angularConvergencyBox->setRange(0.00001,1.0);
    angularConvergencyBox->setSingleStep(0.00001);
    angularConvergencyBox->setValue(ptManager->getAngularConvergencyValue());
    angularConvergencyBox->setAccelerated(true);
    angularConvergencyBox->setAlignment(Qt::AlignCenter);

    QHBoxLayout *parametersLayout= new QHBoxLayout();
    parametersLayout->addStretch();
    parametersLayout->addWidget(iterationsLabel);
    parametersLayout->addWidget(iterationsBox);
    parametersLayout->addStretch();
    parametersLayout->addWidget(metricConvergencyLabel);
    parametersLayout->addWidget(metricConvergencyBox);
    parametersLayout->addWidget(angularConvergencyLabel);
    parametersLayout->addWidget(angularConvergencyBox);
    parametersLayout->addStretch();

    connect(iterationsBox,SIGNAL(valueChanged(int)),this,SLOT(setMaxIteration(int)));
    connect(metricConvergencyBox,SIGNAL(valueChanged(double)),this,SLOT(setMetricConvergencyValue(double)));
    connect(angularConvergencyBox,SIGNAL(valueChanged(double)),this,SLOT(setAngularConvergencyValue(double)));

    QVBoxLayout *layout= new QVBoxLayout(this);
    layout->addWidget(selectionImagesView);
    layout->addWidget(selectionPointsView);
    layout->addLayout(parametersLayout);
    layout->addLayout(buttonsLayout);

    selectionView->setLayout(layout);

    connect(runButton,SIGNAL(clicked()),this,SLOT(calculatePT()));
    connect(cancelButton,SIGNAL(clicked()),selectionView,SLOT(close()));

    selectionView->setWindowModality(Qt::ApplicationModal);
    selectionView->show();
}

// Atualiza a tabela das imagens e garante que a mesma imagem não apareça nas duas comboBoxes
void PTUserInterface_Qt::updateImagesList(QString imageFilename)
{
    int index=0;
    for(int i=0;i<listAllImages.size();i++)
        if(listAllImages.at(i)==imageFilename)
            index=i;
    QStringList temp=listAllImages;
    temp.removeAt(index);

    if(sender()==leftImageComboBox)
    {
        leftImageString=imageFilename.toLocal8Bit().constData();
        leftImageKey = ptManager->getImageId(leftImageString);
        QString currentRightImage=rightImageComboBox->currentText();
        rightImageComboBox->blockSignals(true);
        rightImageComboBox->clear();
        listImageRight=temp;
        rightImageComboBox->addItems(listImageRight);
        rightImageComboBox->setCurrentIndex(rightImageComboBox->findText(currentRightImage,Qt::MatchExactly));
        rightImageComboBox->blockSignals(false);
        viewer->loadLeftImage(QString::fromLocal8Bit(ptManager->getFilePath(leftImageString).c_str()));
        updateImageTable(leftImageTableWidget,leftImageString);
        clearAllMarks(viewer->getLeftDisplay()); markAllpoints(viewer->getLeftDisplay());
        viewer->getLeftDisplay()->updateAll();
    }
    else if(sender()==rightImageComboBox)
    {
        rightImageString = imageFilename.toLocal8Bit().constData();
        rightImageKey = ptManager->getImageId(rightImageString);
        QString currentLefttImage=leftImageComboBox->currentText();
        leftImageComboBox->blockSignals(true);
        leftImageComboBox->clear();
        listImageLeft=temp;
        leftImageComboBox->addItems(listImageLeft);
        leftImageComboBox->setCurrentIndex(leftImageComboBox->findText(currentLefttImage,Qt::MatchExactly));
        leftImageComboBox->blockSignals(false);
        viewer->loadRightImage(QString::fromLocal8Bit(ptManager->getFilePath(rightImageString).c_str()));
        updateImageTable(rightImageTableWidget,rightImageString);
        clearAllMarks(viewer->getRightDisplay()); markAllpoints(viewer->getRightDisplay());
        viewer->getRightDisplay()->updateAll();
    }
}

//Atualiza a tabela de imagens
void PTUserInterface_Qt::updateImageTable(ETableWidget *imageTable, std::string imageFilename, bool move)
{
    bool ok;
    QStringList idImagesPoints, keysImagePoints;
    std::deque<std::string> imagesPoints = ptManager->getStringIdPoints(imageFilename);
    std::deque<std::string> keysPoints = ptManager->getStringKeysPoints(imageFilename);
    for (size_t i=0;i<imagesPoints.size();i++)
    {
        idImagesPoints << QString(imagesPoints.at(i).c_str());
        keysImagePoints << QString(keysPoints.at(i).c_str());
    }
    Matrix imageColLin= ptManager->getColLin(imageFilename);
    imageTable->setSortingEnabled(false);
    imageTable->clearContents();
    imageTable->setRowCount(0);
    imageTable->putInColumn(idImagesPoints,0);
    imageTable->putIn(imageColLin,0,1,'f',3);
    imageTable->putInColumn(keysImagePoints,3);

    replaceTo__(imageTable);
    int pos=findKeyAppearances(imageTable,currentPointKey);
    if (pos >=0 && imageTable->item(pos,1)->text()!="--" && imageTable->item(pos,2)->text()!="--")
    {
        imageTable->selectRow(pos);

        double col =imageTable->item(pos,1)->text().toDouble(&ok);
        double lin =imageTable->item(pos,2)->text().toDouble(&ok);
        QPointF pixel(col,lin);

        if (imageTable==leftImageTableWidget)
        {
            if (move)
            {
                SingleScene* scene = dynamic_cast<SingleScene*>(viewer->getLeftDisplay()->getCurrentScene());
                scene->moveTo(pixel);
                scene->setDetailedPoint(pixel);
            }
            viewer->getLeftDisplay()->update();
        }
        else
        {
            if (move)
            {
                SingleScene* scene = dynamic_cast<SingleScene*>(viewer->getRightDisplay()->getCurrentScene());
                scene->moveTo(pixel);
                scene->setDetailedPoint(pixel);
            }
            viewer->getRightDisplay()->update();
        }
    }
    imageTable->setSortingEnabled(true);

}

void PTUserInterface_Qt::updatePointsTable()
{
    QStringList headerLabelsPoints,idPoints,typePoints,keysPoints;
    std::deque<std::string>  ids  = ptManager->getStringIdPoints();
    std::deque<std::string> types = ptManager->getStringTypePoints();
    std::deque<std::string> keys  = ptManager->getStringKeysPoints();
    headerLabelsPoints<<"Id"<<"Type"<<"E"<<"N"<<"H";

    Matrix pointsMatrix=ptManager->getENH();
    for (size_t i=0;i<ids.size();i++)
    {
        idPoints << QString(ids.at(i).c_str());
        typePoints << QString(types.at(i).c_str());
        keysPoints << QString(keys.at(i).c_str());
    }


    //pointsMatrix.show('f',4,"MatrixENH");
    pointsTableWidget->putInColumn(idPoints,0);
    pointsTableWidget->putInColumn(typePoints,1);
    pointsTableWidget->putIn(pointsMatrix,0,2,'f',3);
    pointsTableWidget->putInColumn(keysPoints,5);
    pointsTableWidget->setHorizontalHeaderLabels(headerLabelsPoints);
    //pointsTableWidget->resizeTable();
}

void PTUserInterface_Qt::selectAllAppearances(int pointKey)
{
    int indexLeftSearched,indexRightSearched, indexPointsSearched;

    indexLeftSearched = findKeyAppearances(leftImageTableWidget,pointKey);
    indexPointsSearched = findKeyAppearances(pointsTableWidget, pointKey);
    indexRightSearched = findKeyAppearances(rightImageTableWidget, pointKey);

    leftImageTableWidget->clearSelection();
    pointsTableWidget->clearSelection();
    rightImageTableWidget->clearSelection();

    if (indexLeftSearched>=0)
    {
        leftImageTableWidget->selectRow(indexLeftSearched);
        leftImageTableWidget->setRangeSelected(QTableWidgetSelectionRange(indexLeftSearched,3,indexLeftSearched,3),false);
    }
    if (indexPointsSearched>=0)
    {
        pointsTableWidget->selectRow(indexPointsSearched);
        pointsTableWidget->setRangeSelected(QTableWidgetSelectionRange(indexPointsSearched,5,indexPointsSearched,5),false);
    }
    if (indexRightSearched>=0)
    {
        rightImageTableWidget->selectRow(indexRightSearched);
        rightImageTableWidget->setRangeSelected(QTableWidgetSelectionRange(indexRightSearched,3,indexRightSearched,3),false);
    }

}

// Se encontrar retorna o indice na tabela da key procurada senão retorna -1
int PTUserInterface_Qt::findKeyAppearances(ETableWidget *table, int searchedPointKey)
{
    bool ok;
    int rows,key;
    if (table == leftImageTableWidget)
    {
        rows=leftImageTableWidget->rowCount();
        for (int i=0;i<rows;i++)
        {
            key=leftImageTableWidget->item(i,3)->text().toInt(&ok);
            if(key==searchedPointKey)
                return i;
        }
        return -1;
    }else if (table == pointsTableWidget)
    {
        rows=pointsTableWidget->rowCount();
        for (int i=0;i<rows;i++)
        {
            key=pointsTableWidget->item(i,5)->text().toInt(&ok);
            if(key==searchedPointKey)
                return i;
        }
        return -1;
    }else if (table == rightImageTableWidget)
    {
        rows=rightImageTableWidget->rowCount();
        for (int i=0;i<rows;i++)
        {
            key=rightImageTableWidget->item(i,3)->text().toInt(&ok);
            if(key==searchedPointKey)
                return i;
        }
        return -1;
    }
    return -1;
}

void PTUserInterface_Qt::updateMark(SingleDisplay *display, int imageKey, int pointKey, QPointF pixel)
{
    bool ok;
    double col=pixel.x();
    double lin=pixel.y();

    if (col<0 || lin<0)
        return;

    if (display == viewer->getLeftDisplay())
    {
        int pos=findKeyAppearances(leftImageTableWidget, pointKey);
        SingleScene* scene = dynamic_cast<SingleScene*>(viewer->getLeftDisplay()->getCurrentScene());
        if(pos<0 || col>scene->imageSize().width() || lin>scene->imageSize().height())
            return;
        saveMarksButton->setEnabled(true);
        ptManager->setMarksSavedState(false);
        double oldCol=leftImageTableWidget->item(pos,1)->text().toDouble(&ok);
        double oldLin=leftImageTableWidget->item(pos,2)->text().toDouble(&ok);
        putInStack(oldCol,oldLin,pointKey,imageKey,leftImageTableWidget->item(pos,0)->text());
        leftImageTableWidget->item(pos,1)->setText(QString::number(col, 'f', 2));
        leftImageTableWidget->item(pos,2)->setText(QString::number(lin, 'f', 2));
        QString pointId=leftImageTableWidget->item(pos,0)->text();
        //scene->geometry()->removePoint(pointId); //scene->geometry()->addPoint(pixel,leftImageTableWidget->item(pos,0)->text(),&mark);
        //qDebug("A %d",pointKey);
        Marker* pointMark;
        int pointsTableIndex = findKeyAppearances(pointsTableWidget,pointKey);
        if (pointsTableWidget->item(pointsTableIndex,1)->text() == "Control")
            pointMark = selectedMark;
        else
            pointMark = photoSelectedMark;
        clearAllMarks(viewer->getLeftDisplay());
        markAllpoints(viewer->getLeftDisplay());
        viewer->getLeftMarker().insertMark(pixel, pointKey, leftImageTableWidget->item(pos,0)->text(),pointMark);
        viewer->getLeftDisplay()->update();

        /*
        if(leftImageTableWidget->getItemSpinBox()!=NULL)
        {
            int colTable=leftImageTableWidget->getCurrentSpinBoxColumn();
            if (colTable==1)
                leftImageTableWidget->getItemSpinBox()->setValue(col);
            else if (colTable==2)
                leftImageTableWidget->getItemSpinBox()->setValue(lin);
        }
                */
    }else if(display == viewer->getRightDisplay())
    {
        int pos=findKeyAppearances(rightImageTableWidget, pointKey);
        SingleScene* scene = dynamic_cast<SingleScene*>(viewer->getRightDisplay()->getCurrentScene());
        if(pos<0 || col>scene->imageSize().width() || lin>scene->imageSize().height())
            return;
        saveMarksButton->setEnabled(true);
        ptManager->setMarksSavedState(false);
        double oldCol=rightImageTableWidget->item(pos,1)->text().toDouble(&ok);
        double oldLin=rightImageTableWidget->item(pos,2)->text().toDouble(&ok);
        putInStack(oldCol,oldLin,pointKey,imageKey,rightImageTableWidget->item(pos,0)->text());
        rightImageTableWidget->item(pos,1)->setText(QString::number(col, 'f', 2));
        rightImageTableWidget->item(pos,2)->setText(QString::number(lin, 'f', 2));
        QString pointId=rightImageTableWidget->item(pos,0)->text();
        //scene->geometry()->removePoint(pointId); //scene->geometry()->addPoint(pixel,rightImageTableWidget->item(pos,0)->text(),&mark);
        //qDebug("B %d",pointKey);
        Marker* pointMark;
        int pointsTableIndex = findKeyAppearances(pointsTableWidget,pointKey);
        if (pointsTableWidget->item(pointsTableIndex,1)->text() == "Control")
            pointMark = selectedMark;
        else
            pointMark = photoSelectedMark;
        clearAllMarks(viewer->getRightDisplay());
        markAllpoints(viewer->getRightDisplay());
        viewer->getRightMarker().insertMark(pixel, pointKey,rightImageTableWidget->item(pos,0)->text(),pointMark);
        viewer->getRightDisplay()->update();

        /*
        if(rightImageTableWidget->getItemSpinBox()!=NULL)
        {
            int colTable=rightImageTableWidget->getCurrentSpinBoxColumn();
            if (colTable==1)
                rightImageTableWidget->getItemSpinBox()->setValue(col);
            else if (colTable==2)
                rightImageTableWidget->getItemSpinBox()->setValue(lin);
        }
                */
    }
    ptManager->updateDigitalCoordinatesPoint(imageKey,pointKey,col,lin );
}

/* Method into disuse:
 *
void PTUserInterface_Qt::updateCoordinatesInfo(QPointF *pixel)
{
    if (pixel==NULL)
        return;
    //int col = pixel->x();
    //int lin = pixel->y();
}
*/

void PTUserInterface_Qt::imageClicked(QPointF pixel)
{
    if (sender() == &viewer->getLeftMarker())
    {
        if (insertionMode)
        {
            if(findKeyAppearances(leftImageTableWidget,currentPointKey)==-1)// Verificar se point nao esta na tabela da direita pelas pointkey
            {
                ptManager->connectPointInImage(currentPointKey,leftImageKey);
                updateImageTable(leftImageTableWidget,leftImageString,false);
                //conecte o ponto corrente na imagem da direita
            }
        }
        updateMark(viewer->getLeftDisplay(),ptManager->getImageId(leftImageString),currentPointKey,pixel);
        //leftView->moveTo(*pixel);
    }
    else if (sender() == &viewer->getRightMarker())
    {
        if (insertionMode)
        {

            if(findKeyAppearances(rightImageTableWidget,currentPointKey)==-1)// Verificar se point nao esta na tabela da esquerda pelas pointkey
            {
                //conecte o ponto corrente na imagem da esquerda
                ptManager->connectPointInImage(currentPointKey,rightImageKey);
                updateImageTable(rightImageTableWidget,rightImageString,false);
            }
        }
        updateMark(viewer->getRightDisplay(),ptManager->getImageId(rightImageString),currentPointKey,pixel);
        //rightView->moveTo(*pixel);
        //previsionMark(currentPointKey,pixel);
    }
    showImagesAppearances(currentPointKey);
}


/* Deixa o usuario entrar com o valor da linha e coluna na mao atualizando na interface. Atualmente abilitada
   */
/*
void PTUserInterface_Qt::updatePoint(int tableRow,int tableCol, double value)
{
    bool ok;
    double col=-1;
    double lin=-1;
    //int rowPoint=item->row();
    int pointKey;
    QTableWidgetItem *item;

    if (sender()==leftImageTableWidget)
    {
        int imageKey=ptManager->getImageId(leftImageString);
        item=leftImageTableWidget->item(tableRow,tableCol);
        pointKey=leftImageTableWidget->item(tableRow,3)->text().toInt(&ok);
        // Se os valores forem iguais então não houve alteração e não há o que ser alterado
        //if (leftImageTableWidget->getPreviousValue()==value)
        //              return;
        SingleScene* scene = (SingleScene*)viewer->getLeftDisplay()->getCurrentScene();
        if(tableCol==1)
        {
            if (value > scene->imageSize().width())
            {
                value=leftImageTableWidget->getPreviousValue();
                leftImageTableWidget->item(tableRow,tableCol)->setText(QString::number(value));
            }
            col=value;
            lin=leftImageTableWidget->item(tableRow,2)->text().toDouble(&ok);
            //leftImageTableWidget->avaliateType(tableRow,2);
        }
        else if(tableCol==2)
        {
            if (value > scene->imageSize().height())
            {
                value=leftImageTableWidget->getPreviousValue();
                leftImageTableWidget->item(tableRow,tableCol)->setText(QString::number(value));
            }
            lin=value;//item->text().toDouble(&ok);
            col=leftImageTableWidget->item(tableRow,1)->text().toDouble(&ok);
            //leftImageTableWidget->avaliateType(tableRow+1 == leftImageTableWidget->rowCount() ? tableRow:tableRow+1,1);
        }
        //qDebug("updateLeftimage %dx%d",col,lin);
        updateMark(viewer->getLeftDisplay(),imageKey,pointKey,QPointF(col,lin));
        viewer->getLeftDisplay()->getCurrentScene()->moveTo(QPointF(col,lin));
        viewer->getLeftDisplay()->update();
        //ptManager->updateDigitalCoordinatesPoint(imageKey,pointKey,col,lin);
        //qDebug("col %d lin %d",col,lin);
    } else if (sender()==rightImageTableWidget)
    {
        int imageKey=ptManager->getImageId(rightImageString);
        item=rightImageTableWidget->item(tableRow,tableCol);
        pointKey=rightImageTableWidget->item(tableRow,3)->text().toInt(&ok);
        // Se os valores forem iguais então não houve alteração e não há o que ser alterado
        //if (rightImageTableWidget->getPreviousValue()==value)
        //                return;
        SingleScene* scene = (SingleScene*)viewer->getRightDisplay()->getCurrentScene();
        if(tableCol==1)
        {
            if (value > scene->imageSize().width())
            {
                value=rightImageTableWidget->getPreviousValue();
                rightImageTableWidget->item(tableRow,tableCol)->setText(QString::number(value));
            }
            col=value;//item->text().toDouble(&ok);
            lin=rightImageTableWidget->item(tableRow,2)->text().toDouble(&ok);
            //rightImageTableWidget->avaliateType(tableRow,2);
        }
        else if(tableCol==2)
        {
            if (value > scene->imageSize().height())
            {
                value=rightImageTableWidget->getPreviousValue();
                rightImageTableWidget->item(tableRow,tableCol)->setText(QString::number(value));
            }
            lin=value;//item->text().toDouble(&ok);
            col=rightImageTableWidget->item(tableRow,1)->text().toDouble(&ok);
            //rightImageTableWidget->avaliateType( tableRow+1 == rightImageTableWidget->rowCount() ? tableRow:tableRow+1,1);
        }
        updateMark(viewer->getRightDisplay(),imageKey,pointKey,QPointF(col,lin));
        viewer->getRightDisplay()->getCurrentScene()->moveTo(QPointF(col,lin));
        viewer->getRightDisplay()->update();
        //ptManager->updateDigitalCoordinatesPoint(imageKey,pointKey,col,lin);
        //qDebug("col %d lin %d",col,lin);
    }

}
*/
void PTUserInterface_Qt::acceptResults()
{
    ptManager->saveResults();
    ptManager->setENH();
    updatePointsTable();
    if( ptManager->hasEODone())
        viewReportToolButton->setEnabled(true);
    else
        viewReportToolButton->setEnabled(false);

    // Remove next lines, after solving epp reading problem
    reportButton->setEnabled(true);
    exportToKmlButton->setEnabled(true);
}

void PTUserInterface_Qt::markAllpoints(SingleDisplay *display)
{
    double col,lin;
    bool ok;

    if (display == viewer->getLeftDisplay())
    {
        int pnts=leftImageTableWidget->rowCount();
        for(int i=0;i<pnts;i++)
        {
            col=leftImageTableWidget->item(i,1)->text().toDouble(&ok);
            lin=leftImageTableWidget->item(i,2)->text().toDouble(&ok);
            //colocar as marcas no display
            int pos=findKeyAppearances(leftImageTableWidget, leftImageTableWidget->item(i,3)->text().toInt(&ok));
            if (pos >=0 && (leftImageTableWidget->item(i,1)->text()!="--" && leftImageTableWidget->item(i,2)->text()!="--"))
            {
                //qDebug("left lin: %.3f  col: %.3f",lin,col);
                QPointF pixel(col,lin);
                //viewer->getLeftDisplay()->getCurrentScene()->geometry()->addPoint(pixel,leftImageTableWidget->item(pos,0)->text(),&mark);
                int pointkey = leftImageTableWidget->item(pos,3)->text().toInt();
                //qDebug("C %d", pointkey);
                Marker* pointMark;
                int pointsTableIndex = findKeyAppearances(pointsTableWidget,pointkey);
                if (pointsTableWidget->item(pointsTableIndex,1)->text() == "Control")
                    pointMark = mark;
                else
                    pointMark = photoMark;
                viewer->getLeftMarker().insertMark(pixel, pointkey,leftImageTableWidget->item(pos,0)->text(),pointMark);
                viewer->getLeftDisplay()->update();
            }
        }
    }
    else if (display == viewer->getRightDisplay())
    {
        int pnts=rightImageTableWidget->rowCount();
        for(int i=0;i<pnts;i++)
        {
            col=rightImageTableWidget->item(i,1)->text().toDouble(&ok);
            lin=rightImageTableWidget->item(i,2)->text().toDouble(&ok);
            //colocar as marcas no display
            int pos=findKeyAppearances(rightImageTableWidget, rightImageTableWidget->item(i,3)->text().toInt(&ok));
            if (pos >=0 && (rightImageTableWidget->item(i,1)->text()!="--" && rightImageTableWidget->item(i,2)->text()!="--"))
            {
                //qDebug("right lin: %.3f  col: %.3f",lin,col);
                QPointF pixel(col,lin);
                //viewer->getRightDisplay()->getCurrentScene()->geometry()->addPoint(pixel,rightImageTableWidget->item(pos,0)->text(),&mark);
                int pointkey = rightImageTableWidget->item(pos,3)->text().toInt();
                //qDebug("D %d",pointkey);
                Marker* pointMark;
                int pointsTableIndex = findKeyAppearances(pointsTableWidget,pointkey);
                if (pointsTableWidget->item(pointsTableIndex,1)->text() == "Control")
                    pointMark = mark;
                else
                    pointMark = photoMark;
                viewer->getRightMarker().insertMark(pixel, pointkey, rightImageTableWidget->item(pos,0)->text(),pointMark);
                viewer->getRightDisplay()->update();
            }
        }
    }
}

void PTUserInterface_Qt::clearAllMarks(SingleDisplay *display)
{
    display->getCurrentScene()->geometry()->clear();
    display->update();
}

void PTUserInterface_Qt::showImagesAppearances(int pointKey)
{
    imagesPointTreeWidget->clear();
    removeButton->setEnabled(false);
    removeAllButton->setEnabled(false);
    std::deque<std::string> appearances=ptManager->getImagesAppearances(pointKey);

    if (appearances.size()==0)
        return pointIdLabel->setText(QString("Point %1").arg(QString::fromLocal8Bit(ptManager->getPointId(pointKey).c_str())));

    QList<QTreeWidgetItem*> treelist;
    for(size_t i=0;i<appearances.size();i++)
    {
        QString imageKey= QString::number(ptManager->getImageId(appearances.at(i)));
        Matrix coord=ptManager->getDigitalCoordinate(ptManager->getImageId(appearances.at(i)),pointKey);
        QString linStr=QString::number(coord.get(1,1),'f',3);
        QString colStr=QString::number(coord.get(1,2),'f',3);
        if(coord.get(1,1)==-1 && coord.get(1,2)==-1)
        {
            linStr="--";
            colStr="--";
        }
        QStringList stringList;
        //stringList << QString::fromStdString(appearances.at(i)) << QString::number(coord.get(1,1))<< QString::number(coord.get(1,2)) << imageKey;
        stringList << QString::fromLocal8Bit(appearances.at(i).c_str()) << linStr<< colStr << imageKey << QString::number(pointKey);
        QTreeWidgetItem *item=new QTreeWidgetItem(stringList);
        item->setTextAlignment(0,Qt::AlignCenter);
        item->setTextAlignment(1,Qt::AlignCenter);
        item->setTextAlignment(2,Qt::AlignCenter);
        treelist.push_back(item);
    }

    imagesPointTreeWidget->addTopLevelItems(treelist);
    imagesPointTreeWidget->resizeColumnToContents(1);
    imagesPointTreeWidget->resizeColumnToContents(0);
    imagesPointTreeWidget->resizeColumnToContents(2);

    removeAllButton->setEnabled(true);
    pointIdLabel->setText(QString("Point %1").arg(QString::fromLocal8Bit(ptManager->getPointId(pointKey).c_str())));
}

void PTUserInterface_Qt::saveMarks()
{
    ptManager->saveMarks();
    ptManager->setMarksSavedState(true);
    saveMarksButton->setEnabled(false);
}

void PTUserInterface_Qt::setFlightDirection(QString imageFile, double kappa0)
{
    int imageKey = ptManager->getImageId(imageFile.toLocal8Bit().constData());
    ptManager->setImageFlightDirection(imageKey, kappa0);
}

// Created by Marcelo Teixiera Silveira
void PTUserInterface_Qt::onReportButtonClicked()
{
    if (!ptManager->hasEODone())
    {
        QMessageBox::warning(this,"Warning","Please, calculate photo-triangulation first.");
        return;
    }

    QFileDialog salvar(this,"Save file",".","*.txt");
    salvar.setAcceptMode(QFileDialog::AcceptSave);
    salvar.setDefaultSuffix("txt");
    if(salvar.exec())
    {
        QString fileExport = salvar.selectedFiles()[0];
        if (fileExport.isEmpty())
            return;

        if(!fileExport.endsWith(".txt"))
            fileExport.append(".txt");

        // This will run the complete report for PT. Still not working inside PT class due to conflicts with Project class.
        //    manager->createPhototriReport((char *)fileExport.toStdString().c_str());

        // Create report using ftManager (report inside PhotoTriangulation)
        saveFtReport((char *)fileExport.toLocal8Bit().constData());
    }
}

void PTUserInterface_Qt::exportCoordinatesTxt()
{
    QFileDialog salvar(this,"Save file",".","*.txt");
    salvar.setAcceptMode(QFileDialog::AcceptSave);
    salvar.setDefaultSuffix("txt");
    if(salvar.exec())
    {
        QString fileExport = salvar.selectedFiles()[0];
        if (fileExport.isEmpty())
            return;

        if(!fileExport.endsWith(".txt"))
            fileExport.append(".txt");

        QFile *exportTxt=new QFile(fileExport);
        exportTxt->setFileName(fileExport);
        exportTxt->open(QIODevice::WriteOnly);

        std::stringstream coordinates;

        coordinates << ptManager->getCoordinatesGeodesic();
        coordinates << ptManager->getCoordinatesTopocentric();

        exportTxt->write(coordinates.str().data());
        exportTxt->close();
    }
}

void PTUserInterface_Qt::exportToKml(bool fromXML)
{

    QFileDialog salvar(this,"Save File",QDir::currentPath(),"*.kml");
    salvar.setAcceptMode(QFileDialog::AcceptSave);
    salvar.setDefaultSuffix("kml");
    if(salvar.exec())
    {
        QString fileName = salvar.selectedFiles()[0];
        if (fileName.isEmpty())
            return;

        QFile *exported= new QFile(fileName);
        QFileInfo file(*exported);
        if (!file.fileName().endsWith(".kml"))
            exported->setFileName(fileName.append(".kml"));

        exported->open(QIODevice::WriteOnly);
        exported->write(ptManager->exportBlockTokml(exported->fileName().toLocal8Bit().constData(),fromXML).c_str());
        exported->close();
    }
}

void PTUserInterface_Qt::autoMeasureClicked()
{
    // pergunta se usuario confirma
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Warning!"), tr("This process can create measurements for not measured points in images with know parameters (GNSS and INS). Do you accept run this process?"), QMessageBox::Yes | QMessageBox::Cancel);
    if ( reply == QMessageBox::Yes )
    {
        ptManager->runAutoMeasurement();
        imagesPointTreeWidget->clear();
        updateImageTable(leftImageTableWidget, leftImageString, false);
        updateImageTable(rightImageTableWidget, rightImageString, false);
        clearAllMarks(viewer->getLeftDisplay());
        markAllpoints(viewer->getLeftDisplay());
        clearAllMarks(viewer->getRightDisplay());
        markAllpoints(viewer->getRightDisplay());
    }
}

void PTUserInterface_Qt::measureClicked()
{
    removeButton->setEnabled(true);
}

void PTUserInterface_Qt::removeMeasurement()
{
    int imgKey = imagesPointTreeWidget->currentItem()->data(3,0).toInt();
    int pntKey = imagesPointTreeWidget->currentItem()->data(4,0).toInt();
    ptManager->disconnectPointAndImage(pntKey, imgKey);
    showImagesAppearances(pntKey);
    updateImageTable(leftImageTableWidget, leftImageString);
    updateImageTable(rightImageTableWidget, rightImageString);
    clearAllMarks(viewer->getLeftDisplay());
    markAllpoints(viewer->getLeftDisplay());
    clearAllMarks(viewer->getRightDisplay());
    markAllpoints(viewer->getRightDisplay());
}

void PTUserInterface_Qt::removeAllMeasurements()
{
    // pergunta se usuario confirma
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Warning!"), tr("Do you want to remove all measurements for this point?"), QMessageBox::Yes | QMessageBox::Cancel);
    if ( reply == QMessageBox::Yes )
    {
        // faz loop de trabalho
        int pntKey = imagesPointTreeWidget->model()->data(imagesPointTreeWidget->model()->index(0,4),0).toInt();
        for (int i = 0; i < imagesPointTreeWidget->model()->rowCount(); i++)
        {
            int imgKey = imagesPointTreeWidget->model()->data(imagesPointTreeWidget->model()->index(i,3),0).toInt();
            ptManager->disconnectPointAndImage(pntKey, imgKey);
        }

        // atualiza interface
        showImagesAppearances(pntKey);
        updateImageTable(leftImageTableWidget, leftImageString);
        updateImageTable(rightImageTableWidget, rightImageString);
        clearAllMarks(viewer->getLeftDisplay());
        markAllpoints(viewer->getLeftDisplay());
        clearAllMarks(viewer->getRightDisplay());
        markAllpoints(viewer->getRightDisplay());
    }
}

void PTUserInterface_Qt::tableClicked(QTableWidgetItem* item)
{
    bool ok;
    ETableWidget * table = (ETableWidget*)item->tableWidget();
    int tableRow=item->row();

    double leftCol,leftLin,rightCol,rightLin;

    clearAllMarks(viewer->getLeftDisplay());
    markAllpoints(viewer->getLeftDisplay());
    clearAllMarks(viewer->getRightDisplay());
    markAllpoints(viewer->getRightDisplay());

    if (table==leftImageTableWidget)
    {
        setCurrentPointKey(leftImageTableWidget->item(tableRow,3)->text().toInt(&ok));
        Marker* pointMark;
        int pointsTableIndex = findKeyAppearances(pointsTableWidget,currentPointKey);
        if (pointsTableWidget->item(pointsTableIndex,1)->text() == "Control")
            pointMark = selectedMark;
        else
            pointMark = photoSelectedMark;
        leftCol=leftImageTableWidget->item(tableRow,1)->text().toDouble(&ok);
        leftLin=leftImageTableWidget->item(tableRow,2)->text().toDouble(&ok);
        if (leftImageTableWidget->item(tableRow,1)->text()!="--" && leftImageTableWidget->item(tableRow,2)->text()!="--")
        {
            SingleScene* scene = dynamic_cast<SingleScene*>(viewer->getLeftDisplay()->getCurrentScene());
            scene->moveTo(QPointF(leftCol,leftLin));
            scene->geometry()->updatePoint(QPointF(leftCol,leftLin),currentPointKey,leftImageTableWidget->item(tableRow,0)->text(),pointMark);
            scene->setDetailedPoint(QPointF(leftCol,leftLin));
            viewer->getLeftDisplay()->update();
        }
    }
    else if (table==rightImageTableWidget)
    {
        setCurrentPointKey(rightImageTableWidget->item(tableRow,3)->text().toInt(&ok));
        Marker* pointMark;
        int pointsTableIndex = findKeyAppearances(pointsTableWidget,currentPointKey);
        if (pointsTableWidget->item(pointsTableIndex,1)->text() == "Control")
            pointMark = selectedMark;
        else
            pointMark = photoSelectedMark;
        rightCol=rightImageTableWidget->item(tableRow,1)->text().toDouble(&ok);
        rightLin=rightImageTableWidget->item(tableRow,2)->text().toDouble(&ok);

        if (rightImageTableWidget->item(tableRow,1)->text()!="--" && rightImageTableWidget->item(tableRow,2)->text()!="--")
        {
            SingleScene* scene = dynamic_cast<SingleScene*>(viewer->getRightDisplay()->getCurrentScene());
            scene->moveTo(QPointF(rightCol,rightLin));
            scene->geometry()->updatePoint(QPointF(rightCol,rightLin),currentPointKey,rightImageTableWidget->item(tableRow,0)->text(),pointMark);
            scene->setDetailedPoint(QPointF(rightCol,rightLin));
            viewer->getRightDisplay()->update();
        }
    }
    else if (table==pointsTableWidget)
    {
        setCurrentPointKey(pointsTableWidget->item(tableRow,5)->text().toInt(&ok));
        Marker* pointMark;
        int pointsTableIndex = findKeyAppearances(pointsTableWidget,currentPointKey);
        if (pointsTableWidget->item(pointsTableIndex,1)->text() == "Control")
            pointMark = selectedMark;
        else
            pointMark = photoSelectedMark;
        int leftTableIndex = findKeyAppearances(leftImageTableWidget,currentPointKey);
        int rightTableIndex = findKeyAppearances(rightImageTableWidget,currentPointKey);
        if (leftTableIndex >=0)
        {
            leftCol=leftImageTableWidget->item(leftTableIndex,1)->text().toDouble(&ok);
            leftLin=leftImageTableWidget->item(leftTableIndex,2)->text().toDouble(&ok);
            if (leftImageTableWidget->item(leftTableIndex,1)->text()!="--" && leftImageTableWidget->item(leftTableIndex,2)->text()!="--")
            {
                SingleScene* scene = dynamic_cast<SingleScene*>(viewer->getLeftDisplay()->getCurrentScene());
                scene->moveTo(QPointF(leftCol,leftLin));
                scene->geometry()->updatePoint(QPointF(leftCol,leftLin),currentPointKey,leftImageTableWidget->item(leftTableIndex,0)->text(),pointMark);
                scene->setDetailedPoint(QPointF(leftCol,leftLin));
                viewer->getLeftDisplay()->update();
            }
        }
        if (rightTableIndex >=0)
        {
            rightCol=rightImageTableWidget->item(rightTableIndex,1)->text().toDouble(&ok);
            rightLin=rightImageTableWidget->item(rightTableIndex,2)->text().toDouble(&ok);
            if (rightImageTableWidget->item(rightTableIndex,1)->text()!="--" && rightImageTableWidget->item(rightTableIndex,2)->text()!="--")
            {
                SingleScene* scene = dynamic_cast<SingleScene*>(viewer->getRightDisplay()->getCurrentScene());
                scene->moveTo(QPointF(rightCol,rightLin));
                scene->geometry()->updatePoint(QPointF(rightCol,rightLin),currentPointKey,rightImageTableWidget->item(rightTableIndex,0)->text(),pointMark);
                scene->setDetailedPoint(QPointF(rightCol,rightLin));
                viewer->getRightDisplay()->update();
            }
        }
    }
}

void PTUserInterface_Qt::setMaxIteration(int iterations)
{
    ptManager->setMaxIteration(iterations);
}

void PTUserInterface_Qt::setMetricConvergencyValue(double value)
{
    ptManager->setMetricConvergencyValue(value);
}

void PTUserInterface_Qt::setAngularConvergencyValue(double value)
{
    ptManager->setAngularConvergencyValue(value);
}

void PTUserInterface_Qt::setCurrentPointKey(int newPointKey)
{
    currentPointKey = (newPointKey > 0 ? newPointKey : 1);
    selectAllAppearances(currentPointKey);
    showImagesAppearances(currentPointKey);

}

void PTUserInterface_Qt::addPoint()
{
    int idNewPoint = ptManager->createNewPoint();
    ptManager->setENH();
    ptManager->reloadPointsCoordinates();
    updatePointsTable();
    setCurrentPointKey(idNewPoint);
    clearAllMarks(viewer->getLeftDisplay());
    markAllpoints(viewer->getLeftDisplay());
    clearAllMarks(viewer->getRightDisplay());
    markAllpoints(viewer->getRightDisplay());
}

void PTUserInterface_Qt::toggleInsertPointMode(bool insertionMode)
{
    this->insertionMode=insertionMode;
    if (insertionMode)
        insertPointInButton->setStatusTip("Insertion mode ON");
    else
        insertPointInButton->setStatusTip("Insertion mode OFF");

}

void PTUserInterface_Qt::replaceTo__(ETableWidget *imageTable)
{
    int rows= imageTable->rowCount();
    int cols= imageTable->columnCount();
    for (int i=0;i<rows;i++)
    {
        for (int j=0;j<cols;j++)
        {
            if (imageTable->item(i,j)->text()=="-1.000")
                imageTable->item(i,j)->setText("--");
        }
    }
}

//Retorna a posicao do ponto na tabela se o ponto nao estiver retorna -1
/*
int PTUserInterface_Qt::findKeyAppearances(ETableWidget *table, int pointKey)
{
 int rows=table->rowCount();
 bool ok;
 for (int i=0;i<rows;i++)
 {
  int key=table->item(i,3)->text().toInt(&ok);
  if (key==pointKey)
   return i;
 }
 return -1;
}*/


void PTUserInterface_Qt::putInStack(int oldCol,int oldLin,int pointKey, int imageKey,QString pointID)
{
    QPointF tempPixel(oldCol,oldLin);
    PointMark *markPoint= new PointMark(tempPixel,pointKey,imageKey,pointID);
    undoStack.push_back(markPoint);

    //qDebug() << "Empilhando "<< markPoint->toString();
}

void PTUserInterface_Qt::FTdone()
{
    close();
}

void PTUserInterface_Qt::undoMark()
{
    //qDebug("undo Chamado");
    if(!undoStack.isEmpty())
    {
        PointMark *mark=undoStack.last();
        QPointF *point=mark->getDigitalCoordinate();
        //qDebug() << "Desempilhando: "<< mark->toString();
        ptManager->updateDigitalCoordinatesPoint(mark->getKeyImage(),mark->getKeyPoint(),point->x(),point->y());

        if (currentPointKey==mark->getKeyPoint())
        {
            if (findKeyAppearances(leftImageTableWidget,mark->getKeyPoint())!=-1)
                updateImageTable(leftImageTableWidget,leftImageString);
            if (findKeyAppearances(rightImageTableWidget,mark->getKeyPoint())!=-1)
                updateImageTable(rightImageTableWidget,rightImageString);
        }
        else
        {
            if (findKeyAppearances(leftImageTableWidget,mark->getKeyPoint())!=-1)
                updateImageTable(leftImageTableWidget,leftImageString);
            if (findKeyAppearances(rightImageTableWidget,mark->getKeyPoint())!=-1)
                updateImageTable(rightImageTableWidget,rightImageString);
            setCurrentPointKey(mark->getKeyPoint());
        }

        undoStack.pop_back();
    }

}

// Created by Marcelo Teixiera Silveira
int PTUserInterface_Qt::saveFtReport(char * filename)
{
    // Open file to save
    std::ofstream arq(filename);
    if (arq.fail())
    {
        printf("Problems while saving ...\n");
        return 0;
    }

    arq << "E-FOTO Photo-triangulation Brief Report\n";
    arq << "=======================================\n\n";
    arq << "Digital image unit: pixels\n";
    arq << "Angle unit: degrees\n";
    arq << "Ground X, Y, Z unit: meters\n\n";

    arq << "Number of iterations: " << ptManager->getBundleAdjustment()->getTotalIterations() << "\n";
    std::string converged;
    (ptManager->getBundleAdjustment()->isConverged()) ? converged = "Yes" : converged = "No";
    arq << "Converged: " << converged << "\n";
    arq << "RMSE: " << Conversion::doubleToString(ptManager->getRMSE(),5) << "\n\n\n";

    arq << "Exterior Orientation:\n\n";

    // todo: review line:
    arq << "left" << std::setw(25) << "Image Id" << std::setw(25) << "X0" << std::setw(25) << "Y0" << std::setw(25) <<  "Z0" << std::setw(25) <<  "Omega" << std::setw(25) <<  "Phy" << std::setw(25) <<  "kappa";

    arq << "\n";

    std::deque<std::string> images = selectionImagesView->getSelectedItens();
    Matrix mat_aux = ptManager->getMatrixOE();

    for (size_t i=1; i <= mat_aux.getRows(); i++)
        arq << std::setw(25) << images.at(i-1) << std::setw(25) << Conversion::doubleToString(mat_aux.get(i,4),5) << std::setw(25) << Conversion::doubleToString(mat_aux.get(i,5),5) << std::setw(25) << Conversion::doubleToString(mat_aux.get(i,6),5) << std::setw(25) << Conversion::doubleToString(mat_aux.get(i,1),5) << std::setw(25) << Conversion::doubleToString(mat_aux.get(i,2),5) << std::setw(25) << Conversion::doubleToString(mat_aux.get(i,3),5) << "\n";

    arq << "\nMVC:\n";

    mat_aux = ptManager->getMVC();

    for (size_t i=1; i <= mat_aux.getRows(); i++)
        arq << std::setw(25) << images.at(i-1) << std::setw(25) << Conversion::doubleToString(mat_aux.get(i,4),8) << std::setw(25) << Conversion::doubleToString(mat_aux.get(i,5),8) << std::setw(25) << Conversion::doubleToString(mat_aux.get(i,6),8) << std::setw(25) << Conversion::doubleToString(mat_aux.get(i,1),8) << std::setw(25) << Conversion::doubleToString(mat_aux.get(i,2),8) << std::setw(25) << Conversion::doubleToString(mat_aux.get(i,3),8) << "\n";


    arq << "\n\nPhotogrammetric points:\n\n";

    arq << std::setw(25) << "Point Id" << std::setw(25) << "E" << std::setw(25) << "N" << std::setw(25) << "H" << std::setw(25) << "δE" << std::setw(25) << "δN" << std::setw(25) << "δH";

    arq << "\n";

    std::deque<std::string>  ids  = ptManager->getSelectedPointIdPhotogrammetric();
    if (ids.size()!=0)
    {
        Matrix pointsPhotogrametricMatrix = ptManager->getPhotogrammetricENH();
        Matrix pointsResiduosPhotogrametricMatrix = ptManager->getResiduoPhotogrammetric();

        for (size_t i=0;i<ids.size();i++)
            arq << std::setw(25) << ids.at(i) << std::setw(25) << Conversion::doubleToString(pointsPhotogrametricMatrix.get(i+1, 1),5) << std::setw(25) << Conversion::doubleToString(pointsPhotogrametricMatrix.get(i+1, 2),5) << std::setw(25) << Conversion::doubleToString(pointsPhotogrametricMatrix.get(i+1, 3),5) << std::setw(25) << Conversion::doubleToString(pointsResiduosPhotogrametricMatrix.get(i+1, 1),8) << std::setw(25) << Conversion::doubleToString(pointsResiduosPhotogrametricMatrix.get(i+1, 2),8) << std::setw(25) << Conversion::doubleToString(pointsResiduosPhotogrametricMatrix.get(i+1, 3),8) << "\n";
    }

    arq << "\n\nEnd of the report.";

    arq.close();

    return 1;
}

PointMark::PointMark(QPointF coord, int keypoint,int imagekey,QString id)
{
    this->setDigitalCoordinate(coord);
    this->setPointId(id);
    this->setKeyPoint(keypoint);
    this->setKeyImage(imagekey);
}

void PointMark::setDigitalCoordinate(QPointF newCoord)
{
    digitalCoordinate = new QPointF(newCoord.x(),newCoord.y());
}

void PointMark::setKeyPoint(int newKeyPoint)
{
    keyPoint=  (newKeyPoint>0 ? newKeyPoint:1);
}

void PointMark::setPointId(QString newPointId)
{
    pointId= newPointId;
}

void PointMark::setKeyImage(int newKeyImage)
{
    keyImage = (newKeyImage > 0 ? newKeyImage : 1);
}

QPointF * PointMark::getDigitalCoordinate()
{
    return digitalCoordinate;
}

QString PointMark::getPointId() const
{
    return pointId;
}

unsigned int PointMark::getKeyPoint() const
{
    return keyPoint;
}

unsigned int PointMark::getKeyImage() const
{
    return keyImage;
}

QString PointMark::toString()
{
    return QString(pointId + " " + QString::number(keyPoint)+ " from " +QString::number(keyImage)+ " with " +QString::number(digitalCoordinate->x())+ "x" +QString::number(digitalCoordinate->y()));
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
