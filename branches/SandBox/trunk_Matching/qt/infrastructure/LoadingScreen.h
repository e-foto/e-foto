#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H

#include <QWidget>
#include "ui_LoadingScreen.h"

class LoadingScreen: public QWidget, public Ui::LoadingScreen
{
	Q_OBJECT
public:
	static LoadingScreen& instance();

protected:
	LoadingScreen(QWidget *parent = 0);
	~LoadingScreen();
	void changeEvent(QEvent *e);

private:
	Ui::LoadingScreen *ui;
	static LoadingScreen* loadingScreenInstance;
};

#endif // LOADINGSCREEN_H
