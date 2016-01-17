#ifndef GEOSYSTEM_H
#define GEOSYSTEM_H
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

#include <string>
#include "CommonDef.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {


/**
* \file GeoSystem.h
* \class GeoSystem
* \brief Classe que cria um objeto sistema geodesico, arbitrario ou um existente a partir de um GeoSystemEnum
* \copyright E-Foto group
* \author Paulo Pupim
*/
class GeoSystem
{
private:
	double a;//eixoMenor
	double b;//eixoMaior;
	double f;// achatamento
    std::string systemName;//nome do sistema
    /**
  * \brief Altera o valor do semi-eixo maior
  * \param newA : Novo valor de a;
  */
    void setEixoMaior(double newA);
    /**
  * \brief Altera o valor do semi-eixo menor
  * \param newB : Novo valor de b;
  */
    void setEixoMenor(double newB);

    /**
  * \brief Altera o valor do achatamento
  * \param newF : Novo valor de f;
  */
    void setF(double newF);

    /**
  * \brief Altera o nome do sistema
  * \param name : Valor se systemName;
  * \attention Alterar o nome do sistema nao altera os seus atributos automaticamente
  */
    void setSystemName(std::string name);

    /**
  * \brief Altera o objeto para o sistema geodesico do enumerador
  * \param system : Valor do enumerador;
  */
    void setSystem(DatumType system);

    /**
  * \brief Altera os atributos dos sistemas, ou seja, altera o sistema
  * \param a: semi-eixo maior do elipsoide
  * \param f: achatamento do elipsoide
  * \param newName: nome do sistema geodesico
  */
    void setSystem(double a, double f, std::string newName);

    /**
  * \brief Retorna umm representaçao textual do objeto, ou seja, seus principais atributos em forma de texto
  * \return string : Texto com os principais atributos do objeto, a saber, semi-eixo maior, menor e achatamento do elipsoide
  */
    std::string toString();

public:
	/**
  * \brief Construtor a partir de um enumerador
  * \param system : Enumerador do sistema geodesico
  */
    explicit GeoSystem(DatumType system=DatumType::WGS84);

	/**
  * \brief Construtor com as informaççoes basicas de um sistema
  * \param a: semi-eixo maior do elipsoide
  * \param f: achatamento do elipsoide
  * \param name: nome do sistema geodesico
  */
    //explicit GeoSystem(double a, double f, std::string name);

	/**
  * \brief Construtor com o nome do sistema geodesico
  * \param sysName : nome do sistema geodesico
  * \attention Se o \param sysName for inexistente o objeto tera valores incorretos, use esse construtor com cuidado
  */
    explicit GeoSystem(std::string sysName);

	/**
  * \brief Retorna o valor do semi-eixo maior
  * \return double : Valor se a;
  */
	double getA();

	/**
  * \brief Retorna o valor do semi-eixo menor
  * \return double : Valor se b;
  */
	double getB();

	/**
  * \brief Retorna o valor do achatamento
  * \return double : Valor se f;
  */
	double getF();

	/**
  * \brief Metodo que retorna o valor da primeira excentricidade do elipsoide
  * \return double : Valor da primeira excentricidade
  */
	double getFirstEccentricity();

	/**
  * \brief Metodo que retorna o valor da segunda excentricidade do elipsoide
  * \return double : Valor da segunda excentricidade
  */
	double getSecondEccentricity();

	/**
  * \brief Retorna o nome do sistema geodesico
  * \return string : Valor de systemName;
  */
    std::string getSystemName();


};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // GEOSYSTEM_H
