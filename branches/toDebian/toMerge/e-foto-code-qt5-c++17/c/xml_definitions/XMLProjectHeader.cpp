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
#include "XMLProjectHeader.h"

#include "EDom.h"

#include <sstream>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {



XMLProjectHeader::XMLProjectHeader()
{
}

void XMLProjectHeader::xmlSetData(std::string xml)
{
	EDomElement root(xml);

	setName(root.elementByTagName("name").toString());
	setDescription(root.elementByTagName("description").toString());
	setFilePath(root.elementByTagName("filePath").toString());
	setFileName(root.elementByTagName("fileName").toString());
	setCreation(root.elementByTagName("creation").toString());
	setModification(root.elementByTagName("modification").toString());
	setOwner(root.elementByTagName("owner").toString());
	setAims(root.elementByTagName("aims").toString());
	setContext(root.elementByTagName("context").toString());
}

std::string XMLProjectHeader::xmlGetData()
{
    std::stringstream result;
	result << "<projectHeader>\n";
	result << "<name>" << getName() << "</name>\n";
	result << "<description>" << getDescription() << "</description>\n";
	result << "<filePath>" << getFilePath() << "</filePath>\n";
	result << "<fileName>" << getFileName() << "</fileName>\n";
	result << "<creation>" << getCreation() << "</creation>\n";
	result << "<modification>" << getModification() << "</modification>\n";
	result << "<owner>" << getOwner() << "</owner>\n";
	result << "<aims>" << getAims() << "</aims>\n";
	result << "<context>" << getContext() << "</context>\n";
	result << "</projectHeader>\n";
	return result.str();
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
