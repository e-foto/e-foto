#include "ImageViewers.h"
#include <QDesktopWidget>
#include <QStatusBar>

#include "Features.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

SingleViewer::SingleViewer(QWidget* parent) : QMainWindow(parent)
{
	sd = new SingleDisplay(this);
	setCentralWidget(sd);
	tool = new SingleToolsBar(sd, this);
	addToolBar(Qt::TopToolBarArea,tool);
	statusBar()->addWidget(tool->getInfo());
	addDockWidget(Qt::RightDockWidgetArea, tool->getNearview());
	addDockWidget(Qt::RightDockWidgetArea, tool->getOverview());

	QRect desktop = QDesktopWidget().screenGeometry();
	move(-pos().x() - width()/2 + desktop.width()/2, -pos().y() - height()/2 + desktop.height()/2);

	connect(&tool->mark, SIGNAL(clicked(QPointF)), this, SLOT(emitMark(QPointF)));
}

void SingleViewer::closeEvent(QCloseEvent *)
{
	if (tool->showNearview->isChecked())
		tool->showNearview->trigger();
	if (tool->showOverview->isChecked())
		tool->showOverview->trigger();
	delete tool;
	delete sd;
}

void SingleViewer::markMode()
{
	tool->executeAction(tool->setMarkTool);
}

void SingleViewer::moveMode()
{
	tool->executeAction(tool->setMoveTool);
}

void SingleViewer::zoomInMode()
{
	tool->executeAction(tool->setZoomTool);
}

void SingleViewer::zoomOutMode()
{
	tool->executeAction(tool->setZoomTool);
}


void SingleViewer::setDetailRelation(double zoom)
{
//rever!
}

void SingleViewer::setDetailTracking(bool status)
{
//rever!
}


void SingleViewer::hideOpen(bool status)
{
	tool->setOpenVisible(status);
}

void SingleViewer::hideSave(bool status)
{
	tool->setSaveVisible(status);
}

void SingleViewer::hideMark(bool status)
{
	tool->setMarkVisible(status);
	if (!status)
		tool->near_.setNearCursor(QCursor(Qt::ArrowCursor));
	else
		tool->near_.setNearCursor(QCursor(QPixmap::fromImage(SymbolsResource::getBordedCross(QColor(255,255,255,255), QColor(0,0,0,255), QSize(25, 25)))));
}

void SingleViewer::hideDetail(bool status)
{
	if (tool->nearIsVisible() == status)
		tool->executeAction(tool->showNearview);
}

void SingleViewer::hideOverView(bool status)
{
	if (tool->overIsVisible() == status)
		tool->executeAction(tool->showOverview);
}


void SingleViewer::startOpenImageDialog()
{
	tool->executeAction(tool->openImage);
}

void SingleViewer::startSaveImageDialog()
{
	tool->executeAction(tool->saveImage);
}


bool SingleViewer::loadImage(QString filename)
{
	SingleScene* ss = (SingleScene*) sd->getCurrentScene();
	ss->loadImage(filename);
	tool->setFitView->trigger();
}

void SingleViewer::loadImage(QImage* image)
{
	SingleScene* ss = (SingleScene*) sd->getCurrentScene();
	ss->loadImage(*image);
	tool->setFitView->trigger();
}

void SingleViewer::loadImage(Matrix* image, bool isGrayscale)
{
	SingleScene* ss = (SingleScene*) sd->getCurrentScene();
	ss->loadImage(image, isGrayscale);
	tool->setFitView->trigger();
}


QImage SingleViewer::getPrintScreen(bool rasterOnly)
{
//rever!
}

void SingleViewer::fit()
{
	tool->executeAction(tool->setFitView);
}

void SingleViewer::setZoom(double zoom)
{
	tool->scaleSpinBox->setValue(zoom*100);
	sd->getCurrentScene()->scaleTo(zoom);
	sd->updateAll();
}

