#ifndef COMMONMETHODS_H
#define COMMONMETHODS_H

#include <string>
#include <iostream>
using namespace std;

namespace CM
{
	enum ImplementationType{None, CVMethods, QtMethods/*, SDLMethods*/};
};

class CommonMethods
{
public:
	static CommonMethods* instance(CM::ImplementationType type = CM::CVMethods);
	virtual bool loadImage(int& width, int& height, int& format, string filePath) = 0;
	virtual void rotateImage(double ang) = 0;
	virtual bool cutImage(int x, int y, int w, int h) = 0;
	virtual void freeImage() = 0;
	virtual void freeCropped() = 0;
	virtual void* getImagePixels() = 0;
	virtual void* getCroppedPixels() = 0;
	virtual ~CommonMethods();

private:
	static CommonMethods* commonInst;
	static bool destroyInstance();
	static CM::ImplementationType impType;

};

#endif // COMMONMETHODS_H
