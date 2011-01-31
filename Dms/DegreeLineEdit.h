#ifndef DEGREELINEEDIT_H
#define DEGREELINEEDIT_H

#include <QObject>
#include <QLineEdit>
#include "Dms.h"

class DegreeLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    DegreeLineEdit(QWidget *parent=0);

private:
	Dms *degMinSecLine;
    QRegExpValidator *validatorDegree;
signals:


public slots:
    void validate(QString);

};

#endif // DEGREELINEEDIT_H
