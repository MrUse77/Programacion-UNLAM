#ifndef DYNAMIC_QUEUE_H_INCLUDED
#define DYNAMIC_QUEUE_H_INCLUDED
#include "types.h"

/* Cola Dinámica - Implementación con Linked List (nodos en heap) */

/* Definiciones de estado para retornos de funciones */
typedef enum {
  QUEUE_SUCCESS = 0,
  QUEUE_ERR_EMPTY = -1, /* ERR_COLA_VACIA */
  QUEUE_ERR_FULL = -2,
  QUEUE_ERR_INVAL = -3 /* Parámetros inválidos */
} queue_status_t;

/* =============================================================================
   Definición de estructura dinámica (Linked List con head)

   Nota: En una cola tradicional, el 'head' es el primer elemento (donde se
   dequeuea) y el 'tail' es donde se enquean nuevos elementos.
   =============================================================================
 */

typedef struct queue_node {
  void *dato;             /* Puntero al dato almacenado */
  unsigned tamDato;       /* Tamaño del dato */
  struct queue_node *sig; /* Siguiente nodo (para enqueue) */
} queue_node_t;

/* Cola dinámica: puntero a primer nodo (head, donde se dequeuea) */
typedef struct {
  queue_node_t *prim, *ult;
} queue_t;

/* =============================================================================
   Declaraciones de funciones
   =============================================================================
 */

/**
 * @brief Crea una cola dinámica y la inicializa (no hay nodos)
 * @param c Puntero a la estructura de cola (puntero al head)
 */
void queue_create(queue_t *c);

/**
 * @brief Añade un elemento al final de la cola (enqueue)
 * @param c Cola donde agregar el elemento
 * @param d Dato a insertar (puntero a datos)
 * @param tamDato Tamaño del dato en bytes
 * @return OK si se realizó con éxito, ERR_MEM_LLENA si no hay memoria
 * disponible
 */
int queue_push(queue_t *c, const void *d, unsigned tamDato);

/**
 * @brief Comproba si la cola está llena (generalmente no aplica para dinámica)
 * @param c Cola a verificar
 * @param tam Tamaño del elemento que se intentaría insertar
 * @return FALSE siempre (la cola dinámica solo falla por falta de memoria)
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
 * @brief Comproba si la cola está vacía (no hay nodos)
 * @param c Cola a verificar
 * @return FALSE si tiene elementos, TRUE si está vacía
 */
int queue_is_empty(queue_t *c);

/**
 * @brief Elimina todos los elementos de la cola (clear) y libera memoria
 * @param c Cola a vaciar
 */
void queue_clear(queue_t *c);

#endif // DYNAMIC_QUEUE_H_INCLUDED
