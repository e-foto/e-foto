/*Copyright 2002-2014 e-foto team (UERJ)
  This file is part of e-foto.

    e-foto is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    e-foto is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with e-foto.  If not, see <http://www.gnu.org/licenses/>.
*/
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

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

ETableWidget::ETableWidget(QWidget *parent): QTableWidget(parent)
{
		//io= new Matrix(1,1);
		currentSpinBoxRow=currentSpinBoxColumn=currentDoubleSpinBoxColumn=currentDoubleSpinBoxRow=-1;
		installEventFilter(this);
		setRowCount(0);
		setColumnCount(0);
		setMode('f');
		setDecimals(6);

		horizontalHeader()->setResizeMode(QHeaderView::Stretch);
		verticalHeader()->setResizeMode(QHeaderView::Stretch);

		/*
		horizontalHeader()->setResizeMode(QHeaderView::Interactive);
		horizontalHeader()->setStretchLastSection(true);
		horizontalHeader()->setMinimumSectionSize(-1);
*/
		/*
		verticalHeader()->setResizeMode(QHeaderView::Interactive);
		verticalHeader()->setStretchLastSection(true);
		verticalHeader()->setMinimumSectionSize(-1);*/

		connect(this, SIGNAL(itemSelectionChanged()),this,SLOT(autoCopy()));
		//connect(this,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(avaliateType(QTableWidgetItem *)));

		//connect(this,SIGNAL(itemSelectionChanged()),this,SLOT(selectionBackground()));
		enableAutoCopy(false);
}
ETableWidget::ETableWidget(Matrix values,char mode,int precision, QWidget *parent):QTableWidget(parent)
{
	//setTableData(values,mode,precision);

	currentSpinBoxRow=currentSpinBoxColumn=currentDoubleSpinBoxColumn=currentDoubleSpinBoxRow=-1;
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
	/*
	int widthSection=this->width()/columnCount();
	qDebug("widthSection: %d",widthSection);
	this->horizontalHeader()->setMinimumSectionSize(widthSection);*/
}

/*
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
}*/

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
	}else /*if (evento->type()==QEvent::Resize)
	{
		QResizeEvent *resEvent= static_cast<QResizeEvent*>(evento);
		resizeTable();
		//resizeEvent(resEvent);
		return true;
	}else*/
	{
		return QTableWidget::eventFilter(obj,evento);
	}

	/*
	if (evento->type()==QEvent::MouseButtonRelease)
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(evento);
		mouseReleaseEvent(mouseEvent);
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

/*
void ETableWidget::focusInEvent(QFocusEvent *evento)
{
		if(evento->reason()==Qt::OtherFocusReason)
		{
				//qDebug()<<"app call:: focus in";
		}
		else
		{
				QTableWidget::focusInEvent(evento);
				//qDebug()<<"outro motivo:: focus in "<<evento->reason();
		}
}
*/
/*
void ETableWidget::focusOutEvent(QFocusEvent *evento)
{
		if(evento->reason()==Qt::OtherFocusReason)
		{
				evento->gotFocus();
				//evento->
				//qDebug()<<"app call:: focus out";
		}
		else
		{
				//qDebug()<<"outro motivo::focus out "<<evento->reason();
				QTableWidget::focusOutEvent(evento);
		}
}
*/

void ETableWidget::enableAutoCopy(bool enable)
{
	//testado e atÃ© agora sem problemas
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
	else                    // caso em que um valor invalido Ã© passado
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

		horizontalHeader()->setMinimumSectionSize(this->width()/newCols);
		for (int i=0;i<input.getRows();i++)
		{
				for(int j=0;j<input.getCols();j++)
				{
						QTableWidgetItem *temp= new QTableWidgetItem(QString::number(input.get(i+1,j+1),getMode(),getDecimals()));
						temp->setTextAlignment(Qt::AlignCenter);
						temp->setFlags(Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled);
						//qDebug("set[%d,%d] = %.3f in [%d,%d]",i,j,input.get(i,j),i+row-1,j-1+column);
						this->setItem(i+row,j+column,temp);
				}
		}


}

void ETableWidget::clearSelection()
{
		setRangeSelected(QTableWidgetSelectionRange(0,0,rowCount()-1,columnCount()-1),false);
}


