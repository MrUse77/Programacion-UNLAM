
#include <ListaDoble.h>
#include <Comun.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Crea una lista vacia.
 * @param l Puntero a la lista.
 */
void list_create(double_list_t *l)
{
	*l = NULL;
}

/**
 * @brief Vacia la lista completa.
 * @param l Puntero a la lista.
 */
int list_clear(double_list_t *l)
{
	double_list_node_t *act = *l;
	int cant = 0;
	if (act) {
		while (act->ant) {
			act = act->ant;
		}
		while (*l) {
			double_list_node_t *aux = act->sig;
			free(aux->dato);
			free(aux);
			act = aux;
			cant++;
		}
		*l = NULL;
	}
	return cant;
}

/**
 * @brief Indica si la lista esta vacia.
 * @param l Puntero a la lista.
 * @return int TRUE si esta vacia, FALSE en caso contrario.
 */
int list_is_empty(const double_list_t *l)
{
	return *l == NULL;
}

/**
 * @brief Indica si no hay mas espacio disponible en la lista.
 * @param l Puntero a la lista.
 * @param tam Tamano del elemento.
 * @return int FALSE si hay espacio disponible, ERR_MEM_LLENA (`101`) si no.
 */
int list_is_full(const double_list_t *l, const unsigned tam)
{
	double_list_node_t *nue =
		(double_list_node_t *)malloc(sizeof(double_list_node_t));
	void *aux = malloc(tam);
	free(aux);
	free(nue);
	return aux == NULL || nue == NULL;
}

/**
 * @brief Inserta un elemento al final de la lista.
 * @param l Puntero a la lista.
 * @param d Puntero al dato a insertar.
 * @param tam Tamano del dato.
 * @return int OK (`200`) si se realizo exitosamente, ERR_MEM_LLENA (`101`) si
 * no hay espacio.
 */
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

	if (*l == NULL) {
		nue->sig = nue;
		nue->ant = nue;
		*l = nue;
	} else {
		nue->sig = *l;
		nue->ant = act;
		act->sig = nue;
		(*l)->ant = nue;
	}
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

int list_push_orederer(double_list_t *l, const void *d, const unsigned tam,
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

int list_show_rl(const double_list_t *l, const t_Prnt print)
{
	double_list_node_t *act = *l;
	int cant = 0;
	if (act) {
		print(NULL);
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

int list_show_lr(const double_list_t *l, const t_Prnt print)
{
	double_list_node_t *act = *l;
	int cant = 0;
	if (act) {
		print(NULL);
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
