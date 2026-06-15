/**
 * @file ListaSimple.h
 * @brief Interfaz de una lista simplemente enlazada.
 */

#ifndef LISTA_DOBLE_H
#define LISTA_DOBLE_H

#include <stdbool.h>
#include <types.h>

/* Definiciones de estado para retornos de funciones */
typedef enum {
  LIST_SUCCESS = 0,
  LIST_ERR_EMPTY = -1,     /* ERR_LISTA_VACIA */
  LIST_ERR_NOT_FOUND = -2, /* ERR_LISTA_NO_ENCONTRADO */
  LIST_ERR_MEM_FULL = -3,  /* ERR_MEM_LLENA */
  LIST_ERR_INVAL = -4      /* Parámetros inválidos */
} list_status_t;

#define OK LIST_SUCCESS

/**
 * @def MERGE
 * @brief Identificador de ordenamiento Merge Sort.
 */
#define MERGE 10

/**
 * @def RADIX
 * @brief Identificador de ordenamiento Radix Sort.
 */
#define RADIX 20

/**
 * @def QUICK
 * @brief Identificador de ordenamiento Quick Sort.
 */
#define QUICK 30

/**
 * @struct list_node_t
 * @brief Nodo de la lista doblemente enlazada.
 */
typedef struct list_node_t {
  void *dato;
  unsigned tam;
  struct list_node_t *sig, *ant;
} list_node_t;

/**
 * @brief Tipo de dato que representa la lista.
 */
typedef list_node_t *list_t;

/**
 * @brief Crea una lista vacia.
 * @param l Puntero a la lista.
 */
void list_create(list_t *l);

/**
 * @brief Indica si la lista esta vacia.
 * @param l Puntero a la lista.
 * @return int TRUE si esta vacia, FALSE en caso contrario.
 */
int list_is_empty(const list_t *l);

/**
 * @brief Indica si no hay mas espacio disponible en la lista.
 * @param l Puntero a la lista.
 * @param tam Tamano del elemento.
 * @return int FALSE si hay espacio disponible.
 */
int list_is_full(const list_t *l, const unsigned tam);

/**
 * @brief Vacia la lista completa.
 * @param l Puntero a la lista.
 */
int list_clear(list_t *l);

/**
 * @brief Inserta un elemento al principio de la lista.
 * @param l Puntero a la lista.
 * @param d Puntero al dato a insertar.
 * @param tam Tamano del dato.
 * @return int OK si se realizo exitosamente, LIST_ERR_MEM_FULL si
 * no hay espacio.
 */
int list_push_first(list_t *l, const void *d, const unsigned tam);

/**
 * @brief Obtiene y elimina el primer elemento de la lista.
 * @param l Puntero a la lista.
 * @param buff Buffer de salida.
 * @param tam Tamano del buffer.
 * @return int OK si se realizo exitosamente, LIST_ERR_EMPTY si la
 * lista esta vacia.
 */
int list_pull_first(list_t *l, void *buff, const unsigned tam);

/**
 * @brief Inserta un elemento al final de la lista.
 * @param l Puntero a la lista.
 * @param d Puntero al dato a insertar.
 * @param tam Tamano del dato.
 * @return int OK si se realizo exitosamente, LIST_ERR_MEM_FULL si
 * no hay espacio.
 */
int list_push_last(list_t *l, const void *d, const unsigned tam);

/**
 * @brief Obtiene y elimina el ultimo elemento de la lista.
 *        (Comportamiento dictado por los tests: remueve desde la cabeza /
 * FIFO).
 * @param l Puntero a la lista.
 * @param d Buffer de salida.
 * @param tam Tamano del buffer.
 * @return int OK si se realizo exitosamente, LIST_ERR_EMPTY si la
 * lista esta vacia.
 */
int list_pull_last(list_t *l, void *d, const unsigned tam);

/**
 * @brief Obtiene el ultimo elemento de la lista sin eliminarlo.
 * @param l Puntero a la lista.
 * @param buff Buffer de salida.
 * @param tam Tamano del buffer.
 * @return int OK si se realizo exitosamente, LIST_ERR_EMPTY si la
 * lista esta vacia.
 */
int list_see_last(list_t *l, void *buff, const unsigned tam);

