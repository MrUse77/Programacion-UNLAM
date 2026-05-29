#include <Arbol.h>
#include <string.h>
#include <stdio.h>

void crearArbol(t_Arbol *a)
{
	*a = NULL;
}

void recorrerArbolInOrder(t_Arbol *a, t_Prnt mostrar)
{
	if (*a == NULL || a == NULL) {
		return;
	}
	recorrerArbolInOrder(&(*a)->izq, mostrar);
	mostrar((*a)->dato);
	recorrerArbolInOrder(&(*a)->der, mostrar);
}
void recorrerArbolPreOrder(t_Arbol *a, t_Prnt mostrar)
{
	if (*a == NULL) {
		return;
	}
	mostrar((*a)->dato);
	recorrerArbolPreOrder(&(*a)->izq, mostrar);
	recorrerArbolPreOrder(&(*a)->der, mostrar);
}
void recorrerArbolPostOrder(t_Arbol *a, t_Prnt mostrar)
{
	if (*a == NULL) {
		return;
	}
	recorrerArbolPostOrder(&(*a)->izq, mostrar);
	recorrerArbolPostOrder(&(*a)->der, mostrar);
	mostrar((*a)->dato);
}
int insertarEnArbol(t_Arbol *a, const void *d, const size_t tam,
		    const t_Cmp cmp)
{
	if (*a != NULL) {
		int comp = cmp((*a)->dato, d);
		if (comp < 0) {
			return insertarEnArbol(&((*a)->der), d, tam, cmp);
		} else if (comp > 0) {
			return insertarEnArbol(&((*a)->izq), d, tam, cmp);
		} else if (comp == 0) {
			return MISMO_VALOR;
		}
	}
	t_Nodo *aux = (t_Nodo *)malloc(sizeof(t_Nodo));
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

int insertarEnArbolIterativo(t_Arbol *a, const void *d, const size_t tam,
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
	*a = (t_Nodo *)malloc(sizeof(t_Nodo));
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

void contarNodosDeArbol(t_Arbol *a)
{
}
