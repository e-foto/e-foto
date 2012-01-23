#ifndef DIGITALFIDUCTIALMARK_H
#define DIGITALFIDUCTIALMARK_H

#include "AbstractDigitalCoordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
* \file DigitalFiductialMark.h
* \class DigitalFiductialMark
* \brief Classes de marca fiducial digital. É a referência impressa sobre uma imagem por um sensor métrico.
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class DigitalFiductialMark : public AbstractDigitalCoordinate
{

private:
	int id;
	int imageId;

public:

	/**
 * \brief Construtor vazio.
 */
	DigitalFiductialMark();

	/**
 * \brief Construtor que cria uma marca fiducial digital com seus identificadores.
 * \param myId	Identificador da marca fiducial digital.
 * \param myImageId	Identificador da imagem à qual a marca pertence.
 */
	DigitalFiductialMark(int myId, int myImageId);

	/**
 * \brief Construtor que cria uma marca fiducial digital com todos os seus valores.
 * \param myId	Identificador da marca fiducial digital.
 * \param myImageId	Identificador da imagem à qual a marca pertence.
 * \param myCol	Valor da coluna.
 * \param myLin	Valor da linha.
 */
	DigitalFiductialMark(int myId, int myImageId, int myCol, int myLin);

	/**
 * \brief Construtor que cria uma marca fiducial digital com todos os seus valores usando matrizes.
 * \param myId	Identificador da marca fiducial digital.
 * \param myImageId	Identificador da imagem à qual a marca pertence.
 * \param myPositio	Matriz coluna com os valores de posicionamento da marca.
 */
	DigitalFiductialMark(int myId, int myImageId, const PositionMatrix& myPosition);

	/**
 * \brief Destrutor padrão.
 */
	~DigitalFiductialMark();

	/**
 * \brief Método de acesso que seta o valor de id da marca fiducial digital.
 * \param newId	Novo identificador da marca fiducial digital.
 */
	void setId(int newId);

	/**
 * \brief Método de acesso que seta o valor de id da imagem à qual a marca pertence.
 * \param newSensorId	Novo identificador da imagem à qual a marca pertence.
 */
	void setImageId(int newImageId);

	/**
 * \brief Método de acesso que retorna o valor de id da marca fiducial digital.
 * \return int	Identificador da marca fiducial digital.
 */
	int getId();

	/**
 * \brief Método de acesso que retorna o valor de id da imagem à qual a marca pertence.
 * \return int	int	Identificador da imagem à qual a marca pertence.
 */
	int getImageId();

	/**
 * \brief Método para emitir o nome de classe.
 * \return string	Retorna o nome de classe do objeto.
 */
	string objectType(void);

	/**
 * \brief Método para emitir as associações de uma instância.
 * \return string	Retorna vazio para esta classe.
 * \deprecated Este método não possui uso ou deve ser evitado o seu uso, pois ele será removido em versões futuras.
 */
	string objectAssociations(void);

	/**
 * \brief Método de teste para o nome/tipo de instância.
 * \param s	Texto com o nome da classe que é esperado.
 * \return bool	Retorna verdadeiro caso o nome passado seja DigitalFiductialMark. Retorna falso no caso contrário.
 */
	bool is(string s);

	/**
 * \brief Método para setar os valores de atributos de uma instância utilizando sua descrição em xml.
 * \param xml	String contendo o xml com todos os valores de atributos adequados a uma instância da classe DigitalFiductialMark.
 */
	void xmlSetData(string xml);

	/**
 * \brief Método para extrair o equivalente em dados xml de uma instância.
 * \return string	Retorna o string contendo o xml para uma instância da classe DigitalFiductialMark.
 */
	string xmlGetData();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // DIGITALFIDUCTIALMARK_H
