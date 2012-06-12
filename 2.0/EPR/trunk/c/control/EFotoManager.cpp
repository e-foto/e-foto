/*******************************************************************************
		  EFotoManager.cpp
*******************************************************************************/

#include "EFotoManager.h"
#include "ReportManager.h"
#include "LoadingScreen.h"
#include <QApplication>
#include <QFileDialog>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

// Constructors and destructor
//

/**
 *
 */
EFotoManager::EFotoManager() : updater("")
{
    savedState = true;
	report = NULL;
    nextModule = 1;
}

/**
 *
 */
EFotoManager::~EFotoManager()
{

}

/**
 *
 */
void EFotoManager::setInterfaceType(string newInterfaceType)
{
	interfaceType = newInterfaceType;
}

/**
 *
 */
string EFotoManager::getInterfaceType()
{
	return interfaceType;
}

/**
 *
 */
void EFotoManager::setNextModule(int newModule)
{
	nextModule = newModule;
}

/**
 *
 */
void EFotoManager::setNextImage(int newImage)
{
	nextImage = newImage;
}

bool EFotoManager::execEPR()
{
	bool result;

	nextModule = 2;

	report = new ReportManager(this);

	result = report->exec();

	return result;
}

void EFotoManager::stopEPR()
{
	if (report != NULL)
		delete report;
	report = NULL;
}

/**
 *
 */
bool EFotoManager::exec()
{
	switch (nextModule)
	{
	case 1:
        if (loadProject())
            execEPR();
        else
            exit(0);
        break;
    }

	return true;
}

bool EFotoManager::loadProject()
{
    QString filename = QFileDialog::getOpenFileName(0, "Open File", ".", "*.epp");

    if (!filename.isEmpty())
    {
        stringstream myData;
        ifstream myFile(filename.toStdString().c_str());

        if (myFile.is_open())
        {
            string line;
            while (!myFile.eof())
            {
                getline (myFile,line);
                myData << line << endl;
            }
            myFile.close();

            string xmlData = EDomElement(myData.str()).removeBlankLines(true).getContent();
            updater = XmlUpdater(xmlData);
            if (updater.isUpdated())
            {
                xmlData = updater.getAllXml().getContent();
            }
            project.setXml(xmlData);
            return true;
        }
    }
    return false;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
