#ifndef DMS_H
#define DMS_H
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

#include "CommonDef.h"

#include <string>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
* \file Dms.h
* \class Dms
* \brief Classe que manipula e faz conversoes entre graus minutos segundos(DMS), grau decimal e radiano
* \todo Esta classe usa QString do Qt, logo isso sera alterado para string
* \copyright E-Foto group
* \author Paulo Pupim
*/
class Dms
{

public:
    /**
    * \brief Construtor padrao
    */
    Dms();

    /**
    * \brief Construtor do tipo grau minuto segundo, sinal padrao e positivo, isto e, signal=false
    * \param degree : valor do grau
    * \param minute : valor do minuto
    * \param second : valor do segundo
    * \param signal : sinal
    */
    explicit Dms(int degree, int minute, double second,bool signal=false);

    /**
    * \brief Construtor do tipo grau minuto segundo, informando apenas o total de segundos
    * \param seconds : total de segundos que um Dms possui
    */
    explicit Dms(double seconds);

    /**
    * \brief Construtor do tipo grau minuto segundo, informando a QString equivalente
    * \param dms : QString no formato dd°mm'ss.sss" ou dd mm ss.sss
    * \attention QString em outros formatos podem gerar resultados inesperados
    */
    //Dms(QString dms);


    /**
    * \brief Construtor do tipo grau minuto segundo, informando a QString equivalente
    * \param dms : string no formato dd°mm'ss.sss" ou dd mm ss.sss
    * \attention string em outros formatos podem gerar resultados inesperados
    */
    explicit Dms(std::string dms);

    /**
    * \brief Metodo para retornar o valor absoluto do grau
    * \return int : Valor do grau do Dms
    */
    int getDegree();

    /**
    * \brief Metodo para retornar o valor absoluto do minuto
    * \return int : Valor do minuto do Dms
    */
    int getMinute();

    /**
    * \brief Metodo para retornar o valor absoluto do segundo
    * \return int : Valor do segundo do Dms
    */
    double getSeconds();

    /**
    * \brief Metodo que retorna o sinal do Dms
    * \return bool : sinal
    */
    bool hasSignal();

    /**
    * \brief Metodo para retornar se o Dms e valido ou nao, ou seja, se grau, minuto e segundo sao valores validos
    * \return bool : Validade do Dms
    */
    bool isValid();

    /**
    * \brief Retorna ponteiro para o objeto
    * \return Dms : ponteiro para o objeto
    */
    Dms* getDms();

    /**
    * \brief Metodo para retornar o objeto em formato texto, com precisao nos segundos
    * \param decimals : precisao de retorno dos segundos
    * \return QString : Objeto em formato texto dd°mm'ss.sss"
    */
    //QString toString(int decimals=20);

    /**
    * \brief Metodo para retornar o objeto em formato texto, com precisao nos segundos
    * \param decimals : precisao de retorno dos segundos
    * \return string : Objeto em formato texto dd°mm'ss.sss"
    */
    std::string toString(int decimals=20);

    /**
    * \brief converte uma QString num formato padronizado em objeto Dms
    * \param dms : QString no formato dd°mm'ss.sss" ou dd mm ss.sss
    * \attention QString em outros formatos podem gerar resultados inesperados
    * \return Dms : ponteiro para objeto Dms
    */
    //Dms* stringToDms(QString dms);

    /**
    * \brief converte uma QString num formato padronizado em objeto Dms
    * \param dms : QString no formato dd°mm'ss.sss" ou dd mm ss.sss
    * \attention QString em outros formatos podem gerar resultados inesperados
    * \return Dms : ponteiro para objeto Dms
    */
    Dms* stringToDms(std::string dms);

    /**
    * \brief Metodo para alterar os valores do objeto apartir de outro Dms
    * \param Dms : objeto a ter seus valores clonados
    */
    void setDms(Dms newDms);

    /**
    * \brief Metodo para alterar os valores do objeto
    * \param degree : Novo valor de grau
    * \param minute : Novo valor de minuto
    * \param seconds : Novo valor de segundo
    * \param signal : Novo sinal
    */
    void setDms(int degree, int minute , double seconds, bool signal=false);

    /**
    * \brief Metodo para alterar o valor do grau
    * \param newDegrees: Novo valor de grau
    */
    void setDegree(int newDegrees);

    /**
    * \brief Metodo para alterar o valor do minuto
    * \param newMinutes: Novo valor de minuto
    */
    void setMinute(int newMinutes);

    /**
    * \brief Metodo para alterar o valor do segundo
    * \param newDegrees: Novo valor de segundo
    */
    void setSeconds(double newSeconds);

    /**
    * \brief Metodo para alterar o sinal
    * \param newDegrees: Novo sinal
    */
    void setSignal(bool newSignal);

    /**
    * \brief Diz a precisao numerica que os segundos terao
    * \param precision : digitos da precisao
    */
    void setSecondsPrecision(int precision=2);

    /**
    * \brief Retorna o valor da precisao dos segundos
    * \return int : precisao numerica dos segundos
    */
    int getSecondsPrecision();

    /**
    * \brief Soma ao objeto os valores dos atributos de degMinSec
    * \param degMinSec1 : Objeto a ser somado
    */
    void addDegMinSecs(Dms* degMinSec);

    /**
    * \brief Multiplica o objeto por uma constante inteira
    * \param factor : fator de multiplicaçao
    */
    void mulDegMinSecs(int factor);

    /**
    * \brief Converte o objeto Dms para grau decimal
    * \return double: equivalente em grau decimal do objeto Dms
    */
    double dmsToDegreeDecimal();

    /**
    * \brief Converte o objeto Dms para radiano
    * \return double: equivalente em radinao do objeto Dms
    */
    double dmsToRadiano();

    /**
    * \brief Converte um valor em grau decimal para um objeto Dms
    * \param degreeDecimal : Valor em grau decimal
    * \return Dms : Objeto Dms a partir do valor grau decimal
    */
    Dms* degreeDecimalToDms(double degreeDecimal);

    /**
    * \brief Retorna um valor em grau decimal equivalente ao valor em radiano
    * \param degree : Valor em grau decimal
    * \return double: Valor em radiano
    */
    static double degreeDecimalToRadiano(double degreeDecimal);

    /**
    * \brief Retorna um ponteiro para um objeto Dms a partir de um valor radiano
    * \param radiano : Valor em radiano
    */
    Dms* radianoToDms(double radiano);

    /**
    * \brief Retorna um valor em grau decimal equivalente ao valor em radiano
    * \param radiano : Valor em radiano
    * \return double : Valor em grau decimal
    */
    static double radianoToDegreeDecimal(double radiano);

    /**
    * \brief Metodo para converter um valor em segundos em um objeto Dms
    * \param seconds : Total de segundos
    * \return Dms : ponteiro para objeto
    */
    static Dms * secondsToDms(double seconds);

    /**
    * \brief Metodo para comparar dois objetos Dms, se degMinSec for maior retorna 1, se for menor retorna -1, se for igual retorna 0
    * \param degMinSec : objeto a ser comparado
    * \return int : valor de comparaçao
    */
    int compareDegMinSecs(Dms *degMinSec);


protected:
    int degree;
    int min;
    double sec;
    bool signal;
    int secondsPrecision;
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // DMS_H
