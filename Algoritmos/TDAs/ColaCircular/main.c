#include <ColaCircular.h>
#include <stdlib.h>
#include <string.h>

void crearCola(t_Cola *c)
{
	*c = NULL;
}

int ponerEnCola(t_Cola *c, const void *d, unsigned tamDato)
{
	t_Nodo *n = (t_Nodo *)malloc(sizeof(t_Nodo));
	n->dato = malloc(tamDato);
	if (!n || !n->dato) {
		free(n);
		return ERR_MEM_LLENA;
	}
	memcpy(n->dato, d, tamDato);
	n->tamDato = tamDato;
	if (*c == NULL) {
		n->sig = n;
		*c = n;
	} else {
		n->sig = (*c)->sig;
		(*c)->sig = n;
		*c = n;
	}
	return OK;
}

int colaLlena(t_Cola *c, unsigned tam)
{
	t_Nodo *n = (t_Nodo *)malloc(sizeof(t_Nodo));
	n->dato = malloc(tam);
	if (!n || !n->dato) {
		free(n);
		return TRUE;
	}
	free(n->dato);
	free(n);
	return FALSE;
}

int verPrimero(t_Cola *c, void *buff, unsigned tamDato);

int sacarDeCola(t_Cola *c, void *buff, unsigned tamDato)
{
	if (*c == NULL) {
		return ERR_COLA_VACIA;
	}
	memcpy(buff, (*c)->sig->dato, MIN(tamDato, (*c)->sig->tamDato));
	t_Nodo *aux = (*c)->sig;
	(*c)->sig = aux->sig;
	free((*c)->sig->dato);
	free((*c)->sig);
	if (*c == NULL) {
		*c = NULL;
	}
	return OK;
}

int colaVacia(t_Cola *c)
{
	return *c == NULL ? TRUE : FALSE;
}

void vaciarCola(t_Cola *c);
