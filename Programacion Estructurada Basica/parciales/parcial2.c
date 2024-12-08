#include "myShortCuts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
	int code;
	char desc[31];
	char codeRubro[21];
	float price;
	int stock;
} Producto; // ordenado por codeRubro
typedef struct {
	int code;
	int cant;
	float total;
} Rubro;

void GenerarArchivos(FILE* f);
int BuscarProd(FILE* f, int code, Producto* p);
int main() {
	int code, cant, pos;
	Producto p;
	FILE* f = fopen("productos.dat", "r+b");
	do {
		printf("Ingrese el codigo del producto a buscar: ");
		scanf("%d", &code);
	} while (code < 0);
	while (code > 0) {
		do {
			printf("Ingrese la cantidad a agregar: ");
			scanf("%d", &cant);
		} while (cant < 0);
		pos = BuscarProd(f, code, &p);
		if (pos == -1) {
			printf("Producto no encontrado\n");
		}
		else {
			p.stock += cant;
			fseek(f, -sizeof(Producto), SEEK_CUR);
			fwrite(&p, sizeof(Producto), 1, f);
		}
		do {
			printf("Ingrese el codigo del producto a buscar: ");
			scanf("%d", &code);
		} while (code < 0);
	}
	GenerarArchivos(f);
	fclose(f);
	return 0;
}
int BuscarProd(FILE* f, int code, Producto* p) {
	int pos = -1, i = 0;
	rewind(f);
	fread(p, sizeof(Producto), 1, f);
	while (!feof(f) && p->code < code) {
		pos = i;
	}
	if (p->code != code) {
		i++;
		fread(p, sizeof(Producto), 1, f);
	}
	return pos;
}

void GenerarArchivos(FILE* f) {
	Producto p;
	Rubro r;
	char ruta[25], codeAnt[21];
	FILE* fRubro;
	fread(&p, sizeof(Producto), 1, f);
	while (!feof(f)) {
		strcpy(ruta, p.codeRubro);
		strcpy(codeAnt, p.codeRubro);
		strcat(ruta, ".dat");
		fRubro = fopen(ruta, "wb");
		do {
			r.cant = p.stock;
			r.total = p.price * p.stock;
			r.code = p.code;
			fwrite(&r, sizeof(Rubro), 1, fRubro);
			fread(&p, sizeof(Producto), 1, f);
		} while (!feof(f) && strcmp(p.codeRubro, codeAnt) == 0);
		fclose(fRubro);
		fread(&p, sizeof(Producto), 1, f);
	}
}