void SingleViewer::moveTo(double x, double y)
{
	sd->getCurrentScene()->moveTo(QPointF(x,y));
}

void SingleViewer::insertMark(double x, double y, unsigned int key, QString label, Marker *marker)
{
	tool->mark.insertMark(QPointF(x,y),key,label, marker);
}

unsigned int SingleViewer::addMark(double x, double  y, QString label, Marker *marker)
{
	tool->mark.addMark(QPointF(x,y), 0, label, marker);
}

void SingleViewer::deleteMark(unsigned int key)
{
//rever!
}

void SingleViewer::setSelectedMark(unsigned int key)
{
//rever!
}

void SingleViewer::setSelectedMarker(QImage marker, int hotX, int hotY)
{
//rever!
}

void SingleViewer::setDefaultMarker(QImage marker, int hotX, int hotY)
{
//rever!
}


void SingleViewer::setImageMode()
{
	tool->setImageMode();
}

void SingleViewer::setOrtoImageMode(double xi, double dx, double yi, double dy)
{
	tool->setOrtoImageMode(xi, dx, yi, dy);
}

void SingleViewer::setElevationImageMode(double xi, double dx, double yi, double dy, double zi, double dz)
{
	tool->setElevationImageMode(xi, dx, yi, dy, zi, dz);
}











SeparatedStereoViewer::SeparatedStereoViewer(QWidget* parent) : QMainWindow(parent)
{
	QWidget* centerWidget = new QWidget(this);
	QMainWindow* leftPart = new QMainWindow(this);
	QMainWindow* rightPart = new QMainWindow(this);

	leftDisplay = new SingleDisplay(this);
	rightDisplay = new SingleDisplay(this);
	tool = new SeparatedStereoToolsBar(leftDisplay, rightDisplay, this);

	/** Mudança feita pelo Paulo para mudar o local inicial da toolbar*/
	addToolBar(Qt::RightToolBarArea, tool);

	leftPart->setCentralWidget(leftDisplay);
	leftPart->statusBar()->addWidget(tool->getLeftInfo());
	leftPart->addDockWidget(Qt::TopDockWidgetArea, tool->getLeftOverview());
	leftPart->addDockWidget(Qt::TopDockWidgetArea, tool->getLeftNearview());

	rightPart->setCentralWidget(rightDisplay);
	rightPart->statusBar()->addWidget(tool->getRightInfo());
	rightPart->addDockWidget(Qt::TopDockWidgetArea, tool->getRightNearview());
	rightPart->addDockWidget(Qt::TopDockWidgetArea, tool->getRightOverview());

	QHBoxLayout* layout = new QHBoxLayout(0);
	layout->addWidget(leftPart);
	layout->addWidget(rightPart);
	centerWidget->setLayout(layout);
	setCentralWidget(centerWidget);

	QRect desktop = QDesktopWidget().screenGeometry();
	move(-pos().x() - width()/2 + desktop.width()/2, -pos().y() - height()/2 + desktop.height()/2);
}

void SeparatedStereoViewer::closeEvent(QCloseEvent *)
{
	if (tool->showNearview->isChecked())
		tool->showNearview->trigger();
	if (tool->showOverview->isChecked())
		tool->showOverview->trigger();
	delete tool;
	delete leftDisplay;
	delete rightDisplay;
}

void SeparatedStereoViewer::loadLeftImage(QString filename)
{
	SingleScene* ss = (SingleScene*) leftDisplay->getCurrentScene();
	ss->loadImage(filename);
	//tool->setFitLeftView->trigger();
}

void SeparatedStereoViewer::loadRightImage(QString filename)
{
	SingleScene* ss = (SingleScene*) rightDisplay->getCurrentScene();
	ss->loadImage(filename);
	//tool->setFitRightView->trigger();
}

void SeparatedStereoViewer::loadLeftImage(QImage *image)
{
	SingleScene* ss = (SingleScene*) leftDisplay->getCurrentScene();
	ss->loadImage(*image);
	//tool->setFitLeftView->trigger();
}

