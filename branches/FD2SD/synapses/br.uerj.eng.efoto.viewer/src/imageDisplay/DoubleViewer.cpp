#include "DoubleViewer.h"
#include <QDesktopWidget>
#include <QStatusBar>

#include "Features.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

SeparatedStereoViewer::SeparatedStereoViewer(QWidget *parent)
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

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
