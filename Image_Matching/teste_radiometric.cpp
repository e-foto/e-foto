#include <stdio.h>
#include "Matrix.h"
#include "RadiometricTransformation.h"

int main(void)
{
	Matrix A(2,2);

	A.set(1,1,4);
	A.set(1,2,5);
	A.set(2,1,7);
	A.set(2,2,9);

	printf("Original image:\n");
	A.show();

	RadiometricTransformation rt;

	rt.im2double(&A);
	printf("Double space:\n");
	A.show();

/*
	// imadjust
	rt.imadjust(&A);
	printf("\nAfter transformation:\n");
	A.show();
*/

/*
	// Histogram
	rt.imhist(&A);
*/

	rt.histeq(&A);
	printf("Histogram equalization:\n");
	A.show();

	return 1;
}
