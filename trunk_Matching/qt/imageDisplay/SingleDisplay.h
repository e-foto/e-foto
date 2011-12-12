#ifndef SINGLEDISPLAY_H
#define SINGLEDISPLAY_H

#include <QWidget>
#include <QList>

#include "SingleScene.h"
#include "SingleTools.h"

enum DisplayMode {IntermediatedScreen, TopViewScreen, MostDetailedScreen};

class AbstractScene;

class SingleDisplay : public QWidget
{
	Q_OBJECT

protected:
	AbstractScene* _currentScene;
	DisplayMode _displayMode;
	SingleDisplay* _over;
	SingleDisplay* _detail;
	QList< SingleTool* > _tool;

	//bool _onMove;
	bool _cloneScale;
	QPointF _mouseLastPos;
	bool _onPainting;

	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *);
	bool eventFilter(QObject *o, QEvent *e);
	void enterEvent(QHoverEvent *e);
	void leaveEvent(QHoverEvent *e);
	void moveEvent(QHoverEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseDoubleClickEvent(QMouseEvent *e);
	void wheelEvent(QWheelEvent *e);

public:
	SingleDisplay(QWidget * parent = 0, AbstractScene* currentScene = NULL);
	~SingleDisplay();

	SingleDisplay* getOverDisplay();
	SingleDisplay* getDetailDisplay();

	void updateMousePosition();
	bool positionIsVisible(QPointF);
	QPointF getLastMousePosition();
	QPointF getPosition(QPoint screenPosition);
	QPointF getMouseScreenPosition();
	bool painting();

	void setCurrentScene(AbstractScene*);
	AbstractScene* getCurrentScene();

	virtual void fitView();
	virtual void pan(int dx, int dy);
	virtual void zoom(double zoomFactor, QPoint* atPoint = NULL);

	//void setDetailZoom(double zoomFactor);
	void updateAll();
	void updateDetail();
	void updateDetail(QPointF* newMousePosition, bool emitClicked = false);

	void setOverviewMode(SingleDisplay* display);
	void setDetailMode(SingleDisplay* display);
	void setActivatedTool(SingleTool* tool, bool active = true);

public slots:
	void setCloneScale(bool status);

signals:
	void mousePositionChanged(QPointF*);
	void mouseClicked(QPointF*);
};

#endif // SINGLEDISPLAY_H
