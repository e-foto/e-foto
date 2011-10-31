#ifndef IMAGEFORM_H
#define IMAGEFORM_H

#include "ui_ImageLayout.h"
#include "AbstractForm.h"
#include "SigmaForm.h"
#include "EDom.h"

class ProjectUserInterface_Qt;

class ImageForm : public AbstractForm, public Ui::ImageView
{
    Q_OBJECT
	string tagXml;
	QString fileImagePath; QString fileImageName;
	QString lastPath;
	bool activeGNSS; bool activeINS;

public:
    ProjectUserInterface_Qt* proj;
	SigmaFormController* gnssSigmaController;
	SigmaFormController* insSigmaController;


	/**
	* \brief Construtor do formulario
	* \param parent : QWidget pai do formulario
	*/
    ImageForm(QWidget *parent=0);

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
	* \brief Metodo que altera todos os objetos do formulario para conven��o de data, hora etc. para o local especificado
	* \param locale : Objeto que informa o local
	*/
	void setFormLocale(QLocale locale);

	/**
	* \brief Metodo que esvazia ou zera todos os campos do formul�rio
	*/
	void cleanForm();

	/**
	* \brief Metodo que retorna um texto contendo o caminho do arquivo de imagem apontado no formulario
	* \return string : Caminho do arquivo de imagem
	*/
	string getFileImagePath();

	/**
	* \brief Metodo que retorna um texto contendo o nome do arquivo de imagem apontado no formulario
	* \return string : Nome do arquivo de imagem
	*/
	string getFileImageName();


	/**
	* \brief Metodo que informa se o formulario � o mesmo do parametro formName
	* \param formName : nome do formulario
	* \return bool : Verdadeiro se formName � esse formulario, falso caso contr�rio
	*/
	bool isForm(string formName);

	/**
	* \brief Metodo que informa no formulario se a orienta�ao interior foi feita ou n�o
	* \param state : Booleano informando se o estado da orienta�ao interior da imagem apontada por esse formulario
	*/
	void setIOAvailable(bool state);

	/**
	* \brief Metodo que informa no formulario se a orienta�ao exterior foi feita ou n�o
	* \param state : Booleano informando se o estado da orienta�ao exterior da imagem apontada por esse formulario
	*/
	void setEOAvailable(bool state);


private slots:
	/**
	* \brief Metodo que informa o nome do arquivo de imagem escolhido pelo usuario no formulario e obtem as dimens�es das imagens
	* \return QString : Contem o nome do arquivo de imagem
	*/
    QString loadImageFile();

	/**
	* \brief Metodo que mostra ou n�o os metadados baseado no campo correspondente ao nome do arquivo.
	* \param metadata : Conteudo do campo correspondente ao nome do arquivo
	* \deprecated Metodo em desuso atualmente
	*/
	void metadataVisibleChanged(QString);

};

#endif // IMAGEFORM_H
