#include "EfotoDoubleSpinBox.h"
#include <QDebug>

EfotoDoubleSpinBox::EfotoDoubleSpinBox(QWidget *parent):QDoubleSpinBox(parent)
{
	installEventFilter(this);


}

bool EfotoDoubleSpinBox::eventFilter(QObject *obj, QEvent *evt)
{
	if (evt->type()==QEvent::KeyPress)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(evt);
		keyPressEvent(keyEvent);
		return true;
	}else
	{
		return QDoubleSpinBox::eventFilter(obj,evt);
	}



}

void EfotoDoubleSpinBox::keyPressEvent(QKeyEvent *event)
{

	//qDebug()<<"tecla apertada"<< event->key();

	if(event->key()==Qt::Key_Period )
	{
		if(locale().decimalPoint()==',')
		{
			keyPressEvent(new QKeyEvent(QEvent::KeyPress,Qt::Key_Comma,event->modifiers(),","));
			//return;
		}
		else
			QDoubleSpinBox::keyPressEvent(event);
	}
	else if(event->key()==Qt::Key_Comma)
		{
			if(locale().decimalPoint()=='.')
			{
				keyPressEvent(new QKeyEvent(QEvent::KeyPress,Qt::Key_Period,event->modifiers(),"."));
				//return;
			}
			else
				QDoubleSpinBox::keyPressEvent(event);
		}
	else
		QDoubleSpinBox::keyPressEvent(event);
}
