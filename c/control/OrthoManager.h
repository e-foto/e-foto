/**************************************************************************
														   OrthoManager.h
**************************************************************************/


#ifndef ORTHOMANAGER_H
#define ORTHOMANAGER_H

#include "EObject.h"
#include <deque>
#include "Image.h"
#include "ExteriorOrientation.h"
#include "Matrix.h"
#include "DemGrid.h"
#include "ProjectiveRay.h"
#include "Orthorectification.h"
#include "Interpolation.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class OrthoUserInterface;
class EFotoManager;

/**
* \file OrthoManager.h
* \class OrthoManager
* \brief Classe que gerencia a ortoimagem e ortorretifica��o.
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
	deque<Image*> listAllImages;
	deque<ExteriorOrientation*> listEOs;
	DemGrid *grid;
	/**
	* \brief M�todo que inclui os dados das imagens no formul�rio.
	*/
	void addImagesToForm();
	Orthorectification *ortho;
	/**
	* \brief M�todo que executa a ortoimagem para uma imagem.
	*/
	void runOrthoIndividual(int);
	/**
	* \brief M�todo que executa a ortoimagem para todas as imagens.
	*/
	void runAllOrthoTogheter();
	bool flag_cancel;
	bool show_image;
	int inter_method;

public:

	// Constructors and Destructors
	//
	/**
	* \brief Construtor padr�o.
	*/
	OrthoManager();
	/**
	* \brief Construtor que j� identifica o seu gerenciador, as imagens que ser�o usadas e os dados de uma orienta��o exterior a ser extra�das.
	*/
	OrthoManager(EFotoManager* manager, deque<Image*> images, deque<ExteriorOrientation*> eos);
	/**
	* \brief Destrutor padr�o.
	*/
	~OrthoManager();

	// Association Methods
	//
	/**
	* \brief M�todo de associa��o que seta uma interface.
	*/
	void setInterface(OrthoUserInterface* newInterface);
	/**
	* \brief M�todo que retorna a Interface corrente.
	*/
	OrthoUserInterface* getInterface();

	// Other Methods
	//
	/**
	* \brief M�todo que executa a ortoimagem.
	*/
	bool exec();
	/**
	* \brief M�todo que interrompe a ortoimagem.
	*/
	void returnProject();
	/**
	* \brief M�todo que altera a barra de progresso na interface.
	*/
	void setProgress(int);
	/**
	* \brief M�todo que carrega a grade do DEM a partir de um arquivo.
	*/
	int loadDemGrid(char * filename, int fileType);
	/**
	* \brief M�todo que executa a ortorretifica��o.
	*/
	int orthoRectification(char * filename, int fileType, int option, double user_res_x, double user_res_y);
	/**
	* \brief M�todo que interrompe a ortorretifica��o.
	*/
	void setFlagCancel() { flag_cancel = true; };
	/**
	* \brief Construtor.
	*/
	void setShowImage(bool _shw) { show_image = _shw; };
	/**
	* \brief M�todo que altera o m�todo de interpola��o.
	*/
    void setInterMethod(int _mth) { inter_method = _mth; };
    /**
    * \brief M�todo que carrega a ortorretifica��o.
    */
    void loadOrtho(char *filename);
    /**
    * \brief Registra no XML o endere�o de um arquivo de Ortho.
    */
    void addOrthoToXML(string filename);
    /**
    * \brief Registra no XML o endere�o de um arquivo de Ortho.
    */
    void addOrthoToXML2(string filename);

    /**
    * \brief Registra no XML o endere�o de um arquivo de Ortho.
    */
    Orthorectification * getOrtho() { return ortho; };

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //OrthoMANAGER_H
