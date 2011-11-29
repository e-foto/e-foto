#ifndef SINGLEDISPLAY_H
#define SINGLEDISPLAY_H

#include <QWidget>
#include <QList>

#include "SingleScene.h"
#include "SingleTools.h"

enum DisplayMode {IntermediatedScreen, TopViewScreen, MostDetailedScreen};

enum DefaultCursor {
NoCursor, RedCursor, GreenCursor, BlueCursor, CyanCursor, MagentaCursor, YellowCursor, DarkRedCursor, DarkGreenCursor, DarkBlueCursor, DarkCyanCursor, DarkMagentaCursor, DarkYellowCursor, BlackCursor, WhiteCursor, GrayCursor, BlackAndWhiteCursor
};

class AbstractScene;

class Cursor : public QCursor
{
public:
	Cursor(const QImage &image2Cursor, QPoint offset = QPoint(-1, -1));
	Cursor(DefaultCursor defaultCursorType);
        Cursor& noCursor();
	Cursor& toCross(QColor color, QSize size = QSize(24,24), unsigned int weigth = 2);
	Cursor& toCross(QColor colorBrush, QColor colorPen, QSize size = QSize(24,24), unsigned int weigth = 3);
	Cursor& toX(QColor color, QSize size = QSize(24,24), unsigned int weigth = 2);
	Cursor& toX(QColor colorBrush, QColor colorPen, QSize size = QSize(24,24), unsigned int weigth = 3);
	QCursor toQCursor();
	QImage toQImage();
};

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
	QPointF getLastMousePosition();
	QPointF getPosition(QPoint screenPosition);
	QPointF getMouseScreenPosition();

	void setCurrentScene(AbstractScene*);
	AbstractScene* getCurrentScene();

	virtual void fitView();
	virtual void pan(int dx, int dy);
	virtual void zoom(double zoomFactor, QPoint* atPoint = NULL);

	void setDetailZoom(double zoomFactor);
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
