#include <stdio.h>
#include <stdlib.h>
#include "Pila.h"
#include "Fecha.h"
#include <Comun.h>

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

int main()
{
	Prod productos[] = { { "P00001",
			       "Leche",
			       "LaSeren",
			       { 1, 3, 2026 },
			       { 1, 5, 2026 },
			       12,
			       850.50f,
			       1200.00f },
			     { "P00002",
			       "Arroz",
			       "DonVal",
			       { 15, 2, 2026 },
			       { 15, 2, 2027 },
			       25,
			       620.00f,
			       890.00f },
			     { "P00003",
			       "Azucar",
			       "SolSur",
			       { 10, 1, 2026 },
			       { 10, 1, 2027 },
			       18,
			       540.75f,
			       790.00f },
			     { "P00004",
			       "Fideos",
			       "Pastal",
			       { 20, 2, 2026 },
			       { 20, 8, 2026 },
			       30,
			       430.00f,
			       650.00f },
			     { "P00005",
			       "Harina",
			       "MolinoN",
			       { 5, 3, 2026 },
			       { 5, 3, 2027 },
			       20,
			       390.00f,
			       580.00f },
			     { "P00006",
			       "Te",
			       "Andes",
			       { 12, 2, 2026 },
			       { 12, 2, 2027 },
			       14,
			       760.00f,
			       1090.00f },
			     { "P00007",
			       "Cafe",
			       "Brava",
			       { 18, 1, 2026 },
			       { 18, 1, 2027 },
			       10,
			       1450.00f,
			       1980.00f },
			     { "P00008",
			       "Yerba",
			       "MateLuz",
			       { 25, 2, 2026 },
			       { 25, 2, 2027 },
			       16,
			       1320.00f,
			       1790.00f },
			     { "P00009",
			       "Galletas",
			       "DulceMar",
			       { 8, 3, 2026 },
			       { 8, 9, 2026 },
			       40,
			       210.00f,
			       350.00f },
			     { "P00010",
			       "Jabon",
			       "Limpio",
			       { 11, 2, 2026 },
			       { 11, 2, 2028 },
			       22,
			       310.00f,
			       470.00f },
			     { "P00011",
			       "Shampoo",
			       "Capilar",
			       { 14, 3, 2026 },
			       { 14, 9, 2027 },
			       15,
			       980.00f,
			       1420.00f },
			     { "P00012",
			       "Detergente",
			       "Brillo",
			       { 3, 3, 2026 },
			       { 3, 3, 2027 },
			       11,
			       670.00f,
			       950.00f },
			     { "P00013",
			       "Rodillo",
			       "Pintar",
			       { 28, 2, 2026 },
			       { 28, 2, 2028 },
			       8,
			       2500.00f,
			       3350.00f },
			     { "P00014",
			       "Cuaderno",
			       "Nota",
			       { 17, 2, 2026 },
			       { 17, 2, 2029 },
			       50,
			       120.00f,
			       190.00f },
			     { "P00015",
			       "Lapiz",
			       "Grafi",
			       { 9, 3, 2026 },
			       { 9, 3, 2029 },
			       100,
			       45.00f,
			       80.00f } };

	t_Pila p;
	crearPila(&p);
	int code;
	for (int i = 0; i < 15; i++) {
		code = apilar(&p, &productos[i], sizeof(Prod));

		if (code != OK) {
			return code;
		}
	}
	Prod prod2;
	code = verTope(&p, &prod2, sizeof(Prod));
	if (code != OK) {
		return code;
	}
	printf("Tope: \n");
	printf("code: %s \n desc: %s \n  prov: %s \n", prod2.code, prod2.desc,
	       prod2.prov);
	mostrarFecha(&prod2.Fcomp);
	mostrarFecha(&prod2.venc);
	printf("cant: %d \n compra: %.2f \n  venta: %.2f \n", prod2.cant,
	       prod2.precio_compra, prod2.precio_venta);

	for (int i = 14; i >= 0; i--) {
		code = desapilar(&p, &prod2, sizeof(Prod));
		if (code != OK) {
			return code;
		}
		printf("Producto %d\n", i);
		printf("code: %s \n desc: %s \n  prov: %s \n", prod2.code,
		       prod2.desc, prod2.prov);
		mostrarFecha(&prod2.Fcomp);
		mostrarFecha(&prod2.venc);
		printf("cant: %d \n compra: %.2f \n  venta: %.2f \n",
		       prod2.cant, prod2.precio_compra, prod2.precio_venta);
	}

	return 0;
}
