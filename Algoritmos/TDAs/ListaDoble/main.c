#include "double_list.h"
#include <types.h>
#include <stdlib.h>
#include <string.h>

static list_node_t *list_head(list_node_t *n)
{
	while (n && n->ant) {
		n = n->ant;
	}
	return n;
}

static list_node_t *list_tail(list_node_t *n)
{
	while (n && n->sig) {
		n = n->sig;
	}
	return n;
}

void list_create(list_t *l)
{
	*l = NULL;
}

int list_clear(list_t *l)
{
	list_node_t *act = *l;
	int cant = 0;
	if (act) {
		while (act->ant) {
			act = act->ant;
		}
		while (act) {
			list_node_t *aux = act->sig;
			free(act->dato);
			free(act);
			act = aux;
			cant++;
		}
		*l = NULL;
	}
	return cant;
}

int list_is_empty(const list_t *l)
{
	return *l == NULL;
}

int list_is_full(const list_t *l, const unsigned tam)
{
	list_node_t *nue = (list_node_t *)malloc(sizeof(list_node_t));
	void *aux = malloc(tam);
	free(aux);
	free(nue);
	return aux == NULL || nue == NULL;
}

int list_push_last(list_t *l, const void *d, const unsigned tam)
{
	list_node_t *act = *l, *nue;
	if (act) {
		while (act->sig) {
			act = act->sig;
		}
	}
	if ((nue = (list_node_t *)malloc(sizeof(list_node_t))) == NULL ||
	    (nue->dato = malloc(tam)) == NULL) {
		free(nue);
		return LIST_ERR_MEM_FULL;
	}
	memcpy(nue->dato, d, tam);
	nue->tam = tam;
	nue->tam = tam;
	nue->sig = NULL;
	nue->ant = act;

	if (act) {
		act->sig = nue;
	}
	*l = nue;
	return OK;
}

int list_push_first(list_t *l, const void *d, const unsigned tam)
{
	list_node_t *act = *l, *nue;
	if (act) {
		while (act->ant) {
			act = act->ant;
		}
	}
	if ((nue = (list_node_t *)malloc(sizeof(list_node_t))) == NULL ||
	    (nue->dato = malloc(tam)) == NULL) {
		free(nue);
		return LIST_ERR_MEM_FULL;
	}
	memcpy(nue->dato, d, tam);
	nue->tam = tam;
	nue->sig = act;
	nue->ant = NULL;
	if (act) {
		act->ant = nue;
	}
	*l = nue;
	return OK;
}

int list_push_orderer(list_t *l, const void *d, const unsigned tam, cmp_t cmp,
		      acum_t acumulador)
{
	list_node_t *act = *l, *sig, *ant, *nue;
	if (act == NULL) {
		ant = NULL;
		sig = NULL;
	} else {
		int aux;
		while (act->sig && cmp(act->dato, d) < 0) {
			act = act->sig;
		}
		while (act->ant && cmp(act->dato, d) > 0) {
			act = act->ant;
		}
		aux = cmp(act->dato, d);
		if (aux == 0) {
			*l = act;
			if (acumulador) {
				if (acumulador(&act->dato, &act->tam, d, tam) ==
				    0) {
					return LIST_ERR_MEM_FULL;
				}
				return LIST_ERR_INVAL;
			}
		}
		if (aux < 0) {
			ant = act;
			sig = act->sig;
		} else {
			ant = act->ant;
			sig = act;
		}
	}
	if ((nue = (list_node_t *)malloc(sizeof(list_node_t))) == NULL ||
	    (nue->dato = malloc(tam)) == NULL) {
		free(nue);
		return LIST_ERR_MEM_FULL;
	}
	memcpy(nue->dato, d, tam);
	nue->tam = tam;
	nue->sig = sig;
	nue->ant = ant;
	if (sig) {
		sig->ant = nue;
	}
	if (ant) {
		ant->sig = nue;
	}
	*l = nue;
	return OK;
}

int list_show_lr(const list_t *l, prnt_t print)
{
	list_node_t *act = *l;
	int cant = 0;
	if (act) {
		while (act->sig) {
			act = act->sig;
		}
		while (act) {
			print(act->dato);
			act = act->ant;
			cant++;
		}
	}
	return cant;
}

int list_show_rl(const list_t *l, prnt_t print)
{
	list_node_t *act = *l;
	int cant = 0;
	if (act) {
		while (act->ant) {
			act = act->ant;
		}
		while (act) {
			print(act->dato);
			act = act->sig;
			cant++;
		}
	}
	return cant;
}

