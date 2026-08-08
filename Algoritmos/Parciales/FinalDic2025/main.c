#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include "lista_doble.h"
#include <string.h>
#define PRODUCTOS "productos.txt"

typedef struct {
	int dia;
	int mes;
	int año;
} fecha_t;
typedef struct {
	char code[9];
	char desc[31];
	unsigned int stock;
	float precio;
	fecha_t f_ult_actualizacion;

} producto_t;
int fecha_cmp(fecha_t a, fecha_t b)
{
	if (a.año != b.año) {
		return b.año - a.año;
	}
	if (a.mes != b.mes) {
		return b.mes - a.mes;
	}
	return b.dia - a.dia;
}
int cmp_code(void *a, void *b)
{
	return strcmp(((producto_t *)a)->code, ((producto_t *)b)->code);
}
void acumular_producto(void *ctx, void *param)
{
	producto_t *prod = (producto_t *)ctx, *dup = (producto_t *)param;
	prod->stock += dup->stock;
	if (fecha_cmp(prod->f_ult_actualizacion, dup->f_ult_actualizacion) >
	    0) {
		prod->f_ult_actualizacion = dup->f_ult_actualizacion;
		strcpy(prod->desc, dup->desc);
		prod->precio = dup->precio;
	}
}
void acumular_ingresos(void *ctx, void *param)
{
	producto_t *prod = (producto_t *)ctx;
	float *buff = (float *)param;
	*buff += prod->precio * prod->stock;
}
void prnt(void *dato, void *param)
{
	producto_t prod = *(producto_t *)dato;
	float importe = *(float *)param;

	if (importe < prod.precio * prod.stock) {
		printf("%8s;%30s;%d;%f;%2d/%2d/%4d\n", prod.code, prod.desc,
		       prod.stock, prod.precio, prod.f_ult_actualizacion.dia,
		       prod.f_ult_actualizacion.mes,
		       prod.f_ult_actualizacion.año);
	}
}
int leer_registro(FILE *f, producto_t *p)
{
	char linea[200];
	if (!fgets(linea, 200, f)) {
		return 0;
	}
	sscanf(linea, "%8[^;];%30[^;];%d;%f;%2d/%2d/%4d\n", p->code, p->desc,
	       &p->stock, &p->precio, &p->f_ult_actualizacion.dia,
	       &p->f_ult_actualizacion.mes, &p->f_ult_actualizacion.año);
	return 1;
}
int main()
{
	FILE *f = fopen(PRODUCTOS, "rt");
	lista_doble_t l;
	float ingresos_brutos;
	producto_t prod;
	float ingresos = 0;
	lista_doble_iniciar(&l);
	//MART0001;Martillo;10;5000;01/01/2025

	while (leer_registro(f, &prod)) {
		lista_doble_insertar_ultimo(&l, &prod, sizeof(producto_t));
	}

	lista_doble_eliminar_dup(&l, cmp_code, acumular_producto);
	printf("Ingrese para mostrar a partir de que ingresos: ");
	scanf("%f", &ingresos_brutos);
	lista_doble_mostrar(&l, prnt, &ingresos_brutos);
	lista_doble_reduce(&l, acumular_ingresos, &ingresos);
	printf("Ingresos brutos: %f\n", ingresos);
	printf("Hello world!\n");
	fclose(f);
	lista_doble_vaciar(&l);
	return 0;
}
