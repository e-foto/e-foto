#ifndef POSITIONMATRIX_H
#define POSITIONMATRIX_H

#include "RectSupport.h"
#include "Matrix.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h>

using namespace std;

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Matrix;

/**
* \file PositionMatrix.h
* \class PositionMatrix
* \brief Classe para descrição de coordenadas em forma matricial (matriz coluna).
* \copyright E-Foto group
* \authors Rafael Aguiar, Irving Badolato & Sarina Lustosa.
*/
class PositionMatrix : public RectSupport
{

private:
	double* _Mat;
	string unit;

protected:

	/**
 * \brief Método interno para liberar a memória.
 */
	void del();

	/**
 * \brief Método interno para alocar a memória e preencher o espaço com zeros.
 * \param rows	Número de linhas a ser alocado.
 * \param cols	Número de colunas a ser alocado.
 */
	void nw(const unsigned int rows, const unsigned int cols);

public:

	friend class Matrix;

	/**
 * \brief Construtor padrão.
 * \param dimensions	Define qual a dimensão utilizada para a coordenada a ser armazenada pela matriz de posição.
 * \param newunit	define qual o descritor textual da unidade de medida utilizada.
 */
	PositionMatrix(unsigned int dimensions, string newUnit);

	/**
 * \brief Construtor de cópia para matrizes de cálculos.
 * \param anotherMatrix	Matriz a ser copiada.
 */
	PositionMatrix(const Matrix& anotherMatrix);

	/**
 * \brief Construtor de cópia para matrizes de posição.
 * \param anotherMatrix	Matriz a ser copiada.
 */
	PositionMatrix(const PositionMatrix& anotherMatrix);

	/**
 * \brief Construtor vazio padrão.
 */
	PositionMatrix();

	/**
 * \brief Destrutor padrão.
 */
	~PositionMatrix();

	/**
  * \brief Método para alterar as dimensões da matriz.
  * \param rows	Nova quantidade de colunas.
  * \param cols	Nova quantidade de linhas.
  * \return PositionMatrix	Referência para a matriz redimensionada.
  */
	PositionMatrix& resize(unsigned int rows, unsigned int cols = 1);

	/**
  * \brief Método para zerar todo o conteúdo da matriz.
  * \return PositionMatrix	Referência para a matriz como as células zeradas.
  */
	PositionMatrix& zero();

	/**
  * \brief Método para carregar a matrix de um formato ascii.
  * \deprecated Este método não possui uso ou deve ser evitado o seu uso, pois ele será removido em versões futuras.
  * \attention No presente momento matrizes não são salvas isoladamente.
  * \attention  Elas só são salvas no contexto em que estão inseridas em formato xml (mml ou gml) no arquivo .epp. Por tal motivo foi feita a sugestão de remoção deste método.
  * \param filename	Caminho e nome do arquivo a ser salvo.
  * \return int	1 em caso de sucesso e 0 em caso de falha.
  */
	int load(char* filename);

	/**
  * \brief Método para salvar a matrix sobre um formato ascii.
  * \deprecated Este método não possui uso ou deve ser evitado o seu uso, pois ele será removido em versões futuras.
  * \attention No presente momento matrizes não são salvas isoladamente.
  * \attention  Elas só são salvas no contexto em que estão inseridas em formato xml (mml ou gml) no arquivo .epp. Por tal motivo foi feita a sugestão de remoção deste método.
  * \param filename	Caminho e nome do arquivo a ser salvo.
  * \return int	1 em caso de sucesso e 0 em caso de falha.
  */
	int save(char* filename);

	/**
 * \brief Método para retornar o valor da linha e coluna (i,j).
 * \param i	Identificador da linha consultada.
 * \param j	Identificador da coluna consultada.
 * \param value	Valor da célula (i,j).
 */
	double get(const unsigned int i, const unsigned int j = 1) const;

	/**
 * \brief Método para retornar o valor (convertido em inteir) da linha e coluna (i,j).
 * \param i	Identificador da linha consultada.
 * \param j	Identificador da coluna consultada.
 * \param value	Valor da célula (i,j).
 */
	int getInt(const unsigned int i, const unsigned int j = 1) const;

	/**
 * \brief Método para retornar a unidade considerada para a matriz.
 * \return string	Texto descritor da unidade de medidas considerada para a matriz.
 */
	string getUnit() const;

	/**
  * \brief Método para retornar uma selecão do conteúdo da matriz.
  * \param FirstRow	Primeira linha da seleção.
  * \param LastRow	Última linha da seleção.
  * \param FirstCol	Primeira coluna da seleção.
  * \param LastCol	Ultima coluna da seleção.
  * \return PositionMatrix	A sub matriz mxn denotada por m = (LastRow - FirstRow) e n = (LastCol - FirstCol).
  */
	PositionMatrix sel(const unsigned int FirstRow, const unsigned int LastRow,
					   const unsigned int FirstCol = 1, const unsigned int LastCol = 1);

	/**
 * \brief Método para exibição em terminal dos valores contidos na matriz.
 */
	void show();

	/**
 * \brief Método para alterar o valor da linha e coluna (i,j).
 * \param i	Identificador da linha alterada.
 * \param j	Identificador da coluna alterada.
 * \param value	Novo valor para (i,j).
 */
	void set(unsigned int i, unsigned int j, double value) const;

