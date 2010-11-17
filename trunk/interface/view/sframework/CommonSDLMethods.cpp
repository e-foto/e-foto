#include "CommonSDLMethods.h"

bool CommonSDLMethods::loadImage(int &width, int &height, int &format, string filePath)
{
	SDL_Init(SDL_INIT_NOPARACHUTE | SDL_INIT_VIDEO);
	if (currentImage != NULL)
	{
		freeImage();
	}
	currentImage = IMG_Load(filePath.c_str());
	if (currentImage == NULL)
	{
		cerr << "Error: sorry, can't load image.\n";
		width = 0;
		height = 0;
		format = 0;
		return false;
	}
	width = currentImage->w;
	height = currentImage->h;
	format = currentImage->format->BytesPerPixel;
	SDL_Quit();
	return true;
}

void CommonSDLMethods::rotateImage(double ang)
{
	//por fazer
}

bool CommonSDLMethods::cutImage(int x, int y, int w, int h)
{
	if (currentImage == NULL)
		return false;
	if (this->cropped != NULL)
	{
		freeCropped();
	}
	SDL_Surface* cropped;
	Uint32 rmask, gmask, bmask, amask;
	if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
	{
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	}
	else
	{
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	}
	/* New surface of the cropped size */
	cropped = SDL_CreateRGBSurface(currentImage->flags, w, h, currentImage->format->BitsPerPixel, rmask, gmask, bmask, amask);
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	/* Copy the cropped piece */
	SDL_BlitSurface(currentImage, &r, cropped, NULL);
	this->cropped = cropped;
	return true;
}

void CommonSDLMethods::freeImage()
{
	if (currentImage != NULL)
	{
		SDL_FreeSurface(currentImage);
		currentImage = NULL;
	}
}

void CommonSDLMethods::freeCropped()
{
	if (cropped != NULL)
	{
		SDL_FreeSurface(cropped);
		cropped = NULL;
	}
}

void* CommonSDLMethods::getImagePixels()
{
	if (currentImage != NULL)
		return currentImage->pixels;
	return NULL;
}
void* CommonSDLMethods::getCroppedPixels()
{
	if (cropped != NULL)
		return cropped->pixels;
	return NULL;
}

CommonSDLMethods::CommonSDLMethods()
{
	currentImage = NULL;
	cropped = NULL;
}

CommonSDLMethods::~CommonSDLMethods()
{
	if (this->currentImage != NULL)
	{
		freeImage();
	}
	if (this->cropped != NULL)
	{
		freeCropped();
	}
}
