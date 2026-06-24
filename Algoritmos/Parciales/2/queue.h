#ifndef QUEUE_H
#define QUEUE_H
#define TAM_QUEUE 5000 // 5 bytes por movimiento (1 por dir, 4 por sizeof)

typedef struct {
  char info[TAM_QUEUE];
  unsigned prim, ult, tamDisp;
} queue_t;

void queue_init(queue_t *q);
int queue_push(queue_t *q, const void *d, const unsigned tam);
int queue_pull(queue_t *q, void *buff, const unsigned tam);
int queue_see_first(queue_t *q, void *buff, const unsigned);
int queue_is_empty(queue_t *q);
int queue_is_full(queue_t *q, const unsigned tam);
void queue_clear(queue_t *q);

#endif
