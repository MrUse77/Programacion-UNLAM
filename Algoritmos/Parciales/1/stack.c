#include "stack.h"
#include <stdlib.h>
#include <string.h>
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int stack_init(stack_t *s, const unsigned tam)
{
	s->info = malloc(tam);
	if (!s->info) {
		return ERR_MEM;
	}
	s->tam = tam;
	return OK;
}
int stack_push(stack_t *s, const void *d, const unsigned tam)
{
	if (s->tam < tam + sizeof(tam)) {
		return ERR_MEM;
	}
	s->tam -= tam;
	memcpy(s->info + s->tam, d, tam);
	s->tam -= sizeof(tam);
	memcpy(s->info + s->tam, &tam, sizeof(tam));
	return OK;
}
int stack_pull(stack_t *s, void *buff, const unsigned tam)
{
	if (s->tam == sizeof(s->info)) {
		return ERR_VACIO;
	}
	unsigned tamAux;
	memcpy(&tamAux, s->info + s->tam, sizeof(tam));
	s->tam += sizeof(tam);
	memcpy(buff, s->info + s->tam, MIN(tamAux, tam));
	s->tam += tam;
	return OK;
}
int stack_is_full(const stack_t *s, const unsigned tam)
{
	return s->tam < tam + sizeof(tam) ? TRUE : FALSE;
}
int stack_is_empty(const stack_t *s)
{
	return s->tam == sizeof(s->info) ? TRUE : FALSE;
}
int stack_see_first(stack_t *s, void *buff, const unsigned tam)
{
	if (s->tam == sizeof(s->info)) {
		return ERR_VACIO;
	}
	unsigned tamAux;
	memcpy(&tamAux, s->info + s->tam, sizeof(tam));
	memcpy(buff, s->info + s->tam, MIN(tamAux, tam));
	return OK;
}
void stack_clear(stack_t *s)
{
	s->tam = sizeof(s->info);
	free(s->info);
}
