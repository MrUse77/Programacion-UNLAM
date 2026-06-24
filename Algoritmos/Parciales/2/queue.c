#include "queue.h"
#include <string.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

void queue_init(queue_t *q)
{
	q->prim = TAM_QUEUE / 2;
	q->ult = q->prim;
	q->tamDisp = TAM_QUEUE;
}

int queue_push(queue_t *q, const void *d, const unsigned tam)
{
	if (q->tamDisp < tam + sizeof(tam)) {
		return -1;
	}
	if (q->ult + sizeof(tam) < TAM_QUEUE) {
		memcpy(q->info + q->ult, &tam, sizeof(tam));
		q->ult += sizeof(tam);
		q->tamDisp -= sizeof(tam);
	} else {
		memcpy(q->info + q->ult, &tam, TAM_QUEUE - q->ult);
		memcpy(q->info, &tam, sizeof(tam) - (TAM_QUEUE - q->ult));
		q->ult = sizeof(tam) - (TAM_QUEUE - q->ult);
		q->tamDisp -= sizeof(tam);
	}
	if (tam + q->ult < TAM_QUEUE) {
		memcpy(q->info + q->ult, d, tam);
		q->ult += tam;
		q->tamDisp -= tam;
	} else {
		memcpy(q->info + q->ult, d, TAM_QUEUE - q->ult);
		memcpy(q->info, d, tam - (TAM_QUEUE - q->ult));
		q->ult = tam - (TAM_QUEUE - q->ult);
		q->tamDisp -= tam;
	}
	return 0;
}
int queue_pull(queue_t *q, void *buff, const unsigned tam)
{
	unsigned tam_aux, min;
	if (q->tamDisp == TAM_QUEUE) {
		return -1;
	}
	if (q->prim + sizeof(tam) < TAM_QUEUE) {
		memcpy(&tam_aux, q->info + q->prim, sizeof(tam));
		q->prim += sizeof(tam);
	} else {
		memcpy(&tam_aux, q->info + q->prim, TAM_QUEUE - q->prim);
		memcpy(&tam_aux + TAM_QUEUE - q->prim, q->info,
		       sizeof(tam) - (TAM_QUEUE - q->prim));
		q->prim = sizeof(tam) - (TAM_QUEUE - q->prim);
	}
	q->tamDisp += sizeof(tam);
	min = MIN(tam_aux, tam);
	if (tam + q->ult < TAM_QUEUE) {
		memcpy(buff, q->info + q->prim, min);
		q->prim += min;
	} else {
		memcpy(buff, q->info + q->prim, TAM_QUEUE - q->prim);
		memcpy(buff + TAM_QUEUE - q->prim, q->info,
		       min - (TAM_QUEUE - q->prim));
		q->ult = tam - (TAM_QUEUE - q->prim);
	}
	q->tamDisp += tam;
	return 0;
}
int queue_see_first(queue_t *q, void *buff, const unsigned tam)
{
	unsigned tam_aux, min, prim = q->prim;
	if (q->tamDisp == TAM_QUEUE) {
		return -1;
	}
	if (prim + sizeof(tam) < TAM_QUEUE) {
		memcpy(&tam_aux, q->info + prim, sizeof(tam));
		prim += sizeof(tam);
	} else {
		memcpy(&tam_aux, q->info + prim, TAM_QUEUE - prim);
		memcpy(&tam_aux + TAM_QUEUE - prim, q->info,
		       sizeof(tam) - (TAM_QUEUE - prim));
		prim = sizeof(tam) - (TAM_QUEUE - prim);
	}
	min = MIN(tam_aux, tam);
	if (tam + prim < TAM_QUEUE) {
		memcpy(buff, q->info + prim, min);
	} else {
		memcpy(buff, q->info + prim, TAM_QUEUE - q->prim);
		memcpy(buff + TAM_QUEUE - prim, q->info,
		       min - (TAM_QUEUE - prim));
	}
	return 0;
}
int queue_is_empty(queue_t *q)
{
	return (q->tamDisp == TAM_QUEUE) ? 1 : 0;
}
int queue_is_full(queue_t *q, const unsigned tam)
{
	return (q->tamDisp < tam + sizeof(tam)) ? 1 : 0;
}
void queue_clear(queue_t *q)
{
	q->tamDisp = TAM_QUEUE;
	q->ult = q->prim;
}
