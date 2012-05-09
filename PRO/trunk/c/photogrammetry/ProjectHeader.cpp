/*******************************************************************************
   ProjectHeader.cpp
*******************************************************************************/

#include "ProjectHeader.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

ProjectHeader::~ProjectHeader()
{
}

void ProjectHeader::setName(string newName)
{
	name = newName;
}

void ProjectHeader::setDescription(string newDescription)
{
	description = newDescription;
}

void ProjectHeader::setFilePath(string newFilePath)
{
	filePath = newFilePath;
}

void ProjectHeader::setFileName(string newFileName)
{
	fileName = newFileName;
}

void ProjectHeader::setCreation(string newCreation)
{
	creation = newCreation;
}

void ProjectHeader::setModification(string newModification)
{
	modification = newModification;
}

void ProjectHeader::setOwner(string newOwner)
{
	owner = newOwner;
}

void ProjectHeader::setAims(string newAims)
{
	aims = newAims;
}

void ProjectHeader::setContext(string newContext)
{
	context = newContext;
}

string ProjectHeader::getName()
{
	return name;
}

string ProjectHeader::getDescription()
{
	return description;
}

string ProjectHeader::getFilePath()
{
	return filePath;
}

string ProjectHeader::getFileName()
{
	return fileName;
}

string ProjectHeader::getCreation()
{
	return creation;
}

string ProjectHeader::getModification()
{
	return modification;
}

string ProjectHeader::getOwner()
{
	return owner;
}

string ProjectHeader::getAims()
{
	return aims;
}

string ProjectHeader::getContext()
{
	return context;
}

string ProjectHeader::objectType(void)
{
	return "ProjectHeader";
}

string ProjectHeader::objectAssociations(void)
{
	return "";
}

bool ProjectHeader::is(string s)
{
	return (s == "ProjectHeader" ? true : false);
}

void ProjectHeader::xmlSetData(string xml)
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

string ProjectHeader::xmlGetData()
{
	stringstream result;
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
