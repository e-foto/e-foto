/**************************************************************************
		  IOManager.h
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

#ifndef IOMANAGER_H
#define IOMANAGER_H

#include "PositionMatrix.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Sensor;
class Image;
class InteriorOrientation;
class IOUserInterface;
class EFotoManager;

/**
* \file IOManager.h
* \class IOManager
* \brief Classe de controle de Orientação Interior.
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class IOManager
{
	// Private Attributes
	//
	bool started;
	bool status;
	EFotoManager* manager;
	Sensor* mySensor;
	Image* myImage;
	InteriorOrientation* myIO;
	IOUserInterface* myInterface;

public:

	// Constructors and Destructors
	//
	/**
	* \brief Construtor vazio.
	*/
	IOManager();
	/**
	* \brief Construtor padrão.
	* \param manager Ponteiro para o controlador central da aplicação.
	* \param mySensor Ponteiro para o objeto sensor utilizado.
	* \param myImage Ponteiro para o objeto imagem utilizado.
	* \param myIO Ponteiro para o objeto de cálculo utilizado.
	*/
    explicit IOManager(EFotoManager* manager, Sensor* mySensor, Image* myImage, InteriorOrientation* myIO);
	/**
	* \brief Destrutor padrão.
	*/
	~IOManager();

	// Association Methods
	//
	/**
	* \brief Método de associação que seta uma interface.
	* \param newInterface Interface que será usada.
	*/
	void setInterface(IOUserInterface* newInterface);
	/**
	* \brief Método que retorna a Interface corrente.
	* \return IOUserInterface Interface corrente.
	*/
	IOUserInterface* getInterface();

	// Other Methods
	//
	/**
	* \brief Método que retorna a posição da matriz das marcas fiduciais analógicas.
	* \return PositionMatrix Posição da Matriz das Marcas Fiduciais Analógicas.
	*/
	PositionMatrix getAnalogMarks();
	/**
	* \brief Método que substitui uma marca fiducial digital com novos dados.
	* \param id Identificador da Marca Fiducial.
	* \param col Valor da Coluna.
	* \param lin Valor da Linha.
	* \return bool Retorna verdadeiro caso a substituição foi feita corretamente. Retorna falso, caso contrário.
	*/
	bool measureMark(int id, double col, double lin);
	/**
	* \brief Método que retorna o número de marcas fiduciais digitais.
	* \return unsigned int Valor da quantidade de marcas fiduciais digitais.
	*/
	unsigned int countMarks();
	/**
	* \brief Método que armazena num deque as posições de marcas fiduciais analógicas e também de alguma marca fiducial digital.
	* \param index Posição do deque da marca fiducial.
    * \return deque<string> Deque de marcas fiduciais.
	*/
    std::deque<std::string> markData(int index);
	/**
	* \brief Método que retorna o total de marcas fiduciais analógicas.
	* \return unsigned int O valor da quantidade de marcas fiduciais analógicas.
	*/
	unsigned int getTotalMarks();
	/**
	* \brief Método que retorna o modo de cálculo que será usado dependendo se estiver com marcas fiduciais, dimensão dos sensores ou com parâmetros fixos.
	* \return int O modo de cálculo que será utilizado.
	*/
	int getCalculationMode();
	/**
	* \brief Método que diz se é possível realizar o cálculo de Orientação Interior na imagem.
	* \return bool Retorna verdadeiro se é possível realizar o cálculo. Retorna falso, caso contrário.
	*/
	bool calculateIO();
	/**
	* \brief Método que diz se a Orientação Interior está pronta.
	* \return bool Retorna verdadeiro se a Orientação Interior está pronta. Retorna falso, caso contrário.
	*/
	bool interiorDone();
	/**DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS */
	/**Xa La sigma0squared V SigmaXa SigmaLa Xa La sigma0squared V SigmaXa SigmaLa Xa La sigma0squared V SigmaXa SigmaLa Xa La sigma0squared V SigmaXa SigmaLa Xa La sigma0squared V SigmaXa SigmaLa */
	/**DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS */
	/**
	* \brief Método que armazena num deque os valores de Xa, La, sigma0squared, V, SigmaXa e SigmaLa vindos de um xml para exibir o relatório na interface gráfica.
    * \return deque<string> Os valores de Xa, La, sigma0squared, V, SigmaXa e SigmaLa.
	*/
    std::deque<std::string> makeReport();
	/**
	* \brief Método que inicia o módulo de Orientação Interior e retorna se foi feito algum cálculo neste mesmo módulo anteriormente para a alertar o usuário, caso o projeto não tenha sido salvo.
	* \return bool Retorna verdadeiro se o projeto foi salvo. Retorna falso, caso contrário.
	*/
	bool exec();
	/**
	* \brief Método que retorna o identificador da imagem utilizada na Orientação Interior.
	* \return int Identificador da Imagem.
	*/
	int getId();
	/**
	* \brief Método que carrega a interface do projeto quando o módulo de Orientação Interior é finalizado.
	*/
	void returnProject();
	/**
	* \brief Método que salva o projeto e informa se o mesmo foi salvo com sucesso.
    *
	* \param path Caminho do projeto.
	* \return bool Retorna verdadeiro se foi salvo com sucesso. Retorna falso, caso contrário.
	*/
    bool save(std::string path);
	/**
	* \brief Método que carrega o projeto e informa se o mesmo foi salvo com sucesso.
	* \param path Caminho do projeto.
	* \return bool Retorna verdadeiro se foi salvo com sucesso. Retorna falso, caso contrário.
	*/
    bool load(std::string path);
	/**
	* \brief Método que retorna o caminho da imagem com o nome do arquivo.
    * \return string Caminho da imagem e o nome do arquivo.
	*/
    std::string getImageFile();
	/**
	* \brief Método que retorna o número de linhas quando o sensor conhece as dimensões da imagem.
	* \return int Número de linhas.
	*/
	int getFrameRows();
	/**
	* \brief Método que retorna o número de colunas quando o sensor conhece as dimensões da imagem.
	* \return int Número de colunas.
	*/
	int getFrameColumns();
	/**
	* \brief Método que salva os dados da Orientação Interior no XML.
	*/
	void acceptIO();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //IOMANAGER_H
