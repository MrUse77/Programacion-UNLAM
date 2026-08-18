#ifndef DYNAMIC_QUEUE_H_INCLUDED
#define DYNAMIC_QUEUE_H_INCLUDED
#include <types.h>

typedef enum {
  QUEUE_SUCCESS = 0,
  QUEUE_ERR_EMPTY = -1,    /* ERR_COLA_VACIA */
  QUEUE_ERR_FULL = -2,     /* ERR_COLA_LLENA */
  QUEUE_ERR_MEM_FULL = -3, /* ERR_MEM_LLENA (no aplica aquí) */
  QUEUE_ERR_INVAL = -4     /* Parámetros inválidos */
} queue_status_t;

typedef struct queue_d_node_s {
  void *data;
  unsigned tam;
  struct queue_d_node_s *sig;
} queue_d_node_t;

typedef struct {
  queue_d_node_t *pri, *ult;
} queue_t;

void queue_init(queue_t *q);
queue_status_t queue_push(queue_t *q, void *d, unsigned tam);
queue_status_t queue_pull(queue_t *q, void *b, unsigned tam);
queue_status_t queue_see_first(queue_t *q, void *b, unsigned tam);
bool_t queue_is_full(queue_t *q, unsigned tam);
bool_t queue_is_empty(queue_t *q);
queue_status_t queue_clear(queue_t *q);

#endif
