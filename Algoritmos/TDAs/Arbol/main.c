#include <binary_tree.h>
#include <string.h>
#include <stdio.h>
#include <types.h>

/**
 * @def MIN
 * @brief verifica cual es el menor entre 2 elementos
 */

static tree_node_t **tree_node_max_key(tree_t *t)
{
	if (!*t) {
		return NULL;
	}
	while ((*t)->der) {
		*t = (*t)->der;
	}
	return t;
}

static tree_node_t **tree_node_min(tree_t *t)
{
	if (!*t) {
		return NULL;
	}
	while ((*t)->izq) {
		*t = (*t)->izq;
	}
	return t;
}

static unsigned read_bin_file(void **d, void *f, unsigned pos, void *params)
{
	unsigned tam = *(int *)params;
	*d = malloc(tam);
	if (!*d) {
		return TREE_ERR_MEM_FULL;
	}
	fseek((FILE *)f, pos * tam, SEEK_SET);
	return fread(*d, tam, 1, (FILE *)f);
}

static int tree_load_order(tree_t *t, void *d, read_t read, int li, int ls,
			   void *params)
{
	int med = (li + ls) / 2, code;
	if (li > ls) {
		return EXIT_SUCCESS;
	}
	*t = (tree_node_t *)malloc(sizeof(tree_node_t));
	if (!(*t) || !((*t)->tam = read(&(*t)->dato, d, med, params))) {
		free(*t);
		return TREE_ERR_MEM_FULL;
	}
	(*t)->izq = (*t)->der = NULL;
	if ((code = tree_load_order(&(*t)->izq, d, read, li, med - 1, params) !=
		    EXIT_SUCCESS)) {
		return code;
	}
	return tree_load_order(&(*t)->der, d, read, med + 1, ls, params);
}

void tree_init(tree_t *a)
{
	*a = NULL;
}

int tree_load_to_bin_file_sorted(tree_t *t, const char *path, unsigned tamInfo)
{
	if (*t) {
		return TREE_ERR_INVAL;
	}
	FILE *f = fopen(path, "rb");
	int cant = 0, code = EXIT_SUCCESS;
	if (!f) {
		return TREE_ERR_INVAL;
	}
	fseek(f, 0, SEEK_END);
	cant = ftell(f) / tamInfo;
	code = tree_load_order(t, f, read_bin_file, 0, cant - 1, &tamInfo);
	fclose(f);
	return code;
}

int tree_load_to_bin_file_sorted_abstract(tree_t *t, const char *path,
					  unsigned tamInfo, accion_t accion)
{
	if (*t) {
		return TREE_ERR_INVAL;
	}
	FILE *f = fopen(path, "rb");
	int cant = 0, code = EXIT_SUCCESS;
	if (!f) {
		return TREE_ERR_INVAL;
	}
	fseek(f, 0, SEEK_END);
	cant = ftell(f) / tamInfo;
	code = tree_load_order(t, f, read_bin_file, 0, cant - 1, &tamInfo);
	fclose(f);
	return code;
}

void tree_walk_in_order(tree_t *a, accion_t accion, void *param)
{
	if (*a == NULL || a == NULL) {
		return;
	}
	tree_walk_in_order(&(*a)->izq, accion, param);
	accion(param, (*a)->dato);
	tree_walk_in_order(&(*a)->der, accion, param);
}

void tree_walk_pre_order(tree_t *a, accion_t accion, void *param)
{
	if (*a == NULL) {
		return;
	}
	accion(param, (*a)->dato);
	tree_walk_pre_order(&(*a)->izq, accion, param);
	tree_walk_pre_order(&(*a)->der, accion, param);
}

void tree_walk_post_order(tree_t *a, accion_t accion, void *param)
{
	if (*a == NULL) {
		return;
	}
	tree_walk_post_order(&(*a)->izq, accion, param);
	tree_walk_post_order(&(*a)->der, accion, param);
	accion(param, (*a)->dato);
}

int tree_insert(tree_t *a, const void *d, const size_t tam, const cmp_t cmp)
{
	if (*a != NULL) {
		int comp = cmp((*a)->dato, d);
		if (comp < 0) {
			return tree_insert(&((*a)->der), d, tam, cmp);
		} else if (comp > 0) {
			return tree_insert(&((*a)->izq), d, tam, cmp);
		} else if (comp == 0) {
			return TREE_EQUAL_VALUE;
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
	return TREE_SUCCESS;
}

int tree_insert_iter(tree_t *a, const void *d, const size_t tam,
		     const cmp_t cmp)
{
	tree_node_t **current = a;
	while (*current) {
		int comp = cmp((*current)->dato, d);
		if (comp < 0) {
			current = &(*current)->der;
		} else if (comp > 0) {
			current = &(*current)->izq;
		} else {
			return TREE_EQUAL_VALUE;
		}
	}
	*current = (tree_node_t *)malloc(sizeof(tree_node_t));
	if (!*current) {
		return -1;
	}
	(*current)->dato = malloc(tam);
	if (!(*current)->dato) {
		free(*current);
		*current = NULL;
		return -2;
	}
	memcpy((*current)->dato, d, tam);
	(*current)->izq = NULL;
	(*current)->der = NULL;
	(*current)->tam = tam;
	return TREE_SUCCESS;
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
			return 1 + tree_node_wr_count(&(*a)->der) +
			       tree_node_wr_count(&(*a)->izq);
		}
		return tree_node_wr_count(&(*a)->der) +
		       tree_node_wr_count(&(*a)->izq);
	}
	return 0;
}

