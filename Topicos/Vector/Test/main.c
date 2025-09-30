#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../TDA/vector.h"

#define CODIGO_TAM 21
#define DESC_TAM 31
#define CLASIFICADOR_TAM 31
#define VALOR_TAM 17
#define REGION_TAM 10
#define PERIODO_TAM 30
typedef struct {
	char codigo[CODIGO_TAM];
	char descripcion[DESC_TAM];
	char clasificador[CLASIFICADOR_TAM];
	char indice_icc[VALOR_TAM];
	char var_mensual[VALOR_TAM];
	char var_interanual[VALOR_TAM];
	char region[REGION_TAM];
	char periodo[PERIODO_TAM];
} Registro;
int comp(const void *a, const void *b);
void formato(const void *reg);
void quitarComillar(void *elem);
void formatear(char *elem, void *reg);
void formatearATXT(FILE *f, void *reg);
size_t longitudString(const char *str)
{
	const char *letraActual = str;

	while (*letraActual != '\0')
		letraActual++;

	return letraActual - str;
}
char *buscarEnStringReversa(const char *str, char c)
{
	size_t longStr = longitudString(str);
	char *letraActual = (char *)str + longStr - 1;

	while (letraActual >= str && *letraActual != c)
		letraActual--;

	if (*letraActual == c)
		return letraActual;

	return NULL;
}

int main(int argc, char *argv[])
{
	//	srand(time(NULL));
	int cod;
	FILE *f = fopen("serie_ipc_divisiones.csv", "rt");
	if (!f) {
		perror("Error al abrir el archivo");
		return ERR;
	}
	Vector v;
	vectorCrear(&v, sizeof(Registro));
	cod = vectorInsertarDeArchivoTXT(&v, f, formatear, 5);
	FILE *f2 = fopen("serie_ipc_divisiones_out.csv", "wt");
	if (!f2) {
		perror("Error al abrir el archivo");
		vectorDestruir(&v);
		fclose(f);
		return ERR;
	}
	cod = vectorGuardarAArchivoTXT(&v, f2, formatearATXT);
	/*
	for (int i = 0; i < 100000; i++) {
		int random = rand() % 100000;
		vectorInsertar(&v, &random);
	}

	puts("\n");
	int init = time(NULL);
	qsort(v.vec, v.cantElem, v.tamElem, &comp); // La funcion qsort de la libreria estandar
	//vectorOrdenar(&v, BURBUJEO);
	int end = time(NULL);
	printf("Tiempo de ordenacion: %d segundos\n", end - init);
	*/

	//vectorMostrar(&v, formato);
	fclose(f);
	fclose(f2);
	vectorDestruir(&v);
	return cod;
}

void formatear(char *elem, void *elemSal)
{
	Registro *reg = (Registro *)elemSal;

	quitarComillar(elem);
	char *act;
	act = buscarEnStringReversa(elem, '\n');
	*act = '\0';
	act = buscarEnStringReversa(elem, ';');
	sscanf(act + 1, "%s", reg->periodo);
	*act = '\0';
	act = buscarEnStringReversa(elem, ';');
	sscanf(act + 1, "%s", reg->region);
	*act = '\0';
	act = buscarEnStringReversa(elem, ';');
	sscanf(act + 1, "%s", reg->var_interanual);
	*act = '\0';
	act = buscarEnStringReversa(elem, ';');
	sscanf(act + 1, "%s", reg->var_mensual);
	*act = '\0';
	act = buscarEnStringReversa(elem, ';');
	sscanf(act + 1, "%s", reg->indice_icc);
	*act = '\0';
	act = buscarEnStringReversa(elem, ';');
	sscanf(act + 1, "%s", reg->clasificador);
	*act = '\0';
	act = buscarEnStringReversa(elem, ';');
	sscanf(act + 1, "%s", reg->descripcion);
	*act = '\0';
	sscanf(elem, "%s", reg->codigo);
}
void quitarComillar(void *elem)
{
	char *str = (char *)elem;
	char *src = str, *dst = str;
	while (*src) {
		if (*src != '"') {
			*dst++ = *src;
		}
		src++;
	}
	*dst = '\0';
}
void formato(const void *elem)
{
	Registro *reg = (Registro *)elem;
	printf("%s %s %s %s %s %s %s %s\n", reg->codigo, reg->descripcion,
	       reg->clasificador, reg->indice_icc, reg->var_mensual,
	       reg->var_interanual, reg->region, reg->periodo);
}
int comp(const void *a, const void *b)
{
	return *(int *)a - *(int *)b;
}
void formatearATXT(FILE *f, void *reg)
{
	Registro r = *(Registro *)reg;
	fprintf(f, "\"%s\";\"%s\";\"%s\";%s;%s;%s;%s;%s\n", r.codigo,
		r.descripcion, r.clasificador, r.indice_icc, r.var_mensual,
		r.var_interanual, r.region, r.periodo);
}
