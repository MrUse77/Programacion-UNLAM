
#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED

#include "Comun.h"
/**
 * @def ERR_COLA_LLENA
 * @brief Error: cola sin lugar para el elemento (usado en Cola Estatica)
 * */
#define ERR_COLA_LLENA 311

/**
 * @def ERR_COLA_VACIA
 * @brief Error: cola sin elementos
 * */
#define ERR_COLA_VACIA 312

/* Definicion de si usa la estructura de la cola estatica o de la cola dinamica*/
#ifdef COLA_ESTATICA

/* Version Estatica */

/**
 * @def TAM_COLA
 * @brief Define el tamaño de la cola estatica
 */
#define TAM_COLA 100000

/**
 * @struct t_Cola
 * @brief Representa a una pila, solo se puede sacar, ver o poner en la primera
 * posicion
 */
typedef struct {
  char Vector[TAM_COLA];
  unsigned prim;
  unsigned ult;
  unsigned tamDisp;
} t_Cola;

#else

/* Version Dinamica */

typedef struct sNodo { /* sNodo defino la estructura*/
  void *dato;
  unsigned tamDato;
  struct sNodo *sig;
} t_Nodo;

/**
 * @struct t_Cola
 * @brief uso la definicion de t_Nodo para definir el primer y ultimo nodo de la
 * cola
 */
typedef struct {
  t_Nodo *prim, *ult;
} t_Cola;

#endif

/**
 * @brief Crea una cola de tamaño fijo o dinamica dependiendo de la version que
 * se use
 * @param p puntero a cola
 */
void crearCola(t_Cola *c);

/**
 * @brief Pone en cola un elemento (se coloca en la ultima posicion)
 * @param c puntero a cola
 * @return int Devuelve OK si se realizo exitosamente, devuelve ERR_COLA_LLENA
 * (ver Estatica) o ERR_MEM_LLENA (ver Dinamica) si no se puede colocar el
 * elemento en la cola
 */
int ponerEnCola(t_Cola *c, const void *d, unsigned tamDato);

/**
 * @brief Revisa si la cola esta llena para el elemento que se quiera insertar
 * @param c Puntero a cola
 * @param tam tamaño del elemento
 * @return int Devuelve FALSE si hay espacio y TRUE si esta llena
 */
int colaLlena(t_Cola *c, unsigned tam);

/**
 * @brief Se obtiene el primer dato de la cola sin eliminarlo
 * @param c puntero a cola
 * @param buffer puntero a buffer para obtener el elemento
 * @param tam tamaño del buffer
 * @returns int Si salio bien reotrna OK, devuelve ERR_COLA_VACIA si no hay
 * elementos en la cola
 */
int verPrimero(t_Cola *c, void *buff, unsigned tamDato);

/**
 * @brief Se obtiene el primer dato de la cola eliminandolo
 * @param c puntero a cola
 * @param buffer puntero a buffer para obtener el elemento
 * @param tam tamaño del buffer
 * @returns int Si salio bien reotrna OK, devuelve ERR_COLA_VACIA si no hay
 * elementos en la cola
 */
int sacarDeCola(t_Cola *c, void *buff, unsigned tamDato);

/**
 * @brief Revisa si la cola esta vacia
 * @param c Puntero a cola
 * @return int Devuelve FALSE si hay elementos y TRUE si esta vacia
 */
int colaVacia(t_Cola *c);

/**
 * @brief Elimina todos los elementos de la cola
 * @param c Puntero a cola
 */
void vaciarCola(t_Cola *c);

#endif // COLA_H_INCLUDED