void list_order(list_t *l, const cmp_t cmp)
{
	list_node_t *act = *l, *sup = NULL, *inf = NULL;
	int marca = 1;
	if (act == NULL) {
		return;
	}
	while (act->ant) {
		act = act->ant;
	}
	while (marca) {
		marca = 0;
		while (act->sig != sup) {
			if (cmp(act->dato, act->sig->dato) > 0) {
				void *inf = act->dato;
				unsigned tam = act->tam;
				marca = 1;
				act->dato = act->sig->dato;
				act->sig->dato = inf;
				act->tam = act->sig->tam;
				act->sig->tam = tam;
			}
			act = act->sig;
		}
		sup = act;
		while (act->ant != inf) {
			if (cmp(act->dato, act->ant->dato) > 0) {
				void *inf = act->dato;
				unsigned tam = act->tam;
				marca = 1;
				act->dato = act->ant->dato;
				act->ant->dato = inf;
				act->tam = act->ant->tam;
				act->ant->tam = tam;
			}
			act = act->ant;
		}
		inf = act;
	}
}

int list_delete_by_key(list_t *l, void *buff, const unsigned int tam, cmp_t cmp)
{
	list_node_t *act = *l;
	int aux;
	if (act == NULL) {
		return 0;
	}
	while (act->sig && cmp(act->dato, buff) < 0) {
		act = act->sig;
	}
	while (act->ant && cmp(act->dato, buff) > 0) {
		act = act->ant;
	}
	aux = cmp(act->dato, buff);
	if (aux == 0) {
		list_node_t *ant = act->ant, *sig = act->sig;
		if (ant) {
			ant->sig = sig;
		}
		if (sig) {
			sig->ant = ant;
			*l = sig;
		} else {
			*l = sig;
		}
		memcpy(buff, act->dato, tam);
		free(act->dato);
		free(act);
		return OK;
	}
	return LIST_ERR_INVAL;
}

int list_pull_first(list_t *l, void *buff, const unsigned tam)
{
	list_node_t *act;
	if (l == NULL || *l == NULL) {
		return LIST_ERR_EMPTY;
	}
	act = list_head(*l);
	if (buff) {
		memcpy(buff, act->dato, MIN(tam, act->tam));
	}
	if (act->sig) {
		act->sig->ant = NULL;
	}
	*l = act->sig;
	free(act->dato);
	free(act);
	return OK;
}

int list_pull_last(list_t *l, void *buff, const unsigned tam)
{
	list_node_t *act;
	if (l == NULL || *l == NULL) {
		return LIST_ERR_EMPTY;
	}
	act = list_tail(*l);
	if (buff) {
		memcpy(buff, act->dato, MIN(tam, act->tam));
	}
	if (act->ant) {
		act->ant->sig = NULL;
	}
	*l = act->ant;
	free(act->dato);
	free(act);
	return OK;
}

int list_see_last(list_t *l, void *buff, const unsigned tam)
{
	list_node_t *act;
	if (l == NULL || *l == NULL) {
		return LIST_ERR_EMPTY;
	}
	act = list_tail(*l);
	if (buff) {
		memcpy(buff, act->dato, MIN(tam, act->tam));
	}
	return OK;
}

int list_see_first(list_t *l, void *buff, const unsigned tam)
{
	list_node_t *act;
	if (l == NULL || *l == NULL) {
		return LIST_ERR_EMPTY;
	}
	act = list_head(*l);
	if (buff) {
		memcpy(buff, act->dato, MIN(tam, act->tam));
	}
	return OK;
}

int list_see_in_pos(list_t *l, void *buff, const unsigned tam, int pos)
{
	list_node_t *act;
	int i = 0;
	if (l == NULL || *l == NULL || pos < 0) {
		return LIST_ERR_EMPTY;
	}
	act = list_head(*l);
	while (act && i < pos) {
		act = act->sig;
		i++;
	}
	if (act == NULL) {
		return LIST_ERR_NOT_FOUND;
	}
	if (buff) {
		memcpy(buff, act->dato, MIN(tam, act->tam));
	}
	return OK;
}

int list_len(list_t *l)
{
	int cont = 0;
	list_node_t *act;
	if (l == NULL || *l == NULL) {
		return cont;
	}
	act = list_head(*l);
	while (act) {
		cont++;
		act = act->sig;
	}
	return cont;
}

int list_search(list_t *l, void *buff, const unsigned tam, cmp_t cmp)
{
	list_node_t *act;
	if (l == NULL || *l == NULL) {
		return LIST_ERR_EMPTY;
	}
	act = list_head(*l);
	while (act && cmp(buff, act->dato) != 0) {
		act = act->sig;
	}
	if (act == NULL) {
		return LIST_ERR_NOT_FOUND;
	}
	if (buff) {
		memcpy(buff, act->dato, MIN(tam, act->tam));
	}
	*l = act;
	return OK;
}

