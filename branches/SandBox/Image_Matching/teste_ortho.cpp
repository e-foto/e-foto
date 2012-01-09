#include "Orthorectification.h"

int main(void)
{
	Orthorectification ortho(680332, 7464243, 682292, 7465952, 1.0, 1.0);

	ortho.getOrthoimage().set(1,1,4); // Read only - Ok
//	ortho.setOrthoimagePixel(1,1,4);

	ortho.printData();

	return 1;
}
