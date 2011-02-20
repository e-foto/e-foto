#include "AbstractSWidget.h"

bool SWidget::containsViewport(string nickname)
{
	for (unsigned int i = 0; i < viewport.size(); i++)
		if (viewport.at(i).getNickname() == nickname)
			return true;
	return false;
}
bool SWidget::containsImage(string nickname)
{
	for (unsigned int i = 0; i < image.size(); i++)
		if (image.at(i).getNickname() == nickname)
			return true;
	return false;
}
bool SWidget::containsPin(string nickname)
{
	for (unsigned int i = 0; i < pin.size(); i++)
		if (pin.at(i).getNickname() == nickname)
			return true;
	return false;
}
bool SWidget::containsPoint(string nickname)
{
	for (unsigned int i = 0; i < point.size(); i++)
		if (point.at(i).getNickname() == nickname)
			return true;
	return false;
}

int SWidget::countViewports()
{
	return viewport.size();
}
int SWidget::countImages()
{
	return image.size();
}
int SWidget::countPins()
{
	return pin.size();
}
int SWidget::countPoints()
{
	return point.size();
}

bool SWidget::createViewport(string nickname)
{
	// Impedimos o nickname vazio ou com ponto e virgula e a replica de nicknames
	if (nickname == "" || string::npos != nickname.find(";") || containsViewport(nickname))
		return false;
	viewport.push_back(sclass::SViewport(this, nickname, 0, 0, 100, 100));
	return true;
}
bool SWidget::createViewport(string nickname, double left, double bottom, double width, double height)
{
	// Impedimos o nickname vazio ou com ponto e virgula e a replica de nicknames
	if (nickname == "" || string::npos != nickname.find(";") || containsViewport(nickname))
		return false;
	viewport.push_back(sclass::SViewport(this, nickname, left, bottom, width, height));
	return true;
}
bool SWidget::createViewport(string nickname, double left, int leftOffset, double bottom, int bottomOffset,
							 double width, int widthOffset, double height, int heightOffset)
{
	// Impedimos o nickname vazio ou com ponto e virgula e a replica de nicknames
	if (nickname == "" || string::npos != nickname.find(";") || containsViewport(nickname))
		return false;
	viewport.push_back(sclass::SViewport(this, nickname, left, leftOffset, bottom, bottomOffset, width, widthOffset, height, heightOffset));
	return true;
}
bool SWidget::createImage(string nickname, string filename)
{
	// Impedimos o nickname vazio ou com ponto e virgula e a replica de nicknames
	if (nickname == "" || string::npos != nickname.find(";") || containsImage(nickname))
		return false;
	sclass::SImage img(this, nickname, filename);
	// E a imagem que não consegue fazer o load não entra na estrutura da SWidget.
	if (img.loaded())
	{
		image.push_back(img);
		return true;
	}
	else
		return false;
}
bool SWidget::createPin(string nickname, string filename)
{
	// Impedimos o nickname vazio ou com ponto e virgula e a replica de nicknames
	if (nickname == "" || string::npos != nickname.find(";") || containsPin(nickname))
		return false;
	sclass::SPin p(this, nickname, filename);
	// E o pin que não consegue fazer o load não entra na estrutura da SWidget.
	if (p.loaded())
	{
		pin.push_back(p);
		return true;
	}
	else
		return false;
}
bool SWidget::createPoint(string nickname, string markerNickname)
{
	// Impedimos o nickname vazio ou com ponto e virgula e a replica de nicknames
	if (nickname == "" || string::npos != nickname.find(";") || containsPoint(nickname))
		return false;
	// Verificamos primeiro se o ponto pode ser associado a um pin para o nome informado
	if (containsPin(markerNickname))
	{
		point.push_back(sclass::SPoint(this,nickname,getPin(markerNickname)));
		return true;
	}
	else
	{
		point.push_back(sclass::SPoint(this,nickname));
		return false;
	}
}

