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
* \brief Classe que gerencia a ortoimagem e ortorretificação.
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
	* \brief Método que inclui os dados das imagens no formulário.
	*/
	void addImagesToForm();
	Orthorectification *ortho;
	/**
	* \brief Método que executa a ortoimagem para uma imagem.
	*/
	void runOrthoIndividual(int);
	/**
	* \brief Método que executa a ortoimagem para todas as imagens.
	*/
	void runAllOrthoTogheter();
	bool flag_cancel;
	bool show_image;
	int inter_method;

public:

	// Constructors and Destructors
	//
	/**
	* \brief Construtor padrão.
	*/
	OrthoManager();
	/**
	* \brief Construtor que já identifica o seu gerenciador, as imagens que serão usadas e os dados de uma orientação exterior a ser extraídas.
	*/
	OrthoManager(EFotoManager* manager, deque<Image*> images, deque<ExteriorOrientation*> eos);
	/**
	* \brief Destrutor padrão.
	*/
	~OrthoManager();

	// Association Methods
	//
	/**
	* \brief Método de associação que seta uma interface.
	*/
	void setInterface(OrthoUserInterface* newInterface);
	/**
	* \brief Método que retorna a Interface corrente.
	*/
	OrthoUserInterface* getInterface();

	// Other Methods
	//
	/**
	* \brief Método que executa a ortoimagem.
	*/
	bool exec();
	/**
	* \brief Método que interrompe a ortoimagem.
	*/
	void returnProject();
	/**
	* \brief Método que altera a barra de progresso na interface.
	*/
	void setProgress(int);
	/**
	* \brief Método que carrega a grade do DEM a partir de um arquivo.
	*/
	int loadDemGrid(char * filename, int fileType);
	/**
	* \brief Método que executa a ortorretificação.
	*/
	int orthoRectification(char * filename, int fileType, int option, double user_res_x, double user_res_y);
	/**
	* \brief Método que interrompe a ortorretificação.
	*/
	void setFlagCancel() { flag_cancel = true; };
	/**
	* \brief Construtor.
	*/
	void setShowImage(bool _shw) { show_image = _shw; };
	/**
	* \brief Método que altera o método de interpolação.
	*/
    void setInterMethod(int _mth) { inter_method = _mth; };
    /**
    * \brief Método que carrega a ortorretificação.
    */
    void loadOrtho(char *filename);
    /**
    * \brief Registra no XML o endereço de um arquivo de Ortho.
    */
    void addOrthoToXML(string filename);
    /**
    * \brief Registra no XML o endereço de um arquivo de Ortho.
    */
    void addOrthoToXML2(string filename);

    /**
    * \brief Registra no XML o endereço de um arquivo de Ortho.
    */
    Orthorectification * getOrtho() { return ortho; };

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //OrthoMANAGER_H
