#ifndef BR_UERJ_ENG_EFOTO_VIEWER_ISTEREOVIEWER_H
#define BR_UERJ_ENG_EFOTO_VIEWER_ISTEREOVIEWER_H

#include <QSharedPointer>
#include <QMainWindow>
#include <QImage>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Marker;
class StereoMarkerListener;
class StereoDisplay;
class StereoToolsBar;

enum TargetChannel {LEFT_CHANNEL, RIGHT_CHANNEL};

class IStereoViewer : public QMainWindow
{
	Q_OBJECT

public slots:

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

	virtual void startOpenLeftImageDialog() = 0;
	virtual void startOpenRightImageDialog() = 0;
	virtual void startSaveLeftImageDialog() = 0;
	virtual void startSaveRightImageDialog() = 0;

	virtual bool loadImage(TargetChannel target, QString filename) = 0;
	virtual void loadImage(TargetChannel target, QImage* image) = 0;

	virtual QImage getPrintScreen(TargetChannel target, bool rasterOnly = false) = 0;
	virtual void fit() = 0;
	virtual void setZoom(double zoom) = 0;
	virtual void setDiffScaleTo(TargetChannel target, double zoom) = 0;
	virtual void moveTo(TargetChannel target, double x, double y) = 0;
	virtual void insertMark(TargetChannel target, double x, double  y, unsigned int key, QString label, Marker *marker = NULL) = 0;
	virtual unsigned int addMark(TargetChannel target, double x, double  y, QString label, Marker *marker = NULL) = 0;
	virtual void deleteMark(TargetChannel target, unsigned int key) = 0;
	virtual void setSelectedMark(TargetChannel target, unsigned int key = 0) = 0;
	virtual void setSelectedMarker(QImage marker, int hotX = -1, int hotY = -1) = 0;
	virtual void setDefaultMarker(QImage marker, int hotX = -1, int hotY = -1) = 0;

	virtual void installListener(StereoMarkerListener* listener) = 0;
    virtual StereoDisplay* getDisplay() = 0;
    virtual StereoToolsBar* getToolBar() = 0;

};

typedef QSharedPointer<IStereoViewer> IStereoViewerPtr;

}
}
}
} //namespace

#endif // BR_UERJ_ENG_EFOTO_VIEWER_ISTEREOVIEWER_H
