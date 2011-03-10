#include "TableIOEOWidget.h"
#include <QApplication>
#include <QtGui>
#include <QObject>

int main(int argc,char ** argv)
{
    QApplication aplic(argc,argv);

    stringstream aux;
    aux << "<mml:matrix>\n";
    aux << "<mml:matrixrow>\n";
    aux << "<mml:cn>0.001098026866230257</mml:cn>\n";
    aux << "<mml:cn>-3.282247427218354e-007</mml:cn>\n";
    aux << "<mml:cn>-3.103369313837061e-007</mml:cn>\n";
    aux << "<mml:cn>0</mml:cn>\n";
    aux << "<mml:cn>0</mml:cn>\n";
    aux << "<mml:cn>0</mml:cn>\n";
    aux << "</mml:matrixrow>\n";
    aux << "<mml:matrixrow>\n";
    aux << "<mml:cn>-3.282247427218354e-007</mml:cn>\n";
    aux << "<mml:cn>2.282550119234003e-010</mml:cn>\n";
    aux << "<mml:cn>-1.717466074703072e-013</mml:cn>\n";
    aux << "<mml:cn>0</mml:cn>\n";
    aux << "<mml:cn>0</mml:cn>\n";
    aux << "<mml:cn>0</mml:cn>\n";
    aux << "</mml:matrixrow>\n";
    aux << "<mml:matrixrow>\n";
    aux << "<mml:cn>-3.103369313837062e-007</mml:cn>\n";
    aux << "<mml:cn>-1.717466074703072e-013</mml:cn>\n";
    aux << "<mml:cn>2.277426762616725e-010</mml:cn>\n";
    aux << "<mml:cn>0</mml:cn>\n";
    aux << "<mml:cn>0</mml:cn>\n";
    aux << "<mml:cn>0</mml:cn>\n";
    aux << "</mml:matrixrow>\n";
    aux << "<mml:matrixrow>\n";
    aux << "<mml:cn>0</mml:cn>\n";
    aux << "<mml:cn>0</mml:cn>\n";
    aux << "<mml:cn>0</mml:cn>\n";
    aux << "<mml:cn>0.001098026866230257</mml:cn>\n";
    aux << "<mml:cn>-3.282247427218354e-007</mml:cn>\n";
    aux << "<mml:cn>-3.103369313837061e-007</mml:cn>\n";
    aux << "</mml:matrixrow>\n";
    aux << "<mml:matrixrow>\n";
    aux << "<mml:cn>0</mml:cn>\n";
    aux << "<mml:cn>0</mml:cn>\n";
    aux << "<mml:cn>0</mml:cn>\n";
    aux << "<mml:cn>-3.282247427218354e-007</mml:cn>\n";
    aux << "<mml:cn>2.282550119234003e-010</mml:cn>\n";
    aux << "<mml:cn>-1.717466074703072e-013</mml:cn>\n";
    aux << "</mml:matrixrow>\n";
    aux << "<mml:matrixrow>\n";
    aux << "<mml:cn>0</mml:cn>\n";
    aux << "<mml:cn>0</mml:cn>\n";
    aux << "<mml:cn>0</mml:cn>\n";
    aux << "<mml:cn>-3.103369313837062e-007</mml:cn>\n";
    aux << "<mml:cn>-1.717466074703072e-013</mml:cn>\n";
    aux << "<mml:cn>2.277426762616725e-010</mml:cn>\n";
    aux << "</mml:matrixrow>\n";
    aux << "</mml:matrix>\n";

    EDomElement teste2(aux.str());

    //Matrix teste(6,6);
    Matrix teste;

    for (int i=0;i<teste.getRows();i++)
    {
        for(int j=0;j<teste.getCols();j++)
        {
            teste.set(i+1,j+1,(j+teste.getCols()*i)-12.0);
        }
    }

    teste.xmlSetData(teste2.getContent());
    TableIOEOWidget *tabela= new TableIOEOWidget(teste);//teste2);//teste);

//Novo 11.03.08
    {
    QWidget *tela=new QWidget();
    QVBoxLayout* verLayout = new QVBoxLayout(tela);
    QHBoxLayout *horLayout = new QHBoxLayout();

    QCheckBox *caixa=new QCheckBox("Enable AutoCopy",tela);
    caixa->setChecked(true);
    QComboBox *modoCombo= new QComboBox();
    modoCombo->addItem("Cientifico");        //'e'
    modoCombo->addItem("Decimal");           //'f'
    QSpinBox *decimals = new QSpinBox();
    decimals->setMinimum(1);
    decimals->setMaximum(9);

    horLayout->addWidget(caixa);
    horLayout->addWidget(modoCombo);
    horLayout->addWidget(decimals);
    verLayout->addLayout(horLayout);
    verLayout->addWidget(tabela);

    tabela->updateDecimalsValues(decimals->value());
    tabela->updateModoValues(modoCombo->currentIndex());

    tela->connect(caixa,SIGNAL(stateChanged(int)),tabela,SLOT(enableAutoCopy(int)));
    tela->connect(modoCombo,SIGNAL(currentIndexChanged(int)),tabela,SLOT(updateModoValues(int)));
    tela->connect(decimals,SIGNAL(valueChanged(int)),tabela,SLOT(updateDecimalsValues(int)));

    tela->setLayout(verLayout);
    tela->show();
    }

    return aplic.exec();
}
