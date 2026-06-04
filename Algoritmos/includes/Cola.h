#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED

#include "Comun.h"

/* Definiciones de errores (unificadas para las 3 implementaciones) */
#define ERR_COLA_LLENA 311 /* Cola sin lugar para el elemento (estática) */
#define ERR_COLA_VACIA 312 /* Cola sin elementos */
#define ERR_MEM_LLENA 310  /* Memoria llena (dinámica) */

/* =============================================================================
   Definición de estructuras según la implementación seleccionada
   =============================================================================
   */

// #define COLA_ESTATICA
// #define COLA_DINAMICA
#define COLA_CIRCULAR

#ifdef COLA_ESTATICA
/* --- Versión Estática --- */

/**
 * @def TAM_COLA
 * @brief Define el tamaño máximo de la cola estática (por defecto: 100000)
 */
#ifndef TAM_COLA
#define TAM_COLA 100000
#endif

typedef struct {
  void *Vector[TAM_COLA]; /* Vector de punteros a datos */
  unsigned prim;          /* Índice del primer elemento */
  unsigned ult;           /* Índice del último elemento */
  unsigned tamDisp;       /* Espacio disponible */
} queue_t;

#elif defined(COLA_DINAMICA)
/* --- Versión Dinámica (Linked List con nodos en memoria heap) --- */

typedef struct queue_node {
  void *dato;             /* Puntero al dato almacenado */
  unsigned tamDato;       /* Tamaño del dato */
  struct queue_node *sig; /* Siguiente nodo */
} queue_node_t;

/* Cola dinámica: puntero a primer nodo (lista simple con head) */
typedef queue_node_t *queue_t;

#elif defined(COLA_CIRCULAR)
/* --- Versión Circular (Linked List circular con nodos en memoria heap) --- */

typedef struct queue_node {
  void *dato;             /* Puntero al dato almacenado */
  unsigned tamDato;       /* Tamaño del dato */
  struct queue_node *sig; /* Siguiente nodo */
} queue_node_t;

typedef queue_node_t *queue_t;

#else
/* --- Implementación por defecto: Dinámica --- */
/* Se define implícitamente COLA_DINAMICA si no se especifica otra opción */
#define COLA_DINAMICA
#endif

/* =============================================================================
   Declaraciones de funciones (7 firmas unificadas)
   =============================================================================
 */

/**
 * @brief Crea una cola según la implementación seleccionada
 * (estática/dinámica/circular)
 * @param p Puntero a la estructura de cola
 */
void queue_create(queue_t *c);

/**
 * @brief Añade un elemento al final de la cola (enqueue)
 * @param c Cola donde agregar el elemento
 * @param d Dato a insertar (puntero a datos)
 * @param tamDato Tamaño del dato
 * @return OK si se realizó con éxito, o error correspondiente:
 *         - ERR_COLA_LLENA (estática): cola llena
 *         - ERR_MEM_LLENA (dinámica/circular): no hay memoria disponible
 */
int queue_push(queue_t *c, const void *d, unsigned tamDato);

/**
 * @brief Comproba si la cola está llena (útil para implementación estática)
 * @param c Cola a verificar
 * @param tam Tamaño del elemento que se intentaría insertar
 * @return TRUE si está llena, FALSE si hay espacio
 */
int queue_is_full(queue_t *c, unsigned tam);

/**
 * @brief Obtiene el primer dato de la cola SIN eliminarlo (peek)
 * @param c Cola de donde obtener el dato
 * @param buff Puntero al buffer donde copiar el elemento
 * @param tamDato Tamaño del buffer
 * @return OK si se realizó con éxito, ERR_COLA_VACIA si la cola está vacía
 */
int queue_see_first(queue_t *c, void *buff, unsigned tamDato);

/**
 * @brief Obtiene y elimina el primer dato de la cola (dequeue)
 * @param c Cola de donde obtener y eliminar el dato
 * @param buff Puntero al buffer donde copiar el elemento eliminado
 * @param tamDato Tamaño del buffer
 * @return OK si se realizó con éxito, ERR_COLA_VACIA si la cola está vacía
 */
int queue_pull(queue_t *c, void *buff, unsigned tamDato);

/**
 * @brief Comproba si la cola está vacía
 * @param c Cola a verificar
 * @return FALSE si tiene elementos, TRUE si está vacía
 */
int queue_is_empty(queue_t *c);

/**
 * @brief Elimina todos los elementos de la cola (clear)
 * @param c Cola a vaciar
 */
void queue_clear(queue_t *c);

#endif // COLA_H_INCLUDED
