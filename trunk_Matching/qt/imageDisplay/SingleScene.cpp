#include "SingleScene.h"
#include <math.h>



SingleScene::SingleScene(QObject* parent, QString filepath):
	AbstractScene(parent),
	viewpoint_(0,0),
	viewportSize_(800,600)
{
	if (filepath.isEmpty())
	{
		rasterRsrc_ = NULL;
		minScale_ = maxScale_ = 1;
	}
	else
	{
		scale_ = 1.0;
		rasterRsrc_ = new RasterResource(filepath);
		if (!rasterRsrc_->isValid())
		{
			delete(rasterRsrc_);
			rasterRsrc_ = NULL;
			return;
		}
		minScale_ = 1;
		maxScale_ = 16;
		viewpoint_ = QPointF(rasterRsrc_->center().toPoint());
	}
}

SingleScene::~SingleScene()
{
	if (rasterRsrc_ != NULL)
		delete(rasterRsrc_);
}

QPointF SingleScene::getViewpoint()
{
	return viewpoint_;
}

unsigned int SingleScene::getWidth()
{
	if (rasterRsrc_ && rasterRsrc_->isValid())
		return rasterRsrc_->width();
	return 0;
}

unsigned int SingleScene::getHeight()
{
	if (rasterRsrc_ && rasterRsrc_->isValid())
		return rasterRsrc_->height();
	return 0;
}

void SingleScene::centerContent()
{
	viewpoint_ = rasterRsrc_->center();
}

void SingleScene::moveTo(QPointF pos)
{
	viewpoint_ = pos;
}

void SingleScene::pan(QPointF dPos)
{
	viewpoint_ += dPos;
}

void SingleScene::scaleTo(double newScale, QPointF at)
{
	double oldScale = scale_;
	scale_ = newScale;

	limitScale();

	if (at == QPointF(-1,-1) || rasterRsrc_ == NULL)
		return;
	if (!((at.x() >= 0 && at.x() <= rasterRsrc_->width()) && (at.y() >= 0 && at.y() <= rasterRsrc_->height())))
		at = rasterRsrc_->center();
	moveTo((viewpoint_-at)*oldScale/scale_ + at);
}

void SingleScene::zoom(double zoomFactor, QPointF at)
{
	double oldScale = scale_;
	scale_ *= zoomFactor;

	limitScale();

	if (at == QPointF(-1,-1) || rasterRsrc_ == NULL)
		return;
	if (!((at.x() >= 0 && at.x() <= rasterRsrc_->width()) && (at.y() >= 0 && at.y() <= rasterRsrc_->height())))
		at = rasterRsrc_->center();
	moveTo((viewpoint_-at)*oldScale/scale_ + at);
}

void SingleScene::setViewport(QSize viewportSize)
{
	viewportSize_ = viewportSize;
	setLimitScale(viewportSize_.width()/(double)getWidth() < viewportSize_.height()/(double)getHeight() ? viewportSize_.width()/(double)getWidth() : viewportSize_.height()/(double)getHeight(), 16);
	limitScale();
}

void SingleScene::transformImage(double H[9])
{
	rasterRsrc_->transformImage(H);
}

bool SingleScene::createImage(QSize size, QColor color)
{
	RasterResource* rsrc = new RasterResource("");
	QImage image(size,QImage::Format_ARGB32);
	image.fill(color.rgba());

	rsrc->load(image);
	if (rsrc->isValid())
	{
		if (rasterRsrc_)
			delete(rasterRsrc_);
		rasterRsrc_ = rsrc;
		viewpoint_ = QPointF(rasterRsrc_->center().toPoint());
		scale_ = 1;
		setLimitScale(viewportSize_.width()/(double)getWidth() < viewportSize_.height()/(double)getHeight() ? viewportSize_.width()/(double)getWidth() : viewportSize_.height()/(double)getHeight(), 16);
		return true;
	}
	else
		delete(rsrc);
	return false;
}

bool SingleScene::saveImage(QString filepath, QString format)
{
	if (rasterRsrc_)
	{
		return rasterRsrc_->save(filepath,format);
	}
	return false;
}

bool SingleScene::loadImage(QString filepath)
{
	RasterResource* rsrc = new RasterResource(filepath);
	if (rsrc->isValid())
	{
		if (rasterRsrc_)
			delete(rasterRsrc_);
		rasterRsrc_ = rsrc;
		viewpoint_ = QPointF(rasterRsrc_->center().toPoint());
		scale_ = 1;
		setLimitScale(viewportSize_.width()/(double)getWidth() < viewportSize_.height()/(double)getHeight() ? viewportSize_.width()/(double)getWidth() : viewportSize_.height()/(double)getHeight(), 16);
		return true;
	}
	else
		delete(rsrc);
	return false;
}

