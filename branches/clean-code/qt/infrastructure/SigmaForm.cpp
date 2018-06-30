#include "SigmaForm.h"
#include "ScienceSpinBox.h"
//#include <math.h>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

SigmaFormController::SigmaFormController()
{
	init();
	dimension = 1;

	this->toNotAvailable();
}

SigmaFormController::SigmaFormController(std::string myMode, unsigned int myDimension)
{
	init();
	dimension = myDimension;

	if (myMode == "Not Available")
	{
		this->toNotAvailable();
	}
	else if (myMode == "Standard Deviations" || myMode == "Available")
	{
		this->toStDev();
	}
	else if (myMode == "Covariance Matrix")
	{
		this->toMatrix();
	}
	mode = myMode;
}

SigmaFormController::SigmaFormController(Matrix myValues)
{
	init();
	if (myValues.getCols() == 1) //Standard deviations
	{
		dimension = myValues.getRows();
		this->toStDev();
	}
	else if (myValues.getCols() == myValues.getRows()) //Variance and covariance matrix
	{
		dimension = myValues.getRows();
		this->toMatrix();
	}
}

void SigmaFormController::init()
{
	contentWidget = new QWidget();
	contentWidget->setContentsMargins(0,0,0,0);
	direction = "vertical";
	labels.clear();
	edits.clear();
}

QWidget* SigmaFormController::getContent()
{
	return contentWidget;
}

void SigmaFormController::setDirection(std::string newDirection)
{
	QBoxLayout* myLayout = (QBoxLayout*)contentWidget->layout();
	if (newDirection == "horizontal")
	{
		direction = newDirection;
		myLayout->setDirection(QBoxLayout::LeftToRight);
	}
	else
	{
		direction = "vertical";
		myLayout->setDirection(QBoxLayout::TopToBottom);
	}
}

void SigmaFormController::setMode(std::string newMode)
{
	mode = newMode;
}

std::string SigmaFormController::getMode()
{
	return mode;
}

void SigmaFormController::fillValues(std::string xml)
{
	EDomElement root(xml);
	values = Matrix();
	if (root.elementByTagName("mml:matrix").getContent() == "")
	{
		changeToMode("Not Available");
		return;
	}
	values.xmlSetData(root.elementByTagName("mml:matrix").getContent());
	if (values.getCols() == 1) //Standard deviations
	{
		changeToMode("Standard Deviations");
		for (unsigned int i = 0; i < dimension; i++)
		{
			edits.at(i)->setTextValue(QString::number(values.get(i+1,1)));
		}
	}
	else if (values.getCols() == values.getRows()) //Variance and covariance matrix
	{
		changeToMode("Covariance Matrix");
		for (unsigned int i = 0; i < dimension; i++)
		{
			edits.at(i)->setTextValue(QString::number(values.get(i+1,i+1)));
		}
	}
}

std::string SigmaFormController::getValues()
{
	if (mode == "Not Available")
	{
		return "<sigma>Not Available</sigma>\n";
	}
	else if (mode == "Standard Deviations"  || mode == "Available")
	{
		Matrix result(dimension,1);
		for (unsigned int i = 0; i < dimension; i++)
		{
			if (!edits.at(i)->text().isEmpty())
				result.set(1+i,1,Conversion::stringToDouble(edits.at(i)->textValue().toStdString()));
			else
				result.set(1+i,1,0);
		}
		//if (!result.isZeroes())--> O metodo isZeroes() não é muito util para o caso de
		//sigma, pois qualquer valor abaixo de 0.000001 é considerado zero
		return  "<sigma>\n" + result.xmlGetData() + "</sigma>\n";
		/*else
  {
   toMode("Not Available");
   return "<sigma>Not Available</sigma>\n";
  }*/
	}
	else if (mode == "Covariance Matrix")
	{
		if (!values.isZeroes())

			return  "<sigma>\n" + values.xmlGetData() + "</sigma>\n";
		else
		{
			toMode("Not Available");
			return "<sigma>Not Available</sigma>\n";
		}
	}
	toMode("Not Available");
	return "<sigma>Error</sigma>\n";
}

