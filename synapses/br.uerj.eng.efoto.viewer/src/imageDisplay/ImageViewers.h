#ifndef IMAGEVIEWERS_H
#define IMAGEVIEWERS_H

#include <QMainWindow>
#include "SingleDisplay.h"
#include "StereoDisplay.h"
#include "StereoTools.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class SingleViewer : public QMainWindow
{
	Q_OBJECT

	SingleDisplay* sd;
	SingleToolsBar* tool;
	void closeEvent(QCloseEvent *);

public:
	SingleViewer(QWidget *parent=0);
	SingleDisplay* getDisplay() {return sd;}
	MarkTool* getMarker() {return &tool->mark;}

public slots:
	void loadImage(QString filename);
	void loadImage(QImage* image);
	void loadImage(Matrix* image, bool isGrayscale = true);
	void blockOpen();
	void blockSave();
	void blockMark();

	void setImageMode();
	void setOrtoImageMode(double xi, double dx, double yi, double dy);
	void setElevationImageMode(double xi, double dx, double yi, double dy, double zi, double dz);

};

class SeparatedStereoViewer : public QMainWindow
{
	Q_OBJECT

	SingleDisplay* leftDisplay;
	SingleDisplay* rightDisplay;
	SeparatedStereoToolsBar* tool;
	//SingleToolsBar* leftTool;
	//SingleToolsBar* rightTool;
	void closeEvent(QCloseEvent *);

public:
	SeparatedStereoViewer(QWidget *parent=0);
	SingleDisplay* getLeftDisplay() {return leftDisplay;}
	SingleDisplay* getRightDisplay() {return rightDisplay;}
	MarkTool& getLeftMarker() {return (tool->leftMark);}
	MarkTool& getRightMarker() {return (tool->rightMark);}
	SeparatedStereoToolsBar* getToolBar() {return tool;}

public slots:
	void loadLeftImage(QString filename);
	void loadRightImage(QString filename);
	void loadLeftImage(QImage* image);
	void loadRightImage(QImage* image);
	void loadLeftImage(Matrix* image, bool isGrayscale = true);
	void loadRightImage(Matrix* image, bool isGrayscale = true);
	void setFeatures(Features* df);
	void blockOpen();
	void blockSave();
	void blockMark();
};

class StereoViewer : public QMainWindow
{
	Q_OBJECT

	StereoDisplay* stereoDisplay;
	StereoToolsBar* stereoTool;
	void closeEvent(QCloseEvent *);

public:
	StereoViewer(QWidget *parent=0);
	StereoDisplay* getDisplay() {return stereoDisplay;}
	MarkStereoTool& getMarker() {return stereoTool->_mark;}
	StereoToolsBar* getToolBar() {return stereoTool;}

public slots:
	void loadLeftImage(QString filename);
	void loadRightImage(QString filename);
	void loadLeftImage(QImage* image);
	void loadRightImage(QImage* image);
	void loadLeftImage(Matrix* image, bool isGrayscale = true);
	void loadRightImage(Matrix* image, bool isGrayscale = true);
	void setFeatures(Features* df);
	void blockOpen();
	void blockSave();
	void blockMark();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // IMAGEVIEWERS_H
