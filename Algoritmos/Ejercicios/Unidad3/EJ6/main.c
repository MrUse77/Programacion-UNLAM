#include <stdio.h>
#include <stdlib.h>
#include <Cola.h>
#define MOVIMIENTOS "movimientos.dat"

typedef struct {
	int codigoProducto;
	int cantidad;
} Movimiento;

void crearMovimientosEjemplo(const char *nombreArchivo)
{
	Movimiento lote[] = { { 101, 30 }, { 102, 60 }, { 102, 50 },
			      { 103, 20 }, { 103, 10 }, { 104, 120 } };
	int i = 0;
	FILE *f = fopen(nombreArchivo, "wb");
	if (f == NULL) {
		return;
	}

	for (i = 0; i < 6; i++) {
		Movimiento aux = lote[i];

		fwrite(&aux, sizeof(Movimiento), 1, f);
	}
	fclose(f);
}

int cargarMovimientos(const char *nombreArchivo, int maxLote)
{
	FILE *f = fopen(nombreArchivo, "rb"),
	     *mayor = fopen("mayoristas.dat", "wb"),
	     *minor = fopen("minoristas.dat", "wb"), *destino = NULL;
	if (!f || !mayor || !minor) {
		return -1;
	}
	Movimiento m, mAux;
	t_Cola cAux;
	crearCola(&cAux);
	int code;
	fread(&m, sizeof(Movimiento), 1, f);
	while (!feof(f)) {
		int total = 0;
		int codeAux = m.codigoProducto;
		while (codeAux == m.codigoProducto && !feof(f)) {
			code = ponerEnCola(&cAux, &m, sizeof(Movimiento));
			if (code != OK) {
				return code;
			}
			total += m.cantidad;
			fread(&m, sizeof(Movimiento), 1, f);
		}
		if (total >= maxLote) {
			destino = mayor;
		} else {
			destino = minor;
		}
		while (!colaVacia(&cAux)) {
			code = sacarDeCola(&cAux, &mAux, sizeof(Movimiento));
			if (code != OK) {
				return code;
			}
			fwrite(&mAux, sizeof(Movimiento), 1, destino);
		}
	}

	fclose(f);
	fclose(mayor);
	fclose(minor);
	return OK;
}
int verCantidades(char *nombArchivo)
{
	FILE *f = fopen(nombArchivo, "rb");
	if (!f) {
		return -1;
	}
	Movimiento m;
	printf("Codigo \t cantidad \n");
	fread(&m, sizeof(Movimiento), 1, f);
	while (!feof(f)) {
		printf("%d \t %d \n", m.codigoProducto, m.cantidad);
		fread(&m, sizeof(Movimiento), 1, f);
	}
	return OK;
}

int main()
{
	crearMovimientosEjemplo(MOVIMIENTOS);
	cargarMovimientos(MOVIMIENTOS, 100);
	printf("Mayoristas \n");
	verCantidades("mayoristas.dat");
	printf("Minoristas \n");
	verCantidades("minoristas.dat");

	return 0;
}
