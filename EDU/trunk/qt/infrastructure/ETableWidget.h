#ifndef ETABLEWIDGET_H
#define ETABLEWIDGET_H

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QResizeEvent>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include "EDom.h"
#include "Matrix.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
* \file ETableWidget.h
* \class ETableWidget
* \brief Classe que gerencia as tabelas reimplementada da QTableWidget
* \copyright E-Foto group
* \author Paulo Andr� Batista Pupim e Irving Badolato
*/

class ETableWidget: public QTableWidget
{
	Q_OBJECT

private:
	Matrix *io;
	char mode;
	int decimals;
	QString type;
	QSpinBox *itemSpinBox;
	int currentSpinBoxRow,currentSpinBoxColumn,currentDoubleSpinBoxRow,currentDoubleSpinBoxColumn;
	QDoubleSpinBox *itemDoubleSpinBox;
	double previousValue;

	QList<int> typedSpinColumns;
	QList<int> typedDoubleSpinColumns;

public:
	/**
	* \brief Construtor default
	* \param parent: Ponteiro para objeto QWidget pai
	*/
	ETableWidget(QWidget *parent=0);

	/**
	* \brief Construtor com valores num�ricos e alguma formata��o para exibi��o da tabela
	* \param values: Matriz contendo os valores num�ricos a serem exibidos na tabela
	* \param mode  : Tipo de exibi��o com que os valores num�ricos aparecer�o: 'e' � formato exponencial , 'f' � formato decimal
	* \param precision: Precis�o com a qual os valores numericos ser�o exibidos
	* \param parent: Ponteiro para objeto QWidget pai
	*/
	ETableWidget(Matrix values,char mode='e',int precision=6,QWidget *parent=0);

	/**
	* \brief Metodo para inserir valores numericos na tabela a partir da coluna zero e linha zero
	* \param values: Matriz contendo os valores num�ricos a serem exibidos na tabela
	* \param mode  : Tipo de exibi��o com que os valores num�ricos aparecer�o: 'e' � formato exponencial , 'f' � formato decimal
	* \param precision: Precis�o com a qual os valores numericos ser�o exibidos
	*/
	void setTableData(Matrix values,char mode='e',int precision=6);

	/**
	* \brief Metodo para inserir valores numericos na tabela a partir da coluna zero e linha zero
	* \param xml: Objeto EDomElement com o xml contendo os valores num�ricos a serem exibidos na tabela
	* \param mode  : Tipo de exibi��o com que os valores num�ricos aparecer�o: 'e' � formato exponencial , 'f' � formato decimal
	* \param precision: Precis�o com a qual os valores numericos ser�o exibidos
	*/
	void setTableData(EDomElement xml,char mode='e',int precision=6);

	/**
	* \brief Metodo para inserir valores numericos na tabela a partir da coluna zero e linha zero
	* \param xmlString: String com o xml contendo os valores num�ricos a serem exibidos na tabela
	* \param mode  : Tipo de exibi��o com que os valores num�ricos aparecer�o: 'e' � formato exponencial , 'f' � formato decimal
	* \param precision: Precis�o com a qual os valores numericos ser�o exibidos
	*/
	void setTableData(string xmlString,char mode='e',int precision=6);

	/**
	*  \brief Metodo para redimensionar a tabela de forma que ela fique em um tamanho compacto, tentando sempre n�o prejudicar a visulaiza��o dos dados
	*/
	void resizeTable();

	/**
	* \brief Metodo que permite habilitar a copia automatica dos conteudos das celulas apenas por selecionad as celulas
	* \param enable : Habilita o modo automatico de copia
	*/
	void enableAutoCopy(bool enable=true);

	/**
	* \brief Metodo que permite especificar a partir de onde ser� colocada matriz com os valores numericos nas tabelas
	* \param input : Matriz que contem os valores numericos a serem colocados na tabela
	* \param row   : Linha inicial do preenchimento das celulas
	* \param column: Coluna inicial do preenchimento das celulas
	* \param mode  : Tipo de exibi��o com que os valores num�ricos aparecer�o: 'e' � formato exponencial , 'f' � formato decimal
	* \param precision: Precis�o com a qual os valores numericos ser�o exibidos
	* \attention Cuidado ao sobrescrever celulas j� preenchidas
	*/
	void putIn(Matrix input, int row, int column,char mode='e',int precision=6);

