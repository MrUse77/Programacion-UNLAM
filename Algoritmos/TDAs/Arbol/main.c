#include <binary_search_tree.h>
#include <string.h>
#include <stdio.h>

static binary_search_tree_node_t **buscar_mayor(binary_search_tree_t *b)
{
	if (*b) {
		return NULL;
	}
	if ((*b)->der == NULL) {
		return b;
	}
	return buscar_mayor(&(*b)->der);
}
static binary_search_tree_node_t **buscar_menor(binary_search_tree_t *b)
{
	if (*b) {
		return NULL;
	}
	if ((*b)->izq == NULL) {
		return b;
	}
	return buscar_mayor(&(*b)->izq);
}

static binary_search_tree_node_t **buscar_nodo(binary_search_tree_t *b, void *d,
					       cmp_t cmp)
{
	if (*b == NULL) {
		return NULL;
	}
	int cmpInt = cmp((*b)->dato, d);
	if (cmpInt > 0) {
		buscar_nodo(&(*b)->izq, d, cmp);
	} else if (cmpInt < 0) {
		buscar_nodo(&(*b)->der, d, cmp);
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
	remp = binary_search_tree_height(&(*b)->izq) >
			       binary_search_tree_height(&(*b)->der) ?
		       buscar_menor(&(*b)->der) :
		       buscar_mayor(&(*b)->izq);
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
	binary_search_tree_walk_in_order(&(*b)->izq, accion, param);
	binary_search_tree_walk_in_order(&(*b)->der, accion, param);
	accion((*b)->dato, param);
}

void binary_search_tree_walk_pre_order(binary_search_tree_t *b, accion_t accion,
				       void *param)
{
	if (*b == NULL) {
		return;
	}
	accion((*b)->dato, param);
	binary_search_tree_walk_in_order(&(*b)->izq, accion, param);
	binary_search_tree_walk_in_order(&(*b)->der, accion, param);
}

binary_search_tree_status_t binary_search_tree_insert(binary_search_tree_t *b,
						      const void *d,
						      const size_t tam,
						      const cmp_t cmp)
{
	binary_search_tree_node_t *node = NULL;
	if (*b != NULL) {
		if (cmp((*b)->dato, d) < 0) {
			return binary_search_tree_insert(&(*b)->izq, d, tam,
							 cmp);
		} else if (cmp((*b)->dato, d) > 0) {
			return binary_search_tree_insert(&(*b)->der, d, tam,
							 cmp);
		}
	}
	node = (binary_search_tree_node_t *)malloc(
		sizeof(binary_search_tree_node_t));
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
	return 1 + izq < der ? der : izq;
}

int binary_search_tree_node_count_in_level(const binary_search_tree_t *b,
					   int height)
{
	int izq, der;
	if (*b == NULL) {
		return 0;
	}
	if (height == 0) {
		return 1;
	}
	izq = binary_search_tree_node_count_in_level(&(*b)->izq, height - 1);
	der = binary_search_tree_node_count_in_level(&(*b)->der, height - 1);
	return der + izq;
}

int binary_search_tree_node_count_below_level(const binary_search_tree_t *b,
					      int height)
{
	int izq, der;
	if (*b == NULL) {
		return 0;
	}
	if (height < 0) {
		return 1;
	}
	izq = binary_search_tree_node_count_below_level(&(*b)->izq, height - 1);
	der = binary_search_tree_node_count_below_level(&(*b)->der, height - 1);
	return der + izq;
}

int binary_search_tree_node_count_within_level(const binary_search_tree_t *b,
					       int height)
{
	int izq, der;
	if (*b == NULL) {
		return 0;
	}
	if (height > 0) {
		return 1;
	}
	izq = binary_search_tree_node_count_below_level(&(*b)->izq, height - 1);
	der = binary_search_tree_node_count_below_level(&(*b)->der, height - 1);
	return der + izq;
}

int binary_search_tree_node_count_from_level_inclusive(
	const binary_search_tree_t *b, int height)
{
	int izq, der;
	if (*b == NULL) {
		return 0;
	}
	if (height >= 0) {
		return 1;
	}
	izq = binary_search_tree_node_count_below_level(&(*b)->izq, height - 1);
	der = binary_search_tree_node_count_below_level(&(*b)->der, height - 1);
	return der + izq;
}

int binary_search_tree_node_count_beyond_level(const binary_search_tree_t *b,
					       int height)
{
	int izq, der;
	if (*b == NULL) {
		return 0;
	}
	if (height <= 0) {
		return 1;
	}
	izq = binary_search_tree_node_count_below_level(&(*b)->izq, height - 1);
	der = binary_search_tree_node_count_below_level(&(*b)->der, height - 1);
	return der + izq;
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

int binary_search_tree_delete_node(binary_search_tree_t *b, void *buff,
				   const unsigned tam, cmp_t cmp)
{
	if (!(b = buscar_nodo(b, buff, cmp))) {
		return TREE_ERR_NOT_FOUND;
	}
	memcpy(buff, (*b)->dato, MIN((*b)->tam, tam));
	return eliminar_raiz(b);
}

int binary_search_tree_search_r(const binary_search_tree_t *bree,
				const void *key, void *buf, size_t n,
				cmp_t cmp);
