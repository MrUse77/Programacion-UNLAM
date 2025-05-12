#include "../../headers/ejercicios.h"
#include <stdio.h>

void ej1_1() {
  int vector[10] = {0}, *v, a, b;
  v = vector;
  printf("Ingrese un numero: ");
  scanf("%d", &a);
  printf("Ingrese donde quiere guardar el numero: ");
  scanf("%d", &b);
  *(v + b) = a;
  printf("El numero guardado es: %d\n", *(v + b));
}
