#include "../../headers/ejercicios.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/* Desarrollar una función que inserte un elemento en un arreglo de enteros,
 * ordenado en forma ascendente, de forma de no alterar el orden.
 */
void ordenar(int *n, int ce);
void dejarEspacio(int *n, int ce, int num);
void ej1_2() {
  int n[50] = {0}, i, num, ce = 0;
  srand(time(NULL));
  printf("Ingrese la cantidad de elementos del arreglo (maximo 100): ");
  scanf("%d", &ce);
  printf("Ingrese los elementos del arreglo en orden ascendente:\n");
  for (i = 0; i < ce; i++) {
    n[i] = rand() % 100; // Generar números aleatorios entre 0 y 99
    printf("%d\n", n[i]);
  }
  ordenar(n, ce); // Asegurarse de que el arreglo esté ordenado
  while (ce < 50 || num < 0) {

    printf("Ingrese el numero a insertar: ");
    scanf("%d", &num);
    // Encontrar la posición correcta para insertar el nuevo número
    if (num > n[ce - 1]) {
      n[ce] =
          num; // Si el número es mayor que el último, lo agregamos al finalj
    } else if (ce >= 50) {
      printf("El arreglo ya está lleno. No se puede insertar más elementos.\n");
      break; // Si el arreglo está lleno, no se puede insertar más
    } else {
      int li = 0;
      while (n[li] < num && li < 50) {
        li++;
      }
      int *pos = n + li;
      dejarEspacio(pos, ce, num); // Insertar el número en la posición correcta
      *pos = num;                 // Asignar el número en la posición encontrada
    }
    ce++; // Incrementar el tamaño del arreglo
    for (i = 0; i < ce; i++) {
      printf("%d ", n[i]); // Imprimir el arreglo resultante
    }
    printf("\n");
  }
}

void ordenar(int *n, int ce) {
  int *ult = n + ce - 1;
  int elemenAis;
  for (int *i = n + 1; i <= ult; i++) {

    int *j = i - 1;
    elemenAis = *i;
    while (*j > elemenAis && j >= n) {
      *(j + 1) = *(j);
      j--;
    }
    *(j + 1) = elemenAis;
  }
}

void dejarEspacio(int *n, int ce, int num) {
  for (int *i = n + ce; i > n; i--) {
    *(i + 1) = *i;
  }
}
