#include "SViewport.h"
#include "AbstractSWidget.h"

#include "EDom.h"

sclass::SViewport::SViewport()
{
	this->parent = NULL;
	this->nickname = "";
	this->visibility = false;
	this->anaglyphMode = false;
	this->left = 0.0;
	this->bottom = 0.0;
	this->width = 0.0;
	this->height = 0.0;
	this->leftOffset = 0;
	this->bottomOffset = 0;
	this->widthOffset = 0;
	this->heightOffset = 0;
	this->zoomFactor = 1.0;
	this->minZoomFactor = 0.03125;
	this->maxZoomFactor = 32.0;
	this->zoomStep = 2;
	this->leftChannel = NULL;
	this->rightChannel = NULL;
	lRMask = true;
	lGMask = false;
	lBMask = false;
	rRMask = false;
	rGMask = true;
	rBMask = true;
}
sclass::SViewport::SViewport(SWidget* parent, string nickname, double left, double bottom, double width, double height)
{
	this->parent = parent;
	this->nickname = nickname;
	this->visibility = true;
	this->anaglyphMode = false;
	this->left = left;
	this->bottom = bottom;
	this->width = width;
	this->height = height;
	this->leftOffset = 0;
	this->bottomOffset = 0;
	this->widthOffset = 0;
	this->heightOffset = 0;
	this->zoomFactor = 1.0;
	this->minZoomFactor = 0.03125;
	this->maxZoomFactor = 32.0;
	this->zoomStep = 2;
	this->leftChannel = NULL;
	this->rightChannel = NULL;
	lRMask = true;
	lGMask = false;
	lBMask = false;
	rRMask = false;
	rGMask = true;
	rBMask = true;
	//this->correctDimensions();
}
sclass::SViewport::SViewport(SWidget* parent, string nickname, double left, int leftOffset, double bottom,
					 int bottomOffset, double width, int widthOffset, double height, int heightOffset)
{
	this->parent = parent;
	this->nickname = nickname;
	this->visibility = true;
	this->anaglyphMode = false;
	this->left = left;
	this->bottom = bottom;
	this->width = width;
	this->height = height;
	this->leftOffset = leftOffset;
	this->bottomOffset = bottomOffset;
	this->widthOffset = widthOffset;
	this->heightOffset = heightOffset;
	this->zoomFactor = 1.0;
	this->minZoomFactor = 0.03125;
	this->maxZoomFactor = 32.0;
	this->zoomStep = 2;
	this->leftChannel = NULL;
	this->rightChannel = NULL;
	lRMask = true;
	lGMask = false;
	lBMask = false;
	rRMask = false;
	rGMask = true;
	rBMask = true;
	//this->correctDimensions();
}
sclass::SViewport::~SViewport()
{
	this->removeImages();
	//this->removeMarks();
}

string sclass::SViewport::getNickname()
{
	return nickname;
}
double sclass::SViewport::getZoomFactor()
{
	return zoomFactor;
}
bool sclass::SViewport::isAnaglyphMode()
{
	return anaglyphMode;
}

