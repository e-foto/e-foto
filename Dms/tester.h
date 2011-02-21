#ifndef TESTER_H
#define TESTER_H

#include "DmsEdit.h"
#include <QtGui>

class Tester: public QWidget
{
    Q_OBJECT
public :
    Tester();
    DmsEdit *dmsedit1;
   // DmsEdit *dmsedit2;
    //DmsEdit *dmsedit3;
    QComboBox *choose;

public slots:
    void changeMode(int mode);

};


#endif // TESTER_H
