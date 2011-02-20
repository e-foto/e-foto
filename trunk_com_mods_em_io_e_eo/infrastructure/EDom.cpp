/*******************************************************************************
                              EDOM.cpp
*******************************************************************************/

#include "EDom.h"

// Common functions
//
string doubleToString(double value)
{
    stringstream converter;
    converter << setprecision(16);
    converter << value;
    return converter.str();
}

string doubleToString(double value, int precision)
{
	stringstream converter;
	converter << setprecision(precision);
	converter << fixed;
	converter << value;
	return converter.str();
}

double stringToDouble(string value)
{
    double result;
    stringstream converter;
    converter << setprecision(16);
    converter << value;
    converter >> result;
    return result;
}

string intToString(int value)
{
    stringstream converter;
    converter << value;
    return converter.str();
}

int stringToInt(string value)
{
    int result;
    stringstream converter;
    converter << value;
    converter >> result;
    return result;
}

string longToString(long value)
{
    stringstream converter;
    converter << value;
    return converter.str();
}

long stringToLong(string value)
{
    long result;
    stringstream converter;
    converter << value;
    converter >> result;
    return result;
}

bool testAvailable(EDomElement element)
{
    if (element.children().size() == 0)
    {
        string value = element.toString();
        if (value == "Not Available" || value == "Not available" || value == "not available" || value == "N/A" || value == "n/a")
            return false;
    }
    return true;
}

bool testTagName(EDomElement element, string tagname)
{
    EDomElement check = element.elementByTagName(tagname);
    if (check.tagName() == tagname)
        return true;
    else
        return false;
}

// Private methods
//
int EDomElement::tagType(string tag)
{
    if (tag.find("<") != 0)
        return INVALID_TAG;
    else if (tag.find(">") != tag.length() - 1)
        return INVALID_TAG;
    else if (tag.find(">") == 1)
        return INVALID_TAG;
    else if (tag.at(1) == ' ' || tag.at(1) == '\t' || tag.at(1) == '\n')
        return INVALID_TAG;
    else if (tag.find("<?") != string::npos || tag.find("?>") != string::npos)
        return INVALID_TAG;
    else if (tag.find("<!") != string::npos)
        return INVALID_TAG;
    else if (tag.find("</") == string::npos && tag.find("/>") == string::npos)
        return OPEN_TAG;
    else if (tag.find("</") == 0 && tag.find("/>") == string::npos)
        return CLOSE_TAG;
    else if (tag.find("</") != 0 && tag.find("/>") != string::npos)
        return SIMPLE_TAG;
    else
        return INVALID_TAG;
}

string EDomElement::tagName(string tag)
{
    int first = 0;
    int last;

    if (tagType(tag) == INVALID_TAG)
        return "";
    else if (tagType(tag) == OPEN_TAG || tagType(tag) == SIMPLE_TAG)
        first = 1;
    else if (tagType(tag) == CLOSE_TAG)
        first = 2;

    last = first;

    try
    {
        while (not(tag.at(last) == '>' || tag.at(last) == ' ' || tag.at(last) == '\t' || tag.at(last) == '\n' || tag.at(last) == '/'))
            last++;
        return tag.substr(first, last - first);
    }
    catch (std::out_of_range& e)
    {

        return "";
    }
}

bool EDomElement::setAttribute(string att, string newAttValue)
{
	string tag = "";
	try
	{
		if (content.at(0) == '<')
		{
			tag = content.substr(0, content.find('>') + 1);
		}
	}
	catch (std::out_of_range& e)
	{
		return false;
	}

	if (tagType(tag) == INVALID_TAG || tagType(tag) == CLOSE_TAG)
		return false;

	unsigned long pos1 = content.find(att);
	if (pos1 == string::npos)
		return false;
	unsigned long pos2 = content.find('\"', pos1);
	unsigned long pos3 = content.find('\"', pos2 + 1);
	string value = "\"";
	value += newAttValue;
	value += "\"";
	content.replace(pos2, pos3-pos2+1, value);

	return true;
}

bool EDomElement::addAttribute(string newAttName, string newAttValue)
{
	if (attribute(newAttName) != "")
		return false;

	string tag = "";
	try
	{
		if (content.at(0) == '<')
		{
			tag = content.substr(0, content.find('>') + 1);
		}
	}
	catch (std::out_of_range& e)
	{
		return false;
	}

	if (tagType(tag) == INVALID_TAG || tagType(tag) == CLOSE_TAG)
		return false;

	unsigned long pos = content.find('>');
	string value = " ";
	value += newAttName;
	value += "=\"";
	value += newAttValue;
	value += "\"";
	content.insert(pos, value);

	return true;
}