int tree_node_w_cond_count(const tree_t *a, cmp_t cmp, void *d)
{
	if (*a != NULL) {
		if (cmp((*a)->dato, d) == 0) {
			return 1 + tree_node_w_cond_count(&(*a)->der, cmp, d) +
			       tree_node_w_cond_count(&(*a)->izq, cmp, d);
		}
		return tree_node_w_cond_count(&(*a)->der, cmp, d) +
		       tree_node_w_cond_count(&(*a)->izq, cmp, d);
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
		if (height < 0) {
			return 1 +
			       tree_node_count_below_level(&(*a)->der,
							   height - 1) +
			       tree_node_count_below_level(&(*a)->izq,
							   height - 1);
		}
		return tree_node_count_below_level(&(*a)->der, height - 1) +
		       tree_node_count_below_level(&(*a)->izq, height - 1);
	}
	return 0;
}

int tree_node_count_beyond_level(const tree_t *a, int height)
{
	if (*a != NULL) {
		if (height >= 0) {
			return 1 +
			       tree_node_count_beyond_level(&(*a)->der,
							    height - 1) +
			       tree_node_count_beyond_level(&(*a)->izq,
							    height - 1);
		}
		return 0;
	}
	return 0;
}

int tree_node_count_from_level_inclusive(const tree_t *a, int height)
{
	if (*a != NULL) {
		if (height <= 0) {
			return 1 +
			       tree_node_count_from_level_inclusive(
				       &(*a)->der, height - 1) +
			       tree_node_count_from_level_inclusive(&(*a)->izq,
								    height - 1);
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
	if (*t == NULL) {
		return 0;
	}
	if ((*t)->izq == NULL && (*t)->der == NULL) {
		free((*t)->dato);
		free(*t);
		*t = NULL;
		return 1;
	}
	return tree_cut_leaves(&(*t)->izq) + tree_cut_leaves(&(*t)->der);
}

int tree_destroy(tree_t *tree)
{
	if (!tree || !*tree) {
		return 0;
	}

	tree_destroy(&(*tree)->izq);
	tree_destroy(&(*tree)->der);
	free((*tree)->dato);
	free((*tree));
	return TREE_SUCCESS;
}

static int tree_delete_node_raiz(tree_t *t)
{
	tree_node_t **remp, *elim;
	if (*t == NULL) {
		return TREE_ERR_EMPTY;
	}
	free((*t)->dato);
	if ((*t)->izq == NULL && (*t)->der == NULL) {
		free(*t);
		*t = NULL;
	} else {
		remp = tree_height(&(*t)->izq) > tree_height(&(*t)->der) ?
			       tree_node_max_key(&(*t)->izq) :
			       tree_node_min(&(*t)->der);

		elim = *remp;

		(*t)->dato = elim->dato; //Cambia puntero del dato
		(*t)->tam = elim->tam; //Cambia valor entero
		*remp = elim->izq ? elim->izq : elim->der;
		free(elim);
	}
	return TREE_SUCCESS;
}

tree_node_t **tree_search(tree_t *t, void *buff, const unsigned tam, cmp_t cmp)
{
	if (*t != NULL) {
		int comp = cmp((*t)->dato, buff);
		if (comp < 0) {
			return tree_search(&((*t)->der), buff, tam, cmp);
		} else if (comp > 0) {
			return tree_search(&((*t)->izq), buff, tam, cmp);
		} else if (comp == 0) {
			return t;
		}
	}
	return NULL;
}

int tree_delete_node(tree_t *t, void *buff, const unsigned tam, cmp_t cmp)
{
	if ((t = tree_search(t, buff, tam, cmp)) == NULL) {
		return TREE_ERR_EMPTY;
	}
	memcpy(buff, (*t)->dato, MIN((*t)->tam, tam));
	return tree_delete_node_raiz(t);
}
int binarysearchtree_search_r(const tree_t *tree, const void *key, void *buf,
			      size_t n, cmp_t cmp)
{
	int compar;

	if (*tree) {
		compar = cmp((*tree)->dato, key);

		if (compar > 0) {
			return binarysearchtree_search_r(&(*tree)->izq, key,
							 buf, n, cmp);
		} else if (compar < 0) {
			return binarysearchtree_search_r(&(*tree)->der, key,
							 buf, n, cmp);
		} else {
			memcpy(buf, (*tree)->dato, MIN((*tree)->tam, n));
			return EXIT_SUCCESS;
		}
	}

	return EXIT_FAILURE;
}
