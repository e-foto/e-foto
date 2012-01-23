#ifndef DIGITALIMAGESPACECOORDINATE_H
#define DIGITALIMAGESPACECOORDINATE_H

#include "AbstractDigitalCoordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
* \file DigitalImageSpaceCoordinate.h
* \class DigitalImageSpaceCoordinate
* \brief Classes de coordenada digital sobre o espaço imagem (espaço discreto 2D).
* \todo Em breve poderão surgir novas classes denominadas DiscreetDigitalCoordinate para manter extamente a implementação desta interface e SubPixelatedDigitalCoordinate para extender esta classe em ocasiões onde for importante a medição em subpixels.
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class DigitalImageSpaceCoordinate : public AbstractDigitalCoordinate
{

private:
	int imageId;

public:

	/**
 * \brief Construtor vazio.
 */
	DigitalImageSpaceCoordinate();

	/**
 * \brief Construtor que cria uma coordenada digital identificada sobre o espaço imagem.
 * \param myImageId	Identificador da imagem à qual a coordenada pertence.
 */
	DigitalImageSpaceCoordinate(int myImageId);

	/**
 * \brief Construtor que cria uma coordenada digital identificada sobre o espaço imagem com todos os seus valores.
 * \param myImageId	Identificador da imagem à qual a coordenada pertence.
 * \param myCol	Valor da coluna.
 * \param myLin	Valor da linha.
 */
	DigitalImageSpaceCoordinate(int myImageId, int myCol, int myLin);

	/**
 * \brief Construtor que cria uma coordenada digital identificada sobre o espaço imagem com todos os seus valores usando matrizes.
 * \param myImageId	Identificador da imagem à qual a coordenada pertence.
 * \param myPositio	Matriz coluna com os valores de posicionamento da coordenada.
 */
	DigitalImageSpaceCoordinate(int myImageId, const PositionMatrix& myPosition);

	/**
 * \brief Destrutor padrão.
 */
	~DigitalImageSpaceCoordinate();

	/**
 * \brief Método de acesso que seta o valor de id da imagem à qual a coordenada pertence.
 * \param newSensorId	Novo identificador da imagem à qual a coordenada pertence.
 */
	void setImageId(int newImageId);

	/**
 * \brief Método de acesso que retorna o valor de id da imagem à qual a coordenada pertence.
 * \return int	int	Identificador da imagem à qual a coordenada pertence.
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
 * \return bool	Retorna verdadeiro caso o nome passado seja DigitalImageSpaceCoordinate. Retorna falso no caso contrário.
 */
	bool is(string s);

	/**
 * \brief Método para setar os valores de atributos de uma instância utilizando sua descrição em xml.
 * \param xml	String contendo o xml com todos os valores de atributos adequados a uma instância da classe DigitalImageSpaceCoordinate.
 */
	void xmlSetData(string xml);

	/**
 * \brief Método para extrair o equivalente em dados xml de uma instância.
 * \return string	Retorna o string contendo o xml para uma instância da classe DigitalImageSpaceCoordinate.
 */
	string xmlGetData();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // DIGITALIMAGESPACECOORDINATE_H
