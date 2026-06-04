#include <Cola.h>
#include <Comun.h>
#include <stdlib.h>
#include <string.h>

/**
 * @def MIN
 * @brief verifica cual es el menor entre 2 elementos
 */
#define MIN(a, b) (a > b ? b : a)

void queue_create(queue_t *c)
{
	c->prim = NULL;
	c->ult = NULL;
}

int queue_push(queue_t *c, const void *d, unsigned tamDato)
{
	queue_node_t **ult = &c->ult;
	queue_node_t *n = (queue_node_t *)malloc(sizeof(queue_node_t));
	n->dato = malloc(tamDato);
	if (!n || !n->dato) {
		free(n);
		return ERR_MEM_LLENA;
	}
	memcpy(n->dato, d, tamDato);
	n->tamDato = tamDato;
	n->sig = NULL;
	if (c->ult != NULL) {
		(*ult)->sig = n;
	} else {
		c->prim = n;
	}
	c->ult = n;
	return OK;
}

int queue_is_full(queue_t *c, unsigned tam)
{
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
	if (c->prim == NULL) {
		return ERR_COLA_VACIA;
	}
	memcpy(buff, c->prim->dato, MIN(tamDato, c->prim->tamDato));
	return OK;
}

int queue_pull(queue_t *c, void *buff, unsigned tamDato)
{
	if (c->prim == NULL) {
		return ERR_COLA_VACIA;
	}
	queue_node_t *prim = c->prim;
	c->prim = prim->sig;
	memcpy(buff, prim->dato, MIN(tamDato, prim->tamDato));
	free(prim->dato);
	free(prim);
	if (c->prim == NULL) {
		c->ult = NULL;
	}
	return OK;
}

int queue_is_empty(queue_t *c)
{
	return c->prim == NULL ? TRUE : FALSE;
}

void queue_clear(queue_t *c)
{
	queue_node_t *prim = c->prim;
	while (prim) {
		queue_node_t *aux = prim;
		prim = aux->sig;
		free(aux->dato);
		free(aux);
	}
	c->ult = NULL;
}
