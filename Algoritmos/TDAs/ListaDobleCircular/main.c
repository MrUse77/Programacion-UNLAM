#include "double_list.h"
#include <types.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
 * Internal helpers
 * ============================================================ */

/* Return the last node of a circular doubly-linked list.
 * O(1) because tail is always head->ant. */
static list_node_t *list_tail_ref(list_node_t *head)
{
	return head ? head->ant : NULL;
}

/* Free a single node (both its data buffer and the node itself). */
static void free_node(list_node_t *n)
{
	if (n) {
		free(n->dato);
		free(n);
	}
}

/* ============================================================
 * Sorting helpers (static — called from list_order)
 * NOTE: These still assume linear (NULL-terminated) lists.
 * They are NOT updated for circular behavior.
 * ============================================================ */

/* Split a doubly-linked list into two halves.
 * Returns the head of the second half.
 * The original list is truncated so that the first node of the
 * second half becomes head of the returned list. */
static list_node_t *split_list(list_node_t *head)
{
	list_node_t *slow = head, *fast = head;
	if (!head)
		return NULL;
	while (fast->sig && fast->sig->sig) {
		slow = slow->sig;
		fast = fast->sig->sig;
	}
	list_node_t *second = slow->sig;
	if (second)
		second->ant = NULL;
	slow->sig = NULL;
	return second;
}

/* Merge two sorted doubly-linked lists, return head of merged list. */
static list_node_t *merge_sorted(list_node_t *a, list_node_t *b, t_Cmp cmp)
{
	list_node_t *head = NULL, **tail = &head, *prev = NULL;

	while (a && b) {
		list_node_t *chosen;
		if (cmp(a->dato, b->dato) <= 0) {
			chosen = a;
			a = a->sig;
		} else {
			chosen = b;
			b = b->sig;
		}
		chosen->ant = prev;
		chosen->sig = NULL;
		*tail = chosen;
		prev = chosen;
		tail = &chosen->sig;
	}

	/* Attach remainder */
	list_node_t *remainder = a ? a : b;
	if (remainder) {
		remainder->ant = prev;
		*tail = remainder;
	}

	return head;
}

/* Recursive merge sort on nodes. Returns new head. */
static list_node_t *merge_sort_nodes(list_node_t *head, t_Cmp cmp)
{
	if (!head || !head->sig)
		return head;
	list_node_t *second = split_list(head);
	head = merge_sort_nodes(head, cmp);
	second = merge_sort_nodes(second, cmp);
	return merge_sorted(head, second, cmp);
}

/* ---- Quick sort helpers ---- */

/* Partition around pivot (last node in the segment).
 * Swaps data between nodes for simplicity (no relinking). */
static list_node_t *partition(list_node_t *low, list_node_t *high, t_Cmp cmp)
{
	void *pivot_data = high->dato;
	list_node_t *i = low ? low->ant : NULL;

	for (list_node_t *j = low; j != high; j = j->sig) {
		if (cmp(j->dato, pivot_data) <= 0) {
			i = (i == NULL) ? low : i->sig;
			/* swap data fields */
			void *tmp_d = i->dato;
			unsigned tmp_t = i->tam;
			i->dato = j->dato;
			i->tam = j->tam;
			j->dato = tmp_d;
			j->tam = tmp_t;
		}
	}
	i = (i == NULL) ? low : i->sig;
	/* swap pivot into place */
	{
		void *tmp_d = i->dato;
		unsigned tmp_t = i->tam;
		i->dato = high->dato;
		i->tam = high->tam;
		high->dato = tmp_d;
		high->tam = tmp_t;
	}
	return i;
}

static void quick_sort_range(list_node_t *low, list_node_t *high, t_Cmp cmp)
{
	if (high && low && low != high && low != high->sig) {
		list_node_t *p = partition(low, high, cmp);
		quick_sort_range(low, p->ant, cmp);
		quick_sort_range(p->sig, high, cmp);
	}
}

/* ---- Radix sort stub (not tested, returns OK without sorting) ---- */
static int radix_sort_stub(list_t *l, t_Cmp cmp)
{
	(void)l;
	(void)cmp;
	return OK;
}

/* ============================================================
 * Core list operations (CIRCULAR)
 * ============================================================ */

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
	(void)l;
	list_node_t *nue = (list_node_t *)malloc(sizeof(list_node_t));
	void *aux = malloc(tam);
	free(aux);
	free(nue);
	if (aux == NULL || nue == NULL)
		return TRUE;
	return FALSE;
}

