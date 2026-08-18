#include <lista_simple.h>
#include <stdlib.h>
#include <string.h>
void list_init(list_t *l)
{
	*l = NULL;
}
list_status_t list_push_last(list_t *l, void *d, unsigned tam)
{
	list_d_node_t *nodo = (list_d_node_t *)malloc(sizeof(list_d_node_t)),
		      *aux = *l;
	if (!nodo) {
		return LIST_ERR_MEM_FULL;
	}
	void *dato = malloc(tam);
	if (!dato) {
		free(nodo);
		return LIST_ERR_MEM_FULL;
	}
	memcpy(dato, d, tam);
	nodo->tam = tam;
	nodo->dato = dato;
	nodo->sig = NULL;
	if (*l == NULL) {
		*l = nodo;
	} else {
		while (aux->sig) {
			aux = aux->sig;
		}
		aux->sig = nodo;
	}
	return LIST_SUCCESS;
}
list_status_t list_pull_last(list_t *l, void *b, unsigned tam)
{
	list_d_node_t *aux = *l;
	if (*l == NULL) {
		return LIST_ERR_EMPTY;
	}
	while (aux->sig) {
		aux = aux->sig;
	}
	memcpy(b, aux->dato, MIN(tam, aux->tam));
	free(aux->dato);
	free(aux);
	aux = NULL;
	return LIST_SUCCESS;
}
list_status_t list_push_first(list_t *l, void *d, unsigned tam)
{
	list_d_node_t *nodo = (list_d_node_t *)malloc(sizeof(list_d_node_t)),
		      *aux = *l;
	if (!nodo) {
		return LIST_ERR_MEM_FULL;
	}
	void *dato = malloc(tam);
	if (!dato) {
		free(nodo);
		return LIST_ERR_MEM_FULL;
	}
	memcpy(dato, d, tam);
	nodo->tam = tam;
	nodo->dato = dato;
	if (*l == NULL) {
		*l = nodo;
		nodo->sig = NULL;
	} else {
		nodo->sig = aux;
		*l = nodo;
	}
	return LIST_SUCCESS;
}
list_status_t list_pull_first(list_t *l, void *b, unsigned tam)
{
	list_d_node_t *aux = *l;
	if (*l == NULL) {
		return LIST_ERR_EMPTY;
	}
	memcpy(b, aux->dato, MIN(tam, aux->tam));
	*l = aux->sig;
	free(aux->dato);
	free(aux);
	return LIST_SUCCESS;
}
list_status_t list_push_in_pos(list_t *l, void *d, unsigned tam, int pos)
{
	list_d_node_t *nodo = NULL, *aux = *l;
	void *dato = NULL;
	if (*l == NULL) {
		return LIST_ERR_EMPTY;
	}
	while (aux->sig && pos - 1 != 0) {
		aux = aux->sig;
		pos--;
	}
	if (pos - 1 != 0) {
		return LIST_ERR_INVAL;
	}
	nodo = (list_d_node_t *)malloc(sizeof(list_d_node_t));
	if (!nodo) {
		return LIST_ERR_MEM_FULL;
	}
	dato = malloc(tam);
	if (!dato) {
		free(nodo);
		return LIST_ERR_MEM_FULL;
	}
	memcpy(dato, d, tam);
	nodo->tam = tam;
	nodo->dato = dato;
	nodo->sig = aux->sig;
	aux->sig = nodo;
	return LIST_SUCCESS;
}
list_status_t list_pull_in_pos(list_t *l, void *b, unsigned tam, int pos)
{
	list_d_node_t *aux = *l, *ant = NULL;
	if (*l == NULL) {
		return LIST_ERR_EMPTY;
	}
	while (aux->sig && pos - 1 != 0) {
		aux = aux->sig;
		pos--;
	}
	if (pos - 1 != 0 || aux->sig == NULL) {
		return LIST_ERR_NOT_FOUND;
	}
	ant = aux;
	memcpy(b, aux->sig->dato, MIN(tam, aux->tam));
	ant->sig = aux->sig->sig;
	free(aux->sig->dato);
	free(aux->sig);
	return LIST_SUCCESS;
}
list_status_t list_see_first(list_t *l, void *b, unsigned tam)
{
	list_d_node_t *aux = *l;
	if (*l == NULL) {
		return LIST_ERR_EMPTY;
	}
	memcpy(b, aux->dato, MIN(tam, aux->tam));
	return LIST_SUCCESS;
}
list_status_t list_see_last(list_t *l, void *b, unsigned tam)
{
	list_d_node_t *aux = *l;
	if (*l == NULL) {
		return LIST_ERR_EMPTY;
	}
	while (aux->sig) {
		aux = aux->sig;
	}
	memcpy(b, aux->dato, MIN(tam, aux->tam));
	return LIST_SUCCESS;
}
list_status_t list_reduce(list_t *l, void *param, accion_t accion)
{
	return LIST_SUCCESS;
}
void list_show(list_t *l, prnt_t prnt)
{
	list_d_node_t *aux = *l;
	while (aux) {
		prnt(aux->dato);
		aux = aux->sig;
	}
}
void list_walk(list_t *l, void *b, accion_t accion)
{
	list_d_node_t *aux = *l;
	while (aux) {
		accion(b, aux->dato);
		aux = aux->sig;
	}
}
bool_t list_is_full(list_t *l, unsigned tam)
{
	list_d_node_t *nodo = (list_d_node_t *)malloc(sizeof(list_d_node_t));
	if (!nodo) {
		return TRUE;
	}
	void *dato = malloc(tam);
	if (!dato) {
		free(nodo);
		return TRUE;
	}
	free(nodo);
	free(dato);
	return FALSE;
}
bool_t list_is_empty(list_t *l)
{
	return *l == NULL ? TRUE : FALSE;
}
void list_clear(list_t *l)
{
	list_d_node_t *aux;
	while (*l) {
		aux = (*l)->sig;
		free((*l)->dato);
		free(*l);
		*l = aux;
	}
	*l = NULL;
}
