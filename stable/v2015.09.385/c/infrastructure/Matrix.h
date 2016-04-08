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

#ifndef MATRIX_H
#define MATRIX_H

#ifdef WIN32
#define _USE_MATH_DEFINES // for C++
#include <cmath>
#endif
#ifdef unix
#include <math.h>
#endif

#define MAX(X,Y) (((X)>(Y))?(X):(Y))
#define MIN(X,Y) (((X)<(Y))?(X):(Y))

#include "RectSupport.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class PositionMatrix;

/**
* \file Matrix.h
* \class Matrix
* \brief Classe para cálculos matriciais.
* \attention Este componente foi desenvolvido para atender as necessidades do projeto E-Foto ao lidar com cálculos matriciais e portanto pode não contemplar ainda um grande leque de métodos matriciais.
* \copyright E-Foto group
* \authors Guilherme Abelha, Marcelo Silveira, Rafael Aguiar, Irving Badolato, Sarina Lustosa & Paulo Pupim.
*/
class Matrix : public RectSupport
{

private:
    double* _Mat;
    std::string unit_;

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

    friend class PositionMatrix;

    /**
 * \brief Construtor padrão.
 * \param rows	Define a quantidade de linhas da nova matriz.
 * \param cols	Define a quantidade de colunas da nova matriz.
 */
    Matrix(unsigned int rows, unsigned int cols);

    /**
 * \brief Construtor de cópia para matrizes de posição.
 * \param anotherMatrix	Matriz de posição a ser copiada.
 */
    Matrix(const PositionMatrix& anotherMatrix);

    /**
 * \brief Construtor de cópia para matrizes.
 * \param anotherMatrix	Matriz a ser copiada.
 */
    Matrix(const Matrix& anotherMatrix);

    /**
 * \brief Construtor vazio padrão.
 */
    Matrix();

    /**
 * \brief Destrutor padrão.
 */
    ~Matrix();

    /**
 * \brief Método para alterar as dimensões da matriz.
 * \param rows	Nova quantidade de colunas.
 * \param cols	Nova quantidade de linhas.
 * \return Matrix	Referência para a matriz redimensionada.
 */
    Matrix& resize(unsigned int rows, unsigned int cols);

    /**
  * \brief Método para transformar a matriz numa matriz identidade.
  * \return Matrix	Referência para a matriz transformada em identidade.
  */
    Matrix& identity(unsigned int size);

    /**
  * \brief Método para preencher com o valor 1 o conteúdo de todas as células da matriz.
  * \return Matrix	Referência para a matriz como as células preenchidas com o valor 1.
  */
    Matrix& ones();

    /**
  * \brief Método para zerar todo o conteúdo da matriz.
  * \return Matrix	Referência para a matriz como as células zeradas.
  */
    Matrix& zero();

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
    double get(const unsigned int i, const unsigned int j) const;

    /**
 * \brief Método para retornar o valor (convertido em inteiro) da linha e coluna (i,j).
 * \param i	Identificador da linha consultada.
 * \param j	Identificador da coluna consultada.
 * \param value	Valor da célula (i,j).
 */
    int getInt(const unsigned int i, const unsigned int j) const;

    /**
 * \brief Método para retornar a unidade considerada para a matriz.
 * \return string	Texto descritor da unidade de medidas considerada para a matriz.
 */
    std::string getUnit();

    /**
  * \brief Método para retornar uma selecão do conteúdo da matriz.
  * \param FirstRow	Primeira linha da seleção.
  * \param LastRow	Última linha da seleção.
  * \param FirstCol	Primeira coluna da seleção.
  * \param LastCol	Ultima coluna da seleção.
  * \return PositionMatrix	A sub matriz mxn denotada por m = (LastRow - FirstRow) e n = (LastCol - FirstCol).
  */
    Matrix sel(const unsigned int FirstRow, const unsigned int LastRow,
               const unsigned int FirstCol, const unsigned int LastCol);

    /**
 * \brief Método para exibição em terminal dos valores contidos na matriz.
 * \param mode	Modo de exibição dos valores. Pode ser "e" (notação científica), "f" (notação decimal) ou "g" (a melhor notação para cada valor).
 * \param precision	Precisão em quantidade de números significativos (para notação científica) ou em quantidade de casa decimais (para notação decimal).
 * \param name	Nome a ser dado para a matriz durante sua exibição.
 */
    void show(char mode='f',int precision=3, std::string name = "");