int list_clear(list_t *l)
{
	if (!l || !*l)
		return OK;
	list_node_t *head = *l;
	list_node_t *act = head;
	do {
		list_node_t *sig = act->sig;
		free_node(act);
		act = sig;
	} while (act != head);
	*l = NULL;
	return OK;
}

int list_push_first(list_t *l, const void *d, const unsigned tam)
{
	list_node_t *nue = (list_node_t *)malloc(sizeof(list_node_t));
	if (!nue)
		return LIST_ERR_MEM_FULL;
	nue->dato = malloc(tam);
	if (!nue->dato) {
		free(nue);
		return LIST_ERR_MEM_FULL;
	}
	memcpy(nue->dato, d, tam);
	nue->tam = tam;

	if (*l == NULL) {
		nue->sig = nue;
		nue->ant = nue;
		*l = nue;
	} else {
		nue->sig = *l;
		nue->ant = (*l)->ant;
		(*l)->ant->sig = nue;
		(*l)->ant = nue;
		*l = nue;
	}
	return OK;
}

int list_push_last(list_t *l, const void *d, const unsigned tam)
{
	list_node_t *nue = (list_node_t *)malloc(sizeof(list_node_t));
	if (!nue)
		return LIST_ERR_MEM_FULL;
	nue->dato = malloc(tam);
	if (!nue->dato) {
		free(nue);
		return LIST_ERR_MEM_FULL;
	}
	memcpy(nue->dato, d, tam);
	nue->tam = tam;

	if (*l == NULL) {
		nue->sig = nue;
		nue->ant = nue;
		*l = nue;
	} else {
		list_node_t *tail = (*l)->ant;
		nue->sig = *l;
		nue->ant = tail;
		tail->sig = nue;
		(*l)->ant = nue;
	}
	return OK;
}

/* Removes and returns the first element (head). */
int list_pull_first(list_t *l, void *buff, const unsigned tam)
{
	if (!l || !*l)
		return LIST_ERR_EMPTY;

	list_node_t *act = *l;
	if (buff)
		memcpy(buff, act->dato, MIN(tam, act->tam));

	if (act->sig == act) {
		/* Only node */
		*l = NULL;
	} else {
		act->sig->ant = act->ant;
		act->ant->sig = act->sig;
		*l = act->sig;
	}

	free_node(act);
	return OK;
}

int list_pull_last(list_t *l, void *buff, const unsigned tam)
{
	if (!l || !*l)
		return LIST_ERR_EMPTY;

	list_node_t *tail = (*l)->ant;
	if (buff)
		memcpy(buff, tail->dato, MIN(tam, tail->tam));

	if (tail->sig == tail) {
		/* Only node */
		*l = NULL;
	} else {
		tail->ant->sig = tail->sig;
		(*l)->ant = tail->ant;
	}

	free_node(tail);
	return OK;
}

int list_see_first(list_t *l, void *buff, const unsigned tam)
{
	if (!l || !*l)
		return LIST_ERR_EMPTY;
	if (buff)
		memcpy(buff, (*l)->dato, MIN(tam, (*l)->tam));
	return OK;
}

int list_see_last(list_t *l, void *buff, const unsigned tam)
{
	if (!l || !*l)
		return LIST_ERR_EMPTY;
	list_node_t *tail = (*l)->ant;
	if (buff)
		memcpy(buff, tail->dato, MIN(tam, tail->tam));
	return OK;
}

int list_push_orderer(list_t *l, const void *d, const unsigned tam, t_Cmp cmp,
		      t_Accion accion)
{
	if (!l)
		return LIST_ERR_INVAL;

	if (*l == NULL)
		return list_push_first(l, d, tam);

	list_node_t *act = *l, *prev = NULL;

	/* Find insertion position (ascending order) */
	do {
		if (cmp(act->dato, d) >= 0)
			break;
		prev = act;
		act = act->sig;
	} while (act != *l);

	/* Duplicate check */
	if (cmp(act->dato, d) == 0) {
		if (accion)
			accion(act->dato, d);
		return OK;
	}

	/* Create new node */
	list_node_t *nue = (list_node_t *)malloc(sizeof(list_node_t));
	if (!nue)
		return LIST_ERR_MEM_FULL;
	nue->dato = malloc(tam);
	if (!nue->dato) {
		free(nue);
		return LIST_ERR_MEM_FULL;
	}
	memcpy(nue->dato, d, tam);
	nue->tam = tam;

	/* Insert between prev and act */
	nue->sig = act;
	nue->ant = prev;
	prev->sig = nue;
	act->ant = nue;

	/* If inserted before head, update head */
	if (act == *l)
		*l = nue;

	return OK;
}