// Constructors and Destructors
//
EDomElement::EDomElement()
{
    content = "";
}

EDomElement::EDomElement(string myXml)
{
    content = myXml;
}

EDomElement::~EDomElement()
{
}

// Attribute accessor methods
//
void EDomElement::setContent(string newXml)
{
    content = newXml;
}

string EDomElement::getContent()
{
    return content;
}

// XML methods
//
deque<EDomElement> EDomElement::children()
{
    deque<EDomElement> result;
    EDomElement* item;
    unsigned long opening = 1;
    unsigned long closing = 0;
    unsigned long pos = 0;
    unsigned long nesting = 0;

    try
    {
        while (opening < content.length())
        {
            if (content.at(opening) == '<')
            {
                closing = content.find(">", opening) + 1;
                if (tagType(content.substr(opening, closing - opening)) == OPEN_TAG)
                {
                    nesting = 1;
                    pos = opening + 1;
                    while (nesting > 0)
                    {
                        pos = content.find("<", pos);
                        {
                            closing = content.find(">", pos) + 1;
                            if (tagType(content.substr(pos, closing - pos)) == OPEN_TAG)
                                nesting++;
                            else if (tagType(content.substr(pos, closing - pos)) == CLOSE_TAG)
                                nesting--;
                        }
                        pos = closing;
                    }
                }
                else if (tagType(content.substr(opening, closing - opening)) != SIMPLE_TAG)
                    closing = opening;
                if (closing != opening)
                {
                    item = new EDomElement(content.substr(opening, closing - opening));
                    result.push_back(*item);
                    opening = closing - 1;
                }
            }
            opening++;
        }
        return result;
    }
    catch (std::out_of_range& e)
    {

        deque<EDomElement> dummy;
        dummy.clear();
        return dummy;
    }
}

deque<EDomElement> EDomElement::elementsByTagName(string name)
{
    deque<EDomElement> result;
    EDomElement* item;
    unsigned long opening = 0;
    unsigned long closing = 0;
    unsigned long pos = 0;
    unsigned long nesting = 0;

    try
    {
        while (opening < content.length())
        {
            if (content.at(opening) == '<')
            {
                closing = content.find(">", opening) + 1;
                if (tagName(content.substr(opening, closing - opening)).compare(name) == 0)
                {
                    if (tagType(content.substr(opening, closing - opening)) == OPEN_TAG)
                    {
                        nesting = 1;
                        pos = opening + 1;
                        while (nesting > 0)
                        {
                            pos = content.find("<", pos);
                            {
                                closing = content.find(">", pos) + 1;
                                if (tagType(content.substr(pos, closing - pos)) == OPEN_TAG)
                                    nesting++;
                                else if (tagType(content.substr(pos, closing - pos)) == CLOSE_TAG)
                                    nesting--;
                            }
                            pos = closing;
                        }
                    }
                    else if (tagType(content.substr(opening, closing - opening)) != SIMPLE_TAG)
                        closing = opening;
                    if (closing != opening)
                    {
                        item = new EDomElement(content.substr(opening, closing - opening));
                        result.push_back(*item);
                    }
                }
            }
            opening++;
        }
        return result;
    }
    catch (std::out_of_range& e)
    {

        deque<EDomElement> dummy;
        dummy.clear();
        return dummy;
    }
}

deque<EDomElement> EDomElement::elementsByTagAtt(string tagname, string att, string value)
{
    deque<EDomElement> result, temp;
    temp = elementsByTagName(tagname);
    for (unsigned int i = 0; i < temp.size(); i++)
        if (temp.at(i).attribute(att).compare(value) == 0)
            result.push_back(temp.at(i));
    return result;
}

EDomElement EDomElement::elementByTagName(string tagname)
{
    EDomElement result;
    deque<EDomElement> list = elementsByTagName(tagname);
    if (!list.empty())
        result = list.at(0);
    return result;
}

EDomElement EDomElement::elementByTagAtt(string tagname, string att, string value)
{
    EDomElement result;
    deque<EDomElement> list = elementsByTagAtt(tagname, att, value);
    if (!list.empty())
        result = list.at(0);
    return result;
}

