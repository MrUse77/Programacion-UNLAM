#ifndef STATIC_QUEUE_H_INCLUDED
#define STATIC_QUEUE_H_INCLUDED
#include "types.h"

/* Cola Estática - Implementación con array de tamaño fijo */

/* Definiciones de estado para retornos de funciones */
typedef enum {
  QUEUE_SUCCESS = 0,
  QUEUE_ERR_EMPTY = -1,    /* ERR_COLA_VACIA */
  QUEUE_ERR_FULL = -2,     /* ERR_COLA_LLENA */
  QUEUE_ERR_MEM_FULL = -3, /* ERR_MEM_LLENA (no aplica aquí) */
  QUEUE_ERR_INVAL = -4     /* Parámetros inválidos */
} queue_status_t;

/* =============================================================================
   Definición de estructura estática
   =============================================================================
 */

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

/* =============================================================================
   Declaraciones de funciones
   =============================================================================
 */

/**
 * @brief Crea una cola estática y la inicializa (cola vacía)
 * @param c Puntero a la estructura de cola
 */
void queue_create(queue_t *c);

/**
 * @brief Añade un elemento al final de la cola (enqueue)
 * @param c Cola donde agregar el elemento
 * @param d Dato a insertar (puntero a datos)
 * @param tamDato Tamaño del dato
 * @return OK si se realizó con éxito, ERR_COLA_LLENA si la cola está llena
 */
int queue_push(queue_t *c, const void *d, unsigned tamDato);

/**
 * @brief Comproba si la cola está llena
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

#endif // STATIC_QUEUE_H_INCLUDED
