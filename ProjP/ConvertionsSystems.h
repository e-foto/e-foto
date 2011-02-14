#ifndef CONVERTIONSSYSTEMS_H
#define CONVERTIONSSYSTEMS_H

#include "Matrix.h"

enum GeoSystem { CORREGOALEGRE, SAD69, SIRGAS2000, WGS84};

class ConvertionsSystems
{
private:
        double lambda;
        double phi;
        double haltura;
        double E, N, H;

public :
        ConvertionsSystems();
        static Matrix GeoelipToGeocentricCartesian(double phi, double lambda, double haltura, GeoSystem system=SIRGAS2000);
        static Matrix GeocentricCartesianToGeoelip(double Xi, double Yi, double Zi, GeoSystem system=SIRGAS2000);

        Matrix geoElipToUtm(double phi, double lambda, double lambda0, double haltura);
        Matrix uTmToGeo(double x, double y, double haltura,double lambda0);
        Matrix getMatrixR(double phi, double lambda);

};


#endif // CONVERTIONSSYSTEMS_H