void SeparatedStereoViewer::loadRightImage(QImage *image)
{
	SingleScene* ss = (SingleScene*) rightDisplay->getCurrentScene();
	ss->loadImage(*image);
	//tool->setFitRightView->trigger();
}

void SeparatedStereoViewer::loadLeftImage(Matrix *image, bool isGrayscale)
{
	SingleScene* ss = (SingleScene*) leftDisplay->getCurrentScene();
	ss->loadImage(image, isGrayscale);
	//tool->setFitLeftView->trigger();
}

void SeparatedStereoViewer::loadRightImage(Matrix *image, bool isGrayscale)
{
	SingleScene* ss = (SingleScene*) rightDisplay->getCurrentScene();
	ss->loadImage(image, isGrayscale);
	//tool->setFitRightView->trigger();
}

void SeparatedStereoViewer::setFeatures(Features *df)
{
	leftDisplay->getCurrentScene()->geometry()->setFeatures(df,0);
	rightDisplay->getCurrentScene()->geometry()->setFeatures(df,1);
}

void SeparatedStereoViewer::blockOpen()
{
	tool->setOpenVisible(false);
}

void SeparatedStereoViewer::blockSave()
{
	tool->setSaveVisible(false);
}

void SeparatedStereoViewer::blockMark()
{
	tool->setMarkVisible(false);
}




StereoViewer::StereoViewer(QWidget* parent) : QMainWindow(parent)
{
	stereoDisplay = new StereoDisplay(this);
	stereoTool = new StereoToolsBar(stereoDisplay, this);

	addToolBar(Qt::TopToolBarArea,stereoTool);

	statusBar()->addWidget(stereoTool->getStereoInfo());
	statusBar()->addWidget(stereoTool->getLeftInfo());
	statusBar()->addWidget(stereoTool->getRightInfo());

	setCentralWidget(stereoDisplay);

	QDockWidget* nearDock = stereoTool->getNearviews();
	addDockWidget(Qt::RightDockWidgetArea, nearDock, Qt::Horizontal);

	QDockWidget* overDock = stereoTool->getOverviews();
	addDockWidget(Qt::RightDockWidgetArea, overDock, Qt::Vertical);

	//QRect desktop = QDesktopWidget().screenGeometry();
	//move(-pos().x() - width()/2 + desktop.width()/2, -pos().y() - height()/2 + desktop.height()/2);
}

void StereoViewer::closeEvent(QCloseEvent *)
{
	//delete tool;
	//delete leftDisplay;
	//delete rightDisplay;
}

void StereoViewer::loadLeftImage(QString filename)
{
	stereoDisplay->loadLeftImage(filename);
}

void StereoViewer::loadRightImage(QString filename)
{
	stereoDisplay->loadRightImage(filename);
}

void StereoViewer::loadLeftImage(QImage *image)
{
	stereoDisplay->loadLeftImage(image);
}

void StereoViewer::loadRightImage(QImage *image)
{
	stereoDisplay->loadRightImage(image);
}

void StereoViewer::loadLeftImage(Matrix *image, bool isGrayscale)
{
	stereoDisplay->loadLeftImage(image,isGrayscale);
}

void StereoViewer::loadRightImage(Matrix *image, bool isGrayscale)
{
	stereoDisplay->loadRightImage(image,isGrayscale);
}

void StereoViewer::setFeatures(Features *df)
{
	stereoDisplay->getCurrentScene()->getLeftScene()->geometry()->setFeatures(df,0);
	stereoDisplay->getCurrentScene()->getRightScene()->geometry()->setFeatures(df,1);
}

void StereoViewer::blockOpen()
{
	stereoTool->setOpenVisible(false);
}

void StereoViewer::blockSave()
{
	stereoTool->setSaveVisible(false);
}

void StereoViewer::blockMark()
{
	stereoTool->setMarkVisible(false);
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
