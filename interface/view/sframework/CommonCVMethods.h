#ifndef COMMONCVMETHODS_H
#define COMMONCVMETHODS_H

#include "CommonMethods.h"
#include "cv.h"
#include "highgui.h"

class CommonCVMethods : public CommonMethods
{
public:
	friend class CommonMethods;
	bool loadImage(int& width, int& height, int& format, string filePath);
	void rotateImage(double ang);
	bool cutImage(int x, int y, int w, int h);
	void freeImage();
	void freeCropped();
	void* getImagePixels();
	void* getCroppedPixels();

protected:
	CommonCVMethods();
	~CommonCVMethods();

	IplImage* currentImage;
	IplImage* cropped;

};

#endif // COMMONCVMETHODS_H
