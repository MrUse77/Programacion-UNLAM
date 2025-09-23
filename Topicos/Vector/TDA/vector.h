#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#define TAM_INIT 10
#define TAM 100
#define FACT_INC 2
#define FACT_DEC 0.5
#define FACT_OCUP 0.25 // Siempre menos a FACT_OCUP
#define OK 1
#define ERR_ESPACIO 2
#define ERR_REP 3
#define ERR 4
#define SIN_MEM 5

typedef struct {
	void *vec;
	size_t cantElem;
	size_t cap;
	size_t tamElem;
} Vector;

//memmove: usar cuando entre destino y origen tocan una misma direccion de memoria, usa un buffer
//memcpy: usar cuando entre destino y origen no tocan una misma direccion de memoria, no usa un buffer
//La diferencia esta en la especificacion y no en la funcionalidad, memmove es mas SEGURO en estos casos

// Primitivas
bool vectorCrear(Vector *v, size_t tam); //Reservo memoria (malloc)
int vectorOrdInsertar(Vector *v, void *elem);
// puede retornar mas de 2 cosas --> exito, fallo (sin espacio), fallo (sin repetidos), etc.
int vectorInsertar(Vector *v, void *elem);
int vectorInsertarAlInicio(Vector *v, void *elem);
int vectroBuscar(Vector *v, int elem);
bool vectorOrdEliminar(Vector *v, int elem); //retorna exito o fallo
bool vectorEliminar(Vector *v, int elem); //retorna exito o fallo
bool vectorEliminarDePosicion(Vector *v, size_t pos);
void vectorOrdenar(Vector *V);
void vectorVaciar(Vector *V);
void vectorMostrar(const Vector *v);
int vectorOrdBuscar(const Vector *v, void *elem);
void vectorDestruir(Vector *v);
int vectorCE(Vector *v);
// No Primitivas

#endif // !VECTOR_H
