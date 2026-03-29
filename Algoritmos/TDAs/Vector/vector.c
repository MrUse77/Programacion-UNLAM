#include "include/vector.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static bool redimensionarVector(Vector *v, float factor)
{
	size_t nCap = v->cap * factor;
	void *nV = realloc(v->v, nCap);
	if (!nV) {
		return false;
	}
	v->cap = nCap;
	v->v = nV;
	return true;
}

int crearVector(Vector *vec, size_t tam)
{
	vec->cap = 10;
	vec->ce = 0;
	vec->te = tam;
	vec->v = malloc(vec->cap * tam);
	if (!vec->v) {
		return ERR_ESPACIO;
	}
	return OK;
}

int vectorInsertar(Vector *v, void *elem)
{
	if (v->ce == v->cap)
		if (!redimensionarVector(v, FACTOR_INC))
			return SIN_MEM;

	void *posIns = v->v + v->te * v->ce;
	memcpy(posIns, elem, v->te);
	v->ce++;
	return OK;
}
bool vectorLleno(Vector *v)
{
	return v->cap == v->ce;
}
int vectorVaciar(Vector *v)
{
	v->ce = 0;
	v->cap = TAM_INIT;
	v->v = realloc(v->v, TAM_INIT * v->cap);
	return OK;
}

bool vectorVacio(Vector *v)
{
	return v->ce == 0;
}
int vectorOrdInsertar(Vector *v, void *elem, Cmp cmp)
{
	if (v->ce >= v->cap) {
		bool redim = redimensionarVector(v, FACTOR_INC);
		if (!redim) {
			return ERR_MEM;
		}
	}
	void *posIns = v->v;
	void *ult = v->v + v->ce * v->te;
	// Buscar la posición correcta: avanza mientras el elemento actual sea MENOR
	while (posIns <= ult && cmp(elem, posIns) > 0) {
		posIns += v->te;
	}
	// Mover elementos hacia la derecha para hacer espacio
	memmove(posIns + v->te, posIns, ult - posIns);
	// Insertar el nuevo elemento
	memcpy(posIns, elem, v->te);
	v->ce++;
	return OK;
}
int crearVectorDeArchivoTxtConCant(Vector *v, char *file, size_t tam,
				   FmtIns format)
{
	FILE *f = fopen(file, "rt");
	if (!f) {
		return ERR;
	}
	v->v = malloc(tam);
	if (!v->v) {
		return ERR_MEM;
	}
	int cod = OK;
	cod = crearVector(v, tam);
	char *elem = malloc(tam);
	fgets(elem, tam, f);
	v->cap = atoi(elem);
	v->te = tam;
	while (fgets(elem, v->te, f)) {
		void *elemFmt = malloc(tam);
		format(elem, elemFmt);
		cod = vectorInsertarAlInicio(v, elemFmt);
		free(elemFmt);
		if (cod != OK) {
			free(elem);
			return cod;
		}
	}
	free(elem);
	return cod;
}
void vectorVerElemDePos(Vector *v, int pos, Prnt Prnt)
{
	void *elem = v->v + pos * v->te;
	Prnt(elem);
}
int vectorEliminar(Vector *v, void *elem, Cmp cmp)
{
	void *posEl = v->v;
	void *ult = v->v + v->ce * v->te;
	while (posEl < ult && cmp(posEl, elem) != 0) {
		posEl += v->te;
	}
	if (posEl < ult) {
		memmove(posEl, posEl + v->te, ult - posEl - v->te);
		v->ce--;
	}
	return OK;
}
int vectorDestruir(Vector *v)
{
	free(v->v);
	v->te = 0;
	v->cap = 0;
	v->ce = 0;
	return OK;
}
int vectorInsertarAlInicio(Vector *v, void *elem)
{
	int cod;
	if (v->ce == v->cap) {
		cod = redimensionarVector(v, FACTOR_INC);
		if (cod != OK)
			return SIN_MEM;
	}
	//void *ult = v->v + (v->ce) * v->te;

	//Otra forma:
	if (v->ce != 0) {
		memmove(v->v + v->te, v->v, v->te * v->ce);
	}
	/*for (; ult > v->vec; ult -= v->te) {
		memcpy(ult, ult - v->te, v->te);
	}*/

	memcpy(v->v, elem, v->te);
	v->ce++;
	return OK;
}
void vectorMostrar(Vector *v, Prnt print)
{
	void *init = v->v;
	void *ult = v->v + v->cap * v->te;
	for (; init < ult; init += v->te) {
		print(init);
	}
}
