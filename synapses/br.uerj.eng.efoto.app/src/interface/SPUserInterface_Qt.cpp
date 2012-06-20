#include "SPUserInterface_Qt.h"

#include "StereoDisplay.h"

#include <QDesktopWidget>
#include <QApplication>
#include <QVariant>
#include <QImage>
#include <QPixmap>
#include <QAction>
#include <QString>
#include <QCloseEvent>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

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
		: QMainWindow(parent, fl)
{
		setupUi(this);

		// Connections
		QObject::connect(comboBox_3, SIGNAL(currentIndexChanged(int)), this, SLOT(updateClass(int)));
		QObject::connect(loadButton, SIGNAL(clicked()), this, SLOT(onLoadButton()));
		QObject::connect(saveButton, SIGNAL(clicked()), this, SLOT(onSaveButton()));
		QObject::connect(addButton, SIGNAL(clicked()), this, SLOT(onAddButton()));
		QObject::connect(removeButton, SIGNAL(clicked()), this, SLOT(onRemoveButton()));
		QObject::connect(removeAllButton, SIGNAL(clicked()), this, SLOT(onRemoveAllButton()));
		QObject::connect(endButton, SIGNAL(clicked()), this, SLOT(onCloseFeature()));
		QObject::connect(selButton, SIGNAL(clicked()), this, SLOT(onSelPtButton()));
		QObject::connect(addPtButton, SIGNAL(clicked()), this, SLOT(onAddPtButton()));
		QObject::connect(removePtButton, SIGNAL(clicked()), this, SLOT(onRemovePtButton()));
		QObject::connect(editPtButton, SIGNAL(clicked()), this, SLOT(onEditPtButton()));
		QObject::connect(treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(onFeatureListClicked(QModelIndex)));
		QObject::connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangePair(int)));

		// Edit mode = None
		measure_mode = 0;

		this->manager = manager;
		setWindowState(this->windowState());

		showMaximized();

		updateClass(0);

		allow_close = true;

        tree = new TreeFeatures("");
        treeView->setModel(tree);

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
    QTabWidget* viewersTab = new QTabWidget();
    setCentralWidget(viewersTab);

#ifdef INTEGRATED_EFOTO
    stereoViewer = new StereoViewer();
    viewersTab->addTab(stereoViewer,"StereoViewer");
#endif //INTEGRATED_EFOTO
#ifdef SYNAPSE_EFOTO
    viewerService = ICortex::getInstance()->getSynapse<viewer::IViewerService>();
    stereoViewer = viewerService->instanceStereoViewer();
    viewersTab->addTab(stereoViewer.data(),"StereoViewer");
#endif //SYNAPSE_EFOTO

    stereoViewer->hideOpen(true);
    stereoViewer->hideSave(true);

    stereoViewer->getDisplay()->getCurrentScene()->getLeftScene()->geometry()->setFeatures(manager->getFeaturesLink(),0);
    stereoViewer->getDisplay()->getCurrentScene()->getRightScene()->geometry()->setFeatures(manager->getFeaturesLink(),1);

    connect(&stereoViewer->getToolBar()->_mark,SIGNAL(clicked(QPointF, QPointF)),this,SLOT(stereoClicked(QPointF,QPointF)));
    connect(&stereoViewer->getToolBar()->_mark,SIGNAL(mouseMoved(QPointF,QPointF)),this,SLOT(stereoMoved(QPointF,QPointF)));
    connect(stereoViewer->getDisplay(),SIGNAL(resized(int,int)),this,SLOT(adjustFit(int,int)));

    show();
	qApp->processEvents();
    changePair(1,2);

    stereoViewer->getToolBar()->executeAction(stereoViewer->getToolBar()->setMoveTool);
    stereoViewer->getDisplay()->updateAll();

	LoadingScreen::instance().close();
    return true;
}

void SPUserInterface_Qt::updateData()
{
	updateTable();
    //stereoViewer->getDisplay()->updateAll();  //rever! para capturar estes eventos
}

void SPUserInterface_Qt::updateTable()
{
	QString txt = QString::fromStdString(manager->getFeaturesList());
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
		comboBox_4->addItem("Undefined");
		comboBox_4->addItem("Paved street");
		comboBox_4->addItem("Unpaved street");
		comboBox_4->addItem("Trail");
		comboBox_4->addItem("Railway");
		comboBox_4->addItem("River");
		comboBox_4->addItem("Bridge");
	}

	// Polygon
	if (feat_type == 2)
	{
		comboBox_4->addItem("Undefined");
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
	}
}

/*
 *  Button clicks
 **/

void SPUserInterface_Qt::onLoadButton()
{
	// File open dialog
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Fetures file"), ".", tr("Text file (*.txt);; All files (*.*)")) ;
	// if no file name written, return
	if (filename=="")
			return;

	// Save last dir
	int i=filename.lastIndexOf("/");
	QDir dir(filename.left(i));
	dir.setCurrent(dir.absolutePath());

	// Load DEM
	bool sp_load_flag = manager->loadFeatures((char *)filename.toStdString().c_str());

	// Report error
	if (!sp_load_flag)
	{
			QMessageBox::critical(this,"Error","Invalid features file format.");
			return;
	}

	manager->updateProjections();
	updateData();
}

void SPUserInterface_Qt::onSaveButton()
{
	// File open dialog
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Fetures file"), ".", tr("Text file (*.txt);; All files (*.*)")) ;
	// if no file name written, return
	if (filename=="")
			return;

	// Save last dir
	int i=filename.lastIndexOf("/");
	QDir dir(filename.left(i));
	dir.setCurrent(dir.absolutePath());

	// Load DEM
	manager->saveFeatures((char *)filename.toStdString().c_str());

}

