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
    installEventFilter(this);
    enableAutoCopy();
    setMode('f');
    setDecimals(6);
    horizontalHeader()->setResizeMode(QHeaderView::Stretch);//novo
    verticalHeader()->setResizeMode(QHeaderView::Stretch);
}
TableIOEOWidget::TableIOEOWidget(Matrix values,char mode,int precision, QWidget *parent):QTableWidget(parent)
{
    //setTableData(values,mode,precision);
    connect(this, SIGNAL(itemSelectionChanged()),this,SLOT(autoCopy()));
    enableAutoCopy();
    setMode(mode);
    setDecimals(precision);
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
    //resizeTable();
}

void TableIOEOWidget::setTableData(Matrix values,char mode,int precision)
{
    horizontalHeader()->setResizeMode(QHeaderView::Stretch);//novo
    verticalHeader()->setResizeMode(QHeaderView::Stretch);  //novo

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
    //resizeColumnsToContents();
    //resizeRowsToContents();
    resizeTable();
}

void TableIOEOWidget::setTableData(EDomElement xml, char mode, int precision)
{
    io= new Matrix();
    io->xmlSetData(xml.getContent());
    setTableData(*io, mode, precision);
}

void TableIOEOWidget::setTableData(string xmlString, char mode, int precision)
{
    io= new Matrix();
    io->xmlSetData(xmlString);
    setTableData(*io, mode, precision);
}

void TableIOEOWidget::resizeTable()
{
    int widlinhas=lineWidth()*(columnCount()-1);
    int heilinhas=lineWidth()*(rowCount()-1);

    resizeRowsToContents();   //novo
    resizeColumnsToContents();//novo

    if (columnCount()==1)
        setBaseSize(columnWidth(0)+widlinhas+verticalHeader()->width()+15,rowCount()*rowHeight(0)+heilinhas+horizontalHeader()->height());
    else
        setBaseSize(columnCount()*columnWidth(0)+widlinhas+verticalHeader()->width(),rowCount()*rowHeight(0)+heilinhas+horizontalHeader()->height());
}

void TableIOEOWidget::setVerticalHeadersLabelsHtml(QStringList list)
{
	verticalHeader()->setVisible(false);
	setColumnCount(columnCount()+1);

	for (int i=0; i<list.size(); i++)
	{
		QLabel *lab=new QLabel(list.at(i));
		lab->setAlignment(Qt::AlignCenter);
		setCellWidget(i,1,lab);

	}
	resizeTable();
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

bool TableIOEOWidget::eventFilter(QObject *obj, QEvent *evento)
{
    if (evento->type()==QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(evento);
        keyPressEvent(keyEvent);
        return true;
	}else if (evento->type()==QEvent::FocusIn)
    {
        QFocusEvent *focusInEvento = static_cast<QFocusEvent *>(evento);
        focusInEvent(focusInEvento);
        //emit focusReceived();
        return true;
	}else
	{
		return QTableWidget::eventFilter(obj,evento);
	}

    /*
    if (evento->type()==QEvent::MouseButtonRelease)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(evento);
        mouseReleaseEvent(mouseEvent);
        return true;
    }
    if (evento->type()==QEvent::Resize)
    {
        QResizeEvent *resEvent= static_cast<QResizeEvent*>(evento);
        resizeEvent(resEvent);
        return true;
    }*/

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

void TableIOEOWidget::focusInEvent(QFocusEvent *evento)
{
    emit focusReceived();

    QTableWidget::focusInEvent(evento);

}
/*
void TableIOEOWidget::mouseReleaseEvent(QMouseEvent *event)
{
    autoCopy();
    //qDebug("Dragmoveevent");
    QTableWidget::mouseReleaseEvent(event);
}


void TableIOEOWidget::resizeEvent(QResizeEvent *event)
{
    if(event->type()==QResizeEvent::Resize)
    {
        qDebug("Evento de resize");
        resizeTable();
    }else
    {
        qDebug("Não evento de resize ");
        QTableWidget::resizeEvent(event);
    }
}
*/
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
