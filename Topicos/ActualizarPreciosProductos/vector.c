#include <stddef.h>
#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

typedef void *(*busqueda)(void *init, void *fin, size_t tamElem, Cmp cmp);
typedef bool (*funcCmp)(const void *a, const void *b, Cmp cmp);
//Sin static podria declarar un prototipo en otro .c y usarla sin problemas
static void ordenarBurbujeo(Vector *v, Cmp cmp, int ord);
static void ordenarSeleccion(Vector *v, Cmp cmp, busqueda b);
static void ordenarSeleccion2(Vector *v, Cmp cmp, busqueda a, busqueda b);
static void ordenarInsercion(Vector *v, Cmp cmpFunc);
static void *buscarMenor(void *init, void *fin, size_t tamElem, Cmp cmp);
static void *buscarMayor(void *init, void *fin, size_t tamElem, Cmp cmp);
static bool cmpAsc(const void *a, const void *b, Cmp cmp);
static bool cmpDesc(const void *a, const void *b, Cmp cmp);

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
int vectorCrearDeArchivoBIN(Vector *v, size_t tamElem, char *nombArch)
{
	FILE *f = fopen(nombArch, "rb");
	if (!f) {
		perror("Error al abrir el archivo");
		v->vec = NULL;
		v->cantElem = 0;
		v->tamElem = 0;
		v->cap = 0;
		return ERR_ARCH;
	}
	fseek(f, 0, SEEK_END);
	v->vec = malloc(v->tamElem);
	if (!v->vec) {
		return ERR_MEM;
	}
	v->tamElem = tamElem;
	size_t tam = ftell(f);
	fseek(f, 0, SEEK_SET);
	fread(v->vec, tam, 1, f);
	v->cantElem = tam / v->tamElem;
	v->cap = tam;
	return OK;
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
	int cod;
	if (ce == v->cap) {
		cod = redimensionarVector(v, FACT_INC);
		if (cod != OK)
			return SIN_MEM;
	}

	void *posIns = v->vec;
	void *ult = v->vec + (v->cantElem - 1) * v->tamElem;
	while (posIns <= ult && cmp(posIns, elem) > 0)
		posIns += v->tamElem;

	memmove(posIns + v->tamElem, posIns, (ult - posIns) + v->tamElem);
	/*Otra forma:
	for (void *j = ult; j >= posIns; j -= v->tamElem)
		memcpy(j + v->tamElem, j, v->tamElem);
	*/
	memcpy(posIns, elem, v->tamElem);
	v->cantElem++;
	return OK;
}
int vectorInsertarAlInicio(Vector *v, void *elem)
{
	int cod;
	if (v->cantElem == v->cap) {
		cod = redimensionarVector(v, FACT_INC);
		if (cod != OK)
			return SIN_MEM;
	}
	void *ult = v->vec + (v->cantElem) * v->tamElem;

	//Otra forma:
	memmove(v->vec, v->vec + 1, v->tamElem * v->cantElem);

	/*for (; ult > v->vec; ult -= v->tamElem) {
		memcpy(ult, ult - v->tamElem, v->tamElem);
	}*/

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
//La cabecera de, por ejemplo un csv, debera leerse antes de llamar a esta funcion
int vectorInsertarDeArchivoTXT(Vector *v, FILE *f, FmtInsert formatear,
			       int count)
{
	if (count <= 0) {
		count = -1;
	}
	int cod = OK;
	char *elem = malloc(v->tamElem);
	if (!elem)
		return ERR_MEM;
	while (fgets(elem, v->tamElem, f) && (count-- != 0)) {
		void *elemFmt = malloc(v->tamElem);
		formatear(elem, elemFmt);
		cod = vectorInsertarAlInicio(v, elemFmt);

		if (cod != OK) {
			free(elem);
			return cod;
		}
		free(elemFmt);
	}
	free(elem);
	return cod;
}
//Funcion para guardar datos del vector, la cabecera del archivo, si es que lo desea, debera introducirse antes
int vectorGuardarAArchivoTXT(Vector *v, FILE *f, FmtWrite formatear)
{
	int cod = OK;
	void *ult = v->vec + (v->cantElem - 1) * v->tamElem;
	void *i = v->vec;
	char *elem = malloc(sizeof(v->tamElem));
	for (; i < ult; i += v->tamElem) {
		formatear(f, i);
	}
	free(elem);
	return cod;
}
int vectorGuardarAArchivoBIN(Vector *v, char *nombArch)
{
	FILE *f = fopen(nombArch, "wb");
	if (!f) {
		perror("Error al abrir el archivo");
		return ERR_ARCH;
	}
	fwrite(v->vec, v->tamElem, v->cantElem, f);
	return OK;
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
		m = li + ((ls - li) / v->tamElem / 2) * v->tamElem;
		if (cmp(elem, m) == 0) {
			encontrado = true;
			memcpy(elem, m, v->tamElem);
			pos = (m - v->vec) * v->tamElem;
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
//ord = 1 ascendente, -1 descendente
int vectorOrdenar(Vector *vector, int metodo, Cmp cmp, int ord)
{
	void *a, *b;
	if (ord != 1 && ord != -1)
		return ERR_ORD;
	if (ord == 1) {
		a = buscarMayor;
		b = buscarMenor;
	} else if (ord == -1) {
		a = buscarMenor;
		b = buscarMayor;
	}
	switch (metodo) {
	case BURBUJEO:
		ordenarBurbujeo(vector, cmp, ord);
		break;
	case SELECCION:
		ordenarSeleccion(vector, cmp, a);
		break;
	case INSERCION:
		ordenarInsercion(vector, cmp);
		break;
	case SELECCION2:
		ordenarSeleccion2(vector, cmp, a, b);
		break;
	}
	return OK;
}

static void ordenarBurbujeo(Vector *v, Cmp cmp, int ord)
{
	void *ult = v->vec + (v->cantElem - 1) * v->tamElem;
	void *i, *j;
	void *aux = malloc(v->tamElem);
	funcCmp func;
	if (ord == 1) {
		func = cmpAsc;
	} else {
		func = cmpDesc;
	}
	if (!aux)
		return;

	for (i = v->vec; i < ult; i += v->tamElem) {
		for (j = v->vec; j < ult - (i - v->vec); j += v->tamElem) {
			if (func(j, j + v->tamElem, cmp)) {
				intercambiar(j, j + v->tamElem, v->tamElem);
			}
		}
	}
	free(aux);
}
static void ordenarSeleccion(Vector *v, Cmp cmp, busqueda b)
{
	void *ult = v->vec + (v->cantElem - 1) * v->tamElem;
	void *i, *elem;
	for (i = v->vec; i < ult; i += v->tamElem) {
		elem = b(i, ult, v->tamElem, cmp);
		if (elem != i) {
			intercambiar(i, elem, v->tamElem);
		}
	}
}
static void ordenarSeleccion2(Vector *v, Cmp cmp, busqueda a, busqueda b)
{
	void *ult = v->vec + (v->cantElem - 1) * v->tamElem;
	void *i, *e1, *e2;
	for (i = v->vec; i < ult; i += v->tamElem, ult -= v->tamElem) {
		e1 = a(i, ult, v->tamElem, cmp);
		e2 = b(i, ult, v->tamElem, cmp);
		if (e2 != i) {
			intercambiar(i, e1, v->tamElem);
		}
		if (e1 != ult) {
			intercambiar(ult, e2, v->tamElem);
		}
	}
}
static void ordenarInsercion(Vector *v, Cmp cmpFunc)
{
	void *ult = v->vec + (v->cantElem - 1) * v->tamElem;
	void *j;
	void *elemAIns = malloc(v->tamElem);
	for (void *i = v->vec + v->tamElem; i < ult + v->tamElem;
	     i += v->tamElem) {
		memcpy(elemAIns, i, v->tamElem);
		j = i - v->tamElem;
		while (j >= v->vec && cmpFunc(j, elemAIns) > 0) {
			memcpy(j + v->tamElem, j, v->tamElem);
			j -= v->tamElem;
		}
		memcpy(j + v->tamElem, elemAIns, v->tamElem);
	}
	free(elemAIns);
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
static void *buscarMayor(void *init, void *fin, size_t tamElem, Cmp cmp)
{
	void *j, *elem = init;
	for (j = init + tamElem; j <= fin; j += tamElem) {
		if (cmp(j, elem) > 0) {
			elem = j;
		}
	}
	return elem;
}
static void *buscarMenor(void *init, void *fin, size_t tamElem, Cmp cmp)
{
	void *j, *elem = init;
	for (j = init + tamElem; j <= fin; j += tamElem) {
		if (cmp(j, elem) < 0) {
			elem = j;
		}
	}
	return elem;
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
static bool cmpAsc(const void *a, const void *b, Cmp cmp)
{
	return (cmp(a, b) > 0);
}
static bool cmpDesc(const void *a, const void *b, Cmp cmp)
{
	return (cmp(a, b) < 0);
}
