#include <lista_simple.h>
#include <stdlib.h>
#include <string.h>

static list_d_node_t *node_create(const void *d, const unsigned tam)
{
	list_d_node_t *nodo = (list_d_node_t *)malloc(sizeof(list_d_node_t));
	if (!nodo) {
		return NULL;
	}
	void *dato = malloc(tam);
	if (!dato) {
		free(nodo);
		return NULL;
	}
	memcpy(dato, d, tam);
	nodo->tam = tam;
	nodo->dato = dato;
	nodo->sig = NULL;
	return nodo;
}

void radix_sort(list_t *l, cmp_t cmp)
{
}

static void divide(list_t *l, list_t *izq, list_t *der)

{
	list_d_node_t *slow, *fast;

	if (l == NULL || *l == NULL) {
		*izq = NULL;
		*der = NULL;
		return;
	}

	slow = *l;
	fast = (*l)->sig;

	while (fast != NULL && fast->sig != NULL) {
		slow = slow->sig;
		fast = fast->sig->sig;
	}

	*der = slow->sig;
	slow->sig = NULL;
	*izq = *l;
	*l = NULL;
}

static list_t merge(list_t izq, list_t der, cmp_t cmp)
{
	if (izq == NULL) {
		return der;
	}
	if (der == NULL) {
		return izq;
	}
	if (cmp(izq->dato, der->dato) <= 0) {
		izq->sig = merge(izq->sig, der, cmp);
		return izq;
	} else {
		der->sig = merge(izq, der->sig, cmp);
		return der;
	}
}

static void merge_sort(list_t *l, cmp_t cmp)
{
	list_t izq, der;
	if (*l == NULL || (*l)->sig == NULL) {
		return;
	}
	divide(l, &izq, &der);
	merge_sort(&izq, cmp);
	merge_sort(&der, cmp);
	*l = merge(izq, der, cmp);
}

static void quick_sort(list_t *l, cmp_t cmp)
{
}

static void insertion_sort(list_t *l, cmp_t cmp)
{
}

static void bubble_sort(list_t *l, cmp_t cmp)
{
}

static void selection_sort(list_t *l, cmp_t cmp)
{
}

void list_init(list_t *l)
{
	*l = NULL;
}

list_status_t list_push_last(list_t *l, void *d, unsigned tam)
{
	list_d_node_t *aux = *l, *node = node_create(d, tam);
	if (node == NULL) {
		return LIST_ERR_MEM_FULL;
	}
	if (*l == NULL) {
		*l = node;
	} else {
		while (aux->sig) {
			aux = aux->sig;
		}
		aux->sig = node;
	}
	return LIST_SUCCESS;
}

list_status_t list_pop_last(list_t *l, void *b, unsigned tam)
{
	list_d_node_t *aux = *l, *ant = NULL;
	if (*l == NULL) {
		return LIST_ERR_EMPTY;
	}
	while (aux->sig) {
		ant = aux;
		aux = aux->sig;
	}
	if (ant == NULL) {
		*l = NULL;
	} else {
		ant->sig = NULL;
	}
	memcpy(b, aux->dato, MIN(tam, aux->tam));
	free(aux->dato);
	free(aux);
	return LIST_SUCCESS;
}

list_status_t list_push_first(list_t *l, void *d, unsigned tam)
{
	list_d_node_t *aux = *l, *node = node_create(d, tam);
	if (node == NULL) {
		return LIST_ERR_MEM_FULL;
	}
	if (*l == NULL) {
		*l = node;
		node->sig = NULL;
	} else {
		node->sig = aux;
		*l = node;
	}
	return LIST_SUCCESS;
}

list_status_t list_pop_first(list_t *l, void *b, unsigned tam)
{
	list_d_node_t *aux = *l;
	if (*l == NULL) {
		return LIST_ERR_EMPTY;
	}
	memcpy(b, aux->dato, MIN(tam, aux->tam));
	*l = aux->sig;
	free(aux->dato);
	free(aux);
	return LIST_SUCCESS;
}

