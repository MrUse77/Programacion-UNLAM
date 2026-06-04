#include "Pila.h"
#include <string.h>
#include <string.h>
#include <stdlib.h>
#include "Comun.h"

void stack_init(stack_t *p)
{
	*p = NULL;
}

int stack_is_full(stack_t *p, unsigned tam)
{
	stack_node_t *aux = (stack_node_t *)malloc(sizeof(stack_node_t));
	void *info = malloc(tam);
	free(aux);
	free(info);
	return aux == NULL || info == NULL;
}

int stack_push(stack_t *p, const void *d, unsigned tam)
{
	stack_node_t *nue = (stack_node_t *)malloc(sizeof(stack_node_t));
	nue->dato = malloc(tam);
	if (nue == NULL || nue->dato == NULL) {
		free(nue);
		return ERR_MEM_LLENA;
	}
	memcpy(nue->dato, d, tam);
	nue->tamDato = tam;
	if (*p == NULL) {
		nue->sig = nue;
		*p = nue;
	} else {
		nue->sig = (*p)->sig;
		(*p)->sig = nue;
	}
	return OK;
}

int stack_is_empty(stack_t *p)
{
	return *p == NULL ? TRUE : FALSE;
}

void stack_clear(stack_t *p)
{
	stack_node_t *n = *p;
	free(n->dato);
	free(n);
	*p = NULL;
}

int stack_pull(stack_t *p, void *buffer, unsigned tam)
{
	if (*p == NULL) {
		return ERR_PILA_VACIA;
	}
	stack_node_t *aux = (*p)->sig;
	memcpy(buffer, aux->dato, MIN(aux->tamDato, tam));
	if (aux == *p) {
		*p = NULL;
	} else {
		(*p)->sig = aux->sig;
	}
	free(aux->dato);
	free(aux);
	return OK;
}

int stack_see_top(stack_t *p, void *buffer, unsigned tam)
{
	if (*p == NULL) {
		return ERR_PILA_VACIA;
	}
	stack_node_t *n = *p;
	memcpy(buffer, n->dato, MIN(n->tamDato, tam));
	return OK;
}
