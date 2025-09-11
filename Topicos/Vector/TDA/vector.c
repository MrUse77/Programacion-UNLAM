#include <stddef.h>
#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

//Sin static podria declarar un prototipo en otro .c y usarla sin problemas
static bool redimensionarVector(Vector *v, float factor);

bool vectorCrear(Vector *v)
{
	v->cantElem = 0;
	v->vec = malloc(sizeof(int) * TAM_INIT);
	if (v->vec == NULL) {
		v->cap = 0;
		return false;
	}
	v->cap = TAM_INIT;
	return true;
}

int vectorInsertar(Vector *v, int elem)
{
	size_t ce = v->cantElem;
	if (v->cantElem == v->cap) {
		redimensionarVector(v, FACT_INC);
	}
	v->vec[ce] = elem;
	v->cantElem++;
	return OK;
}

int vectorOrdInsertar(Vector *v, int elem)
{
	size_t ce = v->cantElem;
	if (ce == v->cap) {
		redimensionarVector(v, FACT_INC);
	}
	int *i = v->vec;
	int *ult = v->vec + (v->cantElem - 1);
	while (elem > *i && i <= ult) {
		i++;
	}
	if (elem == *i) {
		return ERR_REP;
	}
	for (int *j = ult; i <= j; j--) {
		*(j + 1) = *j;
	}

	v->cantElem++;
	*i = elem;
	return OK;
}

void vectorMostrar(const Vector *v)
{
	for (int i = 0; i < v->cantElem; i++) {
		printf("%d\n", v->vec[i]);
	}
}

int vectorOrdBuscar(const Vector *v, int elem)
{
	int *li = (int *)v->vec;
	int *ls = (int *)v->vec + (v->cantElem - 1);
	int *m;
	int comp;
	bool encontrado = false;
	int pos;

	while (li <= ls && !encontrado) {
		m = li + (ls - li) / 2;
		comp = elem - *m;
		if (comp == 0) {
			encontrado = true;
			pos = m - v->vec;
		}
		if (elem > *m) {
			li = m + 1;
		} else {
			ls = m - 1;
		}
	}
	return pos;
}

bool vectorEliminarDePosicion(Vector *v, size_t pos)
{
	if (pos < 0 || pos > v->cantElem) {
		return false;
	}
	int *i = v->vec + pos + 1;
	int *ult = v->vec + v->cantElem - 1;
	//puede no tener inicializacion
	for (; i <= ult; i++) {
		*i = *(i + 1);
	}
	v->cantElem--;
	if (((float)v->cantElem / v->cap) <= FACT_OCUP) {
		redimensionarVector(v, FACT_DEC);
	}
	return true;
}

bool vectorOrdEliminar(Vector *v, int elem)
{
	int pos = vectorOrdBuscar(v, elem);
	if (!pos) {
		return false;
	}
	vectorEliminarDePosicion(v, pos);
	return true;
}

bool vectorEliminar(Vector *v, int elem)
{
	int *ult = v->vec + v->cantElem - 1;
	int *i = v->vec;
	bool eliminado = false;
	while (i < ult && !eliminado) {
		if (*i == elem) {
			for (int *j = i + 1; j <= ult; j++) {
				*(j - 1) = *j;
				ult--;
			}
			eliminado = true;
		}
		i++;
	}
	return eliminado;
}

void vectorVaciar(Vector *v)
{
	v->cantElem = 0;
	v->cap = TAM_INIT;
	v->vec = realloc(v->vec, TAM_INIT * sizeof(int));
}

void vectorDestruir(Vector *v)
{
	free(v->vec);
	v->vec = NULL;
}

int vectorCE(Vector *v)
{
	return v->cantElem;
}

static bool redimensionarVector(Vector *v, float factor)
{
	size_t nuevaCap = v->cap * factor;
	int *nuevoVec = realloc(v->vec, nuevaCap * sizeof(int));
	//static int i = 0;
	if (!nuevoVec) {
		return false;
	}
	printf("Redimension de %zu a %zu\n", v->cap, nuevaCap);
	v->cap = nuevaCap;
	v->vec = nuevoVec;
	/*
	printf("Si pibe me redimensione: %d veces. Ahora a dormir\n", i);
	i++;
	sleep(1);
	*/
	return true;
}
