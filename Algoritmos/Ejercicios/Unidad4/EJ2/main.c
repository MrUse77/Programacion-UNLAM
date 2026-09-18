#include <lista_simple.h>
#include <stdio.h>
#define DATOS1 "datos1.csv"
#define DATOS2 "datos2.csv"

typedef struct {
	int clave;
	char prov[15];
	int dia;
	int mes;
	int anio;
	char nomProd[8];
} Prod;

void saltar_linea(FILE *fp)
{
	int c;

	while ((c = fgetc(fp)) != EOF && c != '\n') {
	}
}

int main()
{
	list_t l;
	list_init(&l);
}

void leer_datos1(list_t *l)
{
	Prod p;
	FILE *f = fopen(DATOS1, "wt");
	if (!f) {
		return;
	}
	saltar_linea(f);
	fprintf(f, "%d,%s,%d-%d-%d,%s", p.clave, p.prov, p.dia, p.mes, p.anio,
		p.nomProd);
}
