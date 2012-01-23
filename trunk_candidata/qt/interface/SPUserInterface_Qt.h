#ifndef SPUSERINTERFACE_QT_H
#define SPUSERINTERFACE_QT_H

//#include "ui_DemExtraction.h"
#include "SPUserInterface.h"
#include "ImageView.h"
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

class SPUserInterface_Qt : public QWidget, public SPUserInterface
{
	Q_OBJECT

public:

public slots:

protected:
	static SPUserInterface_Qt* spInst;
	SPUserInterface_Qt(SPManager* manager, QWidget* parent = 0, Qt::WindowFlags fl = Qt::Window);
	~SPUserInterface_Qt();
	SPManager *manager;
	void closeEvent(QCloseEvent *e);
	int checkBoundingBox();

protected slots:
	virtual void languageChange();
	/*
		void onDemExtractionClicked();
		void onDemLoadClicked();
		void onDemSaveClicked();
		void onDemGridSaveClicked();
		void onDemGridClicked();
		void onLSMCheckChanged(int);
		void onAbortClicked();
		*/

private:
	void init();
	//void close();
	bool allow_close;

public:
	static SPUserInterface_Qt* instance(SPManager* manager);
	// Other Methods
	//
	bool exec();
	/*
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
		void disableOptions();
		void enableOptions();
		*/
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // DEMUSERINTERFACE_QT_H
