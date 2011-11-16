#ifndef DEMUSERINTERFACE_QT_H
#define DEMUSERINTERFACE_QT_H

#include "ui_DemExtraction.h"
#include "DEMUserInterface.h"
#include "ImageView.h"
#include "LoadingScreen.h"
#include "Matrix.h"

#include <QPoint>
#include <QGridLayout>
#include <QPushButton>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QMessageBox>

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
        int checkBoundingBox();

protected slots:
	virtual void languageChange();
        void onDemExtractionClicked();
        void onDemLoadClicked();
        void onDemSaveClicked();
        void onDemGridSaveClicked();
        void onDemGridClicked();
        void onLSMCheckChanged(int);

private:
        void init();

public:
        static DEMUserInterface_Qt* instance(DEMManager* manager);
	// Other Methods
	//
	bool exec();
        Matrix * loadImage(char *filename, double sample=1.0);
        int saveImage(char *filename, Matrix *I);
        void addImagePair(char *);
        void setStatus(char *);
        void setAutoExtInfo(int, int , double, double);
        void setBoundingBox(double Xi, double Yi, double Xf, double Yf);
        void setGridData(double Xi, double Yi, double Xf, double Yf, double Zi, double Zf, double res_x, double res_y, int width, int height);
        void setProgress(int);
};

#endif // DEMUSERINTERFACE_QT_H
