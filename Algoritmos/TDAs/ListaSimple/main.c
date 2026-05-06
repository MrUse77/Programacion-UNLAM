#include <ListaSimple.h>
#include <stdlib.h>
#include <string.h>
#include <Comun.h>
#define MIN(a, b) (a > b ? b : a)

// FUNCIONES QUE SE USAN SOLAMENTE ACA DENTRO
static int dividirLista(t_Lista *orig, t_Lista *izq, t_Lista *der)
{
	t_Nodo *slow;
	t_Nodo *fast;
	t_Nodo *prev;
	if (orig == NULL || *orig == NULL) {
		return ERR_LISTA_VACIA;
	}
	if ((*orig)->sig == NULL) {
		*izq = *orig;
		*der = NULL;
		*orig = NULL;
		return OK;
	}
	prev = NULL;
	slow = *orig;
	fast = *orig;
	while (fast != NULL && fast->sig != NULL) {
		prev = slow;
		slow = slow->sig;
		fast = fast->sig->sig;
	}
	*izq = *orig;
	*der = slow;
	prev->sig = NULL;
	*orig = NULL;
	return OK;
}

static t_Lista mezclarListas(t_Lista izq, t_Lista der, t_Cmp cmp)
{
	if (izq == NULL)
		return der;
	if (der == NULL)
		return izq;
	if (cmp(izq->dato, der->dato) <= 0) {
		izq->sig = mezclarListas(izq->sig, der, cmp);
		return izq;
	} else {
		der->sig = mezclarListas(izq, der->sig, cmp);
		return der;
	}
}

static int mergeSort(t_Lista *l, t_Cmp cmp)
{
	t_Lista izq, der;
	if (*l == NULL || (*l)->sig == NULL)
		return ERR_LISTA_VACIA;
	dividirLista(l, &izq, &der);
	mergeSort(&izq, cmp);
	mergeSort(&der, cmp);
	*l = mezclarListas(izq, der, cmp);
	return OK;
}

static t_Nodo *ultimo(t_Nodo *l)
{
	while (l && l->sig) {
		l = l->sig;
	}
	return l;
}

static int quickSort(t_Lista *l, t_Cmp cmp)
{
	t_Lista menor = NULL, mayor = NULL, igual = NULL;
	t_Nodo *pivote = *l, *act = pivote, *sig = NULL;
	t_Nodo *hMen = NULL, *hIg = NULL, *hMay = NULL;
	if (pivote == NULL) {
		return ERR_LISTA_VACIA;
	}
	while (act != NULL) {
		sig = act->sig;
		act->sig = NULL;
		if (cmp(act->dato, pivote->dato) < 0) {
			if (menor == NULL) {
				menor = act;
				hMen = act;
			} else {
				hMen->sig = act;
				hMen = hMen->sig;
			}
		} else if (cmp(act->dato, pivote->dato) > 0) {
			if (mayor == NULL) {
				mayor = act;
				hMay = act;
			} else {
				hMay->sig = act;
				hMay = hMay->sig;
			}
		} else {
			if (igual == NULL) {
				igual = act;
				hIg = act;
			} else {
				hIg->sig = act;
				hIg = hIg->sig;
			}
		}
		act = sig;
	}
	quickSort(&menor, cmp);
	quickSort(&mayor, cmp);
	t_Lista res = NULL;
	if (menor != NULL) {
		res = menor;
		ultimo(menor)->sig = igual;
	} else {
		res = igual;
	}
	if (igual != NULL) {
		ultimo(igual)->sig = mayor;
	} else if (res == NULL) {
		res = mayor;
	}
	*l = res;
	return OK;
}

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

int insertarAlPrincipioDeLista(t_Lista *l, const void *d, const unsigned tam)
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

int sacarPrimeroLista(t_Lista *l, void *buff, const unsigned tam)
{
	if (l == NULL) {
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
	t_Nodo *n = (t_Nodo *)malloc(sizeof(t_Nodo));
	n->dato = malloc(tam);
	if (!n || !n->dato) {
		free(n);
		return ERR_MEM_LLENA;
	}
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
	memcpy(buff, aux->dato, MIN(tam, aux->tam));
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
			accion((*l)->dato, n->dato);
			free(n->dato);
			free(n);
		}
		return OK;
	} else {
		n->sig = *l;
		*l = n;
	}

	return OK;
}

