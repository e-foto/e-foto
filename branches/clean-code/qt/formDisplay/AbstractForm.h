#ifndef ABSTRACTFORM_H
#define ABSTRACTFORM_H
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

#include <QWidget>
#include <string>

class AbstractForm : public QWidget
{
    Q_OBJECT

public:
    explicit AbstractForm(QWidget *parent = 0) : QWidget(parent) {}

    virtual void fillvalues(std::string values) = 0;
    virtual std::string getvalues() = 0;
    virtual void setReadOnly(bool state) = 0;
};



#endif // ABSTRACTFORM_H