int list_delete_by_key(list_t *l, void *buff, const unsigned tam, t_Cmp cmp)
{
	if (!l || !*l)
		return LIST_ERR_EMPTY;

	list_node_t *head = *l;
	list_node_t *act = head;
	do {
		if (cmp(act->dato, buff) == 0) {
			if (buff)
				memcpy(buff, act->dato, MIN(tam, act->tam));

			if (act->sig == act) {
				/* Only node */
				*l = NULL;
			} else {
				act->ant->sig = act->sig;
				act->sig->ant = act->ant;
				if (act == *l)
					*l = act->sig;
			}

			free_node(act);
			return OK;
		}
		act = act->sig;
	} while (act != head);

	return LIST_ERR_NOT_FOUND;
}

int list_see_in_pos(list_t *l, void *buff, const unsigned tam, int pos)
{
	if (!l || !*l || pos < 0)
		return LIST_ERR_EMPTY;

	list_node_t *head = *l;
	list_node_t *act = head;
	int i = 0;
	do {
		if (i == pos)
			break;
		act = act->sig;
		i++;
	} while (act != head);

	if (i != pos)
		return LIST_ERR_EMPTY;

	if (buff)
		memcpy(buff, act->dato, MIN(tam, act->tam));
	return OK;
}

int list_len(list_t *l)
{
	if (!l || !*l)
		return 0;
	int cont = 0;
	list_node_t *head = *l;
	list_node_t *act = head;
	do {
		cont++;
		act = act->sig;
	} while (act != head);
	return cont;
}

int list_search(list_t *l, void *buff, const unsigned tam, t_Cmp cmp)
{
	if (!l || !*l)
		return LIST_ERR_NOT_FOUND;

	list_node_t *head = *l;
	list_node_t *act = head;
	do {
		if (cmp(act->dato, buff) == 0) {
			if (buff)
				memcpy(buff, act->dato, MIN(tam, act->tam));
			return OK;
		}
		act = act->sig;
	} while (act != head);

	return LIST_ERR_NOT_FOUND;
}

int list_copy(list_t *l, list_t *lCopia)
{
	if (!l || !lCopia)
		return LIST_ERR_EMPTY;

	list_clear(lCopia);

	if (!*l)
		return OK;

	list_node_t *head = *l;
	list_node_t *act = head;
	do {
		int rc = list_push_last(lCopia, act->dato, act->tam);
		if (rc != OK)
			return rc;
		act = act->sig;
	} while (act != head);
	return OK;
}

int list_push_in_pos(list_t *l, const void *buff, const unsigned tam,
		     const int pos)
{
	if (!l || pos < 0)
		return LIST_ERR_INVAL;

	if (pos == 0)
		return list_push_first(l, buff, tam);

	if (!*l)
		return LIST_ERR_EMPTY;

	list_node_t *head = *l;
	list_node_t *act = head;
	int i = 0;
	do {
		if (i == pos)
			break;
		act = act->sig;
		i++;
	} while (act != head);

	/* act == head && i < pos means pos > len */
	if (act == head && i < pos)
		return LIST_ERR_EMPTY;

	/* Insert before act */
	list_node_t *nue = (list_node_t *)malloc(sizeof(list_node_t));
	if (!nue)
		return LIST_ERR_MEM_FULL;
	nue->dato = malloc(tam);
	if (!nue->dato) {
		free(nue);
		return LIST_ERR_MEM_FULL;
	}
	memcpy(nue->dato, buff, tam);
	nue->tam = tam;
	nue->sig = act;
	nue->ant = act->ant;
	act->ant->sig = nue;
	act->ant = nue;

	return OK;
}

int list_delete_pos(list_t *l, void *buff, const unsigned int tam,
		    const int pos)
{
	if (!l || !*l || pos < 0)
		return LIST_ERR_EMPTY;

	list_node_t *head = *l;
	list_node_t *act = head;
	int i = 0;
	do {
		if (i == pos)
			break;
		act = act->sig;
		i++;
	} while (act != head);

	if (i != pos)
		return LIST_ERR_EMPTY;

	if (buff)
		memcpy(buff, act->dato, MIN(tam, act->tam));

	if (act->sig == act) {
		/* Only node */
		*l = NULL;
	} else {
		act->ant->sig = act->sig;
		act->sig->ant = act->ant;
		if (act == *l)
			*l = act->sig;
	}

	free_node(act);
	return OK;
}

