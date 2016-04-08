#ifndef PROGRESSPUBLISHER_H
#define PROGRESSPUBLISHER_H
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
