#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "archivos.h"
#include "string.h"

typedef struct {
	char codigo[11];
	char desc[51];
	float precio;
} Producto;
typedef struct {
	char codigo[11];
	int nroReg;
} ProductoIdx;

int mostrarArchivosProductos(const char *nomArchProds);
int actualizarPreciosProductos(const char *nomArchProds, const char *codProd,
			       float porc);
int generarArchivoProducto(const char *nomArchProds);
void crearProductoIdx(void *reg, unsigned int nroReg, void *regIdx);
int cmpRegIdx(const void *a, const void *b);

// ActualizadorPreciosProductos.exe Productos.dat 1 10.5

int main(int argc, char *argv[])
{
	float porc = atof(argv[3]);

	int ret;
	ret = generarArchivoProducto(argv[1]);
	printf("%d\n", ret);
	crearIndice(argv[1], sizeof(Producto), sizeof(ProductoIdx),
		    crearProductoIdx, cmpRegIdx);

	ret = actualizarPreciosProductos(argv[1], argv[2], porc);
	printf("%d\n", ret);
	ret = mostrarArchivosProductos(argv[1]);
	printf("%d\n", ret);
	return ret;
}

int generarArchivoProducto(const char *nomArchProds)
{
	Producto prods[] = { { "PERA", "PERA WILLIAMS", 120.50 },
			     { "MANZANA", "MANZANA ROJA", 100.75 },
			     { "NARANJA", "NARANJA VALENCIA", 80.20 },
			     { "BANANA", "BANANA CAVENDISH", 90.00 },
			     { "KIWI", "KIWI VERDE", 150.30 },
			     { "FRUTILLA", "FRUTILLA FRESCA", 200.10 },
			     { "UVA", "UVA RED GLOBE", 110.40 },
			     { "MANDARINA", "MANDARINA TANGERINA", 70.60 },
			     { "CEREZA", "CEREZA BING", 250.00 },
			     { "MANGO", "MANGO ATAULFO", 180.80 } };
	FILE *archProds = fopen(nomArchProds, "wb");
	fwrite(&prods, sizeof(prods), 1, archProds);
	fclose(archProds);
	return 1;
}
int actualizarPreciosProductos(const char *nomArchProds, const char *codProd,
			       float porc)
{
	FILE *archProds = fopen(nomArchProds, "r+b");
	if (!archProds) {
		return -1;
	}
	float factorIncr = 1 + porc / 100;

	char nomArchProdsIdx[201];
	cambiarExtension(nomArchProds, "idx", nomArchProdsIdx);
	Vector vIdx;
	if (!vectorCrearDeArchivoBIN(&vIdx, sizeof(ProductoIdx),
				     nomArchProdsIdx)) {
		perror("No se pudo crear el vector de indice");
		fclose(archProds);
		return SIN_MEM;
	}

	ProductoIdx pIdx;
	cpyString(pIdx.codigo, codProd);
	if (!vectorOrdBuscar(&vIdx, &pIdx, cmpRegIdx)) {
		perror("No se encontro el producto");
		fclose(archProds);
		vectorDestruir(&vIdx);
		return SIN_MEM;
	};
	Producto prod;

	rewind(archProds);
	fseek(archProds, pIdx.nroReg * sizeof(Producto), SEEK_SET);
	fread(&prod, sizeof(Producto), 1, archProds);
	prod.precio *= factorIncr;
	fseek(archProds, -sizeof(Producto), SEEK_SET);
	fwrite(&prod, sizeof(Producto), 1, archProds);
	fclose(archProds);
	return 1;
}
int mostrarArchivosProductos(const char *nomArchProds)
{
	FILE *archProds = fopen(nomArchProds, "rb");
	Producto prod;
	fread(&prod, sizeof(Producto), 1, archProds);

	while (!feof(archProds)) {
		printf("%-10s\t%-25s\t%06.2f\n", prod.codigo, prod.desc,
		       prod.precio);
		fread(&prod, sizeof(Producto), 1, archProds);
	}
	return 1;
}
void crearProductoIdx(void *reg, unsigned int nroReg, void *regIdx)
{
	Producto *p = reg;
	ProductoIdx *pIdx = regIdx;
	cpyString(p->codigo, pIdx->codigo);
	pIdx->nroReg = nroReg;
}
int cmpRegIdx(const void *a, const void *b)
{
	ProductoIdx *aIdx = (ProductoIdx *)a;
	ProductoIdx *bIdx = (ProductoIdx *)b;
	return cmpString(aIdx->codigo, bIdx->codigo);
}
