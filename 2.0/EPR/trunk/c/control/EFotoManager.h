/*******************************************************************************
		EFotoManager.h
*******************************************************************************/


#ifndef EFOTOMANAGER_H
#define EFOTOMANAGER_H

#include "EObject.h"
#include "Project.h"
#include "XmlUpdater.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class ReportManager;

class EFotoManager
{

	int nextModule;
	int nextImage;

	bool savedState;
	string interfaceType;

    ReportManager* report;
    XmlUpdater updater;
    Project project;

public:

	/**
	* \brief Construtor padrão.
	*/
	EFotoManager();

	/**
	* \brief Destrutor padrão.
	*/
	~EFotoManager();

	/**
	* \brief Método que seta o tipo de interface em que o programa irá rodar.
	* \param newInterfaceType O tipo de interface.
	*/
	void setInterfaceType(string newInterfaceType);

	/**
	* \brief Método que retorna o tipo de interface corrente.
	* \return string O tipo de interface corrente.
	*/
	string getInterfaceType();

	/**
	* \brief Método que seta o próximo módulo a ser executado.
	* \param newModule  O próximo módulo a ser executado.
	*/
	void setNextModule(int newModule);

	/**
	* \brief Método que seta a próxima imagem a ser processada.
	* \param newImage  A próxima imagem a ser executada.
	*/
	void setNextImage(int newImage);

	/**
	* \brief Método que executa um loop de execuções principais.
	* \return bool Retorna verdadeiro se as execuções principais estejam sendo executadas corretamente. Retorna falso, caso contrário.
	*/
    bool exec();

    /**
	* \brief Método que carrega o módulo de Report.
	* \return bool Retorna verdadeiro se o módulo FotoTri já tiver terminado de ser carregado. Retorna falso, caso contrário.
	*/
        bool execEPR();

	/**
	* \brief Método que encerra o módulo de Report.
	*/
        void stopEPR();

    bool loadProject();

    Project* getProject() {return &project;}

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // PROJECT_H