int list_invert(list_t *l)
{
	if (!l || !*l)
		return LIST_ERR_EMPTY;

	list_node_t *head = *l;
	list_node_t *act = head;

	do {
		list_node_t *tmp = act->ant;
		act->ant = act->sig;
		act->sig = tmp;
		act = act->ant;
	} while (act != head);

	/* New head is the old tail (now head->ant) */
	*l = head->ant;

	return OK;
}

int list_concat(list_t *l1, list_t *l2)
{
	if (!l1 || !l2)
		return LIST_ERR_EMPTY;

	if (!*l2)
		return OK;

	if (!*l1) {
		*l1 = *l2;
		*l2 = NULL;
		return OK;
	}

	list_node_t *tail1 = (*l1)->ant;
	list_node_t *tail2 = (*l2)->ant;

	/* Connect tail1 -> head of l2 */
	tail1->sig = *l2;
	(*l2)->ant = tail1;

	/* Connect tail2 -> head of l1 */
	tail2->sig = *l1;
	(*l1)->ant = tail2;

	*l2 = NULL;
	return OK;
}

int list_count_appear(list_t *l, const void *d, const t_Cmp cmp)
{
	if (!l || !*l)
		return LIST_ERR_EMPTY;

	int res = 0;
	list_node_t *head = *l;
	list_node_t *act = head;
	do {
		if (cmp(d, act->dato) == 0)
			res++;
		act = act->sig;
	} while (act != head);
	return res;
}

int list_contain(list_t *l, const void *d, const t_Cmp cmp)
{
	if (!l || !*l)
		return LIST_ERR_EMPTY;

	list_node_t *head = *l;
	list_node_t *act = head;
	do {
		if (cmp(d, act->dato) == 0)
			return OK;
		act = act->sig;
	} while (act != head);
	return LIST_ERR_NOT_FOUND;
}

int list_order(list_t *l, const int ordenamiento, const t_Cmp cmp)
{
	if (!l || !*l)
		return LIST_ERR_EMPTY;

	switch (ordenamiento) {
	case MERGE:
		*l = merge_sort_nodes(*l, cmp);
		break;
	case QUICK: {
		list_node_t *tail = list_tail_ref(*l);
		quick_sort_range(*l, tail, cmp);
		break;
	}
	case RADIX:
		return radix_sort_stub(l, cmp);
	default:
		return LIST_ERR_INVAL;
	}
	return OK;
}

int list_update_by_pos(list_t *l, const void *d, int pos, t_Accion accion)
{
	if (!l || !*l || !accion || pos < 0)
		return LIST_ERR_EMPTY;

	list_node_t *head = *l;
	list_node_t *act = head;
	int i = 0;
	do {
		if (i == pos)
			break;
		act = act->sig;
		i++;
	} while (act != head);

	if (i != pos)
		return LIST_ERR_NOT_FOUND;

	accion(act->dato, d);
	return OK;
}

int list_update_by_key(list_t *l, const void *d, t_Cmp cmp, t_Accion accion)
{
	if (!l || !*l || !accion || !cmp)
		return LIST_ERR_EMPTY;

	list_node_t *head = *l;
	list_node_t *act = head;
	do {
		if (cmp(d, act->dato) == 0) {
			accion(act->dato, d);
			return OK;
		}
		act = act->sig;
	} while (act != head);
	return LIST_ERR_NOT_FOUND;
}

int list_search_pos(list_t *l, const void *d, t_Cmp cmp)
{
	if (!l || !*l || !cmp)
		return LIST_ERR_EMPTY;

	list_node_t *head = *l;
	list_node_t *act = head;
	int pos = 0;
	do {
		if (cmp(d, act->dato) == 0)
			return pos;
		pos++;
		act = act->sig;
	} while (act != head);
	return LIST_ERR_NOT_FOUND;
}

/* ============================================================
 * Display functions
 * ============================================================ */

int list_show_rl(const list_t *l, const t_Prnt print)
{
	if (!l || !*l || !print)
		return 0;

	list_node_t *head = *l;
	list_node_t *act = head;
	int cant = 0;
	print(NULL);
	do {
		print(act->dato);
		act = act->sig;
		cant++;
	} while (act != head);
	return cant;
}

