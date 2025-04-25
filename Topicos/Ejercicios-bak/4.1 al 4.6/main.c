#include "Fecha.h"
#include <stdio.h>

int main() {
  Fecha f = ingresarFecha();
  printf("La fehca ingresada es: %d/%d/%d\n", f.dia, f.mes, f.anio);
  /*  Fecha sig = sumarDias(&f, 1);
    printf("+1 dia: %d/%d/%d\n", sig.dia, sig.mes, sig.anio);
    Fecha sum = sumarDias(&f, 365);
    printf("sumando Dias: %d/%d/%d\n", sum.dia, sum.mes, sum.anio);
    printf("Hello world!\n");

    Fecha restar = restarDias(&f, 365);
    printf("sumando Dias: %d/%d/%d\n", restar.dia, restar.mes, restar.anio);
    Fecha f2 = ingresarFecha();
    printf("La fehca ingresada es: %d/%d/%d\n", f2.dia, f2.mes, f2.anio);
    int diff = getDiasDif(&f, &f2);
    printf("La diferencia de dias es: %d\n", diff);
    */
  printf("El dia de la semana es: %d", getDiaSemana(&f));
  printf("Hello world!\n");

  return 0;
}
