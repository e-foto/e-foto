#include "LoadingScreen.h"
#include "ui_LoadingScreen.h"
#include <QScreen>
#include <QGuiApplication>
#include <QTimer>
#include <QWaitCondition>


LoadingScreen* LoadingScreen::loadingScreenInstance = nullptr;

LoadingScreen& LoadingScreen::instance()
{
        if (loadingScreenInstance == nullptr)
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
        QScreen *screen = QGuiApplication::primaryScreen();
        if (screen) {
          QRect desktopRect = screen->availableGeometry();
          QRect widgetRect = rect();
          move(desktopRect.center() - widgetRect.center());
        }
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

