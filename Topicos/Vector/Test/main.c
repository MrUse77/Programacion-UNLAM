#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../TDA/vector.h"

int main(int argc, char *argv[])
{
	srand(time(NULL));
	Vector v;
	vectorCrear(&v, sizeof(int));
	for (int i = 0; i < 10; i++) {
		int random = rand() % 1000;
		vectorInsertarAlInicio(&v, &random);
	}
	vectorMostrar(&v);
	puts("\n");
	vectorEliminarDePosicion(&v, 0);

	vectorMostrar(&v);
	vectorDestruir(&v);
	return 0;
}
