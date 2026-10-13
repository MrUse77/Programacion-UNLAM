

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
  LIST_FOUND = 1,
} list_status_t;

typedef struct list_d_node_s {
  void *dato;
  unsigned tam;
  struct list_d_node_s *sig;
} list_d_node_t;

typedef list_d_node_t *list_t;

void list_init(list_t *l);
list_status_t list_push_last(list_t *l, void *d, unsigned tam);
list_status_t list_pop_last(list_t *l, void *b, unsigned tam);
list_status_t list_push_first(list_t *l, void *d, unsigned tam);
list_status_t list_pop_first(list_t *l, void *b, unsigned tam);
list_status_t list_push_in_pos(list_t *l, void *d, unsigned tam, int pos);
list_status_t list_pop_in_pos(list_t *l, void *b, unsigned tam, int pos);
list_status_t list_push_in_order(list_t *l, void *d, unsigned tam, cmp_t cmp);
list_status_t list_pop_in_order(list_t *l, void *b, unsigned tam, cmp_t cmp);
list_status_t list_see_first(list_t *l, void *b, unsigned tam);
list_status_t list_see_last(list_t *l, void *b, unsigned tam);
list_status_t list_reduce(list_t *l, void *param, accion_t accion);
list_status_t list_map(const list_t *l, list_t *new_list, const void *param,
                       accion_t accion);
list_status_t list_filter(const list_t *l, list_t *new_list, const void *param,
                          cmp_t cmp);
void list_show(list_t *l, prnt_t prnt);
void list_foreach(list_t *l, void *b, accion_t accion);
list_status_t list_order(list_t *l, const order_t ordenamiento, cmp_t cmp);
bool_t list_is_full(list_t *l, unsigned tam);
bool_t list_is_empty(list_t *l);
void list_clear(list_t *l);
list_status_t list_find(list_t *l, void *param, unsigned tam, cmp_t cmp,
                        bool_t elim);
list_status_t list_top(list_t *l, list_t *l_top, int top);

#endif
