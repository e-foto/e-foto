/**************************************************************************
OrthoManager.h
**************************************************************************/
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

#ifndef ORTHOMANAGER_H
#define ORTHOMANAGER_H

#include <deque>
#include <string>
#include "DemGrid.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class OrthoUserInterface;
class EFotoManager;

class DemGrid;
class ExteriorOrientation;
class Image;
class Orthorectification;

/**
* \file OrthoManager.h
* \class OrthoManager
* \brief Classe que gerencia a ortoimagem e ortorretificação.
* \attention Imagens que variam entre 0,0 e 1,0.
* \copyright E-Foto group
* \authors Marcelo Teixeira Silveira
*/
class OrthoManager {
    // Private Attributes
    bool started;
    OrthoUserInterface* myInterface;
    EFotoManager* manager_;
    Orthorectification* ortho;
    std::deque<Image*> listAllImages;
    std::deque<ExteriorOrientation*> listEOs;
    DemGrid* grid;
    bool flag_cancel;
    bool show_image;
    int inter_method;
    /**
    * \brief Método que inclui os dados das imagens no formulário.
    */
    void addImagesToForm();
    /**
    * \brief Método que executa a ortoimagem para uma imagem.
    */
    void runOrthoIndividual(int);
    /**
    * \brief Método que executa a ortoimagem para todas as imagens.
    */
    void runAllOrthoTogether();

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
    explicit OrthoManager(EFotoManager* manager_, std::deque<Image*> images,
                          std::deque<ExteriorOrientation*> eos);
    /**
    * \brief Destrutor padrão.
    */
    ~OrthoManager();

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
    void setProgress(int) const;
    /**
    * \brief Método que carrega a grade do DEM a partir de um arquivo.
    */
    int loadDemGrid(char* filename, Filetype fileType);
    /**
    * \brief Método que executa a ortorretificação.
    * \todo solve unused parameter fileType
    */
    int orthoRectification(char* filename, int option, double user_res_x,
                           double user_res_y);
    /**
    * \brief Mtodo que executa a ortorretificao e salva em formato GeoTiff.
    */
    int orthoRectificationGeoTiff(char* filename, int option, double user_res_x,
                                  double user_res_y);
    /**
    * \brief Método que interrompe a ortorretificação.
    */
    void setFlagCancel()
    {
        flag_cancel = true;
    };
    /**
    * \brief Construtor.
    */
    void setShowImage(bool _shw)
    {
        show_image = _shw;
    };
    /**
    * \brief Método que altera o método de interpolação.
    */
    void setInterMethod(int _mth)
    {
        inter_method = _mth;
    };
    /**
    * \brief Método que carrega a ortorretificação.
    */
    void loadOrtho(char* filename);
    /**
    * \brief Registra no XML o endereço de um arquivo de Ortho.
    */
    void addOrthoToXML(std::string filename);
    /**
    * \brief Registra no XML o endereço de um arquivo de Ortho.
    */
    static void addOrthoToXML2(std::string filename);

    /**
    * \brief Registra no XML o endereço de um arquivo de Ortho.
    */
    Orthorectification* getOrtho()
    {
        return ortho;
    };

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //OrthoMANAGER_H
