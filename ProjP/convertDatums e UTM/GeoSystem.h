#ifndef GEOSYSTEM_H
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

        double getA();
        double getB();
        double getF();
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
