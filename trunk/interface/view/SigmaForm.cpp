#include "SigmaForm.h"

SigmaFormController::SigmaFormController()
{
	init();
	dimension = 1;
	validator = new QDoubleValidator(this);
	this->toNotAvailable();
}

SigmaFormController::SigmaFormController(string myMode, unsigned int myDimension)
{
	init();
	dimension = myDimension;
	validator = new QDoubleValidator(this);
	if (myMode == "Not Available")
	{
		this->toNotAvailable();
	}
	else if (myMode == "Standard Deviations")
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
	validator = new QDoubleValidator(this);
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

Matrix SigmaFormController::stDevToMatrix(deque<double> stDev)
{
	Matrix result(stDev.size(), stDev.size());
	result.zero();
	for (unsigned int i = 0; i < stDev.size(); i++)
		result.set(i,i,stDev.at(i)*stDev.at(i));
	return result;
}

deque<double> SigmaFormController::matrixToStDev(Matrix mat)
{
	deque<double> result;
	for (unsigned int i = 0; i < mat.getRows(); i++)
		result.push_back(sqrt(mat.get(i,i)));
	return result;
}

QWidget* SigmaFormController::getContent()
{
	return contentWidget;
}

void SigmaFormController::setDirection(string newDirection)
{
	if (newDirection == "horizontal")
		direction = newDirection;
	else
		direction = "vertical";
}

string SigmaFormController::getDirection()
{
	return direction;
}

void SigmaFormController::setMode(string newMode)
{
	mode = newMode;
}

string SigmaFormController::getMode()
{
	return mode;
}

void SigmaFormController::fillValues(string xml)
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
			edits.at(i)->setText(QString::number(values.get(i+1,1)));
		}
	}
	else if (values.getCols() == values.getRows()) //Variance and covariance matrix
	{
		changeToMode("Covariance Matrix");
		for (unsigned int i = 0; i < dimension; i++)
		{
			edits.at(i)->setText(QString::number(values.get(i+1,i+1)));
		}
	}
}

string SigmaFormController::getValues()
{
	if (mode == "Not Available")
	{
		return "<sigma>Not Available</sigma>\n";
	}
	else if (mode == "Standard Deviations")
	{
		Matrix result(dimension,1);
		for (unsigned int i = 0; i < dimension; i++)
		{
			if (!edits.at(i)->text().isEmpty())
				result.set(1+i,1,stringToDouble(edits.at(i)->text().toStdString()));
			else
				result.set(1+i,1,0);
		}
		if (!result.isZeroes())
			return  "<sigma>\n" + result.xmlGetData() + "</sigma>\n";
		else
		{
			toMode("Not Available");
			return "<sigma>Not Available</sigma>\n";
		}
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
	else if (mode == "Standard Deviations")
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
	else if (newMode == "Standard Deviations")
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
		QBoxLayout* myLayout = (QBoxLayout*)contentWidget->layout();
		if (direction == "horizontal")
			myLayout->setDirection(QBoxLayout::LeftToRight);
		else
			myLayout->setDirection(QBoxLayout::TopToBottom);
		for (unsigned int i = 0; i < dimension; i++)
		{
			QDoubleValidator *validator = new QDoubleValidator(this);
			QLabel* newLabel = labels.at(i);
			QLineEdit* newEdit = edits.at(i);
			newEdit->setValidator(0);
			newLabel->setText("StDev");
			newEdit->setText("Not Available");
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
			QLineEdit* newEdit = new QLineEdit("Not Available");
			connect(newEdit,SIGNAL(textChanged(QString)),this,SLOT(changeValidate(QString)));
			newEdit->setEnabled(false);
			subLayout->addWidget(newLabel);
			subLayout->addWidget(newEdit);
			subLayout->setMargin(0);
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
		QBoxLayout* myLayout = (QBoxLayout*)contentWidget->layout();
		if (direction == "horizontal")
			myLayout->setDirection(QBoxLayout::LeftToRight);
		else
			myLayout->setDirection(QBoxLayout::TopToBottom);
		for (unsigned int i = 0; i < dimension; i++)
		{
			QLabel* newLabel = labels.at(i);
			QLineEdit* newEdit = edits.at(i);
			newEdit->setValidator(validator);
			newLabel->setText("StDev");
			newEdit->setText("");
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
			QLineEdit* newEdit = new QLineEdit();
			newEdit->setValidator(validator);
			connect(newEdit,SIGNAL(textChanged(QString)),this,SLOT(changeValidate(QString)));
			subLayout->addWidget(newLabel);
			subLayout->addWidget(newEdit);
			subLayout->setMargin(0);
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
		QBoxLayout* myLayout = (QBoxLayout*)contentWidget->layout();
		if (direction == "horizontal")
			myLayout->setDirection(QBoxLayout::LeftToRight);
		else
			myLayout->setDirection(QBoxLayout::TopToBottom);
		for (unsigned int i = 0; i < dimension; i++)
		{
			QLabel* newLabel = labels.at(i);
			QLineEdit* newEdit = edits.at(i);
			newLabel->setText("Var");
			newEdit->setValidator(validator);
			newEdit->setText("");
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
			QLineEdit* newEdit = new QLineEdit();
			newEdit->setValidator(validator);
			connect(newEdit,SIGNAL(textChanged(QString)),this,SLOT(changeValidate(QString)));
			subLayout->addWidget(newLabel);
			subLayout->addWidget(newEdit);
			subLayout->setMargin(0);
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
	controller = newController;
	connect(this, SIGNAL(currentIndexChanged(QString)), controller, SLOT(toMode(QString)));
	connect(controller, SIGNAL(changeToReadOnly(bool)), this, SLOT(setDisabled(bool)));
	connect(controller, SIGNAL(changeToMode(QString)), this, SLOT(toMode(QString)));
}

void SigmaFormTypeSelector::toMode(QString newMode)
{
	if (newMode == "Not Available")
	{
		setCurrentIndex(0);
	}
	else if (newMode == "Standard Deviations")
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
	removeItem(2);
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