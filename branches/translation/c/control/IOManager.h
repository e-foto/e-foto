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
* \brief Classe de controle de Orienta��o Interior.
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
	* \brief Construtor padr�o.
	* \param manager Ponteiro para o controlador central da aplica��o.
	* \param mySensor Ponteiro para o objeto sensor utilizado.
	* \param myImage Ponteiro para o objeto imagem utilizado.
	* \param myIO Ponteiro para o objeto de c�lculo utilizado.
	*/
    explicit IOManager(EFotoManager* manager, Sensor* mySensor, Image* myImage, InteriorOrientation* myIO);
	/**
	* \brief Destrutor padr�o.
	*/
	~IOManager();

	// Association Methods
	//
	/**
	* \brief M�todo de associa��o que seta uma interface.
	* \param newInterface Interface que ser� usada.
	*/
	void setInterface(IOUserInterface* newInterface);
	/**
	* \brief M�todo que retorna a Interface corrente.
	* \return IOUserInterface Interface corrente.
	*/
	IOUserInterface* getInterface();

	// Other Methods
	//
	/**
	* \brief M�todo que retorna a posi��o da matriz das marcas fiduciais anal�gicas.
	* \return PositionMatrix Posi��o da Matriz das Marcas Fiduciais Anal�gicas.
	*/
	PositionMatrix getAnalogMarks();
	/**
	* \brief M�todo que substitui uma marca fiducial digital com novos dados.
	* \param id Identificador da Marca Fiducial.
	* \param col Valor da Coluna.
	* \param lin Valor da Linha.
	* \return bool Retorna verdadeiro caso a substitui��o foi feita corretamente. Retorna falso, caso contr�rio.
	*/
	bool measureMark(int id, double col, double lin);
	/**
	* \brief M�todo que retorna o n�mero de marcas fiduciais digitais.
	* \return unsigned int Valor da quantidade de marcas fiduciais digitais.
	*/
	unsigned int countMarks();
	/**
	* \brief M�todo que armazena num deque as posi��es de marcas fiduciais anal�gicas e tamb�m de alguma marca fiducial digital.
	* \param index Posi��o do deque da marca fiducial.
    * \return deque<string> Deque de marcas fiduciais.
	*/
    std::deque<std::string> markData(int index);
	/**
	* \brief M�todo que retorna o total de marcas fiduciais anal�gicas.
	* \return unsigned int O valor da quantidade de marcas fiduciais anal�gicas.
	*/
	unsigned int getTotalMarks();
	/**
	* \brief M�todo que retorna o modo de c�lculo que ser� usado dependendo se estiver com marcas fiduciais, dimens�o dos sensores ou com par�metros fixos.
	* \return int O modo de c�lculo que ser� utilizado.
	*/
	int getCalculationMode();
	/**
	* \brief M�todo que diz se � poss�vel realizar o c�lculo de Orienta��o Interior na imagem.
	* \return bool Retorna verdadeiro se � poss�vel realizar o c�lculo. Retorna falso, caso contr�rio.
	*/
	bool calculateIO();
	/**
	* \brief M�todo que diz se a Orienta��o Interior est� pronta.
	* \return bool Retorna verdadeiro se a Orienta��o Interior est� pronta. Retorna falso, caso contr�rio.
	*/
	bool interiorDone();
	/**DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS */
	/**Xa La sigma0squared V SigmaXa SigmaLa Xa La sigma0squared V SigmaXa SigmaLa Xa La sigma0squared V SigmaXa SigmaLa Xa La sigma0squared V SigmaXa SigmaLa Xa La sigma0squared V SigmaXa SigmaLa */
	/**DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS */
	/**
	* \brief M�todo que armazena num deque os valores de Xa, La, sigma0squared, V, SigmaXa e SigmaLa vindos de um xml para exibir o relat�rio na interface gr�fica.
    * \return deque<string> Os valores de Xa, La, sigma0squared, V, SigmaXa e SigmaLa.
	*/
    std::deque<std::string> makeReport();
	/**
	* \brief M�todo que inicia o m�dulo de Orienta��o Interior e retorna se foi feito algum c�lculo neste mesmo m�dulo anteriormente para a alertar o usu�rio, caso o projeto n�o tenha sido salvo.
	* \return bool Retorna verdadeiro se o projeto foi salvo. Retorna falso, caso contr�rio.
	*/
	bool exec();
	/**
	* \brief M�todo que retorna o identificador da imagem utilizada na Orienta��o Interior.
	* \return int Identificador da Imagem.
	*/
	int getId();
	/**
	* \brief M�todo que carrega a interface do projeto quando o m�dulo de Orienta��o Interior � finalizado.
	*/
	void returnProject();
	/**
	* \brief M�todo que salva o projeto e informa se o mesmo foi salvo com sucesso.
    *
	* \param path Caminho do projeto.
	* \return bool Retorna verdadeiro se foi salvo com sucesso. Retorna falso, caso contr�rio.
	*/
    bool save(std::string path);
	/**
	* \brief M�todo que carrega o projeto e informa se o mesmo foi salvo com sucesso.
	* \param path Caminho do projeto.
	* \return bool Retorna verdadeiro se foi salvo com sucesso. Retorna falso, caso contr�rio.
	*/
    bool load(std::string path);
	/**
	* \brief M�todo que retorna o caminho da imagem com o nome do arquivo.
    * \return string Caminho da imagem e o nome do arquivo.
	*/
    std::string getImageFile();
	/**
	* \brief M�todo que retorna o n�mero de linhas quando o sensor conhece as dimens�es da imagem.
	* \return int N�mero de linhas.
	*/
	int getFrameRows();
	/**
	* \brief M�todo que retorna o n�mero de colunas quando o sensor conhece as dimens�es da imagem.
	* \return int N�mero de colunas.
	*/
	int getFrameColumns();
	/**
	* \brief M�todo que salva os dados da Orienta��o Interior no XML.
	*/
	void acceptIO();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //IOMANAGER_H
