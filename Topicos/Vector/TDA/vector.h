#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

//Tamaños
#define TAM_INIT 10
#define TAM 100

//Factores de redimension
#define FACT_INC 2
#define FACT_DEC 0.5
#define FACT_OCUP 0.25 // Siempre menos a FACT_OCUP

//Retornos
#define OK 1
#define ERR_ESPACIO 2
#define ERR_REP 3
#define ERR 4
#define SIN_MEM 5
#define ERR_MEM 6

//Tipos de ordenamiento
#define BURBUJEO 1
#define INSERCION 2
#define SELECCION 3
#define SELECCION2 4

typedef struct {
	void *vec;
	size_t cantElem;
	size_t cap;
	size_t tamElem;
} Vector;
typedef int (*Cmp)(const void *, const void *); //Puntero a funcion
typedef void (*Fmt)(const void *);
typedef void (*FmtInsert)(char *, void *);
typedef void (*FmtRead)(char *, void *);
typedef void (*FmtWrite)(FILE *, void *);

//memmove: usar cuando entre destino y origen tocan una misma direccion de memoria, usa un buffer
//memcpy: usar cuando entre destino y origen no tocan una misma direccion de memoria, no usa un buffer
//La diferencia esta en la especificacion y no en la funcionalidad, memmove es mas SEGURO en estos casos

/*** Primitivas ***/
// Creacion
int vectorCrear(Vector *v, size_t tam);
int vectorCrearDeArchivoBIN(Vector *v, size_t tam, FILE *f);

//Insercion
int vectorOrdInsertar(Vector *v, void *elem, Cmp cmp);
int vectorInsertar(Vector *v, void *elem);
int vectorInsertarAlInicio(Vector *v, void *elem);
int vectorInsertarDeArchivoBIN(Vector *v, FILE *f);
int vectorInsertarDeArchivoTXT(Vector *v, FILE *f, FmtInsert insertar,
			       int count); //Probable csv
int vectorGuardarAArchivoTXT(Vector *v, FILE *f, FmtWrite formatear);

//Busqueda
void *vectorBuscar(Vector *v, int elem);
void *vectorOrdBuscar(const Vector *v, void *elem, Cmp cmp);

//Eliminacion
bool vectorOrdEliminar(Vector *v, void *elem, Cmp cmp);
bool vectorEliminar(Vector *v, void *elem, Cmp cmp);
bool vectorEliminarDePosicion(Vector *v, size_t pos);

//Ordenamiento
void vectorOrdenar(Vector *V, int metodo);

//varios
void vectorVaciar(Vector *V);
void vectorMostrar(const Vector *v, Fmt fmt);
void vectorDestruir(Vector *v);
int vectorCE(Vector *v);

/*** No Primitivas ***/

#endif // !VECTOR_H
