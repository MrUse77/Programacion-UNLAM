/**
 * @file ListaSimple.h
 * @brief Interfaz de una lista simplemente enlazada.
 */

#ifndef LISTA_DOBLE_H
#define LISTA_DOBLE_H

#include "Comun.h"
#include <stdbool.h>

typedef int (*t_Cmp)(const void *a, const void *b);
typedef void (*t_Accion)(void *param, const void *dato);
typedef void (*t_Prnt)(const void *dato);
typedef int (*t_Acum)(void **, unsigned *, const void *, unsigned);

/**
 * @def ERR_LISTA_VACIA
 * @brief Error: lista sin elementos.
 */
#define ERR_LISTA_VACIA 301

/**
 * @def ERR_LISTA_NO_ENCONTRADO
 * @brief Error: elemento no encontrado.
 */
#define ERR_LISTA_NO_ENCONTRADO 302

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
 * @struct double_list_node
 * @brief Nodo de la lista simplemente enlazada.
 */
typedef struct double_list_node {
  void *dato;
  unsigned tam;
  struct double_list_node *sig, *ant;
} double_list_node_t;

/**
 * @brief Tipo de dato que representa la lista.
 */
typedef double_list_node_t *double_list_t;

/**
 * @brief Crea una lista vacia.
 * @param l Puntero a la lista.
 */
void list_create(double_list_t *l);

/**
 * @brief Indica si la lista esta vacia.
 * @param l Puntero a la lista.
 * @return int TRUE si esta vacia, FALSE en caso contrario.
 */
int list_is_empty(const double_list_t *l);

/**
 * @brief Indica si no hay mas espacio disponible en la lista.
 * @param l Puntero a la lista.
 * @param tam Tamano del elemento.
 * @return int FALSE si hay espacio disponible, ERR_MEM_LLENA (`101`) si no.
 */
int list_is_full(const double_list_t *l, const unsigned tam);

/**
 * @brief Vacia la lista completa.
 * @param l Puntero a la lista.
 */
int list_clear(double_list_t *l);

/**
 * @brief Inserta un elemento al principio de la lista.
 * @param l Puntero a la lista.
 * @param d Puntero al dato a insertar.
 * @param tam Tamano del dato.
 * @return int OK (`200`) si se realizo exitosamente, ERR_MEM_LLENA (`101`) si
 * no hay espacio.
 */
int list_push_first(double_list_t *l, const void *d, const unsigned tam);

/**
 * @brief Obtiene y elimina el primer elemento de la lista.
 * @param l Puntero a la lista.
 * @param buff Buffer de salida.
 * @param tam Tamano del buffer.
 * @return int OK (`200`) si se realizo exitosamente, ERR_LISTA_VACIA si la
 * lista esta vacia.
 */
int list_pull_first(double_list_t *l, void *buff, const unsigned tam);

/**
 * @brief Inserta un elemento al final de la lista.
 * @param l Puntero a la lista.
 * @param d Puntero al dato a insertar.
 * @param tam Tamano del dato.
 * @return int OK (`200`) si se realizo exitosamente, ERR_MEM_LLENA (`101`) si
 * no hay espacio.
 */
int list_push_last(double_list_t *l, const void *d, const unsigned tam);

/**
 * @brief Obtiene y elimina el ultimo elemento de la lista.
 * @param l Puntero a la lista.
 * @param d Buffer de salida.
 * @param tam Tamano del buffer.
 * @return int OK (`200`) si se realizo exitosamente, ERR_LISTA_VACIA si la
 * lista esta vacia.
 */
int list_pull_last(double_list_t *l, void *d, const unsigned tam);

/**
 * @brief Obtiene el ultimo elemento de la lista sin eliminarlo.
 * @param l Puntero a la lista.
 * @param buff Buffer de salida.
 * @param tam Tamano del buffer.
 * @return int OK (`200`) si se realizo exitosamente, ERR_LISTA_VACIA si la
 * lista esta vacia.
 */
