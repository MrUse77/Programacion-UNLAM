#include <pila_dinamica.h>
#include <stdlib.h>
#include <string.h>
void stack_init(stack_t *s)
{
	*s = NULL;
}
stack_status_t stack_push(stack_t *s, void *d, unsigned tam)
{
	stack_d_node_t *nodo = (stack_d_node_t *)malloc(sizeof(stack_d_node_t));
	if (!nodo) {
		return STACK_ERR_MEM_FULL;
	}
	void *dato = malloc(tam);
	if (!dato) {
		free(nodo);
		return STACK_ERR_MEM_FULL;
	}
	memcpy(dato, d, tam);
	nodo->tam = tam;
	nodo->dato = dato;
	if (*s != NULL) {
		nodo->sig = *s;
	} else {
		nodo->sig = NULL;
	}
	*s = nodo;
	return STACK_SUCCESS;
}
stack_status_t stack_pull(stack_t *s, void *b, unsigned tam)
{
	stack_d_node_t *nodo_aux = *s;
	if (*s == NULL) {
		return STACK_ERR_EMPTY;
	}
	memcpy(b, (*s)->dato, MIN((*s)->tam, tam));
	if (nodo_aux->sig != NULL) {
		*s = nodo_aux->sig;
	} else {
		*s = NULL;
	}
	free(nodo_aux->dato);
	free(nodo_aux);
	return STACK_SUCCESS;
}
stack_status_t stack_see_first(stack_t *s, void *b, unsigned tam)
{
	if (*s == NULL) {
		return STACK_ERR_EMPTY;
	}
	memcpy(b, (*s)->dato, MIN((*s)->tam, tam));
	return STACK_SUCCESS;
}
bool_t stack_is_full(stack_t *s, unsigned tam)
{
	stack_d_node_t *nodo_aux =
		(stack_d_node_t *)malloc(sizeof(stack_d_node_t));
	if (nodo_aux == NULL) {
		return TRUE;
	}
	void *dato_aux = malloc(tam);
	if (dato_aux == NULL) {
		free(nodo_aux);
		return TRUE;
	}
	free(nodo_aux);

	free(dato_aux);

	return FALSE;
}
bool_t stack_is_empty(stack_t *s)
{
	return *s == NULL ? TRUE : FALSE;
}
void stack_clear(stack_t *s)
{
	while (*s != NULL) {
		stack_d_node_t *nodo_aux = *s;
		*s = nodo_aux->sig;
		free(nodo_aux->dato);
		free(nodo_aux);
	}
}
