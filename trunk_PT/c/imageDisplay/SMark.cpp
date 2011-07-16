#include "SMark.h"

sclass::SPoint::SPoint()
{
	parent = 0;
	nickname = "";
	markedWith = 0;
}
sclass::SPoint::SPoint(SWidget* parent, string nickname)
{
	this->parent = parent;
	this->nickname = nickname;
	this->markedWith = 0;
}
sclass::SPoint::SPoint(SWidget* parent, string nickname, S::Pin* markedWith)
{
	this->parent = parent;
	this->nickname = nickname;
	this->markedWith = markedWith;
}
sclass::SPoint::~SPoint()
{
	// Por fazer
}

string sclass::SPoint::getNickname()
{
	return nickname;
}

void sclass::SPoint::setNewMarker(S::Pin *markedWith)
{
	if (markedWith != 0)
	{
		this->markedWith = markedWith;
	}
}
void sclass::SPoint::addExhibition(sclass::SMarkInImage* exhibition)
{
	if (exhibition->getMarkNickname() == nickname)
	{
		string imageNickname = exhibition->getImageNickname();
		// Impedimos associação que aponte uma imagem de nickname inválido.
		if (imageNickname == "" || string::npos != imageNickname.find(";") || isShowedInImage(imageNickname))
			return;
		exposedInImage.push_back(exhibition);
	}
}
void sclass::SPoint::removeExhibition(sclass::SMarkInImage* exhibition)
{
	for (unsigned int i = 0; i < exposedInImage.size(); i++)
	{
		if (exposedInImage.at(i) == exhibition)
		{
			exposedInImage.erase(exposedInImage.begin()+i);
			break;
		}
	}
}
void sclass::SPoint::removeExhibitions()
{
	while (!exposedInImage.empty())
		delete exposedInImage.at(0);
}
bool sclass::SPoint::isShowedInImage(string imageNickname)
{
	for (unsigned int i = 0; i < exposedInImage.size(); i++)
	{
		if (exposedInImage.at(i)->getImageNickname() == imageNickname)
		{
			return true;
		}
	}
	return false;
}
double sclass::SPoint::getXInImage(string imageNickname)
{
	for (unsigned int i = 0; i < exposedInImage.size(); i++)
	{
		if (exposedInImage.at(i)->getImageNickname() == imageNickname)
		{
			return exposedInImage.at(i)->getX();
		}
	}
	return 0;
}
double sclass::SPoint::getYInImage(string imageNickname)
{
	for (unsigned int i = 0; i < exposedInImage.size(); i++)
	{
		if (exposedInImage.at(i)->getImageNickname() == imageNickname)
		{
			return exposedInImage.at(i)->getY();
		}
	}
	return 0;
}
void sclass::SPoint::panInImage(string imageNickname, double x, double y)
{
	for (unsigned int i = 0; i < exposedInImage.size(); i++)
	{
		if (exposedInImage.at(i)->getImageNickname() == imageNickname)
		{
			exposedInImage.at(i)->pan(x,y);
		}
	}
}

void sclass::SPoint::draw(sclass::SMarkInImage *link, double x, double y, double zoom)
{
	if (link != NULL && markedWith != NULL)
	{
		markedWith->draw(x,y,zoom);
	}
}
