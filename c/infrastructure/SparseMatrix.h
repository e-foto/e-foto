#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H
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

#include "Matrix.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
* \file SparseMatrix.h
* \class SparseMatrixElement
* \brief Estrutura base para o processamento de matrizes esparsas.
* \attention Não contrua objetos desta classe. Ela existe apenas como suporte para a classe SparseMatrix.
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class SparseMatrixElement
{
    size_t row_;
    size_t col_;
    double value_;
    SparseMatrixElement* next_;

public:


/**
 * \brief Construtor de um elemento cujos atributos são conhecidos.
 * \param myRow	Linha da matriz na qual o elemento se encontra.
 * \param myCol	Coluna da matriz na qual o elemento se encontra.
 * \param myValue	Valor do elemento.
 */
    explicit SparseMatrixElement(size_t myRow, size_t myCol, double myValue);

	/**
 * \brief Destrutor vazio padrão.
 */
	~SparseMatrixElement();

	/**
 * \brief Recupera o índice da linha onde o elemento se encontra.
 * \return int	A linha na qual elemento se encontra.
 */
    size_t getRow();

	/**
 * \brief Recupera o índice da coluna onde o elemento se encontra.
 * \return int	A coluna na qual o elemento se encontra.
 */
    size_t getCol();

	/**
 * \brief Recupera o valor do elemento.
 * \return double	O valor do elemento.
 */
	double getValue();

	/**
 * \brief Recupera um ponteiro para o próximo elemento não-nulo (diferente de 0.0) localizado na mesma linha que o elemento atual.
 * \return SparseMatrixElement*	O próximo elemento não-nulo.
 */
	SparseMatrixElement* getNext();

	/**
 * \brief Modifica o índice da linha onde o elemento se encontra.
 * \param newRow	A nova linha onde o elemento se encontra.
 */
    void setRow(size_t newRow);

	/**
 * \brief Modifica o índice da coluna onde o elemento se encontra.
 * \param newCol	A nova coluna onde o elemento se encontra.
 */
    void setCol(size_t newCol);

	/**
 * \brief Modifica o valor do elemento.
 * \param newValue	O novo valor do elemento.
 */
	void setValue(double newValue);

	/**
 * \brief Modifica o ponteiro para o próximo elemento não-nulo (diferente de 0.0) localizado na mesma linha que o elemento atual.
 * \param newNext	O novo ponteiro para o próximo elemento.
 */
	void setNext(SparseMatrixElement* newNext);
};

/**
* \file SparseMatrix.h
* \class SparseMatrix
* \brief Classe projetada para aumentar o desempenho no processamento de matrizes esparsas.
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class SparseMatrix
{
    size_t rows_;
    size_t cols_;
    std::deque<SparseMatrixElement*> elements_;

public:


/**
 * \brief Construtor a partir de um elemento do tipo Matrix.
 * \param source	A matriz padrão a ser utilizada como base na construção da matriz esparsa.
 * \param precision	A precisão a ser considerada para decidir se um elemento é nulo ou não.
 */
    explicit SparseMatrix(Matrix source, double precision = 1e-20);

	/**
 * \brief Destrutor vazio padrão.
 */
	~SparseMatrix();

	/**
 * \brief Converte uma matriz esparsa em um elemento do tipo Matrix.
 * \return Matrix Uma matriz padrão equivalente à matriz esparsa corrente.
 */
	Matrix toMatrix();

	/**
 * \brief Multiplica uma matriz esparsa por uma matriz padrão.
 * \param target	O matriz pela qual a atual deve ser multiplicada.
 * \return Matrix	O resultado da operação de multiplicação.
 */
	Matrix operator*(Matrix target);
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // SPARSEMATRIX_H
