#ifndef COMMONQTMETHODS_H
#define COMMONQTMETHODS_H

#include "CommonMethods.h"
#include <QImage>

class CommonQtMethods : public CommonMethods
{
public:
	friend class CommonMethods;
	bool putImage(int& width, int& height, int& format, void* image);
	bool loadImage(int& width, int& height, int& format, string filePath);
	void rotateImage(double ang);
	bool cutImage(int x, int y, int w, int h);
	void freeImage();
	void freeCropped();
	void* getImagePixels();
	void* getCroppedPixels();

protected:
	CommonQtMethods();
	~CommonQtMethods();

	QImage* currentImage;
	QImage* cropped;

};

#endif // COMMONQTMETHODS_H
