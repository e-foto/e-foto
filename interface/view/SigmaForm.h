#ifndef SIGMAFORM_H
#define SIGMAFORM_H

#include <QWidget>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>

#include "Matrix.h"
#include "EDom.h"

class SigmaFormController: public QObject
{
	Q_OBJECT

protected:
	QWidget* contentWidget;
	QList<QLabel*> labels;
	QList<QLineEdit*> edits;
	QDoubleValidator *validator;
	unsigned int dimension;
	string direction;
	string mode;
	Matrix values;

public:
	SigmaFormController();
	SigmaFormController(string myMode, unsigned int myDimension = 1);
	SigmaFormController(Matrix myValues);

protected:
	void init();
	Matrix stDevToMatrix(deque<double> stDev);
	deque<double> matrixToStDev(Matrix mat);

public:
	QWidget* getContent();
	void setDirection(string newDirection);
	string getDirection();
	void setMode(string newMode);
	string getMode();
	void fillValues(string xml);
	string getValues();
	bool getValidate();

public slots:
	virtual void toMode(QString newMode);
	virtual void toNotAvailable();
	virtual void toStDev();
	virtual void toMatrix();
	virtual void showMatrixEditor();
	virtual void setReadOnly(bool);
	virtual void changeValidate(QString);

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
	SigmaFormTypeSelector(QWidget * parent = 0);
	void setSigmaFormController(SigmaFormController* newController);
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
	SigmaFormContent(QWidget * parent = 0);
	void setSigmaFormController(SigmaFormController* newController);
};

class SigmaFormDialogButton: public QPushButton
{
	Q_OBJECT
protected:
	SigmaFormController* controller;
public:
	SigmaFormDialogButton(QWidget * parent = 0);
	void setSigmaFormController(SigmaFormController* newController);
};

#endif // SIGMAFORM_H
