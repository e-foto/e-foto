#ifndef XMLPROJECTHEADER_H
#define XMLPROJECTHEADER_H

#include "EObject.h"

class XMLProjectHeader
{
protected:

    virtual void setName(string newName) = 0;
    virtual void setDescription(string newDescription) = 0;
    virtual void setFilePath(string newFilePath) = 0;
    virtual void setFileName(string newFileName) = 0;
    virtual void setCreation(string newCreation) = 0;
    virtual void setModification(string newModification) = 0;
    virtual void setOwner(string newOwner) = 0;
    virtual void setAims(string newAims) = 0;
    virtual void setContext(string newContext) = 0;

    virtual string getName() = 0;
    virtual string getDescription() = 0;
    virtual string getFilePath() = 0;
    virtual string getFileName() = 0;
    virtual string getCreation() = 0;
    virtual string getModification() = 0;
    virtual string getOwner() = 0;
    virtual string getAims() = 0;
    virtual string getContext() = 0;

public:
    XMLProjectHeader();

    // XML methods
    void xmlSetData(string xml);
    string xmlGetData();
};

#endif // XMLPROJECTHEADER_H
