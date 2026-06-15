#ifndef STACK_H
#define STACK_H

#define OK 0
#define ERR_MEM -1
#define ERR_VACIO -2
#define TRUE 1
#define FALSE 0

typedef struct {
  char *info;
  unsigned tam;
} stack_t;

int stack_init(stack_t *s, const unsigned tam);
int stack_push(stack_t *s, const void *d, const unsigned tam);
int stack_pull(stack_t *s, void *buff, const unsigned tam);
int stack_is_full(const stack_t *s, const unsigned tam);
int stack_is_empty(const stack_t *s);
int stack_see_first(stack_t *s, void *buff, const unsigned tam);
void stack_clear(stack_t *s);

#endif
