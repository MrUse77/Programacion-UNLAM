#include <ListaSimple.h>
#include <stdlib.h>
#include <string.h>
#include <Comun.h>
#define MIN(a, b) (a > b ? b : a)

// FUNCIONES QUE SE USAN SOLAMENTE ACA DENTRO
static int list_divide(list_t *orig, list_t *left, list_t *right)
{
	list_node_t *slow;
	list_node_t *fast;
	list_node_t *prev;
	if (orig == NULL || *orig == NULL) {
		return ERR_LISTA_VACIA;
	}
	if ((*orig)->sig == NULL) {
		*left = *orig;
		*right = NULL;
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
	*left = *orig;
	*right = slow;
	prev->sig = NULL;
	*orig = NULL;
	return OK;
}

static list_t list_merge_lists(list_t left, list_t right, t_Cmp cmp)
{
	if (left == NULL)
		return right;
	if (right == NULL)
		return left;
	if (cmp(left->dato, right->dato) <= 0) {
		left->sig = list_merge_lists(left->sig, right, cmp);
		return left;
	} else {
		right->sig = list_merge_lists(left, right->sig, cmp);
		return right;
	}
}

static int list_merge_sort(list_t *l, t_Cmp cmp)
{
	list_t left, right;
	if (*l == NULL || (*l)->sig == NULL)
		return ERR_LISTA_VACIA;
	list_divide(l, &left, &right);
	list_merge_sort(&left, cmp);
	list_merge_sort(&right, cmp);
	*l = list_merge_lists(left, right, cmp);
	return OK;
}

static list_node_t *list_last(list_node_t *l)
{
	while (l && l->sig) {
		l = l->sig;
	}
	return l;
}

static int list_quick_sort(list_t *l, t_Cmp cmp)
{
	list_t smaller = NULL, larger = NULL, equal = NULL;
	list_node_t *pivot = *l, *current = pivot, *next = NULL;
	list_node_t *head_smaller = NULL, *head_equal = NULL,
		    *head_larger = NULL;
	if (pivot == NULL) {
		return ERR_LISTA_VACIA;
	}
	while (current != NULL) {
		next = current->sig;
		current->sig = NULL;
		if (cmp(current->dato, pivot->dato) < 0) {
			if (smaller == NULL) {
				head_smaller = current;
				head_smaller = current;
			} else {
				head_smaller->sig = current;
				head_smaller = head_smaller->sig;
			}
		} else if (cmp(current->dato, pivot->dato) > 0) {
			if (larger == NULL) {
				head_larger = current;
				head_larger = current;
			} else {
				head_larger->sig = current;
				head_larger = head_larger->sig;
			}
		} else {
			if (equal == NULL) {
				head_equal = current;
				head_equal = current;
			} else {
				head_equal->sig = current;
				head_equal = head_equal->sig;
			}
		}
		current = next;
	}
	list_quick_sort(&smaller, cmp);
	list_quick_sort(&larger, cmp);
	list_t res = NULL;
	if (smaller != NULL) {
		res = smaller;
		list_last(smaller)->sig = equal;
	} else {
		res = equal;
	}
	if (equal != NULL) {
		list_last(equal)->sig = larger;
	} else if (res == NULL) {
		res = head_larger;
	}
	*l = res;
	return OK;
}

void list_create(list_t *l)
{
	*l = NULL;
}

int list_is_empty(const list_t *l)
{
	return *l == NULL ? TRUE : FALSE;
}

int list_is_full(const list_t *l, const unsigned tam)
{
	list_node_t *aux = (list_node_t *)malloc(sizeof(list_node_t));
	void *info = malloc(tam);
	free(aux);
	free(info);
	return aux == NULL || info == NULL ? TRUE : FALSE;
}

void list_clear(list_t *l)
{
	while (*l) {
		list_node_t *aux = *l;
		*l = aux->sig;
		free(aux->dato);
		free(aux);
	}
}

int list_push_first(list_t *l, const void *d, const unsigned tam)
{
	if (list_is_full(l, tam)) {
		return ERR_MEM_LLENA;
	}
	list_node_t *n = (list_node_t *)malloc(sizeof(list_node_t));
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

int list_pull_first(list_t *l, void *buff, const unsigned tam)
{
	if (*l == NULL) {
		return ERR_LISTA_VACIA;
	}
	list_node_t *aux = *l;
	*l = aux->sig;
	memcpy(buff, aux->dato, MIN(tam, aux->tam));
	free(aux->dato);
	free(aux);
	return OK;
}

int list_push_last(list_t *l, const void *d, const unsigned tam)
{
	list_node_t *n = (list_node_t *)malloc(sizeof(list_node_t));
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

int list_pull_last(list_t *l, void *buff, const unsigned tam)
{
	if (*l == NULL) {
		return ERR_LISTA_VACIA;
	}
	list_node_t *aux = *l;
	while (aux->sig) {
		aux = aux->sig;
	}
	memcpy(buff, aux->dato, MIN(tam, aux->tam));
	free(aux->dato);
	free(aux);
	*l = NULL;
	return OK;
}

int list_see_last(list_t *l, void *buff, const unsigned tam)
{
	if (*l == NULL) {
		return ERR_LISTA_VACIA;
	}
	list_node_t *aux = *l;
	while ((*l)->sig) {
		l = &(*l)->sig;
	}
	memcpy(buff, (*l)->dato, MIN(tam, aux->tam));
	free((*l)->dato);
	free(*l);
	*l = NULL;
	return OK;
}

int list_push_orrighter(list_t *l, const void *d, const unsigned tam, t_Cmp cmp,
			const int conDup, t_Accion accion)
{
	list_node_t *n = (list_node_t *)malloc(sizeof(list_node_t));
	n->dato = malloc(tam);
	if (!n || !n->dato) {
		free(n);
		return ERR_MEM_LLENA;
	}
	n->sig = NULL;
	n->tam = tam;
	memcpy(n->dato, d, tam);
	while (*l && cmp(n->dato, (*l)->dato) > 0) {
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

int list_delete_by_key(list_t *l, void *buff, const unsigned tam, t_Cmp cmp)
{
	if (*l == NULL) {
		return ERR_LISTA_VACIA;
	}
	while (*l && cmp(buff, (*l)->dato) != 0) {
		l = &(*l)->sig;
	}
	if (*l == NULL) {
		return ERR_LISTA_NO_ENCONTRADO;
	}
	memcpy(buff, (*l)->dato, MIN(tam, (*l)->tam));
	list_node_t *elim = *l;
	*l = elim->sig;
	free(elim->dato);
	free(elim);
	return OK;
}

int list_get_at_position(list_t *l, void *buff, const unsigned tam,
			 const int position)
{
	if (*l == NULL) {
		return ERR_LISTA_VACIA;
	}
	int i;
	for (i = 0; i < position && *l != NULL; i++) {
		l = &(*l)->sig;
	}
	memcpy(buff, (*l)->dato, MIN(tam, (*l)->tam));
	return OK;
}

int list_length(list_t *l)
{
	int count = 0;
	if (*l == NULL) {
		return count;
	}

	while (*l != NULL) {
		l = &(*l)->sig;
		count++;
	}
	return count;
}

int list_search(list_t *l, void *buff, const unsigned tam, t_Cmp cmp)
{
	if (*l == NULL) {
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

int list_copy(list_t *source, list_t *dest)
{
	if (*source == NULL) {
		return ERR_LISTA_VACIA;
	}
	while (*source != NULL) {
		list_node_t *n = malloc(sizeof(list_node_t));
		n->dato = malloc((*source)->tam);
		if (!n || !n->dato) {
			free(n);
			return ERR_MEM_LLENA;
		}
		n->tam = (*source)->tam;
		memcpy(n->dato, (*source)->dato, sizeof((*source)->tam));
		n->sig = NULL;
		if (*dest != NULL) {
			(*dest)->sig = n;
		} else {
			*dest = n;
		}
		source = &(*source)->sig;
	}
	return OK;
}

int list_push_at_position(list_t *l, const void *d, const unsigned tam,
			  const int pos)
{
	list_node_t *n = malloc(sizeof(list_node_t));
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

int list_push_after_key(list_t *l, const void *d, const unsigned tam,
			const void *clave, const t_Cmp cmp)
{
	list_node_t *n = (list_node_t *)malloc(sizeof(list_node_t));
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

int list_insert_before_key(list_t *l, const void *d, const unsigned tam,
			   const void *clave, const t_Cmp cmp)
{
	list_node_t *n = (list_node_t *)malloc(sizeof(list_node_t));
	list_node_t **aux = l;
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

int list_peek_first(list_t *l, void *buff, const unsigned tam)
{
	if (*l == NULL) {
		return ERR_LISTA_VACIA;
	}
	memcpy(buff, (*l)->dato, MIN(tam, (*l)->tam));
	return OK;
}

int list_delete_at_position(list_t *l, void *buff, const unsigned int tam,
			    const int pos)
{
	int i;
	if (l == NULL) {
		return ERR_LISTA_VACIA;
	}
	list_node_t **aux = l, *elim = NULL;
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

int list_delete_before_key(list_t *l, void *buff, const unsigned int tam,
			   const void *clave, const t_Cmp cmp)
{
	if (l == NULL) {
		return ERR_LISTA_VACIA;
	}
	list_node_t **aux = l;
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

int list_delete_after_key(list_t *l, void *buff, const unsigned int tam,
			  const void *clave, const t_Cmp cmp)
{
	if (l == NULL) {
		return ERR_LISTA_VACIA;
	}
	list_node_t **aux = l;
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

void list_show(const list_t *l, const t_Prnt prnt)
{
	while (*l) {
		prnt((*l)->dato);
		l = &(*l)->sig;
	}
}

int list_invert(list_t *l)
{
	list_node_t *prev, *act, *sig;
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

int list_concat(list_t *l1, list_t *l2)
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
	list_node_t *aux = *l1;
	while (aux->sig != NULL) {
		aux = aux->sig;
	}
	aux->sig = *l2;
	*l2 = NULL;
	return OK;
}

int list_count_appear(list_t *l, const void *d, int *res, const t_Cmp cmp)
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

int list_contain(list_t *l, const void *d, const t_Cmp cmp)
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

int list_order(list_t *l, const int ordenamiento, const t_Cmp cmp)
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

void list_show_invert(list_t *l, const t_Prnt mostrar)
{
	list_t lAux = NULL;
	list_node_t *aux = NULL;
	if (l == NULL || mostrar == NULL) {
		return;
	}
	aux = *l;
	while (aux) {
		list_node_t *nue = (list_node_t *)malloc(sizeof(list_node_t));
		if (nue == NULL) {
			break;
		}
		nue->dato = malloc(aux->tam);
		if (nue->dato == NULL) {
			free(nue);
			break;
		}
		memcpy(nue->dato, aux->dato, aux->tam);
		nue->tam = aux->tam;
		nue->sig = lAux;
		lAux = nue;
		aux = aux->sig;
	}
	while (lAux) {
		list_node_t *sig = lAux->sig;
		mostrar(lAux->dato);
		free(lAux->dato);
		free(lAux);
		lAux = sig;
	}
}