/**
 * @brief Inserta un elemento en la lista con control de duplicados y orden.
 * @param l Puntero a la lista.
 * @param d Puntero al dato a insertar.
 * @param tam Tamano del dato.
 * @param cmp Funcion de comparacion.
 * @param accion Accion a ejecutar cuando hay duplicados (NULL = no insertar).
 * @return int OK si se realizo exitosamente, LIST_ERR_MEM_FULL si
 * no hay espacio.
 */
int list_push_orderer(list_t *l, const void *d, const unsigned tam, cmp_t cmp,
                      accion_t accion);

/**
 * @brief Obtiene el primer elemento de la lista sin eliminarlo.
 * @param l Puntero a la lista.
 * @param buff Buffer de salida.
 * @param tam Tamano del buffer.
 * @return int OK si se realizo exitosamente, LIST_ERR_EMPTY si la
 * lista esta vacia.
 */
int list_see_first(list_t *l, void *buff, const unsigned tam);

/**
 * @brief Elimina un elemento por clave.
 * @param l Puntero a la lista.
 * @param buff Buffer de entrada/salida (clave a buscar, dato eliminado).
 * @param tam Tamano del buffer.
 * @param cmp Funcion de comparacion.
 * @return int OK si se realizo exitosamente, LIST_ERR_NOT_FOUND si
 * no existe.
 */
int list_delete_by_key(list_t *l, void *buff, const unsigned tam, cmp_t cmp);

/**
 * @brief Obtiene el elemento de una posicion.
 * @param l Puntero a la lista.
 * @param buff Buffer de salida.
 * @param tam Tamano del buffer.
 * @param pos Posicion solicitada (0-indexada).
 * @return int OK si se realizo exitosamente.
 */
int list_see_in_pos(list_t *l, void *buff, const unsigned tam, int pos);

/**
 * @brief Devuelve la longitud de la lista.
 * @param l Puntero a la lista.
 * @return int Cantidad de elementos.
 */
int list_len(list_t *l);

/**
 * @brief Busca un elemento en la lista.
 * @param l Puntero a la lista.
 * @param buff Dato a buscar (entrada/salida).
 * @param tam Tamano del dato.
 * @param cmp Funcion de comparacion.
 * @return int OK si se encuentra, LIST_ERR_NOT_FOUND si no.
 */
int list_search(list_t *l, void *buff, const unsigned tam, cmp_t cmp);

/**
 * @brief Copia una lista en otra (deep copy).
 * @param l Puntero a la lista origen.
 * @param lCopia Puntero a la lista destino.
 * @return int OK si se realizo exitosamente.
 */
int list_copy(list_t *l, list_t *lCopia);

/**
 * @brief Inserta un elemento en una posicion especifica.
 * @param l Puntero a la lista.
 * @param buff Puntero al dato a insertar.
 * @param tam Tamano del dato.
 * @param pos Posicion de insercion (0-indexada).
 * @return int OK si se realizo exitosamente.
 */
int list_push_in_pos(list_t *l, const void *buff, const unsigned tam,
                     const int pos);

/**
 * @brief Inserta un elemento despues de una clave.
 * @param l Puntero a la lista.
 * @param d Puntero al dato a insertar.
 * @param tam Tamano del dato.
 * @param clave Clave de referencia.
 * @param cmp Funcion de comparacion.
 * @return int OK si se realizo exitosamente.
 */
int list_push_after_key(list_t *l, const void *d, const unsigned tam,
                        const void *clave, const cmp_t cmp);

/**
 * @brief Inserta un elemento antes de una clave.
 * @param l Puntero a la lista.
 * @param d Puntero al dato a insertar.
 * @param tam Tamano del dato.
 * @param clave Clave de referencia.
 * @param cmp Funcion de comparacion.
 * @return int OK si se realizo exitosamente.
 */
int list_push_before_key(list_t *l, const void *d, const unsigned tam,
                         const void *clave, const cmp_t cmp);

/**
 * @brief Elimina un elemento por posicion.
 * @param l Puntero a la lista.
 * @param buff Buffer de salida.
 * @param tam Tamano del buffer.
 * @param pos Posicion a eliminar (0-indexada).
 * @return int OK si se realizo exitosamente.
 */
int list_delete_pos(list_t *l, void *buff, const unsigned int tam,
                    const int pos);