bool EDomElement::addChildAtTagName(string tagname, string newChild)
{
    bool result = false;
    unsigned long opening = 0;
    unsigned long closing = 0;
    unsigned long pos = 0;
    unsigned long nesting = 0;

    try
    {
        while (opening < content.length())
        {
            if (content.at(opening) == '<')
            {
                closing = content.find(">", opening) + 1;
                if (tagName(content.substr(opening, closing - opening)).compare(tagname) == 0)
                {
                    if (tagType(content.substr(opening, closing - opening)) == OPEN_TAG)
                    {
                        nesting = 1;
                        pos = opening + 1;
                        while (nesting > 0)
                        {
                            pos = content.find("<", pos);
                            {
                                closing = content.find(">", pos) + 1;
                                if (tagType(content.substr(pos, closing - pos)) == OPEN_TAG)
                                    nesting++;
                                else if (tagType(content.substr(pos, closing - pos)) == CLOSE_TAG)
                                    nesting--;
                            }
                            pos = closing;
                        }
                    }
                    else if (tagType(content.substr(opening, closing - opening)) != OPEN_TAG)
                        closing = opening;
                    if (closing != opening)
                    {
                        int position = content.rfind("<", closing);
                        content.insert(position, "\n");
                        content.insert(position, newChild);
                        result = true;
                    }
                }
            }
            opening++;
        }
        return result;
    }
    catch (std::out_of_range& e)
    {

        return false;
    }
}

bool EDomElement::addChildAtTagAtt(string tagname, string att, string value, string newChild)
{
    bool result = false;
    unsigned long opening = 0;
    unsigned long closing = 0;
    unsigned long pos = 0;
    unsigned long nesting = 0;

    try
    {
        while (opening < content.length())
        {
            if (content.at(opening) == '<')
            {
                closing = content.find(">", opening) + 1;
                if (tagName(content.substr(opening, closing - opening)).compare(tagname) == 0)
                {
                    if (tagType(content.substr(opening, closing - opening)) == OPEN_TAG)
                    {
                        nesting = 1;
                        pos = opening + 1;
                        while (nesting > 0)
                        {
                            pos = content.find("<", pos);
                            {
                                closing = content.find(">", pos) + 1;
                                if (tagType(content.substr(pos, closing - pos)) == OPEN_TAG)
                                    nesting++;
                                else if (tagType(content.substr(pos, closing - pos)) == CLOSE_TAG)
                                    nesting--;
                            }
                            pos = closing;
                        }
                    }
                    else if (tagType(content.substr(opening, closing - opening)) != OPEN_TAG)
                        closing = opening;
                    if (closing != opening)
                    {
                        EDomElement tester(content.substr(opening, closing - opening));
                        if (tester.attribute(att).compare(value) == 0)
                        {
                            int position = content.rfind("<", closing);
                            content.insert(position, "\n");
                            content.insert(position, newChild);
                            result = true;
                        }
                    }
                }
            }
            opening++;
        }
        return result;
    }
    catch (std::out_of_range& e)
    {

        return false;
    }
}

bool EDomElement::replaceChildByTagName(string tagname, string newChild)
{
    bool result = false;
    unsigned long opening = 0;
    unsigned long closing = 0;
    unsigned long pos = 0;
    unsigned long nesting = 0;

    try
    {
        while (opening < content.length())
        {
            if (content.at(opening) == '<')
            {
                closing = content.find(">", opening) + 1;
                if (tagName(content.substr(opening, closing - opening)).compare(tagname) == 0)
                {
                    if (tagType(content.substr(opening, closing - opening)) == OPEN_TAG)
                    {
                        nesting = 1;
                        pos = opening + 1;
                        while (nesting > 0)
                        {
                            pos = content.find("<", pos);
                            {
                                closing = content.find(">", pos) + 1;
                                if (tagType(content.substr(pos, closing - pos)) == OPEN_TAG)
                                    nesting++;
                                else if (tagType(content.substr(pos, closing - pos)) == CLOSE_TAG)
                                    nesting--;
                            }
                            pos = closing;
                        }
                    }
                    else if (tagType(content.substr(opening, closing - opening)) != SIMPLE_TAG)
                        closing = opening;
                    if (closing != opening)
                    {
                        content.replace(opening, closing - opening, newChild);;
                        result = true;
                    }
                }
            }
            opening++;
        }
        return result;
    }
    catch (std::out_of_range& e)
    {

        return false;
    }
}

