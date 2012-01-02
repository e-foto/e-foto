#ifndef RASTERRESOURCE_H
#define RASTERRESOURCE_H

#include <QString>
#include <QImage>
#include <QColor>

class RasterResource
{
protected:
	QImage** _pyramid;
	QSize _imageDim;
	int _levels;
	bool _isValid;
	bool _useSmoothIn;
	bool _useSmoothOut;

public:
	RasterResource(QString filepath, bool withSmoothIn = false, bool withSmoothOut = true);
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
	QColor getColor(QPoint at);
	unsigned int getGrayColor(QPointF at, bool linear = false);
	void useSmoothIn(bool useSmooth);
	void transformImage(double H[9]);
};

#endif // RASTERRESOURCE_H