bool SingleScene::loadImage(QImage image)
{
	RasterResource* rsrc = new RasterResource("");
	rsrc->load(image);
	if (rsrc->isValid())
	{
		if (rasterRsrc_)
			delete(rasterRsrc_);
		rasterRsrc_ = rsrc;
		viewpoint_ = QPointF(rasterRsrc_->center().toPoint());
		scale_ = 1;
		setLimitScale(viewportSize_.width()/(double)getWidth() < viewportSize_.height()/(double)getHeight() ? viewportSize_.width()/(double)getWidth() : viewportSize_.height()/(double)getHeight(), 16);
		return true;
	}
	else
		delete(rsrc);
	return false;
}

QSize SingleScene::imageSize()
{
	if (rasterRsrc_ && rasterRsrc_->isValid())
		return QSize(rasterRsrc_->width(),rasterRsrc_->height());
	return QSize(0,0);
}

void SingleScene::setLimitScale(double minScale, double maxScale)
{
	minScale_ = minScale;
	maxScale_ = maxScale;
}

void SingleScene::limitScale()
{
	if (scale_ < minScale_)
		scale_ = minScale_;
	else if (scale_ > maxScale_)
		scale_ = maxScale_;
}

QImage SingleScene::getImage()
{
	if (!rasterRsrc_)
		return QImage();

	QRectF imageCut;
	imageCut.setSize(rasterRsrc_->size());
	imageCut.moveCenter(rasterRsrc_->center());

	return rasterRsrc_->getImageCut(rasterRsrc_->size(), imageCut);
}

QImage SingleScene::getFrame(QSize targetSize)
{
	if (!rasterRsrc_)
		return QImage();

	QRectF imageCut;
	QSizeF newSize;
	newSize.setWidth(targetSize.width()/scale_);
	newSize.setHeight(targetSize.height()/scale_);
	imageCut.setSize(newSize);
	imageCut.moveCenter(viewpoint_);

	return geometryRsrc_.draw(rasterRsrc_->getImageCut(targetSize, imageCut),targetSize, viewpoint_, scale_);
}

QImage SingleScene::getFrame(QSize targetSize, double scale)
{
	if (!rasterRsrc_)
		return QImage();

	QRectF imageCut;
	QSizeF newSize;
	newSize.setWidth(targetSize.width()/scale);
	newSize.setHeight(targetSize.height()/scale);
	imageCut.setSize(newSize);
	imageCut.moveCenter(viewpoint_);

	return geometryRsrc_.draw(rasterRsrc_->getImageCut(targetSize, imageCut),targetSize, viewpoint_, scale);
}

QImage SingleScene::getThumb(QSize targetSize, QRect* rect)
{
	if (!rasterRsrc_)
		return QImage();

	QRectF imageCut;
	imageCut.setSize(rasterRsrc_->size());
	imageCut.moveCenter(rasterRsrc_->center());
	QSize newTargetSize = rasterRsrc_->size();
	newTargetSize.scale(targetSize,Qt::KeepAspectRatio);

	double scale = newTargetSize.width()/(double)rasterRsrc_->size().width();

	QRect viewedRect(((viewpoint_-QPointF(rect->width()/(scale_*2.0), rect->height()/(scale_*2.0)))*scale).toPoint(), ((scale*QRectF(*rect).size())/scale_).toSize());

	QImage result( geometryRsrc_.draw( rasterRsrc_->getImageCut(newTargetSize, imageCut), newTargetSize, imageCut.center(), scale));
	QPainter painter(&result);
	painter.setPen(QPen(QColor(Qt::yellow)));
	painter.drawRect(viewedRect);
	painter.end();
	return result;
}

QImage SingleScene::getDetail(QSize targetSize, QPointF point, double zoom)
{
	if (!rasterRsrc_)
		return QImage();

	QRectF imageCut;
	QSizeF newSize;
	newSize.setWidth(targetSize.width()/zoom);
	newSize.setHeight(targetSize.height()/zoom);
	imageCut.setSize(newSize);
	imageCut.moveCenter(point);

	return geometryRsrc_.draw(rasterRsrc_->getImageCut(targetSize, imageCut),targetSize, point, zoom);
}

bool SingleScene::isValid()
{
	return rasterRsrc_ ? (rasterRsrc_->isValid() ? true : false) : false;
}

double SingleScene::getScale()
{
	return scale_;
}

RasterResource* SingleScene::rasters(int &rastersCount)
{
	rastersCount = 1;
	return rasterRsrc_;
}

GeometryResource* SingleScene::geometries(int &geometriesCount)
{
	geometriesCount = 1;
	return &geometryRsrc_;
}
