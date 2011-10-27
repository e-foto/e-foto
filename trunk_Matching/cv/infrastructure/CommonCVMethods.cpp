#include "CommonCVMethods.h"

bool CommonCVMethods::putImage(int &width, int &height, int &format, void *image)
{
	//por fazer
	return false;
}

bool CommonCVMethods::loadImage(int &width, int &height, int &format, string filePath)
{
	if (currentImage != NULL)
	{
		freeImage();
	}
	IplImage* temp = cvLoadImage(filePath.c_str(),-1);
	if (temp == NULL)
	{
		cerr << "Error: sorry, can't load image.\n";
		width = 0;
		height = 0;
		format = 0;
		return false;
	}
	if (temp->nChannels == 3)
	{
		currentImage = cvCreateImage(cvGetSize(temp), IPL_DEPTH_8U, temp->nChannels);
		cvCvtColor(temp, currentImage, CV_BGR2RGB);
		cvReleaseImage(&temp);
	}
	else
	{
		currentImage = temp;
	}
	width = currentImage->width;
	height = currentImage->height;
	format = currentImage->nChannels;
	return true;
}

void CommonCVMethods::rotateImage(double ang)
{
	//por fazer
}

bool CommonCVMethods::cutImage(int x, int y, int w, int h)
{
	if (currentImage == NULL)
		return false;
	if (this->cropped != NULL)
	{
		freeCropped();
	}
	// Must have dimensions of output image
	cropped = cvCreateImage( cvSize(w,h), currentImage->depth, currentImage->nChannels );

	// Say what the source region is
	cvSetImageROI( currentImage, cvRect( x, y, w, h ) );

	// Do the copy
	cvCopy( currentImage, cropped );
	cvResetImageROI( currentImage );

	return cropped;
}

void CommonCVMethods::freeImage()
{
	if (currentImage != NULL)
	{
		cvReleaseImage(&currentImage);
		currentImage = NULL;
	}
}

void CommonCVMethods::freeCropped()
{
	if (cropped != NULL)
	{
		cvReleaseImage(&cropped);
		cropped = NULL;
	}
}

void* CommonCVMethods::getImagePixels()
{
	if (currentImage != NULL)
		return currentImage->imageData;
	return NULL;
}
void* CommonCVMethods::getCroppedPixels()
{
	if (cropped != NULL)
		return cropped->imageData;
	return NULL;
}

CommonCVMethods::CommonCVMethods()
{
	currentImage = NULL;
	cropped = NULL;
}

CommonCVMethods::~CommonCVMethods()
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
