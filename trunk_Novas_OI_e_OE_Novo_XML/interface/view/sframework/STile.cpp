#include "STile.h"
#include "SImage.h"
#include "CommonMethods.h"

sclass::STile::STile()
{
}
sclass::STile::STile(double xOffset, double yOffset, SImage* image, int l, int t, int w, int h)
{
	myImage = image;
	width = w;
	height = h;
	left = l;
	top = t;
	texture = 0;
	offset2X = xOffset;
	offset2Y = yOffset;
	texturized = false;
	texturize();
}
sclass::STile::~STile()
{
}

void sclass::STile::texturize()
{
	if (CommonMethods::instance()->cutImage(left,top,width,height))
	{
		texturized = createTexture(&texture, myImage->getFormat(), width, height, CommonMethods::instance()->getCroppedPixels());
		CommonMethods::instance()->freeCropped();
	}
}
void sclass::STile::untexturize()
{
	if (texturized)
	{
		deleteTexture(&texture);
	}
}
bool sclass::STile::isTexturized()
{
	return texturized;
}
void sclass::STile::draw(double centerCoordX, double centerCoordY, double zoomFactor)
{
	double left = (centerCoordX + (offset2X * zoomFactor)) - (width/2 * zoomFactor);
	double right = (centerCoordX + (offset2X * zoomFactor)) + (width/2 * zoomFactor);
	double top = (centerCoordY + (offset2Y * zoomFactor)) - (height/2 * zoomFactor);
	double bottom = (centerCoordY + (offset2Y * zoomFactor)) + (height/2 * zoomFactor);
	drawImage(texture, left, right, top, bottom);
}
