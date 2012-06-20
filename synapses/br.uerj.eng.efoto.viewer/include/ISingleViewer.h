#ifndef BR_UERJ_ENG_EFOTO_VIEWER_ISINGLEVIEWER_H
#define BR_UERJ_ENG_EFOTO_VIEWER_ISINGLEVIEWER_H

#include <QSharedPointer>
#include <QMainWindow>
#include <QImage>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Marker;
class MarkerListener;
class SingleDisplay;
class SingleToolsBar;

class ISingleViewer : public QMainWindow
{
	Q_OBJECT

public:
	virtual void markMode() = 0;
	virtual void moveMode() = 0;
	virtual void zoomInMode() = 0;
	virtual void zoomOutMode() = 0;

	virtual void setDetailRelation(double zoom) = 0;
	virtual void setDetailTracking(bool status) = 0;

	virtual void hideOpen(bool status) = 0;
	virtual void hideSave(bool status) = 0;
	virtual void hideMark(bool status) = 0;
	virtual void hideDetail(bool status) = 0;
	virtual void hideOverView(bool status) = 0;

	virtual void startOpenImageDialog() = 0;
	virtual void startSaveImageDialog() = 0;

	virtual bool loadImage(QString filename) = 0;
	virtual void loadImage(QImage* image) = 0;

	virtual QImage getPrintScreen(bool rasterOnly = false) = 0;
	virtual void fit() = 0;
	virtual void setZoom(double zoom) = 0;
	virtual void moveTo(double x, double y) = 0;
	virtual void insertMark(double x, double  y, unsigned int key, QString label, Marker *marker = NULL) = 0;
	virtual unsigned int addMark(double x, double  y, QString label, Marker *marker = NULL) = 0;
	virtual void deleteMark(unsigned int key) = 0;
	virtual void setSelectedMark(unsigned int key = 0) = 0;
    virtual void setSelectedMarker(Marker* marker) = 0;
    virtual void setDefaultMarker(Marker* marker) = 0;

	virtual void setImageMode() = 0;
	virtual void setOrtoImageMode(double xi, double dx, double yi, double dy) = 0;
	virtual void setElevationImageMode(double xi, double dx, double yi, double dy, double zi, double dz) = 0;

    virtual void installListener(MarkerListener* listener) = 0;
    virtual SingleDisplay* getDisplay() = 0;
    virtual SingleToolsBar* getToolBar() = 0;
};

typedef QSharedPointer<ISingleViewer> ISingleViewerPtr;


}
}
}
} //namespace

#endif // BR_UERJ_ENG_EFOTO_VIEWER_ISINGLEVIEWER_H
