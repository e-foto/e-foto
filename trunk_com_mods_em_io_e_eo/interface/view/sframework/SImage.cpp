#include "SImage.h"
#include "AbstractSWidget.h"
#include "CommonMethods.h"

sclass::SImage::SImage()
{
	this->parent = NULL;
	this->nickname = "";
	this->filePath = "";
	this->tile.clear();
	this->tileSize = 32768;
	this->width = 0;
	this->height = 0;
	this->canLoaded = false;
	this->format = 0;
}
sclass::SImage::SImage(SWidget* parent, string nickname, string filename)
{
	this->parent = parent;
	this->nickname = nickname;
	this->filePath = filename;
	this->tile.clear();
	// Ate o momento a imagem que não consegue fazer o load não entra na estrutura da SWidget e não é texturizada.
	if (this->load(filePath))
	{
		this->tileSize = 32768;
		canLoaded = this->createTextureTiles();
	}
}
sclass::SImage::~SImage()
{
	this->removeViewportExhibitions();
	//this->removeMarks();
}

string sclass::SImage::getNickname()
{
	return nickname;
}
int sclass::SImage::getWidth()
{
	return width;
}
int sclass::SImage::getHeight()
{
	return height;
}
bool sclass::SImage::loaded()
{
	return canLoaded;
}
int sclass::SImage::getFormat()
{
	return this->format;
}
bool sclass::SImage::isInsideImage(double x, double y, string viewportNickname)
{
	sclass::SImageInViewport* exhibition;
	bool found = false;
	for (unsigned int i = 0; i < exposedInViewport.size(); i++)
	{
		if (exposedInViewport.at(i)->getViewportNickname() == viewportNickname)
		{
			exhibition = exposedInViewport.at(i);
			found = true;
		}
	}
	if (!found)
		return false;

	double newX = (x - exhibition->getX())/exhibition->getEfectiveZoom();
	double newY = (y - exhibition->getY())/exhibition->getEfectiveZoom();

	if (!(newX < -width/2 || newX > width/2 || newY > height/2 || newY < -height/2))
		return true;
	else
		return false;
}

void sclass::SImage::addExhibition(sclass::SImageInViewport* exhibition)
{
	if (exhibition->getImageNickname() == nickname)
	{
		string viewportNickname = exhibition->getViewportNickname();
		// Impedimos associação que aponte um viewport de nickname inválido.
		if (viewportNickname == "" || string::npos != viewportNickname.find(";") || isShowedInViewport(viewportNickname))
			return;
		exposedInViewport.push_back(exhibition);
	}
}
void sclass::SImage::removeExhibition(sclass::SImageInViewport* exhibition)
{
	for (unsigned int i = 0; i < exposedInViewport.size(); i++)
	{
		if (exposedInViewport.at(i) == exhibition)
		{
			exposedInViewport.erase(exposedInViewport.begin()+i);
			break;
		}
	}
}
void sclass::SImage::removeViewportExhibitions()
{
	while (!exposedInViewport.empty())
		delete exposedInViewport.at(0);
}
bool sclass::SImage::isShowedInViewport(string viewportNickname)
{
	for (unsigned int i = 0; i < exposedInViewport.size(); i++)
	{
		if (exposedInViewport.at(i)->getViewportNickname() == viewportNickname)
		{
			return true;
		}
	}
	return false;
}
void sclass::SImage::panInViewport(int dx, int dy, string viewportNickname)
{
	for (unsigned int i = 0; i < exposedInViewport.size(); i++)
	{
		if (exposedInViewport.at(i)->getViewportNickname() == viewportNickname)
		{
			exposedInViewport.at(i)->pan(dx, dy);
		}
	}
}

void sclass::SImage::addExhibition(sclass::SMarkInImage* exhibition)
{
	if (exhibition->getImageNickname() == nickname)
	{
		string markNickname = exhibition->getMarkNickname();
		// Impedimos associação que aponte uma image de nickname inválido.
		if (markNickname == "" || string::npos != markNickname.find(";") || containsMark(markNickname))
			return;
		exposeMark.push_back(exhibition);
	}
}
void sclass::SImage::removeExhibition(sclass::SMarkInImage* exhibition)
{
	for (unsigned int i = 0; i < exposeMark.size(); i++)
	{
		if (exposeMark.at(i) == exhibition)
		{
			exposeMark.erase(exposeMark.begin()+i);
			break;
		}
	}
}
void sclass::SImage::removeMarkExhibitions()
{
	while (!exposeMark.empty())
		delete exposeMark.at(0);
}
bool sclass::SImage::containsMark(string markNickname)
{
	for (unsigned int i = 0; i < exposeMark.size(); i++)
	{
		if (exposeMark.at(i)->getMarkNickname() == markNickname)
		{
			return true;
		}
	}
	return false;
}


