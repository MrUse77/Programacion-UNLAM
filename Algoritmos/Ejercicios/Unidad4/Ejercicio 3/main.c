#include <lista_simple.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define DATOS "datos.csv"
typedef struct {
	int clave;
	int cant;
	int dia_comp;
	int mes_comp;
	int anio_comp;
	int dia_venc;
	int mes_venc;
	int anio_venc;
	int precio_comp;
	int precio_vent;
	char prod[8];
} Prod;

typedef struct {
	Prod prod;
	int encontrado;
} prod_ctx;

int cmp_fecha(int ad, int ac, int md, int mc, int dd, int dc)
{
	if (ad != ac) {
		return ad - ac;
	}
	if (md != mc) {
		return md - mc;
	}
	if (dd != dc) {
		return dd - dc;
	}
}

void intentar_acumular(void *data, void *param)
{
	Prod *prod = (Prod *)data;
	prod_ctx *ctx = (prod_ctx *)param;
	if (ctx->encontrado != 0)
		return;
	if (prod->clave == ctx->prod.clave &&
	    strcmp(prod->prod, ctx->prod.prod) == 0) {
		ctx->encontrado = 1;
		if (cmp_fecha(prod->anio_comp, ctx->prod.anio_comp,
			      prod->mes_comp, ctx->prod.mes_comp,
			      prod->dia_comp, ctx->prod.dia_comp) < 0) {
			prod->anio_comp = ctx->prod.anio_comp;
			prod->mes_comp = ctx->prod.mes_comp;
			prod->dia_comp = ctx->prod.dia_comp;
		}
		if (cmp_fecha(prod->anio_venc, ctx->prod.anio_venc,
			      prod->mes_venc, ctx->prod.mes_venc,
			      prod->dia_venc, ctx->prod.dia_venc) > 0) {
			prod->anio_venc = ctx->prod.anio_venc;
			prod->mes_venc = ctx->prod.mes_venc;
			prod->dia_venc = ctx->prod.dia_venc;
		}
		if (prod->precio_comp < ctx->prod.precio_comp) {
			prod->precio_comp = ctx->prod.precio_comp;
		}
		if (prod->precio_vent < ctx->prod.precio_vent) {
			prod->precio_vent = ctx->prod.precio_vent;
		}
		prod->cant += ctx->prod.cant;
	}
}
int main()
{
}

void leer_datos1(list_t *l)
{
	Prod p;
	char linea[1000];
	FILE *f = fopen(DATOS, "rt");
	if (!f) {
		return;
	}
	while (fgets(linea, 45, f)) {
		prod_ctx aux;
		sscanf(linea, "%d,%7[^,],%d,%4d/%2d/%2d,%4d/%2d/%2d,%d,%d,%*s",
		       &p.clave, p.prod, &p.cant, &p.anio_comp, &p.mes_comp,
		       &p.dia_comp, &p.anio_venc, &p.mes_venc, &p.dia_venc,
		       &p.precio_comp, &p.precio_vent);
		aux.prod = p;
		aux.encontrado = 0;
		list_foreach(l, &aux, intentar_acumular);
		if (aux.encontrado == 0) {
			list_push_last(l, &p, sizeof(Prod));
		}
	}
}