bool SigmaFormController::getValidate()
{
	if (mode == "Not Available")
	{
		return true;
	}
	else if (mode == "Standard Deviations" || mode == "Available")
	{
		for (unsigned int i = 0; i < dimension; i++)
		{
			if (edits.at(i)->text().isEmpty())
				return false;
		}
	}
	else if (mode == "Covariance Matrix")
	{
		return true;
	}
	return true;
}

void SigmaFormController::toMode(QString newMode)//???
{
    if (newMode == "Not Available")
	{
		this->toNotAvailable();
	}
	else if (newMode == "Standard Deviations" || newMode == "Available")
	{
		this->toStDev();
	}
	else if (newMode == "Covariance Matrix")
	{
		this->toMatrix();
	}
	mode = newMode.toStdString();
}

void SigmaFormController::toNotAvailable()
{
	if (dimension == (unsigned int)edits.size())
	{
		for (unsigned int i = 0; i < dimension; i++)
		{
			QLabel* newLabel = labels.at(i);
			ScienceSpinBox* newEdit = edits.at(i);
			newLabel->setText("StDev");
			newEdit->setSpecialValueText("Not Available");
			newEdit->setTextValue("Not Available");
			newEdit->setEnabled(false);
		}
	}
	else
	{
		QBoxLayout* myLayout = new QBoxLayout(QBoxLayout::TopToBottom);
		if (direction == "horizontal")
			myLayout->setDirection(QBoxLayout::LeftToRight);
		for (unsigned int i = 0; i < dimension; i++)
		{
			QHBoxLayout* subLayout = new QHBoxLayout;
			QLabel* newLabel = new QLabel("StDev");
			ScienceSpinBox* newEdit = new ScienceSpinBox();
			newEdit->setSpecialValueText("Not Available");
			newEdit->setTextValue("Not Available");
			newEdit->setRange(-10,10); // No futuro a classe SigmaForm terá um método para setar o Range admissível de todos os seus campos.
			connect(newEdit,SIGNAL(valueChanged(QString)),this,SLOT(changeValidate(QString)));
			newEdit->setEnabled(false);
			subLayout->addWidget(newLabel);
			subLayout->addWidget(newEdit);
			subLayout->setMargin(0);
			subLayout->setStretch(0,0);
			subLayout->setStretch(1,1);
			myLayout->addLayout(subLayout);
			labels.push_back(newLabel);
			edits.push_back(newEdit);
		}
		myLayout->setMargin(0);
		contentWidget->setLayout(myLayout);
	}
	emit changeMatrixButtonVisibility(false);
}

void SigmaFormController::toStDev()
{
	if (dimension == (unsigned int)edits.size())
	{
		for (unsigned int i = 0; i < dimension; i++)
		{
			QLabel* newLabel = labels.at(i);
			ScienceSpinBox* newEdit = edits.at(i);
			newLabel->setText("StDev");
			newEdit->setSpecialValueText("");
			newEdit->setTextValue("0");
			newEdit->setEnabled(true);
		}
	}
	else
	{
		QBoxLayout* myLayout = new QBoxLayout(QBoxLayout::TopToBottom);
		if (direction == "horizontal")
			myLayout->setDirection(QBoxLayout::LeftToRight);
		for (unsigned int i = 0; i < dimension; i++)
		{
			QHBoxLayout* subLayout = new QHBoxLayout;
			QLabel* newLabel = new QLabel("StDev");
			ScienceSpinBox* newEdit = new ScienceSpinBox();
			newEdit->setSpecialValueText("");
			connect(newEdit,SIGNAL(valueChanged(QString)),this,SLOT(changeValidate(QString)));
			subLayout->addWidget(newLabel);
			subLayout->addWidget(newEdit);
			subLayout->setMargin(0);
			subLayout->setStretch(0,0);
			subLayout->setStretch(1,1);
			myLayout->addLayout(subLayout);
			labels.push_back(newLabel);
			edits.push_back(newEdit);
		}
		myLayout->setMargin(0);
		contentWidget->setLayout(myLayout);
	}
	emit changeMatrixButtonVisibility(false);
}

