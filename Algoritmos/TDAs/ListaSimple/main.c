#include <ListaSimple.h>
#include <stdlib.h>
#include <string.h>
#include <Comun.h>
#define MIN(a, b) (a > b ? b : a)

void crearLista(t_Lista *l)
{
	*l = NULL;
}
int listaVacia(const t_Lista *l)
{
	return *l == NULL ? TRUE : FALSE;
}
int listaLlena(const t_Lista *l, const unsigned tam)
{
	t_Nodo *aux = (t_Nodo *)malloc(sizeof(t_Nodo));
	void *info = malloc(tam);
	free(aux);
	free(info);
	return aux == NULL || info == NULL ? TRUE : FALSE;
}
void vaciarLista(t_Lista *l)
{
	t_Nodo *n = (*l)->sig;
	while (n) {
		t_Nodo *aux = n;
		n = aux->sig;
		free(aux->dato);
		free(aux);
	}
	l = NULL;
}
int ponerAlComienzoDeLista(t_Lista *l, const void *d, const unsigned tam)
{
	if (listaLlena(l, tam)) {
		return ERR_MEM_LLENA;
	}
	t_Nodo *n = (t_Nodo *)malloc(sizeof(t_Nodo));
	n->dato = malloc(tam);
	if (!n || !n->dato) {
		free(n);
		return ERR_MEM_LLENA;
	}
	n->sig = *l;
	*l = n;
	memcpy(n->dato, d, tam);
	n->tam = tam;

	return OK;
}
int ponerEnLista(t_Lista *l, const void *d, const unsigned tam);
int sacarPrimeroLista(t_Lista *l, void *buff, const unsigned tam)
{
	if (listaVacia(l)) {
		return ERR_LISTA_VACIA;
	}
	t_Nodo *aux = *l;
	*l = aux->sig;
	memcpy(buff, aux->dato, MIN(tam, aux->tam));
	free(aux->dato);
	free(aux);
	return OK;
}
int ponerAlFinalDeLista(t_Lista *l, const void *d, const unsigned tam)
{
	if (listaLlena(l, tam)) {
		return ERR_LISTA_VACIA;
	}
	t_Nodo *aux = *l;
	t_Nodo *n = (t_Nodo *)malloc(sizeof(t_Nodo));
	n->dato = malloc(tam);
	memcpy(n->dato, d, tam);
	n->tam = tam;
	n->sig = NULL;
	while (*l) {
		l = &(*l)->sig;
	}
	*l = n;
	return OK;
}
int sacarUltimoDeLista(t_Lista *l, void *buff, const unsigned tam)
{
	if (listaVacia(l)) {
		return ERR_LISTA_VACIA;
	}
	t_Nodo *aux = *l;
	while ((*l)->sig) {
		l = &(*l)->sig;
	}
	memcpy(buff, (*l)->dato, MIN(tam, aux->tam));
	free((*l)->dato);
	free(*l);
	*l = NULL;
	return OK;
}
int verUltimoDeLista(t_Lista *l, void *buff, unsigned tam)
{
	if (listaVacia(l)) {
		return ERR_LISTA_VACIA;
	}
	t_Nodo *aux = *l;
	while (aux->sig != NULL) {
		aux = aux->sig;
	}
	memcpy(aux->dato, buff, MIN(tam, aux->tam));
	return OK;
}
