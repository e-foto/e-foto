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
#include "HeaderForm.h"

#include "EDom.h"
#include <sstream>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

HeaderForm::HeaderForm(QWidget *parent) : AbstractForm(parent)
{
	setupUi(this);
	//connect(lineEditFileName,SIGNAL(textChanged(QString)),this,SLOT(metadataVisibleChanged(QString)));
	//metadataFrame->setVisible(false);
}

void HeaderForm::fillvalues(std::string values)
{
	cleanForm();
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

std::string HeaderForm::getvalues()
{
    std::string xmlString;
    std::stringstream auxStream;
    auxStream << "<projectHeader>\n";
	auxStream << "<name>" << lineEditName->text().toUtf8().data() << "</name>\n";
	auxStream << "<description>" << textEditDescription->toPlainText().toUtf8().data() << "</description>\n";
	if (lineEditFilePath->text().isEmpty())
		auxStream << "<filePath>.</filePath>\n";
	else
		auxStream << "<filePath>" << lineEditFilePath->text().toUtf8().data() << "</filePath>\n";
	auxStream << "<fileName>" << lineEditFileName->text().toUtf8().data() << "</fileName>\n";
	auxStream << "<creation>" << dateTimeEditCreationDate->dateTime().toString(Qt::ISODate).toUtf8().data() << "</creation>\n";
	auxStream << "<modification>" << dateTimeEditModificationDate->dateTime().toString(Qt::ISODate).toUtf8().data() << "</modification>\n";
	auxStream << "<owner>" << lineEditOwner->text().toUtf8().data() << "</owner>\n";
	auxStream << "<aims>" << lineEditAims->text().toUtf8().data() << "</aims>\n";
	auxStream << "<context>" << lineEditContext->text().toUtf8().data() << "</context>\n";
	auxStream << "</projectHeader>";
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

bool HeaderForm::isForm(std::string formName)
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

void HeaderForm::cleanForm()
{
	lineEditName->clear();
	lineEditContext->clear();
	lineEditAims->clear();
	textEditDescription->clear();
	lineEditOwner->clear();
	lineEditFilePath->clear();
	lineEditFileName->clear();
	dateTimeEditCreationDate->clear();
	dateTimeEditModificationDate->clear();
}

void HeaderForm::setFormLocale(QLocale locale)
{
	lineEditName->setLocale(locale);
	lineEditContext->setLocale(locale);
	lineEditAims->setLocale(locale);
	textEditDescription->setLocale(locale);
	lineEditOwner->setLocale(locale);
	lineEditFilePath->setLocale(locale);
	lineEditFileName->setLocale(locale);
	dateTimeEditCreationDate->setLocale(locale);
	dateTimeEditModificationDate->setLocale(locale);
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
