#ifndef ETREEMODEL_H
#define ETREEMODEL_H

#include <deque>
#include <string>
using namespace std;

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
	unsigned int id;
	string description;
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
	ETreeElement(int id, string description);

	/**
	* \brief Construtor de um elemento a partir de sua representação XML.
	* \param data	A representação XML do elemento.
	*/
	ETreeElement(string data);

	/**
	* \brief Recupera a chave de identificação do elemento.
	* \return unsigned	A chave identificadora do elemento.
	*/
	unsigned int getId();

	/**
	* \brief Recupera a descrição textual do elemento.
	* \return string	A descrição do elemento.
	*/
	string getDescription();
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
	string description;
	deque<ETreeElement> children;
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
	ETreeNode(string description, deque<ETreeElement> children);

	/**
	* \brief Construtor de um nó a partir de sua representação XML.
	* \param data	A representação XML do nó.
	*/
	ETreeNode(string data);

	/**
	* \brief Recupera a descrição textual do nó.
	* \return string	A descrição do nó.
	*/
	string getDescription();

	/**
	* \brief Conta a quantidade de elementos folhas associados ao nó.
	* \return unsigned	O número de elementos filhos do nó.
	*/
	unsigned int countChildren();

	/**
	* \brief Recupera a lista de elementos folhas associados ao nó.
	* \return deque<ETreeElement> A lista de elementos filhos do nó.
	*/
	deque<ETreeElement> getChildren();

	/**
	* \brief Recupera um elemento folha a partir do seu índice na lista.
	* \param index	O índice do elemento desejado.
	* \return ETreeElement	O elemento desejado.
	*/
	ETreeElement getChild(unsigned int index);

	/**
	* \brief Recupera a descrição textual de um elemento folha do nó.
	* \param index	O índice do elemento desejado.
	* \return string	A descrição do elemento desejado.
	*/
	string dataAt(unsigned int index);

	/**
	* \brief Recupera a chave de identificação de um elemento folha do nó.
	* \param index	O índice do elemento desejado.
	* \return unsigned	O identificador do elemento desejado.
	*/
	unsigned int idAt(unsigned int index);

	/**
	* \brief Lista as descrições textuais de todos os elementos folhas do nó.
	* \return deque<string>	A lista de todas as descrições dos elementos filhos.
	*/
	deque<string> data();
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
	deque<ETreeNode> children;
public:

	/**
	* \brief Construtor vazio padrão.
	*/
	ETreeModel();

	/**
	* \brief Construtor de uma árvore com estrutua conhecida.
	* \param children	A lista de nós da árvore.
	*/
	ETreeModel(deque<ETreeNode> children);

	/**
	* \brief Construtor de uma árvore a partir da raiz do xml contido em um arquivo .epp.
	* \param data	Uma string com o conteúdo do arquivo .epp.
	*/
	ETreeModel(string data);

	/**
	* \brief Conta quantos nós a árvore possui.
	* \return unsigned	A quantidade de nós da árvore.
	*/
	unsigned int countChildren();

	/**
	* \brief Conta quantas folhas um determinado nó árvore possui.
	* \param index	O índice do nó a ser contabilizado.
	* \return unsigned	A quantidade de folhas existentes no nó selecionado.
	*/
	unsigned int countGrandchildren(unsigned int index);

	/**
	* \brief Recupera a lista de nós existentes na árvore.
	* \return deque<ETreeNode>	A lista de nós da árvore.
	*/
	deque<ETreeNode> getChildren();

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
	* \return string	A descriçao da folha.
	*/
	string dataAt(unsigned int nodeIndex, unsigned int elementIndex);

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
	* \return string	A descrição do nó.
	*/
	string dataAt(unsigned int index);

	/**
	* \brief Recupera uma lista contendo as listas de descrições das folhas da árvore, organizada por nós.
	* \return deque<deque<string>>	A lista das listas de descrições das folhas da árvore.
	*/
	deque< deque<string> > data();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // ETREEMODEL_H
