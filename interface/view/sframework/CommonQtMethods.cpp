#include "CommonQtMethods.h"
#include <QLabel>
#include <QPixmap>

bool CommonQtMethods::loadImage(int &width, int &height, int &format, string filePath)
{

	if (currentImage != NULL)
	{
		freeImage();
	}
	QImage* temp = new QImage(filePath.c_str());
	if (temp == NULL)
	{
		cerr << "Error: sorry, can't load image.\n";
		width = 0;
		height = 0;
		format = 0;
		return false;
	}
	if (!temp->isGrayscale())
	{
		currentImage = new QImage(temp->rgbSwapped());
		if (currentImage->hasAlphaChannel())
		{
			format = 4;
		}
		else
		{
			format = 3;
		}
		delete(temp);
	}
	else
	{
		currentImage = temp;
		format = 1;
	}
	width = currentImage->width();
	height = currentImage->height();
	return true;
}

void CommonQtMethods::rotateImage(double ang)
{
	if (currentImage == NULL)
	{
		//return;
	}
	QTransform transf;
	QImage* temp = new QImage(currentImage->transformed(transf.rotate(-ang),Qt::SmoothTransformation));
	*temp = temp->copy(temp->width()/2-16,temp->height()/2-16,32,32);
	delete(currentImage);
	currentImage = temp;
}

bool CommonQtMethods::cutImage(int x, int y, int w, int h)
{
	if (currentImage == NULL)
		return false;
	if (this->cropped != NULL)
	{
		freeCropped();
	}

	cropped = new QImage(currentImage->copy(x, y, w, h));

	return cropped;
}

void CommonQtMethods::freeImage()
{
	if (currentImage != NULL)
	{
		delete(currentImage);
		currentImage = NULL;
	}
}

void CommonQtMethods::freeCropped()
{
	if (cropped != NULL)
	{
		delete(cropped);
		cropped = NULL;
	}
}

void* CommonQtMethods::getImagePixels()
{
	if (currentImage != NULL)
		return currentImage->bits();
	return NULL;
}
void* CommonQtMethods::getCroppedPixels()
{
	if (cropped != NULL)
		return cropped->bits();
	return NULL;
}

CommonQtMethods::CommonQtMethods()
{
	currentImage = NULL;
	cropped = NULL;
}

CommonQtMethods::~CommonQtMethods()
{
	if (this->currentImage != NULL)
	{
		freeImage();
	}
	if (this->cropped != NULL)
	{
		freeCropped();
	}
}
