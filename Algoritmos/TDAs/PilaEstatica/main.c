#include <pila_estatica.h>
#include <string.h>

void stack_init(stack_t *s)
{
	s->tamDisp = TAM_PILA;
}
stack_status_t stack_push(stack_t *s, void *d, unsigned tam)
{
	if (s->tamDisp < tam + sizeof(tam)) {
		return STACK_ERR_MEM_FULL;
	}
	s->tamDisp -= tam;
	memcpy(s->dato + s->tamDisp, d, tam);
	s->tamDisp -= sizeof(tam);
	memcpy(s->dato + s->tamDisp, &tam, sizeof(tam));
	return STACK_SUCCESS;
}
stack_status_t stack_pull(stack_t *s, void *b, unsigned tam)
{
	unsigned tamInfo;
	if (s->tamDisp == TAM_PILA) {
		return STACK_ERR_EMPTY;
	}
	memcpy(&tamInfo, s->dato + s->tamDisp, sizeof(unsigned));
	s->tamDisp += sizeof(unsigned);
	memcpy(b, s->dato + s->tamDisp, MIN(tam, tamInfo));
	s->tamDisp += tamInfo;
	return STACK_SUCCESS;
}
stack_status_t stack_see_first(stack_t *s, void *b, unsigned tam)
{
	unsigned tamInfo, tamDispAux = s->tamDisp;
	if (s->tamDisp == TAM_PILA) {
		return STACK_ERR_EMPTY;
	}
	memcpy(&tamInfo, s->dato + tamDispAux, sizeof(unsigned));
	tamDispAux += sizeof(unsigned);
	memcpy(b, s->dato + tamDispAux, MIN(tam, tamInfo));
	return STACK_SUCCESS;
}
bool_t stack_is_full(stack_t *s, unsigned tam)
{
	return s->tamDisp < tam + sizeof(tam) ? TRUE : FALSE;
}
bool_t stack_is_empty(stack_t *s)
{
	return s->tamDisp == TAM_PILA ? TRUE : FALSE;
}
void stack_clear(stack_t *s)
{
	s->tamDisp = TAM_PILA;
}
