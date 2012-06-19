#ifndef SINGLEVIEWER_H
#define SINGLEVIEWER_H

#include <QMainWindow>
#include <deque>
using namespace std;

#ifdef SYNAPSE_EFOTO
#include "ISingleViewer.h"
#endif //SYNAPSE_EFOTO

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class SingleDisplay;
class SingleToolsBar;
class Matrix;
class Marker;

class MarkerListener
{
public:
	virtual void receiveMark(QPointF) = 0;
};

#ifdef INTEGRATED_EFOTO
class SingleViewer : public QMainWindow
#endif //INTEGRATED_EFOTO
#ifdef SYNAPSE_EFOTO
class Q_DECL_EXPORT SingleViewer : public ISingleViewer
#endif //SYNAPSE_EFOTO
{
	Q_OBJECT

	deque<MarkerListener*> listener;
	SingleDisplay* sd;
	SingleToolsBar* tool;
	void closeEvent(QCloseEvent *);

public:
    SingleViewer();
    //SingleViewer(QWidget* parent);
    ~SingleViewer() {}
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
    void setSelectedMarker(Marker *marker);
    void setDefaultMarker(Marker* marker);

	void setImageMode();
	void setOrtoImageMode(double xi, double dx, double yi, double dy);
	void setElevationImageMode(double xi, double dx, double yi, double dy, double zi, double dz);

	void installListener(MarkerListener *newListener) {listener.push_front(newListener);}
	void emitMark(QPointF p) {for (int i = 0; i < listener.size(); i++) listener.at(i)->receiveMark(p);}

};


} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // SINGLEVIEWER_H
