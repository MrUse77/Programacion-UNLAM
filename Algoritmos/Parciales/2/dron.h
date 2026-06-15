#ifndef DRON_H
#define DRON_H
#include "queue.h"
#define NORTE 'N'
#define SUR 'S'
#define OESTE 'O'
#define ESTE 'E'

typedef struct {
  char dir;
  queue_t queue_sec;
} dron_t;

int guardar_secuencia(char *nom, dron_t *d);
int procesar_secuencia(dron_t *d, queue_t *sec);

#endif
