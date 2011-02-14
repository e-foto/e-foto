#ifndef DEGREELINEEDIT_H
#define DEGREELINEEDIT_H

#include <QObject>
#include <QLineEdit>
#include "Dms.h"
#include <QKeyEvent>
#include <QEvent>

enum PositionValue{ NONE = -1 ,DEGREES, MINUTES, SECONDS};

class DmsEdit : public QLineEdit
{
    Q_OBJECT
public:
    DmsEdit(QWidget *parent=0);
    void setDecimals(int newDecimals);
    int getDecimals();

protected:
    bool eventFilter(QObject* objeto, QEvent* evento);
    void keyPressEvent(QKeyEvent *evento);
    void focusInEvent(QFocusEvent *);

private:
    bool direction;//True =esquerda pra direita //False ==direita pra esquerda
    PositionValue positionValue();
    PositionValue positionValue(int pos);
    Dms *degMinSecLine;
    int decimals;
    QString previousValue;
    QRegExpValidator *validatorDegree;

signals:

public slots:
    void validate(QString);
    void stepDown();
    void stepUp();
    void stepBy(int steps);
    void selectField(PositionValue pos);
    void changedField(int oldPos, int newPos);

};

#endif // DEGREELINEEDIT_H
