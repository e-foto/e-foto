#ifndef SRMANAGER_H
#define SRMANAGER_H
/**************************************************************************
                               SRManager.h
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

#include <deque>
#include <string>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Terrain;
class Sensor;
class Flight;
class Image;
class InteriorOrientation;
class SpatialRessection;
class Point;
class SRUserInterface;
class EFotoManager;

/**
* \file SRManager.h
* \class SRManager
* \brief Classe de controle de Orientação Exterior.
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class SRManager {

public:

    // Constructors and Destructors
    //
    /**
    * \brief Construtor vazio.
    */
    SRManager();
    /**
    * \brief Construtor padrão.
    * \param manager manager Ponteiro para o controlador central da aplicação.
    * \param myTerrain Ponteiro para o objeto terreno utilizado.
    * \param mySensor Ponteiro para o objeto sensor utilizado.
    * \param myFlight Ponteiro para o objeto voo utilizado.
    * \param myImage Ponteiro para o objeto imagem utilizado.
    * \param myIO Ponteiro para os parâmetros de orientação interior.
    * \param mySR Ponteiro para o objeto de cálculo utilizado.
    */
    explicit SRManager(EFotoManager* manager_, Terrain* myTerrain_, Sensor* mySensor_,
                       Flight* myFlight_, Image* myImage_, InteriorOrientation* myIO_,
                       SpatialRessection* mySR_, std::deque<Point*> myPoints_);
    /**
    * \brief Destrutor padrão.
    */
    ~SRManager();

    // Association Methods
    /**
    * \brief Método que retorna a Interface que está sendo utilizada pelo usuário.
    * \return SRUserInterface Interface que está sendo utilizada pelo usuário.
    */
    SRUserInterface* getInterface();

    // Other Methods
    //
    /**
    * \brief Método que informa se o ponto informado está dentro das dimensões da imagem.
    * \param id Identificador do ponto.
    * \param col Valor da quantidade de colunas da imagem.
    * \param lin Valor da quantidade de linhas da imagem.
    * \return bool Retorna verdadeiro se o ponto está dentro das dimensões da imagem. Retorna falso, caso contrário.
    */
    bool measurePoint(int id, double col, double lin);
    /**
    * \brief Método que seleciona um ponto para a Orientação Exterior.
    * \param id Identificador do ponto.
    */
    void selectPoint(int id);
    /**
    * \brief Método que remove um ponto da seleção de pontos para a Orientação Exterior.
    * \param id Identificador do ponto.
    */
    void unselectPoint(int id);
    /**
    * \brief Método que converte coordenadas da imagem para as coordenadas do detector
    * \param col Valor da quantidade de colunas da imagem.
    * \param lin Valor da quantidade de linhas da imagem.
    * \return deque<double> Deque com as coordenadas do detector.
    */
    std::deque<double> pointToDetector(double col, double lin);
    /**
    * \brief Método que retira a seleção do ponto da imagem.
    * \param id Identificador do ponto.
    * \return bool Retorna verdadeiro se a remoção foi efetuada com sucesso. Retorna falso, caso contrário.
    */
    bool removePoint(int id);

    /**
    * \brief Método que armazena num deque os pontos da imagem selecionados.
    * \return deque<string> Deque dos pontos selecionados da imagem.
    */
    std::deque<std::string> listSelectedPoints();

    /**
    * \brief Método que armazena num deque todos os pontos do projeto.
    * \return deque<string> Deque dos todos os pontos do projeto.
    */
    std::deque<std::string> listAllPoints();
    /**
    * \brief Método que armazena num deque as informações do ponto.
    * \param index Posição do ponto no deque de pontos.
    * \return deque<string> Deque com informações do ponto.
    */
    std::deque<std::string> pointData(int index);
    /**
    * \brief Método que conta quantos pontos da imagem foram selecionados.
    * \return unsigned int Valor da quantidade de pontos selecionados da imagem.
    */
    unsigned int countSelectedPoints() const;
    /**
    * \brief Método executado antes do módulo de Orientação Exterior iniciar para que os pontos do projeto que estão na imagem sejam marcados corretamente.
    * \return bool Retorna verdadeiro se as conexões forem feitas com sucesso.
    */
    bool connectImagePoints();
    /**
    * \brief Método que informa se o módulo já foi iniciado
    * \return bool Retorna verdadeiro se o módulo já foi iniciado
    */
    bool updatePointsTable() const;
    /**
    * \brief Método que informa se as marcas fiduciais  para a direção de vôo estão disponíveis.
    * \param markId Identificador da marca fiducial.
    * \return bool Retorna verdadeiro caso a marca fiducial esteja disponível. Retorna falso, caso contrário.
    */
    bool flightDirection(int markId);
    /**
    * \brief Método que informa se as marcas fiduciais  para a direção de vôo estão disponíveis.
    * \param col Coluna da marca fiducial.
    * \param linha Linha da marca fiducial.
    * \return bool Retorna verdadeiro caso a marca fiducial esteja disponível. Retorna falso, caso contrário.
    */
    bool flightDirection(double col, double lin);
    /**
    * \brief Método que informa se as marcas fiduciais  para a direção de vôo estão disponíveis.
    * \param kappa0 Valor inicial de direção de voo.
    * \return bool Retorna verdadeiro caso a marca fiducial esteja disponível. Retorna falso, caso contrário.
    */
    bool flightDirection(double kappa0);
    /**
    * \brief Método que realiza o cálculo de Orientação Exterior que só é realizada caso tenha mais de 3 pontos selecionados.
    * \param iterations Limite de iterações.
    * \param gnssPrecision Precisão necessária para os parâmetros métricos.
    * \param insPrecision Precisão necessária para os parâmetros angulares.
    * \return bool Retorna verdadeiro se o cálculo for executádo com sucesso.
    */
    bool calculateSR(int iterations, double gnssPrecision, double insPrecision);
    /**
    * \brief Método que diz se a Orientação Exterior está pronta.
    * \return bool Retorna verdadeiro se a Orientação Exterior está pronta. Retorna falso, caso contrário.
    */
    bool exteriorDone() const;

    /**DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS */
    /**Xa La sigma0squared V SigmaXa SigmaLa Xa La sigma0squared V SigmaXa SigmaLa Xa La sigma0squared V SigmaXa SigmaLa Xa La sigma0squared V SigmaXa SigmaLa Xa La sigma0squared V SigmaXa SigmaLa */
    /**DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS */
    /**
    * \brief Método que armazena num deque os valores de Xa, La, sigma0squared, V, SigmaXa, SigmaLa para exibir o relatório na interface gráfica.
    * \return deque<string> .
    */
    std::deque<std::string> makeReport();

    /**
    * \brief Método que inicia o módulo de Orientação Exterior e informa se foi feito algum cálculo neste mesmo módulo anteriormente para a alertar o usuário, caso o projeto não tenha sido salvo.
    * \return bool Retorna verdadeiro se a execução é iniciada com sucesso.
    */
    bool exec();

    /**
    * \brief Método que retorna o identificador de uma imagem
    * \return int Identificador da imagem.
    */
    int getId() const;
    /**
    * \brief Método que carrega a interface do projeto quando o modulo de Orientação Exterior é finalizado.
    */
    void returnProject();
    /**
    * \brief Método que informa se o projeto foi salvo com sucesso.
    * \param path Caminho do arquivo do projeto.
    * \return bool Retorna verdadeiro caso o projeto foi salvo com sucesso. Retorna falso, caso contrário.
    */
    bool save(std::string path);
    /**
    * \brief Método que informa se o projeto foi carregado com sucesso.
    * \param path Caminho do arquivo do projeto.
    * \return bool Retorna verdadeiro caso o projeto foi carregado com sucesso. Retorna falso, caso contrário.
    */
    bool load(std::string path);
    /**
    * \brief Método que retorna o caminho com o nome do arquivo da imagem.
    * \return string Caminho com o nome do arquivo da imagem.
    */
    std::string getImageFile() const;
    /**
    * \brief Método que salva os dados da Orientação Exterior no XML.
    */
    void acceptSR();

private:
    EFotoManager* manager_;
    Terrain* myTerrain_;
    Sensor* mySensor_;
    Flight* myFlight_;
    Image* myImage_;
    InteriorOrientation* myIO_;
    SpatialRessection* mySR_;
    std::deque<Point*> myPoints_;
    SRUserInterface* myInterface_;
    bool started_;
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //SRMANAGER_H
