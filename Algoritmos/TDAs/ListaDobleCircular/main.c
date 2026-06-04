
#include <ListaDoble.h>
#include <Comun.h>
#include <stdlib.h>
#include <string.h>

void crearLista(double_list_t *l)
{
	*l = NULL;
}

int vaciarLista(double_list_t *l)
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

int listaVacia(const double_list_t *l)
{
	return *l == NULL;
}

int listaLlena(const double_list_t *l, unsigned tam)
{
	double_list_node_t *nue =
		(double_list_node_t *)malloc(sizeof(double_list_node_t));
	void *aux = malloc(tam);
	free(aux);
	free(nue);
	return aux == NULL || nue == NULL;
}

int insertarAlFinal(double_list_t *l, const void *d, unsigned tam)
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

int insertarAlComienzo(double_list_t *l, const void *d, unsigned tam)
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

int insertarOrdenadoEnLista(double_list_t *l, const void *d, const unsigned tam,
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

int mostrarDeDerAIzq(const double_list_t *l, t_Prnt print)
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

int mostrarDeIzqADer(const double_list_t *l, t_Prnt print)
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

void ordenarLista(double_list_t *l, const t_Cmp cmp)
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

int eliminarPorClave(double_list_t *l, void *buff, const unsigned int tam,
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
