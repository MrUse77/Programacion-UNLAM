

#ifndef DYNAMIC_STACK_H_INCLUDED
#define DYNAMIC_STACK_H_INCLUDED
#include <types.h>

typedef enum {
  STACK_SUCCESS = 0,
  STACK_ERR_EMPTY = -1,    /* ERR_COLA_VACIA */
  STACK_ERR_FULL = -2,     /* ERR_COLA_LLENA */
  STACK_ERR_MEM_FULL = -3, /* ERR_MEM_LLENA (no aplica asuí) */
  STACK_ERR_INVAL = -4     /* Parámetros inválidos */
} stack_status_t;

typedef struct stack_d_node_s {
  void *dato;
  unsigned tam;
  struct stack_d_node_s *sig;
} stack_d_node_t;

typedef stack_d_node_t *stack_t;

void stack_init(stack_t *s);
stack_status_t stack_push(stack_t *s, void *d, unsigned tam);
stack_status_t stack_pull(stack_t *s, void *b, unsigned tam);
stack_status_t stack_see_first(stack_t *s, void *b, unsigned tam);
bool_t stack_is_full(stack_t *s, unsigned tam);
bool_t stack_is_empty(stack_t *s);
void stack_clear(stack_t *s);

#endif
