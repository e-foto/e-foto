#include "StereoViewer.h"
#include <QDesktopWidget>
#include <QStatusBar>

#include "Features.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

StereoViewer::StereoViewer(QWidget *parent)
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