list_status_t list_push_in_pos(list_t *l, void *d, unsigned tam, int pos)
{
	list_d_node_t **pp = l, *node = NULL;
	if (pos < 0) {
		return LIST_ERR_INVAL;
	}
	/* pos es el indice final (0-based) */
	while (*pp && pos > 0) {
		pp = &(*pp)->sig;
		pos--;
	}
	if (pos != 0) {
		return LIST_ERR_INVAL;
	}
	node = node_create(d, tam);
	if (node == NULL) {
		return LIST_ERR_MEM_FULL;
	}
	node->sig = *pp;
	*pp = node;
	return LIST_SUCCESS;
}

list_status_t list_pop_in_pos(list_t *l, void *b, unsigned tam, int pos)
{
	list_d_node_t *aux = *l, *ant = NULL;
	if (*l == NULL) {
		return LIST_ERR_EMPTY;
	}
	while (aux->sig && pos - 1 != 0) {
		aux = aux->sig;
		pos--;
	}
	if (pos - 1 != 0 || aux->sig == NULL) {
		return LIST_ERR_NOT_FOUND;
	}
	ant = aux;
	memcpy(b, aux->sig->dato, MIN(tam, aux->tam));
	ant->sig = aux->sig->sig;
	free(aux->sig->dato);
	free(aux->sig);
	return LIST_SUCCESS;
}

list_status_t list_push_in_order(list_t *l, void *d, unsigned tam, cmp_t cmp)
{
	list_d_node_t **pp = l, *node = NULL;
	if (l == NULL) {
		return LIST_ERR_INVAL;
	}
	/* orden ascendente: cmp(a, b) < 0 significa que a va antes que b */
	while (*pp && cmp((*pp)->dato, d) <= 0) {
		pp = &(*pp)->sig;
	}
	node = node_create(d, tam);
	if (node == NULL) {
		return LIST_ERR_MEM_FULL;
	}
	node->sig = *pp;
	*pp = node;
	return LIST_SUCCESS;
}

list_status_t list_pop_in_order(list_t *l, void *b, unsigned tam, cmp_t cmp)
{
	list_d_node_t **pp = l, *aux = NULL;
	if (*l == NULL) {
		return LIST_ERR_EMPTY;
	}
	/* b entra como clave de busqueda y sale con el dato extraido: se copia
	   recien despues de encontrar el nodo, nunca dentro del recorrido. */
	while (*pp && cmp((*pp)->dato, b) != 0) {
		pp = &(*pp)->sig;
	}
	if (*pp == NULL) {
		return LIST_ERR_NOT_FOUND;
	}
	aux = *pp;
	*pp = aux->sig;
	memcpy(b, aux->dato, MIN(tam, aux->tam));
	free(aux->dato);
	free(aux);
	return LIST_SUCCESS;
}

list_status_t list_see_first(list_t *l, void *b, unsigned tam)
{
	list_d_node_t *aux = *l;
	if (*l == NULL) {
		return LIST_ERR_EMPTY;
	}
	memcpy(b, aux->dato, MIN(tam, aux->tam));
	return LIST_SUCCESS;
}

list_status_t list_see_last(list_t *l, void *b, unsigned tam)
{
	list_d_node_t *aux = *l;
	if (*l == NULL) {
		return LIST_ERR_EMPTY;
	}
	while (aux->sig) {
		aux = aux->sig;
	}
	memcpy(b, aux->dato, MIN(tam, aux->tam));
	return LIST_SUCCESS;
}

list_status_t list_reduce(list_t *l, void *param, accion_t accion)
{
	list_d_node_t *aux = *l;
	while (aux) {
		accion(aux->dato, param);
		aux = aux->sig;
	}
	return LIST_SUCCESS;
}

list_status_t list_map(const list_t *l, list_t *new_l, const void *param,
		       accion_t accion)
{
	list_d_node_t *aux = *l, *aux_new = NULL;
	while (aux) {
		list_d_node_t *new_node = node_create(aux->dato, aux->tam);
		if (new_node == NULL) {
			return LIST_ERR_MEM_FULL;
		}
		accion(new_node->dato, (void *)param);
		if (*new_l == NULL) {
			*new_l = new_node;
			aux_new = *new_l;
		} else {
			aux_new->sig = new_node;
			aux_new = aux_new->sig;
		}
		aux = aux->sig;
	}
	return LIST_SUCCESS;
}

