/*******************************************************************************
   EDOM.h
*******************************************************************************/

#ifndef EDOM_H
#define EDOM_H

#include <string>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <deque>

using namespace std;

/**
* class EDOM
*
* @author Rafael Alves de Aguiar
*
* * * * * * * * * * * *
* @date 22/4/2010
* @version 2.1 - Rafael Alves de Aguiar
*/

/*******************************************************************************

Disclaimer: EDom and EDomElement are two classes made to solve all of E-Foto's
XML needs. They were never meant to be a full DOM system, so a lot of things
still aren't considered. Use this at your own risk.

New in version 2.0: EDom should not crash on badly formed XML strings anymore.
New in version 2.1: Added methods to insert new attributes.

*******************************************************************************/

class EDomElement;

// Common functions
//
string doubleToString(double value);
string doubleToString(double value, int precision);
double stringToDouble(string value);
string intToString(int value);
int stringToInt(string value);
string longToString(long value);
long stringToLong(string value);
bool testAvailable(EDomElement element);
bool testTagName(EDomElement element, string tagname);

class EDomElement
{
	// Constants
	//
	enum { INVALID_TAG = 0, OPEN_TAG = 1, CLOSE_TAG = 2, SIMPLE_TAG = 3 };

	// Private attributes
	//
	string content;

	// Private methods (functions)
	//
	int tagType(string myTag);
	string tagName(string myTag);

public:

	// Constructors and Destructors
	//
	EDomElement();
	EDomElement(string myXml);
	~EDomElement();

	// Attribute accessor methods
	//
	void setContent(string newXml);
	string getContent();

	// XML methods
	//
	deque<EDomElement> children();
	deque<EDomElement> elementsByTagName(string name);
	deque<EDomElement> elementsByTagAtt(string tagname, string att, string value);
	EDomElement elementByTagName(string tagname);
	EDomElement elementByTagAtt(string tagname, string att, string value);
	bool addChildAtTagName(string tagname, string newChild);
	bool addChildAtTagAtt(string tagname, string att, string value, string newChild);
	bool replaceChildByTagName(string tagname, string newChild);
	bool replaceChildByTagAtt(string tagname, string att, string value, string newChild);
	string tagName();
	string attribute(string att);
	bool setAttribute(string att, string newAttValue);
	bool addAttribute(string newAttName, string newAttValue);
	string toString();
	int toInt();
	long toLong();
	double toDouble();
	deque<double> toGmlPos();
	bool isAvailable();
	bool hasTagName(string tagname);

	// Test method
	//
	EDomElement indent(char indentation);
	EDomElement trim(char charToTrim);
	EDomElement removeBlankLines(bool removeIndentation = false);
};

class EDom
{

public:

	// XML methods
	//
	virtual string xmlGetData() = 0;
	virtual void xmlSetData(string xml) = 0;

};

#endif //EDOM_H
