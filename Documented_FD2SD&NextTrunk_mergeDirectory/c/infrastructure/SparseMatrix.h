#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <deque>
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
	int row;
	int col;
	double value;
	SparseMatrixElement* next;

public:

	/**
	* \brief Construtor vazio padrão.
	*/
	SparseMatrixElement();

	/**
	* \brief Construtor de um elemento cujos atributos são conhecidos.
	* \param myRow	Linha da matriz na qual o elemento se encontra.
	* \param myCol	Coluna da matriz na qual o elemento se encontra.
	* \param myValue	Valor do elemento.
	*/
	SparseMatrixElement(int myRow, int myCol, double myValue);

	/**
	* \brief Destrutor vazio padrão.
	*/
	~SparseMatrixElement();

	/**
	* \brief Recupera o índice da linha onde o elemento se encontra.
	* \return int	A linha na qual elemento se encontra.
	*/
	int getRow();

	/**
	* \brief Recupera o índice da coluna onde o elemento se encontra.
	* \return int	A coluna na qual o elemento se encontra.
	*/
	int getCol();

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
	void setRow(int newRow);

	/**
	* \brief Modifica o índice da coluna onde o elemento se encontra.
	* \param newCol	A nova coluna onde o elemento se encontra.
	*/
	void setCol(int newCol);

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
	int rows;
	int cols;
	deque<SparseMatrixElement*> elements;

public:

	/**
	* \brief Construtor vazio padrão.
	*/
	SparseMatrix();

	/**
	* \brief Construtor a partir de um elemento do tipo Matrix.
	* \param source	A matriz padrão a ser utilizada como base na construção da matriz esparsa.
	* \param precision	A precisão a ser considerada para decidir se um elemento é nulo ou não.
	*/
	SparseMatrix(Matrix source, double precision = 1e-20);

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