int eliminarPorClave(t_Lista *l, void *buff, const unsigned tam, t_Cmp cmp)
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

int verElementoEnPosicion(t_Lista *l, void *buff, const unsigned tam,
			  const int pos)
{
	if (l == NULL) {
		return ERR_LISTA_VACIA;
	}
	int i;
	for (i = 0; i < pos; i++) {
		l = &(*l)->sig;
	}
	memcpy(buff, (*l)->dato, MIN(tam, (*l)->tam));
	return OK;
}

int longitudLista(t_Lista *l)
{
	int cont = 0;
	if (l == NULL) {
		return cont;
	}

	while (*l) {
		l = &(*l)->sig;
		cont++;
	}
	return cont;
}

int buscarEnLista(t_Lista *l, void *buff, const unsigned tam, t_Cmp cmp)
{
	if (l == NULL) {
		return ERR_LISTA_VACIA;
	}
	while (*l && cmp(buff, (*l)->dato) != 0) {
		l = &(*l)->sig;
	}
	if (*l == NULL) {
		return ERR_LISTA_NO_ENCONTRADO;
	}
	memcpy(buff, (*l)->dato, MIN(tam, (*l)->tam));
	return OK;
}

int copiarLista(t_Lista *l, t_Lista *lCopia)
{
	if (l == NULL) {
		return ERR_LISTA_VACIA;
	}
	while (*l) {
		t_Nodo *n = malloc(sizeof(t_Nodo));
		n->dato = malloc((*l)->tam);
		if (!n || !n->dato) {
			free(n);
			return ERR_MEM_LLENA;
		}
		n->tam = (*l)->tam;
		memcpy(n->dato, (*l)->dato, sizeof((*l)->tam));
		n->sig = NULL;
		if (*lCopia) {
			lCopia = &(*lCopia)->sig;
		}
		*lCopia = n;
		l = &(*l)->sig;
	}
	return OK;
}

int insertarEnPosicion(t_Lista *l, const void *d, const unsigned tam,
		       const int pos)
{
	t_Nodo *n = malloc(sizeof(t_Nodo));
	n->dato = malloc(tam);
	if (!n || !n->dato) {
		free(n);
		return ERR_MEM_LLENA;
	}
	int i;
	for (i = 0; i < pos; i++) {
		l = &(*l)->sig;
	}
	n->tam = tam;
	n->sig = *l;
	memcpy(n->dato, d, tam);
	*l = n;
	return OK;
}

int insertarDespuesDeClave(t_Lista *l, const void *d, const unsigned tam,
			   const void *clave, const t_Cmp cmp)
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
	while (*l != NULL && cmp(clave, (*l)->dato) != 0) {
		l = &(*l)->sig;
	}
	l = &(*l)->sig;
	n->sig = *l;
	*l = n;
	return OK;
}

int insertarAntesDeClave(t_Lista *l, const void *d, const unsigned tam,
			 const void *clave, const t_Cmp cmp)
{
	t_Nodo *n = (t_Nodo *)malloc(sizeof(t_Nodo));
	t_Nodo **aux = l;
	n->dato = malloc(tam);
	if (!n || !n->dato) {
		free(n);
		return ERR_MEM_LLENA;
	}
	n->sig = NULL;
	n->tam = tam;
	memcpy(n->dato, d, tam);
	while (*aux != NULL && cmp(clave, (*aux)->dato) != 0) {
		aux = &(*aux)->sig;
	}
	if (aux == NULL) {
		return ERR_LISTA_NO_ENCONTRADO;
	}
	n->sig = *aux;
	*aux = n;
	return OK;
}

int verPrimeroDeLista(t_Lista *l, void *buff, const unsigned tam)
{
	if (l == NULL) {
		return ERR_LISTA_VACIA;
	}
	memcpy(buff, (*l)->dato, MIN(tam, (*l)->tam));
	return OK;
}

