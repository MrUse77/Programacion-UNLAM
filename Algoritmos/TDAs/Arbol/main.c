#include <binary_search_tree.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static binary_search_tree_node_t **buscar_mayor(binary_search_tree_t *b)
{
	if (!*b) {
		return NULL;
	}
	if ((*b)->der == NULL) {
		return b;
	}
	return buscar_mayor(&(*b)->der);
}

static binary_search_tree_node_t **buscar_menor(binary_search_tree_t *b)
{
	if (!*b) {
		return NULL;
	}
	if ((*b)->izq == NULL) {
		return b;
	}
	return buscar_menor(&(*b)->izq);
}

static binary_search_tree_t *buscar_nodo(binary_search_tree_t *b, void *d,
					 cmp_t cmp)
{
	if (*b == NULL) {
		return NULL;
	}
	int cmpInt = cmp((*b)->dato, d);
	if (cmpInt > 0) {
		return buscar_nodo(&(*b)->izq, d, cmp);
	} else if (cmpInt < 0) {
		return buscar_nodo(&(*b)->der, d, cmp);
	}
	return b;
}

static binary_search_tree_status_t eliminar_raiz(binary_search_tree_t *b)
{
	binary_search_tree_node_t **remp, *elim;
	if (*b == NULL) {
		return TREE_ERR_EMPTY;
	}
	free((*b)->dato);
	if (!(*b)->der && !(*b)->izq) {
		free(*b);
		*b = NULL;
		return TREE_SUCCESS;
	}
	remp = binary_search_tree_height(&(*b)->izq) >=
			       binary_search_tree_height(&(*b)->der) ?
		       buscar_mayor(&(*b)->izq) :
		       buscar_menor(&(*b)->der);
	elim = *remp;
	(*b)->dato = elim->dato;
	(*b)->tam = elim->tam;
	*remp = elim->izq ? elim->izq : elim->der;
	free(elim);
	return TREE_SUCCESS;
}

void binary_search_tree_init(binary_search_tree_t *b)
{
	*b = NULL;
}

void binary_search_tree_walk_in_order(binary_search_tree_t *b, accion_t accion,
				      void *param)
{
	if (*b == NULL) {
		return;
	}
	binary_search_tree_walk_in_order(&(*b)->izq, accion, param);
	accion((*b)->dato, param);
	binary_search_tree_walk_in_order(&(*b)->der, accion, param);
}

void binary_search_tree_walk_post_order(binary_search_tree_t *b,
					accion_t accion, void *param)
{
	if (*b == NULL) {
		return;
	}
	binary_search_tree_walk_post_order(&(*b)->izq, accion, param);
	binary_search_tree_walk_post_order(&(*b)->der, accion, param);
	accion((*b)->dato, param);
}

void binary_search_tree_walk_pre_order(binary_search_tree_t *b, accion_t accion,
				       void *param)
{
	if (*b == NULL) {
		return;
	}
	accion((*b)->dato, param);
	binary_search_tree_walk_pre_order(&(*b)->izq, accion, param);
	binary_search_tree_walk_pre_order(&(*b)->der, accion, param);
}

binary_search_tree_status_t binary_search_tree_insert(binary_search_tree_t *b,
						      const void *d,
						      const size_t tam,
						      const cmp_t cmp)
{
	binary_search_tree_t node = NULL;
	if (*b != NULL) {
		if (cmp((*b)->dato, d) > 0) {
			return binary_search_tree_insert(&(*b)->izq, d, tam,
							 cmp);
		} else if (cmp((*b)->dato, d) < 0) {
			return binary_search_tree_insert(&(*b)->der, d, tam,
							 cmp);
		}
	}
	node = malloc(sizeof(*node));
	if (!node) {
		return TREE_ERR_MEM_FULL;
	}
	node->dato = malloc(tam);
	if (!node->dato) {
		free(node);
		return TREE_ERR_MEM_FULL;
	}
	memcpy(node->dato, d, tam);
	node->tam = tam;
	node->der = NULL;
	node->izq = NULL;
	*b = node;
	return TREE_SUCCESS;
}

int binary_search_tree_load_to_bin_file_sorted(binary_search_tree_t *b,
					       const char *path,
					       unsigned tamInfo);

int binary_search_tree_insert_iter(binary_search_tree_t *b, const void *d,
				   const size_t tam, const cmp_t cmp);

