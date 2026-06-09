#include "static_queue.h"
#include <string.h>

/* =============================================================================
   Implementación Estática - Cola con Array Fijo en Heap (TAM_COLA elementos)
   Cada elemento es un puntero a datos almacenados externamente
   ============================================================================= */

#define TAM_COLA 100000

void queue_create(queue_t *c)
{
	/* Inicializar cola estática vacía */
	c->prim = 0; /* Índice del primer elemento = 0 (vacío) */
	c->ult = -1; /* Índice del último elemento = -1 (vacío) */
	c->tamDisp = TAM_COLA; /* Espacio disponible inicial */
}

int queue_push(queue_t *c, const void *d, unsigned tamDato)
{
	if (c->tamDisp == 0) {
		return QUEUE_ERR_FULL; /* No hay espacio para el elemento */
	}

	unsigned pos = c->ult + 1;

	/* Ajuste circular cuando llegamos al final del array */
	if (pos >= TAM_COLA) {
		pos = 0;
	}

	c->ult = pos;
	memcpy(c->Vector[pos], d, tamDato);
	c->tamDisp -= tamDato;

	return QUEUE_SUCCESS;
}

int queue_is_full(queue_t *c, unsigned tam)
{
	/* Para estática: cola llena cuando no hay espacio disponible */
	return (c->tamDisp < tam) ? TRUE : FALSE;
}

int queue_see_first(queue_t *c, void *buff, unsigned tamDato)
{
	if (c->tamDisp == TAM_COLA) {
		return QUEUE_ERR_EMPTY;
	}

	/* El primer elemento está en c->prim */
	unsigned pos = c->prim;

	/* Ajuste circular */
	while (pos >= TAM_COLA) {
		pos -= TAM_COLA;
	}

	memcpy(buff, c->Vector[pos], MIN(tamDato, tamDato));
	return QUEUE_SUCCESS;
}

int queue_pull(queue_t *c, void *buff, unsigned tamDato)
{
	if (c->tamDisp == TAM_COLA) {
		return QUEUE_ERR_EMPTY;
	}

	/* Obtener datos del primer elemento */
	unsigned pos = c->prim;

	/* Ajuste circular */
	while (pos >= TAM_COLA) {
		pos -= TAM_COLA;
	}

	memcpy(buff, c->Vector[pos], MIN(tamDato, tamDato));

	/* Avanzar el primer índice */
	c->prim++;

	/* Ajuste circular */
	if (c->prim >= TAM_COLA) {
		c->prim = 0;
	}

	c->tamDisp += tamDato;

	return QUEUE_SUCCESS;
}

int queue_is_empty(queue_t *c)
{
	return (c->tamDisp == TAM_COLA) ? TRUE : FALSE;
}

void queue_clear(queue_t *c)
{
	/* Reiniciar cola a estado vacío */
	c->prim = 0;
	c->ult = -1;
	c->tamDisp = TAM_COLA;
}
