#ifndef ETREEMODEL_H
#define ETREEMODEL_H

#include <deque>
#include <string>
using namespace std;

class ETreeElement
{
    unsigned int id;
    string description;
public:
    ETreeElement();
    ETreeElement(int id, string description);
    ETreeElement(string data);
    unsigned int getId();
    string getDescription();
};

class ETreeNode
{
    string description;
    deque<ETreeElement> children;
public:
    ETreeNode();
    ETreeNode(string description, deque<ETreeElement> children);
    ETreeNode(string data);
    string getDescription();
    unsigned int countChildren();
    deque<ETreeElement> getChildren();
    ETreeElement getChild(unsigned int index);
    string dataAt(unsigned int index);
    unsigned int idAt(unsigned int index);
    deque<string> data();
};

class ETreeModel
{
    deque<ETreeNode> children;
public:
    ETreeModel();
    ETreeModel(deque<ETreeNode> children);
    ETreeModel(string data);
    unsigned int countChildren();
    unsigned int countGrandchildren(unsigned int index);
    deque<ETreeNode> getChildren();
    ETreeNode getChild(unsigned int index);
    string dataAt(unsigned int nodeIndex, unsigned int elementIndex);
    unsigned int idAt(unsigned int nodeIndex, unsigned int elementIndex);
    string dataAt(unsigned int index);
    deque< deque<string> > data();
};

#endif // ETREEMODEL_H
