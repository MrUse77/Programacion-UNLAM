#include <PilaCircular.h>
#include <string.h>
#include <string.h>
#include <stdlib.h>
#include "Comun.h"

void crearPila(t_Pila *p)
{
	*p = NULL;
}

int pilaLlena(const t_Pila *p, unsigned tam)
{
	t_Nodo *aux = (t_Nodo *)malloc(sizeof(t_Nodo));
	void *info = malloc(tam);
	free(aux);
	free(info);
	return aux == NULL || info == NULL;
}

int apilar(t_Pila *p, const void *d, unsigned tam)
{
	t_Nodo *nue = (t_Nodo *)malloc(sizeof(t_Nodo));
	nue->dato = malloc(tam);
	if (nue == NULL || nue->dato == NULL) {
		free(nue);
		return ERR_PIL_LLENA;
	}
	memcpy(nue->dato, d, tam);
	nue->tamDato = tam;
	nue->sig = *p;
	if (*p == NULL) {
		nue->sig = nue;
		*p = nue;
	} else {
		nue->sig = (*p)->sig;
		(*p)->sig = nue;
	}
	return OK;
}

int pilaVacia(const t_Pila *p)
{
	return *p == NULL ? TRUE : FALSE;
}

void vaciarPila(t_Pila *p)
{
	t_Nodo *n = *p;
	free(n->dato);
	free(n);
	*p = NULL;
}

int desapilar(t_Pila *p, void *buffer, unsigned tam)
{
	if (*p == NULL) {
		return ERR_PIL_VACIA;
	}
	memcpy(buffer, (*p)->sig->dato, MIN((*p)->sig->tamDato, tam));
	t_Nodo *n = (*p)->sig;
	(*p)->sig = n->sig;
	free((*p)->sig->dato);
	free(*p);
	return OK;
}

int verTope(const t_Pila *p, void *buffer, unsigned tam)
{
	if (*p == NULL) {
		return ERR_PIL_VACIA;
	}
	t_Nodo *n = *p;
	memcpy(buffer, n->dato, MIN(n->tamDato, tam));
	return OK;
}
