#ifndef CIRCULAR_STACK_H_INCLUDED
#define CIRCULAR_STACK_H_INCLUDED

/* Pila Circular - Implementación con Linked List circular (nodos en heap) */

/* Definiciones de errores (específicas para circular) */
typedef enum {
  STACK_SUCCESS = 0,
  STACK_ERR_EMPTY = -1,       /* ERR_PILA_VACIA */
  STACK_ERR_CAPACITY = -2,    /* No aplica - dinámico */
  STACK_ERR_MEM_FULL = -3,    /* ERR_MEM_LLENA */
  STACK_ERR_INVAL = -4        /* Parámetros inválidos */
} stack_status_t;

/* =============================================================================
   Definición de estructura circular (Linked List circular)

   En esta implementación, el último nodo apunta al primero para crear un ciclo.
   Esto permite reutilizar nodos liberados en lugar de alojarlos todo en heap
   lineal.
   =============================================================================
 */

typedef struct stack_node {
  void *dato;             /* Dato almacenado (puntero genérico) */
  unsigned tamDato;       /* Tamaño del dato en bytes */
  struct stack_node *sig; /* Puntero al siguiente nodo */
} stack_node_t;

/* Pila circular: puntero a primer nodo + puntero que indica el fin del ciclo */
typedef struct {
  stack_node_t *head; /* Primer nodo (tope) de la pila */
  stack_node_t *last; /* Último nodo (cierra el ciclo hacia head) */
} stack_t;

/* =============================================================================
   Declaraciones de funciones
   =============================================================================
 */

/**
 * @brief Crea una pila circular y la inicializa (no hay nodos)
 * @param p Puntero a la estructura de pila
 */
void stack_init(stack_t *p);

/**
 * @brief Añade un elemento al tope de la pila (push)
 * @param p Pila donde agregar el elemento
 * @param d Dato a insertar (puntero a datos)
 * @param tamDato Tamaño del dato en bytes
 * @return OK si se realizó con éxito, ERR_MEM_LLENA si no hay memoria
 * disponible
 */
int stack_push(stack_t *p, const void *d, unsigned tamDato);

/**
 * @brief Comproba si la pila está llena (generalmente no aplica para circular)
 * @param p Pila a verificar
 * @param tam Tamaño del elemento que se intentaría insertar
 * @return FALSE siempre (la pila circular solo falla por falta de memoria)
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
 * @brief Comproba si la pila está vacía (no hay nodos)
 * @param p Pila a verificar
 * @return FALSE si tiene elementos, TRUE si está vacía
 */
int stack_is_empty(stack_t *p);

/**
 * @brief Elimina todos los elementos de la pila (clear) y libera memoria
 * @param p Pila a vaciar
 */
void stack_clear(stack_t *p);

#endif // CIRCULAR_STACK_H_INCLUDED
