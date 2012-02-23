#ifndef IMAGESPACECOORDINATE_H
#define IMAGESPACECOORDINATE_H

#include "AbstractImageCoordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
* \file ImageSpaceCoordinate.h
* \class ImageSpaceCoordinate
* \brief Classes de coordenada digital sobre o espaço imagem (espaço discreto 2D).
* \todo Em breve poderão surgir novas classes denominadas DiscreetDigitalCoordinate para manter extamente a implementação desta interface e SubPixelatedDigitalCoordinate para extender esta classe em ocasiões onde for importante a medição em subpixels.
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class ImageSpaceCoordinate : public AbstractImageCoordinate
{

private:
	int imageId;

public:

	/**
 * \brief Construtor vazio.
 */
	ImageSpaceCoordinate();

	/**
 * \brief Construtor que cria uma coordenada digital identificada sobre o espaço imagem.
 * \param myImageId	Identificador da imagem à qual a coordenada pertence.
 */
	ImageSpaceCoordinate(int myImageId);

	/**
 * \brief Construtor que cria uma coordenada digital identificada sobre o espaço imagem com todos os seus valores.
 * \param myImageId	Identificador da imagem à qual a coordenada pertence.
 * \param myCol	Valor da coluna.
 * \param myLin	Valor da linha.
 */
	ImageSpaceCoordinate(int myImageId, double myCol, double myLin);

	/**
 * \brief Construtor que cria uma coordenada digital identificada sobre o espaço imagem com todos os seus valores usando matrizes.
 * \param myImageId	Identificador da imagem à qual a coordenada pertence.
 * \param myPositio	Matriz coluna com os valores de posicionamento da coordenada.
 */
	ImageSpaceCoordinate(int myImageId, const PositionMatrix& myPosition);

	/**
 * \brief Destrutor padrão.
 */
	~ImageSpaceCoordinate();

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
 * \return bool	Retorna verdadeiro caso o nome passado seja ImageSpaceCoordinate. Retorna falso no caso contrário.
 */
	bool is(string s);

	/**
 * \brief Método para setar os valores de atributos de uma instância utilizando sua descrição em xml.
 * \param xml	String contendo o xml com todos os valores de atributos adequados a uma instância da classe ImageSpaceCoordinate.
 */
	void xmlSetData(string xml);

	/**
 * \brief Método para extrair o equivalente em dados xml de uma instância.
 * \return string	Retorna o string contendo o xml para uma instância da classe ImageSpaceCoordinate.
 */
	string xmlGetData();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // IMAGESPACECOORDINATE_H
