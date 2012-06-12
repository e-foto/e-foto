#ifndef PROGRESSPUBLISHER_H
#define PROGRESSPUBLISHER_H

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
* \file ProgressPublisher.h
* \class ProgressPublisher
* \brief Estrutura básica para comunicação de progresso pelas interfaces gráficas.
* \abstract A classe ProgressPublisher é considerada pelos autores como uma classe abstrata, então você não deve usá-la diretamente.
* \attention Contrua apenas objetos das classes derivadas.
* \copyright E-Foto group
* \authors Irving Badolato.
*/
class ProgressPublisher
{

public:

	/**
	* \brief Método que altera a barra de progresso na interface.
	*/
	virtual void setProgress(int progress) = 0;

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //PROGRESSPUBLISHER_H
