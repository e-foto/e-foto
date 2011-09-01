#include "ETableWidget.h"
#include "QHeaderView"
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QtGui>

#define MAX(a,b) (a<b) ? b : a
#define MIN(a,b) (a<b) ? a : b


ETableWidget::ETableWidget(QWidget *parent):QTableWidget(parent)
{
	//io= new Matrix(1,1);
	installEventFilter(this);
	setRowCount(0);
	setColumnCount(0);
	setMode('f');
	setDecimals(6);

	horizontalHeader()->setResizeMode(QHeaderView::Stretch);//novo
	verticalHeader()->setResizeMode(QHeaderView::Stretch);
	connect(this, SIGNAL(itemSelectionChanged()),this,SLOT(autoCopy()));
	//connect(this,SIGNAL(itemSelectionChanged()),this,SLOT(selectionBackground()));
	enableAutoCopy(false);
}
ETableWidget::ETableWidget(Matrix values,char mode,int precision, QWidget *parent):QTableWidget(parent)
{
    //setTableData(values,mode,precision);
	installEventFilter(this);
    connect(this, SIGNAL(itemSelectionChanged()),this,SLOT(autoCopy()));
    enableAutoCopy();
    setMode(mode);
    setDecimals(precision);
    setTableData(values,getMode(),getDecimals());

    horizontalHeader()->setResizeMode(QHeaderView::Stretch);//novo
    verticalHeader()->setResizeMode(QHeaderView::Stretch);

	// melhoramento na interface da tabela
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

void ETableWidget::setTableData(Matrix values,char mode,int precision)
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

void ETableWidget::setTableData(EDomElement xml, char mode, int precision)
{
    io= new Matrix();
    io->xmlSetData(xml.getContent());
    setTableData(*io, mode, precision);
}

void ETableWidget::setTableData(string xmlString, char mode, int precision)
{
    io= new Matrix();
    io->xmlSetData(xmlString);
    setTableData(*io, mode, precision);
}

void ETableWidget::resizeTable()
{
    int widlinhas=lineWidth()*(columnCount()-1);
    int heilinhas=lineWidth()*(rowCount()-1);

    resizeRowsToContents();   //novo
    resizeColumnsToContents();//novo

    if (columnCount()==1)
        setBaseSize(columnWidth(0)+widlinhas+verticalHeader()->width()+15,rowCount()*rowHeight(0)+heilinhas+horizontalHeader()->height());
    else
        setBaseSize(columnCount()*columnWidth(0)+widlinhas+verticalHeader()->width(),rowCount()*rowHeight(0)+heilinhas+horizontalHeader()->height());

	resizeRowsToContents();   //novo
	resizeColumnsToContents();//novo
}

void ETableWidget::setVerticalHeadersLabelsHtml(QStringList list)
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

void ETableWidget::autoCopy()
{
    QString copyStr;
	if (selectedRanges().size()!=0)
    {
		int rows=selectedRanges().at(0).rowCount();
		int cols=selectedRanges().at(0).columnCount();
		/*qDebug("rows: %d cols: %d",rows,cols);

		for (int i=0;i<selectedItems().size();i++)
			qDebug("%s",selectedItems().at(i)->text().toStdString().c_str());
*/
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

bool ETableWidget::eventFilter(QObject *obj, QEvent *evento)
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
	}else if (evento->type()==QEvent::FocusOut)
	{
		QFocusEvent *focusOutEvento = static_cast<QFocusEvent *>(evento);
		focusOutEvent(focusOutEvento);
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

void ETableWidget::keyPressEvent(QKeyEvent *event)
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

void ETableWidget::focusInEvent(QFocusEvent *evento)
{
	if(evento->reason()==Qt::OtherFocusReason)
	{
		qDebug()<<"app call:: focus in";
	}
	else
	{
		QTableWidget::focusInEvent(evento);
		qDebug()<<"outro motivo:: focus in "<<evento->reason();
	}
}

void ETableWidget::focusOutEvent(QFocusEvent *evento)
{
	if(evento->reason()==Qt::OtherFocusReason)
	{
		evento->gotFocus();
		//evento->
		qDebug()<<"app call:: focus out";
	}
	else
	{
		qDebug()<<"outro motivo::focus out "<<evento->reason();
		QTableWidget::focusOutEvent(evento);
	}
}

void ETableWidget::enableAutoCopy(bool enable)
{
    //testado e at√© agora sem problemas
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
void ETableWidget::enableAutoCopy(int enable)
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

void ETableWidget::updateTableValues(char mode, int precision)
{
	for (int i=0;i<rowCount();i++)
		for(int j=0;j<columnCount();j++)
			item(i,j)->setText(QString::number(io->get(i+1,j+1),mode,precision));
	resizeTable();
}

void ETableWidget::updateDecimalsValues(int decimal)
{
    setDecimals(decimal);
    updateTableValues(getMode(),getDecimals());
}

void ETableWidget::updateModoValues(int modo)
{
    if (modo==0)            // 'e' == Cientifico
    {
        setMode('e');
    }
    else if (modo==1)       // 'f' == Decimal
    {
        setMode('f');
    }
    else                    // caso em que um valor invalido √© passado
        setMode('f');

    updateTableValues(getMode(),getDecimals());
}

void ETableWidget::setMode(char newMode)
{
    mode=( (newMode=='e' || newMode=='f')? newMode: 'e');
}

void ETableWidget::setDecimals(int newDecimals)
{
	decimals=(newDecimals>=0 ? newDecimals : 1 );
}

char ETableWidget::getMode()
{
    return mode;
}

int ETableWidget::getDecimals()
{
    return decimals;
}

void ETableWidget::putIn(Matrix input, int row, int column, char mode, int precision)
{
	setMode(mode);
	setDecimals(precision);
	//horizontalHeader()->setResizeMode(QHeaderView::Stretch);//novo
	//verticalHeader()->setResizeMode(QHeaderView::Stretch);  //novo
	int newRows=MAX(row-1+input.getRows(),rowCount());
	int newCols=MAX(column-1+input.getCols(),columnCount());

	setRowCount(newRows);
	setColumnCount(newCols);
	//qDebug("[%d,%d]",newRows,newCols);
	for (int i=1;i<=input.getRows();i++)
	{
		for(int j=1;j<=input.getCols();j++)
		{
			QTableWidgetItem *temp= new QTableWidgetItem(QString::number(input.get(i,j),getMode(),getDecimals()));
			temp->setTextAlignment(Qt::AlignCenter);
			temp->setFlags(Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled);
			//qDebug("set[%d,%d] = %.3f in [%d,%d]",i,j,input.get(i,j),i+row-1,j-1+column);
			this->setItem(i+row-1,j+column-1,temp);
		}
	}
	//resizeTable();
}

void ETableWidget::putInColumn(QStringList list, int column)
{
	if(columnCount()<column)
		setColumnCount(column);

	//qDebug() << list;
	//horizontalHeader()->setResizeMode(QHeaderView::Stretch);//novo
	//verticalHeader()->setResizeMode(QHeaderView::Stretch);  //novo
	setRowCount(list.size());

	//qDebug("rows: %d \tcols: %d",rowCount(),columnCount());
	for(int i=0;i<rowCount();i++)
	{
		if (item(i,column-1)==0)
		{
			QTableWidgetItem *temp= new QTableWidgetItem(list.at(i));
			temp->setTextAlignment(Qt::AlignCenter);
			temp->setFlags(Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled);
			setItem(i,column-1,temp);
			//qDebug("Nulo: [%d,%d]= %s",i,column,list.at(i).toStdString().c_str());
		}else
		{
			//qDebug("Nao Nulo: [%d,%d]= %s",i,column,item(i,column)->text().toStdString().c_str());
		}
	}
	//resizeTable();
}

void ETableWidget::putInRow(QStringList list, int row)
{
	if(rowCount()<row)
		setRowCount(row);

	setColumnCount(list.size());
	horizontalHeader()->setResizeMode(QHeaderView::Stretch);//novo
	verticalHeader()->setResizeMode(QHeaderView::Stretch);  //novo
	for(int j=0;j<columnCount();j++)
	{
		QTableWidgetItem *temp= new QTableWidgetItem(list.at(j));
		temp->setTextAlignment(Qt::AlignCenter);
		temp->setFlags(Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled);
		this->setItem(row,j,temp);
	}
	resizeTable();
}

void ETableWidget::selectionBackground()
{
	if(!hasFocus())
	{
		//qDebug()<<"n„o tem focus mas a seleÁ„o mudou "<<objectName();
		//this->updateMicroFocus();
		//this->adjustSize();
	}else
	{
		//qDebug()<<"Tem focus mas a seleÁ„o mudou "<<objectName();
	}

}

void ETableWidget::clearSelection()
{
	setRangeSelected(QTableWidgetSelectionRange(0,0,rowCount()-1,columnCount()-1),false);
}
