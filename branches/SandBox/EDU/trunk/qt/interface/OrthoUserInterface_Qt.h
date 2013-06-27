#ifndef ORTHOUSERINTERFACE_QT_H
#define ORTHOUSERINTERFACE_QT_H

#include "ui_OrthoForm.h"
#include "ui_OrthoQualityEditor.h"
#include "OrthoUserInterface.h"
#include "ImageViewers.h"
#include "LoadingScreen.h"
#include "Matrix.h"

#include <QPoint>
#include <QGridLayout>
#include <QPushButton>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QMessageBox>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class OrthoQualityUserInterface_Qt;

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
        OrthoQualityUserInterface_Qt *ortho_qual_form;

protected slots:
	virtual void languageChange();
	void onAbortClicked();
	void onLoadDemClicked();
        void onLoadOrthoClicked();
	void onOrthoClicked();
        void onOrthoQualityButtonClicked();
	void onShowImageChanged(int);
        void onCloseOrthoQualityForm();

private:
	void init();
	void close();
	bool allow_close;
        QString lastDir;

public:
	static OrthoUserInterface_Qt* instance(OrthoManager* manager);
	// Other Methods
	//
        void loadImage(Matrix & I, char *filename, double sample=1.0);
	int saveImage(char *filename, Matrix *I);
	bool exec();
	void setProgress(int);
	void setCurrentWork(string msg);
	void setAllowClose(bool _b) { allow_close = _b; };
	void disableOptions();
	void enableOptions();
        void showImage2D(Matrix* image, double xi, double dx, double yi, double dy, bool isGrayscale = true);
        void showImage3D(Matrix* image, double xi, double dx, double yi, double dy, double zi, double dz, bool isGrayscale = true);
	void showErrorMessage(QString msg);
};

// Ortho-image Quality Tool

class OrthoQualityUserInterface_Qt : public QMainWindow, public Ui::OrthoQualityWindow
{
private:
        Q_OBJECT
        OrthoManager *manager;
        SingleViewer* viewer;
        Marker *mark_ortho, *mark_gnd, *mark_empty;
        void updateMarks();
//        void updateData(int i);
        void closeEvent(QCloseEvent *);
        int loadPointsFromSP(char *filename);
        int loadPointsFromTxt(char *filename);
        int loadPointsFromQuality(char *filename);
        void addTableEnding(int tab_pos);
        string getTableAt(int row, int col);
        double getDoubleTableAt(int row, int col);
        void setTableAt(int row, int col, double value);
        QString lastDir;

public:
        OrthoQualityUserInterface_Qt(OrthoManager* manager, QWidget* parent = 0);
        void showImage2D(Matrix* image, double xi, double dx, double yi, double dy, bool isGrayscale = true);

public slots:
        void imageClicked(QPointF);
        void saveQuality();
        void loadPoints();
        void onDeletePoint();
        void calculateAll();
        void onCheckBoxChanged(int state);

signals:
        void closed(bool);

};



} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // ORTHOUSERINTERFACE_QT_H
