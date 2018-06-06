#ifndef IMAGEVIEWERS_H
#define IMAGEVIEWERS_H
/*Copyright 2002-2014 e-foto team (UERJ)
  This file is part of e-foto.

    e-foto is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    e-foto is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with e-foto.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QMainWindow>
#include "ui_EfotoViewer.h"
//#include "SingleDisplay.h"
//#include "StereoDisplay.h"
//#include "StereoTools.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class DemFeatures;
class SingleDisplay;
class SingleToolsBar;
class SeparatedStereoToolsBar;
class MarkTool;
class StereoDisplay;
class StereoToolsBar;
class MarkStereoTool;
class Matrix;

class SingleViewer : public QMainWindow, public Ui::EVMainWindow
{
	Q_OBJECT

	SingleDisplay* sd;
	SingleToolsBar* tool;
	void closeEvent(QCloseEvent *);

public:
    explicit SingleViewer(QWidget *parent=0);
	SingleDisplay* getDisplay() {return sd;}
    MarkTool* getMarker();

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
    explicit SeparatedStereoViewer(QWidget *parent=0);
	SingleDisplay* getLeftDisplay() {return leftDisplay;}
	SingleDisplay* getRightDisplay() {return rightDisplay;}
    MarkTool& getLeftMarker();
    MarkTool& getRightMarker();
	SeparatedStereoToolsBar* getToolBar() {return tool;}

public slots:
	void loadLeftImage(QString filename);
	void loadRightImage(QString filename);
	void loadLeftImage(QImage* image);
	void loadRightImage(QImage* image);
	void loadLeftImage(Matrix* image, bool isGrayscale = true);
	void loadRightImage(Matrix* image, bool isGrayscale = true);
	void setFeatures(DemFeatures* df);
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
    explicit StereoViewer(QWidget *parent=0);
    StereoDisplay* getDisplay() {return stereoDisplay;}
    MarkStereoTool& getMarker();
	StereoToolsBar* getToolBar() {return stereoTool;}

public slots:
	void loadLeftImage(QString filename);
	void loadRightImage(QString filename);
	void loadLeftImage(QImage* image);
	void loadRightImage(QImage* image);
	void loadLeftImage(Matrix* image, bool isGrayscale = true);
	void loadRightImage(Matrix* image, bool isGrayscale = true);
	void setFeatures(DemFeatures* df);
	void blockOpen();
	void blockSave();
	void blockMark();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // IMAGEVIEWERS_H
