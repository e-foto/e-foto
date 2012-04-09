#include "SPUserInterface_Qt.h"

#include <qdesktopwidget.h>
#include <qapplication.h>
#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qaction.h>
#include <qstring.h>

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

	viewerSeparated = new SeparatedStereoViewer();
	viewerSeparated->blockOpen();
	viewerSeparated->blockSave();
	viewerSeparated->getLeftMarker().setToOnlyEmitClickedMode();
	viewerSeparated->getRightMarker().setToOnlyEmitClickedMode();
	viewerSeparated->getLeftDisplay()->setCurrentScene(viewer->getDisplay()->getCurrentScene()->getLeftScene());
	viewerSeparated->getRightDisplay()->setCurrentScene(viewer->getDisplay()->getCurrentScene()->getRightScene());

	QTabWidget* viewersTab = new QTabWidget();
	viewersTab->addTab(viewer,"StereoViewer");
	viewersTab->addTab(viewerSeparated,"SeparatedViewers");
	setCentralWidget(viewersTab);

	connect(&viewer->getMarker(),SIGNAL(clicked(QPointF, QPointF)),this,SLOT(stereoClicked(QPointF,QPointF)));
	connect(&viewer->getMarker(),SIGNAL(mouseMoved(QPointF,QPointF)),this,SLOT(stereoMoved(QPointF,QPointF)));

    show();
	qApp->processEvents();
    changePair(1,2);
    viewer->getToolBar()->executeAction(viewer->getToolBar()->setMoveTool);
    viewer->getDisplay()->updateAll();

	LoadingScreen::instance().close();
	return true;
}

void SPUserInterface_Qt::updateData()
{
	updateTable();
	viewer->getDisplay()->updateAll();
	viewerSeparated->update();
}

void SPUserInterface_Qt::updateTable()
{
	QString txt = QString::fromStdString(manager->getFeaturesList());

	TreeFeatures *tree = new TreeFeatures(txt);

	treeView->setModel(tree);
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
	viewer->getToolBar()->changeMode(1);
	viewerSeparated->getToolBar()->changeMode(1);
	if (editPtButton->isChecked())
		editPtButton->setChecked(false);

	if (selButton->isChecked())
		selButton->setChecked(false);

	measure_mode = addPtButton->isChecked();
}

void SPUserInterface_Qt::onEditPtButton()
{
	viewer->getToolBar()->changeMode(1);
	viewerSeparated->getToolBar()->changeMode(1);
	if (addPtButton->isChecked())
		addPtButton->setChecked(false);

	if (selButton->isChecked())
		selButton->setChecked(false);

	editPtButton->isChecked() ? measure_mode = 2 : measure_mode = 0;
}

void SPUserInterface_Qt::onSelPtButton()
{
	viewer->getToolBar()->changeMode(1);
	viewerSeparated->getToolBar()->changeMode(1);
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

	viewer->getDisplay()->updateAll();
}

void SPUserInterface_Qt::onFeatureSelected()
{
	// Get Feature data
	string fname;
	int fclass, ftype;

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
		manager->getSelected(fid,pid);
		treeView->setCurrentIndex(treeView->model()->index(fid-1,0));
	}
}

void SPUserInterface_Qt::stereoMoved(QPointF lPos, QPointF rPos)
{
	double lx = lPos.x(), ly = lPos.y(), rx = rPos.x(), ry = rPos.y(), X , Y, Z;
	manager->computeIntersection(lx, ly, rx, ry, X, Y, Z);
	viewer->getToolBar()->actualizeStereoInfoLabel(X, Y, Z);
}

void SPUserInterface_Qt::changePair(int leftKey, int rightKey)
{
	viewer->loadLeftImage(QString(manager->getFullImagePath(leftKey).c_str()));
	viewer->loadRightImage(QString(manager->getFullImagePath(rightKey).c_str()));
	viewer->update();
}

void SPUserInterface_Qt::onChangePair(int pos)
{
	int lk, rk;
	manager->changePair(pos, lk, rk);
	changePair(lk, rk);
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
