#include "TableIOEOWidget.h"
#include "QHeaderView"
#include <QLineEdit>

TableIOEOWidget::TableIOEOWidget(QWidget *parent):QTableWidget(parent)
{
    io= new Matrix(1,1);
    setRowCount(io->getRows());
    setColumnCount(io->getCols());
    installEventFilter(this);

}
TableIOEOWidget::TableIOEOWidget(Matrix values, QWidget *parent):QTableWidget(parent)
{
    setTableData(values);
    //connect(this, SIGNAL(itemSelectionChanged()),this,SLOT(autoCopy()));
}

void TableIOEOWidget::setTableData(Matrix values,char mode,int precision)
{
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
    this->resizeColumnsToContents();
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

void TableIOEOWidget::resizeTable()
{
    int widlinhas=lineWidth()*(columnCount()-1);
    int heilinhas=lineWidth()*(rowCount()-1);

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
    if (evento->type()==QEvent::DragMove)
    {
        QDragMoveEvent *dragEvent = static_cast<QDragMoveEvent *>(evento);
        dragMoveEvent(dragEvent);
        return true;
    }
}

void TableIOEOWidget::focusInEvent(QFocusEvent *evento)
{
    emit focusReceived();

    QTableWidget::focusInEvent(evento);

}

void TableIOEOWidget::dragMoveEvent(QDragMoveEvent *event)
{
    autoCopy();
    QTableWidget::dragMoveEvent(event);
}
