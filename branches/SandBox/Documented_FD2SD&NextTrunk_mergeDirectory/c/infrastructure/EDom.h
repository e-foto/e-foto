#ifndef EDOM_H
#define EDOM_H

#include <string>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <deque>

using namespace std;

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class EDomElement;

/**
* \file EDom.h
* \class Conversion
* \brief Básico de operações para conversão entre valores numéricos e textos.
* \attention Todos os métodos desta classe são estáticos.
* \attention Não contrua objetos desta classe.
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class Conversion
{
public:
	/**
	* \brief Método para conversão de double para string.
	* \param value	Valor a ser convertido.
	* \return string	Texto que descreve o valor inserido.
	*/
	static string doubleToString(double value);

	/**
	* \brief Sobrecarga do método para conversão de double para string.
	* \param value	Valor a ser convertido.
	* \param precision	Definição da precisão desejada na conversão.
	* \return string	Texto que descreve o valor inserido.
	*/
	static string doubleToString(double value, int precision);

	/**
	* \brief Método para conversão de string para double.
	* \param value	Valor descrito em texto.
	* \return double	Valor convertido.
	*/
	static double stringToDouble(string value);

	/**
	* \brief Método para conversão de inteiro para string.
	* \param value	Valor a ser convertido.
	* \return string	Texto que descreve o valor inserido.
	*/
	static string intToString(int value);

	/**
	* \brief Método para conversão de string para inteiro.
	* \param value	Valor descrito em texto.
	* \return int	Valor convertido.
	*/
	static int stringToInt(string value);

	/**
	* \brief Método para conversão de inteiro longo para string.
	* \param value	Valor a ser convertido.
	* \return string	Texto que descreve o valor inserido.
	*/
	static string longToString(long value);

	/**
	* \brief Método para conversão de string para inteiro longo.
	* \param value	Valor descrito em texto.
	* \return long	Valor convertido.
	*/
	static long stringToLong(string value);
};

/**
* \file EDom.h
* \class EDomElement
* \brief Básico de operações de xml. Uma interface para acesso a elementos xml ao estilo DOM implementada internamente como um SAX.
* \attention Este componente não foi desenvolvido para ser um DOM completo, mas para atender as necessidades do projeto E-Foto ao lidar com permanencia de seus objetos com formato xml.
* \attention Não assuma que todas as considerações de uma interface DOM são atendidas por esta classe.
* \copyright E-Foto group
* \author Rafael Aguiar
*/
class EDomElement
{

	enum { INVALID_TAG = 0, OPEN_TAG = 1, CLOSE_TAG = 2, SIMPLE_TAG = 3 };

	string content;

	/**
	* \brief Verifica o tipo de uma tag xml passada como parâmetro.
	*        Tipos possíveis são INVALID_TAG, OPEN_TAG, CLOSE_TAG e SIMPLE_TAG.
	* \param myTag	Tag a ser verificada.
	* \return int	0 se INVALID_TAG, 1 se OPEN_TAG, 2 se CLOSE_TAG e 3 se SIMPLE_TAG.
	*/
	int tagType(string myTag);

	/**
	* \brief Retorna o tagname de uma tag xml passada como parâmetro.
	* \param myTag	Tag com o nome a ser retornado.
	* \return string	O nome da tag passada.
	*/
	string tagName(string myTag);

	/**
	* \brief Muda o valor de um dos atributos do elemento atual.
	* \param att	O nome do atributo a ser alterado.
	* \param newAttValue	Novo valor para o atributo.
	* \return bool	Verdadeiro se a alteração foi efetuada com sucesso, falso caso contrário.
	*/
	bool setAttribute(string att, string newAttValue);

	/**
	* \brief Adiciona um novo atributo ao elemento atual.
	* \param newAttName	Nome do atributo a ser adicionado.
	* \param newAttValue	Valor a ser atribuído ao novo atributo
	* \return bool Verdadeiro se a adição foi efetuada com sucesso, falso caso contrário.
	*/
	bool addAttribute(string newAttName, string newAttValue);

public:

