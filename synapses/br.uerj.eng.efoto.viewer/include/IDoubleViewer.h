#ifndef BR_UERJ_ENG_EFOTO_VIEWER_IDOUBLEVIEWER_H
#define BR_UERJ_ENG_EFOTO_VIEWER_IDOUBLEVIEWER_H

#include <QSharedPointer>
#include <QImage>
using namespace std;

namespace br {
namespace uerj {
namespace eng {
namespace efoto {
namespace viewer {

enum TargetDisplay {LEFT_DISPLAY, RIGHT_DISPLAY};

class IDoubleViewer
{
public:
    virtual void startOpenLeftImageDialog() = 0;
    virtual void startOpenRightImageDialog() = 0;
    virtual void startSaveLeftImageDialog() = 0;
    virtual void startSaveRightImageDialog() = 0;
    virtual void markMode() = 0;
    virtual void moveMode() = 0;
    virtual void zoomInMode() = 0;
    virtual void zoomOutMode() = 0;
    virtual void hideOpen(bool status) = 0;
    virtual void hideSave(bool status) = 0;
    virtual void hideMark(bool status) = 0;
    virtual void hideDetail(bool status) = 0;
    virtual void hideOverView(bool status) = 0;
    virtual bool loadImage(TargetDisplay target, string filename) = 0;
    virtual void loadImage(TargetDisplay target, QImage* image) = 0;
    virtual QImage getPrintScreen(TargetDisplay target, bool rasterOnly = false) = 0;
    virtual void insertMark(TargetDisplay target, double x, double  y, unsigned int key, string label, QImage *marker = NULL, int hotX = -1, int hotY = -1) = 0;
    virtual unsigned int addMark(TargetDisplay target, double x, double  y, string label, QImage *marker, int hotX, int hotY) = 0;
    virtual void deleteMark(TargetDisplay target, unsigned int key) = 0;
    virtual void setDefaultMarker(TargetDisplay target, QImage marker, int hotX = -1, int hotY = -1) = 0;
    virtual void setSelectedMarker(TargetDisplay target, QImage marker, int hotX = -1, int hotY = -1) = 0;
    virtual void setSelectedMark(TargetDisplay target, unsigned int key = 0) = 0;
    virtual void fit(TargetDisplay target) = 0;
    virtual void setDetailRelation(double zoom) = 0;
    virtual void setDetailTracking(bool status) = 0;
    virtual void setEqualZoom(bool status) = 0;
    virtual void setEqualMove(bool status) = 0;
    virtual void setZoom(TargetDisplay target, double zoom) = 0;
    virtual void moveTo(TargetDisplay target, double x, double y) = 0;
};

typedef QSharedPointer<IDoubleViewer> IDoubleViewerPtr;

}
}
}
}
} //namespace


#endif // BR_UERJ_ENG_EFOTO_VIEWER_IDOUBLEVIEWER_H
