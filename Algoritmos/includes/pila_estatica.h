

#ifndef STATIC_STACK_H_INCLUDED
#define STATIC_STACK_H_INCLUDED
#include <types.h>
#define TAM_PILA 1000

typedef enum {
  STACK_SUCCESS = 0,
  STACK_ERR_EMPTY = -1,    /* ERR_COLA_VACIA */
  STACK_ERR_FULL = -2,     /* ERR_COLA_LLENA */
  STACK_ERR_MEM_FULL = -3, /* ERR_MEM_LLENA (no aplica asuí) */
  STACK_ERR_INVAL = -4     /* Parámetros inválidos */
} stack_status_t;

typedef struct {
  unsigned char dato[TAM_PILA];
  unsigned tamDisp;
} stack_t;

void stack_init(stack_t *s);
stack_status_t stack_push(stack_t *s, void *d, unsigned tam);
stack_status_t stack_pull(stack_t *s, void *b, unsigned tam);
stack_status_t stack_see_first(stack_t *s, void *b, unsigned tam);
bool_t stack_is_full(stack_t *s, unsigned tam);
bool_t stack_is_empty(stack_t *s);
void stack_clear(stack_t *s);

#endif
