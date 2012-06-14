#ifndef DEMUSERINTERFACE_QT_H
#define DEMUSERINTERFACE_QT_H

#include "ui_DemExtraction.h"
#include "ui_SeedEditor.h"
#include "DEMUserInterface.h"
#include "LoadingScreen.h"
#include "Matrix.h"
#include "ProgressWindow_Qt.h"
#include <QPoint>
#include <QGridLayout>
#include <QPushButton>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QMessageBox>
#include <QList>

#ifdef INTEGRATED_EFOTO
#include "ImageViewers.h"
#include "MatchingPoints.h"
#endif //INTEGRATED_EFOTO REVER!

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

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
	int demSource;

protected slots:
	virtual void languageChange();
	void onDemExtractionClicked();
	void onDemLoadClicked();
	void onDemGridLoadClicked();
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
	void enableAfterDEM(int sender);
	void enableAfterGrid();
	void onLoadPtsButtonClicked();
	void onSavePtsButtonClicked();
	void onLSMTemplateSizeChanged(int ts);
	void onCorrTemplateSizeChanged(int ts);
	void onDownValueChanged(double value);

private:
	void init();
	void close();
	bool allow_close;

public:
	static DEMUserInterface_Qt* instance(DEMManager* manager);
	// Other Methods
	//
	bool exec();
	void loadImage(Matrix & I, char *filename, double sample=1.0);
	int saveImage(char *filename, Matrix *I);
	void addImagePair(char *);
	void setStatus(char *);
	void setAutoExtInfo(int, int , double, double);
	void setManualExtInfo(int, double, double);
	void updateSeedsLabel(int nseeds);
	void setBoundingBox(double Xi, double Yi, double Xf, double Yf);
	void setGridData(double Xi, double Yi, double Xf, double Yf, double Zi, double Zf, double res_x, double res_y, int width, int height);
	void setProgress(int);
	void setMathcingHistogram(int *);
    void setAllowClose(bool _b) { allow_close = _b; }
	void showImage2D(Matrix* image, double xi, double dx, double yi, double dy, bool isGrayscale = true);
	void showImage3D(Matrix* image, double xi, double dx, double yi, double dy, double zi, double dz, bool isGrayscale = true);
	void disableOptions();
	void enableOptions();
	void showFatalErrorMessage(QString msg, bool abort);
    void showErrorMessage(QString msg) { showFatalErrorMessage(msg, false); }
	void setElapsedTime(double t, int opt);
};

class SeedEditorUserInterface_Qt : public QMainWindow, public Ui::SeedWindow
{
private:
	Q_OBJECT
	DEMManager *manager;

#ifdef INTEGRATED_EFOTO
	SeparatedStereoViewer* viewer;
	MatchingPointsList seeds, pairs;
	Marker *mark_seeds, *mark_pairs, *mark_empty;
#endif //INTEGRATED_EFOTO REVER!

	ProgressWindow pw;
	void getImagesIds(int &left_id, int &right_id);
	void updateMarks();
	void updateData(int i);
	void addPairs();
	void closeEvent(QCloseEvent *);
	void addMatchingPoints();
	void addSeedsAndTable();
	int findKey(int seed_id);
	vector<int> sel_seeds;
	void checkSelectedSeeds();
	int no_seeds, no_pairs; // Number for each pair, not the total

public:
	SeedEditorUserInterface_Qt(DEMManager* manager, QWidget* parent = 0);

public slots:
	void imageClicked(QPointF);
	void saveSeeds();
	void loadSeeds();
	void onAddButtonClicked();
	void onRemoveButtonClicked();
	void closeOk();
	void onComboBox1Changed(int index);
	void onCheckBoxChanged(int state);
	void onTableClicked(int row, int col);

signals:
	void closed(bool);
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // DEMUSERINTERFACE_QT_H
