#ifndef VIEW_H
#define VIEW_H

#include <QObject>
#include <QRect>
#include "RasterResource.h"
#include "Geometry.h"

class MonoView: public QObject
{
	Q_OBJECT
protected:
	RasterResource* rasterRsrc_;
	GeometryResource geometryRsrc_;
	QPointF viewpoint_;
	double scale_;
	void limitScale();
public:
	MonoView(QObject* parent, QString filepath);
	~MonoView();
	QPointF getViewpoint();
	void moveTo(QPointF pos);
	void pan(QPointF dPos);
	void scaleTo(double newScale);
	void zoom(double zoomFactor);
	bool loadImage(QString filepath);
	bool imageLoaded();
	QSize imageSize();
	QImage getFrame(QSize targetSize);
	QImage getFrame(QSize targetSize, double scale);
	QImage getThumb(QSize targetSize, QRect* rect = NULL);
	QImage getDetail(QSize targetSize, QPointF point, double zoom);
	double getScale();

	GeometryResource* geometries();
};

class StereoView: public QObject
{
	Q_OBJECT
protected:
	MonoView* leftView_;
	MonoView* rightView_;
public:
	StereoView(QObject* parent, QString leftImageFilepath, QString rightImageFilepath);
	~StereoView();

	MonoView* getLeftView() const;
	MonoView* getRightView() const;

	void setLeftView(MonoView* leftView);
	void setRightView(MonoView* rightView);
};
inline MonoView* StereoView::getLeftView() const
{
	return leftView_;
}
inline MonoView* StereoView::getRightView() const
{
	return rightView_;
}

#endif // VIEW_H

/* IView interface
class IView : public IStructuredItem
{
  public:
  IView();
  void appendChild(IStructuredItem * child);
  virtual void add(const string & fileName) = 0;
};
*/
/* inline IView methods
inline IView::IView() :
IStructuredItem(VIEW)
{
  // Bouml preserved body begin 00027402
 QIcon icon;
 icon.addPixmap(QPixmap(":itemIcons/images/view_24x24.png"));

 setIcon(icon);
  // Bouml preserved body end 00027402
}
inline void IView::appendChild(IStructuredItem * child)
{
  // Bouml preserved body begin 00027602
 if(child->getItemType() == THEME)
  IStructuredItem::appendChild(child);
  // Bouml preserved body end 00027602
}
*/
/* TeView class
class Q_DECL_EXPORT Te3View : public IView
{
  public:
  Te3View(::TeView * view);
  ~Te3View();
  void add(const string & fileName);
  QString getLabel() const;
  void setLabel(const ::QString & label);
  int getId() const;
  void setId(int id);
  void* getTeObject();
  protected:
  bool isImage(const ::QString & fileName) const;
 ::TeView * view_; //!< Pointer to a view.
 static int themeId_; //!< Auxiliary variable for getting a valid theme id.
  private:
  Te3View(const Te3View & source);
  Te3View & operator=(const Te3View & source);
};
*/
