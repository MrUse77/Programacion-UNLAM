#include <stdio.h>
#include <stdlib.h>
#include "include/vector.h"
void format(const char *linea, void *elemFmt);
void print(const void *cod);
int main()
{
	printf("hola mundo\n");
	Vector v;

	int cod = crearVectorDeArchivoTxtConCant(&v, "Ej3.txt", sizeof(int),
						 format);
	if (cod != OK) {
		return 0;
	}
	vectorMostrar(&v, print);
	vectorDestruir(&v);
	return 0;
}
void print(const void *v)
{
	printf("%d\n", *(int *)v);
}

void format(const char *linea, void *elemFmt)
{
	int *num = (int *)elemFmt;
	*num = atoi(linea);
}
