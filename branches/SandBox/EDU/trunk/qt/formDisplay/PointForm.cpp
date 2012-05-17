#include "PointForm.h"
#include <QCheckBox>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

PointForm::PointForm(QWidget *parent) : AbstractForm(parent)
{
	setupUi(this);
	sigmaController = new SigmaFormController("Not Available",3);
	sigmaSelector->setSigmaFormController(sigmaController);
	sigmaSelector->blockCovarianceMatrixOption();
	sigmaContent->setSigmaFormController(sigmaController);
	sigmaDialogButton->setSigmaFormController(sigmaController);
	sigmaDialogButton->setVisible(false);
	imageMeasurementsTable->setColumnWidth(1, 20);
	imageMeasurementsTable->setColumnWidth(2, 170);
	imageMeasurementsTable->setColumnWidth(3, 65);
	imageMeasurementsTable->setColumnWidth(4, 65);
	imageMeasurementsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	imageMeasurementsTable->setColumnHidden(0,true);
}

void PointForm::setImageList(deque<int> myKeyList, deque<string> myNameList)
{
	imageKeyList = myKeyList;
	imageNameList = myNameList;
}

void PointForm::fillvalues(string values)
{
	cleanForm();

	EDomElement ede(values);

	key = ede.attribute("key");
	setType(ede.attribute("type"));

	lineEdit_gcp_id->setText(QString::fromUtf8(ede.elementByTagName("pointId").toString().c_str()));
	textEditDescription->setText(QString::fromUtf8(ede.elementByTagName("description").toString().c_str()));
	deque<double> aux = ede.elementByTagName("spatialCoordinates").elementByTagName("gml:pos").toGmlPos();
	if (aux.size() == 3)
	{
		eDoubleSpinBox->setValue(aux.at(0));
		nDoubleSpinBox->setValue(aux.at(1));
		hDoubleSpinBox->setValue(aux.at(2));
	}
	else
	{
		eDoubleSpinBox->setValue(0);
		nDoubleSpinBox->setValue(0);
		hDoubleSpinBox->setValue(0);
	}
	QString suffix(ede.elementByTagName("spatialCoordinates").attribute("uom").c_str());
	eDoubleSpinBox->setSuffix(" "+suffix.right(1));
	nDoubleSpinBox->setSuffix(" "+suffix.right(1));
	hDoubleSpinBox->setSuffix(" "+suffix.right(1));

	if (ede.elementByTagName("sigma").hasTagName("mml:matrix"))
	{
		sigmaController->fillValues(ede.elementByTagName("sigma").getContent());
		//Paulo: block e unblock signals
		//Gambiarra feita para a classe sigma selector se atualizar sem zerar os stdev
		//Caso contrario ela muda a combo box para um valor inexistente, causando um bug na exibiçao do conteudo
		sigmaSelector->blockSignals(true);
		sigmaSelector->setCurrentIndex(1);
		sigmaSelector->blockSignals(false);
	}
	//sigmaSelector->setCurrentIndex(0);

	//sigmaController->fillValues(ede.elementByTagName("sigma").toString());
	//lineEditImageCoordinates1->setText(QString::fromUtf8(ede.elementByTagAtt("imageCoordinates","image_key","1").elementByTagName("gml:pos").toString().c_str()));

	imageMeasurementsTable->setRowCount(imageKeyList.size());
	bool ok;
	for (unsigned int i = 0; i < imageKeyList.size(); i++)
	{
		QTableWidgetItem *keyItem = new QTableWidgetItem(QString::number(imageKeyList.at(i)));
		QCheckBox *checkItem = new QCheckBox();
		QTableWidgetItem *imageItem = new QTableWidgetItem(QString::fromUtf8(imageNameList.at(i).c_str()));
		QTableWidgetItem *linItem = new QTableWidgetItem();
		QTableWidgetItem *colItem = new QTableWidgetItem();
		linItem->setTextAlignment(Qt::AlignCenter);
		colItem->setTextAlignment(Qt::AlignCenter);
		EDomElement imgMeasure = ede.elementByTagAtt("imageCoordinates","image_key",Conversion::intToString(imageKeyList.at(i)));
		if (imgMeasure.getContent() != "")
		{
			checkItem->setChecked(true);
			QStringList linColStr = QString(imgMeasure.elementByTagName("gml:pos").toString().c_str()).split(" ");
			if (linColStr.size() == 2)
			{
				QString linStr=linColStr.at(0);
				QString colStr=linColStr.at(1);
				//qDebug() << "FillValues linstr:" <<linStr << "colStr:"<<colStr;
				if ((linStr=="-1" && colStr=="-1") || (linStr=="" && colStr==""))
				{
					linItem->setText("--");
					colItem->setText("--");
				}
				else
				{
					//linItem->setText(QString::number(linColStr.at(0).toDouble(),'f',3));
					//colItem->setText(QString::number(linColStr.at(1).toDouble(),'f',3));
					linItem->setText(QString::number(linStr.toDouble(&ok),'f',3));
					colItem->setText(QString::number(colStr.toDouble(&ok),'f',3));
				}
			}
        }

		imageMeasurementsTable->setItem(i,0,keyItem);
		imageMeasurementsTable->setCellWidget(i,1,checkItem);
		imageMeasurementsTable->setItem(i,2,imageItem);
		imageMeasurementsTable->setItem(i,3,linItem);
        imageMeasurementsTable->setItem(i,4,colItem);
        imageMeasurementsTable->setRowHidden(i,!checkItem->isChecked());
	}
    imageMeasurementsTable->setDisabled(true);
}

