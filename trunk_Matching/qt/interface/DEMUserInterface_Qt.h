#ifndef DEMUSERINTERFACE_QT_H
#define DEMUSERINTERFACE_QT_H

#include "ui_DemExtraction.h"
#include "ui_SeedEditor.h"
#include "DEMUserInterface.h"
#include "ImageView.h"
#include "LoadingScreen.h"
#include "Matrix.h"
#include "ImageViewers.h"

#include <QPoint>
#include <QGridLayout>
#include <QPushButton>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QMessageBox>

class SeedEditorUserInterface_Qt;

class DEMUserInterface_Qt : public QWidget, public Ui::DEMInterface, public DEMUserInterface
{
	Q_OBJECT

public:

public slots:

protected:
        static DEMUserInterface_Qt* demInst;
        SeedEditorUserInterface_Qt* sed;
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
        void onAbortClicked();
        void onGridAreaLimitsStateChanged(int);
        void onInterStateChanged(int);
        void onShowImageStateChanged(int);
        void onSeedsEditorClicked();
        void onStereoplotterClicked();
        void onMatchingMethodChanged(int);
        void enableAfterDEM();
        void enableAfterGrid();

private:
        void init();
        void close();
        bool allow_close;

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
        void setMathcingHistogram(int *);
        void setAllowClose(bool _b) { allow_close = _b; };
        void showImage(Matrix* image, bool isGrayscale = true);
        void disableOptions();
        void enableOptions();
        void showErrorMessage(QString msg);
};

class SeedEditorUserInterface_Qt : public QMainWindow, public Ui::SeedWindow
{
    Q_OBJECT
    DEMManager *manager;
    SeparatedStereoViewer* viewer;

    void closeEvent(QCloseEvent *);

public:
    SeedEditorUserInterface_Qt(DEMManager* manager, QWidget* parent = 0);

public slots:
    void imageClicked(QPointF);
    void updateImagesList(QString);

signals:
    void closed(bool);
};

#endif // DEMUSERINTERFACE_QT_H
