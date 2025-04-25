#include "vector.h"

void crearVector(Vector *v) { v->ce = 0; }
int llenarVector(const Vector *v) { return v->ce == TAM_MAX; }
int vaciarVector(const Vector *v) { return v->ce == 0; }
int setVectorOrdenado(Vector *v, int elemento) {
  int i = 0, j;
  if (v->ce == TAM_MAX)
    return LLENO;
  while (i < v->ce && v->vector[i] < elemento) {
    i++;
  }
  for (j = v->ce; j; j--) {
    v->vector[j] = v->vector[j - 1];
  }
  v->vector[i] = elemento;
  v->ce++;
  return VERDADERO;
}
void destoryVector(Vector *v) { v->ce = 0; }
