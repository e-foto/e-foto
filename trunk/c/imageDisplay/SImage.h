#ifndef SIMAGE_H
#define SIMAGE_H

#include <string>
#include <math.h>
#include <deque>
using namespace std;

#include "GLMethods.h"
#include "SExhibition.h"
#include "SMark.h"
#include "STile.h"

class SWidget;

namespace S {class Image {public: virtual void addExhibition(sclass::SMarkInImage* exhibition)=0; virtual void removeExhibition(sclass::SMarkInImage* exhibition)=0; virtual void addMark(Point* point, double x, double y, S::Viewport* viewport = 0, bool withImageCoord = false)=0; virtual bool isInsideImage(double x, double y, string viewportNickname)=0; virtual int getWidth()=0; virtual int getHeight()=0; virtual void panInViewport(int dx, int dy, string viewportNickname)=0; virtual void removeExhibition(sclass::SImageInViewport* exhibition)=0; virtual void draw(sclass::SImageInViewport* link)=0; virtual string getNickname()=0; virtual void addExhibition(sclass::SImageInViewport* exhibition)=0;};}

namespace sclass
{

	class SImage : public S::Image, public GLMethods
	{
	public:
		SImage();
		SImage(SWidget* parent, string nickname, string filename);
		~SImage();

		string getNickname();
		int getWidth();
		int getHeight();
		bool loaded();
		int getFormat();
		bool isInsideImage(double x, double y, string viewportNickname);

		void addExhibition(sclass::SImageInViewport* exhibition);
		void removeExhibition(sclass::SImageInViewport* exhibition);
		void removeViewportExhibitions();
		bool isShowedInViewport(string viewportNickname);
		void panInViewport(int dx, int dy, string viewportNickname);

		void addExhibition(sclass::SMarkInImage* exhibition);
		void removeExhibition(sclass::SMarkInImage* exhibition);
		void removeMarkExhibitions();
		bool containsMark(string markNickname);

		void addMark(S::Point* point, double x, double y, S::Viewport* viewport = 0, bool withImageCoord = false);

		void draw(sclass::SImageInViewport* link);
		void unload();
		bool reload(string filename);

	protected:
		bool load(string filePath);
		bool createTextureTiles();

		SWidget* parent;
                string nickname; string filePath;
		int tileSize;
                int width; int height;
		bool canLoaded;
		int format;
		deque<sclass::SImageInViewport*> exposedInViewport;
		deque<sclass::STile> tile;
		deque<sclass::SMarkInImage*> exposeMark;

	};

} // namespace sclass

#endif // SIMAGE_H
