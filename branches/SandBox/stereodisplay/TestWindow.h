#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QStatusBar>
#include <QToolBar>
#include <QFileDialog>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QAction>
#include "StereoService.h"

class InitWindow : public QMainWindow
{
	Q_OBJECT
public:
	InitWindow();
public slots:
	void startTestStereo();
	void startTestPT();
};

class TestStereoWindow : public QMainWindow
{
	Q_OBJECT
	Marker mark_;
	unsigned int marksCount_;
public:
	TestStereoWindow();
	StereoDisplay* sd;
	MonoDisplay* ld;
	MonoDisplay* rd;
	double detailZoom;
public slots:
	void changeStatusBar(QPointF*);
	void changeStatusBar(QPointF*, QPointF*);
	void changeStatusBarOnClickLeft(QPointF*);
	void changeStatusBarOnClickRight(QPointF*);
	void changeStatusBarOnClick(QPointF*, QPointF*);
	void changeDetailZoom(int);
	void leftLoad();
	void rightLoad();
	void clearMarks();
};

class TestPTWindow : public QMainWindow
{
	Q_OBJECT
	Marker mark_;
	unsigned int lMarksCount_;
	unsigned int rMarksCount_;
public:
	TestPTWindow();
	MonoDisplay* ld;
	MonoDisplay* rd;
	double detailZoom;

public slots:
	void changeStatusBar(QPointF*);
	void changeStatusBarOnClickLeft(QPointF*);
	void changeStatusBarOnClickRight(QPointF*);
	void changeDetailZoom(int);
	void leftLoad();
	void rightLoad();
	void clearMarks();
};

#endif // TESTWINDOW_H
