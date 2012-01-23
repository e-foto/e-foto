#ifndef STILE_H
#define STILE_H

#include <string>
using namespace std;

#include "GLMethods.h"

namespace sclass
{

class SImage;

class STile : public GLMethods
{
protected:
	SImage* myImage;
	int left; int top;
	unsigned int texture;
	bool texturized;
	double width; double height; double offset2X; double offset2Y;

public:
	STile();
	STile(double xOffset, double yOffset, SImage* image, int l, int t, int w, int h);
	~STile();

	void texturize();
	void untexturize();
	bool isTexturized();
	void draw(double centerCoordX, double centerCoordY, double zoomFactor);
};

} // namespace sclass

#endif // STILE_H
