/***************************************************************************
 *   E-Foto Project: Digital Photogrammetric Workstation                   *
 *   Rio de Janeiro State University                                       *
 *   Program of Post-Graduation in Geomatics                               *
 *                                                                         *
 *   Copyright (C) 2010 by E-Foto Project                                  *
 *                                                                         *
 *   Developers:           Guilherme Lucio Abelha Mota                     *
 *                         Bruno Cardozo Cotrim da Costa                   *
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

#include "SensorForm.h"
#include <QtGui>

SensorForm::SensorForm(QWidget *parent , string XmlString)
{
    QVBoxLayout* MyLayout = new QVBoxLayout(this);
    psensorview = new SensorView(this);
    psensorview->fillvalues(XmlString);
    MyLayout->addWidget(psensorview);
    setLayout(MyLayout);
}


void SensorForm::fillvalues(string values)
{
    psensorview->fillvalues(values);
}

string SensorForm::getvalues()
{
return psensorview->getvalues();
}

void SensorForm::setReadOnly(bool state)
{
    psensorview->setReadOnly(state);
}

bool SensorForm::isForm(string formName)
{
}
