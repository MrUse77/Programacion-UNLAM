#include <cola_estatica.h>
#include <string.h>

void queue_init(queue_t *q)
{
	q->pri = TAM_COLA / 2;
	q->ult = q->pri;
	q->tam_disp = TAM_COLA;
}

queue_status_t queue_push(queue_t *q, const void *d, unsigned tam)
{
	unsigned int ini, fin;
	if (q->tam_disp < tam + sizeof(tam)) {
		return QUEUE_ERR_MEM_FULL;
	}
	q->tam_disp -= tam + sizeof(tam);
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

queue_status_t queue_pop(queue_t *q, void *b, unsigned tam)
{
	int ini, fin;
	unsigned tam_info;
	if (q->tam_disp == TAM_COLA) {
		return QUEUE_ERR_EMPTY;
	}
	if ((ini = MIN(sizeof(tam), TAM_COLA - q->pri)) != 0) {
		memcpy(&tam_info, q->dato + q->pri, ini);
	}
	if ((fin = sizeof(tam) - ini) != 0) {
		memcpy(((char *)&tam_info) + ini, q->dato, fin);
	}
	q->pri = fin ? fin : q->pri + ini;
	q->tam_disp += tam_info + sizeof(unsigned);
	if ((ini = MIN(MIN(tam_info, tam), TAM_COLA - q->pri)) != 0) {
		memcpy((char *)b, q->dato + q->pri, ini);
	}
	if ((fin = tam_info - ini) != 0) {
		memcpy(((char *)b) + ini, q->dato, fin);
	}
	q->pri = fin ? fin : q->pri + ini;
	return QUEUE_SUCCESS;
}

queue_status_t queue_see_first(queue_t *q, void *b, unsigned tam)
{
	int ini, fin;
	unsigned tam_info, pri;
	if (q->tam_disp == TAM_COLA) {
		return QUEUE_ERR_EMPTY;
	}
	if ((ini = MIN(sizeof(tam), TAM_COLA - q->pri)) != 0) {
		memcpy(&tam_info, q->dato + q->pri, ini);
	}
	if ((fin = sizeof(tam) - ini) != 0) {
		memcpy(((char *)&tam_info) + ini, q->dato, fin);
	}
	pri = fin ? fin : q->ult + ini;
	tam_info = MIN(tam_info, tam);
	if ((ini = MIN(tam_info, TAM_COLA - pri)) != 0) {
		memcpy(b, q->dato + pri, ini);
	}
	if ((fin = tam_info - ini) != 0) {
		memcpy(((char *)b) + ini, q->dato, fin);
	}
	return QUEUE_SUCCESS;
}

bool_t queue_is_full(queue_t *q, unsigned tam)
{
	return q->tam_disp < tam + sizeof(tam) ? TRUE : FALSE;
}

bool_t queue_is_empty(queue_t *q)
{
	return q->tam_disp == TAM_COLA ? TRUE : FALSE;
}

void queue_clear(queue_t *q)
{
	q->ult = q->pri;
	q->tam_disp = TAM_COLA;
}
