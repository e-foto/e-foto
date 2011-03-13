#ifndef COMMONSDLMETHODS_H
#define COMMONSDLMETHODS_H

#include "CommonMethods.h"
#include "SDL.h"
#include "SDL_image.h"

class CommonSDLMethods : public CommonMethods
{
public:
	friend class CommonMethods;
	bool putImage(int& width, int& height, int& format, void* image);
	bool loadImage(int& width, int& height, int& format, string filePath);
	void rotateImage(double ang);
	bool cutImage(int x, int y, int w, int h);
	void freeImage();
	void freeCropped();
	void* getImagePixels();
	void* getCroppedPixels();

protected:
	CommonSDLMethods();
	~CommonSDLMethods();

	SDL_Surface* currentImage;
	SDL_Surface* cropped;

};

#endif // COMMONSDLMETHODS_H
