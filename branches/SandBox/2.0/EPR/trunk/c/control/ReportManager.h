/**************************************************************************
		  ReportManager.h
**************************************************************************/


#ifndef REPORTMANAGER_H
#define REPORTMANAGER_H

#include "EObject.h"
#include "Project.h"

#include <QtGui>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class ReportUserInterface;
class EFotoManager;

class ReportManager
{
	// Private Attributes
	//
	bool started;
	bool status;
    EFotoManager* efotoManager;
    Project* project;
	ReportUserInterface* myInterface;

public:

	// Constructors and Destructors
	//
	/**
	* \brief Construtor vazio.
	*/
	ReportManager();
	/**
	* \brief Construtor vazio.
	*/
	ReportManager(EFotoManager* manager);
	/**
	* \brief Destrutor padro.
	*/
	~ReportManager();

	// Association Methods
	//
	/**
	* \brief Mtodo de associao que seta uma interface.
	* \param newInterface Interface que ser usada.
	*/
	void setInterface(ReportUserInterface* newInterface);
	/**
	* \brief Mtodo que retorna a Interface corrente.
	* \return IOUserInterface Interface corrente.
	*/
	ReportUserInterface* getInterface();

	// Other Methods
	//
	/**
	* \brief Mtodo que inicia o mdulo de Orientao Interior e retorna se foi feito algum clculo neste mesmo mdulo anteriormente para a alertar o usurio, caso o projeto no tenha sido salvo.
	* \return bool Retorna verdadeiro se o projeto foi salvo. Retorna falso, caso contrrio.
	*/
	bool exec();
	/**
	* \brief Mtodo que carrega a interface do projeto quando o mdulo de Orientao Interior  finalizado.
	*/
	void returnProject();


        //EFotoManager* getEfotoManager();

        EDomElement getHeaderXml();
        EDomElement getTerrainXml();        
        EDomElement getSensorXml();
        EDomElement getFlightXml();
        string getAllXml();

        string eprHeader();
        string eprTerrain();
        string eprSensors();
        string eprFlights();
        string eprImages();
        string eprBlockPoints();
        string eprAffineTransformation();
        string eprSpatialRessection();
        string eprPhotogrammetricBlock();
        string eprInteriorOrientation();
        string eprExteriorOrientation();
        string eprStereoPairs();
        string eprStereoPlotting();
        string eprDSM();
        string eprOrthorectification();

        bool makeFile(string filename, int idExt, QList<QTreeWidgetItem*> treeItems);
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //REPORTMANAGER_H