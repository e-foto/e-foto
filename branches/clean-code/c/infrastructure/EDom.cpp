/*******************************************************************************
   EDOM.cpp
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

#ifdef WIN32
#define _USE_MATH_DEFINES // for C++
#include <cmath>
#endif
#ifdef unix
#include <math.h>
#endif

#include "EDom.h"

#include <sstream>
#include <stdexcept>
#include <iomanip>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

std::string Conversion::doubleToString(double value)
{
    std::stringstream converter;
    converter << std::setprecision(16);
    converter << value;
    return converter.str();
}

std::string Conversion::doubleToString(double value, int precision)
{
    std::stringstream converter;
    converter << std::setprecision(precision);
    converter << std::fixed;
    converter << value;
    return converter.str();
}

double Conversion::stringToDouble(std::string value)
{
    double result;
    std::stringstream converter;
    converter << std::setprecision(16);
    converter << value;
    converter >> result;
    return result;
}

std::string Conversion::intToString(int value)
{
    std::stringstream converter;
    converter << value;
    return converter.str();
}

bool Conversion::stringToBool(std::string value)
{
    bool result;
    std::stringstream converter;
    converter << value;
    converter >> std::boolalpha >> result;
    return result;
}

int Conversion::stringToInt(std::string value)
{
    int result;
    std::stringstream converter;
    converter << value;
    converter >> result;
    return result;
}

// Private methods
int EDomElement::tagType(std::string tag)
{
    if (tag.at(0) != '<'
            || tag.at(tag.length() - 1) != '>'
            || tag.at(1) == '>'
            || tag.at(1) == ' '
            || tag.at(1) == '\t'
            || tag.at(1) == '\n'
            || tag.find("<?") != std::string::npos
            || tag.find("?>") != std::string::npos
            || tag.find("<!") != std::string::npos
            ) {
        return INVALID_TAG;
    }

    if (tag.find("</") == std::string::npos
            && tag.find("/>") == std::string::npos) {
        return OPEN_TAG;
    }
    if (tag.at(1) == '/' && tag.find("/>") == std::string::npos) {
        return CLOSE_TAG;
    }
    if (tag.at(1) != '/' && tag.find("/>") != std::string::npos) {
        return SIMPLE_TAG;
    }
    return INVALID_TAG;
}

std::string EDomElement::tagName(std::string tag)
{
    auto first = 0;

    if (tagType(tag) == INVALID_TAG) {
        return "";
    } else if (tagType(tag) == OPEN_TAG || tagType(tag) == SIMPLE_TAG) {
        first = 1;
    } else if (tagType(tag) == CLOSE_TAG) {
        first = 2;
    }

    auto last = first;

    try {
        while (!(tag.at(last) == '>'
                 || tag.at(last) == ' '
                 || tag.at(last) == '\t'
                 || tag.at(last) == '\n'
                 || tag.at(last) == '/')) {
            last++;
        }

        return tag.substr(first, last - first);
    } catch (std::out_of_range& e) {
        return "";
    }
}

bool EDomElement::setAttribute(std::string att, std::string newAttValue)
{
    std::string tag = "";

    try {
        if (content.at(0) == '<') {
            tag = content.substr(0, content.find('>') + 1);
        }
    } catch (std::out_of_range& e) {
        return false;
    }

    if (tagType(tag) == INVALID_TAG || tagType(tag) == CLOSE_TAG) {
        return false;
    }

    unsigned long pos1 = content.find(att);

    if (pos1 == std::string::npos) {
        return false;
    }

    unsigned long pos2 = content.find('\"', pos1);
    unsigned long pos3 = content.find('\"', pos2 + 1);
    std::string value = "\"";
    value += newAttValue;
    value += "\"";
    content.replace(pos2, pos3 - pos2 + 1, value);
    return true;
}

bool EDomElement::addAttribute(std::string newAttName, std::string newAttValue)
{
    if (attribute(newAttName) != "") {
        return false;
    }

    std::string tag = "";

    try {
        if (content.at(0) == '<') {
            tag = content.substr(0, content.find('>') + 1);
        }
    } catch (std::out_of_range& e) {
        return false;
    }

    if (tagType(tag) == INVALID_TAG || tagType(tag) == CLOSE_TAG) {
        return false;
    }

    unsigned long pos = content.find('>');
    std::string value = " ";
    value += newAttName;
    value += "=\"";
    value += newAttValue;
    value += "\"";
    content.insert(pos, value);
    return true;
}

// Constructors and Destructors
//
EDomElement::EDomElement():
    content{""}
{
}

EDomElement::EDomElement(std::string myXml):
    content{myXml}
{
}

EDomElement::~EDomElement()
{
}

// Attribute accessor methods
//
void EDomElement::setContent(std::string newXml)
{
    content = newXml;
}

std::string EDomElement::getContent() const
{
    return content;
}

// XML methods
//
std::deque<EDomElement> EDomElement::children()
{
    std::deque<EDomElement> result;

    try {
        unsigned long opening = 1;
        unsigned long closing = 0;
        while (opening < content.length()) {
            if (content.at(opening) == '<') {
                closing = content.find(">", opening) + 1;

                if (tagType(content.substr(opening, closing - opening)) == OPEN_TAG) {
                    unsigned long nesting = 1;
                    unsigned long pos = opening + 1;

                    while (nesting > 0) {
                        pos = content.find("<", pos);
                        {
                            closing = content.find(">", pos) + 1;

                            if (tagType(content.substr(pos, closing - pos)) == OPEN_TAG) {
                                nesting++;
                            } else if (tagType(content.substr(pos, closing - pos)) == CLOSE_TAG) {
                                nesting--;
                            }
                        }
                        pos = closing;
                    }
                } else if (tagType(content.substr(opening, closing - opening)) != SIMPLE_TAG) {
                    closing = opening;
                }

                if (closing != opening) {
                    EDomElement* item = new EDomElement(content.substr(opening, closing - opening));
                    result.push_back(*item);
                    opening = closing - 1;
                }
            }

            opening++;
        }

        return result;
    } catch (std::out_of_range& e) {
        std::deque<EDomElement> dummy;
        dummy.clear();
        return dummy;
    }
}

std::deque<EDomElement> EDomElement::elementsByTagName(std::string name)
{
    std::deque<EDomElement> result;

    try {
        unsigned long opening = 0;
        unsigned long closing = 0;

        while (opening < content.length()) {
            if (content.at(opening) == '<') {
                closing = content.find(">", opening) + 1;

                if (tagName(content.substr(opening, closing - opening)).compare(name) == 0) {
                    if (tagType(content.substr(opening, closing - opening)) == OPEN_TAG) {
                        unsigned long nesting = 1;
                        unsigned long pos = opening + 1;

                        while (nesting > 0) {
                            pos = content.find("<", pos);
                            {
                                closing = content.find(">", pos) + 1;

                                if (tagType(content.substr(pos, closing - pos)) == OPEN_TAG) {
                                    nesting++;
                                } else if (tagType(content.substr(pos, closing - pos)) == CLOSE_TAG) {
                                    nesting--;
                                }
                            }
                            pos = closing;
                        }
                    } else if (tagType(content.substr(opening, closing - opening)) != SIMPLE_TAG) {
                        closing = opening;
                    }

                    if (closing != opening) {
                        EDomElement* item = new EDomElement(content.substr(opening, closing - opening));
                        result.push_back(*item);
                    }
                }
            }

            opening++;
        }

        return result;
    } catch (std::out_of_range& e) {
        std::deque<EDomElement> dummy;
        dummy.clear();
        return dummy;
    }
}

std::deque<EDomElement> EDomElement::elementsByTagAtt(std::string tagname,
        std::string att, std::string value)
{
    std::deque<EDomElement> result, temp;
    temp = elementsByTagName(tagname);

    for (unsigned int i = 0; i < temp.size(); i++)
        if (temp.at(i).attribute(att).compare(value) == 0) {
            result.push_back(temp.at(i));
        }

    return result;
}

EDomElement EDomElement::elementByTagName(std::string tagname)
{
    EDomElement result;
    std::deque<EDomElement> list = elementsByTagName(tagname);

    if (!list.empty()) {
        result = list.at(0);
    }

    return result;
}

EDomElement EDomElement::elementByTagAtt(std::string tagname, std::string att,
        std::string value)
{
    EDomElement result;
    std::deque<EDomElement> list = elementsByTagAtt(tagname, att, value);

    if (!list.empty()) {
        result = list.at(0);
    }

    return result;
}

bool EDomElement::addChildAtTagName(std::string tagname, std::string newChild)
{
    try {
        bool result = false;
        unsigned long opening = 0;
        unsigned long closing = 0;

        while (opening < content.length()) {
            if (content.at(opening) == '<') {
                closing = content.find(">", opening) + 1;

                if (tagName(content.substr(opening, closing - opening)).compare(tagname) == 0) {
                    if (tagType(content.substr(opening, closing - opening)) == OPEN_TAG) {
                        unsigned long nesting = 1;
                        unsigned long pos = opening + 1;

                        while (nesting > 0) {
                            pos = content.find("<", pos);
                            {
                                closing = content.find(">", pos) + 1;

                                if (tagType(content.substr(pos, closing - pos)) == OPEN_TAG) {
                                    nesting++;
                                } else if (tagType(content.substr(pos, closing - pos)) == CLOSE_TAG) {
                                    nesting--;
                                }
                            }
                            pos = closing;
                        }
                    } else if (tagType(content.substr(opening, closing - opening)) != OPEN_TAG) {
                        closing = opening;
                    }

                    if (closing != opening) {
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
    } catch (std::out_of_range& e) {
        return false;
    }
}

bool EDomElement::replaceChildByTagName(std::string tagname,
                                        std::string newChild)
{
    try {
        bool result = false;
        unsigned long opening = 0;

        while (opening < content.length()) {
            if (content.at(opening) == '<') {
                unsigned long closing = content.find(">", opening) + 1;

                if (tagName(content.substr(opening, closing - opening)).compare(tagname) == 0) {
                    if (tagType(content.substr(opening, closing - opening)) == OPEN_TAG) {
                        unsigned long nesting = 1;
                        unsigned long pos = opening + 1;

                        while (nesting > 0) {
                            pos = content.find("<", pos);
                            {
                                closing = content.find(">", pos) + 1;

                                if (tagType(content.substr(pos, closing - pos)) == OPEN_TAG) {
                                    nesting++;
                                } else if (tagType(content.substr(pos, closing - pos)) == CLOSE_TAG) {
                                    nesting--;
                                }
                            }
                            pos = closing;
                        }
                    } else if (tagType(content.substr(opening, closing - opening)) != SIMPLE_TAG) {
                        closing = opening;
                    }

                    if (closing != opening) {
                        content.replace(opening, closing - opening, newChild);
                        result = true;
                    }
                }
            }

            opening++;
        }

        return result;
    } catch (std::out_of_range& e) {
        return false;
    }
}

bool EDomElement::replaceChildByTagAtt(std::string tagname, std::string att,
                                       std::string value, std::string newChild)
{
    try {
        bool result = false;
        unsigned long opening = 0;

        while (opening < content.length()) {
            if (content.at(opening) == '<') {
                unsigned long closing = content.find(">", opening) + 1;

                if (tagName(content.substr(opening, closing - opening)).compare(tagname) == 0) {
                    if (tagType(content.substr(opening, closing - opening)) == OPEN_TAG) {
                        unsigned long nesting = 1;
                        unsigned long pos = opening + 1;

                        while (nesting > 0) {
                            pos = content.find("<", pos);
                            {
                                closing = content.find(">", pos) + 1;

                                if (tagType(content.substr(pos, closing - pos)) == OPEN_TAG) {
                                    nesting++;
                                } else if (tagType(content.substr(pos, closing - pos)) == CLOSE_TAG) {
                                    nesting--;
                                }
                            }
                            pos = closing;
                        }
                    } else if (tagType(content.substr(opening, closing - opening)) != SIMPLE_TAG) {
                        closing = opening;
                    }

                    if (closing != opening) {
                        EDomElement tester(content.substr(opening, closing - opening));

                        if (tester.attribute(att).compare(value) == 0) {
                            content.replace(opening, closing - opening, newChild);
                            result = true;
                        }
                    }
                }
            }

            opening++;
        }

        return result;
    } catch (std::out_of_range& e) {
        return false;
    }
}

std::string EDomElement::tagName()
{
    std::string tag = "";

    try {
        if (content.at(0) == '<') {
            tag = content.substr(0, content.find('>') + 1);
        }
    } catch (std::out_of_range& e) {
        return "";
    }

    if (tagType(tag) == INVALID_TAG || tagType(tag) == CLOSE_TAG) {
        return "";
    }




    try {
        int last = 1;
        while (!(tag.at(last) == '>' || tag.at(last) == ' ' || tag.at(last) == '\t'
                 || tag.at(last) == '\n' || tag.at(last) == '/')) {
            last++;
        }
        return tag.substr(1, last - 1);
    } catch (std::out_of_range& e) {
        return "";
    }
}

std::string EDomElement::attribute(std::string att)
{
    std::string result = "";

    try {
        unsigned long pos = 0;

        while (pos < content.length() && content.at(pos) != '>') {
            if (content.substr(pos, att.length()).compare(att) == 0
                    && content.at(pos - 1) == ' ' && content.at(pos + att.length()) == '=') {
                pos = content.find("\"", pos) + 1;
                result = content.substr(pos, content.find("\"", pos) - pos);
                pos = content.find("\"", pos) + 1;
            }

            pos++;
        }

        return result;
    } catch (std::out_of_range& e) {
        return "";
    }
}

bool EDomElement::addAttributeByTagName(std::string tagname, std::string newAtt,
                                        std::string newAttValue)
{
    EDomElement ede = elementByTagName(tagname);

    if (!ede.addAttribute(newAtt, newAttValue)) {
        return false;
    }

    replaceChildByTagName(tagname, ede.getContent());
    return true;
}

bool EDomElement::replaceAttributeByTagName(std::string tagname,
        std::string replaceAtt, std::string newAttValue)
{
    EDomElement ede = elementByTagName(tagname);

    if (!ede.setAttribute(replaceAtt, newAttValue)) {
        return false;
    }

    replaceChildByTagName(tagname, ede.getContent());
    return true;
}

bool EDomElement::replaceAttributeByTagAtt(std::string tagname, std::string att,
        std::string value, std::string replaceAtt, std::string newAttValue)
{
    EDomElement ede = elementByTagAtt(tagname, att, value);

    if (!ede.setAttribute(replaceAtt, newAttValue)) {
        return false;
    }

    replaceChildByTagAtt(tagname, att, value, ede.getContent());
    return true;
}

std::string EDomElement::toString()
{
    std::string result = "";

    try {
        if (content.find("/") != (content.find(">") - 1)) { //Melhorar isso.
            result = content.substr(content.find(">") + 1, content.find("<",
                                    1) - content.find(">") - 1);
        }

        return result;
    } catch (std::out_of_range& e) {
        return "";
    }
}

bool EDomElement::toBool()
{
    try {
        int result = 0;

        if (content.find("/") != (content.find(">") - 1)) { //Melhorar isso.
            std::string value = content.substr(content.find(">") + 1, content.find("<",
                                               1) - content.find(">") - 1);
            result = Conversion::stringToBool(value);
        }

        return result;
    } catch (std::out_of_range& e) {
        return 0;
    }
}

int EDomElement::toInt()
{
    try {
        int result = 0;
        if (content.find("/") != (content.find(">") - 1)) { //Melhorar isso.
            std::string value = content.substr(content.find(">") + 1, content.find("<",
                                               1) - content.find(">") - 1);
            result = Conversion::stringToInt(value);
        }

        return result;
    } catch (std::out_of_range& e) {
        return 0;
    }
}

double EDomElement::toDouble()
{
   try {
        double result = 0.0;
        if (content.find("/") != (content.find(">") - 1)) { //Melhorar isso.
            std::string value = content.substr(content.find(">") + 1, content.find("<",
                                               1) - content.find(">") - 1);
            result = Conversion::stringToDouble(value);
        }

        return result;
    } catch (std::out_of_range& e) {
        return 0.0;
    }
}

std::deque<double> EDomElement::toGmlPos()
{
    std::deque<double> result;
    std::string values = toString();  

    try {
        unsigned int pos = 0;
        while (pos < values.length()) {
            double value;
            int limit{0};

            if (values.find(" ", pos) != std::string::npos) {
                limit = values.find(" ", pos);
            } else {
                limit = values.length();
            }

            std::string valueStr = values.substr(pos, limit - pos);
            value = Conversion::stringToDouble(valueStr);
            result.push_back(value);
            pos = limit + 1;
        }

        return result;
    } catch (std::out_of_range& e) {
        std::deque<double> dummy;
        dummy.clear();
        return dummy;
    }
}

bool EDomElement::isAvailable()
{
    if (children().size() == 0) {
        std::string value = toString();

        for (std::string::size_type i = 0; i < value.length(); i++) {
            value[i] = toupper (value[i]);
        }

        if (value == "NOT AVAILABLE" || value == "N/A" || value == "") {
            return false;
        }
    }

    return true;
}

bool EDomElement::hasTagName(std::string tagname)
{
    EDomElement check = this->elementByTagName(tagname);

    if (check.tagName() == tagname) {
        return true;
    } else {
        return false;
    }
}

// Test method
//
EDomElement EDomElement::indent(char indentation)
{
    std::stringstream result;


    try {
        long nesting = 0;
        bool firstchar = true;

        for (unsigned long pos = 0; pos < content.length(); pos++) {
            if ((content.at(pos) == '<' && content.at(pos + 1) == '/')
                    || (content.at(pos) == '/' && content.at(pos + 1) == '>')) {
                nesting--;
            } else if (content.at(pos) == '<' && !(content.at(pos + 1) == '?'
                                                   || content.at(pos + 1) == '!')) {
                nesting++;
            }

            if (firstchar == true) {
                if (content.at(pos) != ' ' && content.at(pos) != '\t') {
                    firstchar = false;
                    result << content.at(pos);
                }
            } else {
                if (content.at(pos) == '\n') {
                    firstchar = true;
                }

                result << content.at(pos);
            }

            if (content.at(pos) == '\n' && pos + 2 < content.length()
                    && !(content.find("<", pos) == content.find("/", pos) - 1))
                for (long indents = 0; indents < nesting; indents++) {
                    result << indentation;
                }
            else if (content.at(pos) == '\n')
                for (long indents = 0; indents < nesting - 1; indents++) {
                    result << indentation;
                }
        }

        return EDomElement(result.str());
    } catch (std::out_of_range& e) {
        return EDomElement();
    }
}

EDomElement EDomElement::removeBlankLines(bool removeIndentation)
{
    std::stringstream result;

    try {
        bool isBlank = true;
        unsigned long lineBegin = 0;

        for (unsigned long pos = 0; pos < content.length(); pos++) {
            if (isBlank == true) {
                if (content.at(pos) != ' ' && content.at(pos) != '\t'
                        && content.at(pos) != '\r' && content.at(pos) != '\n') {
                    isBlank = false;

                    if (!removeIndentation) {
                        for (; lineBegin < pos; lineBegin++) {
                            result << content.at(lineBegin);
                        }
                    }

                    result << content.at(pos);
                } else if (content.at(pos) == '\n') {
                    lineBegin = pos + 1;
                }
            } else {
                if (content.at(pos) == '\n') {
                    isBlank = true;
                    lineBegin = pos + 1;
                }

                result << content.at(pos);
            }
        }

        return EDomElement(result.str());
    } catch (std::out_of_range& e) {
        return EDomElement();
    }
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
