#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Fecha.h"
#include <Cola.h>
#include "utils.h"
#include <Comun.h>

#define ERR_LLENO -1
#define ERR_FILE -2

#define FILE_URL "prod.bin"
#define FILE_URL_TXT "prod.txt"
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

void format(Prod *p, char *linea);
int colocarProdEnPila_TXT(t_Cola *c);
int colocarProdEnPila_BIN(t_Cola *c);
void printProd(Prod *c);
void menu(t_Cola *c);
int save(t_Cola *c);

int main()
{
	t_Cola c;
	crearCola(&c);
	int code = colocarProdEnPila_BIN(&c);
	if (code != OK) {
		printf("No hay datos guardados\n");
	}
	menu(&c);
	if (colaVacia(&c) != TRUE) {
		save(&c);
	} else {
		printf("Sin datos en pila, eliminando archivo");
		remove(FILE_URL);
	}

	return 0;
}

void printProd(Prod *p)
{
	printf("code: %s \ndesc: %s \nprov: %s \n", p->code, p->desc, p->prov);
	printf("Fecha compra: ");
	mostrarFecha(&p->Fcomp);
	printf("\n");
	printf("Fecha Venta: ");
	mostrarFecha(&p->venc);
	printf("\n");
	printf("cant: %d \ncompra: %.2f \nventa: %.2f \n", p->cant,
	       p->precio_compra, p->precio_venta);
}

int colocarProdEnPila_TXT(t_Cola *p)
{
	Prod prod;
	char *linea = malloc(TAM_LINEA);
	FILE *f = fopen(FILE_URL_TXT, "rt");
	if (!f) {
		return ERR_FILE;
	}
	fgets(linea, TAM_LINEA, f);
	while (fgets(linea, TAM_LINEA, f)) {
		format(&prod, linea);
		ponerEnCola(p, &prod, sizeof(Prod));
	}
	free(linea);
	fclose(f);
	return 1;
}

int colocarProdEnPila_BIN(t_Cola *p)
{
	Prod prod;
	FILE *f = fopen(FILE_URL, "rb");
	if (!f) {
		return ERR_FILE;
	}
	while (fread(&prod, sizeof(Prod), 1, f)) {
		ponerEnCola(p, &prod, sizeof(Prod));
	}
	fclose(f);
	return 1;
}

void format(Prod *p, char *linea)
{
	int d, m, y;
	char *buff = strchr(linea, '\n');

	*buff = '\0';
	buff = strrchr(linea, ',');
	sscanf(buff + 1, "%f", &p->precio_venta);

	*buff = '\0';
	buff = strrchr(linea, ',');
	sscanf(buff + 1, "%f", &p->precio_compra);

	*buff = '\0';
	buff = strrchr(linea, ',');
	sscanf(buff + 1, "%d", &p->cant);

	*buff = '\0';
	buff = strrchr(linea, ',');
	sscanf(buff + 1, "%d/%d/%d", &d, &m, &y);
	fechaSet(&p->venc, d, m, y);

	*buff = '\0';
	buff = strrchr(linea, ',');
	sscanf(buff + 1, "%d/%d/%d", &d, &m, &y);
	fechaSet(&p->Fcomp, d, m, y);

	*buff = '\0';
	buff = strrchr(linea, ',');
	sscanf(buff + 1, "%s", p->prov);

	*buff = '\0';
	buff = strrchr(linea, ',');
	sscanf(buff + 1, "%s", p->desc);

	*buff = '\0';
	sscanf(linea, "%s", p->code);
}

int cargar(t_Cola *p)
{
	Prod prod;
	int d, m, y;
	flush();
	printf("Ingrese codigo: ");
	fsgets(prod.code, 7);

	printf("\n");
	printf("Ingrese descripcion: ");
	fsgets(prod.desc, 15);

	printf("\n");
	printf("Ingrese proveedor: ");
	fsgets(prod.prov, 15);

	printf("\n");
	printf("Ingrese Fecha de compra (d,m,y): ");
	scanf("%d %d %d", &d, &m, &y);

	flush();
	fechaSet(&prod.Fcomp, d, m, y);
	printf("Ingrese Fecha de vencimiento (d,m,y): ");
	scanf("%d %d %d", &d, &m, &y);

	flush();
	fechaSet(&prod.venc, d, m, y);
	printf("Ingrese cantidad: ");
	scanf("%d", &prod.cant);

	printf("Ingrese precio venta: ");
	scanf("%f", &prod.precio_venta);

	printf("Ingrese precio compra: ");
	scanf("%f", &prod.precio_compra);
	ponerEnCola(p, &prod, sizeof(Prod));
	return 1;
}

void verProd(t_Cola *p)
{
	Prod prod;
	verPrimero(p, &prod, sizeof(Prod));
	printProd(&prod);
}

void menu(t_Cola *p)
{
	int opcion = -1;
	while (opcion != 0) {
		printf("Elija la opcion: \n");
		printf("1.Cargar dato\n");
		printf("2.Quitar ultimo dato cargado\n");
		printf("3.Ver ultimo dato cargado\n");
		printf("0.Salir\n");
		scanf("%d", &opcion);
		switch (opcion) {
		case 0:
			break;
		case 1:
			if (colaLlena(p, sizeof(Prod) == TRUE)) {
				printf("Pila llena, saque uno o mas datos y vuelva a intentar \n");
				break;
			}
			limpiar_terminal();
			cargar(p);
			printf("Producto cargado\n");
			break;
		case 2:
			limpiar_terminal();
			Prod aux;
			sacarDeCola(p, &aux, sizeof(Prod));
			printf("Ultimo dato eliminado");
			break;
		case 3:
			limpiar_terminal();
			verProd(p);
			break;
		default:
			printf("Selecciones una opcion valida");
			break;
		}
		esperar_segundos(5);
		limpiar_terminal();
	}
}

int save(t_Cola *p)
{
	Prod prod;
	FILE *f = fopen(FILE_URL, "wb");
	if (!f) {
		return ERR_FILE;
	}
	while (colaVacia(p) != TRUE) {
		sacarDeCola(p, &prod, sizeof(Prod));
		fwrite(&prod, sizeof(Prod), 1, f);
	}
	printf("Datos guardados correctamente");
	fclose(f);
	return TRUE;
}
