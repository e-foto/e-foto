#ifndef DEMUSERINTERFACE_QT_H
#define DEMUSERINTERFACE_QT_H

#include "ui_DemExtraction.h"
#include "DEMUserInterface.h"
#include "ImageView.h"
#include "LoadingScreen.h"

#include <QPoint>
#include <QGridLayout>
#include <QPushButton>
#include <QStandardItemModel>

class DEMUserInterface_Qt : public QWidget, public Ui::DEMInterface, public DEMUserInterface
{
	Q_OBJECT

public:

public slots:

protected:
        static DEMUserInterface_Qt* demInst;
        DEMUserInterface_Qt(DEMManager* manager, QWidget* parent = 0, Qt::WindowFlags fl = Qt::Window);
        ~DEMUserInterface_Qt();
        DEMManager *manager;
	void closeEvent(QCloseEvent *e);

protected slots:
	virtual void languageChange();

private:
        void init();

public:
        static DEMUserInterface_Qt* instance(DEMManager* manager);
	// Other Methods
	//
	bool exec();

};

#endif // DEMUSERINTERFACE_QT_H