bool EDomElement::replaceChildByTagAtt(string tagname, string att, string value, string newChild)
{
    bool result = false;
    unsigned long opening = 0;
    unsigned long closing = 0;
    unsigned long pos = 0;
    unsigned long nesting = 0;

    try
    {
        while (opening < content.length())
        {
            if (content.at(opening) == '<')
            {
                closing = content.find(">", opening) + 1;
                if (tagName(content.substr(opening, closing - opening)).compare(tagname) == 0)
                {
                    if (tagType(content.substr(opening, closing - opening)) == OPEN_TAG)
                    {
                        nesting = 1;
                        pos = opening + 1;
                        while (nesting > 0)
                        {
                            pos = content.find("<", pos);
                            {
                                closing = content.find(">", pos) + 1;
                                if (tagType(content.substr(pos, closing - pos)) == OPEN_TAG)
                                    nesting++;
                                else if (tagType(content.substr(pos, closing - pos)) == CLOSE_TAG)
                                    nesting--;
                            }
                            pos = closing;
                        }
                    }
                    else if (tagType(content.substr(opening, closing - opening)) != SIMPLE_TAG)
                        closing = opening;
                    if (closing != opening)
                    {
                        EDomElement tester(content.substr(opening, closing - opening));
                        if (tester.attribute(att).compare(value) == 0)
                        {
                            content.replace(opening, closing - opening, newChild);
                            result = true;
                        }
                    }
                }
            }
            opening++;
        }
        return result;
    }
    catch (std::out_of_range& e)
    {

        return false;
    }
}

string EDomElement::tagName()
{
    string tag = "";
    try
    {
        if (content.at(0) == '<')
        {
            tag = content.substr(0, content.find('>') + 1);
        }
    }
    catch (std::out_of_range& e)
    {

        return "";
    }

    if (tagType(tag) == INVALID_TAG || tagType(tag) == CLOSE_TAG)
        return "";

    int first = 1;
    int last = 1;

    try
    {
        while (not(tag.at(last) == '>' || tag.at(last) == ' ' || tag.at(last) == '\t' || tag.at(last) == '\n' || tag.at(last) == '/'))
            last++;
        return tag.substr(first, last - first);
    }
    catch (std::out_of_range& e)
    {

        return "";
    }
}

string EDomElement::attribute(string att)
{
    string result = "";
    unsigned long pos = 0;
    try
    {
        while (pos < content.length() && content.at(pos) != '>')
        {
            if (content.substr(pos, att.length()).compare(att) == 0 && content.at(pos - 1) == ' ' && content.at(pos + att.length()) == '=')
            {
                pos = content.find("\"", pos) + 1;
                result = content.substr(pos, content.find("\"", pos) - pos);
                pos = content.find("\"", pos) + 1;
            }
            pos++;
        }
        return result;
    }
    catch (std::out_of_range& e)
    {

        return "";
    }
}

bool EDomElement::addAttributeByTagName(string tagname, string newAtt, string newAttValue)
{
	EDomElement ede = elementByTagName(tagname);
	if (!ede.addAttribute(newAtt,newAttValue))
		return false;
	replaceChildByTagName(tagname,ede.getContent());
	return true;
}

bool EDomElement::addAttributeByTagAtt(string tagname, string att, string value, string newAtt, string newAttValue)
{
	EDomElement ede = elementByTagAtt(tagname,att,value);
	if (!ede.addAttribute(newAtt,newAttValue))
		return false;
	replaceChildByTagAtt(tagname,att,value,ede.getContent());
	return true;
}

bool EDomElement::replaceAttributeByTagName(string tagname, string replaceAtt, string newAttValue)
{
	EDomElement ede = elementByTagName(tagname);
	if (!ede.setAttribute(replaceAtt,newAttValue))
		return false;
	replaceChildByTagName(tagname,ede.getContent());
	return true;
}

bool EDomElement::replaceAttributeByTagAtt(string tagname, string att, string value, string replaceAtt, string newAttValue)
{
	EDomElement ede = elementByTagAtt(tagname,att,value);
	if (!ede.setAttribute(replaceAtt,newAttValue))
		return false;
	replaceChildByTagAtt(tagname,att,value,ede.getContent());
	return true;
}

string EDomElement::toString()
{
    string result = "";
    try
    {
        if (content.find("/") != (content.find(">") - 1)) //Melhorar isso.
        {
            result = content.substr(content.find(">") + 1, content.find("<", 1) - content.find(">") - 1);
        }
        return result;
    }
    catch (std::out_of_range& e)
    {

        return "";
    }
}

int EDomElement::toInt()
{
    int result = 0;
    try
    {
        if (content.find("/") != (content.find(">") - 1)) //Melhorar isso.
        {
            string value = content.substr(content.find(">") + 1, content.find("<", 1) - content.find(">") - 1);
            result = stringToInt(value);
        }
        return result;
    }
    catch (std::out_of_range& e)
    {

        return 0;
    }
}

long EDomElement::toLong()
{
    long result = 0;
    try
    {
        if (content.find("/") != (content.find(">") - 1)) //Melhorar isso.
        {
            string value = content.substr(content.find(">") + 1, content.find("<", 1) - content.find(">") - 1);
            result = stringToLong(value);
        }
        return result;
    }
    catch (std::out_of_range& e)
    {

        return 0;
    }
}

