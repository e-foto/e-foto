#ifndef IMAGESFORM_H
#define IMAGESFORM_H


#include "ui_ImagesLayout.h"
#include "AbstractForm.h"
#include "EDom.h"
#include <QTranslator>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
 * \file ImagesForm.h
 * \class ImagesForm
 * \brief Classe que controla a interface do formulario de Imagens
 * \author
 */
class ImagesForm : public AbstractForm, public Ui::ImagesView
{
	Q_OBJECT

public:
	/**
 * \brief Construtor do formulario
 * \param parent : QWidget pai do formulario
 */
	ImagesForm(QWidget *parent=0);

	/**
 * \brief Metodo que preenche um formulário previamente salvo em um arquivo *.epp
 * \param values : string que contém o xml com os valores que preencherão o formulario
 */
	void fillvalues(string values);

	/**
 * \brief Metodo que cria uma string, padronizada em xml, com os valores dos campos do formulario
 * \return string : Texto em xml contendo tags com os campos do formulario
 */
	string getvalues();

	/**
 * \brief Metodo que altera todos os objetos editaveis do formulario no modo de somente leitura
 * \param state : Diz se objetos devem ficar no modo somente leitura ou não
 */
	void setReadOnly(bool state);

	/**
 * \brief Metodo que informa se o formulario é o mesmo do parametro formName
 * \param formName : nome do formulario
 * \return bool : Verdadeiro se formName é esse formulario, falso caso contrário
 */
	bool isForm(string formName);

	/**
 * \brief Metodo que informa no formulario se a orientaçao interior foi feita ou não para cada imagem
 * \param xmlIOs : string informando se o estado da orientaçao interior de todas as imagens cadastradas no projeto
 */
	void setIOsAvailable(string xmlIOs);

	/**
 * \brief Metodo que informa no formulario se a orientaçao exterior foi feita ou não para cada imagem
 * \param xmlIOs : string informando se o estado da orientaçao exterior de todas as imagens cadastradas no projeto
 */
	void setEOsAvailable(string xmlEOs);

private slots:

	/**
 * \brief Metodo que envia um sinal toda vez que uma coluna/campo de uma imagem é clicada
 * \param i : Valor numérico da linha da tabela contendo as imagens
 */
	void emitSignal(int i);

signals:

	/**
 * \brief Sinal emitido informando qual a linha em que a tabela foi clicada
 * \param index : Valor numérico da linha em que a tabela de imagens foi clicado
 */
	void clicked(int index);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // IMAGESFORM_H