/**
 * @brief Elimina el elemento anterior a una clave.
 * @param l Puntero a la lista.
 * @param buff Buffer de salida.
 * @param tam Tamano del buffer.
 * @param clave Clave de referencia.
 * @param cmp Funcion de comparacion.
 * @return int OK si se realizo exitosamente.
 */
int list_delete_before_key(list_t *l, void *buff, const unsigned int tam,
                           const void *clave, const cmp_t cmp);

/**
 * @brief Elimina el elemento posterior a una clave.
 * @param l Puntero a la lista.
 * @param buff Buffer de salida.
 * @param tam Tamano del buffer.
 * @param clave Clave de referencia.
 * @param cmp Funcion de comparacion.
 * @return int OK si se realizo exitosamente.
 */
int list_delete_after_key(list_t *l, void *buff, const unsigned int tam,
                          const void *clave, const cmp_t cmp);

/**
 * @brief Muestra todos los elementos de la lista, de derecha a izquierda.
 * @param l Puntero a la lista.
 * @param print Funcion de impresion.
 */
int list_show_lr(const list_t *l, const prnt_t print);

/**
 * @brief Muestra todos los elementos de la lista, de izquierda a derecha.
 * @param l Puntero a la lista.
 * @param print Funcion de impresion.
 */
int list_show_rl(const list_t *l, const prnt_t print);

/**
 * @brief Muestra todos los elementos de la lista en orden inverso.
 * @param l Puntero a la lista.
 * @param mostrar Funcion de impresion.
 */
void list_show_invert(list_t *l, const prnt_t mostrar);

/**
 * @brief Invierte el orden de los elementos de la lista.
 * @param l Puntero a la lista.
 * @return int OK si se realizo exitosamente.
 */
int list_invert(list_t *l);

/**
 * @brief Concatena la segunda lista al final de la primera.
 * @param l1 Lista destino.
 * @param l2 Lista a concatenar (queda vacia).
 * @return int OK si se realizo exitosamente.
 */
int list_concat(list_t *l1, list_t *l2);

/**
 * @brief Cuenta cuantas veces aparece un dato en la lista.
 * @param l Puntero a la lista.
 * @param d Dato a contar.
 * @param cmp Funcion de comparacion.
 * @return int Cantidad de apariciones.
 */
int list_count_appear(list_t *l, const void *d, const cmp_t cmp);

/**
 * @brief Indica si la lista contiene un dato.
 * @param l Puntero a la lista.
 * @param d Dato a buscar.
 * @param cmp Funcion de comparacion.
 * @return int OK si lo contiene, LIST_ERR_NOT_FOUND si no.
 */
int list_contain(list_t *l, const void *d, const cmp_t cmp);

/**
 * @brief Ordena la lista con el algoritmo indicado.
 * @param l Puntero a la lista.
 * @param ordenamiento Tipo de ordenamiento (MERGE, RADIX, QUICK).
 * @param cmp Funcion de comparacion.
 * @return int LIST_ERR_EMPTY si lista vacia, OK en caso contrario.
 */
int list_order(list_t *l, const int ordenamiento, const cmp_t cmp);

/**
 * @brief Actualiza un elemento por posicion mediante una accion.
 * @param l Puntero a la lista.
 * @param d Dato usado por la accion.
 * @param pos Posicion a actualizar.
 * @param accion Funcion de actualizacion.
 * @return int OK si se realizo exitosamente.
 */
int list_update_by_pos(list_t *l, const void *d, int pos, accion_t accion);

/**
 * @brief Actualiza un elemento por clave mediante una accion.
 * @param l Puntero a la lista.
 * @param d Dato usado por la accion.
 * @param cmp Funcion de comparacion.
 * @param accion Funcion de actualizacion.
 * @return int OK si se realizo exitosamente.
 */
int list_update_by_key(list_t *l, const void *d, cmp_t cmp, accion_t accion);

/**
 * @brief Busca la posicion de un elemento en la lista.
 * @param l Puntero a la lista.
 * @param d Dato a buscar.
 * @param cmp Funcion de comparacion.
 * @return int Posicion (0-indexada) si se encuentra, LIST_ERR_NOT_FOUND si no.
 */
int list_search_pos(list_t *l, const void *d, cmp_t cmp);

#endif
