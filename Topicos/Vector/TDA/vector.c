#include <stddef.h>
#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

//Sin static podria declarar un prototipo en otro .c y usarla sin problemas
static void ordenarBurbujeo(Vector *v);
static void ordenarSeleccion(Vector *v);
static void ordenarInsercion(Vector *v);
static void intercambiar(void *a, void *b, size_t tamElem);
static bool redimensionarVector(Vector *v, float factor);

bool vectorCrear(Vector *v, size_t tam)
{
	v->cantElem = 0;
	v->vec = malloc(tam * TAM_INIT);
	if (v->vec == NULL) {
		v->cap = 0;
		return false;
	}
	v->cap = TAM_INIT;
	v->tamElem = tam;
	return true;
}

int vectorInsertar(Vector *v, void *elem)
{
	if (v->cantElem == v->cap) {
		redimensionarVector(v, FACT_INC);
	}
	void *posIns = v->vec + v->tamElem * v->cantElem;
	memcpy(posIns, elem, v->tamElem);
	v->cantElem++;
	return OK;
}

/*Esperar a clase de funciones para terminarlo
int vectorOrdInsertar(Vector *v, void *elem)
{
	size_t ce = v->cantElem;
	if (ce == v->cap) {
		redimensionarVector(v, FACT_INC);
	}
	void *posIns = v->vec;
	void *ult = v->vec + (v->cantElem - 1) * v->tamElem;
	while (posIns <= ult && cmp(posIns, elem) > 0) {
		posIns += v->tamElem;
	}
	for (void *j = ult; j >= posIns; j -= v->tamElem) {
		memcpy(j + v->tamElem, j, v->tamElem);
	}

	memcpy(posIns, elem, v->tamElem);
	v->cantElem++;
	return OK;
}
*/

int vectorInsertarAlInicio(Vector *v, void *elem)
{
	if (v->cantElem == v->cap)
		redimensionarVector(v, FACT_INC);

	void *ult = v->vec + (v->cantElem) * v->tamElem;

	//Otra forma: memmvoe(v->vec,v->vec+1, v-tamElem * v->cantElem);
	for (; ult > v->vec; ult -= v->tamElem) {
		memcpy(ult, ult - v->tamElem, v->tamElem);
	}

	memcpy(v->vec, elem, v->tamElem);
	v->cantElem++;
	return OK;
}

void vectorMostrar(const Vector *v)
{
	for (size_t i = 0; i < v->cantElem; i++) {
		printf("%d\n", *(int *)(v->vec + i * v->tamElem));
	}
}

/*
int vectorOrdBuscar(const Vector *v, void *elem, Cmp cmp)
{
	void *li = v->vec;
	void *ls = v->vec + (v->cantElem - 1);
	void *m;
	int comp;
	bool encontrado = false;
	int pos;

	while (li <= ls && !encontrado) {
		m = li + (ls - li) / 2;
		if (cmp(elem, m) == 0) {
			encontrado = true;
			pos = m - v->vec;
		}
		if (cmp(elem, m) > 0) {
			li = m + 1;
		} else {
			ls = m - 1;
		}
	}
	return pos;
}
*/

bool vectorEliminarDePosicion(Vector *v, size_t pos)
{
	if (pos < 0 || pos > v->cantElem) {
		return false;
	}
	int *i = v->vec + (pos + 1) * v->tamElem;
	int *ult = v->vec + v->cantElem - 1;
	//Otra forma:
	memmove(v->vec + pos, i, v->tamElem * v->cantElem);
	//puede no tener inicializacion
	/*for (; i <= ult; i++) {
		memcpy(i - 1, i, v->tamElem);
	}*/
	v->cantElem--;
	if (((float)v->cantElem / v->cap) <= FACT_OCUP) {
		redimensionarVector(v, FACT_DEC);
	}
	return true;
}

/*
bool vectorOrdEliminar(Vector *v, int elem)
{
	int pos = vectorOrdBuscar(v, elem);
	if (!pos) {
		return false;
	}
	vectorEliminarDePosicion(v, pos);
	return true;
}
*/

/*
bool vectorEliminar(Vector *v, void *elem, Cmp cmp)
{
	void *ult = v->vec + v->cantElem - 1;
	void *i = v->vec;
	bool eliminado = false;
	while (i < ult && !eliminado) {
		if (cmp(i, elem) == 0) {
			for (void *j = i + 1; j <= ult; j++) {
				memcpy(j - 1, j, v->tamElem);
				ult--;
			}
			eliminado = true;
		}
		i++;
	}
	return eliminado;
}
*/

void vectorVaciar(Vector *v)
{
	v->cantElem = 0;
	v->cap = TAM_INIT;
	v->vec = realloc(v->vec, TAM_INIT * v->tamElem);
}

void vectorDestruir(Vector *v)
{
	free(v->vec);
	v->vec = NULL;
	v->cantElem = 0;
	v->tamElem = 0;
	v->cap = 0;
}

int vectorCE(Vector *v)
{
	return v->cantElem;
}

void vectorOrdenar(Vector *vector, int metodo)
{
	switch (metodo) {
	case BURBUJEO:
		ordenarBurbujeo(vector);
		break;

	case SELECCION:
		ordenarSeleccion(vector);
		break;

	case INSERCION:
		ordenarInsercion(vector);
		break;
	}
}

static bool redimensionarVector(Vector *v, float factor)
{
	size_t nuevaCap = v->cap * factor;
	void *nuevoVec = realloc(v->vec, nuevaCap * sizeof(void *));
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

static void ordenarBurbujeo(Vector *v)
{
	void *ult = v->vec + (v->cantElem - 1) * v->tamElem;
	void *i, *j;
	void *aux = malloc(v->tamElem);
	if (!aux)
		return;
	for (i = v->vec; i < ult; i += v->tamElem) {
		for (j = v->vec; j < ult - (i - v->vec); j += v->tamElem) {
			if (*(int *)j > *(int *)(j + v->tamElem)) {
				memcpy(aux, j, v->tamElem);
				memcpy(j, j + v->tamElem, v->tamElem);
				memcpy(j + v->tamElem, aux, v->tamElem);
			}
		}
	}
	free(aux);
}

static void ordenarSeleccion(Vector *v)
{
}
static void ordenarInsercion(Vector *v)
{
}
static void intercambiar(void *a, void *b, size_t tamElem)
{
	void *aux = malloc(tamElem);
	if (!aux)
		return;
	memcpy(aux, a, tamElem);
	memcpy(a, b, tamElem);
	memcpy(b, aux, tamElem);
	free(aux);
}