int binary_search_tree_node_count(const binary_search_tree_t *b)
{
	if (*b == NULL) {
		return 0;
	}
	return 1 + binary_search_tree_node_count(&(*b)->izq) +
	       binary_search_tree_node_count(&(*b)->der);
}

int binary_search_tree_node_leaves_count(const binary_search_tree_t *b)
{
	if (*b == NULL) {
		return 0;
	}
	if ((*b)->der == NULL && (*b)->izq == NULL) {
		return 1;
	}
	return binary_search_tree_node_leaves_count(&(*b)->izq) +
	       binary_search_tree_node_leaves_count(&(*b)->der);
}

int binary_search_tree_node_wl_count(const binary_search_tree_t *b)
{
	if (*b == NULL) {
		return 0;
	}
	if ((*b)->izq) {
		return 1 + binary_search_tree_node_wl_count(&(*b)->izq);
	}
	return binary_search_tree_node_wl_count(&(*b)->der);
}

int binary_search_tree_node_wr_count(const binary_search_tree_t *b)
{
	if (*b == NULL) {
		return 0;
	}
	if ((*b)->der) {
		return 1 + binary_search_tree_node_wr_count(&(*b)->der);
	}
	return binary_search_tree_node_wr_count(&(*b)->izq);
}

int binary_search_tree_node_w_cond_count(const binary_search_tree_t *b,
					 cmp_t cmp, void *d)
{
	if (*b == NULL) {
		return 0;
	}
	return cmp((*b)->dato, d) == 0 ?
		       1 +
			       binary_search_tree_node_w_cond_count(&(*b)->izq,
								    cmp, d) +
			       binary_search_tree_node_w_cond_count(&(*b)->izq,
								    cmp, d) :
		       binary_search_tree_node_w_cond_count(&(*b)->der, cmp,
							    d) +
			       binary_search_tree_node_w_cond_count(&(*b)->izq,
								    cmp, d);
}

int binary_search_tree_height(const binary_search_tree_t *b)
{
	int izq, der;
	if (*b == NULL) {
		return 0;
	}
	izq = binary_search_tree_height(&(*b)->izq);
	der = binary_search_tree_height(&(*b)->der);
	return 1 + (izq < der ? der : izq);
}

int binary_search_tree_node_count_in_level(const binary_search_tree_t *b,
					   int height)
{
	if (*b == NULL) {
		return 0;
	}
	if (height == 0) {
		return 1;
	}
	if (height < 0) {
		return 0;
	}
	return binary_search_tree_node_count_in_level(&(*b)->izq, height - 1) +
	       binary_search_tree_node_count_in_level(&(*b)->der, height - 1);
}

int binary_search_tree_node_count_below_level(const binary_search_tree_t *b,
					      int height)
{
	if (*b == NULL) {
		return 0;
	}
	if (height < 0) {
		return binary_search_tree_node_count(b);
	}
	return binary_search_tree_node_count_below_level(&(*b)->izq,
							 height - 1) +
	       binary_search_tree_node_count_below_level(&(*b)->der,
							 height - 1);
}

int binary_search_tree_node_count_within_level(const binary_search_tree_t *b,
					       int height)
{
	if (*b == NULL) {
		return 0;
	}
	if (height == 0) {
		return 1;
	}
	if (height < 0) {
		return 0;
	}
	return 1 +
	       binary_search_tree_node_count_within_level(&(*b)->izq,
							  height - 1) +
	       binary_search_tree_node_count_within_level(&(*b)->der,
							  height - 1);
}

int binary_search_tree_node_count_from_level_inclusive(
	const binary_search_tree_t *b, int height)
{
	if (*b == NULL) {
		return 0;
	}
	if (height <= 0) {
		return binary_search_tree_node_count(b);
	}
	return binary_search_tree_node_count_from_level_inclusive(&(*b)->izq,
								  height - 1) +
	       binary_search_tree_node_count_from_level_inclusive(&(*b)->der,
								  height - 1);
}

int binary_search_tree_node_count_beyond_level(const binary_search_tree_t *b,
					       int height)
{
	if (*b == NULL) {
		return 0;
	}
	if (height <= 0) {
		return 0;
	}
	return 1 +
	       binary_search_tree_node_count_beyond_level(&(*b)->izq,
							  height - 1) +
	       binary_search_tree_node_count_beyond_level(&(*b)->der,
							  height - 1);
}

