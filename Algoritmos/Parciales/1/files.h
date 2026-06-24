#ifndef FILES_H
#define FILES_H
#include "containers.h"
#include "stack.h"

int escribirLote(char *nom, lote_t *l, const int cant);
void cargarLote(char *nom, stack_t *s);

#endif
