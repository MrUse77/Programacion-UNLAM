#include "circular_queue.h"
#include <stdlib.h>
#include <string.h>

/* =============================================================================
   Implementación Circular - Cola Dinámica con Linked List Circular
   Uso la estructura queue_t definida en Cola.h: typedef queue_node_t *queue_t;
   Cada nodo es un puntero a struct queue_node
   ============================================================================= */

void queue_create(queue_t *c)
{
	/* Inicializa cola vacía: prim y ult apuntan al mismo nodo NULL */
	*c = NULL;
}

int queue_push(queue_t *c, const void *d, unsigned tamDato)
{
	queue_node_t *n = (queue_node_t *)malloc(sizeof(queue_node_t));

	if (!n) {
		return QUEUE_ERR_FULL;
	}

	n->dato = malloc(tamDato);
	if (!n->dato) {
		free(n);
		return QUEUE_ERR_FULL;
	}

	memcpy(n->dato, d, tamDato);
	n->tamDato = tamDato;

	/* Insertar al final de la lista circular */
	if (*c == NULL) {
		n->sig = n;
		*c = n;
	} else {
		n->sig = (*c)->sig;
		(*c)->sig = n;
		*c = n;
	}

	return EXIT_SUCCESS;
}

int queue_is_full(queue_t *c, unsigned tam)
{
	(void)c;
	queue_node_t *n = (queue_node_t *)malloc(sizeof(queue_node_t));
	n->dato = malloc(tam);
	if (!n || !n->dato) {
		free(n);
		return TRUE;
	}
	free(n->dato);
	free(n);
	return FALSE;
}

int queue_see_first(queue_t *c, void *buff, unsigned tamDato)
{
	if (*c) {
		return QUEUE_ERR_EMPTY;
	}

	/* En cola circular: el primer dato está en prim->dato */
	memcpy(buff, (*c)->sig->dato, MIN(tamDato, (*c)->sig->tamDato));
	return EXIT_SUCCESS;
}

int queue_pull(queue_t *c, void *buff, unsigned tamDato)
{
	if (*c == NULL) {
		return QUEUE_ERR_EMPTY;
	}
	queue_node_t *aux = (*c)->sig;
	memcpy(buff, aux->dato, MIN(tamDato, aux->tamDato));
	if (aux == *c) {
		*c = NULL;
	} else {
		(*c)->sig = aux->sig;
	}
	free(aux->dato);
	free(aux);
	return EXIT_SUCCESS;
}

int queue_is_empty(queue_t *c)
{
	return *c == NULL ? TRUE : FALSE;
}

void queue_clear(queue_t *c)
{
	if (*c == NULL) {
		/* Cola ya vacía */
		return;
	}

	/* Liberar todos los nodos recorriendo la lista circular */
	queue_node_t *aux =
		(*c)->sig; /* Usar queue_node_t en lugar de queue_node_t */

	while (aux != NULL) {
		queue_node_t *sig =
			aux->sig; /* Guardar siguiente antes de liberar */

		if (aux->dato != NULL) {
			free(aux->dato); /* Liberar datos del nodo */
		}
		free(aux); /* Liberar el nodo */
		aux = sig; /* Avanzar al siguiente */
	}
	*c = NULL;
}
