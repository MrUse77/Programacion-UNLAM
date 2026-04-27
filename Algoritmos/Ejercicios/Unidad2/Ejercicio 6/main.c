#include <stdio.h>
#include "Pila.h"
#include "Fecha.h"
#include <stdlib.h>
#include <string.h>

#define ERR_LLENO -1
#define ERR_FILE -2
#define FILE_URL "prod.bin"
#define TAM_LINEA 65

typedef struct {
	char code[7];
	char desc[15];
	char prov[15];
	Fecha Fcomp;
	Fecha venc;
	int cant;
	float precio_compra;
	float precio_venta;
} Prod;

int colocarProdEnPila_BIN(t_Pila *p);
void ordenar(t_Pila *p1, t_Pila *p2);
void printProd(Prod *p);
void mostrarPila(t_Pila *p2);

int main()
{
	t_Pila p1, p2;
	crearPila(&p1);
	colocarProdEnPila_BIN(&p1);
	crearPila(&p2);
	ordenar(&p1, &p2);
	//Solo muestra
	mostrarPila(&p2);

	return 0;
}

int colocarProdEnPila_BIN(t_Pila *p)
{
	Prod prod;
	FILE *f = fopen(FILE_URL, "rb");
	if (!f) {
		return ERR_FILE;
	}
	while (fread(&prod, sizeof(Prod), 1, f)) {
		apilar(p, &prod, sizeof(Prod));
	}
	fclose(f);
	return 1;
}

void ordenar(t_Pila *p1, t_Pila *p2)
{
	Prod aux, temp;
	while (pilaVacia(p1) != TRUE) {
		desapilar(p1, &temp, sizeof(Prod));
		while (pilaVacia(p2) != TRUE) {
			verTope(p2, &aux, sizeof(Prod));
			//Mayor a menor
			if (strcmp(aux.code, temp.code) > 0) {
				desapilar(p2, &aux, sizeof(Prod));
				apilar(p1, &aux, sizeof(Prod));
			} else {
				break;
			}
		}
		apilar(p2, &temp, sizeof(Prod));
	}
}

void mostrarPila(t_Pila *p2)
{
	Prod p;
	while (!pilaVacia(p2)) {
		desapilar(p2, &p, sizeof(Prod));
		printProd(&p);
	}
}

void printProd(Prod *p)
{
	printf("code: %s\tdesc: %s\tprov: %s \n", p->code, p->desc, p->prov);
}
