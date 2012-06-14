#ifndef BR_UERJ_ENG_EFOTO_VIEWER_ISINGLEVIEWER_H
#define BR_UERJ_ENG_EFOTO_VIEWER_ISINGLEVIEWER_H

#include <QSharedPointer>
#include <QImage>
using namespace std;

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

//class QImage;

namespace viewer {

class ISingleViewer
{
public:
    virtual void startOpenImageDialog() = 0;
    virtual void startSaveImageDialog() = 0;
    virtual void markMode() = 0;
    virtual void moveMode() = 0;
    virtual void zoomInMode() = 0;
    virtual void zoomOutMode() = 0;
    virtual void hideOpen(bool status) = 0;
    virtual void hideSave(bool status) = 0;
    virtual void hideMark(bool status) = 0;
    virtual void hideDetail(bool status) = 0;
    virtual void hideOverView(bool status) = 0;
    virtual bool loadImage(string filename) = 0;
    virtual void loadImage(QImage* image) = 0;
    virtual QImage getPrintScreen(bool rasterOnly = false) = 0;
    virtual void insertMark(double x, double  y, unsigned int key, string label, QImage *marker = NULL, int hotX = -1, int hotY = -1) = 0;
    virtual unsigned int addMark(double x, double  y, string label, QImage *marker, int hotX, int hotY) = 0;
    virtual void deleteMark(unsigned int key) = 0;
    virtual void setDefaultMarker(QImage marker, int hotX = -1, int hotY = -1) = 0;
    virtual void setSelectedMarker(QImage marker, int hotX = -1, int hotY = -1) = 0;
    virtual void setSelectedMark(unsigned int key = 0) = 0;
    virtual void fit() = 0;
    virtual void setDetailRelation(double zoom) = 0;
    virtual void setDetailTracking(bool status) = 0;
    virtual void setZoom(double zoom) = 0;
    virtual void moveTo(double x, double y) = 0;
};

typedef QSharedPointer<ISingleViewer> ISingleViewerPtr;

}
}
}
}
} //namespace

#endif // BR_UERJ_ENG_EFOTO_VIEWER_ISINGLEVIEWER_H
