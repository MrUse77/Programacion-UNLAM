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
	while (*l) {
		t_Nodo *aux = *l;
		*l = aux->sig;
		free(aux->dato);
		free(aux);
	}
}

int insertarAlComienzoDeLista(t_Lista *l, const void *d, const unsigned tam)
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
int insertarAlFinalDeLista(t_Lista *l, const void *d, const unsigned tam)
{
	if (listaLlena(l, tam)) {
		return ERR_LISTA_VACIA;
	}
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
int insertarOrdenadoEnLista(t_Lista *l, const void *d, const unsigned tam,
			    t_Cmp cmp, const int conDup, t_Accion accion)
{
	t_Nodo *n = (t_Nodo *)malloc(sizeof(t_Nodo));
	n->dato = malloc(tam);
	if (!n || !n->dato) {
		free(n);
		return ERR_MEM_LLENA;
	}
	n->sig = NULL;
	n->tam = tam;
	memcpy(n->dato, d, tam);
	while (*l != NULL && cmp(n->dato, (*l)->dato) > 0) {
		l = &(*l)->sig;
	}
	if (*l && cmp(n->dato, (*l)->dato) == 0) {
		if (accion && conDup) {
			accion(n->dato, (*l)->dato);
		}
		return OK;
	} else {
		n->sig = *l;
		*l = n;
	}

	return OK;
}

int elimminarPorClave(t_Lista *l, void *buff, const unsigned tam, t_Cmp cmp)
{
	if (listaVacia(l)) {
		return ERR_LISTA_VACIA;
	}
	while (*l && cmp(buff, (*l)->dato) != 0) {
		l = &(*l)->sig;
	}
	if (*l == NULL) {
		return ERR_LISTA_NO_ENCONTRADO;
	}
	memcpy(buff, (*l)->dato, MIN(tam, (*l)->tam));
	t_Nodo *elim = *l;
	*l = elim->sig;
	free(elim->dato);
	free(elim);
	return OK;
}
