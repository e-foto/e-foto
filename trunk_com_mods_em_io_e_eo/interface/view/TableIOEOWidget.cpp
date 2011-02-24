#include "TableIOEOWidget.h"
#include "QHeaderView"
#include <QLineEdit>

TableIOEOWidget::TableIOEOWidget(QWidget *parent):QTableWidget(parent)
{
    io= new Matrix(1,1);
    setRowCount(io->getRows());
    setColumnCount(io->getCols());
    installEventFilter(this);
    enableAutoCopy();

}
TableIOEOWidget::TableIOEOWidget(Matrix values, QWidget *parent):QTableWidget(parent)
{
    setTableData(values);
    connect(this, SIGNAL(itemSelectionChanged()),this,SLOT(autoCopy()));
    enableAutoCopy();
}

void TableIOEOWidget::setTableData(Matrix values,char mode,int precision)
{
    horizontalHeader()->setResizeMode(QHeaderView::Stretch);//novo
    verticalHeader()->setResizeMode(QHeaderView::Stretch);  //novo

    setSelectionMode(QAbstractItemView::ContiguousSelection);
    io= new Matrix(values);
    setRowCount(io->getRows());
    setColumnCount(io->getCols());
    for (int i=0;i<rowCount();i++)
    {
        for(int j=0;j<columnCount();j++)
        {
            QTableWidgetItem *temp= new QTableWidgetItem(QString::number(io->get(i+1,j+1),mode,precision));
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
/*
    for (int i=0;i<columnCount();i++)
        setColumnWidth(i,(this->width()-this->verticalHeader()->width()-widlinhas)/columnCount());
    for (int j=0;j<rowCount();j++)
        setRowHeight(j,(this->height()-this->horizontalHeader()->height()-heilinhas)/rowCount());
*/
    resizeColumnsToContents();//novo
    resizeRowsToContents();   //novo
    if (columnCount()==1)
        setFixedSize(columnCount()*columnWidth(0)+widlinhas+verticalHeader()->width()+10,rowCount()*rowHeight(0)+heilinhas+horizontalHeader()->height());
    else
        setFixedSize(columnCount()*columnWidth(0)+widlinhas+verticalHeader()->width(),rowCount()*rowHeight(0)+heilinhas+horizontalHeader()->height());
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
    }
    if (evento->type()==QEvent::FocusIn)
    {
        QFocusEvent *focusInEvento = static_cast<QFocusEvent *>(evento);
        focusInEvent(focusInEvento);
        //emit focusReceived();
        return true;
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
    //em teste
    if(enable)
    {
        connect(this,SIGNAL(itemSelectionChanged()),this,SLOT(autoCopy()));
    }
    else
    {
        disconnect(this,SIGNAL(itemSelectionChanged()),this,SLOT(autoCopy()));
    }
}
