#include "FlightDirectionForm.h"
#include "math.h"


namespace br {
namespace uerj {
namespace eng {
namespace efoto {
FlightDirectionForm::FlightDirectionForm(QStringList comboBoxItens,QList<int> newMarkedImages, QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	markedIcon.addFile(":/image/checked.png");
	unmarkedIcon.addFile(":/image/unchecked.png");
	autoPassMode=false;
	imagesFlightDirectionCombo->addItems(comboBoxItens);
	int allImages=comboBoxItens.size();
	for (int i=0;i<allImages;i++)
		imagesFlightDirectionCombo->setItemIcon(i,unmarkedIcon);

	setMarkedImages(newMarkedImages);

	connect(flightDirectionDial,SIGNAL(valueChanged(int)),this,SLOT(setSpinValue(int)));
	connect(flightDirectionSpin,SIGNAL(valueChanged(int)),this,SLOT(setDialValue(int)));
	connect(acceptAngleButton, SIGNAL(clicked()),this,SLOT(acceptValues()));
	connect(finishButton,SIGNAL(clicked()),this,SLOT(close()));

	//connect(imagesFlightDirectionCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(updateItemCombo(int)));
	setWindowTitle("Flight Direction");
}

FlightDirectionForm::FlightDirectionForm(QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	autoPassMode=false;
	markedIcon.addFile(":/image/checked.png");
	unmarkedIcon.addFile(":/image/unchecked.png");

	connect(flightDirectionDial,SIGNAL(valueChanged(int)),this,SLOT(setSpinValue(int)));
	connect(flightDirectionSpin,SIGNAL(valueChanged(int)),this,SLOT(setDialValue(int)));
	connect(acceptAngleButton, SIGNAL(clicked()),this,SLOT(acceptValues()));
	connect(finishButton,SIGNAL(clicked()),this,SLOT(close()));
	//connect(imagesFlightDirectionCombo,SIGNAL((int)),this,SLOT(updateMarkedLabel(int)));
	setWindowTitle("Flight Direction");
}

void FlightDirectionForm::acceptValues()
{
	//setMarked(imagesFlightDirectionCombo->currentIndex());
    emit valuesFlightDirectionForm(imagesFlightDirectionCombo->currentText(),flightDirectionSpin->value()*M_PI/180);

    int currentIndexCombo=imagesFlightDirectionCombo->currentIndex();
    if(!isMarked(currentIndexCombo))
        markedImages.push_back(currentIndexCombo);
    updateMarkedLabel(currentIndexCombo);
    updateItemCombo(currentIndexCombo);
}

void FlightDirectionForm::setSpinValue(int value)
{
	int dialValue=flightDirectionDial->value();

	if(dialValue < 90)
		dialValue+=360;

	flightDirectionSpin->setValue(dialValue-90);
}

void FlightDirectionForm::setDialValue(int value)
{
	int spinValue = flightDirectionSpin->value();
	if (spinValue>=270)
		spinValue-=360;

	flightDirectionDial->setValue(spinValue+90);

}

void FlightDirectionForm::updateMarkedLabel(int comboIndex)
{
	if(isMarked(comboIndex))
		imagesFlightDirectionCombo->setItemIcon(comboIndex,markedIcon);
	else
		imagesFlightDirectionCombo->setItemIcon(comboIndex,unmarkedIcon);
}

void FlightDirectionForm::updateItemCombo(int comboIndex)
{
	if (autoPassMode)
	{
		int currentIndex = imagesFlightDirectionCombo->currentIndex();
		int numComboItens= imagesFlightDirectionCombo->count();
		if (currentIndex == numComboItens-1)
;//			imagesFlightDirectionCombo->setCurrentIndex(numComboItens-1);
		else
			imagesFlightDirectionCombo->setCurrentIndex(currentIndex+1);
	}
}

void FlightDirectionForm::setMarkedImages(QList<int> indexesCombo)
{
	markedImages.clear();
	markedImages.append(indexesCombo);
	int numComboItems=indexesCombo.size();
	for(int i=0;i<numComboItems;i++)
		imagesFlightDirectionCombo->setItemIcon(markedImages.at(i),markedIcon);

}

bool FlightDirectionForm::isMarked(int comboIndex)
{
	int numImagesMarked=markedImages.size();
	for(int i=0;i<numImagesMarked;i++)
		if(markedImages.at(i)==comboIndex)
			return true;
	return false;
}

QList<int> FlightDirectionForm::getMarkedImages()
{
	return markedImages;
}

QStringList FlightDirectionForm::getStringMarkedImages()
{
	QStringList list;
	int numMarkedImages=markedImages.size();
	for(int i=0;i<numMarkedImages;i++)
		list << imagesFlightDirectionCombo->itemText(markedImages.at(i));
	return list;

}

int FlightDirectionForm::getIndexCombo(QString comboText)
{
	int numImages=imagesFlightDirectionCombo->count();
	for (int i=0;i<numImages;i++)
		if (imagesFlightDirectionCombo->itemText(i)==comboText)
			return i;
	return -1;
}

bool FlightDirectionForm::getPassMode()
{
	return autoPassMode;
}

void FlightDirectionForm::setPassMode(bool status)
{
	autoPassMode=status;
}


bool FlightDirectionForm::eventFilter(QObject *obj, QEvent *event)
{
	if(event->type()==QEvent::Close)
	{
		QCloseEvent *closEvent = static_cast<QCloseEvent *>(event);
		closeEvent(closEvent);
		return true;
	}
	else
		return QWidget::eventFilter(obj,event);
}

void FlightDirectionForm::closeEvent(QCloseEvent *event)
{
	emit markedImagesList(markedImages,getStringMarkedImages());
	return QWidget::closeEvent(event);
}



} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