void SigmaFormController::toMatrix()
{
	if (dimension == (unsigned int)edits.size())
	{
		for (unsigned int i = 0; i < dimension; i++)
		{
			QLabel* newLabel = labels.at(i);
			ScienceSpinBox* newEdit = edits.at(i);
			newLabel->setText("Var");
			newEdit->setSpecialValueText("");
			newEdit->setTextValue("0");
			newEdit->setEnabled(true);
		}
	}
	else
	{
		QBoxLayout* myLayout = new QBoxLayout(QBoxLayout::TopToBottom);
		if (direction == "horizontal")
			myLayout->setDirection(QBoxLayout::LeftToRight);
		for (unsigned int i = 0; i < dimension; i++)
		{
			QHBoxLayout* subLayout = new QHBoxLayout;
			QLabel* newLabel = new QLabel("Var");
			ScienceSpinBox* newEdit = new ScienceSpinBox();
			newEdit->setSpecialValueText("");
			connect(newEdit,SIGNAL(valueChanged(QString)),this,SLOT(changeValidate(QString)));
			subLayout->addWidget(newLabel);
			subLayout->addWidget(newEdit);
			subLayout->setMargin(0);
			subLayout->setStretch(0,0);
			subLayout->setStretch(1,1);
			myLayout->addLayout(subLayout);
			labels.push_back(newLabel);
			edits.push_back(newEdit);
		}
		myLayout->setMargin(0);
		contentWidget->setLayout(myLayout);
	}
	if (dimension != 1)
	{
		emit changeMatrixButtonVisibility(true);
	}
}

void SigmaFormController::setReadOnly(bool b)
{
    for (unsigned int i = 0; i < dimension; i++)
    {
        edits.at(i)->setReadOnly(b);
    }
    emit changeToReadOnly(b);
}

void SigmaFormController::changeValidate(QString)
{
	emit validateChanged();
}

SigmaFormTypeSelector::SigmaFormTypeSelector(QWidget * parent) : QComboBox(parent)
{
	addItems(QString("Not Available;Standard Deviations;Covariance Matrix").split(";"));
}

void SigmaFormTypeSelector::setSigmaFormController(SigmaFormController *newController)
{
	connect(this, SIGNAL(currentIndexChanged(QString)), newController, SLOT(toMode(QString)));
	connect(newController, SIGNAL(changeToReadOnly(bool)), this, SLOT(setDisabled(bool)));
	connect(newController, SIGNAL(changeToMode(QString)), this, SLOT(toMode(QString)));
}

void SigmaFormTypeSelector::toMode(QString newMode)
{
	if (newMode == "Not Available")
	{
		setCurrentIndex(0);
	}
	else if (newMode == "Standard Deviations" || newMode == "Available")
	{
		setCurrentIndex(1);
	}
	else if (newMode == "Covariance Matrix")
	{
		setCurrentIndex(2);
	}
}

void SigmaFormTypeSelector::blockCovarianceMatrixOption()
{
	this->clear();
	addItems(QString("Not Available;Available").split(";"));
}

SigmaFormContent::SigmaFormContent(QWidget * parent) : QFrame(parent)
{
	setContentsMargins(0,0,0,0);
	if (layout() == 0)
		this->setLayout(new QVBoxLayout());
	layout()->setMargin(0);
}

void SigmaFormContent::setSigmaFormController(SigmaFormController *newController)
{
	controller = newController;
	layout()->addWidget(controller->getContent());
}

SigmaFormDialogButton::SigmaFormDialogButton(QWidget * parent) :
    QPushButton("Show whole matrix", parent)
{
}

void SigmaFormDialogButton::setSigmaFormController(SigmaFormController *newController)
{
	controller = newController;
	connect(this, SIGNAL(clicked()), controller, SLOT(showMatrixEditor()));
	connect(controller, SIGNAL(changeMatrixButtonVisibility(bool)), this, SLOT(setEnabled(bool)));
	connect(controller, SIGNAL(changeMatrixButtonVisibility(bool)), this, SLOT(setVisible(bool)));
	connect(controller, SIGNAL(changeToReadOnly(bool)), this, SLOT(setDisabled(bool)));
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