int binary_search_tree_cut_leaves(binary_search_tree_t *b)
{
	if (*b == NULL) {
		return TREE_ERR_EMPTY;
	}
	if ((*b)->der == NULL && (*b)->izq == NULL) {
		free((*b)->dato);
		free(*b);
		*b = NULL;
	} else {
		binary_search_tree_cut_leaves(&(*b)->izq);
		binary_search_tree_cut_leaves(&(*b)->der);
	}

	return TREE_SUCCESS;
}

binary_search_tree_status_t binary_search_tree_destroy(binary_search_tree_t *b)
{
	if (*b == NULL) {
		return TREE_ERR_EMPTY;
	}
	binary_search_tree_destroy(&(*b)->izq);
	binary_search_tree_destroy(&(*b)->der);
	free((*b)->dato);
	free(*b);
	*b = NULL;
	return TREE_SUCCESS;
}

int binary_search_tree_is_empty(binary_search_tree_t *b)
{
	return *b == NULL ? TRUE : FALSE;
}

binary_search_tree_status_t
binary_search_tree_delete_node(binary_search_tree_t *b, void *buff,
			       const unsigned tam, cmp_t cmp)
{
	if (!*b) {
		return TREE_ERR_EMPTY;
	}
	if (!(b = buscar_nodo(b, buff, cmp))) {
		return TREE_ERR_NOT_FOUND;
	}
	memcpy(buff, (*b)->dato, MIN((*b)->tam, tam));
	return eliminar_raiz(b);
}

int binary_search_tree_search_r(const binary_search_tree_t *bree,
				const void *key, void *buf, size_t n,
				cmp_t cmp);

static void rotate_right(binary_search_tree_t *t)
{
	binary_search_tree_node_t *n = (*t)->izq;
	(*t)->izq = n->der;
	n->der = *t;
	*t = n;
}
static void rotate_left(binary_search_tree_t *t)
{
	binary_search_tree_node_t *n = (*t)->der;
	(*t)->der = n->izq;
	n->izq = *t;
	*t = n;
}
static void tree_to_vine(binary_search_tree_t *t)
{
	while (*t) {
		if (!(*t)->izq) {
			t = &(*t)->der;
		} else {
			rotate_right(t);
		}
	}
}

static void compress(binary_search_tree_t *t, unsigned cant)
{
	unsigned i;
	binary_search_tree_node_t **n = t;
	for (i = 0; i < cant; i++) {
		if (!*n || !(*n)->der) {
			return;
		}
		rotate_left(n);
		n = &(*n)->der;
	}
}

void binary_search_tree_balanced(binary_search_tree_t *t)
{
	unsigned n, m, leaves;
	if (*t == NULL) {
		return;
	}
	tree_to_vine(t);
	n = binary_search_tree_node_count(t);
	if (n < 2)
		return;
	m = 1;
	while (m * 2 <= n + 1) {
		m *= 2;
	}
	leaves = n + 1 - m;
	compress(t, leaves);
	n -= leaves;

	while (n > 1) {
		compress(t, n / 2);
		n /= 2;
	}
}

static bool_t binary_search_tree_is_complete_to(binary_search_tree_t *t,
						int height)
{
	if (!*t) {
		return height < 0 ? TRUE : FALSE;
	}
	if (height == 0) {
		return TRUE;
	}
	return binary_search_tree_is_complete_to(&(*t)->izq, height - 1) &&
	       binary_search_tree_is_complete_to(&(*t)->der, height - 1);
}

bool_t binary_search_tree_is_balanced(binary_search_tree_t *t)
{
	if (!*t) {
		return FALSE;
	}
	int height = binary_search_tree_height(t);
	return binary_search_tree_is_complete_to(t, height - 2);
}
bool_t binary_search_tree_is_complete(binary_search_tree_t *t)
{
	if (!*t) {
		return FALSE;
	}
	int height = binary_search_tree_height(t);
	return binary_search_tree_is_complete_to(t, height - 1);
}
bool_t binary_search_tree_is_avl(binary_search_tree_t *t)
{
	int hi, hd;
	if (!*t) {
		return TRUE;
	}
	hi = binary_search_tree_height(&(*t)->izq);
	hd = binary_search_tree_height(&(*t)->der);
	if (abs(hi - hd) > 1) {
		return FALSE;
	}
	return binary_search_tree_is_avl(&(*t)->der) &&
	       binary_search_tree_is_avl(&(*t)->izq);
}
