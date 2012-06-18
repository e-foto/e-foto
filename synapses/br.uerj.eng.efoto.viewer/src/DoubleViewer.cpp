#include "DoubleViewer.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {
namespace viewer {

DoubleViewer::DoubleViewer()
{

}

DoubleViewer::~DoubleViewer()
{

}

void DoubleViewer::startOpenLeftImageDialog()
{

}

void DoubleViewer::startOpenRightImageDialog()
{

}

void DoubleViewer::startSaveLeftImageDialog()
{

}

void DoubleViewer::startSaveRightImageDialog()
{

}

void DoubleViewer::markMode()
{

}

void DoubleViewer::moveMode()
{

}

void DoubleViewer::zoomInMode()
{

}

void DoubleViewer::zoomOutMode()
{

}

void DoubleViewer::hideOpen(bool status)
{

}

void DoubleViewer::hideSave(bool status)
{

}

void DoubleViewer::hideMark(bool status)
{

}

void DoubleViewer::hideDetail(bool status)
{

}

void DoubleViewer::hideOverView(bool status)
{

}

bool DoubleViewer::loadImage(TargetDisplay target, string filename)
{

}

void DoubleViewer::loadImage(TargetDisplay target, QImage* image)
{

}

QImage DoubleViewer::getPrintScreen(TargetDisplay target, bool rasterOnly)
{

}

void DoubleViewer::insertMark(TargetDisplay target, double x, double  y, unsigned int key, string label, QImage *marker, int hotX, int hotY)
{

}

unsigned int DoubleViewer::addMark(TargetDisplay target, double x, double  y, string label, QImage *marker, int hotX, int hotY)
{

}

void DoubleViewer::deleteMark(TargetDisplay target, unsigned int key)
{

}

void DoubleViewer::setDefaultMarker(TargetDisplay target, QImage marker, int hotX, int hotY)
{

}

void DoubleViewer::setSelectedMarker(TargetDisplay target, QImage marker, int hotX, int hotY)
{

}

void DoubleViewer::setSelectedMark(TargetDisplay target, unsigned int key)
{

}

void DoubleViewer::fit(TargetDisplay target)
{

}

void DoubleViewer::setDetailRelation(double zoom)
{

}

void DoubleViewer::setDetailTracking(bool status)
{

}

void DoubleViewer::setEqualZoom(bool status)
{

}

void DoubleViewer::setEqualMove(bool status)
{

}

void DoubleViewer::setZoom(TargetDisplay target, double zoom)
{

}

void DoubleViewer::moveTo(TargetDisplay target, double x, double y)
{

}


}
}
}
}
} //namespace