int list_copy(list_t *l, list_t *lCopia)
{
	list_node_t *act;
	list_node_t *tail = NULL;
	if (l == NULL || lCopia == NULL) {
		return LIST_ERR_EMPTY;
	}
	act = *l ? list_head(*l) : NULL;
	*lCopia = NULL;
	while (act) {
		list_node_t *nue = (list_node_t *)malloc(sizeof(list_node_t));
		if (nue == NULL) {
			return LIST_ERR_MEM_FULL;
		}
		nue->dato = malloc(act->tam);
		if (nue->dato == NULL) {
			free(nue);
			return LIST_ERR_MEM_FULL;
		}
		memcpy(nue->dato, act->dato, act->tam);
		nue->tam = act->tam;
		nue->sig = NULL;
		nue->ant = tail;
		if (tail) {
			tail->sig = nue;
		} else {
			*lCopia = nue;
		}
		tail = nue;
		act = act->sig;
	}
	return OK;
}

int list_push_in_pos(list_t *l, const void *buff, const unsigned tam,
		     const int pos)
{
	list_node_t *act;
	list_node_t *nue;
	int i = 0;
	if (l == NULL || pos < 0) {
		return LIST_ERR_EMPTY;
	}
	act = *l ? list_head(*l) : NULL;
	while (act && i < pos) {
		act = act->sig;
		i++;
	}
	if ((nue = (list_node_t *)malloc(sizeof(list_node_t))) == NULL ||
	    (nue->dato = malloc(tam)) == NULL) {
		free(nue);
		return LIST_ERR_MEM_FULL;
	}
	memcpy(nue->dato, buff, tam);
	nue->tam = tam;
	if (act == NULL) {
		list_node_t *tail = *l ? list_tail(*l) : NULL;
		nue->sig = NULL;
		nue->ant = tail;
		if (tail) {
			tail->sig = nue;
		}
		*l = nue;
		return OK;
	}
	nue->sig = act;
	nue->ant = act->ant;
	if (act->ant) {
		act->ant->sig = nue;
	}
	act->ant = nue;
	*l = nue;
	return OK;
}

int list_push_after_key(list_t *l, const void *d, const unsigned tam,
			const void *clave, const cmp_t cmp)
{
	list_node_t *act;
	list_node_t *nue;
	if (l == NULL || *l == NULL) {
		return LIST_ERR_EMPTY;
	}
	act = list_head(*l);
	while (act && cmp(clave, act->dato) != 0) {
		act = act->sig;
	}
	if (act == NULL) {
		return LIST_ERR_NOT_FOUND;
	}
	if ((nue = (list_node_t *)malloc(sizeof(list_node_t))) == NULL ||
	    (nue->dato = malloc(tam)) == NULL) {
		free(nue);
		return LIST_ERR_MEM_FULL;
	}
	memcpy(nue->dato, d, tam);
	nue->tam = tam;
	nue->ant = act;
	nue->sig = act->sig;
	if (act->sig) {
		act->sig->ant = nue;
	}
	act->sig = nue;
	*l = nue;
	return OK;
}

int list_push_before_key(list_t *l, const void *d, const unsigned tam,
			 const void *clave, const cmp_t cmp)
{
	list_node_t *act;
	list_node_t *nue;
	if (l == NULL || *l == NULL) {
		return LIST_ERR_EMPTY;
	}
	act = list_head(*l);
	while (act && cmp(clave, act->dato) != 0) {
		act = act->sig;
	}
	if (act == NULL) {
		return LIST_ERR_NOT_FOUND;
	}
	if ((nue = (list_node_t *)malloc(sizeof(list_node_t))) == NULL ||
	    (nue->dato = malloc(tam)) == NULL) {
		free(nue);
		return LIST_ERR_MEM_FULL;
	}
	memcpy(nue->dato, d, tam);
	nue->tam = tam;
	nue->sig = act;
	nue->ant = act->ant;
	if (act->ant) {
		act->ant->sig = nue;
	}
	act->ant = nue;
	*l = nue;
	return OK;
}

int list_delete_pos(list_t *l, void *buff, const unsigned int tam,
		    const int pos)
{
	list_node_t *act;
	int i = 0;
	if (l == NULL || *l == NULL || pos < 0) {
		return LIST_ERR_EMPTY;
	}
	act = list_head(*l);
	while (act && i < pos) {
		act = act->sig;
		i++;
	}
	if (act == NULL) {
		return LIST_ERR_NOT_FOUND;
	}
	if (buff) {
		memcpy(buff, act->dato, MIN(tam, act->tam));
	}
	if (act->ant) {
		act->ant->sig = act->sig;
	}
	if (act->sig) {
		act->sig->ant = act->ant;
	}
	*l = act->sig ? act->sig : act->ant;
	free(act->dato);
	free(act);
	return OK;
}

