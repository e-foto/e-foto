#include "ImageViewers.h"
#include <QDesktopWidget>

SingleViewer::SingleViewer(QWidget* parent) : QMainWindow(parent)
{
	setupUi(this);
	sd = new SingleDisplay(this);
	setCentralWidget(sd);
	tool = new SingleToolsBar(sd, this);
	addToolBar(Qt::TopToolBarArea,tool);
	statusBar()->addWidget(tool->getInfo());
	addDockWidget(Qt::RightDockWidgetArea, tool->getNearview());
	addDockWidget(Qt::RightDockWidgetArea, tool->getOverview());

	QRect desktop = QDesktopWidget().screenGeometry();
	move(-pos().x() - width()/2 + desktop.width()/2, -pos().y() - height()/2 + desktop.height()/2);
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

void SingleViewer::loadImage(QString filename)
{
	SingleScene* ss = (SingleScene*) sd->getCurrentScene();
	ss->loadImage(filename);
	tool->setFitView->trigger();
}

void SingleViewer::loadImage(QImage *image)
{
	SingleScene* ss = (SingleScene*) sd->getCurrentScene();
	ss->loadImage(*image);
	tool->setFitView->trigger();
}

void SingleViewer::loadImage(Matrix *image, bool isGrayscale)
{
	SingleScene* ss = (SingleScene*) sd->getCurrentScene();
	ss->loadImage(image, isGrayscale);
	tool->setFitView->trigger();
}

void SingleViewer::blockOpen()
{
	tool->setOpenVisible(false);
}

void SingleViewer::blockSave()
{
	tool->setSaveVisible(false);
}

void SingleViewer::blockMark()
{
	tool->setMarkVisible(false);
	tool->mark.setToOnlyEmitClickedMode();
        tool->near_.setNearCursor(QCursor(Qt::ArrowCursor));
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
	addToolBar(Qt::TopToolBarArea,tool);

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

	leftDisplay = stereoDisplay->getLeftDisplay();
	leftTool = new SingleToolsBar(leftDisplay, this);

	rightDisplay = stereoDisplay->getRightDisplay();
	rightTool = new SingleToolsBar(rightDisplay, this);

	addToolBar(Qt::TopToolBarArea,stereoTool);
	addToolBar(Qt::TopToolBarArea,leftTool);
	addToolBar(Qt::TopToolBarArea,rightTool);

	setCentralWidget(stereoDisplay);
	addDockWidget(Qt::RightDockWidgetArea, leftTool->getNearview(), Qt::Horizontal);
	addDockWidget(Qt::RightDockWidgetArea, rightTool->getNearview(), Qt::Horizontal);

	QDockWidget* overDock = new QDockWidget("Overviewers",this);
	QDockWidget* leftOver = leftTool->getOverview();
	QDockWidget* rightOver = rightTool->getOverview();
	QWidget* overviewers = new QWidget(this);
	QHBoxLayout* overLayout = new QHBoxLayout(0);
	overLayout->addWidget(leftOver);
	overLayout->addWidget(rightOver);
	overviewers->setLayout(overLayout);
	overDock->setWidget(overviewers);
	addDockWidget(Qt::RightDockWidgetArea, overDock, Qt::Vertical);

	leftDisplay->setHidden(true);
	rightDisplay->setHidden(true);

	QRect desktop = QDesktopWidget().screenGeometry();
	move(-pos().x() - width()/2 + desktop.width()/2, -pos().y() - height()/2 + desktop.height()/2);
}

void StereoViewer::closeEvent(QCloseEvent *)
{
	//delete tool;
	//delete leftDisplay;
	//delete rightDisplay;
}

void StereoViewer::loadLeftImage(QString filename)
{
	SingleScene* ss = (SingleScene*) leftDisplay->getCurrentScene();
	ss->loadImage(filename);
	leftTool->setFitView->trigger();
}

void StereoViewer::loadRightImage(QString filename)
{
	SingleScene* ss = (SingleScene*) rightDisplay->getCurrentScene();
	ss->loadImage(filename);
	rightTool->setFitView->trigger();
}

void StereoViewer::loadLeftImage(QImage *image)
{
	SingleScene* ss = (SingleScene*) leftDisplay->getCurrentScene();
	ss->loadImage(*image);
	leftTool->setFitView->trigger();
}

void StereoViewer::loadRightImage(QImage *image)
{
	SingleScene* ss = (SingleScene*) rightDisplay->getCurrentScene();
	ss->loadImage(*image);
	rightTool->setFitView->trigger();
}

void StereoViewer::loadLeftImage(Matrix *image, bool isGrayscale)
{
	SingleScene* ss = (SingleScene*) leftDisplay->getCurrentScene();
	ss->loadImage(image, isGrayscale);
	leftTool->setFitView->trigger();
}

void StereoViewer::loadRightImage(Matrix *image, bool isGrayscale)
{
	SingleScene* ss = (SingleScene*) rightDisplay->getCurrentScene();
	ss->loadImage(image, isGrayscale);
	rightTool->setFitView->trigger();
}

void StereoViewer::blockOpen()
{
	//tool->setOpenVisible(false);
}

void StereoViewer::blockSave()
{
	//tool->setSaveVisible(false);
}

void StereoViewer::blockMark()
{
	//tool->setMarkVisible(false);
}
