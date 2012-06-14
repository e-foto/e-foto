#ifndef BR_UERJ_ENG_EFOTO_VIEWER_SINGLEVIEWER_H
#define BR_UERJ_ENG_EFOTO_VIEWER_SINGLEVIEWER_H

#include "ISingleViewer.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {
namespace viewer {

class SingleViewer
{
public:
    SingleViewer();
    ~SingleViewer();
    void startOpenImageDialog();
    void startSaveImageDialog();
    void markMode();
    void moveMode();
    void zoomInMode();
    void zoomOutMode();
    void hideOpen(bool status);
    void hideSave(bool status);
    void hideMark(bool status);
    void hideDetail(bool status);
    void hideOverView(bool status);
    bool loadImage(string filename);
    void loadImage(QImage* image);
    QImage getPrintScreen(bool rasterOnly = false);
    void insertMark(double x, double  y, unsigned int key, string label, QImage *marker = NULL, int hotX = -1, int hotY = -1);
    unsigned int addMark(double x, double  y, string label, QImage *marker, int hotX, int hotY);
    void deleteMark(unsigned int key);
    void setDefaultMarker(QImage marker, int hotX = -1, int hotY = -1);
    void setSelectedMarker(QImage marker, int hotX = -1, int hotY = -1);
    void setSelectedMark(unsigned int key = 0);
    void fit();
    void setDetailRelation(double zoom);
    void setDetailTracking(bool status);
    void setZoom(double zoom);
    void moveTo(double x, double y);


};

}
}
}
}
} //namespace

#endif // BR_UERJ_ENG_EFOTO_VIEWER_SINGLEVIEWER_H
