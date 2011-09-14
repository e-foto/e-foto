#include <stdio.h>

int main(void)
{
	unsigned int a=5, b=5, c;
	c = a-b;

	if (a-b > a)
		printf("Numero negativo\n");
	else
		printf("Numero positivo\n");

	printf("A-B = %d\n",c);

	return 1;
}
