#ifndef DOUBLEVIEWER_H
#define DOUBLEVIEWER_H

#include <QMainWindow>
#include "SingleDisplay.h"
#include "StereoDisplay.h"
#include "StereoTools.h"

#ifdef SYNAPSE_EFOTO
#include "IDoubleViewer.h"
#endif //SYNAPSE_EFOTO

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

#ifdef INTEGRATED_EFOTO
enum TargetDisplay {LEFT_DISPLAY, RIGHT_DISPLAY};
#endif //INTEGRATED_EFOTO

class DoubleToolsBar : public SeparatedStereoToolsBar
{
    Q_OBJECT
};

class DoubleMarkerListener
{
public:
	virtual void receiveLeftMark(QPointF) = 0;
	virtual void receiveRightMark(QPointF) = 0;
};

#ifdef INTEGRATED_EFOTO
class SeparatedStereoViewer : public QMainWindow
#endif //INTEGRATED_EFOTO
#ifdef SYNAPSE_EFOTO
class Q_DECL_EXPORT SeparatedStereoViewer : public IDoubleViewer
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
    SeparatedStereoViewer(QWidget* parent = 0);
    ~SeparatedStereoViewer() {}
	SingleDisplay* getLeftDisplay() {return leftDisplay;}
    SingleDisplay* getRightDisplay() {return rightDisplay;}
    DoubleToolsBar* getToolBar() {return (DoubleToolsBar*) tool;}
    //MarkTool& getLeftMarker() {return (tool->leftMark);}
    //MarkTool& getRightMarker() {return (tool->rightMark);}

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

    bool loadImage(TargetDisplay target, QString filename) {if (LEFT_DISPLAY == target) loadLeftImage(filename);if (RIGHT_DISPLAY == target) loadRightImage(filename);}
    void loadImage(TargetDisplay target, QImage* image) {if (LEFT_DISPLAY == target) loadLeftImage(image);if (RIGHT_DISPLAY == target) loadRightImage(image);}

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

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // DOUBLEVIEWER_H
