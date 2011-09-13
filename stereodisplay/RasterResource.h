#ifndef RASTERRESOURCE_H
#define RASTERRESOURCE_H

#include <QString>
#include <QImage>

class RasterResource
{
protected:
	QImage** pyramid_;
	QSize imageDim_;
	int levels_;
	bool isValid_;
public:
	RasterResource(QString filepath);
	~RasterResource();
	bool isValid();
	int width();
	int height();
	QSize size();
	int levels();
	QPointF center();
	QImage getImageCut(QSize targetSize, QRectF imageCut);
};

#endif // RASTERRESOURCE_H

/* IRaster interface
class IRaster
{
  public:
	 IRaster();
	 virtual ~IRaster();
	 IRaster(const IRaster & source);
	 IRaster & operator=(const IRaster & source);
	 virtual int getWidth() = 0;
	 virtual int getHeight() = 0;
	 virtual void toImageCoord(const double & x, const double & y, double & l, double & c) const = 0;
	 virtual void toWorldCoord(double l, double c, double & x, double & y) const = 0;
	 virtual bool getPixelValue(double x, double y, int band, double & value) const;
	 virtual bool getPixelValue(int l, int c, int band, double & value) = 0;
	 virtual std::string getBandName(int band) = 0;
};
*/

/* inline IRaster methods
inline IRaster::IRaster()
{
  // Bouml preserved body begin 00049417
  // Bouml preserved body end 00049417
}
inline IRaster::~IRaster()
{
  // Bouml preserved body begin 00049497
  // Bouml preserved body end 00049497
}
inline IRaster::IRaster(const IRaster & source)
{
  // Bouml preserved body begin 00049517
  // Bouml preserved body end 00049517
}
inline IRaster & IRaster::operator=(const IRaster & source)
{
  // Bouml preserved body begin 00049597
	return *this;
  // Bouml preserved body end 00049597
}
inline bool IRaster::getPixelValue(double x, double y, int band, double & value) const
{
  // Bouml preserved body begin 00049E97
	double c,
		l;

	toImageCoord(x, y, l, c);

	return getPixelValue(l, c, band, value);
  // Bouml preserved body end 00049E97
}
*/

/* Raster class
class Q_DECL_EXPORT Raster : public IRaster
{
  public:
	 Raster(::TeAbstractTheme * theme);
	 Raster(const Raster & source);
	 ~Raster();
	 Raster & operator=(const Raster & source);
	 virtual int getWidth();
	 virtual int getHeight();
	 virtual void toImageCoord(const double & x, const double & y, double & l, double & c) const;
	 virtual void toWorldCoord(double l, double c, double & x, double & y) const;
	 virtual bool getPixelValue(int l, int c, int band, double & value);
	 virtual std::string getBandName(int band);
  protected:
	 TeRaster* getRaster(::TeAbstractTheme * theme) const;
	::TeAbstractTheme * theme_; //!< Pointer to related theme.
};
*/