	/**
	* \brief Metodo para inserir em deter
	* \param list  : Lista contendo as QStrings que serao colocadas nas celulas
	* \param column: numerod  da coluna em que sera inserida a lista
	*/
	void putInColumn(QStringList list, int column);

	/*void putInRow(QStringList list,int row);
	//void setColumnType(int colIndex,QString type="QSpinBox", bool enable=true, double minValue=0, double maxValue=100);
	//void setType(int column, QString type="NONE");
	//bool checkTypeds(int column,QString type);
	//QString getType(int column);

	//QSpinBox * getItemSpinBox();
	//QDoubleSpinBox* getItemDoubleSpinBox();
	//int getCurrentSpinBoxRow();
	//int getCurrentSpinBoxColumn();
	//int getCurrentDoubleSpinBoxRow();
	//int getCurrentDoubleSpinBoxColumn();
	*/

	/**
	* \brief Metodo auxiliar para apagar sele��o de celulas
	*/
	void clearSelection();

	/**
	* \brief Metodo para alterar o modo de exibi��o dos valores num�ricos
	* \param newMode  : Tipo de exibi��o com que os valores num�ricos aparecer�o: 'e' � formato exponencial , 'f' � formato decimal
	*/
	void setMode(char newMode);

	/**
	* \brief Metodo para alterar a precis�o com que os valores num�ricos sao exibidos
	* \param newDecimals: Precis�o com a qual os valores numericos ser�o exibidos
	*/
	void setDecimals(int newDecimals);

	/**
	* \brief Metodo para retornar o modo atual
	* \return char : Caracter contendo o modo de exibi��o: 'e' � formato exponencial , 'f' � formato decimal
	*/
	char getMode();

	/**
	* \brief Metodo para retornar a precis�o atual
	* \return int : Inteiro contendo o numero de casas decimais exibidas
	*/
	int getDecimals();

	//double getPreviousValue();

public slots:

	/**
	* \brief Metodo que coloca na area de transferencia o conteudo da(s) celula(s) selecionada(s)
	*/
	void autoCopy();

	/**
	* \brief Override de metodo adequa��o do connect com o signal da QCheckBox, a saber, "stateChanged(int)"
	* \param int : Inteiro com o codigo de conex�o com QCheckBox: a saber 0 desconecta e 1 conecta sinal
	*/
	void enableAutoCopy(int enable);

	/**
	* \brief Atualiza os valores da tabela para a formata�ao informada nos parametros
	* \param mode  : Tipo de exibi��o com que os valores num�ricos aparecer�o: 'e' � formato exponencial , 'f' � formato decimal
	* \param precision: Precis�o com a qual os valores numericos ser�o exibidos
	* \attention Comportamento impreciso se houver valorer nao-numericos
	*/

	void updateTableValues(char mode='f', int precision=4);

	/**
	* \brief Atualiza os valores da tabela para a formata�ao informada no parametro
	* \param mode  : Tipo de exibi��o com que os valores num�ricos aparecer�o: 0 � formato exponencial , 1 � formato decimal
	* \attention Comportamento impreciso se houver valorer nao-numericos
	*/
	void updateModoValues(int modo=0);

	/**
	* \brief Atualiza os valoes da tabela para o numero de casa decimais
	* \param decimal : Numero de casa decimais para os valores numericos exibidos
	* \attention Comportamento impreciso se houver valorer nao-numericos
	*/
	void updateDecimalsValues(int decimal=4);

	//void selectionBackground();
	//void avaliateType(QTableWidgetItem *);
	//void avaliateType(int row, int col);
	//void validateItem();

/*
signals:
	void focusReceived();
	void validatedItem(int, int, int);
	void validatedItem(int, int, double);
*/

protected:
	// void resizeEvent(QResizeEvent *event);
	/**
	* \brief Metodo reimplementado do QTableWidget
	* \param QObject : Objeto que gerou um evento
	* \param QEvent : Evento gerado pelo objeto
	*/
	bool eventFilter(QObject *, QEvent *);

	/**
	* \brief Metodo reimplementado do QTableWidget. Quando o usuario pressiona ctrl+c, o conteudo das celulas selecionadas e posto na area de transferencia
	* \param Objeto de evento contendo as informa�oes sobre as teclas pressionadas
	*/
	void keyPressEvent(QKeyEvent *event);

	/*
	void focusInEvent(QFocusEvent *event);
	void focusOutEvent(QFocusEvent *event);
	//void dragMoveEvent(QDragMoveEvent *event);
	*/
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // ETABLEWIDGET_H
