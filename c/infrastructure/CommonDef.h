#ifndef COMMONDEF_H
#define COMMONDEF_H

#ifdef WIN32
#define _USE_MATH_DEFINES // for cmath M_PI availability
#endif
#include <cmath>

typedef enum { CORREGOALEGRE=0, SIRGAS2000, SAD69, WGS84} DatumType;

#endif // COMMONDEF_H
