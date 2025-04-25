
#ifndef FECHAS_H_
#define FECHAS_H_
#include <stdbool.h>
typedef struct {
  int dia;
  int mes;
  int anio;
} Fecha;

Fecha ingresarFecha();
bool setFecha(Fecha *f, int d, int m, int a);
bool esFechaValida(int d, int m, int a);
int cDiaMes(int m, int a);
bool esBisiesto(int a);
Fecha sumarDias(const Fecha *f, int d);
Fecha restarDias(const Fecha *f, int d);
int getDiasDif(const Fecha *f1, const Fecha *f2);
int getDiaSemana(const Fecha *f);
#endif // FECHAS_H_
