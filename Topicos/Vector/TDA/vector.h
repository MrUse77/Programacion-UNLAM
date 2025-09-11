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
	int *vec;
	size_t cantElem;
	size_t cap;
} Vector;

// Primitivas
bool vectorCrear(Vector *v); //Reservo memoria (malloc)
int vectorOrdInsertar(Vector *v, int elem);
int vectorInsertar(
	Vector *v,
	int elem); // puede retornar mas de 2 cosas --> exito, fallo (sin espacio), fallo (sin repetidos), etc.
int vectorInsertarAlFinal(Vector *v, int elem);
int vectroBuscar(Vector *v, int elem);
bool vectorOrdEliminar(Vector *v, int elem); //retorna exito o fallo
bool vectorEliminar(Vector *v, int elem); //retorna exito o fallo
bool vectorEliminarDePosicion(Vector *v, size_t pos);
void vectorOrdenar(Vector *V);
void vectorVaciar(Vector *V);
void vectorMostrar(const Vector *v);
int vectorOrdBuscar(const Vector *v, int elem);
void vectorDestruir(Vector *v);
int vectorCE(Vector *v);
// No Primitivas

#endif // !VECTOR_H
