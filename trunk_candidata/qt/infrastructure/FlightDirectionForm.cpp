#include "FlightDirectionForm.h"
#include "math.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {
FlightDirectionForm::FlightDirectionForm(QStringList comboBoxItens,QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	markedIcon.addFile(":/image/checked.png");
	unmarkedIcon.addFile(":/image/unchecked.png");

	imagesFlightDirectionCombo->addItems(comboBoxItens);
	int allImages=comboBoxItens.size();
	for (int i=0;i<allImages;i++)
		imagesFlightDirectionCombo->setItemIcon(i,unmarkedIcon);

	connect(flightDirectionDial,SIGNAL(valueChanged(int)),this,SLOT(setSpinValue(int)));
	connect(flightDirectionSpin,SIGNAL(valueChanged(int)),this,SLOT(setDialValue(int)));
	connect(acceptAngleButton, SIGNAL(clicked()),this,SLOT(acceptValues()));
	connect(finishButton,SIGNAL(clicked()),this,SLOT(close()));
	//connect(imagesFlightDirectionCombo,SIGNAL((int)),this,SLOT(updateMarkedLabel(int)));
	setWindowTitle("Flight Direction");
}

FlightDirectionForm::FlightDirectionForm(QWidget *parent) : QWidget(parent)
{
	setupUi(this);

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
	int currentIndexCombo=imagesFlightDirectionCombo->currentIndex();
	imagesMarked.push_back(currentIndexCombo);
	updateMarkedLabel(currentIndexCombo);

	emit valuesFlightDirectionForm(imagesFlightDirectionCombo->currentText(),flightDirectionSpin->value()*M_PI/180);
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

	//imagesFlightDirectionCombo->update();
}

void FlightDirectionForm::setMarkedImages(QList<int> indexesCombo)
{
	imagesMarked.clear();
	imagesMarked.append(indexesCombo);
}

bool FlightDirectionForm::isMarked(int comboIndex)
{
	int numImagesMarked=imagesMarked.size();
	for(int i=0;i<numImagesMarked;i++)
		if(imagesMarked.at(i)==comboIndex)
			return true;
	return false;
}

QList<int> FlightDirectionForm::getMarkedImages()
{
	return imagesMarked;
}


} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
