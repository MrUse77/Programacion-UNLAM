#include <Cola.h>
#include <Comun.h>
#include <stdlib.h>
#include <string.h>
#define MIN(a, b) (a > b ? b : a)

void crearCola(t_Cola *c)
{
	c->prim = NULL;
	c->ult = NULL;
}

int ponerEnCola(t_Cola *c, const void *d, unsigned tamDato)
{
	t_Nodo **ult = &c->ult;
	if (colaLlena(c, tamDato) == TRUE) {
		return ERR_COLA_LLENA;
	}
	t_Nodo *n = (t_Nodo *)malloc(sizeof(t_Nodo));
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
int verPrimero(t_Cola *c, void *buff, unsigned tamDato)
{
	if (colaVacia(c)) {
		return ERR_COLA_VACIA;
	}
	memcpy(buff, c->prim->dato, MIN(tamDato, c->prim->tamDato));
	return OK;
}
int sacarDeCola(t_Cola *c, void *buff, unsigned tamDato)
{
	if (colaVacia(c)) {
		return ERR_COLA_VACIA;
	}
	t_Nodo *prim = c->prim;
	c->prim = prim->sig;
	memcpy(buff, prim->dato, MIN(tamDato, prim->tamDato));
	free(prim->dato);
	free(prim);
	if (c->prim == NULL) {
		c->ult = NULL;
	}
	return OK;
}
int colaVacia(t_Cola *c)
{
	return c->prim == NULL ? TRUE : FALSE;
}
void vaciarCola(t_Cola *c)
{
	t_Nodo *prim = c->prim;
	while (prim) {
		t_Nodo *aux = prim;
		prim = aux->sig;
		free(aux->dato);
		free(aux);
	}
	c->ult = NULL;
}