void ETableWidget::putInColumn(QStringList list, int column)//, QString type, bool enable, PositionMatrix *limits)
{
		//Em breve esse metodo sera melhorado e sera usado o insertColumn
		//insertColumn(column);
		if(columnCount()<column)
				setColumnCount(column+1);

		setRowCount(list.size());
		horizontalHeader()->setMinimumSectionSize(this->width()/columnCount());
		for(int i=0;i<rowCount();i++)
		{
				if (item(i,column)==0)
				{
						QTableWidgetItem *temp= new QTableWidgetItem(list.at(i));
						temp->setTextAlignment(Qt::AlignCenter);
						temp->setFlags(Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled);
						setItem(i,column,temp);
				}
		}

}

/*
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

void ETableWidget::setColumnType(int colIndex, QString type, bool enable, double minValue, double maxValue)
{
		int rows=rowCount();
		bool ok;
		setType(type);
		if (type=="QSpinBox")
		{
				for (int i=0;i<rows;i++)
				{
						QSpinBox *spinBox= new QSpinBox();
						spinBox->setAlignment(Qt::AlignHCenter);
						spinBox->setButtonSymbols(QSpinBox::NoButtons);
						spinBox->setEnabled(enable);
						spinBox->setMinimum((int)minValue);
						spinBox->setMaximum((int)maxValue);

						if (item(i,colIndex)!=NULL)
								spinBox->setValue(item(i,colIndex)->text().toInt(&ok));

						//connect(spinBox,SIGNAL(valueChanged(int)),this,SIGNAL());
						//connect(spinBox,SIGNAL())
						this->setCellWidget(i,colIndex,spinBox);
				}
		}else if (type=="QDoubleSpinBox")
		{
				for (int i=0;i<rows;i++)
				{
						QDoubleSpinBox *doubleSpinBox= new QDoubleSpinBox();
						doubleSpinBox->setAlignment(Qt::AlignHCenter);
						doubleSpinBox->setButtonSymbols(QDoubleSpinBox::NoButtons);
						doubleSpinBox->setEnabled(enable);
						doubleSpinBox->setMinimum(minValue);
						doubleSpinBox->setMaximum(maxValue);

						if (item(i,colIndex)!=NULL)
								doubleSpinBox->setValue(item(i,colIndex)->text().toDouble(&ok));

						this->setCellWidget(i,colIndex,doubleSpinBox);
				}
		}

}

void ETableWidget::avaliateType(QTableWidgetItem *item)
{
		bool ok;
		int row=item->row();
		int col=item->column();

		if (currentSpinBoxRow !=-1 || currentSpinBoxColumn !=-1 ||currentDoubleSpinBoxColumn !=-1 || currentDoubleSpinBoxRow !=-1)
				validateItem();

		if (checkTypeds(col,"QSpinBox"))
		{
				/*
				int value=itemSpinBox->value();
				QTableWidgetItem *item= new QTableWidgetItem(QString::number(value));
				item->setTextAlignment(Qt::AlignCenter);
				//this->removeCellWidget(row,col);
				this->setItem(row,col,item);
				itemSpinBox=NULL;
				emit validatedItem(row,col,value);
				*//*
				itemSpinBox= new QSpinBox();
				itemSpinBox->setAlignment(Qt::AlignHCenter);
				itemSpinBox->setButtonSymbols(QSpinBox::NoButtons);
				itemSpinBox->setMaximum(999999999);
				itemSpinBox->setValue(item->text().toInt(&ok));
				itemSpinBox->selectAll();
				previousValue=item->text().toDouble(&ok);
				currentSpinBoxRow=row;
				currentSpinBoxColumn=col;
				connect(itemSpinBox,SIGNAL(editingFinished()),this,SLOT(validateItem()));
				setCellWidget(row,col,itemSpinBox);
				itemSpinBox->setFocus(Qt::OtherFocusReason);

		}
		if (checkTypeds(col,"QDoubleSpinBox"))
		{	/*
				double value=itemDoubleSpinBox->value();
				QTableWidgetItem *item= new QTableWidgetItem(QString::number(value));
				item->setTextAlignment(Qt::AlignCenter);
				this->removeCellWidget(row,col);
				this->setItem(row,col,item);
				itemSpinBox=NULL;
				emit validatedItem(row,col,value);
				*//*
				itemDoubleSpinBox= new QDoubleSpinBox();
				itemDoubleSpinBox->setAlignment(Qt::AlignHCenter);
				itemDoubleSpinBox->setButtonSymbols(QDoubleSpinBox::NoButtons);
				//itemDoubleSpinBox->setEnabled(enable);
				//itemDoubleSpinBox->setMinimum((int)minValue);
				//itemDoubleSpinBox->setMaximum((int)maxValue);
				itemDoubleSpinBox->setMaximum(999999999.999999999);
				itemDoubleSpinBox->setValue(item->text().toDouble(&ok));
				previousValue=item->text().toDouble(&ok);
				currentDoubleSpinBoxRow=row;
				currentDoubleSpinBoxColumn=col;
				connect(itemDoubleSpinBox,SIGNAL(editingFinished()),this,SLOT(validateItem()));
				setCellWidget(row,col,itemDoubleSpinBox);

				itemDoubleSpinBox->setFocus(Qt::OtherFocusReason);

				itemDoubleSpinBox->selectAll();
		}
		/*
		if (getType()=="NONE")
				return;
		if (getType()=="QSpinBox")
		{
				itemSpinBox= new QSpinBox();
				itemSpinBox->setAlignment(Qt::AlignHCenter);
				itemSpinBox->setButtonSymbols(QSpinBox::NoButtons);
				//itemSpinBox->setEnabled(enable);
				//itemSpinBox->setMinimum((int)minValue);
				//itemSpinBox->setMaximum((int)maxValue);
				itemSpinBox->setMaximum(10000);

				itemSpinBox->setValue(item->text().toInt(&ok));
				connect(itemSpinBox,SIGNAL(editingFinished()),this,SLOT(validateItem()));
				setCellWidget(row,col,itemSpinBox);
				spinBoxRow=row;
				spinBoxColumn=col;
		}
		if (getType()=="QDoubleSpinBox")
		{
				itemDoubleSpinBox= new QDoubleSpinBox();
				itemDoubleSpinBox->setAlignment(Qt::AlignHCenter);
				itemDoubleSpinBox->setButtonSymbols(QDoubleSpinBox::NoButtons);
				//itemDoubleSpinBox->setEnabled(enable);
				//itemDoubleSpinBox->setMinimum((int)minValue);
				//itemDoubleSpinBox->setMaximum((int)maxValue);

				itemDoubleSpinBox->setValue(item->text().toInt(&ok));
				connect(itemDoubleSpinBox,SIGNAL(editingFinished()),this,SLOT(validateItem()));
				setCellWidget(row,col,itemDoubleSpinBox);
				doubleSpinBoxRow=row;
				doubleSpinBoxColumn=col;
		}

}*/
/*
void ETableWidget::validateItem()
{
		if (itemSpinBox!=NULL)
		{
				int value=itemSpinBox->value();
				QTableWidgetItem *item= new QTableWidgetItem(QString::number(value));
				item->setTextAlignment(Qt::AlignCenter);
				this->removeCellWidget(currentSpinBoxRow,currentSpinBoxColumn);
				this->setItem(currentSpinBoxRow,currentSpinBoxColumn,item);
				itemSpinBox=NULL;
				emit validatedItem(currentSpinBoxRow,currentSpinBoxColumn,value);
		}
		else if (itemDoubleSpinBox!=NULL)
		{
				double value=itemDoubleSpinBox->value();
				QTableWidgetItem *item= new QTableWidgetItem(QString::number(value));
				item->setTextAlignment(Qt::AlignCenter);
				this->removeCellWidget(currentDoubleSpinBoxRow,currentDoubleSpinBoxColumn);
				this->setItem(currentDoubleSpinBoxRow,currentDoubleSpinBoxColumn,item);
				itemSpinBox=NULL;
				emit validatedItem(currentDoubleSpinBoxRow,currentDoubleSpinBoxColumn,value);
		}
		currentSpinBoxRow=currentSpinBoxColumn=currentDoubleSpinBoxRow=currentDoubleSpinBoxColumn=-1;
}


void ETableWidget::avaliateType(int row, int col)
{
		bool ok;
		//int row=item->row();
		//int col=item->column();
		QTableWidgetItem * item=this->item(row,col);
		if (currentSpinBoxRow !=-1 || currentSpinBoxColumn !=-1 ||currentDoubleSpinBoxColumn !=-1 || currentDoubleSpinBoxRow !=-1)
				validateItem();

		if (checkTypeds(col,"QSpinBox"))
		{
				itemSpinBox= new QSpinBox();
				itemSpinBox->setAlignment(Qt::AlignHCenter);
				itemSpinBox->setButtonSymbols(QSpinBox::NoButtons);
				itemSpinBox->setMaximum(999999999);
				itemSpinBox->setValue(item->text().toInt(&ok));
				itemSpinBox->selectAll();
				previousValue=item->text().toDouble(&ok);
				currentSpinBoxRow=row;
				currentSpinBoxColumn=col;
				connect(itemSpinBox,SIGNAL(editingFinished()),this,SLOT(validateItem()));
				setCellWidget(row,col,itemSpinBox);
				itemSpinBox->setFocus(Qt::OtherFocusReason);

		}
		if (checkTypeds(col,"QDoubleSpinBox"))
		{
				itemDoubleSpinBox= new QDoubleSpinBox();
				itemDoubleSpinBox->setAlignment(Qt::AlignHCenter);
				itemDoubleSpinBox->setButtonSymbols(QDoubleSpinBox::NoButtons);
				itemDoubleSpinBox->setMaximum(999999999.999999999);
				itemDoubleSpinBox->setValue(item->text().toDouble(&ok));
				previousValue=item->text().toDouble(&ok);
				currentDoubleSpinBoxRow=row;
				currentDoubleSpinBoxColumn=col;
				connect(itemDoubleSpinBox,SIGNAL(editingFinished()),this,SLOT(validateItem()));
				setCellWidget(row,col,itemDoubleSpinBox);
				itemDoubleSpinBox->setFocus(Qt::OtherFocusReason);
				itemDoubleSpinBox->selectAll();
		}
}




void ETableWidget::setType(int column, QString type)
{
		if (type=="NONE")
				this->type="NONE";
		else if(type=="QSpinBox" )//&& checkTypeds(column,"QSpinBox"))
		{
				typedSpinColumns.push_back(column);
				this->type="QSpinBox";
		}
		else if(type=="QDoubleSpinBox")// && checkTypeds(column,"QDoubleSpinBox"))
		{
				typedDoubleSpinColumns.push_back(column);
				this->type="QDoubleSpinBox";
		}
}
*/
/*
bool ETableWidget::checkTypeds(int column, QString type)
{
		if (type=="QSpinBox")
				for(int i=0; i<typedSpinColumns.size();i++)
						if(typedSpinColumns.at(i)==column)
								return true;

		if (type=="QDoubleSpinBox")
				for(int i=0; i<typedDoubleSpinColumns.size();i++)
						if(typedDoubleSpinColumns.at(i)==column)
								return true;

		return false;
}

QString ETableWidget::getType(int column)
{
		if (checkTypeds(column,"QSpinBox"))
				return "QSpinBox";
		else if (checkTypeds(column,"QDoubleSpinBox"))
				return "QDoubleSpinBox";
		else
				return "NONE";
}
*/
/*
double ETableWidget::getPreviousValue()
{
		return previousValue;
}


QSpinBox * ETableWidget:: getItemSpinBox()
{
		return itemSpinBox;
}

QDoubleSpinBox * ETableWidget::getItemDoubleSpinBox()
{
		return itemDoubleSpinBox;
}

int ETableWidget::getCurrentSpinBoxColumn()
{
		return currentSpinBoxColumn;
}

int ETableWidget::getCurrentSpinBoxRow()
{
		return currentSpinBoxRow;
}

int ETableWidget::getCurrentDoubleSpinBoxColumn()
{
		return currentDoubleSpinBoxColumn;
}

int ETableWidget::getCurrentDoubleSpinBoxRow()
{
		return currentDoubleSpinBoxRow;
}
*/
} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
