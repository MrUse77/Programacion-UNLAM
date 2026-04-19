#ifndef FECHA_H
#define FECHA_H

#include <stdbool.h>
typedef struct {
	int dia;
	int mes;
	int anio;
} Fecha;

//Primitivas
void fechaSet(Fecha *f, int d, int m, int a);
void fechaGet(const Fecha *f, int *d, int *m, int *a);
Fecha fechaSumarDias(const Fecha *f, int dias);
int fechaDiferencia(const Fecha *f1, const Fecha *f2);
int fechaDiaDelAnio(const Fecha *f);
int fechaDiasRestantesDelAnio(const Fecha *f);

//No primitivas
void ingresarFecha(Fecha *f);
void mostrarFecha(const Fecha *f);
int fechaDeTxt(const char *txt, Fecha *f); //"28/5/2025"

#endif // FECHA_H
