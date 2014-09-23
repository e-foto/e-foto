#include "SigmaForm.h"
#include "ScienceSpinBox.h"
#include <math.h>

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

// Método do Panda
void SigmaFormController::setNameLabels(QStringList newNames)
{
	int i=0;
	if (labels.length()==newNames.length())
	{
		for (i=0;i<labels.length();i++)
		{
			labels.at(i)->setText(newNames.at(i));
		}
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

Matrix SigmaFormController::stDevToMatrix(std::deque<double> stDev)
{
	Matrix result(stDev.size(), stDev.size());
	result.zero();
	for (unsigned int i = 0; i < stDev.size(); i++)
		result.set(i,i,stDev.at(i)*stDev.at(i));
	return result;
}

std::deque<double> SigmaFormController::matrixToStDev(Matrix mat)
{
    std::deque<double> result;
	for (unsigned int i = 0; i < mat.getRows(); i++)
		result.push_back(sqrt(mat.get(i,i)));
	return result;
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

std::string SigmaFormController::getDirection()
{
	return direction;
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

void SigmaFormController::toMode(QString newMode)
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

void SigmaFormController::setTitles(QStringList newTitles)
{
	titles = newTitles;
	updateTitles();
}

void SigmaFormController::updateTitles()
{
	if (dimension == (unsigned int)edits.size())
	{
		QString text;
		if (mode == "Covariance Matrix")
		{
			for (unsigned int i = 0; i < dimension; i++)
			{
				//QLabel* newLabel = labels.at(i);
				//newLabel->setText("Var");
			}
		}
		else
		{
			for (unsigned int i = 0; i < dimension; i++)
			{
				//QLabel* newLabel = labels.at(i);
				//text = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\"><html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">p, li { white-space: pre-wrap; }</style></head><body style=\" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;\"><table style=\"-qt-table-type: root; margin-top:4px; margin-bottom:4px; margin-left:4px; margin-right:4px;\"><tr><td style=\"border: none;\"><p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'MS Shell Dlg 2';\">";
				//text += titles.at();
				//newLabel->setText(text);
			}
		}
	}
}

void SigmaFormController::showMatrixEditor()
{
	//QMessageBox msg("e-foto","A classe MatrixEditor ainda n~ao est´a pronta");
	//msg.show();
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

void SigmaFormTypeSelector::disconnectSigmaFormController(SigmaFormController *oldController)
{
	connect(this, SIGNAL(currentIndexChanged(QString)), oldController, SLOT(toMode(QString)));
	connect(oldController, SIGNAL(changeToReadOnly(bool)), this, SLOT(setDisabled(bool)));
	connect(oldController, SIGNAL(changeToMode(QString)), this, SLOT(toMode(QString)));
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

SigmaFormDialogButton::SigmaFormDialogButton(QWidget * parent) : QPushButton("Show whole matrix", parent)
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
