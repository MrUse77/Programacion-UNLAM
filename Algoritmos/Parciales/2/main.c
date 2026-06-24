#include "dron.h"
#include "queue.h"
#include <stdio.h>
#define NOM_ARCH_ORDENES "ordenes.txt"
#define NOM_ARCH_SALIDA "salida.txt"
void escribir_salida_estado(int cont, dron_t d)
{
	FILE *f = fopen(NOM_ARCH_SALIDA, "wt");
	if (!f) {
		perror("Error en crear archivo de salida");
		return;
	}
	fprintf(f, "%d", cont);
	fprintf(f, "%c", d.dir);
	fclose(f);
}

void escribir_salida_movimientos(queue_t *sec_mov, int cont)
{
	char aux;
	FILE *f = fopen(NOM_ARCH_SALIDA, "wt");
	if (!f) {
		perror("Error en crear archivo de salida");
		return;
	}
	fprintf(f, "%d\n", cont);
	while (!queue_is_empty(sec_mov)) {
		queue_pull(sec_mov, &aux, sizeof(char));
		fprintf(f, "%c", aux);
	}
	fclose(f);
}

int main()
{
	dron_t d;
	queue_t sec_mov;
	int cont;

	queue_init(&sec_mov);
	guardar_secuencia(NOM_ARCH_ORDENES, &d);
	cont = procesar_secuencia(&d, &sec_mov);
	if (cont == -1) {
		escribir_salida_estado(cont, d);
	} else {
		escribir_salida_movimientos(&sec_mov, cont);
	}

	return 0;
}
