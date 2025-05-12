#include "../headers/ejercicios.h"
#include <stdio.h>
#include <stdlib.h>

int factorial(int n) {
  if (n == 0 || n == 1)
    return 1;
  return n * factorial(n - 1);
}

void ej0_1() {
  int n;
  printf("Ingrese un numero para calcular su factorial: ");
  scanf("%d", &n);
  printf("El factorial es: %d", factorial(n));
  return;
}
