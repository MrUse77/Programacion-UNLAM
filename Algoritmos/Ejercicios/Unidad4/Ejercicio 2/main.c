#include <lista_simple.h>
#include <stdio.h>
#include <string.h>
#define DATOS1 "datos1.csv"
#define DATOS2 "datos2.csv"

typedef struct {
	int clave;
	char prov[16];
	int dia;
	int mes;
	int anio;
	char nomProd[8];
} Prod;

void leer_datos1(list_t *l);
void printLista(list_t *l);
void leer_datos2(list_t *l);
int cmpProd(const void *dato, const void *param)
{
	int cmp = 0;
	if (((Prod *)dato)->anio != ((Prod *)param)->anio) {
		return ((Prod *)dato)->anio - ((Prod *)param)->anio;
	}
	if (((Prod *)dato)->mes != ((Prod *)param)->mes) {
		return ((Prod *)dato)->mes - ((Prod *)param)->mes;
	}
	if (((Prod *)dato)->dia != ((Prod *)param)->dia) {
		return ((Prod *)dato)->dia - ((Prod *)param)->dia;
	}
	if ((cmp = strcmp(((Prod *)dato)->prov, ((Prod *)param)->prov)) != 0) {
		return cmp;
	}
	return ((Prod *)dato)->clave - ((Prod *)param)->clave;
}
void saltar_linea(FILE *fp)
{
	int c;

	while ((c = fgetc(fp)) != EOF && c != '\n') {
	}
}
void print(void *dato, void *param)
{
	Prod *aux = (Prod *)dato;
	printf("%d,%s,%d-%d-%d,%s\n", aux->clave, aux->prov, aux->anio,
	       aux->mes, aux->dia, aux->nomProd);
}

int main()
{
	list_t l;
	list_init(&l);
	leer_datos1(&l);
	printLista(&l);
	leer_datos2(&l);
	list_order(&l, MERGE, cmpProd);
	printf("\n\n");
	printLista(&l);
}

void printLista(list_t *l)
{
	list_foreach(l, NULL, print);
}
void leer_datos1(list_t *l)
{
	Prod p;
	char linea[1000];
	FILE *f = fopen(DATOS1, "rt");
	if (!f) {
		return;
	}
	while (fgets(linea, 45, f)) {
		sscanf(linea, "%d,%15[^,],%4d-%2d-%2d,%7[^\n]", &p.clave,
		       p.prov, &p.anio, &p.mes, &p.dia, p.nomProd);
		list_push_in_order(l, &p, sizeof(Prod), cmpProd);
	}
}
void leer_datos2(list_t *l)
{
	Prod p;
	char linea[1000];
	FILE *f = fopen(DATOS2, "rt");
	if (!f) {
		return;
	}
	while (fgets(linea, 45, f)) {
		sscanf(linea, "%d,%15[^,],%4d-%2d-%2d,%7[^\n]", &p.clave,
		       p.prov, &p.anio, &p.mes, &p.dia, p.nomProd);
		list_push_first(l, &p, sizeof(Prod));
	}
}