    /**
 * \brief Método para informar o maior valor de uma matriz.
 * \return double	Menor valor contido na matriz.
 */
    double highestValue() const;

    /**
 * \brief Método para informar o menor valor de uma matriz.
 * \return double	Menor valor contido na matriz.
 */
    double lowestValue() const;

    /**
 * \brief Método para alterar o valor da linha e coluna (i,j).
 * \param i	Identificador da linha alterada.
 * \param j	Identificador da coluna alterada.
 * \param value	Novo valor para (i,j).
 */
    void set(unsigned int i, unsigned int j, double value) const;

    /**
 * \brief Método para alterar (com um inteiro) o valor da linha e coluna (i,j).
 * \param i	Identificador da linha alterada.
 * \param j	Identificador da coluna alterada.
 * \param value	Novo valor para (i,j).
 */
    void setInt(unsigned int i, unsigned int j, int value) const;

    /**
 * \brief Método para alterar a unidade considerada para a matriz.
 * \param newUnit	Texto descritor da nova unidade de medidas considerada para a matriz.
 */
    void setUnit(std::string newUnit);

    /**
 * \brief Método para verificar se a matriz é a identidade.
 * \return bool	Verdadeiro se a matriz é igual a identidade. Retorna falso em caso contrário.
 */
    bool isIdentity();

    /**
 * \brief Método para verificar se a matriz é unitriangular superior.
 * \return bool	Verdadeiro se a matriz é unitriangular superior. Retorna falso em caso contrário.
 */
    bool isTriang();

    /**
 * \brief Método para verificar se a matriz só possui valores iguais a zero.
 * \return bool	Verdadeiro se todos os valores da matriz estiverem muito próximos de zero. Retorna falso em caso contrário.
 */
    bool isZeroes();

    /**
 * \brief Operador para a concatenação horizontal de duas matrizes.
 * \param Par_Matrix	Matriz para concatenação.
 * \return PositionMatrix	Retorna a matriz concatenada.
 */
    Matrix operator &(const Matrix& Par_Matrix);

    /**
 * \brief Operador para a concatenação vertical de duas matrizes.
 * \param Par_Matrix	Matriz para concatenação.
 * \return PositionMatrix	Retorna a matriz concatenada.
 */
    Matrix operator |(const Matrix& Par_Matrix);

    /**
 * \brief Operador para soma de matrizes
 * \param Par_matrix	Matriz para a soma.
 * \return Matrix	Matriz somada.
 */
    Matrix operator +(const Matrix& Par_Matrix);

    /**
 * \brief Operador para subtração de matrizes
 * \param Par_matrix	Matriz para a subtração.
 * \return Matrix	Matriz subtraída.
 */
    Matrix operator -(const Matrix& Par_Matrix);

    /**
 * \brief Operador para multiplicação de matrizes
 * \param Par_matrix	Matriz para a multiplicação.
 * \return Matrix	Matriz multiplicada.
 */
    Matrix operator *(const Matrix& Par_Matrix);

    /**
 * \brief Operador para soma com valor escalar.
 * \param Par_double	Valor escalar a soma com a matriz.
 * \return Matrix	Matriz somada com escalar.
 */
    Matrix operator +(double Par_double);

    /**
 * \brief Operador para subtração por valor escalar.
 * \param Par_double	Valor escalar para a subtração da matriz.
 * \return Matrix	Matriz subtraída por escalar.
 */
    Matrix operator -(double Par_double);

    /**
 * \brief Operador para multiplicação por valor escalar.
 * \param Par_double	Valor escalar para multiplicar a matriz.
 * \return Matrix	Matriz multoplicada por escalar.
 */
    Matrix operator *(double Par_double);

    /**
 * \brief Operador para divisão por valor escalar.
 * \param Par_double	Valor escalar dividir a matriz.
 * \return Matrix	Matriz dividida por escalar.
 */
    Matrix operator /(double Par_double);

    /**
 * \brief Operador de cópia para a matriz utilizando uma matriz de posição.
 * \param Par_Matrix	Matriz para cópia.
 * \return Matrix	Retorna referência para a matriz copiada.
 */
    Matrix& operator =(const PositionMatrix& Par_Matrix);

    /**
 * \brief Operador de cópia para matrizes.
 * \param Par_Matrix	Matriz para cópia.
 * \return Matrix	Retorna referência para a matriz copiada.
 */
    Matrix& operator =(const Matrix& Par_Matrix);

