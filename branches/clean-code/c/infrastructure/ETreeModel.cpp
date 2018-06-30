#include "ETreeModel.h"
#include "EDom.h"

#include <sstream>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

ETreeElement::ETreeElement():ETreeElement(0,"")
{
}

ETreeElement::ETreeElement(int id, std::string description):
    id_{id},
    description_{description}
{
}

ETreeElement::ETreeElement(std::string data)
{
	EDomElement xmlData(data);
	if (xmlData.tagName() == "sensor")
	{
        id_ = Conversion::stringToInt(xmlData.attribute("key"));
        description_ = xmlData.elementByTagName("sensorId").toString();
	}
	else if (xmlData.tagName() == "flight")
	{
        id_ = Conversion::stringToInt(xmlData.attribute("key"));
        description_ = xmlData.elementByTagName("flightId").toString();
	}
	else if (xmlData.tagName() == "image")
	{
        id_ = Conversion::stringToInt(xmlData.attribute("key"));
        description_ = xmlData.elementByTagName("imageId").toString();
	}
	else if (xmlData.tagName() == "point")
	{
        id_ = Conversion::stringToInt(xmlData.attribute("key"));
        description_ = xmlData.elementByTagName("pointId").toString();
	}
}

unsigned int ETreeElement::getId() const
{
    return id_;
}

std::string ETreeElement::getDescription() const
{
    return description_;
}

ETreeNode::ETreeNode():ETreeNode("",std::deque<ETreeElement>())
{
}

ETreeNode::ETreeNode(std::string description, std::deque<ETreeElement> children):
    description_ {description},
    children_{children}
{
}

ETreeNode::ETreeNode(std::string data)
{
	EDomElement xmlData(data);
    description_ = xmlData.tagName();
    std::deque<EDomElement> elements = xmlData.children();
	for (unsigned int i = 0; i < elements.size(); i++)
        children_.push_back(ETreeElement(elements.at(i).getContent()));
}

std::string ETreeNode::getDescription() const
{
    return description_;
}

unsigned int ETreeNode::countChildren() const
{
    return children_.size();
}

ETreeElement ETreeNode::getChild(unsigned int index)
{
    if (index < children_.size())
        return children_.at(index);
	else
		return ETreeElement();
}

std::string ETreeNode::dataAt(unsigned int index)
{
    if (index < children_.size())
        return children_.at(index).getDescription();
	else
		return "";
}

unsigned int ETreeNode::idAt(unsigned int index)
{
    if (index < children_.size())
        return children_.at(index).getId();
	else
		return 0;
}

std::deque<std::string> ETreeNode::data()
{
    std::deque<std::string> result;
    for (unsigned int i = 0; i < children_.size(); i++)
        result.push_back(children_.at(i).getDescription());
	return result;
}

ETreeModel::ETreeModel()
{

}

ETreeModel::ETreeModel(std::deque<ETreeNode> children):
    children_{children}
{    
}

ETreeModel::ETreeModel(std::string data)
{
	EDomElement xmlData(data);
    children_.push_back(ETreeNode(xmlData.elementByTagName("projectHeader").getContent()));
    children_.push_back(ETreeNode(xmlData.elementByTagName("terrain").getContent()));
    children_.push_back(ETreeNode(xmlData.elementByTagName("sensors").getContent()));
    children_.push_back(ETreeNode(xmlData.elementByTagName("flights").getContent()));

	EDomElement xmlImages = xmlData.elementByTagName("images");
    std::deque<EDomElement> xmlImageList = xmlImages.elementsByTagName("image");
    std::stringstream imageList;
	imageList << "<images>\n";
    for (size_t i = 0; i < xmlImageList.size(); i++)
	{
		imageList << xmlImageList.at(i).getContent();
	}
	imageList << "</images>\n";
    children_.push_back(ETreeNode(imageList.str()));

    children_.push_back(ETreeNode(xmlData.elementByTagName("points").getContent()));
}

unsigned int ETreeModel::countChildren() const
{
    return children_.size();
}

unsigned int ETreeModel::countGrandchildren(unsigned int index)
{
    if (index < children_.size())
        return children_.at(index).countChildren();
	else
		return 0;
}

ETreeNode ETreeModel::getChild(unsigned int index)
{
    if (index < children_.size())
        return children_.at(index);
	else
		return ETreeNode();
}

std::string ETreeModel::dataAt(unsigned int nodeIndex, unsigned int elementIndex)
{
    if (nodeIndex < children_.size() && elementIndex < children_.at(nodeIndex).countChildren())
        return children_.at(nodeIndex).dataAt(elementIndex);
	else
		return "";
}

unsigned int ETreeModel::idAt(unsigned int nodeIndex, unsigned int elementIndex)
{
    if (nodeIndex < children_.size() && elementIndex < children_.at(nodeIndex).countChildren())
        return children_.at(nodeIndex).idAt(elementIndex);
	else
		return 0;
}

std::string ETreeModel::dataAt(unsigned int index)
{
    if (index < children_.size())
        return children_.at(index).getDescription();
	else
		return "";
}

std::deque< std::deque<std::string> > ETreeModel::data()
{
    std::deque< std::deque<std::string> > result;
    for (unsigned int i = 0; i < children_.size(); i++)
        result.push_back(children_.at(i).data());
	return result;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
