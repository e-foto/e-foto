#ifndef PROJECTUSERINTERFACE_H
#define PROJECTUSERINTERFACE_H
/**************************************************************************
		ProjectUserInterface.h
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

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class ProjectManager;

/**
  * class ProjectUserInterface
  *
  * @author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 30/06/2009
  * @version 1.0 - Rafael Alves de Aguiar & Irving da Silva Badolato
  *
  */

class ProjectUserInterface //: public EObject
{

protected:

	// Associated Objects
	//
	ProjectManager* manager;

public:

	// Association Methods
	//
	ProjectManager* getManager();

	// Other Methods
	//
    virtual bool exec() = 0;
    virtual void saveFile() = 0;
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //PROJECTUSERINTERFACE_H
