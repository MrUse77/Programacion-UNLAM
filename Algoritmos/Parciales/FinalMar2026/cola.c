#include "cola.h"
#include <stdlib.h>
#include <string.h>
#define MIN(a, b) ((a) < (b) ? (a) : (b))

void cola_iniciar(cola_t *c)
{
	c->prim = NULL;
	c->ult = NULL;
}

int cola_insertar(cola_t *c, const void *d, unsigned tam)
{
	nodo_t *nue = (nodo_t *)malloc(sizeof(nodo_t));
	if (!nue) {
		return -1;
	}
	nue->dato = malloc(tam);
	if (!nue->dato) {
		free(nue);
		return -1;
	}
	memcpy(nue->dato, d, tam);
	nue->tam_dato = tam;
	if (!c->prim) {
		c->prim = nue;
		c->ult = nue;
	} else {
		c->ult->sig = nue;
		c->ult = nue;
	}
	return 0;
}
int cola_sacar(cola_t *c, void *buff, unsigned tam)
{
	nodo_t *aux = c->prim;
	if (!c->prim) {
		return -1;
	}
	memcpy(buff, aux->dato, MIN(tam, aux->tam_dato));
	if (c->prim == c->ult) {
		/* Solo hay un elemento: eliminarlo y dejar cola vacía */
		free(aux->dato); /* Liberar datos del nodo */
		free(aux); /* Liberar el nodo */
		c->prim = NULL;
		c->ult = NULL;
	} else {
		/* Hay múltiples elementos: mover el siguiente al frente */
		nodo_t *sig = aux->sig; /* Guardar siguiente antes de liberar */

		free(aux->dato); /* Liberar datos del nodo */
		free(aux); /* Liberar el nodo */

		c->prim = sig; /* Nuevo primero es el antiguo segundo */
	}
	return 0;
}
int cola_ver_primero(cola_t *c, void *buff, unsigned tam)
{
	if (!c->prim) {
		return -1;
	}
	memcpy(buff, c->prim->dato, MIN(tam, c->prim->tam_dato));
	return 0;
}
// Existe solo por mera compatibilidad hacia una cola estatica
int cola_llena(cola_t *c, unsigned tam)
{
	(void)c;
	nodo_t *nue = (nodo_t *)malloc(sizeof(nodo_t));
	if (!nue) {
		return -1;
	}
	nue->dato = malloc(tam);
	if (!nue->dato) {
		free(nue);
		return -1;
	}
	free(nue->dato);
	free(nue);
	return 0;
}
int cola_vacia(cola_t *c)
{
	return c->prim == NULL ? 1 : 0;
}
int cola_vaciar(cola_t *c)
{
	nodo_t *aux = c->prim;
	while (aux) {
		nodo_t *sig = c->prim->sig;
		free(aux->dato);
		free(aux);
		aux = sig;
	}
	c->ult = NULL;
	c->prim = NULL;
	return 0;
}
