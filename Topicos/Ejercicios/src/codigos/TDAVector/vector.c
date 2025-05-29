#include "../../headers/Vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int *BuscarMenor(void *li, void *ls, Cmp cmp, size_t tamElem);
void intercambiar(void *a, void *b, size_t tamElem);
void ordenarBurbujeo(Vector *vector, Cmp cmp);
void ordenarSeleccion(Vector *vector, Cmp cmp);
void ordenarInsercion(Vector *vector, Cmp cmp);
bool redimensionarVector(Vector *vector, int operacion);

bool vectorCrear(Vector *vector, size_t tamElem) {
  vector->ce = 0;
  vector->vec = malloc(CAP_INI * tamElem);

  if (vector->vec == NULL) {
    vector->cap = 0;
    vector->tamElem = 0;
    return false;
  }

  vector->cap = CAP_INI;
  vector->tamElem = tamElem;

  return true;
}

int vectorOrdInsertar(Vector *vector, const void *elem, Cmp cmp) {
  if (vector->ce == vector->cap) {
    if (!redimensionarVector(vector, AUMENTAR)) {
      return SIN_MEM;
    }
  }

  void *i = vector->vec;
  void *ult = vector->vec + (vector->ce - 1) * vector->tamElem;

  while (i <= ult && cmp(elem, i) > 0) {
    i += vector->tamElem;
  }

  if (i <= ult && cmp(elem, i) == 0) { // Duplicado
    return DUPLICADO;
  }

  void *posIns = i;

  for (i = ult; i >= posIns; i -= vector->tamElem) {
    memcpy(i + vector->tamElem, i, vector->tamElem);
  }

  memcpy(posIns, elem, vector->tamElem); // memmove si hay solapamiento

  vector->ce++;

  return TODO_OK;
}

int vectorOrdBuscar(const Vector *vector, void *elem, Cmp cmp) {
  if (vector->ce == 0) {
    return -1;
  }

  void *li = vector->vec;
  void *ls = vector->vec + vector->ce * vector->tamElem - vector->tamElem;
  void *m = li + ((ls - li) / vector->tamElem / 2) * vector->tamElem;
  int comp;

  while (li <= ls && (comp = cmp(elem, m)) != 0) {
    if (comp < 0) {
      ls = m - vector->tamElem; // El elemento está a la izquierda
    } else                      // > 0
    {
      li = m + vector->tamElem; // El elemento está a la derecha
    }
    m = li + ((ls - li) / vector->tamElem / 2) * vector->tamElem;
  }

  if (li > ls) {
    return -1;
  }
  memcpy(elem, m, vector->tamElem); // Copia el elemento encontrado
  return (m - vector->vec) /
         vector->tamElem; // Devuelve la posición del elemento
}

/*bool vectorOrdEliminar(Vector *vector, int elem) {
  int posElim = vectorOrdBuscar(vector, elem);

  if (posElim == -1) {
    return false;
  }

  vectorEliminarDePos(vector, posElim);

  return true;
}
*/
bool vectorEliminarDePos(Vector *vector, int pos) {
  if (pos >= vector->ce) {
    return false;
  }

  int *ult = vector->vec + vector->ce - 1;

  for (int *i = vector->vec + pos; i < ult; i++) {
    *i = *(i + 1);
  }

  vector->ce--;

  if ((float)vector->ce / vector->cap <= FACTOR_OCUP) {
    redimensionarVector(vector, DISMINUIR);
  }

  return true;
}

void vectorDestruir(Vector *vector) {
  free(vector->vec);
  vector->vec = NULL;
  vector->ce = 0;
  vector->cap = 0;
}

bool redimensionarVector(Vector *vector, int operacion) {
  size_t nuevaCap = operacion == AUMENTAR
                        ? vector->cap * FACTOR_INCR
                        : max(vector->cap * FACTOR_DECR, CAP_INI);

  int *nVec = realloc(vector->vec, nuevaCap * vector->tamElem);

  if (!nVec) {
    return false;
  }

  printf("Redimensión de %zu, a %zu\n", vector->cap, nuevaCap);

  vector->vec = nVec;
  vector->cap = nuevaCap;

  return true;
}

size_t max(size_t a, size_t b) { return a >= b ? a : b; }

void vectorOrdenar(Vector *vector, int metodo, Cmp cmp) {
  switch (metodo) {
  case BURBUJEO:
    ordenarBurbujeo(vector, cmp);
    break;
  case SELECCION:
    // Implementar otro método de ordenamiento
    ordenarSeleccion(vector, cmp);
    break;
  case INSERCION:
    // ordenarInsercion(vector,cmp);
    break;
  case QSORT:
    qsort(vector->vec, vector->ce, vector->tamElem, cmp);
    break;
  default:
    printf("Método de ordenamiento no válido\n");
    break;
  }
}
void ordenarBurbujeo(Vector *vector, Cmp cmp) {
  int *ult = vector->vec + vector->ce - 1;
  bool huboIntercambio = true;
  for (int i = 1, *limJ = ult - 1; huboIntercambio && i < vector->ce;
       i++, limJ--) {
    huboIntercambio = false;
    for (int *j = vector->vec; j <= limJ; j++) {
      if (*j > *(j + 1)) {
        intercambiar(j, j + 1, vector->tamElem);
        huboIntercambio = true;
      }
    }
  }
}

void intercambiar(void *a, void *b, size_t tamElem) {
  void *aTemp = malloc(tamElem);
  memcpy(aTemp, a, tamElem); // memmove si hay solapamiento
  memcpy(a, b, tamElem);
  memcpy(b, aTemp, tamElem); // memmove si hay solapamiento
  free(aTemp);
}

int vectorInsertarAlFinal(Vector *vector, const void *elem) {
  if (vector->ce == vector->cap) {
    if (!redimensionarVector(vector, AUMENTAR)) {
      return SIN_MEM;
    }
  }
  void *posIns = vector->vec + vector->ce * vector->tamElem;
  memcpy(posIns, elem, vector->tamElem); // memove si hay solapamiento
  vector->ce++;
  return TODO_OK;
}

void vectorMostrar(const Vector *vector, Imprimir imp) {
  void *ult = vector->vec + (vector->ce - 1) * vector->tamElem;
  for (void *i = vector->vec; i <= ult; i += vector->tamElem) {
    imp(i);
    putchar(' ');
  }
  putchar('\n');
}

void ordenarSeleccion(Vector *vector, Cmp cmp) {
  void *ult = vector->vec + (vector->ce - 1) * vector->tamElem;
  void *m;
  for (void *i = vector->vec; i < ult; i += vector->tamElem) {
    m = BuscarMenor(i, ult, cmp, vector->tamElem);

    intercambiar(i, m, vector->tamElem);
  }
}

int *BuscarMenor(void *li, void *ls, Cmp cmp, size_t tamElem) {
  void *menor = li;
  for (void *i = li + tamElem; i <= ls; i += tamElem) {
    if (cmp(i, menor) < 0) {
      menor = i;
    }
  }
  return menor;
}

/*void ordenarInsercion(Vector *vector, Cmp cmp) {
  void *ult = vector->vec + (vector->ce - 1) * vector->tamElem;
  void elemAIns;
  for (void *i = vector->vec + vector->tamElem; i <= ult;
       i += vector->tamElem) {
    elemAIns = *i;
    int *j = i - 1;
    while (j >= vector->vec && elemAIns < *j) {
      *(j + 1) = *j;
      j--;
    }

    *(j + 1) = elemAIns;
  }
}
*/