void SWidget::destroyViewport(string nickname)
{
	for (unsigned int i = 0; i < viewport.size(); i++)
	{
		if (viewport.at(i).getNickname() == nickname)
		{
			viewport.erase(viewport.begin()+i);
			break;
		}
	}
}
void SWidget::destroyImage(string nickname)
{
	for (unsigned int i = 0; i < image.size(); i++)
	{
		if (image.at(i).getNickname() == nickname)
		{
			image.at(i).unload();
			image.erase(image.begin()+i);
			break;
		}
	}
}
void SWidget::destroyPin(string nickname)
{
	for (unsigned int i = 0; i < pin.size(); i++)
	{
		if (pin.at(i).getNickname() == nickname)
		{
			pin.at(i).unload();
			pin.erase(pin.begin()+i);
			break;
		}
	}
}
void SWidget::destroyPoint(string nickname)
{
	for (unsigned int i = 0; i < point.size(); i++)
	{
		if (point.at(i).getNickname() == nickname)
		{
			point.erase(point.begin()+i);
			break;
		}
	}
}

S::Viewport* SWidget::getViewport(string nickname)
{
	for (unsigned int i = 0; i < viewport.size(); i++)
		if (viewport.at(i).getNickname() == nickname)
			return &viewport.at(i);
	return NULL;
}
S::Image* SWidget::getImage(string nickname)
{
	for (unsigned int i = 0; i < image.size(); i++)
		if (image.at(i).getNickname() == nickname)
			return &image.at(i);
	return NULL;
}
S::Pin* SWidget::getPin(string nickname)
{
	for (unsigned int i = 0; i < pin.size(); i++)
		if (pin.at(i).getNickname() == nickname)
			return &pin.at(i);
	return NULL;
}
S::Point* SWidget::getPoint(string nickname)
{
	for (unsigned int i = 0; i < point.size(); i++)
		if (point.at(i).getNickname() == nickname)
			return &point.at(i);
	return NULL;
}

S::Viewport* SWidget::getSelectedViewport()
{
	return selectedViewport;
}
S::Image* SWidget::getSelectedImage()
{
	return selectedImage;
}
S::Pin* SWidget::getSelectedPin()
{
	return selectedPin;
}
S::Point* SWidget::getSelectedPoint()
{
	return selectedPoint;
}

void SWidget::selectViewport(string nickname)
{
	selectedViewport = getViewport(nickname);
}
void SWidget::selectImage(string nickname)
{
	selectedImage = getImage(nickname);
}
void SWidget::selectPin(string nickname)
{
	selectedPin = getPin(nickname);
}
void SWidget::selectPoint(string nickname)
{
	selectedPoint = getPoint(nickname);
}

void SWidget::setZoomCursor()
{
	cursorType = 0;
}
void SWidget::setMarkingCursor()
{
	cursorType = 1;
}
void SWidget::setMoveCursor()
{
	cursorType = 2;
}
void SWidget::setDirectionCursor()
{
	cursorType = 3;
}


void SWidget::pan(int dx, int dy)
{
	if (selectedViewport != NULL)
	{
		if (selectedImage != NULL)
			selectedImage->panInViewport(dx,dy,selectedViewport->getNickname());
		else
			selectedViewport->panImages(dx,dy);
		repaint();
	}
}
void SWidget::zoom(double zoom)
{
	if (selectedViewport != NULL)
	{
		selectedViewport->zoom(zoom,0,0);
	}
}
void SWidget::zoom(double zoom, int x, int y)
{
	if (selectedViewport != NULL)
	{
		selectedViewport->zoom(zoom,x,y);
	}
}
void SWidget::zoomIn()
{
	if (selectedViewport != NULL)
	{
		selectedViewport->zoomIn(0,0);
	}
}
void SWidget::zoomIn(int x, int y)
{
	if (selectedViewport != NULL)
	{
		selectedViewport->zoomIn(x,y);
	}
}
void SWidget::zoomOut()
{
	if (selectedViewport != NULL)
	{
		selectedViewport->zoomOut(0,0);
	}
}
void SWidget::zoomOut(int x, int y)
{
	if (selectedViewport != NULL)
	{
		selectedViewport->zoomOut(x,y);
	}
}