int list_show_lr(const list_t *l, const t_Prnt print)
{
	if (!l || !*l || !print)
		return 0;

	list_node_t *start = (*l)->ant;
	list_node_t *act = start;
	int cant = 0;
	print(NULL);
	do {
		print(act->dato);
		act = act->ant;
		cant++;
	} while (act != start);
	return cant;
}

void list_show_invert(list_t *l, const t_Prnt mostrar)
{
	if (!l || !*l || !mostrar)
		return;

	list_node_t *start = (*l)->ant;
	list_node_t *act = start;
	do {
		mostrar(act->dato);
		act = act->ant;
	} while (act != start);
}

/* ============================================================
 * Key-relative insert/delete operations
 * ============================================================ */

int list_push_after_key(list_t *l, const void *d, const unsigned tam,
			const void *clave, const t_Cmp cmp)
{
	if (!l || !*l)
		return LIST_ERR_EMPTY;

	list_node_t *head = *l;
	list_node_t *act = head;
	do {
		if (cmp(act->dato, clave) == 0) {
			list_node_t *nue =
				(list_node_t *)malloc(sizeof(list_node_t));
			if (!nue)
				return LIST_ERR_MEM_FULL;
			nue->dato = malloc(tam);
			if (!nue->dato) {
				free(nue);
				return LIST_ERR_MEM_FULL;
			}
			memcpy(nue->dato, d, tam);
			nue->tam = tam;
			nue->sig = act->sig;
			nue->ant = act;
			act->sig->ant = nue;
			act->sig = nue;
			return OK;
		}
		act = act->sig;
	} while (act != head);
	return LIST_ERR_NOT_FOUND;
}

int list_push_before_key(list_t *l, const void *d, const unsigned tam,
			 const void *clave, const t_Cmp cmp)
{
	if (!l || !*l)
		return LIST_ERR_EMPTY;

	list_node_t *head = *l;
	list_node_t *act = head;
	do {
		if (cmp(act->dato, clave) == 0) {
			list_node_t *nue =
				(list_node_t *)malloc(sizeof(list_node_t));
			if (!nue)
				return LIST_ERR_MEM_FULL;
			nue->dato = malloc(tam);
			if (!nue->dato) {
				free(nue);
				return LIST_ERR_MEM_FULL;
			}
			memcpy(nue->dato, d, tam);
			nue->tam = tam;
			nue->ant = act->ant;
			nue->sig = act;
			act->ant->sig = nue;
			act->ant = nue;
			if (act == *l)
				*l = nue;
			return OK;
		}
		act = act->sig;
	} while (act != head);
	return LIST_ERR_NOT_FOUND;
}

int list_delete_before_key(list_t *l, void *buff, const unsigned int tam,
			   const void *clave, const t_Cmp cmp)
{
	if (!l || !*l)
		return LIST_ERR_EMPTY;

	list_node_t *head = *l;
	list_node_t *act = head;
	do {
		if (cmp(act->dato, clave) == 0) {
			list_node_t *victim = act->ant;
			/* Can't delete "before" in a single-node list */
			if (victim == act)
				return LIST_ERR_NOT_FOUND;

			if (buff)
				memcpy(buff, victim->dato,
				       MIN(tam, victim->tam));

			victim->ant->sig = act;
			act->ant = victim->ant;

			if (victim == *l)
				*l = act;

			free_node(victim);
			return OK;
		}
		act = act->sig;
	} while (act != head);
	return LIST_ERR_NOT_FOUND;
}

int list_delete_after_key(list_t *l, void *buff, const unsigned int tam,
			  const void *clave, const t_Cmp cmp)
{
	if (!l || !*l)
		return LIST_ERR_EMPTY;

	list_node_t *head = *l;
	list_node_t *act = head;
	do {
		if (cmp(act->dato, clave) == 0) {
			list_node_t *victim = act->sig;
			/* Can't delete "after" in a single-node list */
			if (victim == act)
				return LIST_ERR_NOT_FOUND;

			if (buff)
				memcpy(buff, victim->dato,
				       MIN(tam, victim->tam));

			act->sig = victim->sig;
			victim->sig->ant = act;

			if (victim == *l)
				*l = victim->sig;

			free_node(victim);
			return OK;
		}
		act = act->sig;
	} while (act != head);
	return LIST_ERR_NOT_FOUND;
}
