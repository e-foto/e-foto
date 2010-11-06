#include "HeaderForm.h"

HeaderForm::HeaderForm(QWidget *parent) : AbstractForm(parent)
{
	setupUi(this);
	//connect(lineEditFileName,SIGNAL(textChanged(QString)),this,SLOT(metadataVisibleChanged(QString)));
	//metadataFrame->setVisible(false);
}

void HeaderForm::fillvalues(string values)
{
	EDomElement ede(values);
	lineEditName->setText(QString::fromUtf8(ede.elementByTagName("name").toString().c_str()));
	textEditDescription->setPlainText(QString::fromUtf8(ede.elementByTagName("description").toString().c_str()));
	if (!ede.elementByTagName("filePath").toString().compare("."))
		lineEditFilePath->setText("");
	else
		lineEditFilePath->setText(QString::fromUtf8(ede.elementByTagName("filePath").toString().c_str()));
	lineEditFileName->setText(QString::fromUtf8(ede.elementByTagName("fileName").toString().c_str()));
	dateTimeEditCreationDate->setDateTime(QDateTime::fromString(QString::fromUtf8(ede.elementByTagName("creation").toString().c_str()),Qt::ISODate));
	dateTimeEditModificationDate->setDateTime(QDateTime::fromString(QString::fromUtf8(ede.elementByTagName("modification").toString().c_str()),Qt::ISODate));
	lineEditOwner->setText(QString::fromUtf8(ede.elementByTagName("owner").toString().c_str()));
	lineEditAims->setText(QString::fromUtf8(ede.elementByTagName("aims").toString().c_str()));
	lineEditContext->setText(QString::fromUtf8(ede.elementByTagName("context").toString().c_str()));
}

string HeaderForm::getvalues()
{
	string xmlString;
	stringstream auxStream;
		auxStream << "<projectHeader>\n";
		auxStream << "\t\t<name>" << lineEditName->text().toUtf8().data() << "</name>\n";
		auxStream << "\t\t<description>" << textEditDescription->toPlainText().toUtf8().data() << "</description>\n";
		if (lineEditFilePath->text().isEmpty())
			auxStream << "\t\t<filePath>.</filePath>\n";
		else
			auxStream << "\t\t<filePath>" << lineEditFilePath->text().toUtf8().data() << "</filePath>\n";
		auxStream << "\t\t<fileName>" << lineEditFileName->text().toUtf8().data() << "</fileName>\n";
		auxStream << "\t\t<creation>" << dateTimeEditCreationDate->dateTime().toString(Qt::ISODate).toUtf8().data() << "</creation>\n";
		auxStream << "\t\t<modification>" << dateTimeEditModificationDate->dateTime().toString(Qt::ISODate).toUtf8().data() << "</modification>\n";
		auxStream << "\t\t<owner>" << lineEditOwner->text().toUtf8().data() << "</owner>\n";
		auxStream << "\t\t<aims>" << lineEditAims->text().toUtf8().data() << "</aims>\n";
		auxStream << "\t\t<context>" << lineEditContext->text().toUtf8().data() << "</context>\n";
		auxStream << "\t</projectHeader>";
	xmlString = auxStream.str();
	return xmlString;
}

void HeaderForm::setReadOnly(bool state)
{
	lineEditName->setReadOnly(state);
	textEditDescription->setReadOnly(state);
	lineEditOwner->setReadOnly(state);
	lineEditAims->setReadOnly(state);
	lineEditContext->setReadOnly(state);
}

bool HeaderForm::isForm(string formName)
{
	return !formName.compare("HeaderForm");
}

void HeaderForm::metadataVisibleChanged(QString newText)
{
	if (newText.isEmpty())
		metadataFrame->setVisible(false);
	else
		metadataFrame->setVisible(true);
}

void HeaderForm::focusInEvent(QFocusEvent *)
{
	lineEditName->setFocus();
}
