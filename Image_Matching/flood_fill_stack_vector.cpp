/*
 * Flood-fill using stack
 */

// http://en.wikipedia.org/wiki/Flood_fill
// http://www.student.kuleuven.be/~m0216922/CG/floodfill.html#Recursive_Scanline_Floodfill_Algorithm

#include <stdio.h>
#include "Matrix.h"

#define stackSize 0xFFFF

class stackCell
{
public:
	int coord;
	stackCell *prev;
};

stackCell *stack, *aux;
Matrix I(10,10), map;
int img_width, img_height;

using namespace std;

bool pop(int &x, int &y)
{
	if(stack != NULL)
	{
		int p = stack->coord;
		x = p / img_height;
		y = p % img_height;
		aux = stack;
		stack = stack->prev;
		delete aux;
		return 1;
	}    
	else
	{
		return 0;
	}
}   
 
bool push(int x, int y)
{
	aux = stack;
	stack = new stackCell();
	stack->coord = img_height * x + y;
	stack->prev = aux;
	return 1;
}    

void emptyStack()
{
    int x, y;
    while(pop(y,x));
}

void flood_fill(int x, int y, unsigned char val, unsigned char newval)
{
	if(val == newval)
		return;

	emptyStack();

	if(!push(y,x))
		return; 

	int i,j;

	while(pop(y,x))
	{
		i = y+1;
		j = x+1;

		if (map.get(i,j) != 0 || I.get(i,j) != val)
			continue;

        	I.set(i,j,newval);
		map.set(i,j,1.0); // Visited

		// Add 4-neighbor pixels
	        if(x + 1 < img_width)
	        {
			if (!push(y, x+1))
				return;
	        }    
	        if(x - 1 >= 0)
	        {
			if (!push(y, x-1))
				return;
	        }    
	        if(y + 1 < img_height)
	        {
			if (!push(y+1, x))
				return;
		}    
	        if(y - 1 >= 0)
	        {
			if (!push(y-1, x))
				return;
        	}
	}
}

int main(void)
{
	img_width = I.getCols();
	img_height = I.getRows();

	stack = NULL;

	// Create image pattern
	I.set(1,1,0);  I.set(1,2,0);  I.set(1,3,0);  I.set(1,4,0);  I.set(1,5,0);  I.set(1,6,0);  I.set(1,7,0);  I.set(1,8,0);  I.set(1,9,0);  I.set(1,10,0);
	I.set(2,1,0);  I.set(2,2,0);  I.set(2,3,0);  I.set(2,4,0);  I.set(2,5,0);  I.set(2,6,0);  I.set(2,7,0);  I.set(2,8,0);  I.set(2,9,0);  I.set(2,10,0);
	I.set(3,1,0);  I.set(3,2,0);  I.set(3,3,0);  I.set(3,4,0);  I.set(3,5,1);  I.set(3,6,1);  I.set(3,7,1);  I.set(3,8,0);  I.set(3,9,0);  I.set(3,10,0);
	I.set(4,1,0);  I.set(4,2,0);  I.set(4,3,0);  I.set(4,4,1);  I.set(4,5,0);  I.set(4,6,0);  I.set(4,7,0);  I.set(4,8,1);  I.set(4,9,0);  I.set(4,10,0);
	I.set(5,1,0);  I.set(5,2,0);  I.set(5,3,1);  I.set(5,4,0);  I.set(5,5,0);  I.set(5,6,0);  I.set(5,7,0);  I.set(5,8,0);  I.set(5,9,1);  I.set(5,10,0);
	I.set(6,1,0);  I.set(6,2,0);  I.set(6,3,1);  I.set(6,4,0);  I.set(6,5,0);  I.set(6,6,0);  I.set(6,7,0);  I.set(6,8,0);  I.set(6,9,1);  I.set(6,10,0);
	I.set(7,1,0);  I.set(7,2,0);  I.set(7,3,1);  I.set(7,4,0);  I.set(7,5,0);  I.set(7,6,0);  I.set(7,7,0);  I.set(7,8,0);  I.set(7,9,1);  I.set(7,10,0);
	I.set(8,1,0);  I.set(8,2,0);  I.set(8,3,0);  I.set(8,4,1);  I.set(8,5,0);  I.set(8,6,0);  I.set(8,7,0);  I.set(8,8,1);  I.set(8,9,0);  I.set(8,10,0);
	I.set(9,1,0);  I.set(9,2,0);  I.set(9,3,0);  I.set(9,4,0);  I.set(9,5,1);  I.set(9,6,1);  I.set(9,7,1);  I.set(9,8,0);  I.set(9,9,0);  I.set(9,10,0);
	I.set(10,1,0); I.set(10,2,0); I.set(10,3,0); I.set(10,4,0); I.set(10,5,0); I.set(10,6,0); I.set(10,7,0); I.set(10,8,0); I.set(10,9,0); I.set(10,10,0);

	map.resize(img_height,img_width);
	map.zero();

	printf("Before:\n");
	I.show();

	flood_fill(2,2,0,5);

	printf("After fill:\n");
	I.show();

	return 1;
}

