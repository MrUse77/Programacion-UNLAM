#include "../../headers/Vector.h"
#include <stdio.h>
#include <stdlib.h>

bool redimensionarVector(Vector *vector, int operacion);

bool vectorCrear(Vector *vector) {
  vector->ce = 0;
  vector->vec = malloc(CAP_INI * sizeof(int));

  if (vector->vec == NULL) {
    vector->cap = 0;
    return false;
  }

  vector->cap = CAP_INI;

  return true;
}

int vectorOrdInsertar(Vector *vector, int elem) {
  if (vector->ce == vector->cap) {
    if (!redimensionarVector(vector, AUMENTAR)) {
      return SIN_MEM;
    }
  }

  int *i = vector->vec;
  int *ult = vector->vec + vector->ce - 1;

  while (i <= ult && elem > *i) {
    i++;
  }

  if (i <= ult && elem == *i) // Duplicado
  {
    return DUPLICADO;
  }

  int *posIns = i;

  for (i = ult; i >= posIns; i--) {
    *(i + 1) = *i;
  }

  *posIns = elem;

  vector->ce++;

  return TODO_OK;
}

int vectorOrdBuscar(const Vector *vector, int elem) {
  if (vector->ce == 0) {
    return -1;
  }

  int *li = vector->vec;
  int *ls = vector->vec + vector->ce - 1;
  int *m = li + (ls - li) / 2;
  int comp;

  while (li <= ls && (comp = elem - *m) != 0) {
    if (comp < 0) {
      ls = m - 1;
    } else // > 0
    {
      li = m + 1;
    }

    m = li + (ls - li) / 2;
  }

  if (li > ls) {
    return -1;
  }

  return m - vector->vec;
}

bool vectorOrdEliminar(Vector *vector, int elem) {
  int posElim = vectorOrdBuscar(vector, elem);

  if (posElim == -1) {
    return false;
  }

  vectorEliminarDePos(vector, posElim);

  return true;
}

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

  int *nVec = realloc(vector->vec, nuevaCap * sizeof(int));

  if (!nVec) {
    return false;
  }

  printf("Redimensión de %zu, a %zu\n", vector->cap, nuevaCap);

  vector->vec = nVec;
  vector->cap = nuevaCap;

  return true;
}

size_t max(size_t a, size_t b) { return a >= b ? a : b; }
