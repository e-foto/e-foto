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
		int left, top;
		unsigned int texture;
		bool texturized;
		double width, height, offset2X, offset2Y;

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
