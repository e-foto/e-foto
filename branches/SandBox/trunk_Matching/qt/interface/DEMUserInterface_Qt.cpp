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
        QObject::connect(interButton, SIGNAL(clicked()), this, SLOT(onDemGridClicked()));
        QObject::connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(onLSMCheckChanged(int)));
        QObject::connect(abortButton, SIGNAL(clicked()), this, SLOT(onAbortClicked()));
        QObject::connect(comboBox2_2, SIGNAL(currentIndexChanged(int)), this, SLOT(onGridAreaLimitsStateChanged(int)));
        QObject::connect(comboBox1, SIGNAL(currentIndexChanged(int)), this, SLOT(onInterStateChanged(int)));
        QObject::connect(checkBox_3, SIGNAL(stateChanged(int)), this, SLOT(onShowImageStateChanged(int)));
        QObject::connect(seedsButton, SIGNAL(clicked()), this, SLOT(onSeedsEditorClicked()));
        QObject::connect(stereoButton, SIGNAL(clicked()), this, SLOT(onStereoplotterClicked()));
        QObject::connect(comboBox4, SIGNAL(currentIndexChanged(int)), this, SLOT(onMatchingMethodChanged(int)));

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
        onMatchingMethodChanged(comboBox4->currentIndex());

        // Set initial states
        onGridAreaLimitsStateChanged(0);
        onInterStateChanged(0);
        manager->setShowImage(checkBox_3->isChecked());

        allow_close = true;

	qApp->processEvents();
	init();
}

