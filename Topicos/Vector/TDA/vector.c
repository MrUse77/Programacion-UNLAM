#include <stddef.h>
#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

//Sin static podria declarar un prototipo en otro .c y usarla sin problemas
static void ordenarBurbujeo(Vector *v);
static void ordenarSeleccion(Vector *v);
static void ordenarSeleccion2(Vector *v);
static void ordenarInsercion(Vector *v);
static void *buscarMenor(void *init, void *fin, size_t tamElem);
static void *buscarMayor(void *init, void *fin, size_t tamElem);
static void intercambiar(void *a, void *b, size_t tamElem);
static bool redimensionarVector(Vector *v, float factor);

//Creacion
int vectorCrear(Vector *v, size_t tam)
{
	v->cantElem = 0;
	v->vec = malloc(TAM_INIT * tam);
	if (v->vec == NULL) {
		v->cap = 0;
		return ERR_MEM;
	}
	v->cap = TAM_INIT;
	v->tamElem = tam;
	return OK;
}
int vectorCrearDeArchivoBIN(Vector *v, size_t tam, FILE *f)
{
	vectorCrear(v, tam);
	void *elem = malloc(tam); //Solucion al VLA
	if (!elem)
		return false;
	while (fread(elem, tam, 1, f) == 1) {
		vectorInsertar(v, elem);
	}
	free(elem);
	return true;
}

//Insercion
int vectorInsertar(Vector *v, void *elem)
{
	if (v->cantElem == v->cap)
		if (!redimensionarVector(v, FACT_INC))
			return SIN_MEM;

	void *posIns = v->vec + v->tamElem * v->cantElem;
	memcpy(posIns, elem, v->tamElem);
	v->cantElem++;
	return OK;
}
int vectorOrdInsertar(Vector *v, void *elem, Cmp cmp)
{
	size_t ce = v->cantElem;
	if (ce == v->cap)
		if (redimensionarVector(v, FACT_INC))
			return SIN_MEM;

	void *posIns = v->vec;
	void *ult = v->vec + (v->cantElem - 1) * v->tamElem;
	while (posIns <= ult && cmp(posIns, elem) > 0)
		posIns += v->tamElem;

	for (void *j = ult; j >= posIns; j -= v->tamElem)
		memcpy(j + v->tamElem, j, v->tamElem);

	memcpy(posIns, elem, v->tamElem);
	v->cantElem++;
	return OK;
}
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
int vectorInsertarDeArchivoBIN(Vector *v, FILE *f)
{
	int cod = OK;
	void *elem = malloc(v->tamElem);
	if (!elem)
		return ERR_MEM;
	while (fread(elem, v->tamElem, 1, f) == 1) {
		cod = vectorInsertar(v, elem);
		if (cod != OK) {
			free(elem);
			return cod;
		}
	}
	free(elem);
	return cod;
}
int vectorInsertarDeArchivoTXT(Vector *v, FILE *f, FmtInsert formatear)
{
	int cod = OK;
	char *elem = malloc(v->tamElem);
	if (!elem)
		return ERR_MEM;
	while (fgets(elem, v->tamElem, f)) {
		void *elemFmt = malloc(v->tamElem);
		formatear(elem, elemFmt);
		cod = vectorInsertar(v, elemFmt);

		if (cod != OK) {
			free(elem);
			return cod;
		}
		free(elemFmt);
	}
	free(elem);
	return cod;
}

//Busqueda
void *vectorBuscar(Vector *v, int elem)
{
	void *ult = v->vec + (v->cantElem - 1) * v->tamElem;
	void *i = v->vec;
	bool encontrado = false;
	while (i <= ult && !encontrado) {
		if (*(int *)i == elem) {
			encontrado = true;
		}
		i += v->tamElem;
	}
	return i;
}
void *vectorOrdBuscar(const Vector *v, void *elem, Cmp cmp)
{
	void *li = v->vec;
	void *ls = v->vec + (v->cantElem - 1);
	void *m;
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
	return encontrado ? (v->vec + pos) : NULL;
}

//Eliminacion
bool vectorEliminarDePosicion(Vector *v, size_t pos)
{
	if (pos < 0 || pos > v->cantElem) {
		return false;
	}
	int *i = v->vec + (pos + 1) * v->tamElem;
	/*int *ult = v->vec + v->cantElem - 1;
	//puede no tener inicializacion
	for (; i <= ult; i++) {
		memcpy(i - 1, i, v->tamElem);
	}*/
	//Otra forma:
	memmove(v->vec + pos, i, v->tamElem * v->cantElem);
	v->cantElem--;
	if (((float)v->cantElem / v->cap) <= FACT_OCUP) {
		redimensionarVector(v, FACT_DEC);
	}
	return true;
}
bool vectorOrdEliminar(Vector *v, void *elem, Cmp cmp)
{
	int pos = (int *)vectorOrdBuscar(v, elem, cmp) - (int *)v->vec;
	if (!pos) {
		return false;
	}
	vectorEliminarDePosicion(v, pos);
	return true;
}
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

//Ordenamiento
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
	case SELECCION2:
		ordenarSeleccion2(vector);
		break;
	}
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
				intercambiar(j, j + v->tamElem, v->tamElem);
			}
		}
	}
	free(aux);
}
static void ordenarSeleccion(Vector *v)
{
	void *ult = v->vec + (v->cantElem - 1) * v->tamElem;
	void *i, *min;
	for (i = v->vec; i < ult; i += v->tamElem) {
		min = buscarMenor(i, ult, v->tamElem);
		if (min != i) {
			intercambiar(i, min, v->tamElem);
		}
	}
}
static void ordenarSeleccion2(Vector *v)
{
	void *ult = v->vec + (v->cantElem - 1) * v->tamElem;
	void *i, *min, *max;
	for (i = v->vec; i < ult; i += v->tamElem, ult -= v->tamElem) {
		min = buscarMenor(i, ult, v->tamElem);
		max = buscarMayor(i, ult, v->tamElem);
		if (min != i) {
			intercambiar(i, min, v->tamElem);
		}
		if (max != ult) {
			intercambiar(ult, max, v->tamElem);
		}
	}
}
static void ordenarInsercion(Vector *v)
{
	void *ult = v->vec + (v->cantElem - 1) * v->tamElem;
	void *i, *j;
	int elemAIns;
	for (i = v->vec + 1; i < ult; i += v->tamElem) {
		elemAIns = *(int *)i;
		j = i - v->tamElem;
		while (j >= v->vec && *(int *)j > elemAIns) {
			*(int *)(j + v->tamElem) = *(int *)j;
			j -= v->tamElem;
		}
		*(int *)(j + v->tamElem) = elemAIns;
	}
}

//varios
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
void vectorMostrar(const Vector *v, Fmt fmt)
{
	for (void *i = v->vec; i < v->vec + v->cantElem * v->tamElem;
	     i += v->tamElem) {
		fmt(i);
	}
}
static bool redimensionarVector(Vector *v, float factor)
{
	size_t nuevaCap = v->cap * factor;
	void *nuevoVec = realloc(v->vec, nuevaCap * v->tamElem);
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
static void *buscarMayor(void *init, void *fin, size_t tamElem)
{
	void *j;
	for (j = init + tamElem; j <= fin; j += tamElem) {
		if (*(int *)j > *(int *)init) {
			init = j;
		}
	}
	return j;
}
static void *buscarMenor(void *init, void *fin, size_t tamElem)
{
	void *j;
	for (j = init + tamElem; j <= fin; j += tamElem) {
		if (*(int *)j < *(int *)init) {
			init = j;
		}
	}
	return j;
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