	/**
	* \brief Construtor vazio padrão.
	*/
	EDomElement();

	/**
	* \brief Construtor de um elemento cujo conteúdo é conhecido.
	* \param myXml	Conteúdo a ser atribuído ao elemento construído.
	*/
	EDomElement(string myXml);

	/**
	* \brief Destrutor vazio padrão.
	*/
	~EDomElement();

	/**
	* \brief Muda o conteúdo xml do elemento atual.
	* \param newXml	O novo conteúdo xml do elemento.
	*/
	void setContent(string newXml);

	/**
	* \brief Recupera o conteúdo xml do elemento atual em formato string.
	* \return string	O conteúdo xml do elemento.
	*/
	string getContent();

	/**
	* \brief Lista os elementos filhos contidos no elemento atual.
	* \return deque<EDomElement>	Um deque contendo os elementos filhos.
	*/
	deque<EDomElement> children();

	/**
	* \brief Lista os elementos contidos no elemento atual (incluindo o próprio) que possuem o tagname especificado.
	* \param name	O tagname a ser buscado.
	* \return deque<EDomElement>	Um deque com a lista de elementos que correspondem aos critérios de pesquisa.
	*/
	deque<EDomElement> elementsByTagName(string name);

	/**
	* \brief Lista os elementos contidos no elemento atual (incluindo o próprio) que possuem o tagname especificado e um atributo com um determinado valor.
	* \param tagname	O tagname a ser buscado.
	* \param att	O nome do atributo a ser verificado.
	* \param value	O valor do atributo a ser verificado.
	* \return deque<EDomElement>	Um deque com a lista de elementos que correspondem aos critérios de pesquisa.
	*/
	deque<EDomElement> elementsByTagAtt(string tagname, string att, string value);

	/**
	* \brief Retorna o primeiro elemento contido no elemento atual (incluindo o próprio) que possui o tagname especificado.
	* \param name	O tagname a ser buscado.
	* \return EDomElement	O primeiro elemento que corresponde aos critérios de pesquisa.
	*/
	EDomElement elementByTagName(string tagname);

	/**
	* \brief Retorna o primeiro elemento contido no elemento atual (incluindo o próprio) que possui o tagname especificado e um atributo com um determinado valor.
	* \param name	O tagname a ser buscado.
	* \param att	O nome do atributo a ser verificado.
	* \param value	O valor do atributo a ser verificado.
	* \return EDomElement	O primeiro elemento que corresponde aos critérios de pesquisa.
	*/
	EDomElement elementByTagAtt(string tagname, string att, string value);

	/**
	* \brief Adiciona um elemento filho aos elementos contidos no elemento atual (incluindo o próprio) que possuem o tagname especificado.
	* \param tagname	O tagname a ser buscado.
	* \param newChild	O conteúdo do elemento filho a ser adicionado.
	* \return bool	Verdadeiro se a inserção foi realizada com sucesso, falso caso contrário.
	* \todo Fazer uma versão que receba um EDomElement como segundo parâmetro.
	*/
	bool addChildAtTagName(string tagname, string newChild);

	/**
	* \brief Adiciona um elemento filho aos elementos contidos no elemento atual (incluindo o próprio) que possuem o tagname especificado e um atributo com determinado valor.
	* \param tagname	O tagname a ser buscado.
	* \param att	O nome do atributo a ser verificado.
	* \param value	O valor do atributo a ser verificado.
	* \param newChild	O conteúdo do elemento filho a ser adicionado.
	* \return bool	Verdadeiro se a inserção foi realizada com sucesso, falso caso contrário.
	* \todo Fazer uma versão que receba um EDomElement como quarto parâmetro.
	*/
	bool addChildAtTagAtt(string tagname, string att, string value, string newChild);

