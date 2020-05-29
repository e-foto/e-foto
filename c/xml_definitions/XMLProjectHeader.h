#ifndef XMLPROJECTHEADER_H
#define XMLPROJECTHEADER_H
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

#include <string>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class XMLProjectHeader
{
protected:

    virtual void setName(std::string newName) = 0;
    virtual void setDescription(std::string newDescription) = 0;
    virtual void setFilePath(std::string newFilePath) = 0;
    virtual void setFileName(std::string newFileName) = 0;
    virtual void setCreation(std::string newCreation) = 0;
    virtual void setModification(std::string newModification) = 0;
    virtual void setOwner(std::string newOwner) = 0;
    virtual void setAims(std::string newAims) = 0;
    virtual void setContext(std::string newContext) = 0;

    virtual std::string getName() = 0;
    virtual std::string getDescription() = 0;
    virtual std::string getFilePath() = 0;
    virtual std::string getFileName() = 0;
    virtual std::string getCreation() = 0;
    virtual std::string getModification() = 0;
    virtual std::string getOwner() = 0;
    virtual std::string getAims() = 0;
    virtual std::string getContext() = 0;

public:
	XMLProjectHeader();

	// XML methods
    void xmlSetData(std::string xml);
    std::string xmlGetData();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // XMLPROJECTHEADER_H
