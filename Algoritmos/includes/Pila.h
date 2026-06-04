#ifndef PILA_H_INCLUDED
#define PILA_H_INCLUDED

#include "Comun.h"

/* Definiciones de errores (unificadas para las 3 implementaciones) */
#define ERR_PILA_VACIA 301 /* Pila sin elementos */
#define ERR_PILA_LLENA 302 /* Pila sin lugar para el elemento (estática) */
#define ERR_MEM_LLENA 310  /* Memoria llena (dinámica/circular) */

/* =============================================================================
   Definición de estructuras según la implementación seleccionada
   =============================================================================
   */

// #define PILA_ESTATICA
// #define PILA_DINAMICA
#define PILA_CIRCULAR

#ifdef PILA_ESTATICA
/* --- Versión Estática --- */

/**
 * @def TAM_PILA
 * @brief Define el tamaño máximo de la pila estática (por defecto: 200000)
 */
#ifndef TAM_PILA
#define TAM_PILA 200000
#endif

typedef struct {
  char Vector[TAM_PILA];
  unsigned tope;
} stack_t;

#elif defined(PILA_DINAMICA)
/* --- Versión Dinámica (Linked List con nodos en memoria heap) --- */

typedef struct stack_node {
  void *dato;
  unsigned tamDato;
  struct stack_node *sig;
} stack_node_t;

/* Pila dinámica: puntero a primer nodo (lista simple con head) */
typedef stack_node_t *stack_t;

#elif defined(PILA_CIRCULAR)
/* --- Versión Circular (Linked List circular con nodos en memoria heap) --- */

typedef struct stack_node {
  void *dato;
  unsigned tamDato;
  struct stack_node *sig;
} stack_node_t;

typedef stack_node_t *stack_t;

#else
/* --- Implementación por defecto: Dinámica --- */
/* Se define implícitamente PILA_DINAMICA si no se especifica otra opción */
#define PILA_DINAMICA
#endif

/* =============================================================================
   Declaraciones de funciones (7 firmas unificadas)
   =============================================================================
 */

/**
 * @brief Crea una pila según la implementación seleccionada
 * (estática/dinámica/circular)
 * @param p Puntero a la estructura de pila
 */
void stack_init(stack_t *p);

/**
 * @brief Añade un elemento al tope de la pila (push)
 * @param p Pila donde agregar el elemento
 * @param d Dato a insertar (puntero a datos)
 * @param tamDato Tamaño del dato
 * @return OK si se realizó con éxito, o error correspondiente:
 *         - ERR_PILA_LLENA (estática): pila llena
 *         - ERR_MEM_LLENA (dinámica/circular): no hay memoria disponible
 */
int stack_push(stack_t *p, const void *d, unsigned tamDato);

/**
 * @brief Comproba si la pila está llena (útil para implementación estática)
 * @param p Pila a verificar
 * @param tam Tamaño del elemento que se intentaría insertar
 * @return TRUE si está llena, FALSE si hay espacio
 */
int stack_is_full(stack_t *p, unsigned tam);

/**
 * @brief Obtiene el dato del tope SIN eliminarlo (peek)
 * @param p Pila de donde obtener el dato
 * @param buff Puntero al buffer donde copiar el elemento
 * @param tamDato Tamaño del buffer
 * @return OK si se realizó con éxito, ERR_PILA_VACIA si la pila está vacía
 */
int stack_see_top(stack_t *p, void *buff, unsigned tamDato);

/**
 * @brief Obtiene y elimina el dato del tope (pop)
 * @param p Pila de donde obtener y eliminar el dato
 * @param buff Puntero al buffer donde copiar el elemento eliminado
 * @param tamDato Tamaño del buffer
 * @return OK si se realizó con éxito, ERR_PILA_VACIA si la pila está vacía
 */
int stack_pull(stack_t *p, void *buff, unsigned tamDato);

/**
 * @brief Comproba si la pila está vacía
 * @param p Pila a verificar
 * @return FALSE si tiene elementos, TRUE si está vacía
 */
int stack_is_empty(stack_t *p);

/**
 * @brief Elimina todos los elementos de la pila (clear)
 * @param p Pila a vaciar
 */
void stack_clear(stack_t *p);

#endif // PILA_H_INCLUDED