double EDomElement::toDouble()
{
    double result = 0.0;
    try
    {
        if (content.find("/") != (content.find(">") - 1)) //Melhorar isso.
        {
            string value = content.substr(content.find(">") + 1, content.find("<", 1) - content.find(">") - 1);
            result = stringToDouble(value);
        }
        return result;
    }
    catch (std::out_of_range& e)
    {

        return 0.0;
    }
}

deque<double> EDomElement::toGmlPos()
{
    deque<double> result;
    string values = toString();
    unsigned int pos = 0;
    int limit;
    try
    {
        while (pos < values.length())
        {
            stringstream converter;
            double value;
            if (values.find(" ", pos) != string::npos)
                limit = values.find(" ", pos);
            else
                limit = values.length();
            string valueStr = values.substr(pos, limit - pos);
            value = stringToDouble(valueStr);
            result.push_back(value);
            pos = limit + 1;
        }
        return result;
    }
    catch (std::out_of_range& e)
    {

        deque<double> dummy;
        dummy.clear();
        return dummy;
    }
}

bool EDomElement::isAvailable()
{
    if (children().size() == 0)
    {
        string value = toString();
        if (value == "Not Available" || value == "")
            return false;
    }
    return true;
}

bool EDomElement::hasTagName(string tagname)
{
    EDomElement check = this->elementByTagName(tagname);
    if (check.tagName() == tagname)
        return true;
    else
        return false;
}

// Test method
//
EDomElement EDomElement::indent(char indentation)
{
    stringstream result;
    long nesting = 0;
    bool firstchar = true;
    try
    {
        for (unsigned long pos = 0; pos < content.length(); pos++)
        {
            if ((content.at(pos) == '<' && content.at(pos + 1) == '/') || (content.at(pos) == '/' && content.at(pos + 1) == '>'))
                nesting--;
            else if (content.at(pos) == '<' && !(content.at(pos + 1) == '?' || content.at(pos + 1) == '!'))
                nesting++;
            if (firstchar == true)
            {
                if (content.at(pos) != ' ' && content.at(pos) != '\t')
                {
                    firstchar = false;
                    result << content.at(pos);
                }
            }
            else
            {
                if (content.at(pos) == '\n')
                    firstchar = true;
                result << content.at(pos);
            }

            if (content.at(pos) == '\n' && pos + 2 < content.length() && !(content.find("<", pos) == content.find("/", pos) - 1))
                for (long indents = 0; indents < nesting; indents++)
                    result << indentation;
            else if (content.at(pos) == '\n')
                for (long indents = 0; indents < nesting - 1; indents++)
                    result << indentation;
        }
		return EDomElement(result.str());
    }
    catch (std::out_of_range& e)
    {
		return EDomElement();
    }
}

EDomElement EDomElement::trim(char charToTrim)
{
	stringstream result;
	bool checkIndentation = true;
	try
	{
		for (unsigned long pos = 0; pos < content.length(); pos++)
		{
			if (checkIndentation == true)
			{
				if (content.at(pos) != charToTrim)
				{
					checkIndentation = false;
					result << content.at(pos);
				}
				//else ; //Indentation is removed
			}
			else
			{
				if (content.at(pos) == '\n')
					checkIndentation = true;
				result << content.at(pos);
			}
		}
		return EDomElement(result.str());
	}
	catch (std::out_of_range& e)
	{
		return EDomElement();
	}
}

EDomElement EDomElement::removeBlankLines(bool removeIndentation)
{
	stringstream result;
	bool isBlank = true;
	unsigned long lineBegin = 0;
	try
	{
		for (unsigned long pos = 0; pos < content.length(); pos++)
		{
			if (isBlank == true)
			{
				if (content.at(pos) != ' ' && content.at(pos) != '\t' && content.at(pos) != '\r' && content.at(pos) != '\n')
				{
					isBlank = false;
					if (!removeIndentation)
					{
						for (lineBegin; lineBegin < pos; lineBegin++)
						{
							result << content.at(lineBegin);
						}
					}
					result << content.at(pos);
				}
				else if (content.at(pos) == '\n')
				{
					lineBegin = pos + 1;
				}
			}
			else
			{
				if (content.at(pos) == '\n')
				{
					isBlank = true;
					lineBegin = pos + 1;
				}
				result << content.at(pos);
			}
		}
		return EDomElement(result.str());
	}
	catch (std::out_of_range& e)
	{
		return EDomElement();
	}
}
