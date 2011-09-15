#ifndef STEREODISPLAY_H
#define STEREODISPLAY_H

#include <QGLWidget>
#include <QGridLayout>

#include "AbstractDisplay.h"

class DetailDisplay;
class OverviewDisplay;
class StereoDisplay;

class MonoDisplay : public AbstractDisplay
{
	Q_OBJECT
protected:
	MonoView* currentView_;
	OverviewDisplay* overview_;
	DetailDisplay* detail_;
	StereoDisplay* parentDisplay_;
	bool onMove_;
	bool cloneScale_;
	QPointF moveLastPos_;
	QImage frame_;
	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void wheelEvent(QWheelEvent *e);
public:
	MonoDisplay(QWidget * parent, MonoView* currentView = NULL);
	MonoDisplay(StereoDisplay* parent, MonoView* currentView);
	~MonoDisplay();
	QPointF getLastMousePosition();
	MonoView* getCurrentView();
	OverviewDisplay* getOverviewDisplay();
	DetailDisplay* getDetailDisplay();
	virtual void fitView();
	virtual void pan(int dx, int dy);
	virtual void zoom(double zoomFactor, QPoint* atPoint = NULL);
	void setDetailZoom(double zoomFactor);
	void updateAll();
	void updateDetail();
	void updateDetail(QPointF* newMousePosition, bool emitClicked = false);
public slots:
	void setCloneScale(bool status);
signals:
	void mousePositionChanged(QPointF*);
	void mouseClicked(QPointF*);
};

class DetailDisplay : public AbstractDisplay
{
protected:
	MonoDisplay* parentDisplay_;
	double zoom_;
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void wheelEvent(QWheelEvent *e);
public:
	DetailDisplay(MonoDisplay* parent);
	~DetailDisplay();
	virtual void fitView();
	virtual void pan(int dx, int dy);
	virtual void zoom(double zoomFactor, QPoint* atPoint = NULL);
};

class OverviewDisplay : public AbstractDisplay
{
protected:
	MonoDisplay* parentDisplay_;
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void wheelEvent(QWheelEvent *e);
public:
	OverviewDisplay(MonoDisplay* parent);
	~OverviewDisplay();
	virtual void fitView();
	virtual void pan(int dx, int dy);
	virtual void zoom(double zoomFactor, QPoint* atPoint = NULL);
};

class GLDisplay : public QGLWidget
{
protected:
	StereoDisplay* stereoDisplay_;
	unsigned int ltexture;
	unsigned int rtexture;
	unsigned int ctexture;
	QImage ltext;
	QImage rtext;
	bool onMove_;
	bool hasChanged_;
	QPointF moveLastPos_;
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void wheelEvent(QWheelEvent *e);
public:
	GLDisplay(StereoDisplay* parent);
	~GLDisplay();
};

class StereoDisplay : public AbstractDisplay
{
	Q_OBJECT
protected:
	StereoView* currentView_;
	MonoDisplay* leftDisplay_;
	MonoDisplay* rightDisplay_;
	GLDisplay* glDisplay_;
	QPointF leftCursorOffset_;
	QPointF rightCursorOffset_;
public:
	StereoDisplay(QWidget * parent, StereoView* currentView = NULL);
	~StereoDisplay();
	StereoView* getCurrentView();
	MonoDisplay* getLeftDisplay();
	MonoDisplay* getRightDisplay();
	QPointF getLeftCursorOffset();
	QPointF getRightCursorOffset();
	void setLeftCursorOffset(QPointF offset);
	void setRightCursorOffset(QPointF offset);
	virtual void fitView();
	virtual void pan(int dx, int dy);
	virtual void zoom(double zoomFactor, QPoint* atPoint = NULL);
	void updateAll();
	void updateAll(QPointF* left, QPointF* right, bool emitClicked = false);
	void updateDetail(QPointF* left, QPointF* right, bool emitClicked = false);
signals:
	void mousePositionsChanged(QPointF*, QPointF*);
	void mouseClicked(QPointF*, QPointF*);
};

#endif // STEREODISPLAY_H
