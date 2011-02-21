#include "tester.h"

Tester::Tester()
{
    dmsedit1= new DmsEdit();

    choose= new QComboBox();
    choose->addItem("dms");
    choose->addItem("deg");
    choose->addItem("rad");

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(choose);
    layout->addWidget(dmsedit1);

    setLayout(layout);
    connect(choose,SIGNAL(currentIndexChanged(int)),this,SLOT(changeMode(int)));
}

void Tester::changeMode(int mode)
{
    if (mode ==0)
    {
        dmsedit1->setDmsEditMode(DMS);
    }
    else if(mode ==1)
    {
        dmsedit1->setDmsEditMode(DEG);
    }
    else
    {
        dmsedit1->setDmsEditMode(RAD);
    }
}
