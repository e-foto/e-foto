#ifndef ABSTRACTSWIDGET_H
#define ABSTRACTSWIDGET_H

#include <string>
#include <deque>
using namespace std;

#include "GLMethods.h"
#include "CommonMethods.h"
#include "SViewport.h"
#include "SImage.h"
#include "SMarker.h"
#include "SMark.h"

class SWidget : public GLMethods
{
public:
	bool			containsViewport(string nickname);
	bool			containsImage(string nickname);
	bool			containsPin(string nickname);
	bool			containsPoint(string nickname);

	int				countViewports();
	int				countImages();
	int				countPins();
	int				countPoints();

	bool			createViewport(string nickname);
	bool			createViewport(string nickname, double left, double bottom, double width, double height);
	bool			createViewport(string nickname, double left, int leftOffset, double bottom, int bottomOffset,
								   double width, int widthOffset, double height, int heightOffset);
	bool			createImage(string nickname, string filename);
	bool			createPin(string nickname, string filename);
	bool			createPoint(string nickname, string markerNickname);

	void			destroyViewport(string nickname);
	void			destroyImage(string nickname);
	void			destroyPin(string nickname);
	void			destroyPoint(string nickname);

	S::Viewport*	getViewport(string nickname);
	S::Image*		getImage(string nickname);
	S::Pin*			getPin(string nickname);
	S::Point*		getPoint(string nickname);

	S::Viewport*	getSelectedViewport();
	S::Image*		getSelectedImage();
	S::Pin*			getSelectedPin();
	S::Point*		getSelectedPoint();

	void			selectViewport(string nickname);
	void			selectImage(string nickname);
	void			selectPin(string nickname);
	void			selectPoint(string nickname);

	void			setZoomCursor();
	void			setMarkingCursor();
	void			setMoveCursor();
	void			setDirectionCursor();

	void pan(int dx, int dy);
	void zoom(double zoom);
	void zoom(double zoom, int x, int y);
	void zoomIn();
	void zoomIn(int x, int y);
	void zoomOut();
	void zoomOut(int x, int y);

	virtual int		getWidth() = 0;
	virtual int		getHeight() = 0;
	virtual void	paintGL() = 0;
	virtual void	repaint() = 0;

protected:

	deque<sclass::SViewport>	viewport;
	deque<sclass::SImage>		image;
	deque<sclass::SPin>			pin;
	deque<sclass::SPoint>		point;
	/*
 deque<sclass::SPen>			pen;
 deque<sclass::SBrush>		brush;
 deque<sclass::SPoint>		point;
 deque<sclass::SLine>		line;
 deque<sclass::SPolygon>		polygon;
 */

	S::Viewport*				selectedViewport;
	S::Image*					selectedImage;
	S::Pin*						selectedPin;
	S::Point*					selectedPoint;
	/*
 S::Pen*						selectedPen;
 S::Brush*					selectedBrush;
 S::Point*					selectedPoint;
 S::Line*					selectedLine;
 S::Polygon*					selectedPolygon;
 */

	int cursorType;
};

#endif // ABSTRACTSWIDGET_H
