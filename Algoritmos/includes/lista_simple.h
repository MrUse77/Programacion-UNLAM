

#ifndef LISTA_SIMPLE_H_INCLUDED
#define LISTA_SIMPLE_H_INCLUDED
#include <types.h>

typedef enum {
  LIST_SUCCESS = 0,
  LIST_ERR_EMPTY = -1,    /* ERR_COLA_VACIA */
  LIST_ERR_FULL = -2,     /* ERR_COLA_LLENA */
  LIST_ERR_MEM_FULL = -3, /* ERR_MEM_LLENA (no aplica asuí) */
  LIST_ERR_INVAL = -4,    /* Parámetros inválidos */
  LIST_ERR_NOT_FOUND = -5,
} list_status_t;

typedef struct list_d_node_s {
  void *dato;
  unsigned tam;
  struct list_d_node_s *sig;
} list_d_node_t;

typedef list_d_node_t *list_t;

void list_init(list_t *l);
list_status_t list_push_last(list_t *l, void *d, unsigned tam);
list_status_t list_pull_last(list_t *l, void *b, unsigned tam);
list_status_t list_push_first(list_t *l, void *d, unsigned tam);
list_status_t list_pull_first(list_t *l, void *b, unsigned tam);
list_status_t list_push_in_pos(list_t *l, void *d, unsigned tam, int pos);
list_status_t list_pull_in_pos(list_t *l, void *b, unsigned tam, int pos);
list_status_t list_see_first(list_t *l, void *b, unsigned tam);
list_status_t list_see_last(list_t *l, void *b, unsigned tam);
list_status_t list_reduce(list_t *l, void *param, accion_t accion);
void list_show(list_t *l, prnt_t prnt);
void list_walk(list_t *l, void *b, accion_t accion);
bool_t list_is_full(list_t *l, unsigned tam);
bool_t list_is_empty(list_t *l);
void list_clear(list_t *l);

#endif
