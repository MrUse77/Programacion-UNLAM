#include "lista_doble.h"
#include <stdlib.h>
#include <string.h>
#define MIN(a, b) ((a) < (b) ? (a) : (b))

void lista_doble_iniciar(lista_doble_t *l)
{
	*l = NULL;
}
int lista_doble_insertar_ultimo(lista_doble_t *l, const void *d, unsigned tam)
{
	nodo_t *nue = (nodo_t *)malloc(sizeof(nodo_t)), *act = *l;
	if (!nue) {
		return -1;
	}
	nue->dato = malloc(tam);
	if (!nue->dato) {
		free(nue);
		return -1;
	}
	if (act) {
		while (act->sig) {
			act = act->sig;
		}
	}
	memcpy(nue->dato, d, tam);
	nue->tam = tam;
	nue->sig = NULL;
	nue->ant = act;
	if (act) {
		act->sig = nue;
	}
	*l = nue;
	return 0;
}

int lista_doble_sacar_ultimo(lista_doble_t *l, void *buff, unsigned tam)
{
	nodo_t *act = *l;
	if (!act) {
		return -1;
	}
	while (act->sig) {
		act = act->sig;
	}
	memcpy(buff, act, MIN(tam, act->tam));
	*l = act->ant;
	free(act->dato);
	free(act);
	return 0;
}
int lista_doble_vaciar(lista_doble_t *l)
{
	nodo_t *act = *l;
	if (!act) {
		return -1;
	}
	while (act->ant) {
		act = act->ant;
	}
	while (act) {
		nodo_t *aux = act->sig;
		free(act->dato);
		free(act);
		act = aux;
	}
	*l = NULL;
	return 0;
}
int lista_doble_ver_pos(lista_doble_t *l, void *buff, unsigned tam, int pos)
{
	nodo_t *act = *l;
	int i;
	if (!act) {
		return -1;
	}
	while (act->ant) {
		act = act->ant;
	}
	for (i = 0; i < pos; i++) {
		act = act->sig;
	}
	memcpy(buff, act, MIN(tam, act->tam));
	return 0;
}
int lista_doble_eliminar_dup(lista_doble_t *l, cmp_t cmp, acum_t acum)
{
	nodo_t *act = *l, *pri;
	if (!act) {
		return -1;
	}
	while (act->ant) {
		act = act->ant;
	}
	pri = act;
	while (act) {
		nodo_t *dup = act->sig;
		while (dup) {
			nodo_t *aux = dup;
			if (cmp(act->dato, aux->dato) == 0) {
				acum(act->dato, aux->dato);
				dup = aux->sig;
				if (aux->ant) {
					aux->ant->sig = aux->sig;
				}
				if (aux->sig) {
					aux->sig->ant = aux->ant;
				}

				free(aux->dato);
				free(aux);
			} else {
				dup = aux->sig;
			}
		}
		act = act->sig;
	}
	*l = pri;
	return 0;
}
int lista_doble_reduce(lista_doble_t *l, acum_t acum, void *buff)
{
	nodo_t *act = *l;
	if (!act) {
		return -1;
	}
	while (act->ant) {
		act = act->ant;
	}
	while (act->sig) {
		nodo_t *aux = act->sig;
		acum(act->dato, buff);
		free(act->dato);
		free(act);
		act = aux;
	}
	*l = NULL;
	return 0;
}
void lista_doble_mostrar(lista_doble_t *l, accion_t accion, void *param)
{
	nodo_t *act;

	if (!l || !*l || !accion) {
		return;
	}

	act = *l;

	while (act->ant) {
		act = act->ant;
	}

	while (act) {
		accion(act->dato, param);
		act = act->sig;
	}
}