int list_delete_before_key(list_t *l, void *buff, const unsigned int tam,
			   const void *clave, const cmp_t cmp)
{
	list_node_t *act;
	if (l == NULL || *l == NULL) {
		return LIST_ERR_EMPTY;
	}
	act = list_head(*l);
	while (act && cmp(clave, act->dato) != 0) {
		act = act->sig;
	}
	if (act == NULL || act->ant == NULL) {
		return LIST_ERR_NOT_FOUND;
	}
	act = act->ant;
	if (buff) {
		memcpy(buff, act->dato, MIN(tam, act->tam));
	}
	if (act->ant) {
		act->ant->sig = act->sig;
	}
	if (act->sig) {
		act->sig->ant = act->ant;
	}
	*l = act->sig ? act->sig : act->ant;
	free(act->dato);
	free(act);
	return OK;
}

int list_delete_after_key(list_t *l, void *buff, const unsigned int tam,
			  const void *clave, const cmp_t cmp)
{
	list_node_t *act;
	if (l == NULL || *l == NULL) {
		return LIST_ERR_EMPTY;
	}
	act = list_head(*l);
	while (act && cmp(clave, act->dato) != 0) {
		act = act->sig;
	}
	if (act == NULL || act->sig == NULL) {
		return LIST_ERR_NOT_FOUND;
	}
	act = act->sig;
	if (buff) {
		memcpy(buff, act->dato, MIN(tam, act->tam));
	}
	if (act->ant) {
		act->ant->sig = act->sig;
	}
	if (act->sig) {
		act->sig->ant = act->ant;
	}
	*l = act->sig ? act->sig : act->ant;
	free(act->dato);
	free(act);
	return OK;
}

int list_invert(list_t *l)
{
	list_node_t *act;
	list_node_t *tmp;
	if (l == NULL || *l == NULL) {
		return LIST_ERR_EMPTY;
	}
	act = list_head(*l);
	while (act) {
		tmp = act->sig;
		act->sig = act->ant;
		act->ant = tmp;
		if (tmp == NULL) {
			*l = act;
		}
		act = tmp;
	}
	return OK;
}

int list_concat(list_t *l1, list_t *l2)
{
	list_node_t *tail1;
	list_node_t *head2;
	if (l1 == NULL || l2 == NULL) {
		return LIST_ERR_EMPTY;
	}
	if (*l2 == NULL) {
		return OK;
	}
	if (*l1 == NULL) {
		*l1 = *l2;
		*l2 = NULL;
		return OK;
	}
	tail1 = list_tail(*l1);
	head2 = list_head(*l2);
	tail1->sig = head2;
	head2->ant = tail1;
	*l1 = list_tail(head2);
	*l2 = NULL;
	return OK;
}

int list_count_appear(list_t *l, const void *d, const cmp_t cmp)
{
	int res;
	list_node_t *act;
	if (*l == NULL) {
		return LIST_ERR_EMPTY;
	}
	res = 0;
	act = list_head(*l);
	while (act) {
		if (cmp(d, act->dato) == 0) {
			(res)++;
		}
		act = act->sig;
	}
	return res;
}

int list_contain(list_t *l, const void *d, const cmp_t cmp)
{
	list_node_t *act;
	if (l == NULL || *l == NULL) {
		return LIST_ERR_EMPTY;
	}
	act = list_head(*l);
	while (act) {
		if (cmp(d, act->dato) == 0) {
			return OK;
		}
		act = act->sig;
	}
	return LIST_ERR_NOT_FOUND;
}

int list_update_by_pos(list_t *l, const void *d, int pos, accion_t accion)
{
	list_node_t *act;
	int i = 0;
	if (l == NULL || *l == NULL || accion == NULL || pos < 0) {
		return LIST_ERR_EMPTY;
	}
	act = list_head(*l);
	while (act && i < pos) {
		act = act->sig;
		i++;
	}
	if (act == NULL) {
		return LIST_ERR_NOT_FOUND;
	}
	accion(act->dato, d);
	*l = act;
	return OK;
}

int list_update_by_key(list_t *l, const void *d, cmp_t cmp, accion_t accion)
{
	list_node_t *act;
	if (l == NULL || *l == NULL || accion == NULL || cmp == NULL) {
		return LIST_ERR_EMPTY;
	}
	act = list_head(*l);
	while (act && cmp(d, act->dato) != 0) {
		act = act->sig;
	}
	if (act == NULL) {
		return LIST_ERR_NOT_FOUND;
	}
	accion(act->dato, d);
	*l = act;
	return OK;
}

int list_search_pos(list_t *l, const void *d, cmp_t cmp)
{
	list_node_t *act;
	int pos = 0;
	if (l == NULL || *l == NULL || cmp == NULL) {
		return LIST_ERR_EMPTY;
	}
	act = list_head(*l);
	while (act) {
		if (cmp(d, act->dato) == 0) {
			*l = act;
			return pos;
		}
		pos++;
		act = act->sig;
	}
	return LIST_ERR_NOT_FOUND;
}
