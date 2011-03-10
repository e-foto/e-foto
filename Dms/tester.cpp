#include "tester.h"

Tester::Tester()
{
    dmsedit1= new DmsEdit();

    choose= new QComboBox();
    choose->addItem("dms");
    choose->addItem("deg");
    choose->addItem("rad");


//Novo 11.3.08
{
    QSpinBox *decimals= new QSpinBox(this);
    decimals->setMinimum(1);
    decimals->setMaximum(9);
    decimalsChanged(decimals->value());

    QVBoxLayout* verLayout = new QVBoxLayout();
    QHBoxLayout* horLayout = new QHBoxLayout();

    horLayout->addWidget(choose);
    horLayout->addWidget(decimals);
    verLayout->addLayout(horLayout);
    verLayout->addWidget(dmsedit1);

    setLayout(verLayout);

	connect(decimals,SIGNAL(valueChanged(int)),this,SLOT(decimalsChanged(int)));
}

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

//Novo metodo 11.3.08
void Tester::decimalsChanged(int decimals)
{
    dmsedit1->setDecimals(decimals);
}
