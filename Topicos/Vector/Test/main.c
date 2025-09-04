#include <stdio.h>
#include <stdlib.h>
#include "../TDA/vector.h"

int main()
{
	Vector v;
	vectorCrear(&v);
	vectorInsertar(&v, 12);

	vectorInsertar(&v, 123);
	vectorInsertar(&v, 1123);
	vectorInsertar(&v, 121894);
	vectorInsertar(&v, -1);
	vectorInsertar(&v, 238746281);

	vectorMostrar(&v);
	vectorEliminar(&v, 123);
	vectorMostrar(&v);
	return 0;
}
