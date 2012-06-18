#include "SingleViewer.h"
#include <QDesktopWidget>
#include <QStatusBar>

#include "SingleDisplay.h"
#include "SingleTools.h"
#include "Features.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

SingleViewer::SingleViewer()
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
    return QImage();
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



} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
