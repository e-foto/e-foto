#ifndef DEGREELINEEDIT_H
#define DEGREELINEEDIT_H

#include <QObject>
#include <QLineEdit>
#include "Dms.h"
#include <QKeyEvent>
#include <QEvent>
#include <QValidator>
#include <QDoubleValidator>


enum PositionValue{ NONE = -1 ,DEGREES, MINUTES, SECONDS};
enum DmsEditMode { DMS, DEG, RAD };

class DmsEdit : public QLineEdit
{
    Q_OBJECT
protected:
    Dms *dmsMin;
    Dms *dmsMax;
    Dms *oldDegMinSecLine;
    double radMax,radMin,degMin,degMax;

public:
    DmsEdit(QWidget *parent=0);
    void setDecimals(int newDecimals);
    int getDecimals();
    DmsEditMode getDmsEditMode();
    double getRadianValue();
    double getDegreeValue();
    Dms* getDmsValue();
    void setDmsMinimum(int degree,int minutes, double seconds, bool signal);
    void setDmsMaximum(int degree,int minutes, double seconds, bool signal);

    double getMinimumDeg();
    double getMaximumDeg();
    double getMinimumRad();
    double getMaximumRad();
    Dms * getMinimumDms();
    Dms * getMaximumDms();


protected:

    bool eventFilter(QObject* objeto, QEvent* evento);
    void keyPressEvent(QKeyEvent *evento);
	void focusInEvent(QFocusEvent *);
	void focusOutEvent(QFocusEvent *);

private:
    DmsEditMode mode;
    PositionValue positionValue();
    PositionValue positionValue(int pos);
    Dms *degMinSecLine;
    double radValue;
    double degValue;
    int decimals;
    QString previousValue;
    //QRegExpValidator *validatorDegree;
    QDoubleValidator *degValidator;
    QDoubleValidator *radValidator;

public slots:
    void validate();
    void stepDown();
    void stepUp();
    void stepBy(int steps);
    void selectField(PositionValue pos);
    void changedField(int oldPos, int newPos);
    void setDmsEditMode(DmsEditMode newMode);
    void updateValue(QString newValue="");

};

#endif // DEGREELINEEDIT_H
