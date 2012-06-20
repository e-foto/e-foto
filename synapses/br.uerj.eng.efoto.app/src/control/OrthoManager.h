/**************************************************************************
														   OrthoManager.h
**************************************************************************/


#ifndef ORTHOMANAGER_H
#define ORTHOMANAGER_H

#include "EObject.h"
#include "Matrix.h"
#include <deque>

#include "Image.h"
#include "DemGrid.h"
#include "ExteriorOrientation.h"
#include "Interpolation.h"
#include "ProjectiveRay.h"
#include "Orthorectification.h"

#ifdef INTEGRATED_EFOTO
#include "Project.h"
#endif //INTEGRATED_EFOTO
#ifdef SYNAPSE_EFOTO
#include <IProject.h>
#endif //SYNAPSE_EFOTO

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class OrthoUserInterface;
class EFotoManager;

/**
* \file OrthoManager.h
* \class OrthoManager
* \brief Classe que gerencia a ortoimagem e ortorretificao.
* \attention Imagens que variam entre 0,0 e 1,0.
* \copyright E-Foto group
* \authors Marcelo Teixeira Silveira 
*/
class OrthoManager
{
	// Private Attributes
	//
	bool started;
	bool status;
	OrthoUserInterface* myInterface;
	EFotoManager* manager;

	DemGrid *grid;
    Orthorectification *ortho;

#ifdef INTEGRATED_EFOTO
    Project* project;
#endif //INTEGRATED_EFOTO
#ifdef SYNAPSE_EFOTO
    engine::IProjectPtr project;
#endif //SYNAPSE_EFOTO

	/**
	* \brief Mtodo que inclui os dados das imagens no formulrio.
	*/
    void addImagesToForm();
	/**
	* \brief Mtodo que executa a ortoimagem para uma imagem.
	*/
	void runOrthoIndividual(int);
	/**
	* \brief Mtodo que executa a ortoimagem para todas as imagens.
	*/
	void runAllOrthoTogheter();
	bool flag_cancel;
	bool show_image;
	int inter_method;

public:

	// Constructors and Destructors
	//
	/**
	* \brief Construtor padro.
	*/
    OrthoManager();
	/**
	* \brief Destrutor padro.
	*/
	~OrthoManager();

    /**
    * \brief Construtor que j identifica o seu gerenciador, as imagens que sero usadas e os dados de uma orientao exterior a ser extradas.
    */
    OrthoManager(EFotoManager* manager);

	// Association Methods
	//
	/**
    * \brief Mtodo de associao que seta uma interface.
	*/
	void setInterface(OrthoUserInterface* newInterface);
	/**
	* \brief Mtodo que retorna a Interface corrente.
	*/
	OrthoUserInterface* getInterface();

	// Other Methods
	//
	/**
	* \brief Mtodo que executa a ortoimagem.
	*/
	bool exec();
	/**
	* \brief Mtodo que interrompe a ortoimagem.
	*/
	void returnProject();
	/**
	* \brief Mtodo que altera a barra de progresso na interface.
	*/
	void setProgress(int);
	/**
	* \brief Mtodo que carrega a grade do DEM a partir de um arquivo.
	*/
	int loadDemGrid(char * filename, int fileType);
	/**
	* \brief Mtodo que executa a ortorretificao.
	*/
	int orthoRectification(char * filename, int fileType, int option, double user_res_x, double user_res_y);
	/**
	* \brief Mtodo que interrompe a ortorretificao.
	*/
    void setFlagCancel() { flag_cancel = true; }
	/**
	* \brief Construtor.
	*/
    void setShowImage(bool _shw) { show_image = _shw; }
	/**
	* \brief Mtodo que altera o mtodo de interpolao.
	*/
    void setInterMethod(int _mth) { inter_method = _mth; }
    /**
    * \brief Mtodo que carrega a ortorretificao.
    */
    void loadOrtho(char *filename);
    /**
    * \brief Registra no XML o endereo de um arquivo de Ortho.
    */
    void addOrthoToXML(string filename);
    /**
    * \brief Registra no XML o endereo de um arquivo de Ortho.
    */
    void addOrthoToXML2(string filename);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //OrthoMANAGER_H
