#ifndef SWIDGETQT_H
#define SWIDGETQT_H

#include <QGLWidget>
#include <QMouseEvent>

#include "AbstractSWidget.h"

class SWidgetQt : public SWidget, public QGLWidget
{
public:
	SWidgetQt(QWidget* parent = 0);

	// Implementation to abstract methods
	int		getWidth();
	int		getHeight();
	void	setWidth(int w);
	void	setHeight(int h);
	void	resize(int w, int h);
	void	paintGL();
	void	repaint();

	// Event methods
	void mouseMoveEvent(QMouseEvent* event );
	void mousePressEvent(QMouseEvent* event );
	void mouseReleaseEvent (QMouseEvent* event);
	void wheelEvent(QWheelEvent* event);
	void resizeEvent(QResizeEvent* event);
protected:
	QPoint moveStartPos;
	bool onMove;
};

#endif // SWIDGETQT_H