void sclass::SViewport::addImage(string imageNickname)
{
	for (unsigned int i = 0; i < exposeImage.size(); i++)
	{
		if (exposeImage.at(i)->getImageNickname() == imageNickname)
		{
			return;
		}
	}
	if (parent != NULL)
	{
		S::Image* img = parent->getImage(imageNickname);
		if (img != NULL)
		{
			new sclass::SImageInViewport(img,this);
			redefineZoomLimits();
		}
	}
}
void sclass::SViewport::removeImage(string imageNickname)
{
	for (unsigned int i = 0; i < exposeImage.size(); i++)
	{
		if (exposeImage.at(i)->getImageNickname() == imageNickname)
		{
			delete exposeImage.at(i);
			redefineZoomLimits();
			break;
		}
	}
}
void sclass::SViewport::removeImages()
{
	while (!exposeImage.empty())
		delete exposeImage.at(0);
	redefineZoomLimits();
}
void sclass::SViewport::panImages(int dx, int dy)
{
	for (unsigned int i = 0; i < exposeImage.size(); i++)
	{
		exposeImage.at(i)->pan(dx,dy);
	}
}
void sclass::SViewport::addExhibition(sclass::SImageInViewport* exhibition)
{
	if (exhibition != NULL && exhibition->getViewportNickname() == nickname)
	{
		string imageNickname = exhibition->getImageNickname();
		// Impedimos associação que aponte uma imagem de nickname inválido.
		if (imageNickname == "" || string::npos != imageNickname.find(";") || containsImage(imageNickname))
			return;
		exposeImage.push_back(exhibition);
	}
}
void sclass::SViewport::removeExhibition(sclass::SImageInViewport* exhibition)
{
	for (unsigned int i = 0; i < exposeImage.size(); i++)
	{
		if (exposeImage.at(i) == exhibition)
		{
			exposeImage.erase(exposeImage.begin()+i);
			break;
		}
	}
	setAnaglyphMode(isAnaglyphMode());
}
bool sclass::SViewport::containsImage(string imageNickname)
{
	for (unsigned int i = 0; i < exposeImage.size(); i++)
	{
		if (exposeImage.at(i)->getImageNickname() == imageNickname)
		{
			return true;
		}
	}
	return false;
}
string sclass::SViewport::getImageNicknames()
{
	stringstream result;
	if (!exposeImage.empty())
		result << exposeImage.at(0)->getImageNickname();
	for (unsigned int i = 1; i < exposeImage.size(); i++)
		result << ";" << exposeImage.at(i)->getImageNickname();
	return result.str();
}

void sclass::SViewport::zoom(double zoomFactor, int x, int y)
{
	double oldZoomFactor = this->zoomFactor;
	this->zoomFactor = zoomFactor;
	if (zoomFactor < minZoomFactor)
		this->zoomFactor = minZoomFactor;
	if (zoomFactor > maxZoomFactor)
		this->zoomFactor = maxZoomFactor;
	double diff = this->zoomFactor/oldZoomFactor;
	for (unsigned int i = 0; i < exposeImage.size(); i++)
	{
		exposeImage.at(i)->pan(-x,-y,false);
		exposeImage.at(i)->moveTo(exposeImage.at(i)->getX()*diff,exposeImage.at(i)->getY()*diff,false);
		exposeImage.at(i)->pan(x,y);
	}
	if (parent != NULL)
		parent->repaint();
}
void sclass::SViewport::zoomIn(int x, int y)
{
	double newZoomFactor = zoomFactor * zoomStep;
	if (newZoomFactor <= maxZoomFactor)
	{
		zoomFactor = newZoomFactor;
		for (unsigned int i = 0; i < exposeImage.size(); i++)
		{
			exposeImage.at(i)->pan(-x,-y,false);
			exposeImage.at(i)->moveTo(exposeImage.at(i)->getX()*zoomStep,exposeImage.at(i)->getY()*zoomStep,false);
			exposeImage.at(i)->pan(x,y);
		}
		if (parent != NULL)
			parent->repaint();
	}
	else if (zoomFactor < maxZoomFactor)
	{
		zoom(maxZoomFactor, x, y);
	}
}
void sclass::SViewport::zoomOut(int x, int y)
{
	double newZoomFactor = zoomFactor / zoomStep;
	if (newZoomFactor >= minZoomFactor)
	{
		zoomFactor = newZoomFactor;
		for (unsigned int i = 0; i < exposeImage.size(); i++)
		{
			exposeImage.at(i)->pan(-x,-y,false);
			exposeImage.at(i)->moveTo(exposeImage.at(i)->getX()/zoomStep,exposeImage.at(i)->getY()/zoomStep,false);
			exposeImage.at(i)->pan(x,y);
		}
		if (parent != NULL)
			parent->repaint();
	}
	else if (zoomFactor > minZoomFactor)
	{
		zoom(minZoomFactor, x, y);
	}
}
void sclass::SViewport::redefineZoomLimits()
{
	if (exposeImage.size() == 0)
	{
		this->minZoomFactor = 0.03125;
		this->maxZoomFactor = 32.0;
	}
	else
	{
		int maxImgW = 0;
		int maxImgH = 0;
		double minZoomInW, minZoomInH;
		int thisW, thisH, dummie;
		informDimensions(dummie,dummie,thisW, thisH);
		for (int i = 0; i<exposeImage.size(); i++)
		{
			if (exposeImage.at(i)->getImageWidth() > maxImgW)
				maxImgW = exposeImage.at(i)->getImageWidth();
			if (exposeImage.at(i)->getImageHeight() > maxImgH)
				maxImgH = exposeImage.at(i)->getImageWidth();
		}
		minZoomInW = thisW/(maxImgW+150.0);
		minZoomInH = thisH/(maxImgH+150.0);
		if (minZoomInW <= minZoomInH)
			minZoomFactor = minZoomInW;
		else
			minZoomFactor = minZoomInH;
		if (minZoomFactor > 1)
			minZoomFactor = 1;
		if (zoomFactor < minZoomFactor)
			zoom(minZoomFactor,0,0);
	}
}
void sclass::SViewport::fitView(string inImage)
{
	for (unsigned int i = 0; i < exposeImage.size(); i++)
	{
		if (exposeImage.at(i)->getImageNickname() == inImage)
		{
			panImages(-exposeImage.at(i)->getX(),-exposeImage.at(i)->getY());
			zoom(minZoomFactor,0,0);
		}
	}
}

