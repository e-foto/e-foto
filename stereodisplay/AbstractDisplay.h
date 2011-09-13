#ifndef ABSTRACTDISPLAY_H
#define ABSTRACTDISPLAY_H

#include <QWidget>
#include "View.h"

class AbstractDisplay : public QWidget
{
protected:
	AbstractDisplay(QWidget* parent);
	~AbstractDisplay();

public:
	/*
	virtual void setActive(bool active) = 0;
	virtual void updateVisibleArea() = 0;
	//void addListener(IRendererListener * listen);
	//void removeListener(IRendererListener * listen);
	*/

	//virtual void drawView(IView* view) = 0;
	//virtual void fitView() = 0;
	virtual void pan(int dx, int dy) = 0;
	virtual void zoom(double zoomFactor, QPoint* atPoint = NULL) = 0;

	/*
	virtual void usePanTool() = 0;
	virtual void useZoomInTool() = 0;
	virtual void useZoomOutTool() = 0;
	//virtual void setExternalTool(IQMouseListener * listener) = 0;

	//void drawPoints(const vector<QPoint> & pointVec, const ::QPixmap & img);
	*/
};

#endif // ABSTRACTDISPLAY_H

/* IMapDisplay interface
class IMapViewer : public ::QWidget
{
  public:
	 IMapViewer(::QWidget * parent);
	 virtual ~IMapViewer();
	 virtual void drawView(IView * view) = 0;
	 virtual void fitView() = 0;
	 virtual void useZoomInTool() = 0;
	 virtual void useZoomOutTool() = 0;
	 virtual void usePanTool() = 0;
	 virtual void setExternalTool(IQMouseListener * listener) = 0;
	 virtual void setActive(bool active) = 0;
	 virtual void updateVisibleArea() = 0;
	 void addListener(IRendererListener * listen);
	 void removeListener(IRendererListener * listen);
	 void drawPoints(const vector<QPoint> & pointVec, const ::QPixmap & img);
	 virtual void drawPoint(const ::QPoint & pt, const ::QPixmap & img) = 0;
	 virtual void drawPoint(const ::QPointF & pt, const ::QPixmap & img) = 0;
  protected:
	vector<IRendererListener *> rListeners_; //!< Drawing operations listeners.
  private:
	 IMapViewer(const IMapViewer & source);
	 IMapViewer & operator=(const IMapViewer & source);
};
*/
/* inline IMpaViewer methods
inline IMapViewer::IMapViewer(::QWidget * parent) :
QWidget(parent)
{
  // Bouml preserved body begin 00026F02
  // Bouml preserved body end 00026F02
}
inline IMapViewer::~IMapViewer()
{
  // Bouml preserved body begin 00026F82
  // Bouml preserved body end 00026F82
}
inline void IMapViewer::addListener(IRendererListener * listen)
{
  // Bouml preserved body begin 00040917
	rListeners_.push_back(listen);
  // Bouml preserved body end 00040917
}
inline void IMapViewer::removeListener(IRendererListener * listen)
{
  // Bouml preserved body begin 00040997
	vector<IRendererListener*>::iterator it = std::find(rListeners_.begin(), rListeners_.end(), listen);

	if(it != rListeners_.end())
		rListeners_.erase(it);
  // Bouml preserved body end 00040997
}
inline void IMapViewer::drawPoints(const vector<QPoint> & pointVec, const ::QPixmap & img)
{
  // Bouml preserved body begin 00036F97
	vector<QPoint>::const_iterator it;

	for(it = pointVec.begin(); it != pointVec.end(); ++it)
		drawPoint(*it, img);
  // Bouml preserved body end 00036F97
}
*/
/* TeMapViewer class
class Q_DECL_EXPORT TeMapViewer : public IMapViewer, public ::TdkEventHandler
{
  public:
	 TeMapViewer(::QWidget * parent);
	 ~TeMapViewer();
	 void drawView(IView * view);
	 void fitView();
	 void useZoomInTool();
	 void useZoomOutTool();
	 void usePanTool();
	 void setExternalTool(IQMouseListener * listener);
	 void setActive(bool active);
	 virtual void updateVisibleArea();
	 void drawPoint(const ::QPoint & pt, const ::QPixmap & img);
	 virtual void drawPoint(const ::QPointF & pt, const ::QPixmap & img);
  public slots:
	 void updateCursor(::QCursor cursor);
  protected:
	 void enterEvent(::QEvent * e);
	 void leaveEvent(::QEvent * e);
	 void keyPressEvent(::QKeyEvent * e);
	 void keyReleaseEvent(::QKeyEvent * e);
	 void mouseDoubleClickEvent(::QMouseEvent * e);
	 void mouseMoveEvent(::QMouseEvent * e);
	 void mousePressEvent(::QMouseEvent * e);
	 void mouseReleaseEvent(::QMouseEven
	 void paintEvent(::QPaintEvent * e);
	 void resizeEvent(::QResizeEvent * e);
	 void focusInEvent(::QFocusEvent * e);
	 void beginDrawView(IView * view);
	 void endDrawView();
	 void handleEvent(::TdkEvent * event);
	::TdkQt4Canvas * canvas_; //!< TDK canvas draw.
	::TdkDisplay * display_; //!< TDK display.
	::TdkQt4CanvasDispatcher * dispatcher_; //!< Turns a Qt event into a TDK event.
	bool btnPressed_; //!< Flag telling if a button was pressed.
};
*/
