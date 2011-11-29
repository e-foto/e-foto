#ifndef RASTERRESOURCE_H
#define RASTERRESOURCE_H

#include <QString>
#include <QImage>

class RasterResource
{
protected:
	QImage** _pyramid;
	QSize _imageDim;
	int _levels;
	bool _isValid;

public:
	RasterResource(QString filepath);
	~RasterResource();

	bool load(QImage image);
	bool load(QString filepath);
	bool save(QString filepath, QString format);

	bool isValid();
	int levels();

	int width();
	int height();
	QSize size();
	QPointF center();

	QImage getImageCut(QSize targetSize, QRectF imageCut);
	void  transformImage(double H[9]);
};

#endif // RASTERRESOURCE_H
