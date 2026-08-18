#include <cola_dinamica.h>
#include <stdlib.h>
#include <string.h>
void queue_init(queue_t *q)
{
	q->pri = NULL;
	q->ult = NULL;
}
queue_status_t queue_push(queue_t *q, void *d, unsigned tam)
{
	queue_d_node_t *nodo = malloc(sizeof(queue_d_node_t));
	if (!nodo) {
		return QUEUE_ERR_MEM_FULL;
	}
	void *dato = malloc(tam);
	if (!dato) {
		free(nodo);
		return QUEUE_ERR_MEM_FULL;
	}
	memcpy(dato, d, tam);
	nodo->tam = tam;
	nodo->data = dato;
	nodo->sig = NULL;
	if (q->pri == NULL) {
		q->pri = nodo;
	} else {
		q->ult->sig = nodo;
	}

	q->ult = nodo;

	return QUEUE_SUCCESS;
}
queue_status_t queue_pull(queue_t *q, void *b, unsigned tam)
{
	if (q->pri == NULL) {
		return QUEUE_ERR_EMPTY;
	}
	queue_d_node_t *pri = q->pri;
	memcpy(b, pri->data, MIN(tam, pri->tam));
	if (!pri->sig) {
		q->pri = NULL;
		q->ult = NULL;
	} else {
		q->pri = pri->sig;
	}
	free(pri->data);
	free(pri);
	return QUEUE_SUCCESS;
}
queue_status_t queue_see_first(queue_t *q, void *b, unsigned tam)
{
	if (q->pri == NULL) {
		return QUEUE_ERR_EMPTY;
	}
	queue_d_node_t *pri = q->pri;
	memcpy(b, pri->data, MIN(tam, pri->tam));
	return QUEUE_SUCCESS;
}
bool_t queue_is_full(queue_t *q, unsigned tam)
{
	(void)q;
	queue_d_node_t *nodo = malloc(sizeof(queue_d_node_t));
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
bool_t queue_is_empty(queue_t *q)
{
	return q->pri == NULL ? TRUE : FALSE;
}
queue_status_t queue_clear(queue_t *q)
{
	while (q->pri) {
		queue_d_node_t *aux = q->pri;
		q->pri = aux->sig;
		free(aux->data);
		free(aux);
	}
	q->ult = NULL;

	return QUEUE_SUCCESS;
}
