#ifndef STEREOVIEWER_H
#define STEREOVIEWER_H

#include <QMainWindow>
#include "SingleDisplay.h"
#include "StereoDisplay.h"
#include "StereoTools.h"

#ifdef SYNAPSE_EFOTO
#include "IStereoViewer.h"
#endif //SYNAPSE_EFOTO

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

#ifndef STEREOMARKERLISTENER
#define STEREOMARKERLISTENER
class StereoMarkerListener
{
public:
	virtual void receiveStereoMark(QPointF, QPointF) = 0;
};
#endif //STEREOMARKERLISTENER

#ifdef INTEGRATED_EFOTO
class StereoViewer : public QMainWindow
#endif //INTEGRATED_EFOTO
#ifdef SYNAPSE_EFOTO
class Q_DECL_EXPORT StereoViewer : public IStereoViewer
#endif //SYNAPSE_EFOTO
{
	Q_OBJECT

	StereoDisplay* stereoDisplay;
	StereoToolsBar* stereoTool;
	void closeEvent(QCloseEvent *);

public:
	StereoViewer();
	~StereoViewer() {}
	StereoDisplay* getDisplay() {return stereoDisplay;}
	MarkStereoTool& getMarker() {return stereoTool->_mark;}
	StereoToolsBar* getToolBar() {return stereoTool;}

public slots:
	void loadLeftImage(QString filename);
	void loadRightImage(QString filename);
	void loadLeftImage(QImage* image);
	void loadRightImage(QImage* image);
	void loadLeftImage(Matrix* image, bool isGrayscale = true);
	void loadRightImage(Matrix* image, bool isGrayscale = true);
	void setFeatures(Features* df);
	void blockOpen();
	void blockSave();
	void blockMark();

	void markMode() {}
	void moveMode() {}
	void zoomInMode() {}
	void zoomOutMode() {}

	void setDetailRelation(double zoom) {}
	void setDetailTracking(bool status) {}

	void hideOpen(bool status) {}
	void hideSave(bool status) {}
	void hideMark(bool status) {}
	void hideDetail(bool status) {}
	void hideOverView(bool status) {}

	void startOpenLeftImageDialog() {}
	void startOpenRightImageDialog() {}
	void startSaveLeftImageDialog() {}
	void startSaveRightImageDialog() {}

	bool loadImage(TargetChannel target, QString filename) {}
	void loadImage(TargetChannel target, QImage* image) {}

	QImage getPrintScreen(TargetChannel target, bool rasterOnly = false) {}
	void fit() {}
	void setZoom(double zoom) {}
	void setDiffScaleTo(TargetChannel target, double zoom) {}
	void moveTo(TargetChannel target, double x, double y) {}
	void insertMark(TargetChannel target, double x, double  y, unsigned int key, QString label, Marker *marker = NULL) {}
	unsigned int addMark(TargetChannel target, double x, double  y, QString label, Marker *marker = NULL) {}
	void deleteMark(TargetChannel target, unsigned int key) {}
	void setSelectedMark(TargetChannel target, unsigned int key = 0) {}
	void setSelectedMarker(QImage marker, int hotX = -1, int hotY = -1) {}
	void setDefaultMarker(QImage marker, int hotX = -1, int hotY = -1) {}

	void installListener(StereoMarkerListener* listener) {}

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // STEREOVIEWER_H