	/**
 * \brief Método para alterar o primeiro valor da linha i (i,1).
 * \param i	Identificador da linha alterada.
 * \param value	Novo valor para (i,1).
 */
	void set(unsigned int i, double value) const;

	/**
 * \brief Método para alterar (com um inteiro) o valor da linha e coluna (i,j).
 * \param i	Identificador da linha alterada.
 * \param j	Identificador da coluna alterada.
 * \param value	Novo valor para (i,j).
 */
	void setInt(unsigned int i, unsigned int j, int value) const;

	/**
 * \brief Método para alterar (com um inteiro) o primeiro valor da linha i (i,1).
 * \param i	Identificador da linha alterada.
 * \param value	Novo valor para (i,1).
 */
	void setInt(unsigned int i, int value) const;

	/**
 * \brief Método para alterar a unidade considerada para a matriz.
 * \param newUnit	Texto descritor da nova unidade de medidas considerada para a matriz.
 */
	void setUnit(string newUnit);

	/**
 * \brief Operador para a concatenação horizontal de duas matrizes de posição.
 * \param Par_Matrix	Matriz para concatenação.
 * \return PositionMatrix	Retorna a matriz concatenada.
 */
	PositionMatrix operator &(const PositionMatrix& Par_Matrix);

	/**
 * \brief Operador para a concatenação vertical de duas matrizes de posição.
 * \param Par_Matrix	Matriz para concatenação.
 * \return PositionMatrix	Retorna a matriz concatenada.
 */
	PositionMatrix operator |(const PositionMatrix& Par_Matrix);

	/**
 * \brief Operador de cópia para a matriz de posição utilizando uma matriz para cálculos.
 * \param Par_Matrix	Matriz para comparação.
 * \return PositionMatrix	Retorna referência para a matriz de posição copiada.
 */
	PositionMatrix& operator =(const Matrix& Par_Matrix);

	/**
 * \brief Operador de cópia para a matriz de posição utilizando outra matriz de posição.
 * \param Par_Matrix	Matriz para cópia.
 * \return PositionMatrix	Retorna referência para a matriz de posição copiada.
 */
	PositionMatrix& operator =(const PositionMatrix& Par_Matrix);

	/**
 * \brief Operador para testar a igualdade entre a matriz de posição e outra matriz.
 * \attention Este método não leva em consideração informação textual sobre as unidades de medida informadas.
 * \param Par_Matrix	Matriz para comparação.
 * \return bool	Retorna verdadeiro se não existir qualquer diferença numérica ou na dimensão das matrizes. Retora falso em caso contrário.
 */
	bool operator ==(const Matrix& Par_Matrix);

	/**
 * \brief Operador para testar a igualdade entre a matriz de posição e outra matriz de posição.
 * \attention Este método não leva em consideração informação textual sobre as unidades de medida informadas.
 * \param Par_Matrix	Matriz para comparação.
 * \return bool	Retorna verdadeiro se não existir qualquer diferença numérica ou na dimensão das matrizes. Retora falso em caso contrário.
 */
	bool operator ==(const PositionMatrix& Par_Matrix);

	/**
 * \brief Operador para testar a diferença entre a matriz de posição e outra matriz.
 * \attention Este método não leva em consideração informação textual sobre as unidades de medida informadas.
 * \param Par_Matrix	Matriz para comparação.
 * \return bool	Retorna verdadeiro se existir alguma diferença numérica ou na dimensão das matrizes. Retora falso em caso contrário.
 */
	bool operator !=(const Matrix& Par_Matrix);

	/**
 * \brief Operador para testar a diferença entre a matriz de posição e outra matriz de posição.
 * \attention Este método não leva em consideração informação textual sobre as unidades de medida informadas.
 * \param Par_Matrix	Matriz para comparação.
 * \return bool	Retorna verdadeiro se existir alguma diferença numérica ou na dimensão das matrizes. Retora falso em caso contrário.
 */
	bool operator !=(const PositionMatrix& Par_Matrix);

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
 * \return bool	Retorna verdadeiro caso o nome passado seja PositionMatrix. Retorna falso no caso contrário.
 */
	bool is(string s);

	/**
 * \brief Método para extrair o equivalente em dados xml (gml:pos) da matriz de posição.
 * \return string	Retorna o string contendo o xml (gml:pos) da matriz de posição.
 */
	string xmlGetData();

	/**
 * \brief Método para setar os valores de atributos da matriz de posição utilizando sua descrição em xml (gml:pos).
 * \param xml	String contendo o xml (gml:pos) da matriz de posição.
 */
	void xmlSetData(string xml);

	/**
 * \brief Método para retornar o conteúdo da matriz coluna realocado sobre a diagonal principal de uma matriz quadrada.
 * \return PositionMatrix	Coordenada sobre a diagonal principal da matriz.
 */
	PositionMatrix toDiagonal();

	/**
 * \brief Método para retornar o conteúdo da matriz em formato GML (Geography Markup Language).
 * \return string	Coordenada em formato GML.
 */
	string toGmlPosFormat();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //POSITIONMATRIX_H
