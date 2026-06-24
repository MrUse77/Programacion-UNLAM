
#ifndef CONTAINERS_H
#define CONTAINERS_H
#include "stack.h"

#define TAM_CAMPO 11 // 10 chars + '\0'

typedef struct {
  char id[TAM_CAMPO];
  char name[TAM_CAMPO];
} container_t;

typedef struct {
  char op[3];   // IN || EG + '\0'
  char id[11];  // 10 + '\0'
  char emp[11]; // 10 + '\0'
} lote_t;

int escribirLote(char *nom, lote_t *l, const int cant);
void realizarMovimientos(char *nom, const int cant_cols);

#endif
