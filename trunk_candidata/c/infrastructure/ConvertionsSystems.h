#ifndef CONVERTIONSSYSTEMS_H
#define CONVERTIONSSYSTEMS_H

#include "Matrix.h"
#include "Dms.h"
#include "GeoSystem.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

	/**
	* \file ConvertionsSystems.h
	* \class ConvertionsSystems
	* \brief Classe que trata das conversoes entre sistemas geodesicos, e projeçoes, uma especie de proj4 bastante simplificada
	* \copyright E-Foto group
	* \author Paulo Andre Batista Pupim
	*/
class ConvertionsSystems
{
public :

	/**
	* \brief Metodo que converte uma coordenada GeoElipsoidica para GeoCentrica
	* \param phi     : latitude em radianos
	* \param lambda  : longitude em radianos
	* \param altura  : altitude geodesica em metros
	* \param system  : Sistema geodesico
	* \return Matrix : Matriz[1x3] com os valores na sequencia X,Y,Z
	*/
	static Matrix GeoelipToGeocentricCartesian(double phi, double lambda, double altura,GeoSystem system);


	/**
	* \brief Metodo que converte uma coordenada GeoCentrica para GeoElipsoidica
	* \param Xi : Valor de X da Coordenada
	* \param Yi : Valor de Y da Coordenada
	* \param Zi : Valor de Z da Coordenada
	* \param system  : Sistema geodesico
	* \return Matrix : Matriz[1x3] com os valores, em radiano, na sequencia latitude, longitude, altura em metros
	*/
	static Matrix GeocentricCartesianToGeoelip(double Xi, double Yi, double Zi,GeoSystem system);

	/**
	* \brief Metodo que converte uma coordenada de um sistema para outro pelo metodo dos 3 parametros
	* \param oldSystem : Sistema de entrada
	* \param newSystem : Sistema de saida
	* \param coordinates  : Coordenadas geoElipsoidicas,com os valores, em radiano, na sequencia latitude, longitude, altura em metros
	* \return Matrix : Matriz[1x3] com os valores, em radiano, na sequencia latitude, longitude, altura em metros
	*/
	static Matrix convertSystems3Parameters(GeoSystem oldSystem, GeoSystem newSystem, Matrix coordinates);

	/**
	* \brief Metodo que converte uma coordenada de um sistema para outro pelas formulas simplificadas de Molodensky
	* \param oldSystem : Sistema de entrada
	* \param newSystem : Sistema de saida
	* \param phi     : Latitude
	* \param lambda  : Longitude
	* \param altura  : Altitude geodesica
	* \return Matrix : Matriz[1x3] com os valores, em radiano, na sequencia latitude, longitude, altura em metros
	*/
	static Matrix convertSystemsSimplifiedMolodensky(GeoSystem oldSystem, GeoSystem newSystem, double phi, double lambda, double altura);

	/**
	* \brief Metodo que converte uma coordenada GeoElipsoidica para UTM
	* \param phi     : Latitude
	* \param lambda  : Longitude
	* \param system  : Sistema geodesico
    * \return Matrix : Matriz[1x4] com os valores, na sequencia E, N,zona, hemisferio: -1=Sul('S') ou +1=Norte('N')
	*/
	static Matrix geoToUtm(double phi, double lambda, GeoSystem system);//,char hemi, char side);

	/**
	* \brief Metodo que converte uma coordenada UTM para GeoElipsoidica
	* \param E     : Valor de E da coordenada
	* \param N     : Valor de N da coordenada
	* \param zona  : Zona na qual esta a coordenada
    * \param hemi  : Hemisferio no qual se encontra a coordenada, a saber, -1=Sul('S') ou +1=Norte('N')
	* \param system : Sistema geodesico
	* \return Matrix : Matriz[1x3] com os valores, em radiano, latitude , longitude e Meridiano Central em graus
	*/
    static Matrix utmToGeo(double E, double N, int zona,int hemi ,GeoSystem system);

	/**
	* \brief Metodo que retorna os parametros de conversao entre sistemas geodesicos
	* \param oldSystem : Sistema geodesico de entrada
	* \param newSystem : Sistema geodesico de saida
	* \return Matrix : Matriz[1x3] com os paramteros de conversao na sequencia dE, dN e dH
	*/
	static Matrix transformParameters(GeoSystem oldSystem, GeoSystem newSystem);

	/**
	* \brief Metodo que converte coordenadas UTM emm coordenadas Geocentricas
	* \param E     : Valor de E da coordenada
	* \param N     : Valor de N da coordenada
	* \param H     : Valor de H da coordenada
	* \param zona  : Zona na qual esta a coordenada
	* \param hemi  : Hemisferio no qual se encontra a coordenada, a saber, -1=Sul('S') ou +1=Norte('N')
	* \param system : Sistema geodesico
	* \return Matrix : Matrix[1x3] com as coordenadas geocentricas, na sequencia : X, Y, Z
	*/
	static Matrix utmToGeocentrica(double E, double N,double H, int zona, char hemi, GeoSystem system);

	/**
	* \brief Metodo que converte uma coordenada GeoCentrica para coordenadas UTM
	* \param X : Valor de X da Coordenada
	* \param Y : Valor de Y da Coordenada
	* \param Z : Valor de Z da Coordenada
	* \param system  : Sistema geodesico
	* \return Matrix : Matriz[1x5] com os valores, em radiano, na sequencia latitude, longitude, altura em metros, zona e hemisferio, onde hemisferio obedece -1=Sul('S') ou +1=Norte('N')
	*/
	static Matrix GeocentricaToUtm(double X, double Y, double Z, GeoSystem system);

    /**
    * \brief Metodo auxiliar para tuncar numeros na n-esima casa decimal
    * \param value : Valor a ser arrendondado
    * \param decimal : Numero de casas decimais do resultado
    * \return double : Valor truncado
    */
    static double truncate(double value, int decimal);

	static Matrix utmToNunes(double E, double N, double H, int zona, int hemi, double phi0, double lambda0, double h0, GeoSystem system);
	static Matrix nunesToUtm(double X, double Y, double Z, double phi0, double lambda0, double h0, GeoSystem system);

    //E o Lambda nao serve para nada??
    static double getNunesRaio(double phi0, double lambda0, GeoSystem sys);



};


} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // CONVERTIONSSYSTEMS_H
