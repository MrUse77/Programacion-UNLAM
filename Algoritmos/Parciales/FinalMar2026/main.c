#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cola.h"
#define PENDIENTES "pendientes.txt"
#define INEFICIENTES "despachos_ineficientes.txt"
#define OPERACIONES "operaciones.txt"
#define TAM_LINEA 20

typedef struct {
	char code[10];
	int dest;
	char tipo;
} operacion;

int despachar_paquetes(cola_t *c, int max, FILE *f, int destino)
{
	operacion aux;
	int p_despachados = 0;
	if (cola_vacia(c)) {
		fprintf(f, "DES|%d|VACIO\n", destino);
		return 1;
	}
	while (p_despachados < max && !cola_vacia(c)) {
		cola_sacar(c, &aux, sizeof(operacion));
		p_despachados++;
	}
	if (p_despachados < max) {
		fprintf(f, "DES|%d|INCOMPLETO|%d|%d\n", destino, max,
			p_despachados);
		return 1;
	}
	return 0;
}

int procesarLectura(FILE *f)
{
	int cant_max_camion, cant_dest, i;
	char linea[TAM_LINEA];
	cola_t *c;
	FILE *inef = fopen(INEFICIENTES, "wt"), *pendientes;
	if (!inef) {
		return -1;
	}
	pendientes = fopen(PENDIENTES, "wt");
	if (!pendientes) {
		fclose(inef);
		return -1;
	}
	fscanf(f, "%d,%d\n", &cant_max_camion, &cant_dest);
	c = malloc(sizeof(cola_t) * cant_dest);
	if (!c) {
		return -1;
	}
	for (i = 0; i < cant_dest; i++) {
		cola_iniciar(c + i);
	}
	while (fgets(linea, TAM_LINEA, f)) {
		operacion op;
		char *aux = linea;
		aux = strrchr(linea, '\n');
		*aux = '\0';
		aux = strchr(linea, '|');
		aux++;
		if (strncmp(linea, "ING", 3) == 0) {
			sscanf(aux, "%10s|%d|%c", op.code, &op.dest, &op.tipo);
			cola_insertar(c + op.dest - 1, &op, sizeof(operacion));
		} else if (strncmp(linea, "DES", 3) == 0) {
			despachar_paquetes(c + atoi(aux) - 1, cant_max_camion,
					   inef, atoi(aux));
		}

		puts(aux);
	}
	for (i = 0; i < cant_dest; i++) {
		operacion op;
		while (!cola_vacia(c + i)) {
			cola_sacar(c + i, &op, sizeof(operacion));
			fprintf(pendientes, "ING|%10s|%d|%c", op.code, op.dest,
				op.tipo);
		}
	}
	fclose(inef);
	return 0;
}

int main()
{
	FILE *f = fopen(OPERACIONES, "rt");
	if (!f) {
		return -1;
	}
	procesarLectura(f);
	fclose(f);
	return 0;
}
