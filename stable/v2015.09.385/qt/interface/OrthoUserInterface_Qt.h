#ifndef ORTHOUSERINTERFACE_QT_H
#define ORTHOUSERINTERFACE_QT_H
/*Copyright 2002-2014 e-foto team (UERJ)
  This file is part of e-foto.

    e-foto is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    e-foto is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with e-foto.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ui_OrthoForm.h"
#include "ui_OrthoQualityEditor.h"
#include "OrthoUserInterface.h"
#include "ImageViewers.h"
#include "LoadingScreen.h"
#include "Matrix.h"
#include "GeometryResource.h"

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
    void onOrthoGeoTiffClicked();
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
    void setCurrentWork(std::string msg);
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
        std::string getTableAt(int row, int col);
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
        void onCheckBoxChanged(/*int state*/);
        void onTableClicked(int row/*, int col*/);

signals:
        void closed(bool);

};



} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // ORTHOUSERINTERFACE_QT_H
