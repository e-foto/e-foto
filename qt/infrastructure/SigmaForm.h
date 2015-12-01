#ifndef SIGMAFORM_H
#define SIGMAFORM_H

#include <QWidget>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
//#include <ScienceSpinBox>
#include <QTableWidget>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QStringList>

#include "Matrix.h"
#include "EDom.h"
#include "ScienceSpinBox.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class SigmaFormController: public QObject
{
	Q_OBJECT

protected:
	QWidget* contentWidget;
	QList<QLabel*> labels;
	QList<ScienceSpinBox*> edits;
	QStringList titles;
	unsigned int dimension;
    std::string direction;
    std::string mode;
	Matrix values;

public:
	SigmaFormController();
    explicit SigmaFormController(std::string myMode, unsigned int myDimension = 1);
    explicit SigmaFormController(Matrix myValues);

protected:
	void init();
    Matrix stDevToMatrix(std::deque<double> stDev);
    std::deque<double> matrixToStDev(Matrix mat);
	void updateTitles();

public:
	QWidget* getContent();
    void setDirection(std::string newDirection);
    std::string getDirection();
    void setMode(std::string newMode);
    std::string getMode();
    void fillValues(std::string xml);
    std::string getValues();
	bool getValidate();
	//paulo
	void setNameLabels(QStringList newNames);

public slots:
	virtual void toMode(QString newMode);
	virtual void toNotAvailable();
	virtual void toStDev();
	virtual void toMatrix();
	virtual void showMatrixEditor();
	virtual void setReadOnly(bool);
	virtual void changeValidate(QString);
	virtual void setTitles(QStringList);

signals:
	void changeMatrixButtonVisibility(bool);
	void changeToReadOnly(bool);
	void changeToMode(QString);
	void validateChanged();
};

class SigmaFormTypeSelector: public QComboBox
{
	Q_OBJECT
protected:
	SigmaFormController* controller;
public:
    explicit SigmaFormTypeSelector(QWidget * parent = 0);
	void setSigmaFormController(SigmaFormController* newController);
	void disconnectSigmaFormController(SigmaFormController* oldController);
	void blockCovarianceMatrixOption();
public slots:
	virtual void toMode(QString);
};

class SigmaFormContent: public QFrame
{
	Q_OBJECT
protected:
	SigmaFormController* controller;
public:
    explicit SigmaFormContent(QWidget * parent = 0);
	void setSigmaFormController(SigmaFormController* newController);
};

class SigmaFormDialogButton: public QPushButton
{
	Q_OBJECT
protected:
	SigmaFormController* controller;
public:
    explicit SigmaFormDialogButton(QWidget * parent = 0);
	void setSigmaFormController(SigmaFormController* newController);
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // SIGMAFORM_H
