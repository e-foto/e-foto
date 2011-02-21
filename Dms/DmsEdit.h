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
public:
    DmsEdit(QWidget *parent=0);
    void setDecimals(int newDecimals);
    int getDecimals();
    DmsEditMode getDmsEditMode();

protected:
    bool eventFilter(QObject* objeto, QEvent* evento);
    void keyPressEvent(QKeyEvent *evento);
    void focusInEvent(QFocusEvent *);

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

signals:

public slots:
    void validate();
    void stepDown();
    void stepUp();
    void stepBy(int steps);
    void selectField(PositionValue pos);
    void changedField(int oldPos, int newPos);
    void setDmsEditMode(DmsEditMode newMode);
    void updateValue(QString newValue);

};

#endif // DEGREELINEEDIT_H
