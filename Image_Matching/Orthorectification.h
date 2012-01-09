/*******************************************************************************
                              Orthorectification.h
*******************************************************************************/

#ifndef ORTHORECTIFICATION_H
#define ORTHORECTIFICATION_H

#include <stdio.h>
#include <iostream>
#include <fstream>
#include "Matrix.h"

/**
* class Orthorectification
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 24/11/2011
* @version 1.0 - Marcelo Teixeira Silveira
*/

class Orthorectification
{
private:
	Matrix orthoimage;
	double Xi, Yi, Xf, Yf, res_x, res_y;
	int ortho_width, ortho_height;
	void saveOrthoEfoto(char * filename);
	void loadOrthoEfoto(char * filename);

public:
	Orthorectification(double, double, double, double, double, double);
	void createNewGrid(double, double, double, double, double, double);
	void changeGridResolution(double, double);
	void saveOrtho(char *, int);
	void loadOrtho(char *, int);
	void printData();
	Matrix getOrthoimage() { return orthoimage; }; // Read only
	int getWidth() { return ortho_width; };
	int getHeight() { return ortho_height; };
	double getOrthoimagePixel(int, int);
	void setOrthoimagePixel(int, int, double);
	void getXYAt(int, int, double &, double &);

};

#endif
