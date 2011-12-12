#ifndef IMAGEVIEWERS_H
#define IMAGEVIEWERS_H

#include <QMainWindow>
#include "ui_EfotoViewer.h"
#include "SingleDisplay.h"
#include "StereoDisplay.h"
#include "StereoTools.h"

class SingleViewer : public QMainWindow, public Ui::EVMainWindow
{
	Q_OBJECT

	SingleDisplay* sd;
	SingleToolsBar* tool;
	void closeEvent(QCloseEvent *);

public:
	SingleViewer(QWidget *parent=0);

public slots:
	void loadImage(QString filename);
	void loadImage(QImage* image);
	void loadImage(Matrix* image, bool isGrayscale = true);
	void blockOpen();
	void blockSave();
	void blockMark();
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

public slots:
	void loadLeftImage(QString filename);
	void loadRightImage(QString filename);
	void loadLeftImage(QImage* image);
	void loadRightImage(QImage* image);
	void loadLeftImage(Matrix* image, bool isGrayscale = true);
	void loadRightImage(Matrix* image, bool isGrayscale = true);
	void blockOpen();
	void blockSave();
	void blockMark();
};

class StereoViewer : public QMainWindow
{
	Q_OBJECT

	StereoDisplay* stereoDisplay;
	SingleDisplay* leftDisplay;
	SingleDisplay* rightDisplay;
	//SingleToolsBar* tool;
	StereoToolsBar* stereoTool;
	SingleToolsBar* leftTool;
	SingleToolsBar* rightTool;
	void closeEvent(QCloseEvent *);

public:
	StereoViewer(QWidget *parent=0);

public slots:
	void loadLeftImage(QString filename);
	void loadRightImage(QString filename);
	void loadLeftImage(QImage* image);
	void loadRightImage(QImage* image);
	void loadLeftImage(Matrix* image, bool isGrayscale = true);
	void loadRightImage(Matrix* image, bool isGrayscale = true);
	void blockOpen();
	void blockSave();
	void blockMark();
};

#endif // IMAGEVIEWERS_H