	/**
	* \brief Substitui os elementos contidos no elemento atual (incluindo o próprio) que possuem o tagname especificado.
	* \param tagname	O tagname a ser buscado.
	* \param newChild	O novo conteúdo dos elementos a serem substituídos.
	* \return bool	Verdadeiro se a substituição foi realizada com sucesso, falso caso contrário.
	* \todo Fazer uma versão que receba um EDomElement como segundo parâmetro.
	*/
	bool replaceChildByTagName(string tagname, string newChild);

	/**
	* \brief Substitui os elementos contidos no elemento atual (incluindo o próprio) que possuem o tagname especificado e um atributo com determinado valor.
	* \param tagname	O tagname a ser buscado.
	* \param att	O nome do atributo a ser verificado.
	* \param value	O valor do atributo a ser verificado.
	* \param newChild	O novo conteúdo dos elementos a serem substituídos.
	* \return bool	Verdadeiro se a substituição foi realizada com sucesso, falso caso contrário.
	* \todo Fazer uma versão que receba um EDomElement como quarto parâmetro.
	*/
	bool replaceChildByTagAtt(string tagname, string att, string value, string newChild);

	/**
	* \brief Recupera o tagname do elemento atual.
	* \return string	O tagname do elemento atual.
	*/
	string tagName();

	/**
	* \brief Recupera o valor de um dos atributos do elemento atual.
	* \param att	O nome do parâmetro a ser retornado.
	* \return string	O valor do atributo desejado.
	*/
	string attribute(string att);

	/**
	* \brief Adiciona um atributo aos elementos contidos no elemento atual (incluindo o próprio) que possuem o tagname especificado.
	* \param tagname	O tagname a ser buscado.
	* \param newAtt	O nome do novo atributo a ser adicionado.
	* \param newAttValue	O valor a ser atribuído ao novo atributo.
	* \return bool	Verdadeiro se a adição foi realizada com sucesso, falso caso contrário.
	*/
	bool addAttributeByTagName(string tagname, string newAtt, string newAttValue);

	/**
	* \brief Adiciona um atributo aos elementos contidos no elemento atual (incluindo o próprio) que possuem o tagname especificado e um atributo com determinado valor.
	* \param tagname	O tagname a ser buscado.
	* \param att	O nome do atributo a ser verificado.
	* \param value	O valor do atributo a ser verificado.
	* \param newAtt	O nome do novo atributo a ser adicionado.
	* \param newAttValue	O valor a ser atribuído ao novo atributo.
	* \return bool	Verdadeiro se a adição foi realizada com sucesso, falso caso contrário.
	*/
	bool addAttributeByTagAtt(string tagname, string att, string value, string newAtt, string newAttValue);

	/**
	* \brief Modifica um atributo existente nos elementos contidos no elemento atual (incluindo o próprio) que possuem o tagname especificado.
	* \param tagname	O tagname a ser buscado.
	* \param replaceAtt	O nome do atributo a ser modificado.
	* \param newAttValue	O novo valor a ser atribuído ao atributo.
	* \return bool	Verdadeiro se a substituição foi realizada com sucesso, falso caso contrário.
	*/
	bool replaceAttributeByTagName(string tagname, string replaceAtt, string newAttValue);

	/**
	* \brief Modifica um atributo existente nos elementos contidos no elemento atual (incluindo o próprio) que possuem o tagname especificado e um atributo com determinado valor.
	* \param tagname	O tagname a ser buscado.
	* \param att	O nome do atributo a ser verificado.
	* \param value	O valor do atributo a ser verificado.
	* \param replaceAtt	O nome do atributo a ser modificado.
	* \param newAttValue	O valor a ser atribuído ao novo atributo.
	* \return bool	Verdadeiro se a substituição foi realizada com sucesso, falso caso contrário.
	*/
	bool replaceAttributeByTagAtt(string tagname, string att, string value, string replaceAtt, string newAttValue);