    /**
 * \brief Operador para testar a igualdade entre a matriz e uma matriz de posição.
 * \attention Este método não leva em consideração informação textual sobre as unidades de medida informadas.
 * \param Par_Matrix	Matriz de posição para comparação.
 * \return bool	Retorna verdadeiro se não existir nenhuma diferença numérica ou na dimensão das matrizes. Retora falso em caso contrário.
 */
    bool operator ==(const PositionMatrix& Par_Matrix);

    /**
 * \brief Operador para testar a igualdade entre duas a matrizes.
 * \attention Este método não leva em consideração informação textual sobre as unidades de medida informadas.
 * \param Par_Matrix	Matriz para comparação.
 * \return bool	Retorna verdadeiro se não existir nenhuma diferença numérica ou na dimensão das matrizes. Retora falso em caso contrário.
 */
    bool operator ==(const Matrix& Par_Matrix);

    /**
 * \brief Operador para testar a diferença entre a matriz e uma matriz de posição.
 * \attention Este método não leva em consideração informação textual sobre as unidades de medida informadas.
 * \param Par_Matrix	Matriz de posição para comparação.
 * \return bool	Retorna verdadeiro se existir alguma diferença numérica ou na dimensão das matrizes. Retora falso em caso contrário.
 */
    bool operator !=(const PositionMatrix& Par_Matrix);

    /**
 * \brief Operador para testar a diferença entre duas a matrizes.
 * \attention Este método não leva em consideração informação textual sobre as unidades de medida informadas.
 * \param Par_Matrix	Matriz para comparação.
 * \return bool	Retorna verdadeiro se existir alguma diferença numérica ou na dimensão das matrizes. Retora falso em caso contrário.
 */
    bool operator !=(const Matrix& Par_Matrix);

    /**
 * \brief Método para emitir o nome de classe.
 * \return string	Retorna o nome de classe do objeto.
 */
    std::string objectType(void);

    /**
 * \brief Método para emitir as associações de uma instância.
 * \return std::string	Retorna vazio para esta classe.
 * \deprecated Este método não possui uso ou deve ser evitado o seu uso, pois ele será removido em versões futuras.
 */
    std::string objectAssociations(void);

    /**
 * \brief Método de teste para o nome/tipo de instância.
 * \param s	Texto com o nome da classe que é esperado.
 * \return bool	Retorna verdadeiro caso o nome passado seja Matrix. Retorna falso no caso contrário.
 */
    bool is(std::string s);

    /**
 * \brief Método para extrair o equivalente em dados xml (mml:matrix) da matriz.
 * \return std::string	Retorna o string contendo o xml (mml:matrix) da matriz.
 */
    std::string xmlGetData();
    std::string xmlGetData(int prec);

    /**
 * \brief Método para setar os valores de atributos da matriz utilizando sua descrição em xml (mml:matrix).
 * \param xml	String contendo o xml (mml:matrix) da matriz.
 */
    void xmlSetData(std::string xml);

    /**
  * Método para computar a transposta da matriz em escopo.
  * @return Matrix	Matriz inversa.
  */
    Matrix inverse();

    Matrix inverse2();

    /**
          * \brief Metodo que inverte uma matriz pelo metodo OSU
          * \return Matrix : Retorna a matrix inversa
          */
    Matrix osuInverse();

    /**
  * Método para computar a transposta da matriz em escopo.
  * @return Matrix	Matrix transposta.
         */
    Matrix transpose();

    /**
  * Método para computar a redução à forma canônica da matriz em escopo.
  * @return Matrix	Matrix na forma canônica.
  */
    Matrix reduction();

    /**
  * \brief Método para computar a matriz triangular superior derivada da matriz em escopo.
  * \return Matrix	Matriz triangular superior.
  */
    Matrix triang();

    /**
 * \brief Método para retornar o conteúdo da matriz coluna realocado sobre a diagonal principal de uma matriz quadrada.
 * \return Matrix	Matrix cuja diagonal principal foi obtida de um vetor (matriz coluna).
 */
    Matrix toDiagonal();

    /**
 * \brief Método para selecionar a diagonal principal de uma matriz e montar uma nova matriz coluna com os valores selecionados.
 * \return Matrix	Matriz coluna com os valores da diagonal selecionados.
 */
    Matrix selDiagonal();

    // Todo: abandon the use of this method
    Matrix& putMatrix(Matrix put,int lin, int col);

    /* Methods into disuse:
     *
    Matrix* convertToMatrix(int *array, int linhas, int colunas);
    Matrix* convertToMatrix(double *array, int linhas, int colunas);
    */
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //MATRIX_H
