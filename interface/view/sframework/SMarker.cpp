#include "SMarker.h"
#include "AbstractSWidget.h"

sclass::SPin::SPin()
{
	this->parent = NULL;
	this->nickname = "";
	this->filePath = "";
	this->width = 0;
	this->height = 0;
	this->canLoaded = false;
	this->format = 0;
	texture = 0;
	ang = 0.0;
	isTexturized = false;
}
sclass::SPin::SPin(SWidget* parent, string nickname, string filename)
{
	this->parent = parent;
	this->nickname = nickname;
	this->filePath = filename;
	// Ate o momento o pin que não consegue fazer o load não entra na estrutura da SWidget e não é texturizado.
	if (this->load(filePath))
	{
		this->texturize();
	}
	ang = 0.0;
}
sclass::SPin::SPin(SWidget *parent, string nickname, void *loadedPin, CM::ImplementationType libType)
{
	this->parent = parent;
	this->nickname = nickname;
	this->filePath = "";
	// Ate o momento o pin que não consegue fazer o load não entra na estrutura da SWidget e não é texturizado.
	if (this->load(loadedPin,libType))
	{
		this->texturize();
	}
	ang = 0.0;
}
sclass::SPin::~SPin()
{
}

string sclass::SPin::getNickname()
{
	return nickname;
}
bool sclass::SPin::loaded()
{
	return canLoaded & isTexturized;
}

void sclass::SPin::unload()
{
	if (isTexturized)
	{
		deleteTexture(&texture);
	}
}

void sclass::SPin::rotate(double ang, void *obj, CM::ImplementationType libType)
{
	if (this->ang != ang)
	{
		unload();
		if (filePath == "")
		{
			if (this->load(obj, libType))
			{
				CommonMethods::instance(libType)->rotateImage(ang);
				this->texturize();
			}
		}
		else
		{
			if (this->load(filePath))
			{
				CommonMethods::instance(CM::QtMethods)->rotateImage(ang);
				this->texturize();
			}
		}
		this->ang = ang;
	}
}

void sclass::SPin::draw(double centerCoordX, double centerCoordY, double zoomFactor)
{
	double left = (centerCoordX) - (width/2 * zoomFactor);
	double right = (centerCoordX) + (width/2 * zoomFactor);
	double top = (centerCoordY) - (height/2 * zoomFactor);
	double bottom = (centerCoordY) + (height/2 * zoomFactor);
	drawImage(texture, left, right, top, bottom, true);
}

bool sclass::SPin::load(string filePath)
{
	return canLoaded = CommonMethods::instance(CM::QtMethods)->loadImage(width,height,format,filePath);
}
bool sclass::SPin::load(void *loadedPin, CM::ImplementationType libType)
{
	return canLoaded = CommonMethods::instance(libType)->putImage(width,height,format,loadedPin);
}
bool sclass::SPin::texturize()
{
	if (width == height && (width == 128 || width == 64 || width == 32 || width == 16 || width == 8 || width == 4 || width == 2 || width == 1))
	{
		isTexturized = createTexture(&texture, format, width, height, CommonMethods::instance(CM::QtMethods)->getImagePixels());
	}
	CommonMethods::instance(CM::QtMethods)->freeImage();
	return isTexturized;
}
