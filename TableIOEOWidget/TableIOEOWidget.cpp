#include "TableIOEOWidget.h"
#include "QHeaderView"
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

TableIOEOWidget::TableIOEOWidget(QWidget *parent):QTableWidget(parent)
{
    io= new Matrix(1,1);
    setRowCount(io->getRows());
    setColumnCount(io->getCols());
    enableAutoCopy();
    setMode('f');
    setDecimals(6);
    horizontalHeader()->setResizeMode(QHeaderView::Stretch);//novo
    verticalHeader()->setResizeMode(QHeaderView::Stretch);
}
TableIOEOWidget::TableIOEOWidget(Matrix values, QWidget *parent):QTableWidget(parent)
{
    enableAutoCopy();
    setMode('e');
    setDecimals(6);
    setTableData(values,getMode(),getDecimals());

    horizontalHeader()->setResizeMode(QHeaderView::Stretch);//novo
    verticalHeader()->setResizeMode(QHeaderView::Stretch);
/*
    QVBoxLayout* verLayout = new QVBoxLayout();
    QHBoxLayout *horLayout = new QHBoxLayout();

    QCheckBox *caixa=new QCheckBox("Enable AutoCopy");
    caixa->setChecked(true);
    QComboBox *modo= new QComboBox();
    modo->addItem("Cientifico"); //'e'
    modo->addItem("Decimal");    //'f'
    QSpinBox *decimals = new QSpinBox();
    decimals->setMinimum(1);
    decimals->setMaximum(9);

    horLayout->addWidget(caixa);
    horLayout->addWidget(modo);
    horLayout->addWidget(decimals);
    verLayout->addWidget(caixa);
    verLayout->addWidget(tabela);
*/

    //connect(this, SIGNAL())
    //connect(this, SIGNAL(itemSelectionChanged()),this,SLOT(autoCopy()));
}

void TableIOEOWidget::setTableData(Matrix values,char mode,int precision)
{
    setSelectionMode(QAbstractItemView::ContiguousSelection);
    //falta tratamento para destruir antiga matrix se ela existir antes
    io= new Matrix(values);

    setMode(mode);
    setDecimals(precision);

    setRowCount(io->getRows());
    setColumnCount(io->getCols());
    for (int i=0;i<rowCount();i++)
    {
        for(int j=0;j<columnCount();j++)
        {
            QTableWidgetItem *temp= new QTableWidgetItem(QString::number(io->get(i+1,j+1),getMode(),getDecimals()));
            temp->setTextAlignment(Qt::AlignCenter);
            temp->setFlags(Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled);
            this->setItem(i,j,temp);
        }
    }
    //this->resizeColumnsToContents();
    resizeTable();
}

void TableIOEOWidget::setTableData(EDomElement xml, char mode, int precision)
{
    io= new Matrix();
    io->xmlSetData(xml.getContent());
    setTableData(*io);
}

void TableIOEOWidget::setTableData(string xmlString, char mode, int precision)
{
    io= new Matrix();
    io->xmlSetData(xmlString);
    setTableData(*io);
}

void TableIOEOWidget::autoCopy()
{
    QString copyStr;
    if (selectedRanges().size()!=0)
    {
        int cols=selectedRanges().at(0).columnCount();
        int rows=selectedRanges().at(0).rowCount();
        for (int i=0;i<rows;i++)
        {
            for (int j=0;j<cols;j++)
            {
                copyStr.append(selectedItems().at(j*rows+i)->text()).append("\t");
            }
            copyStr.append("\n");
        }
        //Gambiarra muito da feia, para acessar o clipboard do sistema
        QLineEdit aux(copyStr);
        aux.selectAll();
        aux.copy();
    }
}

void TableIOEOWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers()==Qt::ControlModifier)
    {
        if (event->key()==Qt::Key_C)
        {
            autoCopy();
        }
        else
        {
            QTableWidget::keyPressEvent(event);
        }
    }
    else
        QTableWidget::keyPressEvent(event);
}

void TableIOEOWidget::resizeEvent(QResizeEvent *event)
{
    if (event->type()==QResizeEvent::Resize)
    {
        resizeTable();
    }else
    {
        //QTableWidget::resizeEvent(event);
    }
}

void TableIOEOWidget::resizeTable()
{
    int widlinhas=lineWidth()*columnCount();
    int heilinhas=lineWidth()*rowCount();

    horizontalHeader()->stretchLastSection();
    verticalHeader()->stretchLastSection();
    resizeColumnsToContents();//novo
    resizeRowsToContents();   //novo
    setFixedSize(columnCount()*columnWidth(0)+widlinhas+verticalHeader()->width(),rowCount()*rowHeight(0)+heilinhas+horizontalHeader()->height());
}

void TableIOEOWidget::enableAutoCopy(bool enable)
{
    //testado e até agora sem problemas
    if(enable)
    {
        connect(this,SIGNAL(itemSelectionChanged()),this,SLOT(autoCopy()));
    }
    else
    {
        disconnect(this,SIGNAL(itemSelectionChanged()),this,SLOT(autoCopy()));
    }
}

//Novo 11.03.08
//Override para connect com o signal da QCheckBox, a saber, "stateChanged(int)"
void TableIOEOWidget::enableAutoCopy(int enable)
{

    if(enable==0)
    {
        disconnect(this,SIGNAL(itemSelectionChanged()),this,SLOT(autoCopy()));
    }
    else
    {
        connect(this,SIGNAL(itemSelectionChanged()),this,SLOT(autoCopy()));
    }
}

void TableIOEOWidget::updateTableValues(char mode, int precision)
{
    bool ok;
    for (int i=0;i<rowCount();i++)
    {
        for(int j=0;j<columnCount();j++)
        {
            //double tempValue=this->item(i,j)->text().toDouble(&ok);
            item(i,j)->setText(QString::number(io->get(i+1,j+1),mode,precision));
        }
    }
    //this->resizeColumnsToContents();
    resizeTable();
}

void TableIOEOWidget::updateDecimalsValues(int decimal)
{
    setDecimals(decimal);
    updateTableValues(getMode(),getDecimals());
}

void TableIOEOWidget::updateModoValues(int modo)
{
    if (modo==0)            // 'e' == Cientifico
    {
        setMode('e');
    }
    else if (modo==1)       // 'f' == Decimal
    {
        setMode('f');
    }
    else                    // caso em que um valor invalido é passado
        setMode('f');

    updateTableValues(getMode(),getDecimals());
}

void TableIOEOWidget::setMode(char newMode)
{
    mode=( (newMode=='e' || newMode=='f')? newMode: 'e');
}

void TableIOEOWidget::setDecimals(int newDecimals)
{
    decimals=(newDecimals>0 ? newDecimals : 1 );
}

char TableIOEOWidget::getMode()
{
    return mode;
}

int TableIOEOWidget::getDecimals()
{
    return decimals;
}
