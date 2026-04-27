#ifndef PILA_H_INCLUDED
#define PILA_H_INCLUDED
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERR 2
#define TAM_PILA 200000

typedef struct {
  char Vector[TAM_PILA];
  unsigned tope;
} t_Pila;

/**
 * Crea una pila de tamaño definido
 * @param *p - puntero a pila
 */
void crearPila(t_Pila *p);

/**
 * Revisa si la pila esta llena para el elemento que se quiera insertar
 * @param *p - puntero a pila
 * @param tam - tamaño del elemento
 * @returns {int} - Si hay espacio, devuelve FALSE, si no hay, devuelve TRUE
 */
int pilaLlena(const t_Pila *p, unsigned tam);

/**
 * Revisa si la pila esta esta vacia
 * @param *p - puntero a pila
 * @returns {int} - Si hay espacio, devuelve FALSE, si no hay, devuelve TRUE
 */
int pilaVacia(const t_Pila *p);

/**
 * Vacia la pila entera
 * @param *p - puntero a pila
 */
void vaciarPila(t_Pila *p);

/**
 * Coloca un elemento en el tope de la pila
 * @param *p - puntero a pila
 * @param const *d - puntero a elemento
 * @param tam - tamaño del elemento
 * @returns {int} - Si salio bien reotrna OK, si la pila esta llena devuelve ERR
 */
int apilar(t_Pila *p, const void *d, unsigned tam);

/**
 * Se obtiene el elemento del tope de la pila y se descarta
 * @param *p - puntero a pila
 * @param *buffer - puntero a buffer para obtener el elemento
 * @param tamDato - tamaño del buffer
 * Puede ocurrir que se obtenga un tamaño menor a tamDato;
 * si el tamaño del dato es mayor que tamDato, el sobrante del dato se pierde
 * @returns {int} - Si salio bien reotrna OK, si la pila esta vacia devuelve ERR
 */
int desapilar(t_Pila *p, void *buffer, unsigned tam);

/**
 *  Se obtiene el dato del tope de la pila sin eliminarlo
 * @param *p - puntero a pila
 * @param *buffer - puntero a buffer para obtener el elemento
 * @param tamDato - tamaño del buffer
 * @returns {int} - Si salio bien reotrna OK, si la pila esta vacia devuelve ERR
 */
int verTope(const t_Pila *p, void *buffer, unsigned tam);

#endif // PILA_H_INCLUDED
