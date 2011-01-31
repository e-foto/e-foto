#include "XMLProjectHeader.h"

XMLProjectHeader::XMLProjectHeader()
{
}

void XMLProjectHeader::xmlSetData(string xml)
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

string XMLProjectHeader::xmlGetData()
{
    stringstream result;
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
