/***************************************************************************
 *   E-Foto Project: Digital Photogrammetric Workstation                   *
 *   Rio de Janeiro State University                                       *
 *   Program of Post-Graduation in Geomatics                               *
 *                                                                         *
 *   Copyright (C) 2010 by E-Foto Project                                  *
 *                                                                         *
 *   Developers:           Guilherme Lucio Abelha Mota                     *
 *                         Marcos Vinícius Meffe                           *
 *                                                                         *
 *   contact:  guimota at ime.uerj.br                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "blockview.h"
#include "digitalimageview.h"

vector <QString> filterImgXML(QString inputString)
{
   vector <QString> filteredDataVector;
   EDomElement ede(inputString.toAscii().data());
   filteredDataVector.push_back(QString::fromUtf8(ede.elementByTagName("image_id").toString().c_str()));
   filteredDataVector.push_back(QString::fromUtf8(ede.elementByTagName("image").attribute("sensor_key").c_str()));
   filteredDataVector.push_back(QString::fromUtf8(ede.elementByTagName("image").attribute("flight_key").c_str()));
   filteredDataVector.push_back(QString::fromUtf8(ede.elementByTagName("fileName").toString().c_str()));
   return filteredDataVector;
};

ImageBlockDelegate::ImageBlockDelegate(QObject *parent)
     : QItemDelegate(parent)
 {
 }

void ImageBlockDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
 QString ItemText;
 QStyleOptionViewItem myOption = option;
 myOption.displayAlignment=Qt::AlignCenter | Qt::AlignVCenter;
 QBrush MyTextBrush;
 MyTextBrush.setColor(Qt::red);
 myOption.font.setItalic(1);

 vector <QString> filteredText = filterImgXML(index.model()->data(index, Qt::DisplayRole).toString());
    if (filteredText.at(0)=="")
        ItemText = tr("no image");
    else
    {
        if (filteredText.at(1)=="-1")
            ItemText = tr("no sensor");
        else
        {
            if (filteredText.at(2)=="-1")
               ItemText = tr("no flight");
            else
            {
                if (filteredText.at(3)=="")
                    ItemText = tr("no file");
                else
                {
                    // The image data was correctly filled in and validated
                    ItemText = tr("image ") + filteredText.at(0);
                    MyTextBrush.setColor(Qt::black);
                    myOption.font.setItalic(0);
                }
            }
        }
    };
    myOption.palette.setBrush(QPalette::Text, MyTextBrush);
    drawDisplay(painter, myOption,myOption.rect, ItemText);
}

MatricialBlockModel::MatricialBlockModel(int inicialImageId, int blockKey,int rows, int cols)
{
    SelectedCol =-1;
    SelectedRow =-1;
    BlockKey= blockKey;
    stringstream ImageXml;
    for (int i=0; i< rows; i++)
        for (int j=0; j< cols; j++)
        {
            int ImageKey;
            ImageKey = i*cols + j + inicialImageId;
            ImageXml.str("");
            ImageXml << "<image key=\""<< ImageKey <<"\" sensor_key=\"-1\" flight_key=\"-1\"><image_id>"<< /*ImageKey<<*/ "</image_id><width uom=\"#px\"></width>"<<
                    "<height uom=\"#px\"></height><fileName></fileName><filePath></filePath><resolution uom=\"#dpi\"></resolution><spatialCoordinates "<<
                     "uom=\"#m\"><gml:pos></gml:pos></spatialCoordinates></image>";

            QStandardItem *item = new QStandardItem;
            item->setText( QString::QString(ImageXml.str().c_str()) );
            item->setEditable(0);
            setItem(i,j, item);
        }
}

MatricialBlockModel::MatricialBlockModel(void)//AbstractEModel()
{
    SelectedCol = -1;
    SelectedRow = -1;
}

void MatricialBlockModel::fillvalues(vector<string> StrVector)
{
    string BlockString = StrVector[0];
    SensorsString = StrVector[1];
    FlightsString = StrVector[2];


    EDomElement edeBlock(BlockString);

    BlockKey = QString::fromUtf8(edeBlock.elementByTagName("matricialBlock").attribute("key").c_str()).toInt();
    deque <EDomElement> rowsDeque = edeBlock.children();
    int numberRows = rowsDeque.size();
    deque <EDomElement> colsDeque = rowsDeque.at(0).children();
    int numberCols = colsDeque.size();
// if necessary realocate space for the items
    if((numberCols!=columnCount())||(numberRows!=rowCount()))
    {
        clear();
        for(int i=0; i<numberRows ; i++)
        {
            for(int j=0; j<numberCols ; j++)
            {
                QStandardItem *item = new QStandardItem();
                item->setEditable(0);
                setItem(i, j, item);
            }
       }
    }

    for(int i=0; i<numberRows ; i++)
    {
        colsDeque = rowsDeque.at(i).children();
        for(int j=0; j<numberCols ; j++)
        {
            this->item(i, j)->setText(QString::fromUtf8(colsDeque.at(j).getContent().c_str()));
        }
    }
}