void SPUserInterface_Qt::onAddButton()
{
	manager->addFeature(nameEdit->text().toStdString(), comboBox_3->currentIndex()+1, comboBox_4->currentIndex());

	updateData();

	int fid, pid;
	manager->getSelected(fid, pid);

	treeView->setCurrentIndex(treeView->model()->index(fid-1,0));
}

void SPUserInterface_Qt::onRemoveButton()
{
	manager->removeFeature();

	updateData();
}

void SPUserInterface_Qt::onRemoveAllButton()
{
	if (QMessageBox::warning(this,"Erase all?","Erase all features?",QMessageBox::Yes,QMessageBox::No) == QMessageBox::No)
		return;

	manager->removeAllFeatures();

	updateData();
}

void SPUserInterface_Qt::onAddPtButton()
{
    stereoViewer->getToolBar()->changeMode(1);
	if (editPtButton->isChecked())
		editPtButton->setChecked(false);

	if (selButton->isChecked())
		selButton->setChecked(false);

    measure_mode = addPtButton->isChecked();
}

void SPUserInterface_Qt::onEditPtButton()
{
    stereoViewer->getToolBar()->changeMode(1);
	if (addPtButton->isChecked())
		addPtButton->setChecked(false);

	if (selButton->isChecked())
		selButton->setChecked(false);

    editPtButton->isChecked() ? measure_mode = 2 : measure_mode = 0;
}

void SPUserInterface_Qt::onSelPtButton()
{
    stereoViewer->getToolBar()->changeMode(1);
	if (addPtButton->isChecked())
		addPtButton->setChecked(false);

	if (editPtButton->isChecked())
		editPtButton->setChecked(false);

    selButton->isChecked() ? measure_mode = 3 : measure_mode = 0;
}

void SPUserInterface_Qt::onRemovePtButton()
{
	manager->removePoint();
	updateData();
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

	onFeatureSelected();

    stereoViewer->getDisplay()->updateAll();
}

void SPUserInterface_Qt::onFeatureSelected()
{
	// Get Feature data
	string fname;
    int fclass, ftype, feat_id, pt_id;

	manager->getFeatureData(fname, ftype, fclass);

	nameEdit->setText(QString::fromStdString(fname));
	comboBox_3->setCurrentIndex(ftype-1);
	comboBox_4->setCurrentIndex(fclass);
}

void SPUserInterface_Qt::addImagePair(char * item)
{
	QString text = QString::fromAscii(item);
	comboBox->addItem(text);
}

void SPUserInterface_Qt::onCloseFeature()
{
	manager->setSelected(-1,-1);
	treeView->clearSelection();
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
        onFeatureSelected();
        treeView->setFocus();
	}
}

void SPUserInterface_Qt::stereoMoved(QPointF lPos, QPointF rPos)
{
	double lx = lPos.x(), ly = lPos.y(), rx = rPos.x(), ry = rPos.y(), X , Y, Z;
	manager->computeIntersection(lx, ly, rx, ry, X, Y, Z);
    stereoViewer->getToolBar()->actualizeStereoInfoLabel(X, Y, Z);
}


void SPUserInterface_Qt::centerImages(ObjectSpaceCoordinate coord, double zoom)
{
    ImageSpaceCoordinate centerLeft = manager->getLeftPoint(coord);
    ImageSpaceCoordinate centerRight = manager->getRightPoint(coord);
    QPointF atL(centerLeft.getPosition().get(1), centerLeft.getPosition().get(2));
    QPointF atR(centerRight.getPosition().get(1), centerRight.getPosition().get(2));
    stereoViewer->getDisplay()->getCurrentScene()->getLeftScene()->scaleTo(zoom);
    stereoViewer->getDisplay()->getCurrentScene()->getRightScene()->scaleTo(zoom);
    stereoViewer->getDisplay()->getCurrentScene()->getLeftScene()->moveTo(atL);
    stereoViewer->getDisplay()->getCurrentScene()->getRightScene()->moveTo(atR);

    stereoViewer->update();
}

void SPUserInterface_Qt::changePair(int leftKey, int rightKey)
{
    stereoViewer->loadImage(IStereoViewer::LEFT_CHANNEL, QString(manager->getFullImagePath(leftKey).c_str()));
    stereoViewer->loadImage(IStereoViewer::LEFT_CHANNEL,QString(manager->getFullImagePath(rightKey).c_str()));
    stereoViewer->update();
}

void SPUserInterface_Qt::onChangePair(int pos)
{
	int lk, rk;
	manager->changePair(pos, lk, rk);
    changePair(lk, rk);
    centerImages(manager->getCentralPoint(), 1.0);
    stereoViewer->getDisplay()->adjustFit(QPointF(manager->getLeftPoint(manager->getBoundingBoxCenter()).getPosition().get(1), manager->getLeftPoint(manager->getBoundingBoxCenter()).getPosition().get(2)),QPointF(manager->getRightPoint(manager->getBoundingBoxCenter()).getPosition().get(1),manager->getRightPoint(manager->getBoundingBoxCenter()).getPosition().get(2)),manager->getBoundingBoxIdealZoom(stereoViewer->getDisplay()->width(),stereoViewer->getDisplay()->height()));
}

void SPUserInterface_Qt::adjustFit(int width, int height)
{
    stereoViewer->getDisplay()->adjustFit(QPointF(manager->getLeftPoint(manager->getBoundingBoxCenter()).getPosition().get(1), manager->getLeftPoint(manager->getBoundingBoxCenter()).getPosition().get(2)),QPointF(manager->getRightPoint(manager->getBoundingBoxCenter()).getPosition().get(1),manager->getRightPoint(manager->getBoundingBoxCenter()).getPosition().get(2)),manager->getBoundingBoxIdealZoom(width,height));
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
