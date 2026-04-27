#include <stdio.h>
#include <stdlib.h>
#include "include/vector.h"
#include "Pila.h"

typedef struct {
	int num;
	char h;
} Ejemplo;

int cmpEj(const void *c1, const void *c2);
int cmpEl(const void *c1, const void *c2);

int main()
{
t_Pila p;
crearPila(&p);
/*
	Vector v;
	crearVector(&v, sizeof(Ejemplo));
	char a = 'a';
	int i = 0;
	Ejemplo e = { i, a };
	for (; i < 5; i++) {
		vectorOrdInsertar(&v, &e, cmpEj);
		a++;

		printf("Ejemplo: %d %c\n", e.num, e.h);
	}
	//	e = *(Ejemplo *)vectorBuscar(&v, 5);
	//	printf("Buscado: %d %c", e.num, e.h);
	vectorEliminar(&v, &e, cmpEl);
	bool lleno = vectorLleno(&v);
	printf("Vector: %b\n", lleno);
	vectorVaciar(&v);
	bool vacio = vectorVacio(&v);
	printf("Vector: %b\n", vacio);
	vectorDestruir(&v);
	*/
	return 0;
}

int cmpEl(const void *c1, const void *c2)
{
	Ejemplo e1 = *(Ejemplo *)c1;
	Ejemplo e2 = *(Ejemplo *)c2;
	return e1.num - e2.num;
}
int cmpEj(const void *c1, const void *c2)
{
	Ejemplo e1 = *(Ejemplo *)c1;
	Ejemplo e2 = *(Ejemplo *)c2;

	return e1.num - e2.num;
}
