#ifndef COMMONDEF_H
#define COMMONDEF_H

#ifdef WIN64
#define _USE_MATH_DEFINES // for C++
#include <cmath>
#endif
#ifdef unix
#include <cmath>
#endif

typedef enum { CORREGOALEGRE=0, SAD69, SIRGAS2000, WGS84} DatumType;

#define M_PI 3.14159265359

#endif // COMMONDEF_H