void sclass::SViewport::show()
{
	int left, bottom, width, height;
	informDimensions(left,bottom,width,height);
	setViewport(left, bottom, width, height);

	if (anaglyphMode && leftChannel != NULL && rightChannel != NULL)
	{
		// Logo o código de openGL a baixo será retirado para fazer parte da classe GLMethods.
		glColorMask((GLboolean)lRMask, (GLboolean)lGMask, (GLboolean)lBMask, GL_TRUE);
		leftChannel->draw();

		glEnable(GL_COLOR_LOGIC_OP);
		glLogicOp(GL_OR);

		glColorMask((GLboolean)rRMask, (GLboolean)rGMask, (GLboolean)rBMask, GL_TRUE);
		rightChannel->draw();

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDisable(GL_COLOR_LOGIC_OP);
	}
	else
		for (unsigned int i = 0; i < exposeImage.size() ; i++)
		{
			exposeImage.at(i)->draw();
		}
}
void sclass::SViewport::setAnaglyphMode(bool status)
{
	if (status && exposeImage.size() > 1)
	{
		this->anaglyphMode = true;
		this->leftChannel = exposeImage.at(0);
		this->rightChannel = exposeImage.at(1);
	}
	else
	{
		this->anaglyphMode = false;
		this->leftChannel = NULL;
		this->rightChannel = NULL;
	}
}
void sclass::SViewport::informDimensions(int &l, int &b, int &w, int &h)
{
	int pw = 0;
	int ph = 0;

	if (parent != NULL)
	{
		pw = parent->getWidth();
		ph = parent->getHeight();
	}

	l = left*pw/100 + leftOffset;
	b = bottom*ph/100 + bottomOffset;
	w = width*pw/100 + widthOffset;
	h = height*ph/100 + heightOffset;
}
bool sclass::SViewport::isInsideViewport(double x, double y)
{
	int pw = 0;
	int ph = 0;
	int l, b, w, h;

	if (parent != NULL)
	{
		pw = parent->getWidth();
		ph = parent->getHeight();
	}

	l = left*pw/100 + leftOffset;
	b = bottom*ph/100 + bottomOffset;
	w = width*pw/100 + widthOffset;
	h = height*ph/100 + heightOffset;

	if (x<l || x>l+w || y<b || y>b+h)
		return false;
	else
		return true;
}

