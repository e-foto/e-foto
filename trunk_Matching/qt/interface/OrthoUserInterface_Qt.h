#ifndef ORTHOUSERINTERFACE_QT_H
#define ORTHOUSERINTERFACE_QT_H

#include "ui_OrthoForm.h"
#include "OrthoUserInterface.h"
#include "ImageView.h"
#include "LoadingScreen.h"
#include "Matrix.h"

#include <QPoint>
#include <QGridLayout>
#include <QPushButton>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QMessageBox>

class OrthoUserInterface_Qt : public QWidget, public Ui::OrthoForm, public OrthoUserInterface
{
	Q_OBJECT

public:

public slots:

protected:
        static OrthoUserInterface_Qt* OrthoInst;
        OrthoUserInterface_Qt(OrthoManager* manager, QWidget* parent = 0, Qt::WindowFlags fl = Qt::Window);
        ~OrthoUserInterface_Qt();
        OrthoManager *manager;
	void closeEvent(QCloseEvent *e);
        int dem_load_flag;

protected slots:
	virtual void languageChange();
        void onAbortClicked();
        void onLoadDemClicked();
        void onOrthoClicked();

private:
        void init();
        void close();
        bool allow_close;

public:
        static OrthoUserInterface_Qt* instance(OrthoManager* manager);
	// Other Methods
	//
        Matrix * loadImage(char *filename, double sample=1.0);
        int saveImage(char *filename, Matrix *I);
	bool exec();
        void setProgress(int);
        void setAllowClose(bool _b) { allow_close = _b; };
        void disableOptions();
        void enableOptions();
};

#endif // ORTHOUSERINTERFACE_QT_H
