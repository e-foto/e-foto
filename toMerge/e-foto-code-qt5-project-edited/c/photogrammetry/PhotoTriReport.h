#ifndef PHOTOTRIREPORT_H
#define PHOTOTRIREPORT_H
/**************************************************************************
PhotoTriReport
**************************************************************************/
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
#include <Matrix.h>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class EFotoManager;
class Project;

/**
* \file PhotoTriReport.h
* \class PhotoTriReport
* \brief Photo-triangulation report generator
* \copyright E-Foto group
* \author Marcelo Teixeira Silveira - July 2013
*/

class PhotoTriReport
{
private:
    EFotoManager* manager;
    Project * project;
    Matrix calculateAvgStd(Matrix m);
    Matrix calculateXYZerror();
    Matrix calculatePixelerror();
    Matrix formatBundleX1();
    Matrix formatBundleX2();

public:
    explicit PhotoTriReport (EFotoManager* newManager);
    int createReport(char *filename);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // PHOTOTRIREPORT_H