int eliminarPorPosicion(t_Lista *l, void *buff, const unsigned int tam,
			const int pos)
{
	int i;
	if (l == NULL) {
		return ERR_LISTA_VACIA;
	}
	t_Nodo **aux = l, *elim = NULL;
	for (i = 0; i < pos; i++) {
		aux = &(*aux)->sig;
	}
	if (*aux == NULL) {
		return ERR_LISTA_NO_ENCONTRADO;
	}
	elim = *aux;
	memcpy(buff, elim->dato, MIN(tam, elim->tam));
	*aux = elim->sig;
	free(elim->dato);
	free(elim);
	return OK;
}

int eliminarAntesDeClave(t_Lista *l, void *buff, const unsigned int tam,
			 const void *clave, const t_Cmp cmp)
{
	if (l == NULL) {
		return ERR_LISTA_VACIA;
	}
	t_Nodo **aux = l;
	while (*aux != NULL && cmp(clave, (*aux)->dato) != 0) {
		aux = &(*aux)->sig;
	}
	if (aux == NULL) {
		return ERR_LISTA_NO_ENCONTRADO;
	}
	memcpy(buff, (*aux)->dato, MIN(tam, (*aux)->tam));
	free((*aux)->dato);
	free(*aux);

	return OK;
}

int eliminarDespuesDeClave(t_Lista *l, void *buff, const unsigned int tam,
			   const void *clave, const t_Cmp cmp)
{
	if (l == NULL) {
		return ERR_LISTA_VACIA;
	}
	t_Nodo **aux = l;
	while (*aux != NULL && cmp(clave, (*aux)->dato) != 0) {
		aux = &(*aux)->sig;
	}
	aux = &(*aux)->sig;
	if (aux == NULL) {
		return ERR_LISTA_NO_ENCONTRADO;
	}
	memcpy(buff, (*aux)->dato, MIN(tam, (*aux)->tam));
	free((*aux)->dato);
	free(*aux);

	return OK;
}

void mostrarLista(const t_Lista *l, const t_Prnt prnt)
{
	while (*l) {
		prnt((*l)->dato);
		l = &(*l)->sig;
	}
}

int invertirLista(t_Lista *l)
{
	t_Nodo *prev, *act, *sig;
	if (l == NULL || *l == NULL) {
		return ERR_LISTA_VACIA;
	}
	prev = NULL;
	act = *l;
	while (act != NULL) {
		sig = act->sig;
		act->sig = prev;
		prev = act;
		act = sig;
	}
	*l = prev;
	return OK;
}

int concatenarListas(t_Lista *l1, t_Lista *l2)
{
	if (l1 == NULL || l2 == NULL) {
		return ERR_LISTA_VACIA;
	}
	if (*l2 == NULL) {
		return OK;
	}
	if (*l1 == NULL) {
		*l1 = *l2;
		*l2 = NULL;
		return OK;
	}
	t_Nodo *aux = *l1;
	while (aux->sig != NULL) {
		aux = aux->sig;
	}
	aux->sig = *l2;
	*l2 = NULL;
	return OK;
}

int contarApariciones(t_Lista *l, const void *d, int *res, const t_Cmp cmp)
{
	if (l == NULL) {
		return ERR_LISTA_VACIA;
	}
	while (*l) {
		if (cmp(d, (*l)->dato) == 0) {
			(*res)++;
		}
		l = &(*l)->sig;
	}
	return OK;
}

int listaContiene(t_Lista *l, const void *d, const t_Cmp cmp)
{
	if (l == NULL) {
		return ERR_LISTA_VACIA;
	}
	while (*l) {
		if (cmp(d, (*l)->dato) == 0) {
			return TRUE;
		}
		l = &(*l)->sig;
	}
	return ERR_LISTA_NO_ENCONTRADO;
}

int ordenarLista(t_Lista *l, const int ordenamiento, const t_Cmp cmp)
{
	int code;
	switch (ordenamiento) {
	case MERGE:
		code = mergeSort(l, cmp);
		break;
	case RADIX:
		code = mergeSort(l, cmp);
		break;
	case QUICK:
		code = quickSort(l, cmp);
		break;
	}
	return code;
}
