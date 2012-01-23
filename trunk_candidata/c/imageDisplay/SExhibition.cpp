#include "SExhibition.h"
#include "SViewport.h"
#include "SImage.h"
#include <math.h>

#include <QApplication>

/** SImageInViewport class **/

sclass::SImageInViewport::SImageInViewport()
{
	image = NULL;
	viewport = NULL;
}
sclass::SImageInViewport::SImageInViewport(S::Image* image, S::Viewport* viewport)
{
	this->image = NULL;
	this->viewport = NULL;
	x = y = 0;
	scale = 1;
	limitXMovement = true;
	limitYMovement = true;
	if (image != NULL && viewport != NULL)
	{
		this->image = image;
		this->viewport = viewport;
		image->addExhibition(this);
		viewport->addExhibition(this);
		calculateMargins();
	}
}
sclass::SImageInViewport::~SImageInViewport()
{
	if (image != NULL)
	{
		image->removeExhibition(this);
	}
	if (viewport != NULL)
	{
		viewport->removeExhibition(this);
	}
}

string sclass::SImageInViewport::getImageNickname()
{
	if (image != NULL)
		return image->getNickname();
	return "";
}
string sclass::SImageInViewport::getViewportNickname()
{
	if (viewport != NULL)
		return viewport->getNickname();
	return "";
}
double sclass::SImageInViewport::getX()
{
	return x;
}
double sclass::SImageInViewport::getY()
{
	return y;
}
double sclass::SImageInViewport::getImageLeft()
{
	return left;
}
double sclass::SImageInViewport::getImageRight()
{
	return right;
}
double sclass::SImageInViewport::getImageTop()
{
	return top;
}
double sclass::SImageInViewport::getImageBottom()
{
	return bottom;
}
int sclass::SImageInViewport::getImageWidth()
{
	if (image != NULL)
		return image->getWidth();
	return 0;
}
int sclass::SImageInViewport::getImageHeight()
{
	if (image != NULL)
		return image->getHeight();
	return 0;
}
double sclass::SImageInViewport::getScale()
{
	return scale;
}
void sclass::SImageInViewport::setScale(double scale)
{
	this->scale = scale;
	calculateMargins();
}
void sclass::SImageInViewport::moveTo(double x, double y, bool autoCorrect)
{
	this->x = x;
	this->y = y;
	calculateMargins();
	if (autoCorrect/* && limitXMovement*/)
		correctXMovement();
	if (autoCorrect/* && limitYMovement*/)
		correctYMovement();
}
void sclass::SImageInViewport::pan(double dx, double dy, bool autoCorrect)
{
	this->x += dx;
	this->y += dy;
	this->left += dx;
	this->right += dx;
	this->top += dy;
	this->bottom +=dy;
	if (autoCorrect/* && limitXMovement*/)
		correctXMovement();
	if (autoCorrect/* && limitYMovement*/)
		correctYMovement();
}

void sclass::SImageInViewport::draw()
{
	if (image != NULL && viewport != NULL)
	{
		image->draw(this);
	}
}
double sclass::SImageInViewport::getEfectiveZoom()
{
	if (image != NULL && viewport != NULL)
	{
		return viewport->getZoomFactor() * scale;
	}
	return 0;
}

void sclass::SImageInViewport::calculateMargins()
{
	if (viewport != NULL && image != NULL)
	{
		double z = viewport->getZoomFactor();
		int w = image->getWidth();
		int h = image->getHeight();
		int vl, vb, vw, vh;
		viewport->informDimensions(vl,vb,vw,vh);

		left = (x - w*scale/2)*z;
		right = (x + w*scale/2)*z;
		top = (y + h*scale/2)*z;
		bottom = (y - h*scale/2)*z;

		if (vw <= w*scale*z)
		{
			//with viewportWidth < imageWidth
			xMax = -vw/2 + w*scale*z/2;
			xMin = vw/2 - w*scale*z/2;
		}
		else
		{
			//with viewportWidth > imageWidth
			//xMin = -vw/2 + w*scale*z/2;
			//xMax = vw/2 - w*scale*z/2;
			//to no moviment in this case
			xMax = xMin = 0;
		}
		if (vh <= h*scale*z)
		{
			//with viewportHeight < imageHeight
			yMax = -vh/2 + h*scale*z/2;
			yMin = vh/2 - h*scale*z/2;
		}
		else
		{
			//with viewportHeight > imageHeight
			//yMin = -vh/2 + h*scale*z/2;
			//yMax = vh/2 - h*scale*z/2;
			//to no moviment in this case
			yMax = yMin = 0;
		}
	}
}

void sclass::SImageInViewport::correctXMovement()
{
	if (x > xMax)
		x = xMax;
	else if (x < xMin)
		x = xMin;
}

void sclass::SImageInViewport::correctYMovement()
{
	if (y > yMax)
		y = yMax;
	else if (y < yMin)
		y = yMin;
}



/** SMarkInImage class **/

sclass::SMarkInImage::SMarkInImage()
{
	point = NULL;
	image = NULL;
}
sclass::SMarkInImage::SMarkInImage(S::Point *point, S::Image *image)
{
	this->point = NULL;
	this->image = NULL;
	x = y = 0;
	scale = 1;
	if (point !=NULL && image != NULL)
	{
		this->point = point;
		this->image = image;
		point->addExhibition(this);
		image->addExhibition(this);
	}
}
sclass::SMarkInImage::SMarkInImage(S::Point *point, S::Image *image, double x, double y, double scale)
{
	this->point = NULL;
	this->image = NULL;
	this->x = x;
	this->y = y;
	this->scale = scale;
	if (point != NULL && image != NULL)
	{
		this->point = point;
		this->image = image;
		point->addExhibition(this);
		image->addExhibition(this);
	}
}
sclass::SMarkInImage::~SMarkInImage()
{
	if (point != NULL)
	{
		point->removeExhibition(this);
	}
	if (image != NULL)
	{
		image->removeExhibition(this);
	}
}