int list_see_last(double_list_t *l, void *buff, const unsigned tam);

/**
 * @brief Inserta un elemento en la lista con control de duplicados y orden.
 * @param l Puntero a la lista.
 * @param d Puntero al dato a insertar.
 * @param tam Tamano del dato.
 * @param cmp Funcion de comparacion.
 * @param conDup Define si se aceptan duplicados.
 * @param accion Accion a ejecutar cuando hay duplicados.
 * @return int OK (`200`) si se realizo exitosamente, ERR_MEM_LLENA (`101`) si
 * no hay espacio.
 */
int list_push_orderer(double_list_t *l, const void *d, const unsigned tam,
                      t_Cmp cmp, t_Acum acumulador);

/**
 * @brief Obtiene el primer elemento de la lista sin eliminarlo.
 * @param l Puntero a la lista.
 * @param buff Buffer de salida.
 * @param tam Tamano del buffer.
 * @return int OK (`200`) si se realizo exitosamente, ERR_LISTA_VACIA si la
 * lista esta vacia.
 */
int list_see_first(double_list_t *l, void *buff, const unsigned tam);

/**
 * @brief Elimina un elemento por clave.
 * @param l Puntero a la lista.
 * @param buff Buffer de salida.
 * @param tam Tamano del buffer.
 * @param cmp Funcion de comparacion.
 * @return int OK (`200`) si se realizo exitosamente, ERR_LISTA_NO_ENCONTRADO si
 * no existe.
 */
int list_delete_by_key(double_list_t *l, void *buff, const unsigned tam,
                       t_Cmp cmp);

/**
 * @brief Obtiene el elemento de una posicion.
 * @param l Puntero a la lista.
 * @param buff Buffer de salida.
 * @param tam Tamano del buffer.
 * @param pos Posicion solicitada.
 * @return int OK (`200`) si se realizo exitosamente.
 */
int list_see_in_pos(double_list_t *l, void *buff, const unsigned tam, int pos);

/**
 * @brief Devuelve la longitud de la lista.
 * @param l Puntero a la lista.
 * @return int Cantidad de elementos.
 */
int list_long(double_list_t *l);

/**
 * @brief Busca un elemento en la lista.
 * @param l Puntero a la lista.
 * @param buff Dato a buscar.
 * @param tam Tamano del dato.
 * @param cmp Funcion de comparacion.
 * @return int OK (`200`) si se encuentra, ERR_LISTA_NO_ENCONTRADO si no.
 */
int list_search(double_list_t *l, void *buff, const unsigned tam, t_Cmp cmp);

/**
 * @brief Copia una lista en otra.
 * @param l Puntero a la lista origen.
 * @param lCopia Puntero a la lista destino.
 * @return int OK (`200`) si se realizo exitosamente.
 */
int list_copy(double_list_t *l, double_list_t *lCopia);

/**
 * @brief Inserta un elemento en una posicion especifica.
 * @param l Puntero a la lista.
 * @param buff Puntero al dato a insertar.
 * @param tam Tamano del dato.
 * @param pos Posicion de insercion.
 * @return int OK (`200`) si se realizo exitosamente.
 */
int list_push_in_pos(double_list_t *l, const void *buff, const unsigned tam,
                     const int pos);

/**
 * @brief Inserta un elemento despues de una clave.
 * @param l Puntero a la lista.
 * @param d Puntero al dato a insertar.
 * @param tam Tamano del dato.
 * @param clave Clave de referencia.
 * @param cmp Funcion de comparacion.
 * @return int OK (`200`) si se realizo exitosamente.
 */
int list_push_after_key(double_list_t *l, const void *d, const unsigned tam,
                        const void *clave, const t_Cmp cmp);

/**
 * @brief Inserta un elemento antes de una clave.
 * @param l Puntero a la lista.
 * @param d Puntero al dato a insertar.
 * @param tam Tamano del dato.
 * @param clave Clave de referencia.
 * @param cmp Funcion de comparacion.
 * @return int OK (`200`) si se realizo exitosamente.
 */
