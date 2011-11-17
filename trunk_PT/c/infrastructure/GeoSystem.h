/*#ifndef GEOSYSTEM_H
#define GEOSYSTEM_H

#include <string>
#include <iostream>
#include <sstream>

enum GeoSystemEnum { CORREGOALEGRE=0, SAD69, SIRGAS2000, WGS84};
using namespace std;
class GeoSystem
{
private:
        double a;//eixoMenor
        double b;//eixoMaior;
        double f;// foco
        string systemName;//nome do sistema
public:
        GeoSystem(GeoSystemEnum system=WGS84);
        GeoSystem(double a, double f, string name);
		GeoSystem(string sysName);
        double getA();
        double getB();
        double getF();
        double getFirstEccentricity();
        double getSecondEccentricity();
        string getSystemName();

        void setEixoMenor(double newA);
        void setEixoMaior(double newB);
        void setFoco(double newF);
        void setSystemName(string name);
        void setSystem(GeoSystemEnum system);
        void setSystem(double a, double f, string newName);
        string toString();
};

#endif // GEOSYSTEM_H
*/
#ifndef GEOSYSTEM_H
#define GEOSYSTEM_H

#include <string>
#include <iostream>
#include <sstream>

/*
namespace br {
namespace uerj {
namespace eng {
namespace efoto {
*/
enum GeoSystemEnum { CORREGOALEGRE=0, SAD69, SIRGAS2000, WGS84};
using namespace std;

/**
* \file GeoSystem.h
* \class GeoSystem
* \brief Classe que cria um objeto sistema geodesico, arbitrario ou um existente a partir de um GeoSystemEnum
* \copyright E-Foto group
* \author Paulo Andre Batista Pupim
*/
class GeoSystem
{
private:
		double a;//eixoMenor
		double b;//eixoMaior;
		double f;// achatamento
		string systemName;//nome do sistema
public:
		/**
		* \brief Construtor a partir de um enumerador
		* \param system : Enumerador do sistema geodesico
		*/
		GeoSystem(GeoSystemEnum system=WGS84);

		/**
		* \brief Construtor com as informaççoes basicas de um sistema
		* \param a: semi-eixo maior do elipsoide
		* \param f: achatamento do elipsoide
		* \param name: nome do sistema geodesico
		*/
		GeoSystem(double a, double f, string name);

		/**
		* \brief Construtor com o nome do sistema geodesico
		* \param sysName : nome do sistema geodesico
		* \attention Se o \param sysName for inexistente o objeto tera valores incorretos, use esse construtor com cuidado
		*/
		GeoSystem(string sysName);

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
		string getSystemName();

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
		void setSystemName(string name);

		/**
		* \brief Altera o objeto para o sistema geodesico do enumerador
		* \param system : Valor do enumerador;
		*/
		void setSystem(GeoSystemEnum system);

		/**
		* \brief Altera os atributos dos sistemas, ou seja, altera o sistema
		* \param a: semi-eixo maior do elipsoide
		* \param f: achatamento do elipsoide
		* \param newName: nome do sistema geodesico
		*/
		void setSystem(double a, double f, string newName);

		/**
		* \brief Retorna umm representaçao textual do objeto, ou seja, seus principais atributos em forma de texto
		* \return string : Texto com os principais atributos do objeto, a saber, semi-eixo maior, menor e achatamento do elipsoide
		*/
		string toString();
};
/*
} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
*/
#endif // GEOSYSTEM_H
