#ifndef STATIC_STACK_H_INCLUDED
#define STATIC_STACK_H_INCLUDED

/* Pila Estática - Implementación con array de tamaño fijo */

/* Definiciones de errores (específicas para estática) */
typedef enum {
  STACK_SUCCESS = 0,
  STACK_ERR_EMPTY = -1,      /* ERR_PILA_VACIA */
  STACK_ERR_CAPACITY = -2,   /* ERR_PILA_LLENA */
  STACK_ERR_MEM_FULL = -3,   /* ERR_MEM_LLENA (no aplica aquí) */
  STACK_ERR_INVAL = -4       /* Parámetros inválidos */
} stack_status_t;

/* =============================================================================
   Definición de estructura estática
   =============================================================================
 */

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

/* =============================================================================
   Declaraciones de funciones
   =============================================================================
 */

/**
 * @brief Crea una pila estática y la inicializa (tamaño = 0)
 * @param p Puntero a la estructura de pila
 */
void stack_init(stack_t *p);

/**
 * @brief Añade un elemento al tope de la pila (push)
 * @param p Pila donde agregar el elemento
 * @param d Dato a insertar (puntero a datos)
 * @param tamDato Tamaño del dato
 * @return OK si se realizó con éxito, ERR_PILA_LLENA si la pila está llena
 */
int stack_push(stack_t *p, const void *d, unsigned tamDato);

/**
 * @brief Comproba si la pila está llena
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

#endif // STATIC_STACK_H_INCLUDED
