#include <ListaDoble.h>
#include <Comun.h>
#include <stdlib.h>
#include <string.h>

static double_list_node_t *list_head(double_list_node_t *n)
{
	while (n && n->ant) {
		n = n->ant;
	}
	return n;
}

static double_list_node_t *list_tail(double_list_node_t *n)
{
	while (n && n->sig) {
		n = n->sig;
	}
	return n;
}

void list_create(double_list_t *l)
{
	*l = NULL;
}

int list_clear(double_list_t *l)
{
	double_list_node_t *act = *l;
	int cant = 0;
	if (act) {
		while (act->ant) {
			act = act->ant;
		}
		while (act) {
			double_list_node_t *aux = act->sig;
			free(act->dato);
			free(act);
			act = aux;
			cant++;
		}
		*l = NULL;
	}
	return cant;
}

int list_is_empty(const double_list_t *l)
{
	return *l == NULL;
}

int list_is_full(const double_list_t *l, const unsigned tam)
{
	double_list_node_t *nue =
		(double_list_node_t *)malloc(sizeof(double_list_node_t));
	void *aux = malloc(tam);
	free(aux);
	free(nue);
	return aux == NULL || nue == NULL;
}

int list_push_last(double_list_t *l, const void *d, const unsigned tam)
{
	double_list_node_t *act = *l, *nue;
	if (act) {
		while (act->sig) {
			act = act->sig;
		}
	}
	if ((nue = (double_list_node_t *)malloc(sizeof(double_list_node_t))) ==
		    NULL ||
	    (nue->dato = malloc(tam)) == NULL) {
		free(nue);
		return 0;
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

int list_push_first(double_list_t *l, const void *d, const unsigned tam)
{
	double_list_node_t *act = *l, *nue;
	if (act) {
		while (act->ant) {
			act = act->ant;
		}
	}
	if ((nue = (double_list_node_t *)malloc(sizeof(double_list_node_t))) ==
		    NULL ||
	    (nue->dato = malloc(tam)) == NULL) {
		free(nue);
		return 0;
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

int list_push_orderer(double_list_t *l, const void *d, const unsigned tam,
		      t_Cmp cmp, t_Acum acumulador)
{
	double_list_node_t *act = *l, *sig, *ant, *nue;
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
					return ERR_MEM_LLENA;
				}
				return ERR;
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
	if ((nue = (double_list_node_t *)malloc(sizeof(double_list_node_t))) ==
		    NULL ||
	    (nue->dato = malloc(tam)) == NULL) {
		free(nue);
		return ERR_MEM_LLENA;
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

int list_show_lr(const double_list_t *l, t_Prnt print)
{
	double_list_node_t *act = *l;
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

int list_show_rl(const double_list_t *l, t_Prnt print)
{
	double_list_node_t *act = *l;
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

void list_order(double_list_t *l, const t_Cmp cmp)
{
	double_list_node_t *act = *l, *sup = NULL, *inf = NULL;
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

int list_delete_by_key(double_list_t *l, void *buff, const unsigned int tam,
		       t_Cmp cmp)
{
	double_list_node_t *act = *l;
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
		double_list_node_t *ant = act->ant, *sig = act->sig;
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
	return ERR;
}

int list_pull_first(double_list_t *l, void *buff, const unsigned tam)
{
	double_list_node_t *act;
	if (l == NULL || *l == NULL) {
		return ERR_LISTA_VACIA;
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

int list_pull_last(double_list_t *l, void *buff, const unsigned tam)
{
	double_list_node_t *act;
	if (l == NULL || *l == NULL) {
		return ERR_LISTA_VACIA;
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

int list_see_last(double_list_t *l, void *buff, const unsigned tam)
{
	double_list_node_t *act;
	if (l == NULL || *l == NULL) {
		return ERR_LISTA_VACIA;
	}
	act = list_tail(*l);
	if (buff) {
		memcpy(buff, act->dato, MIN(tam, act->tam));
	}
	return OK;
}

int list_see_first(double_list_t *l, void *buff, const unsigned tam)
{
	double_list_node_t *act;
	if (l == NULL || *l == NULL) {
		return ERR_LISTA_VACIA;
	}
	act = list_head(*l);
	if (buff) {
		memcpy(buff, act->dato, MIN(tam, act->tam));
	}
	return OK;
}

int list_see_in_pos(double_list_t *l, void *buff, const unsigned tam, int pos)
{
	double_list_node_t *act;
	int i = 0;
	if (l == NULL || *l == NULL || pos < 0) {
		return ERR_LISTA_VACIA;
	}
	act = list_head(*l);
	while (act && i < pos) {
		act = act->sig;
		i++;
	}
	if (act == NULL) {
		return ERR_LISTA_NO_ENCONTRADO;
	}
	if (buff) {
		memcpy(buff, act->dato, MIN(tam, act->tam));
	}
	return OK;
}

int list_long(double_list_t *l)
{
	int cont = 0;
	double_list_node_t *act;
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

int list_search(double_list_t *l, void *buff, const unsigned tam, t_Cmp cmp)
{
	double_list_node_t *act;
	if (l == NULL || *l == NULL) {
		return ERR_LISTA_VACIA;
	}
	act = list_head(*l);
	while (act && cmp(buff, act->dato) != 0) {
		act = act->sig;
	}
	if (act == NULL) {
		return ERR_LISTA_NO_ENCONTRADO;
	}
	if (buff) {
		memcpy(buff, act->dato, MIN(tam, act->tam));
	}
	*l = act;
	return OK;
}

int list_copy(double_list_t *l, double_list_t *lCopia)
{
	double_list_node_t *act;
	double_list_node_t *tail = NULL;
	if (l == NULL || lCopia == NULL) {
		return ERR_LISTA_VACIA;
	}
	act = *l ? list_head(*l) : NULL;
	*lCopia = NULL;
	while (act) {
		double_list_node_t *nue = (double_list_node_t *)malloc(
			sizeof(double_list_node_t));
		if (nue == NULL) {
			return ERR_MEM_LLENA;
		}
		nue->dato = malloc(act->tam);
		if (nue->dato == NULL) {
			free(nue);
			return ERR_MEM_LLENA;
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

int list_push_in_pos(double_list_t *l, const void *buff, const unsigned tam,
		     const int pos)
{
	double_list_node_t *act;
	double_list_node_t *nue;
	int i = 0;
	if (l == NULL || pos < 0) {
		return ERR_LISTA_VACIA;
	}
	act = *l ? list_head(*l) : NULL;
	while (act && i < pos) {
		act = act->sig;
		i++;
	}
	if ((nue = (double_list_node_t *)malloc(sizeof(double_list_node_t))) ==
		    NULL ||
	    (nue->dato = malloc(tam)) == NULL) {
		free(nue);
		return ERR_MEM_LLENA;
	}
	memcpy(nue->dato, buff, tam);
	nue->tam = tam;
	if (act == NULL) {
		double_list_node_t *tail = *l ? list_tail(*l) : NULL;
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

int list_push_after_key(double_list_t *l, const void *d, const unsigned tam,
			const void *clave, const t_Cmp cmp)
{
	double_list_node_t *act;
	double_list_node_t *nue;
	if (l == NULL || *l == NULL) {
		return ERR_LISTA_VACIA;
	}
	act = list_head(*l);
	while (act && cmp(clave, act->dato) != 0) {
		act = act->sig;
	}
	if (act == NULL) {
		return ERR_LISTA_NO_ENCONTRADO;
	}
	if ((nue = (double_list_node_t *)malloc(sizeof(double_list_node_t))) ==
		    NULL ||
	    (nue->dato = malloc(tam)) == NULL) {
		free(nue);
		return ERR_MEM_LLENA;
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

int list_push_before_key(double_list_t *l, const void *d, const unsigned tam,
			 const void *clave, const t_Cmp cmp)
{
	double_list_node_t *act;
	double_list_node_t *nue;
	if (l == NULL || *l == NULL) {
		return ERR_LISTA_VACIA;
	}
	act = list_head(*l);
	while (act && cmp(clave, act->dato) != 0) {
		act = act->sig;
	}
	if (act == NULL) {
		return ERR_LISTA_NO_ENCONTRADO;
	}
	if ((nue = (double_list_node_t *)malloc(sizeof(double_list_node_t))) ==
		    NULL ||
	    (nue->dato = malloc(tam)) == NULL) {
		free(nue);
		return ERR_MEM_LLENA;
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

int list_delete_pos(double_list_t *l, void *buff, const unsigned int tam,
		    const int pos)
{
	double_list_node_t *act;
	int i = 0;
	if (l == NULL || *l == NULL || pos < 0) {
		return ERR_LISTA_VACIA;
	}
	act = list_head(*l);
	while (act && i < pos) {
		act = act->sig;
		i++;
	}
	if (act == NULL) {
		return ERR_LISTA_NO_ENCONTRADO;
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

int list_delete_before_key(double_list_t *l, void *buff, const unsigned int tam,
			   const void *clave, const t_Cmp cmp)
{
	double_list_node_t *act;
	if (l == NULL || *l == NULL) {
		return ERR_LISTA_VACIA;
	}
	act = list_head(*l);
	while (act && cmp(clave, act->dato) != 0) {
		act = act->sig;
	}
	if (act == NULL || act->ant == NULL) {
		return ERR_LISTA_NO_ENCONTRADO;
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

int list_delete_after_key(double_list_t *l, void *buff, const unsigned int tam,
			  const void *clave, const t_Cmp cmp)
{
	double_list_node_t *act;
	if (l == NULL || *l == NULL) {
		return ERR_LISTA_VACIA;
	}
	act = list_head(*l);
	while (act && cmp(clave, act->dato) != 0) {
		act = act->sig;
	}
	if (act == NULL || act->sig == NULL) {
		return ERR_LISTA_NO_ENCONTRADO;
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

int list_invert(double_list_t *l)
{
	double_list_node_t *act;
	double_list_node_t *tmp;
	if (l == NULL || *l == NULL) {
		return ERR_LISTA_VACIA;
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

int list_concat(double_list_t *l1, double_list_t *l2)
{
	double_list_node_t *tail1;
	double_list_node_t *head2;
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
	tail1 = list_tail(*l1);
	head2 = list_head(*l2);
	tail1->sig = head2;
	head2->ant = tail1;
	*l1 = list_tail(head2);
	*l2 = NULL;
	return OK;
}

int list_count_appear(double_list_t *l, const void *d, int *res,
		      const t_Cmp cmp)
{
	double_list_node_t *act;
	if (l == NULL || res == NULL) {
		return ERR_LISTA_VACIA;
	}
	*res = 0;
	act = *l ? list_head(*l) : NULL;
	while (act) {
		if (cmp(d, act->dato) == 0) {
			(*res)++;
		}
		act = act->sig;
	}
	return OK;
}

int list_contain(double_list_t *l, const void *d, const t_Cmp cmp)
{
	double_list_node_t *act;
	if (l == NULL || *l == NULL) {
		return ERR_LISTA_VACIA;
	}
	act = list_head(*l);
	while (act) {
		if (cmp(d, act->dato) == 0) {
			return OK;
		}
		act = act->sig;
	}
	return ERR_LISTA_NO_ENCONTRADO;
}

int list_update_by_pos(double_list_t *l, const void *d, int pos,
		       t_Accion accion)
{
	double_list_node_t *act;
	int i = 0;
	if (l == NULL || *l == NULL || accion == NULL || pos < 0) {
		return ERR_LISTA_VACIA;
	}
	act = list_head(*l);
	while (act && i < pos) {
		act = act->sig;
		i++;
	}
	if (act == NULL) {
		return ERR_LISTA_NO_ENCONTRADO;
	}
	accion(act->dato, d);
	*l = act;
	return OK;
}

int list_update_by_key(double_list_t *l, const void *d, t_Cmp cmp,
		       t_Accion accion)
{
	double_list_node_t *act;
	if (l == NULL || *l == NULL || accion == NULL || cmp == NULL) {
		return ERR_LISTA_VACIA;
	}
	act = list_head(*l);
	while (act && cmp(d, act->dato) != 0) {
		act = act->sig;
	}
	if (act == NULL) {
		return ERR_LISTA_NO_ENCONTRADO;
	}
	accion(act->dato, d);
	*l = act;
	return OK;
}

int list_search_pos(double_list_t *l, const void *d, t_Cmp cmp)
{
	double_list_node_t *act;
	int pos = 0;
	if (l == NULL || *l == NULL || cmp == NULL) {
		return ERR_LISTA_VACIA;
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
	return ERR_LISTA_NO_ENCONTRADO;
}
