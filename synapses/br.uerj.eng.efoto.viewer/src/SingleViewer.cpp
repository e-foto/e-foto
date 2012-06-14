#include "SingleViewer.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {
namespace viewer {

 SingleViewer::SingleViewer()
{
}

SingleViewer::~SingleViewer()
{
}

void SingleViewer::startOpenImageDialog()
{
}

void SingleViewer::startSaveImageDialog()
{
}

void SingleViewer::markMode()
{
}

void SingleViewer::moveMode()
{
}

void SingleViewer::zoomInMode()
{
}

void SingleViewer::zoomOutMode()
{
}

void SingleViewer::hideOpen(bool status)
{
}

void SingleViewer::hideSave(bool status)
{
}

void SingleViewer::hideMark(bool status)
{
}

void SingleViewer::hideDetail(bool status)
{
}

void SingleViewer::hideOverView(bool status)
{
}

bool SingleViewer::loadImage(string filename)
{
}

void SingleViewer::loadImage(QImage* image)
{
}

QImage SingleViewer::getPrintScreen(bool rasterOnly = false)
{
}

void SingleViewer::insertMark(double x, double  y, unsigned int key, string label, QImage *marker = NULL, int hotX = -1, int hotY = -1)
{
}

unsigned int SingleViewer::addMark(double x, double  y, string label, QImage *marker, int hotX, int hotY)
{
}

void SingleViewer::deleteMark(unsigned int key)
{
}

void SingleViewer::setDefaultMarker(QImage marker, int hotX = -1, int hotY = -1)
{
}

void SingleViewer::setSelectedMarker(QImage marker, int hotX = -1, int hotY = -1)
{
}

void SingleViewer::setSelectedMark(unsigned int key = 0)
{
}

void SingleViewer::fit()
{
}

void SingleViewer::setDetailRelation(double zoom)
{
}

void SingleViewer::setDetailTracking(bool status)
{
}

void SingleViewer::setZoom(double zoom)
{
}

void SingleViewer::moveTo(double x, double y)
{
}


}
}
}
}
} //namespace