list_status_t list_filter(const list_t *l, list_t *new_l, const void *param,
			  cmp_t cmp)
{
	list_d_node_t *aux = *l, *aux_new = NULL;
	while (aux) {
		if (cmp(aux->dato, param) == 0) {
			list_d_node_t *new_node =
				node_create(aux->dato, aux->tam);
			if (new_node == NULL) {
				return LIST_ERR_MEM_FULL;
			}
			if (*new_l == NULL) {
				*new_l = new_node;
				aux_new = *new_l;
			} else {
				aux_new->sig = new_node;
				aux_new = aux_new->sig;
			}
		}
		aux = aux->sig;
	}
	return LIST_SUCCESS;
}

void list_show(list_t *l, prnt_t prnt)
{
	list_d_node_t *aux = *l;
	while (aux) {
		prnt(aux->dato);
		aux = aux->sig;
	}
}

void list_foreach(list_t *l, void *b, accion_t accion)
{
	list_d_node_t *aux = *l;
	while (aux) {
		accion(aux->dato, b);
		aux = aux->sig;
	}
}

bool_t list_is_full(list_t *l, unsigned tam)
{
	list_d_node_t *nodo = (list_d_node_t *)malloc(sizeof(list_d_node_t));
	if (!nodo) {
		return TRUE;
	}
	void *dato = malloc(tam);
	if (!dato) {
		free(nodo);
		return TRUE;
	}
	free(nodo);
	free(dato);
	return FALSE;
}

bool_t list_is_empty(list_t *l)
{
	return *l == NULL ? TRUE : FALSE;
}

void list_clear(list_t *l)
{
	list_d_node_t *aux;
	while (*l) {
		aux = (*l)->sig;
		free((*l)->dato);
		free(*l);
		*l = aux;
	}
	*l = NULL;
}

list_status_t list_order(list_t *l, const order_t ordenamiento, cmp_t cmp)
{
	switch (ordenamiento) {
	case BURBUJEO:
		bubble_sort(l, cmp);
		break;
	case SELECCION:
		selection_sort(l, cmp);
		break;
	case INSERCION:
		insertion_sort(l, cmp);
		break;
	case RADIX:
		radix_sort(l, cmp);
		break;
	case MERGE:
		merge_sort(l, cmp);
		break;
	case QUICK:
		quick_sort(l, cmp);
		break;
	default:
		return LIST_ERR_INVAL;
	}
	return LIST_SUCCESS;
}

list_status_t list_find(list_t *l, void *param, unsigned tam, cmp_t cmp,
			bool_t elim)
{
	list_d_node_t **pp = l, *aux = *pp;
	while (aux) {
		if (cmp(aux->dato, param) == 0) {
			memcpy(param, (aux)->dato, MIN((aux)->tam, tam));
			if (elim == TRUE) {
				list_d_node_t *elimin = aux;
				*pp = aux->sig;
				free(elimin->dato);
				free(elimin);
			}
			return LIST_FOUND;
		}
		pp = &aux->sig;
		aux = *pp;
	}
	return LIST_ERR_NOT_FOUND;
}

int list_count_by_key(list_t *l, void *param, cmp_t cmp, bool_t elim)
{
	list_d_node_t **pp = l, *aux = *pp;
	int count = 0;
	while (aux) {
		if (cmp(aux->dato, param) == 0) {
			count++;
			if (elim == TRUE) {
				list_d_node_t *elimin = aux;
				*pp = aux->sig;
				free(elimin->dato);
				free(elimin);
			}
		}
		pp = &aux->sig;
		aux = *pp;
	}
	return count;
}

list_status_t list_find_nth_match(list_t *l, unsigned n, void *param,
				  unsigned tam, cmp_t cmp)
{
	list_d_node_t **pp = l, *aux = *pp;
	while (aux) {
		if (cmp(aux->dato, param) == 0) {
			n--;
			if (n == 0) {
				memcpy(param, (aux)->dato,
				       MIN((aux)->tam, tam));
				return LIST_FOUND;
			}
		}
		pp = &aux->sig;
		aux = *pp;
	}
	return LIST_ERR_NOT_FOUND;
}
list_status_t list_top(list_t *l, list_t *l_top, int top)
{
	list_d_node_t *aux = *l, **aux_top = l_top;
	for (int i = 0; i < top; i++) {
		list_d_node_t *new_node = node_create(aux->dato, aux->tam);
		*aux_top = new_node;
		aux_top = &(*aux_top)->sig;
		aux = aux->sig;
	}
	return LIST_SUCCESS;
}
