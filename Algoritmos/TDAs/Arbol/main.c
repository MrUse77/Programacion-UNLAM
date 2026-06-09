#include <binary_tree.h>
#include <string.h>
#include <stdio.h>

/**
 * @def MIN
 * @brief verifica cual es el menor entre 2 elementos
 */
#define MIN(a, b) (a > b ? b : a)

void tree_init(tree_t *a)
{
	*a = NULL;
}

void tree_walk_in_order(tree_t *a, t_Prnt mostrar)
{
	if (*a == NULL || a == NULL) {
		return;
	}
	tree_walk_in_order(&(*a)->izq, mostrar);
	mostrar((*a)->dato);
	tree_walk_in_order(&(*a)->der, mostrar);
}

void tree_walk_pre_order(tree_t *a, t_Prnt mostrar)
{
	if (*a == NULL) {
		return;
	}
	mostrar((*a)->dato);
	tree_walk_pre_order(&(*a)->izq, mostrar);
	tree_walk_pre_order(&(*a)->der, mostrar);
}

void tree_walk_post_order(tree_t *a, t_Prnt mostrar)
{
	if (*a == NULL) {
		return;
	}
	tree_walk_post_order(&(*a)->izq, mostrar);
	tree_walk_post_order(&(*a)->der, mostrar);
	mostrar((*a)->dato);
}

int tree_insert(tree_t *a, const void *d, const size_t tam, const t_Cmp cmp)
{
	if (*a != NULL) {
		int comp = cmp((*a)->dato, d);
		if (comp < 0) {
			return tree_insert(&((*a)->der), d, tam, cmp);
		} else if (comp > 0) {
			return tree_insert(&((*a)->izq), d, tam, cmp);
		} else if (comp == 0) {
			return MISMO_VALOR;
		}
	}
	tree_node_t *aux = (tree_node_t *)malloc(sizeof(tree_node_t));
	if (!aux) {
		return -1;
	}
	aux->dato = malloc(tam);
	if (!aux->dato) {
		free(aux);
		return -2;
	}
	memcpy(aux->dato, d, tam);
	aux->izq = NULL;
	aux->der = NULL;
	aux->tam = tam;
	*a = aux;
	return OK;
}

int tree_insert_iter(tree_t *a, const void *d, const size_t tam,
		     const t_Cmp cmp)
{
	while (*a) {
		int comp = cmp((*a)->dato, d);
		if (comp < 0) {
			*a = (*a)->der;
		} else if (comp > 0) {
			*a = (*a)->izq;
		} else {
			return MISMO_VALOR;
		}
	}
	*a = (tree_node_t *)malloc(sizeof(tree_node_t));
	(*a)->dato = malloc(tam);
	if (!*a || !(*a)->dato) {
		free((*a));
		return -1;
	}
	memcpy((*a)->dato, d, tam);
	(*a)->izq = NULL;
	(*a)->der = NULL;
	(*a)->tam = tam;
	return OK;
}

int tree_node_count(const tree_t *a)
{
	if (*a != NULL) {
		return 1 + tree_node_count(&(*a)->der) +
		       tree_node_count(&(*a)->izq);
	}
	return 0;
}

int tree_node_leaves_count(const tree_t *a)
{
	if (*a != NULL) {
		if ((*a)->der == NULL && (*a)->izq == NULL) {
			return 1;
		}
		return tree_node_leaves_count(&(*a)->der) +
		       tree_node_leaves_count(&(*a)->izq);
	}
	return 0;
}

int tree_node_wl_count(const tree_t *a)
{
	if (*a != NULL) {
		if ((*a)->izq != NULL) {
			return 1 + tree_node_wl_count(&(*a)->der) +
			       tree_node_wl_count(&(*a)->izq);
		}
		return tree_node_wl_count(&(*a)->der) +
		       tree_node_wl_count(&(*a)->izq);
	}
	return 0;
}

int tree_node_wr_count(const tree_t *a)
{
	if (*a != NULL) {
		if ((*a)->der != NULL) {
			return 1 + tree_node_wl_count(&(*a)->der) +
			       tree_node_wl_count(&(*a)->izq);
		}
		return tree_node_wl_count(&(*a)->der) +
		       tree_node_wl_count(&(*a)->izq);
	}
	return 0;
}

int tree_node_w_cond_count(const tree_t *a, t_Cmp cmp, void *d)
{
	if (*a != NULL) {
		if (cmp((*a)->dato, d) == 0) {
			return 1 + tree_node_wl_count(&(*a)->der) +
			       tree_node_wl_count(&(*a)->izq);
		}
		return tree_node_wl_count(&(*a)->der) +
		       tree_node_wl_count(&(*a)->izq);
	}
	return 0;
}

int tree_height(const tree_t *a)
{
	if (*a != NULL) {
		int i = tree_height(&(*a)->izq), d = tree_height(&(*a)->der);
		return i >= d ? i + 1 : d + 1;
	}
	return 0;
}

int tree_node_count_in_level(const tree_t *a, int height)
{
	if (*a != NULL) {
		if (height == 0) {
			return 1;
		}
		return tree_node_count_in_level(&(*a)->der, height - 1) +
		       tree_node_count_in_level(&(*a)->izq, height - 1);
	}
	return 0;
}

int tree_node_count_below_level(const tree_t *a, int height)
{
	if (*a != NULL) {
		if (height > 0) {
			return 1;
		}
		return tree_node_count_below_level(&(*a)->der, height - 1) +
		       tree_node_count_below_level(&(*a)->izq, height - 1);
	}
	return 0;
}

int tree_node_count_beyond_level(const tree_t *a, int height)
{
	if (*a != NULL) {
		if (height < 0) {
			return 1;
		}
		return tree_node_count_beyond_level(&(*a)->der, height - 1) +
		       tree_node_count_beyond_level(&(*a)->izq, height - 1);
	}
	return 0;
}

int tree_node_count_from_level_inclusive(const tree_t *a, int height)
{
	if (*a != NULL) {
		if (height <= 0) {
			return 1;
		}
		return tree_node_count_from_level_inclusive(&(*a)->der,
							    height - 1) +
		       tree_node_count_from_level_inclusive(&(*a)->izq,
							    height - 1);
	}
	return 0;
}

int tree_cut_leaves(tree_t *t)
{
}
