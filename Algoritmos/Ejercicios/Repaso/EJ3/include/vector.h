#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#define TAM_INIT 10

#define FACTOR_INC 2
#define FACTOR_DEC 0.5

#define OK 0
#define ERR 1
#define ERR_REP 2
#define ERR_ESPACIO 3
#define SIN_MEM 4
#define ERR_MEM 5
#define ERR_ORD 6

typedef struct {
  void *v;
  size_t ce;
  size_t cap;
  size_t te;
} Vector;

typedef int (*Cmp)(const void *c1, const void *c2);
typedef void (*Prnt)(const void *elem);
typedef void (*FmtIns)(const char *, void *);

int crearVector(Vector *v, size_t tam);
bool vectorLleno(Vector *v);
int vectorVaciar(Vector *v);
bool vectorVacio(Vector *v);
int vectorOrdInsertar(Vector *v, void *elem, Cmp cmp);
void vectorVerElemDePos(Vector *v, int pos, Prnt Prnt);
int vectorEliminar(Vector *v, void *elem, Cmp cmp);
int vectorDestruir(Vector *v);
int vectorInsertarAlInicio(Vector *v, void *elem);

int crearVectorDeArchivoTxtConCant(Vector *v, char *file, size_t tam,
                                   FmtIns format);
void vectorMostrar(Vector *v, Prnt print);
#endif