string sclass::SMarkInImage::getMarkNickname()
{
	if (point != NULL)
		return point->getNickname();
	return "";
}
string sclass::SMarkInImage::getImageNickname()
{
	if (image != NULL)
		return image->getNickname();
	return "";
}
double sclass::SMarkInImage::getX()
{
	return x;
}
double sclass::SMarkInImage::getY()
{
	return y;
}
double sclass::SMarkInImage::getScale()
{
	return scale;
}
void sclass::SMarkInImage::setScale(double scale)
{
	this->scale = scale;
}
void sclass::SMarkInImage::moveTo(double x, double y)
{
	this->x = x;
	this->y = y;
	correctMovement();
}
void sclass::SMarkInImage::pan(double dx, double dy)
{
	this->x += dx;
	this->y += dy;
	correctMovement();
}

void sclass::SMarkInImage::draw(double x, double y, double zoom)
{
	if (point != NULL && image != NULL)
	{
		point->draw(this,this->x*zoom+x,this->y*zoom+y, scale);
	}
}
double sclass::SMarkInImage::getEfectiveZoom()
{
	if (point != NULL && image != NULL)
	{
		return scale;
	}
	return 0;
}

void sclass::SMarkInImage::correctMovement()
{
	if (image != NULL)
	{
		if (x > image->getWidth()/2)
			x = image->getWidth()/2;
		else if (x < -image->getWidth()/2)
			x = -image->getWidth()/2;
		if (y > image->getHeight()/2)
			y = image->getHeight()/2;
		else if (y < -image->getHeight()/2)
			y = -image->getHeight()/2;
	}
}



/** SMarkInViewport class **/
/*
sclass::SMarkInViewport::SMarkInViewport()
{
 image = NULL;
 viewport = NULL;
}
sclass::SMarkInViewport::SMarkInViewport(S::Point *point, S::Viewport *viewport)
{
 this->image = NULL;
 this->viewport = NULL;
 x = y = 0;
 scale = 1;
 limitXMovement = true;
 limitYMovement = true;
 if (image != NULL && viewport != NULL)
 {
  this->image = image;
  this->viewport = viewport;
  image->addExhibition(this);
  viewport->addExhibition(this);
  calculateMargins();
 }
}
sclass::SMarkInViewport::~SMarkInViewport()
{
 if (image != NULL)
 {
  image->removeExhibition(this);
 }
 if (viewport != NULL)
 {
  viewport->removeExhibition(this);
 }
}

string sclass::SMarkInViewport::getImageNickname()
{
 if (image != NULL)
  return image->getNickname();
 return "";
}
string sclass::SMarkInViewport::getViewportNickname()
{
 if (image != NULL)
  return viewport->getNickname();
 return "";
}
double sclass::SMarkInViewport::getX()
{
 return x;
}
double sclass::SMarkInViewport::getY()
{
 return y;
}
double sclass::SMarkInViewport::getImageLeft()
{
 return left;
}
double sclass::SMarkInViewport::getImageRight()
{
 return right;
}
double sclass::SMarkInViewport::getImageTop()
{
 return top;
}
double sclass::SMarkInViewport::getImageBottom()
{
 return bottom;
}
double sclass::SMarkInViewport::getScale()
{
 return scale;
}
void sclass::SMarkInViewport::setScale(double scale)
{
 this->scale = scale;
 calculateMargins();
}
void sclass::SMarkInViewport::moveTo(double x, double y)
{
 this->x = x;
 this->y = y;
 calculateMargins();
 if (limitXMovement)
  correctXMovement();
 if (limitYMovement)
  correctYMovement();
}
void sclass::SMarkInViewport::pan(double dx, double dy)
{
 this->x += dx;
 this->y += dy;
 this->left += dx;
 this->right += dx;
 this->top += dy;
 this->bottom +=dy;
 if (limitXMovement)
  correctXMovement();
 if (limitYMovement)
  correctYMovement();
}

void sclass::SMarkInViewport::draw()
{
 if (image != NULL && viewport != NULL)
 {
  image->draw(this);
 }
}
double sclass::SMarkInViewport::getEfectiveZoom()
{
 if (image != NULL && viewport != NULL)
 {
  return viewport->getZoomFactor() * scale;
 }
 return 0;
}

void sclass::SMarkInViewport::calculateMargins()
{
 if (viewport != NULL && image != NULL)
 {
  double z = viewport->getZoomFactor();
  int w = image->getWidth();
  int h = image->getHeight();
  int vl, vb, vw, vh;
  viewport->informDimensions(vl,vb,vw,vh);

  left = (x - w*scale/2)*z;
  right = (x + w*scale/2)*z;
  top = (y + h*scale/2)*z;
  bottom = (y - h*scale/2)*z;
  // Verificar se as contas a baixo estão corretas.
  if (vw <= w)
  {
   xMax = -vw/2 + w/2;
   xMin = vw/2 - w/2;
  }
  else
  {
   xMin = -vw/2 + w/2;
   xMax = vw/2 - w/2;
  }
  if (vh <= h)
  {
   yMax = -vh/2 + h/2;
   yMin = vh/2 - h/2;
  }
  else
  {
   yMin = -vh/2 + h/2;
   yMax = vh/2 - h/2;
  }
 }
}

void sclass::SMarkInViewport::correctXMovement()
{
 // por fazer
}

void sclass::SMarkInViewport::correctYMovement()
{
 // por fazer
}
*/
