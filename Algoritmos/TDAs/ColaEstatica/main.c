#include <cola_estatica.h>
#include <string.h>

void queue_init(queue_t *q)
{
	q->pri = TAM_COLA / 2;
	q->ult = q->pri;
	q->tamDisp = TAM_COLA;
}

queue_status_t queue_push(queue_t *q, void *d, unsigned tam)
{
	int ini, fin;
	if (q->tamDisp < tam + sizeof(tam)) {
		return QUEUE_ERR_MEM_FULL;
	}
	q->tamDisp -= tam + sizeof(tam);
	if ((ini = MIN(sizeof(tam), TAM_COLA - q->ult)) != 0) {
		memcpy(q->dato + q->ult, &tam, ini);
	}
	if ((fin = sizeof(tam) - ini) != 0) {
		memcpy(q->dato, ((char *)&tam) + ini, fin);
	}
	q->ult = fin ? fin : q->ult + ini;
	if ((ini = MIN(tam, TAM_COLA - q->ult)) != 0) {
		memcpy(q->dato + q->ult, d, ini);
	}
	if ((fin = tam - ini) != 0) {
		memcpy(q->dato, ((char *)d) + ini, fin);
	}
	q->ult = fin ? fin : q->ult + ini;
	return QUEUE_SUCCESS;
}

queue_status_t queue_pull(queue_t *q, void *b, unsigned tam)
{
	int ini, fin;
	unsigned tamInfo;
	if (q->tamDisp == TAM_COLA) {
		return QUEUE_ERR_EMPTY;
	}
	if ((ini = MIN(sizeof(tam), TAM_COLA - q->pri)) != 0) {
		memcpy(&tamInfo, q->dato + q->pri, ini);
	}
	if ((fin = sizeof(tam) - ini) != 0) {
		memcpy(((char *)&tamInfo) + ini, q->dato, fin);
	}
	q->pri = fin ? fin : q->pri + ini;
	tamInfo = MIN(tamInfo, tam);
	q->tamDisp += tamInfo + sizeof(unsigned);
	if ((ini = MIN(tamInfo, TAM_COLA - q->pri)) != 0) {
		memcpy(b, q->dato + q->pri, ini);
	}
	if ((fin = tamInfo - ini) != 0) {
		memcpy(((char *)b) + ini, q->dato, fin);
	}
	q->pri = fin ? fin : q->pri + ini;
	return QUEUE_SUCCESS;
}

queue_status_t queue_see_first(queue_t *q, void *b, unsigned tam)
{
	int ini, fin;
	unsigned tamInfo, pri;
	if (q->tamDisp == TAM_COLA) {
		return QUEUE_ERR_EMPTY;
	}
	if ((ini = MIN(sizeof(tam), TAM_COLA - q->pri)) != 0) {
		memcpy(&tamInfo, q->dato + q->pri, ini);
	}
	if ((fin = sizeof(tam) - ini) != 0) {
		memcpy(((char *)&tamInfo) + ini, q->dato, fin);
	}
	pri = fin ? fin : q->ult + ini;
	tamInfo = MIN(tamInfo, tam);
	if ((ini = MIN(tamInfo, TAM_COLA - pri)) != 0) {
		memcpy(b, q->dato + pri, ini);
	}
	if ((fin = tamInfo - ini) != 0) {
		memcpy(((char *)b) + ini, q->dato, fin);
	}
	return QUEUE_SUCCESS;
}
bool_t queue_is_full(queue_t *q, unsigned tam)
{
	return q->tamDisp < tam + sizeof(tam) ? TRUE : FALSE;
}
bool_t queue_is_empty(queue_t *q)
{
	return q->tamDisp == TAM_COLA ? TRUE : FALSE;
}
void queue_clear(queue_t *q)
{
	q->ult = q->pri;
	q->tamDisp = TAM_COLA;
}