string PointForm::getvalues()
{
	stringstream auxStream;
	auxStream << "<point key=\"" << key << "\" type=\"" << getType() <<"\">\n";
	auxStream << "<pointId>" << lineEdit_gcp_id->text().toUtf8().data() << "</pointId>\n";
	auxStream << "<description>" << textEditDescription->toPlainText().toUtf8().data() << "</description>\n";
	auxStream << "<spatialCoordinates uom=\"#" << eDoubleSpinBox->suffix().right(1).toStdString().c_str() << "\">\n";
	auxStream << "<gml:pos>" << Conversion::doubleToString(eDoubleSpinBox->value()) << " " << Conversion::doubleToString(nDoubleSpinBox->value()) << " " << Conversion::doubleToString(hDoubleSpinBox->value()) << "</gml:pos>\n";
	auxStream << sigmaController->getValues();
	auxStream << "</spatialCoordinates>\n";
	auxStream << "<imagesMeasurements>\n";

	for (unsigned int i = 0; i < imageKeyList.size(); i++)
	{
		QCheckBox* myCheck = (QCheckBox*) imageMeasurementsTable->cellWidget(i,1);
		if (myCheck != NULL && myCheck->checkState())
		{
			QString colStr=imageMeasurementsTable->item(i,3)->text();
			QString linStr=imageMeasurementsTable->item(i,4)->text();
			if ((colStr=="" && linStr=="") || (linStr=="--" && colStr=="--"))
			{
				linStr="-1";
				colStr="-1";
			}
			auxStream << "<imageCoordinates uom=\"#px\" image_key=\"";
			auxStream << imageMeasurementsTable->item(i,0)->text().toStdString();
			auxStream << "\"><gml:pos>";
			auxStream << colStr.toStdString();
			auxStream << " ";
			auxStream << linStr.toStdString();
			auxStream << "</gml:pos>";
			auxStream << "</imageCoordinates>\n";
		}
	}
	auxStream << "</imagesMeasurements>\n";
	auxStream << "</point>";
	return auxStream.str();
}

void PointForm::setReadOnly (bool state)
{
	typeComboBox->setEnabled(!state);
	lineEdit_gcp_id->setReadOnly(state);
	textEditDescription->setReadOnly(state);
	eDoubleSpinBox->setReadOnly(state);
	nDoubleSpinBox->setReadOnly(state);
	hDoubleSpinBox->setReadOnly(state);
	sigmaController->setReadOnly(state);
	imageMeasurementsTable->setDisabled(state);
}

void PointForm::setType(string type)
{
	if (!type.compare("control"))
		typeComboBox->setCurrentIndex(0);
	if (!type.compare("checking"))
		typeComboBox->setCurrentIndex(1);
	if (!type.compare("photogrammetric"))
		typeComboBox->setCurrentIndex(2);
}

string PointForm::getType()
{
	if (typeComboBox->currentIndex() == 0)
		return "control";
	if (typeComboBox->currentIndex() == 1)
		return "checking";
	if (typeComboBox->currentIndex() == 2)
		return "photogrammetric";
	return "";
}

bool PointForm::isForm(string formName)
{
	return !formName.compare("PointForm");
}

void PointForm::cleanForm()
{
	lineEdit_gcp_id->clear();
	typeComboBox->setCurrentIndex(0);
	textEditDescription->clear();
	sigmaController->fillValues("Not Available");
	eDoubleSpinBox->clear();
	nDoubleSpinBox->clear();
	hDoubleSpinBox->clear();
	imageMeasurementsTable->clearContents();
}

void PointForm::setFormLocale(QLocale locale)
{
	lineEdit_gcp_id->setLocale(locale);
	textEditDescription->setLocale(locale);
	eDoubleSpinBox->setLocale(locale);
	nDoubleSpinBox->setLocale(locale);
	hDoubleSpinBox->setLocale(locale);
	imageMeasurementsTable->setLocale(locale);
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
