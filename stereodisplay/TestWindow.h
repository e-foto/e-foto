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
#include <QAction>
#include "StereoService.h"

class TestWindow : public QMainWindow
{
	Q_OBJECT
public:
	TestWindow();
	StereoView* sv;
public slots:
	void changeStatusBar(QPointF*);
	void changeStatusBar(QPointF*, QPointF*);
	void changeStatusBarOnClickLeft(QPointF*);
	void changeStatusBarOnClickRight(QPointF*);
	void changeStatusBarOnClick(QPointF*, QPointF*);
	void leftLoad();
	void rightLoad();
};

#endif // TESTWINDOW_H
