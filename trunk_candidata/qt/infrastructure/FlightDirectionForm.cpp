#include "FlightDirectionForm.h"
#include "math.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

FlightDirectionForm::FlightDirectionForm(QWidget *parent) : QWidget(parent)
{
	setupUi(this);

	connect(flightDirectionDial,SIGNAL(valueChanged(int)),this,SLOT(setSpinValue(int)));
	connect(flightDirectionSpin,SIGNAL(valueChanged(int)),this,SLOT(setDialValue(int)));
	connect(acceptAngleButton, SIGNAL(clicked()),this,SLOT(acceptValues()));
	connect(finishButton,SIGNAL(clicked()),this,SLOT(close()));

}

void FlightDirectionForm::acceptValues()
{

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

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
