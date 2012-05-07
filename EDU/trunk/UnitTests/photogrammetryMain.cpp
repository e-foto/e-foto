/*
Pronto:

ProjectHeader.h
Terrain.h
Flight.h
AnalogFiductialMark.h
DigitalFiductialMark.h
AnalogImageSpaceCoordinate.h
DigitalImageSpaceCoordinate.h
ObjectSpaceCoordinate.h



Em processo:

Point.h
CheckingPoint.h
ControlPoint.h
PhotogrammetricPoint.h

Sensor.h
FrameSensor.h
SensorWithFiducialMarks.h
SensorWithKnowDimensions.h
SensorWithKnowParameters.h

InteriorOrientation.h
ExteriorOrientation.h
IOQuality.h
EOQuality.h



Fazer:

Image.h // Precisa receber merge com o que o Paulo fez.
BundleAdjustment.h // Fica a cargo do Paulo.
SpatialRessection.h // Precisa receber merge com o que o Rafael fez.
ProjectiveRay.h // Fica a cargo do Rafael.

RayTester.h // Três classes novas implementadas para o motor pelo Rafael.
SpatialIntersection.h
StereoPair.h

DemGrid.h // Três classes novas implementadas para o dem pelo Marcelo.
MatchingPoints.h
MatchingPointsGrid.h

*/




#include "Matrix.h"
#include <QDebug>
int main( int argc, char ** argv )
{
	br::uerj::eng::efoto::Matrix m1(3,3);
	for (int i = 1; i <= 3; i++)
	{
		for (int j = 1; j <= 3; j++)
		{
			m1.set(i,j, rand()*255);
		}
	}
	br::uerj::eng::efoto::Matrix m2 = m1;
	m1 = m1.inverse();
	m1.show('g', 8, "m1");
	m2 = m2.inverse2();
	m2.show('g', 8, "m2");
	(m1 - m2).show('g', 8, "m1 - m2");
}
