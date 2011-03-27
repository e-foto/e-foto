#ifndef GLMETHODS_H
#define GLMETHODS_H

#include <iostream>
#include <sstream>
using namespace std;

#include "GL/gl.h"
#include <GL/glu.h>

class GLMethods
{
public:
	void clearGL();
	void clearGL(double r, double g, double b, double a);
	void setViewport(int left, int bottom, int width, int height);
	bool checkSizeTile(int size);
	bool createTexture(unsigned int* texture, int format, int width, int height, void* pixels);
	void deleteTexture(unsigned int* texture);
	//void rotateTexture(unsigned int &texture, double ang);
	void drawImage(unsigned int texture, double left, double right, double top, double bottom, bool useAlpha = false);
	void getMousePos(double x, double y, double result[2]);
};

#endif // GLMETHODS_H
