#ifndef BR_UERJ_ENG_EFOTO_VIEWER_DOUBLEVIEWER_H
#define BR_UERJ_ENG_EFOTO_VIEWER_DOUBLEVIEWER_H

#include "IDoubleViewer.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {
namespace viewer {

class DoubleViewer
{
public:
    DoubleViewer();
    ~DoubleViewer();
    void startOpenLeftImageDialog();
    void startOpenRightImageDialog();
    void startSaveLeftImageDialog();
    void startSaveRightImageDialog();
    void markMode();
    void moveMode();
    void zoomInMode();
    void zoomOutMode();
    void hideOpen(bool status);
    void hideSave(bool status);
    void hideMark(bool status);
    void hideDetail(bool status);
    void hideOverView(bool status);
    bool loadImage(TargetDisplay target, string filename);
    void loadImage(TargetDisplay target, QImage* image);
    QImage getPrintScreen(TargetDisplay target, bool rasterOnly = false);
    void insertMark(TargetDisplay target, double x, double  y, unsigned int key, string label, QImage *marker = NULL, int hotX = -1, int hotY = -1);
    unsigned int addMark(TargetDisplay target, double x, double  y, string label, QImage *marker, int hotX, int hotY);
    void deleteMark(TargetDisplay target, unsigned int key);
    void setDefaultMarker(TargetDisplay target, QImage marker, int hotX = -1, int hotY = -1);
    void setSelectedMarker(TargetDisplay target, QImage marker, int hotX = -1, int hotY = -1);
    void setSelectedMark(TargetDisplay target, unsigned int key = 0);
    void fit(TargetDisplay target);
    void setDetailRelation(double zoom);
    void setDetailTracking(bool status);
    void setEqualZoom(bool status);
    void setEqualMove(bool status);
    void setZoom(TargetDisplay target, double zoom);
    void moveTo(TargetDisplay target, double x, double y);
};

}
}
}
}
} //namespace

#endif // BR_UERJ_ENG_EFOTO_VIEWER_DOUBLEVIEWER_H
