#ifndef CIRCULAR_QUEUE_H_INCLUDED
#define CIRCULAR_QUEUE_H_INCLUDED

/* Cola Circular - Implementación con Linked List circular (nodos en heap) */

#include "types.h"

/* Definiciones de estado para retornos de funciones */
typedef enum {
  QUEUE_SUCCESS = 0,
  QUEUE_ERR_EMPTY = -1,    /* ERR_COLA_VACIA */
  QUEUE_ERR_FULL = -2,     /* No aplica - dinámico */
  QUEUE_ERR_MEM_FULL = -3, /* ERR_MEM_LLENA */
  QUEUE_ERR_INVAL = -4     /* Parámetros inválidos */
} queue_status_t;

/* =============================================================================
   Definición de estructura circular (Linked List circular)

   En esta implementación, el último nodo apunta al primero para crear un ciclo.
   head = primer elemento (donde se dequeuea), last = último elemento (cierra
   hacia head)
   =============================================================================
 */

typedef struct queue_node {
  void *dato;             /* Puntero al dato almacenado */
  unsigned tamDato;       /* Tamaño del dato */
  struct queue_node *sig; /* Siguiente nodo en el ciclo */
} queue_node_t;

/* Cola circular: puntero a primer nodo (head) + puntero que indica el fin del
 * ciclo */
typedef queue_node_t *queue_t;

/* =============================================================================
   Declaraciones de funciones
   =============================================================================
 */

/**
 * @brief Crea una cola circular y la inicializa (no hay nodos)
 * @param c Puntero a la estructura de cola
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
 * @brief Comproba si la cola está llena (generalmente no aplica para circular)
 * @param c Cola a verificar
 * @param tam Tamaño del elemento que se intentaría insertar
 * @return FALSE siempre (la cola circular solo falla por falta de memoria)
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

#endif // CIRCULAR_QUEUE_H_INCLUDED
