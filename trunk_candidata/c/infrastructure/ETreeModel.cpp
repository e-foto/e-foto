#include "ETreeModel.h"
#include "EDom.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

ETreeElement::ETreeElement()
{
	this->id = 0;
	this->description = "";
}

ETreeElement::ETreeElement(int id, string description)
{
	this->id = id;
	this->description = description;
}

ETreeElement::ETreeElement(string data)
{
	EDomElement xmlData(data);
	if (xmlData.tagName() == "sensor")
	{
		id = Conversion::stringToInt(xmlData.attribute("key"));
		description = xmlData.elementByTagName("sensorId").toString();
	}
	else if (xmlData.tagName() == "flight")
	{
		id = Conversion::stringToInt(xmlData.attribute("key"));
		description = xmlData.elementByTagName("flightId").toString();
	}
	else if (xmlData.tagName() == "image")
	{
		id = Conversion::stringToInt(xmlData.attribute("key"));
		description = xmlData.elementByTagName("imageId").toString();
	}
	else if (xmlData.tagName() == "point")
	{
		id = Conversion::stringToInt(xmlData.attribute("key"));
		description = xmlData.elementByTagName("pointId").toString();
	}
}

unsigned int ETreeElement::getId()
{
	return id;
}

string ETreeElement::getDescription()
{
	return description;
}

ETreeNode::ETreeNode()
{
	this->description = "";
}

ETreeNode::ETreeNode(string description, deque<ETreeElement> children)
{
	this->description = description;
	this->children = children;
}

ETreeNode::ETreeNode(string data)
{
	EDomElement xmlData(data);
	description = xmlData.tagName();
	deque<EDomElement> elements = xmlData.children();
	for (unsigned int i = 0; i < elements.size(); i++)
		children.push_back(ETreeElement(elements.at(i).getContent()));
}

string ETreeNode::getDescription()
{
	return description;
}

unsigned int ETreeNode::countChildren()
{
	return children.size();
}

deque<ETreeElement> ETreeNode::getChildren()
{
	return children;
}

ETreeElement ETreeNode::getChild(unsigned int index)
{
	if (index < children.size())
		return children.at(index);
	else
		return ETreeElement();
}

string ETreeNode::dataAt(unsigned int index)
{
	if (index < children.size())
		return children.at(index).getDescription();
	else
		return "";
}

unsigned int ETreeNode::idAt(unsigned int index)
{
	if (index < children.size())
		return children.at(index).getId();
	else
		return 0;
}

deque<string> ETreeNode::data()
{
	deque<string> result;
	for (unsigned int i = 0; i < children.size(); i++)
		result.push_back(children.at(i).getDescription());
	return result;
}

ETreeModel::ETreeModel()
{

}

ETreeModel::ETreeModel(deque<ETreeNode> children)
{
	this->children = children;
}

ETreeModel::ETreeModel(string data)
{
	EDomElement xmlData(data);
	children.push_back(ETreeNode(xmlData.elementByTagName("projectHeader").getContent()));
	children.push_back(ETreeNode(xmlData.elementByTagName("terrain").getContent()));
	children.push_back(ETreeNode(xmlData.elementByTagName("sensors").getContent()));
	children.push_back(ETreeNode(xmlData.elementByTagName("flights").getContent()));

	EDomElement xmlImages = xmlData.elementByTagName("images");
	deque<EDomElement> xmlImageList = xmlImages.elementsByTagName("image");
	stringstream imageList;
	imageList << "<images>\n";
	for (unsigned int i = 0; i < xmlImageList.size(); i++)
	{
		imageList << xmlImageList.at(i).getContent();
	}
	imageList << "</images>\n";
	children.push_back(ETreeNode(imageList.str()));

	children.push_back(ETreeNode(xmlData.elementByTagName("points").getContent()));
}

unsigned int ETreeModel::countChildren()
{
	return children.size();
}

unsigned int ETreeModel::countGrandchildren(unsigned int index)
{
	if (index < children.size())
		return children.at(index).countChildren();
	else
		return 0;
}

deque<ETreeNode> ETreeModel::getChildren()
{
	return children;
}

ETreeNode ETreeModel::getChild(unsigned int index)
{
	if (index < children.size())
		return children.at(index);
	else
		return ETreeNode();
}

string ETreeModel::dataAt(unsigned int nodeIndex, unsigned int elementIndex)
{
	if (nodeIndex < children.size() && elementIndex < children.at(nodeIndex).countChildren())
		return children.at(nodeIndex).dataAt(elementIndex);
	else
		return "";
}

unsigned int ETreeModel::idAt(unsigned int nodeIndex, unsigned int elementIndex)
{
	if (nodeIndex < children.size() && elementIndex < children.at(nodeIndex).countChildren())
		return children.at(nodeIndex).idAt(elementIndex);
	else
		return 0;
}

string ETreeModel::dataAt(unsigned int index)
{
	if (index < children.size())
		return children.at(index).getDescription();
	else
		return "";
}

deque< deque<string> > ETreeModel::data()
{
	deque< deque<string> > result;
	for (unsigned int i = 0; i < children.size(); i++)
		result.push_back(children.at(i).data());
	return result;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
