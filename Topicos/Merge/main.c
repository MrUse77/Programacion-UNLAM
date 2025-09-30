#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ERR_ARCHIVO -1
#define TODO_OK 0

typedef struct {
	char codigo[11];
	char descripcion[21];
	int stock;
} Producto;
typedef struct {
	char codigo[11];
	int cantidad;
} Movimiento;
int generarProducto(const char *nomArchProductos);
int generarMovimientos(const char *nomArchMovimientos);
int mostrarProductos(const char *nomArchProductos);
int actualizarProductos(const char *nomArchProductos,
			const char *nomArchMovimientos);
void cambiarExtension(const char *nomArch, const char *ext,
		      char *nomArchCambiado);
void procesarProductoNuevo(Movimiento *m, FILE *archMovs, FILE *archProdsTemp);

int main(int argc, char *argv[])
{
	generarProducto(argv[1]);
	generarMovimientos(argv[2]);
	puts("Productos antes de actualizar:\n");
	mostrarProductos(argv[1]);
	actualizarProductos(argv[1], argv[2]);
	puts("\nProductos despues de actualizar:\n");
	mostrarProductos(argv[1]);

	return 0;
}

int generarProducto(const char *nomArchProductos)
{
	Producto vP[] = { { "ANANA", "Anana amarilla", 100 },
			  { "BANANA", "Banana organica", 50 },
			  { "CEREZA", "Cereza roja", 60 },
			  { "FRESA", "Fresa dulce", 75 },
			  //        {"KIWI", "Kiwi verde", 30},
			  { "MANZANA", "Manzana roja", 200 },
			  { "NARANJA", "Naranja jugosa", 150 },
			  { "PERA", "Pera Williams", 80 },
			  { "UVA", "Uva sin semillas", 120 },
			  { "ZANAHORIA", "Zanahoria fresca", 90 } };
	FILE *fp = fopen(nomArchProductos, "wb");
	if (!fp) {
		perror("Error al abrir el archivo de productos");
		return ERR_ARCHIVO;
	}
	fwrite(vP, sizeof(Producto), sizeof(vP) / sizeof(Producto), fp);

	fclose(fp);
	return TODO_OK;
}

int generarMovimientos(const char *nomArchMovimientos)
{
	// Generar movimientos de stock, ordenados por código.
	Movimiento vMovimientos[] = { { "ANANA", 20 },	{ "BANANA", -10 },
				      { "CEREZA", 15 }, { "CEREZA", 20 },
				      { "FRESA", -5 },	{ "KIWI", 30 },
				      { "KIWI", 20 },	{ "MANZANA", -20 },
				      { "UVA", 25 } };

	FILE *archivo = fopen(nomArchMovimientos, "wb");

	if (!archivo) {
		perror("Error al abrir el archivo de movimientos");
		return ERR_ARCHIVO;
	}

	fwrite(vMovimientos, sizeof(Movimiento),
	       sizeof(vMovimientos) / sizeof(Movimiento), archivo);

	fclose(archivo);
	return 0;
}
int mostrarProductos(const char *nomArchProductos)
{
	FILE *archivo = fopen(nomArchProductos, "rb");

	if (!archivo) {
		perror("Error al abrir el archivo de productos");
		return ERR_ARCHIVO;
	}

	Producto producto;

	printf("%-11s %-21s %s\n", "Codigo", "Descripcion", "Stock");
	printf("%-11s %-21s %s\n", "-------", "-----------", "-----");

	while (fread(&producto, sizeof(Producto), 1, archivo) == 1) {
		printf("%-11s %-21s %3d\n", producto.codigo,
		       producto.descripcion, producto.stock);
	}

	fclose(archivo);
	return 0;
}
int actualizarProductos(const char *nomArchProductos,
			const char *nomArchMovimientos)
{
	FILE *archProds = fopen(nomArchProductos, "rb");
	if (!archProds) {
		perror("Error al abrir el archivo de productos");
		return ERR_ARCHIVO;
	}
	FILE *archMovs = fopen(nomArchMovimientos, "rb");
	if (!archMovs) {
		fclose(archProds);
		perror("Error al abrir el archivo de productos");
		return ERR_ARCHIVO;
	}
	char nomArchProductosTemp[51];
	cambiarExtension(nomArchProductos, "tmp", nomArchProductosTemp);

	FILE *archProdsTemp = fopen(nomArchProductosTemp, "wb");
	if (!archProdsTemp) {
		fclose(archProds);
		fclose(archMovs);
		perror("Error al abrir el archivo de productos");
		return ERR_ARCHIVO;
	}
	Producto p;
	Movimiento m;
	int cmp;
	fread(&p, sizeof(Producto), 1, archProds);
	fread(&m, sizeof(Movimiento), 1, archMovs);
	while (!feof(archProds) && !feof(archMovs)) {
		cmp = strcmp(p.codigo, m.codigo);
		if (cmp == 0) { //Prod con movimiento
			p.stock += m.cantidad;
			fread(&m, sizeof(Movimiento), 1, archMovs);
		} else if (cmp < 0) { //Prod sin movimiento
			fwrite(&p, sizeof(Producto), 1, archProdsTemp);
			fread(&p, sizeof(Producto), 1, archProds);
		} else { //Movimiento sin producto (nuevo Producto)
			procesarProductoNuevo(&m, archMovs, archProdsTemp);
		}
	}
	while (!feof(archProds)) { //Prod sin movimiento
		fwrite(&p, sizeof(Producto), 1, archProdsTemp);
		fread(&p, sizeof(Producto), 1, archProds);
	}
	while (!feof(archMovs)) { //Movimiento sin producto
		procesarProductoNuevo(&m, archMovs, archProdsTemp);
	}
	fclose(archProds);
	fclose(archMovs);
	fclose(archProdsTemp);
	remove(nomArchProductos);
	rename(nomArchProductosTemp, nomArchProductos);
	return TODO_OK;
}
void cambiarExtension(const char *nomArch, const char *ext,
		      char *nomArchCambiado)
{
	strcpy(nomArchCambiado, nomArch);
	char *punto = strchr(nomArchCambiado, '.');
	strcpy(punto + 1, ext);
}

void procesarProductoNuevo(Movimiento *m, FILE *archMovs, FILE *archProdsTemp)
{
	Producto pNew;
	strcpy(pNew.codigo, m->codigo);
	pNew.descripcion[0] = '\0'; // Descripcion vacia
	pNew.stock = m->cantidad;
	fread(m, sizeof(Movimiento), 1, archMovs);
	while (!feof(archMovs) && strcmp(m->codigo, pNew.codigo) == 0) {
		pNew.stock += m->cantidad;
		fread(m, sizeof(Movimiento), 1, archMovs);
	}
	fwrite(&pNew, sizeof(Producto), 1, archProdsTemp);
}
