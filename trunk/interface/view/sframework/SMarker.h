#ifndef SMARKER_H
#define SMARKER_H

#include <string>
#include <math.h>
#include <deque>
using namespace std;

#include "GLMethods.h"

class SWidget;

namespace S {class Pin {public: virtual void rotate(double ang)=0; virtual void draw(double centerCoordX, double centerCoordY, double zoomFactor)=0; virtual string getNickname()=0; virtual bool loaded()=0;};}

namespace sclass
{

	class SPin : public S::Pin, public GLMethods
	{
	public:
		SPin();
		SPin(SWidget* parent, string nickname, string filename);
		~SPin();

		string getNickname();
		bool loaded();

		void unload();

		void rotate(double ang);

		void draw(double centerCoordX, double centerCoordY, double zoomFactor);

	protected:
		bool load(string filePath);
		bool texturize();

		SWidget* parent;
		string nickname, filePath;
		int width, height;
		bool canLoaded;
		int format;
		double ang;
		unsigned int texture;
		bool isTexturized;

	};

} // namespace sclass

#endif // SMARKER_H
