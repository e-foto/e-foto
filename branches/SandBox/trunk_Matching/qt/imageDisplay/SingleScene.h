#ifndef SINGLESCENE_H
#define SINGLESCENE_H

#include <QObject>
#include <QRect>

#include "RasterResource.h"
#include "GeometryResource.h"

class Matrix;

class AbstractScene: public QObject
{
protected:
	AbstractScene(QObject* parent) : QObject(parent) {}

public:
	virtual void centerContent() = 0;
	virtual void moveTo(QPointF pos) = 0;
	virtual void pan(QPointF dPos) = 0;
	virtual void scaleTo(double newScale, QPointF at = QPointF(-1,-1)) = 0;
	virtual void zoom(double zoomFactor, QPointF at = QPointF(-1,-1)) = 0;
	virtual void setViewport(QSize viewportSize) = 0;

	virtual bool isValid() = 0;
	virtual double getScale() = 0;
	virtual QPointF getViewpoint() = 0;
	virtual unsigned int getWidth() = 0;
	virtual unsigned int getHeight() = 0;

	virtual QImage getFrame(QSize targetSize) = 0;
	virtual QImage getFrame(QSize targetSize, double scale) = 0;
	virtual QImage getThumb(QSize targetSize, QRect* rect = NULL) = 0;
	virtual QImage getDetail(QSize targetSize, QPointF point, double zoom) = 0;

	virtual RasterResource* rasters(int &rastersCount) = 0;
	virtual GeometryResource* geometries(int &geometriesCount) = 0;
};

class SingleScene: public AbstractScene
{
	Q_OBJECT
protected:
	RasterResource* rasterRsrc_;
	GeometryResource geometryRsrc_;
	QPointF viewpoint_;
	double scale_;
	double maxScale_;
	double minScale_;
	QSize viewportSize_;
	void setLimitScale(double minScale, double maxScale);
	void limitScale();

public:
	SingleScene(QObject* parent, QString filepath);
	~SingleScene();

	bool createImage(QSize size, QColor color = QColor(Qt::transparent));
	bool loadImage(QString filepath);
	bool loadImage(QImage image);
        bool loadImage(Matrix* image, bool isGrayscale = true);
	bool saveImage(QString filepath, QString format);
	void transformImage(double H[9]);

	QSize imageSize();
	QImage getImage();

	void centerContent();
	void moveTo(QPointF pos);
	void pan(QPointF dPos);
	void scaleTo(double newScale, QPointF at = QPointF(-1,-1));
	void zoom(double zoomFactor, QPointF at = QPointF(-1,-1));
	void setViewport(QSize viewportSize);

	bool isValid();
	double getScale();
	QPointF getViewpoint();
	unsigned int getWidth();
	unsigned int getHeight();

	QImage getFrame(QSize targetSize);
	QImage getFrame(QSize targetSize, double scale);
	QImage getThumb(QSize targetSize, QRect* rect = NULL);
	QImage getDetail(QSize targetSize, QPointF point, double zoom);

	RasterResource* rasters(int &rastersCount);
	GeometryResource* geometries(int &geometriesCount);
};

#endif // SINGLESCENE_H
