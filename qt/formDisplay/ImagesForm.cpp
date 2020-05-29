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

#include "ImagesForm.h"
#include "EDom.h"
#include <QPicture>
#include <sstream>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

ImagesForm :: ImagesForm(QWidget *parent) : AbstractForm(parent)
{
    setupUi(this);
    connect(imagesTable,SIGNAL(cellClicked(int,int)),this,SLOT(emitSignal(int)));
    imagesTable->setColumnHidden(0,true);
    imagesTable->setColumnWidth(1,80);
    imagesTable->setColumnWidth(2,30);
    imagesTable->setColumnWidth(3,30);

}

void ImagesForm::fillvalues(std::string values)
{
    EDomElement ede(values);
    int rows=ede.children().size();
    imagesTable->setRowCount(rows);

    std::deque<EDomElement> imaChildren = ede.children();
    for (int i=0;i<rows;i++){
        std::string tableParameter;
        std::stringstream auxStream;

        auxStream << imaChildren.at(i).elementByTagName("filePath").toString().c_str();
        auxStream << "/";
        auxStream << imaChildren.at(i).elementByTagName("fileName").toString().c_str();

        tableParameter = auxStream.str();

        QTableWidgetItem *keyItem = new QTableWidgetItem( QString::fromUtf8 (imaChildren.at(i).attribute("key").c_str()) );
        QTableWidgetItem *idItem = new QTableWidgetItem(imaChildren.at(i).elementByTagName("imageId").toString().c_str()) ;
        QTableWidgetItem *fileItem = new QTableWidgetItem(tableParameter.c_str());
        keyItem->setTextAlignment(Qt::AlignCenter);
        idItem->setTextAlignment(Qt::AlignCenter);

        imagesTable->setItem(i,0,keyItem);
        imagesTable->setItem(i,1,idItem);
        imagesTable->setItem(i,4,fileItem);
    }
    imaChildren.clear();
}
std::string ImagesForm::getvalues()
{
    std::string xmlString;
    std::stringstream auxStream;

    auxStream << "\nNot Available\n";

    xmlString = auxStream.str();
    return xmlString;
}

void ImagesForm ::setReadOnly(bool state)
{
    imagesTable->setEnabled(state);
    // ImagesForm is only to read in all times.
}

//emite o sinal da linha(row) correspondente a image key
void ImagesForm :: emitSignal(int i)
{
    //int key=Conversion::stringToInt(imagesTable->item(i,0)->text().toStdString().c_str());
    //emit clicked(key);
    // Provisóriamente nós enviamos a posição da linha clicada e não a key efetivamente.
    emit clicked(i);
}

bool ImagesForm::isForm(std::string formName)
{
    return !formName.compare("HeaderForm");
}

void ImagesForm::setIOsAvailable(std::string xmlIOs)
{
    EDomElement ede(xmlIOs);

    for (int i=0;i<imagesTable->rowCount();i++)
    {
        int key = imagesTable->item(i,0)->text().toInt();
        EDomElement IOXml = ede.elementByTagAtt("imageIO","image_key",Conversion::intToString(key));
        QTableWidgetItem *IOItem = new QTableWidgetItem();
        IOItem->setTextAlignment(Qt::AlignCenter);

        QLabel *label=new QLabel();
        label->setAlignment(Qt::AlignCenter);
        //QLabel *unchecked= new QLabel();

        //checked->setPixmap(QPixmap(":/image/checked.png"));
        //unchecked->setPixmap(QPixmap(":/image/unchecked.png"));
        if(IOXml.getContent() != "")
        {
            label->setPixmap(QPixmap(":/image/checked.png"));


            //IOItem->setIcon(QIcon(":/image/checked.png"));
            /*
            IOItem->setTextColor(QColor("green"));
            IOItem->setText(QString::fromUtf8("✓"));
            IOItem->setFont(QFont("Sans",20,QFont::Bold));
            */
        }
        else
        {
            label->setPixmap(QPixmap(":/image/unchecked.png"));
            //IOItem->setIcon(QIcon(":/image/unchecked.png"));
            /*
   IOItem->setTextColor(QColor("red"));
   IOItem->setText("x");
   IOItem->setFont(QFont("Sans",20));
            */
        }
        imagesTable->setCellWidget(i,2,label);
        //imagesTable->setItem(i,2,IOItem);
    }
}

void ImagesForm::setEOsAvailable(std::string xmlEOs)
{
    EDomElement ede(xmlEOs);

    EDomElement ptri = ede.elementByTagName("phototriangulation");
    EDomElement srs = ede.elementByTagName("spatialResections");

    for (int i=0;i < imagesTable->rowCount();i++)
    {
        int key = imagesTable->item(i,0)->text().toInt();
        EDomElement EOXml = ede.elementByTagAtt("imageEO","image_key",Conversion::intToString(key));

        std::string type = EOXml.attribute("type");
        bool converged = true;
        if (type == "photoTriangulation")
            converged = ptri.elementByTagName("converged").toBool();
        else if (type == "spatialResection")
            converged = srs.elementByTagAtt("imageSR","image_key",Conversion::intToString(key)).elementByTagName("converged").toBool();

        QTableWidgetItem *EOItem = new QTableWidgetItem();
        EOItem->setTextAlignment(Qt::AlignCenter);
        QLabel *label=new QLabel();
        label->setAlignment(Qt::AlignCenter);

        if(EOXml.getContent() != "")
        {
            if (converged)
                label->setPixmap(QPixmap(":/image/checked.png"));
            else
                label->setPixmap(QPixmap(":/image/exclamation.png"));
        }
        else
        {
            label->setPixmap(QPixmap(":/image/unchecked.png"));
        }
        imagesTable->setCellWidget(i,3,label);
    }
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
