#include "PointForm.h"
#include <QCheckBox>

PointForm::PointForm(QWidget *parent) : AbstractForm(parent)
{
    setupUi(this);
	imageMeasurementsTable->setColumnWidth(1, 20);
	imageMeasurementsTable->setColumnWidth(2, 170);
	imageMeasurementsTable->setColumnWidth(3, 65);
	imageMeasurementsTable->setColumnWidth(4, 65);
}

void PointForm::setImageList(deque<int> myKeyList, deque<string> myNameList)
{
	imageKeyList = myKeyList;
	imageNameList = myNameList;
}

void PointForm::fillvalues(string values)
{
	EDomElement ede(values);

	key = ede.attribute("key");
	setType(ede.attribute("type"));

    lineEdit_gcp_id->setText(QString::fromUtf8(ede.elementByTagName("gcp_id").toString().c_str()));
	lineEditDescription->setText(QString::fromUtf8(ede.elementByTagName("description").toString().c_str()));
	deque<double> aux = ede.elementByTagName("spatialCoordinates").elementByTagName("gml:pos").toGmlPos();
	if (aux.size() == 3)
	{
		eDoubleSpinBox->setValue(aux.at(0));
		nDoubleSpinBox->setValue(aux.at(1));
		hDoubleSpinBox->setValue(aux.at(2));
	}
	QString suffix(ede.elementByTagName("spatialCoordinates").attribute("uom").c_str());
	eDoubleSpinBox->setSuffix(" "+suffix.right(1));
	nDoubleSpinBox->setSuffix(" "+suffix.right(1));
	hDoubleSpinBox->setSuffix(" "+suffix.right(1));
    lineEditSigma->setText(QString::fromUtf8(ede.elementByTagName("sigma").toString().c_str()));
	//lineEditImageCoordinates1->setText(QString::fromUtf8(ede.elementByTagAtt("imageCoordinates","image_key","1").elementByTagName("gml:pos").toString().c_str()));

	imageMeasurementsTable->setRowCount(imageKeyList.size());
	for (unsigned int i = 0; i < imageKeyList.size(); i++)
	{
		QTableWidgetItem *keyItem = new QTableWidgetItem(QString::number(imageKeyList.at(i)));
		QCheckBox *checkItem = new QCheckBox();
		QTableWidgetItem *imageItem = new QTableWidgetItem(QString::fromUtf8(imageNameList.at(i).c_str()));
		QTableWidgetItem *linItem = new QTableWidgetItem();
		QTableWidgetItem *colItem = new QTableWidgetItem();

		EDomElement imgMeasure = ede.elementByTagAtt("imageCoordinates","image_key",intToString(imageKeyList.at(i)));
		if (imgMeasure.getContent() != "")
		{
			checkItem->setChecked(true);
			QStringList linColStr = QString(imgMeasure.elementByTagName("gml:pos").toString().c_str()).split(" ");
			if (linColStr.size() == 2)
			{
				linItem->setText(linColStr.at(0));
				colItem->setText(linColStr.at(1));
			}
		}

		imageMeasurementsTable->setColumnHidden(0,true);

		imageMeasurementsTable->setItem(i,0,keyItem);
		imageMeasurementsTable->setCellWidget(i,1,checkItem);
		imageMeasurementsTable->setItem(i,2,imageItem);
		imageMeasurementsTable->setItem(i,3,linItem);
		imageMeasurementsTable->setItem(i,4,colItem);
	}
}

string PointForm::getvalues()
{
    stringstream auxStream;
	auxStream << "<point key=\"" << key << "\" type=\"" << getType() <<"\">\n";
	auxStream << "\t<gcp_id>" << lineEdit_gcp_id->text().toUtf8().data() << "</gcp_id>\n";
    auxStream << "\t<description>" << lineEditDescription->text().toUtf8().data() << "</description>\n";
	auxStream << "\t<spatialCoordinates uom=\"#" << eDoubleSpinBox->suffix().toStdString().c_str() << "\">\n";
	auxStream << "\t\t<gml:pos>" << doubleToString(eDoubleSpinBox->value()) << " " << doubleToString(nDoubleSpinBox->value()) << " " << doubleToString(hDoubleSpinBox->value()) << "</gml:pos>\n";
    auxStream << "\t<sigma>" << lineEditSigma->text().toUtf8().data() << "</sigma>\n";
	auxStream << "\t</spatialCoordinates>";
	auxStream << "\t<imagesMeasurements>\n";

	for (unsigned int i = 0; i < imageKeyList.size(); i++)
	{
		QCheckBox* myCheck = (QCheckBox*) imageMeasurementsTable->cellWidget(i,1);
		if (myCheck != NULL && myCheck->checkState())
		{
			auxStream << "\t\t<imageCoordinates uom=\"#px\" image_key=\"";
			auxStream << imageMeasurementsTable->item(i,0)->text().toStdString();
			auxStream << "\"><gml:pos>";
			auxStream << imageMeasurementsTable->item(i,3)->text().toStdString();
			auxStream << " ";
			auxStream << imageMeasurementsTable->item(i,4)->text().toStdString();
			auxStream << "</gml:pos>";
			auxStream << "</imageCoordinates>\n";
		}
	}

	// Aqui dever´a entrar um loop para alimentar cada uma das image coordinates
	//auxStream << "\t<imageCoordinates>" << lineEditImageCoordinates1->text().toUtf8().data() << "</imageCoordinates>\n";
    //auxStream << "\t<imageCoordinates>" << lineEditImageCoordinates2->text().toUtf8().data() << "</imageCoordinates>\n";

	auxStream << "\t</imagesMeasurements>\n";
	auxStream << "</point>";
	return auxStream.str();
}

void PointForm::setReadOnly (bool state)
{
	typeComboBox->setEnabled(!state);
    lineEdit_gcp_id->setReadOnly(state);
	lineEditDescription->setReadOnly(state);
	eDoubleSpinBox->setReadOnly(state);
	nDoubleSpinBox->setReadOnly(state);
	hDoubleSpinBox->setReadOnly(state);
    lineEditSigma->setReadOnly(state);
	//lineEditImageCoordinates1->setReadOnly(state);
    //lineEditImageCoordinates2->setReadOnly(state);
}

void PointForm::setType(string type)
{
	if (!type.compare("control"))
		typeComboBox->setCurrentIndex(0);
	if (!type.compare("verification"))
		typeComboBox->setCurrentIndex(1);
	if (!type.compare("photogrammetric"))
		typeComboBox->setCurrentIndex(2);
}

string PointForm::getType()
{
	if (typeComboBox->currentIndex() == 0)
		return "control";
	if (typeComboBox->currentIndex() == 1)
		return "verification";
	if (typeComboBox->currentIndex() == 2)
		return "photogrammetric";
}

bool PointForm::isForm(string formName)
{
	return !formName.compare("PointForm");
}