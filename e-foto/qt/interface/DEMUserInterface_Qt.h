#ifndef DEMUSERINTERFACE_QT_H
#define DEMUSERINTERFACE_QT_H
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
#include "ui_DemExtraction.h"
#include "ui_SeedEditor.h"
#include "DEMUserInterface.h"
#include "ProgressWindow_Qt.h"


namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class SeedEditorUserInterface_Qt;
class SeparatedStereoViewer;
class Marker;

class DEMUserInterface_Qt : public QWidget, public Ui::DEMInterface, public DEMUserInterface
{
	Q_OBJECT

public:

public slots:

protected:
    static DEMUserInterface_Qt* demInst;
    SeedEditorUserInterface_Qt* sed;
    explicit DEMUserInterface_Qt(DEMManager* manager, QWidget* parent = 0, Qt::WindowFlags fl = Qt::Window);
    ~DEMUserInterface_Qt();
    DEMManager *manager;
    void closeEvent(QCloseEvent *e);
    int checkBoundingBox();
    int demSource;
    QString lastDir;

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
    void onLSMTemplateSizeChanged(/*int ts*/);
    void onCorrTemplateSizeChanged(/*int ts*/);
    void onDownValueChanged(/*double value*/);
    void onSeedEditorClosed();

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
    void setAllowClose(bool _b) { allow_close = _b; };
    void showImage2D(Matrix* image, double xi, double dx, double yi, double dy, bool isGrayscale = true);
    void showImage3D(Matrix* image, double xi, double dx, double yi, double dy, double zi, double dz, bool isGrayscale = true);
    void disableOptions();
    void enableOptions();
    void showFatalErrorMessage(QString msg, bool abort);
    void showErrorMessage(QString msg) { showFatalErrorMessage(msg, false); };
    void setElapsedTime(double t, int opt);
};

class SeedEditorUserInterface_Qt : public QMainWindow, public Ui::SeedWindow
{
private:
	Q_OBJECT
	DEMManager *manager;
	SeparatedStereoViewer* viewer;
	MatchingPointsList seeds, pairs;
	Marker *mark_seeds, *mark_pairs, *mark_empty;
	ProgressWindow pw;
	void getImagesIds(int &left_id, int &right_id);
	void updateMarks();
    void updateData(/*int i*/);
	void addPairs();
	void closeEvent(QCloseEvent *);
	void addMatchingPoints();
	void addSeedsAndTable();
	int findKey(int seed_id);
    std::vector<int> sel_seeds;
	void checkSelectedSeeds();
	int no_seeds, no_pairs; // Number for each pair, not the total
        QString lastDir;

public:
    explicit SeedEditorUserInterface_Qt(DEMManager* manager, QWidget* parent = 0);

public slots:
	void imageClicked(QPointF);
	void saveSeeds();
	void loadSeeds();
	void onAddButtonClicked();
	void onRemoveButtonClicked();
	void closeOk();
    void onComboBox1Changed(/*int index*/);
    void onCheckBoxChanged();
    void onTableClicked(/*int row, int col*/);

signals:
	void closed(bool);
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // DEMUSERINTERFACE_QT_H