	/**
	* \brief Retorna o valor textual contido no elemento atual como uma string.
	* \return string	O valor contido no elemento atual.
	*/
	string toString();

	/**
	* \brief Retorna o valor textual contido no elemento atual convertido em um inteiro.
	* \return int	O valor contido no elemento atual.
	* \attention As regras de conversão utilizadas são as definidas na classe Conversion.
	*/
	int toInt();

	/**
	* \brief Retorna o valor textual contido no elemento atual convertido em um inteiro longo.
	* \return long	O valor contido no elemento atual.
	* \attention As regras de conversão utilizadas são as definidas na classe Conversion.
	*/
	long toLong();

	/**
	* \brief Retorna o valor textual contido no elemento atual convertido em um double.
	* \return double	O valor contido no elemento atual.
	* \attention As regras de conversão utilizadas são as definidas na classe Conversion.
	*/
	double toDouble();

	/**
	* \brief Retorna o valor textual contido no elemento atual convertido em um conjunto de doubles. Os valores são separados de acordo com os espaços em branco existentes entre eles.
	* \return deque<double>	Os valores contidos no elemento atual.
	* \attention As regras de conversão utilizadas são as definidas na classe Conversion.
	*/
	deque<double> toGmlPos();

	/**
	* \brief Testa se o valor textual contido no elemento atual está disponível, isto é, se não é vazio ou alguma variação de "Not Available".
	* \return bool	Verdadeiro se o valor estiver disponível, falso caso contrário.
	*/
	bool isAvailable();

	/**
	* \brief Testa se o conteúdo do elemento atual (incluindo o próprio) possui algum elemento com um determinado tagname.
	* \param tagname	O tagname a ser identificado.
	* \return bool	Verdadeiro se a tagname existir dentro do elemento atual, falso caso contrário.
	*/
	bool hasTagName(string tagname);

	/**
	* \brief Retorna um elemento de conteúdo idêntico ao atual, mas com uma determinada indentação aplicada.
	* \param indentation	O caracter a ser usado como indentação. Geralmente ' ' ou '\t'.
	* \return EDomElement	O elemento com a indentação aplicada.
	*/
	EDomElement indent(char indentation);

	/**
	* \brief Retorna um elemento de conteúdo idêntico ao atual, mas com uma determinada indentação removida.
	* \param indentation	O caracter a ser usado como indentação. Geralmente ' ' ou '\t'.
	* \return EDomElement	O elemento com a indentação removida.
	*/
	EDomElement trim(char charToTrim);

	/**
	* \brief Retorna um elemento de conteúdo idêntico ao atual, mas com as linhas em branco removidas. Adicionalmente, todo tipo de espaço em branco usado como indentação pode ser removido.
	* \param removeIndentation	Flag que seleciona se o método deve remover os espaços em branco ou não.
	* \return EDomElement	O Elemento com as linhas em branco removidas.
	*/
	EDomElement removeBlankLines(bool removeIndentation = false);
};

/**
* \file EDom.h
* \class EDom
* \brief Interface básica para todos os componentes que possuam representação em xml (o formato interno do arquivo .epp).
* \attention A classe EDom foi definida como uma classe abstrata, então você não deve usá-la diretamente.
* \attention Contrua apenas objetos das classes derivadas.
* \copyright E-Foto group
* \authors Rafael Aguiar
*/
class EDom
{

public:

	/**
	* \brief Método para extrair o equivalente em dados xml de um componente.
	* \return string	Retorna o string contendo o xml com todos os valores de atributos contidos numa instância de classe derivada.
	*/
	virtual string xmlGetData() = 0;

	/**
	* \brief Método para setar os valores de atributos de um componente utilizando sua descrição em xml.
	* \param xml	String contendo o xml com todos os valores de atributos adequados a uma determinada instância.
	*/
	virtual void xmlSetData(string xml) = 0;

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //EDOM_H
