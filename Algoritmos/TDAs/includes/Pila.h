#ifndef PILA_H_INCLUDED
#define PILA_H_INCLUDED
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERR 2
#define TAM_PILA 100

typedef struct {
  char Vector[TAM_PILA];
  unsigned tope;
} t_Pila;

// no puede fallar, el tope ya esta definido
// en compilacion
void crearPila(t_Pila *p);

int pilaLlena(const t_Pila *p, unsigned tam);

int pilaVacia(const t_Pila *p);

// Destruye la pila
void vaciarPila(t_Pila *p);

// Se llama a pilaLlena, aunque, es posible no llamarla y que lo
// haga el mono con navaja (cliente)
// tam es el tamaño de d
int apilar(t_Pila *p, const void *d, unsigned tam);

// tam es el tamaño del buffer
// Se hace un memcpy desde la pila al buffer
int desapilar(t_Pila *p, void *buffer, unsigned tam);

// tam es el tamaño del buffer
int verTope(const t_Pila *p, void *buffer, unsigned tam);

#endif // PILA_H_INCLUDED
