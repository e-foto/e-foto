#ifndef SVIEWPORT_H
#define SVIEWPORT_H

#include <string>
#include <deque>
using namespace std;

#include "GLMethods.h"
#include "SExhibition.h"

class SWidget;

namespace S {class Viewport {public: virtual void fitView(string inImage)=0; virtual void redefineZoomLimits()=0; virtual bool isInsideViewport(double x, double y)=0; virtual void setAnaglyphMode(bool status)=0; virtual void zoom(double zoomFactor, int x, int y)=0; virtual void zoomIn(int x, int y)=0; virtual void zoomOut(int x, int y)=0; virtual void panImages(int dx, int dy)=0; virtual void removeExhibition(sclass::SImageInViewport* exhibition)=0; virtual double getZoomFactor()=0; virtual void informDimensions(int& left, int& bottom, int& width, int& height)=0; virtual void addExhibition(sclass::SImageInViewport* exhibition)=0; virtual string getNickname()=0; virtual void addImage(string nickname)=0; virtual string getImageNicknames()=0; virtual bool isAnaglyphMode()=0;};}

namespace sclass
{

	class SViewport : public S::Viewport, public GLMethods
	{
	public:
		SViewport();
		SViewport(SWidget* parent, string nickname, double left, double bottom, double width, double height);
		SViewport(SWidget* parent, string nickname, double left, int leftOffset, double bottom,
				  int bottomOffset, double width, int widthOffset, double height, int heightOffset);
		~SViewport();
		string getNickname();
		double getZoomFactor();
		bool	isAnaglyphMode();

		void addImage(string nickname);
		void removeImage(string nickname);
		void removeImages();
		void panImages(int dx, int dy);
		void addExhibition(sclass::SImageInViewport* exhibition);
		void removeExhibition(sclass::SImageInViewport* exhibition);
		bool containsImage(string imageNickname);
		string	getImageNicknames();

		void zoom(double zoomFactor, int x, int y);
		void zoomIn(int x, int y);
		void zoomOut(int x, int y);
		void redefineZoomLimits();
		void fitView(string inImage);

		void show();
		void setAnaglyphMode(bool status);
		void informDimensions(int& left, int& bottom, int& width, int& height);
		bool isInsideViewport(double x, double y);

	protected:
		SWidget* parent;
		string nickname;
                double left; double bottom; double width; double height;
                int leftOffset; int bottomOffset; int widthOffset; int heightOffset;
                bool visibility; bool anaglyphMode;
                double zoomFactor; double minZoomFactor; double maxZoomFactor; double zoomStep;
		deque<sclass::SImageInViewport*> exposeImage;
		sclass::SImageInViewport* leftChannel;
		sclass::SImageInViewport* rightChannel;
                bool lRMask; bool lGMask; bool lBMask; bool rRMask; bool rGMask; bool rBMask;
		//deque<SMarkInViewport*> exposeMark;

	};

} // namespace sclass

#endif // SVIEWPORT_H