string MatricialBlockModel::getvalues()
{
    string BlockXMLString;
    stringstream auxStream;
    auxStream << "<matricialBlock key=\""<< BlockKey<< "\">";
    for (int i = 0; i<rowCount(); i++)
    {
        auxStream << "<blockRow>";
        for (int j = 0; j< columnCount(); j++)
            auxStream << item(i,j)->text().toUtf8().data();
        auxStream << "</blockRow>";
    }
    auxStream << "</matricialBlock>";
    BlockXMLString = auxStream.str();
    return BlockXMLString;
};

void MatricialBlockModel::selectImage(const QModelIndex &index)
{
    SelectedCol = index.column();
    SelectedRow = index.row();
}

string MatricialBlockModel::getSelected(void)
{
    if ((SelectedCol!=-1)&&(SelectedRow!=-1))
    {
        return item(SelectedRow, SelectedCol)->text().toUtf8().data();
    }
    else
        return "";
}

void MatricialBlockModel::setSelected(string ImageXMLString)
{
    if ((SelectedCol!=-1)&&(SelectedRow!=-1))
    {
        item(SelectedRow, SelectedCol)->setText(QString::fromUtf8(ImageXMLString.c_str()));
    }
}

void MatricialBlockModel::showModel()
{
    for(int i=0; i<this->rowCount(); i++)
    {
        for(int j=0; j<this->columnCount() ; j++)
        {
            cout << item(i,j)->text().toUtf8().data() << " ";
        }
        cout << endl;
    }
};


MatricialBlockView::MatricialBlockView(QWidget* parent=0, MatricialBlockModel* pMatricialBlockModel=0):QTableView(parent)//(QWidget* parent=0, MatricialBlockModel* pMatricialBlockModel=0):QTableView(parent)
{
    verticalHeader()->setResizeMode(QHeaderView::Stretch);
    horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    setModel(pMatricialBlockModel);
    setSelectionBehavior (QAbstractItemView::SelectItems);//
    setSelectionMode (QAbstractItemView::SingleSelection);
    setItemDelegate(&ImageIdDelegate);
}

ConnectionBlock2Image::ConnectionBlock2Image(MatricialBlockModel *pMatricialBlockModel,MatricialBlockView *pMatricialBlockView,
                                             DigitalImageModel *pDigitalImageModel, DigitalImageView* pDigitalImageView ):QObject()
{
    myMatricialBlockModel = pMatricialBlockModel;
    myMatricialBlockView = pMatricialBlockView;
    myDigitalImageModel =   pDigitalImageModel;
    myDigitalImageView = pDigitalImageView;
    myDigitalImageView->setEnabled(0);
    QObject::connect(myMatricialBlockView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(FromBlock2Image(const QModelIndex &)));
    QObject::connect(myDigitalImageModel, SIGNAL(itemChanged(QStandardItem*)),this,SLOT(FromImage2Block(QStandardItem*)));// MatricialBlockView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(FromBlock2Image(const QModelIndex &)));
}

void ConnectionBlock2Image::FromBlock2Image(const QModelIndex &index)
{
    myDigitalImageView->setEnabled(1);
    myMatricialBlockModel->setSelected( myDigitalImageModel->getvalues());
        myDigitalImageModel->getvalues();
    myMatricialBlockModel->selectImage(index);
    vector <string> AuxStrVector;
    AuxStrVector.push_back(myMatricialBlockModel->getSelected());
    AuxStrVector.push_back(myMatricialBlockModel->SensorsString);
    AuxStrVector.push_back(myMatricialBlockModel->FlightsString);
    myDigitalImageModel->fillvalues(AuxStrVector);
}

void ConnectionBlock2Image::FromImage2Block(QStandardItem *)//*item)
{
    myMatricialBlockModel->setSelected(myDigitalImageModel->getvalues());
}
