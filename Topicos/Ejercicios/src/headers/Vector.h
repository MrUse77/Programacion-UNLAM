
#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stddef.h>

#define TAM 10

#define TODO_OK 0
#define DUPLICADO 1
#define LLENO 2
#define SIN_MEM 3

#define CAP_INI 8        // Cant elementos
#define FACTOR_INCR 1.5  // Incr 50%
#define FACTOR_DECR 0.5  // Decr 50%
#define FACTOR_OCUP 0.25 // Ocup 25%

#define AUMENTAR 1
#define DISMINUIR 2
typedef struct {
  int *vec;
  size_t ce;
  size_t cap;
} Vector;

bool vectorCrear(Vector *vector); // malloc
int vectorOrdInsertar(Vector *vector, int elem);
int vectorOrdBuscar(const Vector *vector, int elem);
bool vectorOrdEliminar(Vector *vector, int elem);
bool vectorEliminarDePos(Vector *vector, int pos);
void vectorDestruir(Vector *vector); // free

size_t max(size_t a, size_t b);
#endif // VECTOR_H
