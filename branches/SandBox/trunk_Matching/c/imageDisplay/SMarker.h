#ifndef SMARKER_H
#define SMARKER_H

#include <string>
#include <math.h>
#include <deque>
using namespace std;

#include "GLMethods.h"
#include "CommonMethods.h"

class SWidget;

namespace S {class Pin {public: virtual void rotate(double ang, void *obj = 0, CM::ImplementationType libType = CM::QtMethods)=0; virtual void draw(double centerCoordX, double centerCoordY, double zoomFactor)=0; virtual string getNickname()=0; virtual bool loaded()=0;};}

namespace sclass
{

	class SPin : public S::Pin, public GLMethods
	{
	public:
		SPin();
		SPin(SWidget* parent, string nickname, string filename);
		SPin(SWidget* parent, string nickname, void* loadedPin, CM::ImplementationType libType);
		~SPin();

		string getNickname();
		bool loaded();

		void unload();

		void rotate(double ang, void *obj = 0, CM::ImplementationType libType = CM::QtMethods);

		void draw(double centerCoordX, double centerCoordY, double zoomFactor);

	protected:
		bool load(string filePath);
		bool load(void* loadedPin, CM::ImplementationType libType);
		bool texturize();

		SWidget* parent;
                string nickname; string filePath;
                int width; int height;
		bool canLoaded;
		int format;
		double ang;
		unsigned int texture;
		bool isTexturized;

	};

} // namespace sclass

#endif // SMARKER_H
