#ifndef IMAGEVIEWERS_H
#define IMAGEVIEWERS_H

#include <QMainWindow>
#include "SingleDisplay.h"
#include "StereoDisplay.h"
#include "StereoTools.h"

#ifdef SYNAPSE_EFOTO
#include "ISingleViewer.h"
#include "IDoubleViewer.h"
#include "IStereoViewer.h"
#endif //SYNAPSE_EFOTO

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

#ifdef INTEGRATED_EFOTO
class SingleViewer : public QMainWindow
#endif //INTEGRATED_EFOTO
#ifdef SYNAPSE_EFOTO
class Q_DECL_EXPORT SingleViewer : public QMainWindow, public ISingleViewer
#endif //SYNAPSE_EFOTO
{
	Q_OBJECT

	deque<MarkerListener*> listener;
	SingleDisplay* sd;
	SingleToolsBar* tool;
	void closeEvent(QCloseEvent *);

public:
	SingleViewer(QWidget *parent=0);
	SingleDisplay* getDisplay() {return sd;}
	//MarkTool* getMarker() {return &tool->mark;}

public slots:
	void markMode();
	void moveMode();
	void zoomInMode();
	void zoomOutMode();

	void setDetailRelation(double zoom);
	void setDetailTracking(bool status);

	void hideOpen(bool status);
	void hideSave(bool status);
	void hideMark(bool status);
	void hideDetail(bool status);
	void hideOverView(bool status);

	void startOpenImageDialog();
	void startSaveImageDialog();

	bool loadImage(QString filename);
	void loadImage(QImage* image);
	void loadImage(Matrix* image, bool isGrayscale = true);

	QImage getPrintScreen(bool rasterOnly = false);
	void fit();
	void setZoom(double zoom);
	void moveTo(double x, double y);
	void insertMark(double x, double  y, unsigned int key, QString label, Marker *marker = NULL);
	unsigned int addMark(double x, double  y, QString label, Marker *marker = NULL);
	void deleteMark(unsigned int key);
	void setSelectedMark(unsigned int key = 0);
	void setSelectedMarker(QImage marker, int hotX = -1, int hotY = -1);
	void setDefaultMarker(QImage marker, int hotX = -1, int hotY = -1);

	void setImageMode();
	void setOrtoImageMode(double xi, double dx, double yi, double dy);
	void setElevationImageMode(double xi, double dx, double yi, double dy, double zi, double dz);

	void installListener(MarkerListener *newListener) {listener.push_front(newListener);}
	void emitMark(QPointF p) {for (int i = 0; i < listener.size(); i++) listener.at(i)->receiveMark(p);}

};

#ifdef INTEGRATED_EFOTO
class SeparatedStereoViewer : public QMainWindow
#endif //INTEGRATED_EFOTO
#ifdef SYNAPSE_EFOTO
class Q_DECL_EXPORT SeparatedStereoViewer : public QMainWindow, public IDoubleViewer
#endif //SYNAPSE_EFOTO
{
	Q_OBJECT

	SingleDisplay* leftDisplay;
	SingleDisplay* rightDisplay;
	SeparatedStereoToolsBar* tool;
	//SingleToolsBar* leftTool;
	//SingleToolsBar* rightTool;
	void closeEvent(QCloseEvent *);

public:
	SeparatedStereoViewer(QWidget *parent=0);
	SingleDisplay* getLeftDisplay() {return leftDisplay;}
	SingleDisplay* getRightDisplay() {return rightDisplay;}
	MarkTool& getLeftMarker() {return (tool->leftMark);}
	MarkTool& getRightMarker() {return (tool->rightMark);}
	SeparatedStereoToolsBar* getToolBar() {return tool;}

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
	void setEqualZoom(bool status) {}
	void setEqualMove(bool status) {}

	void hideOpen(bool status) {}
	void hideSave(bool status) {}
	void hideMark(bool status) {}
	void hideDetail(bool status) {}
	void hideOverView(bool status) {}

	void startOpenLeftImageDialog() {}
	void startOpenRightImageDialog() {}
	void startSaveLeftImageDialog() {}
	void startSaveRightImageDialog() {}

	bool loadImage(TargetDisplay target, QString filename) {}
	void loadImage(TargetDisplay target, QImage* image) {}

	QImage getPrintScreen(TargetDisplay target, bool rasterOnly = false) {}
	void fit(TargetDisplay target) {}
	void setZoom(TargetDisplay target, double zoom) {}
	void moveTo(TargetDisplay target, double x, double y) {}
	void insertMark(TargetDisplay target, double x, double  y, unsigned int key, QString label, Marker *marker = NULL) {}
	unsigned int addMark(TargetDisplay target, double x, double  y, QString label, Marker *marker = NULL) {}
	void deleteMark(TargetDisplay target, unsigned int key) {}
	void setSelectedMark(TargetDisplay target, unsigned int key = 0) {}
	void setSelectedMarker(TargetDisplay target, QImage marker, int hotX = -1, int hotY = -1) {}
	void setDefaultMarker(TargetDisplay target, QImage marker, int hotX = -1, int hotY = -1) {}

	void installListener(DoubleMarkerListener* listener) {}

};

#ifdef INTEGRATED_EFOTO
class StereoViewer : public QMainWindow
#endif //INTEGRATED_EFOTO
#ifdef SYNAPSE_EFOTO
class Q_DECL_EXPORT StereoViewer : public QMainWindow, public IStereoViewer
#endif //SYNAPSE_EFOTO
{
	Q_OBJECT

	StereoDisplay* stereoDisplay;
	StereoToolsBar* stereoTool;
	void closeEvent(QCloseEvent *);

public:
	StereoViewer(QWidget *parent=0);
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

#endif // IMAGEVIEWERS_H