int list_push_before_key(double_list_t *l, const void *d, const unsigned tam,
                         const void *clave, const t_Cmp cmp);

/**
 * @brief Elimina un elemento por posicion.
 * @param l Puntero a la lista.
 * @param buff Buffer de salida.
 * @param tam Tamano del buffer.
 * @param pos Posicion a eliminar.
 * @return int OK (`200`) si se realizo exitosamente.
 */
int list_delete_pos(double_list_t *l, void *buff, const unsigned int tam,
                    const int pos);

/**
 * @brief Elimina el elemento anterior a una clave.
 * @param l Puntero a la lista.
 * @param buff Buffer de salida.
 * @param tam Tamano del buffer.
 * @param clave Clave de referencia.
 * @param cmp Funcion de comparacion.
 * @return int OK (`200`) si se realizo exitosamente.
 */
int list_delete_before_key(double_list_t *l, void *buff, const unsigned int tam,
                           const void *clave, const t_Cmp cmp);

/**
 * @brief Elimina el elemento posterior a una clave.
 * @param l Puntero a la lista.
 * @param buff Buffer de salida.
 * @param tam Tamano del buffer.
 * @param clave Clave de referencia.
 * @param cmp Funcion de comparacion.
 * @return int OK (`200`) si se realizo exitosamente.
 */
int list_delete_after_key(double_list_t *l, void *buff, const unsigned int tam,
                          const void *clave, const t_Cmp cmp);

/**
 * @brief Muestra todos los elementos de la lista. de derecha a izq
 * @param l Puntero a la lista.
 * @param prnt Funcion de impresion.
 */
int list_show_lr(const double_list_t *l, const t_Prnt print);

/**
 * @brief Muestra todos los elementos de la lista. de izq a der
 * @param l Puntero a la lista.
 * @param prnt Funcion de impresion.
 */
int list_show_rl(const double_list_t *l, const t_Prnt print);

/**
 * @brief Invierte el orden de los elementos de la lista.
 * @param l Puntero a la lista.
 * @return int OK (`200`) si se realizo exitosamente.
 */
int list_invert(double_list_t *l);

/**
 * @brief Concatena la segunda lista al final de la primera.
 * @param l1 Lista destino.
 * @param l2 Lista a concatenar.
 * @return int OK (`200`) si se realizo exitosamente.
 */
int list_concat(double_list_t *l1, double_list_t *l2);

/**
 * @brief Cuenta cuantas veces aparece un dato en la lista.
 * @param l Puntero a la lista.
 * @param d Dato a contar.
 * @param res Puntero al resultado.
 * @param cmp Funcion de comparacion.
 * @return int OK (`200`) si se realizo exitosamente.
 */
int list_count_appear(double_list_t *l, const void *d, int *res,
                      const t_Cmp cmp);

/**
 * @brief Indica si la lista contiene un dato.
 * @param l Puntero a la lista.
 * @param d Dato a buscar.
 * @param cmp Funcion de comparacion.
 * @return int OK (`200`) si lo contiene, ERR_LISTA_NO_ENCONTRADO si no.
 */
int list_contain(double_list_t *l, const void *d, const t_Cmp cmp);

/**
 * @brief Ordena la lista con el algoritmo indicado.
 * @param l Puntero a la lista.
 * @param ordenamiento Tipo de ordenamiento a usar.
 * @param cmp Funcion de comparacion.
 * @return void
 */
void list_order(double_list_t *l, const t_Cmp cmp);

int list_update_by_pos(double_list_t *l, const void *d, int pos,
                       t_Accion accion);

int list_update_by_key(double_list_t *l, const void *d, t_Cmp, t_Accion accion);

int list_search_pos(double_list_t *l, const void *d, t_Cmp cmp);

#endif
