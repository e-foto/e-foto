#ifndef IMAGESFORM_H
#define IMAGESFORM_H


#include "ui_ImagesLayout.h"
#include "AbstractForm.h"
#include "EDom.h"

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
	* \brief Metodo que preenche um formul�rio previamente salvo em um arquivo *.epp
	* \param values : string que cont�m o xml com os valores que preencher�o o formulario
	*/
    void fillvalues(string values);

	/**
	* \brief Metodo que cria uma string, padronizada em xml, com os valores dos campos do formulario
	* \return string : Texto em xml contendo tags com os campos do formulario
	*/
	string getvalues();

	/**
	* \brief Metodo que altera todos os objetos editaveis do formulario no modo de somente leitura
	* \param state : Diz se objetos devem ficar no modo somente leitura ou n�o
	*/
	void setReadOnly(bool state);

	/**
	* \brief Metodo que informa se o formulario � o mesmo do parametro formName
	* \param formName : nome do formulario
	* \return bool : Verdadeiro se formName � esse formulario, falso caso contr�rio
	*/
	bool isForm(string formName);

	/**
	* \brief Metodo que informa no formulario se a orienta�ao interior foi feita ou n�o para cada imagem
	* \param xmlIOs : string informando se o estado da orienta�ao interior de todas as imagens cadastradas no projeto
	*/
	void setIOsAvailable(string xmlIOs);

	/**
	* \brief Metodo que informa no formulario se a orienta�ao exterior foi feita ou n�o para cada imagem
	* \param xmlIOs : string informando se o estado da orienta�ao exterior de todas as imagens cadastradas no projeto
	*/
	void setEOsAvailable(string xmlEOs);

private slots:

	/**
	* \brief Metodo que envia um sinal toda vez que uma coluna/campo de uma imagem � clicada
	* \param i : Valor num�rico da linha da tabela contendo as imagens
	*/
	void emitSignal(int i);

signals:

	/**
	* \brief Sinal emitido informando qual a linha em que a tabela foi clicada
	* \param index : Valor num�rico da linha em que a tabela de imagens foi clicado
	*/
	void clicked(int index);

};

#endif // IMAGESFORM_H