void sclass::SImage::addMark(S::Point *point, double x, double y, S::Viewport* viewport, bool withImageCoord)
{
	// Se a marca foi adicionada com clique num viewport descontaremos o deslocamento da imagem no mesmo.
	if (viewport != NULL && !withImageCoord)
	{
		for (unsigned int i = 0; i < exposedInViewport.size(); i++)
		{
			if (exposedInViewport.at(i)->getViewportNickname() == viewport->getNickname())
			{
				x -= exposedInViewport.at(i)->getX();
				x /= exposedInViewport.at(i)->getEfectiveZoom();
				y -= exposedInViewport.at(i)->getY();
				y /= exposedInViewport.at(i)->getEfectiveZoom();

			}
		}
	}
	else
	{
		x -= width/2;
		y *= -1;
		y += height/2;
	}

	// Realizamos uma busca para determinar se a marca j´a n~ao ´e vis´ivel sobre a imagem.
	bool found = false;
	for (unsigned int i = 0; i < exposeMark.size(); i++)
	{
		if (exposeMark.at(i)->getMarkNickname() == point->getNickname())
		{
			found = true;
			// Se a marca existe apenas movemos ela.
			exposeMark.at(i)->moveTo(x,y);
		}
	}

	// Se a marca n~ao existe ela deve ser criada.
	if (!found)
		new sclass::SMarkInImage(point,this,x,y);
}

bool sclass::SImage::load(string filePath)
{
	return CommonMethods::instance()->loadImage(width,height,format,filePath);
}
void sclass::SImage::unload()
{
	for (unsigned int i = 0; i < tile.size(); i++)
		tile.at(i).untexturize();
	tile.clear();
	canLoaded = false;
}
bool sclass::SImage::reload(string filePath)
{
	for (unsigned int i = 0; i < tile.size(); i++)
		tile.at(i).untexturize();
	tile.clear();
	this->filePath = filePath;
	if (this->load(filePath))
	{
		this->tileSize = 32768;
		canLoaded = this->createTextureTiles();
	}
	return canLoaded;
}
bool sclass::SImage::createTextureTiles()
{
	int n = (int)(log((float)tileSize)/log(2.0));

	while (n >= 0 && (width < tileSize || height < tileSize || !checkSizeTile(tileSize)))
	{
		tileSize = pow(2,n);
		n--;
	}

	if (tileSize <= 1)
	{
		return false;
	}

	int gridCol = floor((double)width / tileSize);
	int gridRow = floor((double)height / tileSize);
	if (width == tileSize)
		gridCol = 0;
	if (height == tileSize)
		gridRow = 0;
	bool completSucess = true;

	for (int i = 0; i <= gridRow; i++)
	{
		for (int j = 0; j <= gridCol; j++)
		{
			int l,t,w,h;
			if (gridCol != 0)
				l = (width - tileSize) * j / gridCol;
			else
				l = 0;
			if (gridRow != 0)
				t = (height - tileSize) * i / gridRow;
			else
				t = 0;
			w = tileSize;
			h = tileSize;
			sclass::STile thisTile = STile(-width/2 + tileSize/2 + l, -1.0 * (-height/2 + tileSize/2 + t), this, l, t, w, h);
			if (thisTile.isTexturized())
			{
				tile.push_back(thisTile);
			}
			else
			{
				completSucess = false;
				unload();
				break;
			}
		}
		if (!completSucess)
			break;
	}

	// Como o OpenGL copiou a imagem em diversos tiles, apagamos a imagem gerada pelo CommonMethod.
	CommonMethods::instance()->freeImage();

	return completSucess;
}

void sclass::SImage::draw(sclass::SImageInViewport* link)
{
	if (link != NULL)
	{
		double zoom = link->getEfectiveZoom();
		double x = link->getX();
		double y = link->getY();
		for (unsigned int i = 0; i < tile.size(); i++)
		{
			tile.at(i).draw(x, y, zoom);
		}
		for (unsigned int i = 0; i < exposeMark.size(); i++)
		{
			exposeMark.at(i)->draw(x, y, zoom);
		}
	}
}
