#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../TDA/vector.h"

int main(int argc, char *argv[])
{
	int tam = atoi(argv[1]);
	srand(time(NULL));
	Vector v;
	vectorCrear(&v);
	for (int i = 0; i < tam; i++) {
		vectorInsertar(&v, rand() % 1000);
	}
	/*
	while (vectorCE(&v) > 0) {
		vectorEliminarDePosicion(&v, 0);
	}*/

	//vectorMostrar(&v);
	vectorDestruir(&v);
	return 0;
}
