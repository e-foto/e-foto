#ifndef COMMONDEF_H
#define COMMONDEF_H

#ifdef WIN64
#define _USE_MATH_DEFINES // for C++
#include <cmath>
#endif
#ifdef unix
#include <cmath>
#endif

typedef enum { CORREGOALEGRE=0, SIRGAS2000, SAD69, WGS84} DatumType;

#endif // COMMONDEF_H
