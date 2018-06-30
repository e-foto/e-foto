/*******************************************************************************
   ProjectHeader.cpp
*******************************************************************************/
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

#include "ProjectHeader.h"

#include <sstream>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

ProjectHeader::~ProjectHeader()
{
}

std::string ProjectHeader::getName() const
{
    return name;
}

std::string ProjectHeader::getDescription() const
{
    return description;
}

std::string ProjectHeader::getCreation() const
{
    return creation;
}

std::string ProjectHeader::getModification() const
{
    return modification;
}

std::string ProjectHeader::getOwner() const
{
    return owner;
}

void ProjectHeader::xmlSetData(std::string xml)
{
    EDomElement root(xml);
    name = root.elementByTagName("name").toString();
    description = root.elementByTagName("description").toString();
    filePath = root.elementByTagName("filePath").toString();
    fileName = root.elementByTagName("fileName").toString();
    creation = root.elementByTagName("creation").toString();
    modification = root.elementByTagName("modification").toString();
    owner = root.elementByTagName("owner").toString();
    aims = root.elementByTagName("aims").toString();
    context = root.elementByTagName("context").toString();
}

std::string ProjectHeader::xmlGetData()
{
    std::stringstream result;
    result << "<projectHeader>\n";
    result << "<name>" << name << "</name>\n";
    result << "<description>" << description << "</description>\n";
    result << "<filePath>" << filePath << "</filePath>\n";
    result << "<fileName>" << fileName << "</fileName>\n";
    result << "<creation>" << creation << "</creation>\n";
    result << "<modification>" << modification << "</modification>\n";
    result << "<owner>" << owner << "</owner>\n";
    result << "<aims>" << aims << "</aims>\n";
    result << "<context>" << context << "</context>\n";
    result << "</projectHeader>\n";
    return result.str();
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
