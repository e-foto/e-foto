#include "LoadingScreen.h"
#include "ui_LoadingScreen.h"
#include <QDesktopWidget>
#include <QTimer>
#include <QWaitCondition>


LoadingScreen* LoadingScreen::loadingScreenInstance = NULL;

LoadingScreen& LoadingScreen::instance()
{
	if (loadingScreenInstance == NULL)
	{
		loadingScreenInstance = new LoadingScreen;
	}
	return *loadingScreenInstance;
}

LoadingScreen::LoadingScreen(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::LoadingScreen)
{
	ui->setupUi(this);
	setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::CustomizeWindowHint);
	//setWindowModality(Qt::ApplicationModal);
	setWindowTitle("Loading...");
	ui->progressBar->setMaximum(0);
	ui->progressBar->setMinimum(0);
	QDesktopWidget desktopWidget;
	QRect desktopRect(desktopWidget.availableGeometry(desktopWidget.primaryScreen()));
	QRect widgetRect(rect());
	move(desktopRect.center() - widgetRect.center());
}

LoadingScreen::~LoadingScreen()
{
	delete ui;
}

void LoadingScreen::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

