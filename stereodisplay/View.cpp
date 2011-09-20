#include "View.h"
#include "math.h"

// MonoView class
MonoView::MonoView(QObject* parent, QString filepath):
	QObject(parent),
	viewpoint_(0,0)
{
	if (filepath.isEmpty())
		rasterRsrc_ = NULL;
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
		viewpoint_ = rasterRsrc_->center().toPoint();
	}
}
MonoView::~MonoView()
{
	if (rasterRsrc_ != NULL)
		delete(rasterRsrc_);
}
QPointF MonoView::getViewpoint()
{
	return viewpoint_;
}
void MonoView::moveTo(QPointF pos)
{
	viewpoint_ = pos;
}
void MonoView::pan(QPointF dPos)
{
	viewpoint_ += dPos;
}
void MonoView::scaleTo(double newScale)
{
	scale_ = newScale;
	limitScale();
}
void MonoView::zoom(double zoomFactor, QPointF at)
{
	if (!rasterRsrc_)
		return;

	double oldScale = scale_;
	scale_ *= zoomFactor;

	limitScale();

	if (at == QPointF(-1,-1))
		return;
	if (!((at.x() >= 0 && at.x() <= rasterRsrc_->width()) && (at.y() >= 0 && at.y() <= rasterRsrc_->height())))
		at = rasterRsrc_->center();
	moveTo((viewpoint_-at)*oldScale/scale_ + at);
}
bool MonoView::loadImage(QString filepath)
{
	RasterResource* rsrc = new RasterResource(filepath);
	if (rsrc->isValid())
	{
		if (rasterRsrc_)
			delete(rasterRsrc_);
		rasterRsrc_ = rsrc;
		viewpoint_ = rasterRsrc_->center();
		scale_ = 1;
		return true;
	}
	else
		delete(rsrc);
	return false;
}
bool MonoView::imageLoaded()
{
	return rasterRsrc_ != NULL;
}
QSize MonoView::imageSize()
{
	if (rasterRsrc_ && rasterRsrc_->isValid())
		return QSize(rasterRsrc_->width(),rasterRsrc_->height());
	return QSize(0,0);
}
void MonoView::limitScale()
{
	if (scale_ < 1/pow(2,rasterRsrc_->levels()))
		scale_ = 1/pow(2,rasterRsrc_->levels());
	else if (scale_ > pow(2,4))
		scale_ = pow(2,4);
}
QImage MonoView::getFrame(QSize targetSize)
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
QImage MonoView::getFrame(QSize targetSize, double scale)
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
QImage MonoView::getThumb(QSize targetSize, QRect* rect)
{
	if (!rasterRsrc_)
		return QImage();

	QRectF imageCut;
	imageCut.setSize(rasterRsrc_->size());
	imageCut.moveCenter(rasterRsrc_->center());
	QSize newTargetSize = rasterRsrc_->size();
	newTargetSize.scale(targetSize,Qt::KeepAspectRatio);

	return rasterRsrc_->getImageCut(newTargetSize, imageCut);
}
QImage MonoView::getDetail(QSize targetSize, QPointF point, double zoom)
{
	if (!rasterRsrc_)
		return QImage();

	QRectF imageCut;
	QSizeF newSize;
	newSize.setWidth(targetSize.width()/zoom);
	newSize.setHeight(targetSize.height()/zoom);
	imageCut.setSize(newSize);
	imageCut.moveCenter(point);

	return rasterRsrc_->getImageCut(targetSize, imageCut);
}
double MonoView::getScale()
{
	return scale_;
}

GeometryResource* MonoView::geometries()
{
	return &geometryRsrc_;
}



// StereoView class
StereoView::StereoView(QObject* parent, QString leftImageFilepath, QString rightImageFilepath):
	QObject(parent)
{
	leftView_ = new MonoView(parent, leftImageFilepath);
	rightView_ = new MonoView(parent, rightImageFilepath);
}
StereoView::~StereoView()
{
	leftView_ = rightView_ = NULL;
}
void StereoView::setLeftView(MonoView* leftView)
{
	leftView_ = leftView;
}
void StereoView::setRightView(MonoView* rightView)
{
	rightView_ = rightView;
}
