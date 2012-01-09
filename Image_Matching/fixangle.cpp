#include <stdio.h>
#include <math.h>

double fixAngle(double angle)
{
	bool negative = angle < 0.0;

	// Positive signal
	angle = fabs(angle);

	// Bring angle to the first lap
	if (angle > 2*M_PI)
	{
		int laps = floor(angle/(2*M_PI));
		angle = angle - laps*(2*M_PI);
	}

	if (negative)
		angle = 2*M_PI - angle;

	return angle;
}

double angleToFirstQuadrant(double angle)
{
	// Fix angle
	angle = fixAngle(angle);

	// Bring angle to the fisrt quadrant
	if (angle > M_PI/2.0 && angle <= M_PI)
		angle = M_PI - angle;

	if (angle > M_PI && angle <= 3.0*M_PI/2.0)
		angle = angle - M_PI;

	if (angle > 3.0*M_PI/2.0)
		angle = 2*M_PI - angle;

	return angle;
}

double getAngleBetweenImages(double X1, double Y1, double X2, double Y2)
{
	double delta_X = X2 - X1;
	double delta_Y = Y2 - Y1;

	if (delta_X - 0.0 < 0.0000000000001)
	{
		if (delta_Y > 0.0)
			return M_PI/2.0;
		else
			return 3*M_PI/2.0;
	}

	return fixAngle(atan(delta_Y/delta_X));
}

bool checkAnglesAlligned(double angle1, double angle2, double tolerance)
{
	angle1 = fixAngle(angle1);
	angle2 = fixAngle(angle2);

	double distance = fabs(angle1 - angle2);
        
        if (distance > M_PI)
            distance = 2*M_PI - distance;

        return (distance < tolerance || fabs(M_PI - distance) < tolerance);
}

int main(void)
{
	printf("Angle 14.0 fixed: %f\n",fixAngle(14.0));
	printf("Angle -14.0 fixed: %f\n",fixAngle(-14.0));
	printf("Angle 2.0 to first quadrant: %f\n",angleToFirstQuadrant(2.0));
	printf("Angle 3.5 to first quadrant: %f\n",angleToFirstQuadrant(3.5));
	printf("Angle 5.0 to first quadrant: %f\n",angleToFirstQuadrant(5.0));
	printf("Collinear angles: 0.0 and 3.20: %d\n",checkAnglesAlligned(0.0,3.20,0.1745));
	printf("Collinear angles: 0.0 and 0.16: %d\n",checkAnglesAlligned(0.0,0.16,0.1745));
	printf("Collinear angles: 0.0 and 1.57: %d\n",checkAnglesAlligned(0.0,1.57,0.1745));
	printf("Collinear angles: 0.0 and 5.0: %d\n",checkAnglesAlligned(0.0,5.0,0.1745));

	// 10 graus = 0,174532925

	return 1;
}
