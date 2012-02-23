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
		QObject::connect(endButton, SIGNAL(clicked()), this, SLOT(onLoadButton()));
		QObject::connect(selButton, SIGNAL(clicked()), this, SLOT(onLoadButton()));
		QObject::connect(addPtButton, SIGNAL(clicked()), this, SLOT(onLoadButton()));
		QObject::connect(removePtButton, SIGNAL(clicked()), this, SLOT(onLoadButton()));
		QObject::connect(editPtButton, SIGNAL(clicked()), this, SLOT(onLoadButton()));


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
	viewer = new SeparatedStereoViewer();
	viewer->blockOpen();
	viewer->blockSave();
	setCentralWidget(viewer);
	viewer->setFeatures(manager->getFeaturesLink());

	show();
	qApp->processEvents();
	changePair(1,2);

	LoadingScreen::instance().close();
	return true;
}

void SPUserInterface_Qt::updateData()
{
	updateTable();
	viewer->update();
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
	manager->addFeature(nameEdit->text().toStdString(), comboBox_3->currentIndex()+1, comboBox_4->currentIndex()+1);

	updateData();
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

void SPUserInterface_Qt::changePair(int leftKey, int rightKey)
{
	viewer->loadLeftImage(QString(manager->getFullImagePath(leftKey).c_str()));
	viewer->loadRightImage(QString(manager->getFullImagePath(rightKey).c_str()));
	viewer->update();
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br