DEMUserInterface_Qt::~DEMUserInterface_Qt()
{
    if (sed)
        delete sed;
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
            if (QMessageBox::warning(this, "Unsaved work","A DEM was generated and not saved. Continue?", QMessageBox::Ok, QMessageBox::No) == QMessageBox::No)
            {
                e->ignore();
                return;
            }
        }

        if (manager->isGridUnsaved())
        {
            if (QMessageBox::warning(this, "Unsaved work","An interpolated DEM was generated and not saved. Continue?", QMessageBox::Ok, QMessageBox::No) == QMessageBox::No)
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

void DEMUserInterface_Qt::setMathcingHistogram(int *hist)
{
    histProgressBar1->setValue(hist[0]);
    histProgressBar2->setValue(hist[1]);
    histProgressBar3->setValue(hist[2]);
    histProgressBar4->setValue(hist[3]);
    histProgressBar5->setValue(hist[4]);
    histProgressBar6->setValue(hist[5]);
}

void DEMUserInterface_Qt::showImage(Matrix* image, bool isGrayscale)
{
    SingleViewer* sv = new SingleViewer(this);
    sv->loadImage(image, isGrayscale);
    sv->blockOpen();
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
    autoMinZLabel->setText(QString::number(min,'f',5));
    autoMaxZLabel->setText(QString::number(max,'f',5));
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
    // File save dialog
    QString filename;

    if (comboBox9->currentIndex() == 0)
        filename = QFileDialog::getSaveFileName(this, tr("Save file"), ".", tr("DEM Grid (*.dsm);; All files (*.*)"));
    else
        filename = QFileDialog::getSaveFileName(this, tr("Save file"), ".", tr("ASCII DEM Grid (*.txt);; All files (*.*)"));
    if (filename=="")
        return;

    // Save last dir
    int i=filename.lastIndexOf("/");
    QDir dir(filename.left(i));
    dir.setCurrent(dir.absolutePath());

    // Save DEM
    manager->saveDemGrid((char *)filename.toStdString().c_str(), comboBox9->currentIndex());
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
    if (!manager->loadDem((char *)filename.toStdString().c_str(), comboBox8->currentIndex()))
    {
        QMessageBox::critical(this,"Load error","Error while loading file. Check if file format option matches the file.");
        return;
    }

    enableAfterDEM();
}

void DEMUserInterface_Qt::enableAfterDEM()
{
    // Enable options
    saveButton2->setEnabled(true);
    interButton->setEnabled(true);
    tabWidget->setTabEnabled(4, true);
}

void DEMUserInterface_Qt::enableAfterGrid()
{
    // Enable options
    saveButton->setEnabled(true);
}

void DEMUserInterface_Qt::onDemGridClicked()
{
    if (!checkBoundingBox())
        return;

    // Perform interpolation
    manager->interpolateGrid(comboBox0->currentIndex(), comboBox1->currentIndex(), comboBox2_2->currentIndex(), XilineEdit->text().toDouble(), YilineEdit->text().toDouble(), XflineEdit->text().toDouble(), YflineEdit->text().toDouble(), doubleSpinBox_8->value(), doubleSpinBox_9->value(), comboBox6->currentIndex(), doubleSpinBox15->value(), doubleSpinBox16->value(), comboBox7->currentIndex());

    if (manager->cancelFlag())
        return;

    enableAfterGrid();
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

/*
 * Image matching
 **/

void DEMUserInterface_Qt::onDemExtractionClicked()
{
    manager->setAutoExtractionSettings(comboBox3->currentIndex(), comboBox4->currentIndex(), spinBox1->value(), spinBox2->value(), doubleSpinBox0->value());
    manager->setLSMSettings(spinBox3->value(), spinBox4->value(), doubleSpinBox5->value(), doubleSpinBox6->value(), spinBox7->value(), doubleSpinBox8->value(), doubleSpinBox9->value(), doubleSpinBox10->value(), checkBox->isChecked(), doubleSpinBox17->value());
    manager->setNCCSettings(spinBox11->value(), spinBox12->value(), doubleSpinBox13->value(), doubleSpinBox14->value());
    manager->extractDEM(comboBox5->currentIndex(), checkBox_2->checkState());

    if (manager->cancelFlag())
        return;

    enableAfterDEM();
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

/*
 * Call other interfaces
 */

void DEMUserInterface_Qt::onSeedsEditorClicked()
{
    if (sed)
        delete sed;
    sed = new SeedEditorUserInterface_Qt(manager, this);
    this->setHidden(true);
    connect(sed,SIGNAL(closed(bool)),this,SLOT(show()));
    sed->showMaximized();
}

void DEMUserInterface_Qt::onStereoplotterClicked()
{
    QMessageBox::warning(this,"Warning","Sorry, not implemented");
}

void DEMUserInterface_Qt::showErrorMessage(QString msg)
{
    QMessageBox::critical(this, "Error",msg);
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
    connect(comboBox1,SIGNAL(currentIndexChanged(int)),this,SLOT(onComboBox1Changed(int)));

    Marker mark(SymbolsResource::getX(Qt::yellow, QSize(24, 24),2)); // Personalizando as marcas. Que no futuro eu quero melhorar para inserir uso de 2 ou 3 marcas de acordo com o tipo de ponto.
    viewer->getLeftMarker().changeMarker(mark);
    viewer->getRightMarker().changeMarker(mark);

    setCentralWidget(viewer);

    /*
    deque<string> images = ptManager->getStringImages(); // Busca os nomes das imagens
    deque<string> points = manager->getStringIdPoints();
    for (int i=0;i<images.size();i++)
    {
            QString img=images.at(i).c_str();
            listAllImages << img;
            if (i!=1)
                    listImageLeft << img;
            if (i!=0)
                    listImageRight << img;
    }
    for (int i=0;i<images.size();i++)
            listAllPoints << QString(points.at(i).c_str());

    currentPointKey =-1;
    leftImageString = images.at(0);
    rightImageString= images.at(1);
    leftImageKey = ptManager->getImageId(leftImageString);
    rightImageKey = ptManager->getImageId(rightImageString);

    leftImageComboBox->addItems(listImageLeft);
    rightImageComboBox->addItems(listImageRight);
//QTableWidget
    //Esconde as colunas das keys
    leftImageTableWidget->setColumnHidden(3,true);
//	leftImageTableWidget->setDisabled();
    pointsTableWidget->setColumnHidden(5,true);
    rightImageTableWidget->setColumnHidden(3,true);

    leftImageTableWidget->verticalHeader()->hide();
    rightImageTableWidget->verticalHeader()->hide();
    pointsTableWidget->verticalHeader()->hide();

    connect(leftImageComboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(updateImagesList(QString)));
    connect(rightImageComboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(updateImagesList(QString)));
    */

    // Copy seed and pair lists
    manager->getPointList(seeds,pairs);
    addPairs();
    updateData(0);
}

void SeedEditorUserInterface_Qt::closeEvent(QCloseEvent *)
{
    emit closed(true);
}

void SeedEditorUserInterface_Qt::imageClicked(QPointF p)
{

}

void SeedEditorUserInterface_Qt::updateImagesList(QString s)
{

}

void SeedEditorUserInterface_Qt::addPairs()
{
    deque<int> listPairs = manager->getImagesPairs();
    deque<Image*> listAllImages = manager->getImages();

    // Add pairs to the interface
    int left_id, right_id;
    int no_imgs = listAllImages.size();
    string str_left, str_right;
    stringstream txt;

    for (int i=0; i<listPairs.size(); i++)
    {
        // Decode
        left_id = 1 + (listPairs.at(i) % no_imgs);
        right_id = 1 + (listPairs.at(i) / no_imgs);
        str_left = listAllImages.at(left_id-1)->getImageId();
        str_right = listAllImages.at(right_id-1)->getImageId();
        txt.str(""); // Clear stream
        txt << "Images " << str_left << " and " << str_right;
        comboBox1->addItem(QString::fromAscii((char *)txt.str().c_str()));
    }
}

void SeedEditorUserInterface_Qt::saveSeeds()
{
    QString filename;

    filename = QFileDialog::getSaveFileName(this, tr("Save seeds"), ".", tr("Seeds (*.txt);; All files (*.*)"));
    if (filename=="")
        return;

    // Save last dir
    int i=filename.lastIndexOf("/");
    QDir dir(filename.left(i));
    dir.setCurrent(dir.absolutePath());

    // Save seeds
    seeds.save((char *)filename.toStdString().c_str(), 0);
}

void SeedEditorUserInterface_Qt::loadSeeds()
{
    QString filename;

    filename = QFileDialog::getOpenFileName(this, tr("Load seeds"), ".", tr("Seeds (*.txt);; All files (*.*)"));
    if (filename=="")
        return;

    // Save last dir
    int i=filename.lastIndexOf("/");
    QDir dir(filename.left(i));
    dir.setCurrent(dir.absolutePath());

    // Load seeds
    seeds.load((char *)filename.toStdString().c_str(), 0);

    // Update data
    updateData(comboBox1->currentIndex());
}

void SeedEditorUserInterface_Qt::onComboBox1Changed(int index)
{
    updateData(index);
}

void SeedEditorUserInterface_Qt::updateData(int i)
{
    //
    // Update current images
    //
    deque<int> listPairs = manager->getImagesPairs();
    deque<Image*> listAllImages = manager->getImages();

    // Decode
    int left_id, right_id;
    int no_imgs = listAllImages.size();
    left_id = 1 + (listPairs.at(i) % no_imgs);
    right_id = 1 + (listPairs.at(i) / no_imgs);

    //
    // Update Images
    //
    string left_file = listAllImages.at(left_id-1)->getFilepath() + '/' + listAllImages.at(left_id-1)->getFilename();
    string right_file = listAllImages.at(right_id-1)->getFilepath() + '/' + listAllImages.at(right_id-1)->getFilename();

    viewer->loadLeftImage(QString::fromStdString(left_file));
    viewer->loadRightImage(QString::fromStdString(right_file));

    //
    //  Clear marks
    //
    viewer->getLeftDisplay()->getCurrentScene()->geometry()->clear();
    viewer->getRightDisplay()->getCurrentScene()->geometry()->clear();

    //
    // Update table and seed points on image
    //
    MatchingPoints *mp;
    QTableWidgetItem *newItem;
    int p=0;
    QPointF left_coord, right_coord;

    tableWidget->setRowCount(0);

    Marker *mark_seeds = new Marker(SymbolsResource::getX(Qt::yellow, QSize(24, 24),2));

    for (int i=0; i<seeds.size(); i++)
    {
        mp = seeds.get(i+1);

        if (mp->left_image_id != left_id || mp->right_image_id != right_id)
            continue;

        // Image points
        left_coord.setX(double(mp->left_x));
        left_coord.setY(double(mp->left_y));
        right_coord.setX(double(mp->right_x));
        right_coord.setY(double(mp->right_y));
        viewer->getLeftMarker().insertMark(left_coord, i+1, QString::number(i+1), mark_seeds);
        viewer->getRightMarker().insertMark(right_coord, i+1, QString::number(i+1), mark_seeds);

        // Table
        tableWidget->insertRow(p);

        newItem = new QTableWidgetItem(tr("%1").arg(i+1));
        tableWidget->setItem(p, 0, newItem);
        newItem = new QTableWidgetItem(tr("%1").arg(mp->left_x));
        tableWidget->setItem(p, 1, newItem);
        newItem = new QTableWidgetItem(tr("%1").arg(mp->left_x));
        tableWidget->setItem(p, 2, newItem);
        newItem = new QTableWidgetItem(tr("%1").arg(mp->right_x));
        tableWidget->setItem(p, 3, newItem);
        newItem = new QTableWidgetItem(tr("%1").arg(mp->right_y));
        tableWidget->setItem(p, 4, newItem);

        p++;
    }

    //
    // Add matching points, if selected
    //
    if (checkBox->isChecked())
    {
        Marker *mark_pairs = new Marker(SymbolsResource::getX(Qt::red, QSize(24, 24),2));

        for (int i=0; i<pairs.size(); i++)
        {
            mp = pairs.get(i+1);

            if (mp->left_image_id != left_id || mp->right_image_id != right_id)
                continue;

            left_coord.setX(double(mp->left_x));
            left_coord.setY(double(mp->left_y));
            right_coord.setX(double(mp->right_x));
            right_coord.setY(double(mp->right_y));
            viewer->getLeftMarker().insertMark(left_coord, i+1, "", mark_pairs);
            viewer->getRightMarker().insertMark(right_coord, i+1, "", mark_pairs);
        }
    }

    //
    // Update viewer
    //
    viewer->update();
}
