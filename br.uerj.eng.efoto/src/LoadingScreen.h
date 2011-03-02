#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H

#include <QWidget>

namespace Ui {
    class LoadingScreen;
}

class LoadingScreen : public QWidget {
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
