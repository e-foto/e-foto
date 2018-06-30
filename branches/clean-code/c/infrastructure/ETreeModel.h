#ifndef ETREEMODEL_H
#define ETREEMODEL_H
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

/**
* \file ETreeModel.h
* \class ETreeElement
* \brief Elemento folha de uma árvore de dois níveis representativa da estrutura de um arquivo .epp.
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class ETreeElement
{
    int id_;
    std::string description_;
public:

	/**
 * \brief Construtor vazio padrão.
 */
	ETreeElement();

	/**
 * \brief Construtor de um elemento cujos atributos são conhecidos.
 * \param id	A chave de identificação do elemento descrito.
 * \param description	A descriçao do elemento.
 */
    explicit ETreeElement(int id, std::string _description);

	/**
 * \brief Construtor de um elemento a partir de sua representação XML.
 * \param data	A representação XML do elemento.
 */
    explicit ETreeElement(std::string data);

	/**
 * \brief Recupera a chave de identificação do elemento.
 * \return unsigned	A chave identificadora do elemento.
 */
    unsigned int getId() const;

	/**
 * \brief Recupera a descrição textual do elemento.
 * \return std::string	A descrição do elemento.
 */
    std::string getDescription() const;
};

/**
* \file ETreeModel.h
* \class ETreeNode
* \brief Elemento intermediário (nó) de uma árvore de dois níveis representativa da estrutura de um arquivo .epp.
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class ETreeNode
{
    std::string description_;
    std::deque<ETreeElement> children_;
public:

	/**
 * \brief Construtor vazio padrão.
 */
	ETreeNode();

	/**
 * \brief Construtor de um nó cujos atributos são conhecidos.
 * \param description	A descrição do nó.
 * \param children	A lista de elementos filhos do nó.
 */
    explicit ETreeNode(std::string description, std::deque<ETreeElement> children_);

	/**
 * \brief Construtor de um nó a partir de sua representação XML.
 * \param data	A representação XML do nó.
 */
    explicit ETreeNode(std::string data);

	/**
 * \brief Recupera a descrição textual do nó.
 * \return std::string	A descrição do nó.
 */
    std::string getDescription() const;

	/**
 * \brief Conta a quantidade de elementos folhas associados ao nó.
 * \return unsigned	O número de elementos filhos do nó.
 */
    unsigned int countChildren() const;

/**
 * \brief Recupera um elemento folha a partir do seu índice na lista.
 * \param index	O índice do elemento desejado.
 * \return ETreeElement	O elemento desejado.
 */
	ETreeElement getChild(unsigned int index);

	/**
 * \brief Recupera a descrição textual de um elemento folha do nó.
 * \param index	O índice do elemento desejado.
 * \return std::string	A descrição do elemento desejado.
 */
    std::string dataAt(unsigned int index);

	/**
 * \brief Recupera a chave de identificação de um elemento folha do nó.
 * \param index	O índice do elemento desejado.
 * \return unsigned	O identificador do elemento desejado.
 */
	unsigned int idAt(unsigned int index);

	/**
 * \brief Lista as descrições textuais de todos os elementos folhas do nó.
 * \return std::deque<std::string>	A lista de todas as descrições dos elementos filhos.
 */
    std::deque<std::string> data();
};

/**
* \file ETreeModel.h
* \class ETreeModel
* \brief Elemento raiz de uma árvore de dois níveis representativa da estrutura de um arquivo .epp.
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class ETreeModel
{
    std::deque<ETreeNode> children_;
public:

	/**
 * \brief Construtor vazio padrão.
 */
	ETreeModel();

	/**
 * \brief Construtor de uma árvore com estrutua conhecida.
 * \param children	A lista de nós da árvore.
 */
    explicit ETreeModel(std::deque<ETreeNode> children_);

	/**
 * \brief Construtor de uma árvore a partir da raiz do xml contido em um arquivo .epp.
 * \param data	Uma string com o conteúdo do arquivo .epp.
 */
    explicit ETreeModel(std::string data);

	/**
 * \brief Conta quantos nós a árvore possui.
 * \return unsigned	A quantidade de nós da árvore.
 */
    unsigned int countChildren() const;

	/**
 * \brief Conta quantas folhas um determinado nó árvore possui.
 * \param index	O índice do nó a ser contabilizado.
 * \return unsigned	A quantidade de folhas existentes no nó selecionado.
 */
	unsigned int countGrandchildren(unsigned int index);

/**
 * \brief Recupera um nó especifico da árvore.
 * \param index	O índice do nó desejado.
 * \return ETreeNode	O nó desejado.
 */
	ETreeNode getChild(unsigned int index);

	/**
 * \brief Recupera a descrição textual de uma das folhas da árvore.
 * \param nodeIndex	O índice do nó no qual a folha se encontra.
 * \param elementIndex	O índice da folha dentro do nó.
 * \return std::string	A descriçao da folha.
 */
    std::string dataAt(unsigned int nodeIndex, unsigned int elementIndex);

	/**
 * \brief Recupera a chave de identificação de uma das folhas da árvore.
 * \param nodeIndex	O índice do nó no qual a folha se encontra.
 * \param elementIndex	O índice da folha dentro do nó.
 * \return unsigned	O identificador da folha.
 */
	unsigned int idAt(unsigned int nodeIndex, unsigned int elementIndex);

	/**
 * \brief Recupera a descrição textual de um dos nós da árvore.
 * \param index	O índice do nó.
 * \return std::string	A descrição do nó.
 */
    std::string dataAt(unsigned int index);

	/**
 * \brief Recupera uma lista contendo as listas de descrições das folhas da árvore, organizada por nós.
 * \return std::deque<std::deque<std::string>>	A lista das listas de descrições das folhas da árvore.
 */
    std::deque< std::deque<std::string> > data();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // ETREEMODEL_H
