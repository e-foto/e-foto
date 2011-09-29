#ifndef CONVERTIONSSYSTEMS_H
#define CONVERTIONSSYSTEMS_H

#include "Matrix.h"
#include "Dms.h"
#include "GeoSystem.h"

class ConvertionsSystems
{
public :
        ConvertionsSystems();
        GeoSystem systema;
        static Matrix GeoelipToGeocentricCartesian(double phi, double lambda, double haltura,GeoSystem system);
        static Matrix GeocentricCartesianToGeoelip(double Xi, double Yi, double Zi,GeoSystem system);

        Matrix convertSystems3Parameters(GeoSystem oldGeoSystem, GeoSystem newSystem, Matrix oldENH);

        static double* convertSystems3Parameters(GeoSystem oldSystem, GeoSystem newSystem, double phi, double lambda, double haltura);
		static double* convertSystemsFullMolodensky(GeoSystem oldSystem, GeoSystem newSystem, double phi, double lambda, double haltura);
		static double* convertSystemsSimplifiedMolodensky(GeoSystem oldSystem, GeoSystem newSystem, double phi, double lambda, double haltura);


		static Matrix geoToUtmFran(double phi, double lambda, double haltura, GeoSystem system);//,char hemi, char side);
		static Matrix utmToGeoFran(double E, double N, double haltura,int zona, GeoSystem system,char hemi);
		static Matrix utmToGeocentrica(double E, double N, double H, int zona, GeoSystem system, char hemi);

		static Matrix transformParameters(GeoSystem oldSystem, GeoSystem newSystem);

		void setSystem(GeoSystem newGeoSystem);





};


#endif // CONVERTIONSSYSTEMS_H
