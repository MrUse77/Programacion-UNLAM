#ifndef ARBOL_H
#define ARBOL_H

#include "types.h"
#include <stdlib.h>

/* Definiciones de estado para retornos de funciones */
typedef enum {
  TREE_EQUAL_VALUE = 1,
  TREE_SUCCESS = 0,
  TREE_ERR_EMPTY = -1,    /* ARBOL_VACIO (árbol vacío) */
  TREE_ERR_EXISTS = -2,   /* MISMO_VALOR (valor ya existe) */
  TREE_ERR_MEM_FULL = -3, /* ERR_MEM_LLENA (memoria llena) */
  TREE_ERR_INVAL = -4,    /* Parámetros inválidos */
  TREE_ERR_NOT_FOUND = -5 /* Parámetros inválidos */
} binary_search_tree_status_t;

/**
 * @struct binary_search_tree_node_t
 * @brief Nodo del arbol binario de busqueda.
 */

/**
 * @brief Tipo de dato que representa el arbol binario de busqueda.
 */
typedef struct binary_search_tree_node_s {
  void *dato;
  unsigned tam;
  struct binary_search_tree_node_s *izq, *der;
} binary_search_tree_node_t;

typedef binary_search_tree_node_t *binary_search_tree_t;

/* Convencion del TDA: BST estandar. Los valores menores van al hijo izquierdo
 * y los mayores al derecho, de modo que el recorrido in-order devuelve los
 * valores en orden ascendente. */

/**
 * @brief Inicializa un arbol vacio.
 * @param a Puntero al arbol.
 */
void binary_search_tree_init(binary_search_tree_t *a);

/**
 * @brief Recorre el arbol en orden (in-order): izquierda, raiz, derecha.
 * @param a Puntero al arbol.
 * @param accion Funcion a ejecutar por cada nodo. Firma: accion_t, es decir
 *        (dato, contexto): primero el dato del nodo, luego el contexto.
 * @param param Puntero opaco que se pasa como contexto a la funcion.
 */
void binary_search_tree_walk_in_order(binary_search_tree_t *a, accion_t accion,
                                      void *param);

/**
 * @brief Recorre el arbol en orden post-orden (post-order): izquierda, derecha,
 * raiz.
 * @param a Puntero al arbol.
 * @param accion Funcion a ejecutar por cada nodo. Firma: accion_t, es decir
 *        (dato, contexto): primero el dato del nodo, luego el contexto.
 * @param param Puntero opaco que se pasa como contexto a la funcion.
 */
void binary_search_tree_walk_post_order(binary_search_tree_t *a,
                                        accion_t accion, void *param);

/**
 * @brief Recorre el arbol en orden pre-orden (pre-order): raiz, izquierda,
 * derecha.
 * @param a Puntero al arbol.
 * @param accion Funcion a ejecutar por cada nodo. Firma: accion_t, es decir
 *        (dato, contexto): primero el dato del nodo, luego el contexto.
 * @param param Puntero opaco que se pasa como contexto a la funcion.
 */
void binary_search_tree_walk_pre_order(binary_search_tree_t *a, accion_t accion,
                                       void *param);

/**
 * @brief Inserta un elemento en el arbol de forma recursiva con control de
 * duplicados.
 * @param a Puntero al arbol.
 * @param d Puntero al dato a insertar.
 * @param tam Tamano del dato.
 * @param cmp Funcion de comparacion.
 * @return int OK (`200`) si se inserto exitosamente, MISMO_VALOR si el valor ya
 * existe, ERR o ERR_MEM_LLENA si no hay memoria.
 */
binary_search_tree_status_t binary_search_tree_insert(binary_search_tree_t *a,
                                                      const void *d,
                                                      const size_t tam,
                                                      const cmp_t cmp);

int binary_search_tree_load_to_bin_file_sorted(binary_search_tree_t *t,
                                               const char *path,
                                               unsigned tamInfo);

/**
 * @brief Inserta un elemento en el arbol de forma iterativa con control de
 * duplicados.
 * @param a Puntero al arbol.
 * @param d Puntero al dato a insertar.
 * @param tam Tamano del dato.
 * @param cmp Funcion de comparacion.
 * @return int OK (`200`) si se inserto exitosamente, MISMO_VALOR si el
 * valor ya existe, ERR o ERR_MEM_LLENA si no hay memoria.
 */
int binary_search_tree_insert_iter(binary_search_tree_t *a, const void *d,
                                   const size_t tam, const cmp_t cmp);

/**
 * @brief Devuelve la cantidad total de nodos del arbol.
 * @param a Puntero al arbol.
 * @return int Cantidad de nodos.
 */
int binary_search_tree_node_count(const binary_search_tree_t *a);

/**
 * @brief Devuelve la cantidad de nodos hoja (sin hijos) del arbol.
 * @param a Puntero al arbol.
 * @return int Cantidad de nodos hoja.
 */
int binary_search_tree_node_leaves_count(const binary_search_tree_t *a);

/**
 * @brief Devuelve la cantidad de nodos que poseen hijo izquierdo.
 * @param a Puntero al arbol.
 * @return int Cantidad de nodos con hijo izquierdo.
 */
int binary_search_tree_node_wl_count(const binary_search_tree_t *a);

/**
 * @brief Devuelve la cantidad de nodos que poseen hijo derecho.
 * @param a Puntero al arbol.
 * @return int Cantidad de nodos con hijo derecho.
 */
int binary_search_tree_node_wr_count(const binary_search_tree_t *a);

/**
 * @brief Devuelve la cantidad de nodos cuyo dato coincide con el valor pasado
 * como referencia mediante la funcion de comparacion.
 * @param a Puntero al arbol.
 * @param cmp Funcion de comparacion.
 * @param d Dato de referencia para la busqueda.
 * @return int Cantidad de nodos que coinciden con el dato.
 */
int binary_search_tree_node_w_cond_count(const binary_search_tree_t *a,
                                         cmp_t cmp, void *d);

/**
 * @brief Devuelve la altura del arbol (cantidad de niveles).
 * @param a Puntero al arbol.
 * @return int Altura del arbol.
 */
int binary_search_tree_height(const binary_search_tree_t *a);

/**
 * @brief Devuelve la cantidad de nodos en un nivel especifico.
 * @param a Puntero al arbol.
 * @param height Nivel solicitado.
 * @return int Cantidad de nodos en el nivel indicado.
 */
int binary_search_tree_node_count_in_level(const binary_search_tree_t *a,
                                           int height);

/**
 * @brief Devuelve la cantidad de nodos por debajo de un nivel especifico
 * (excluyendo ese nivel).
 * @param a Puntero al arbol.
 * @param height Nivel de referencia.
 * @return int Cantidad de nodos por debajo del nivel indicado.
 */
int binary_search_tree_node_count_below_level(const binary_search_tree_t *a,
                                              int height);

/**
 * @brief Devuelve la cantidad de nodos hasta un nivel especifico inclusive.
 * @param a Puntero al arbol.
 * @param height Nivel de referencia.
 * @return int Cantidad de nodos hasta el nivel indicado (inclusive).
 */
int binary_search_tree_node_count_within_level(const binary_search_tree_t *a,
                                               int height);

/**
 * @brief Devuelve la cantidad de nodos desde un nivel especifico inclusive
 * hacia abajo.
 * @param a Puntero al arbol.
 * @param height Nivel de referencia.
 * @return int Cantidad de nodos desde el nivel indicado (inclusive) hasta las
 * hojas.
 */
int binary_search_tree_node_count_from_level_inclusive(
    const binary_search_tree_t *a, int height);

/**
 * @brief Devuelve la cantidad de nodos por encima de un nivel especifico.
 * @param a Puntero al arbol.
 * @param height Nivel de referencia.
 * @return int Cantidad de nodos por encima del nivel indicado.
 */
int binary_search_tree_node_count_beyond_level(const binary_search_tree_t *a,
                                               int height);

/**
 * @brief Elimina todas las hojas del arbol.
 * @param t Puntero al arbol.
 * @return int OK (`200`) si se realizo exitosamente.
 */
int binary_search_tree_cut_leaves(binary_search_tree_t *t);

/**
 * @brief Destruye el arbol liberando toda la memoria asignada a los nodos y
 * datos.
 * @param t Puntero al arbol.
 * @return int OK (`200`) si se realizo exitosamente, 0 si ya estaba vacio.
 */
binary_search_tree_status_t binary_search_tree_destroy(binary_search_tree_t *t);

/**
 * @brief Indica si el arbol esta vacio.
 * @param t Puntero al arbol.
 * @return int TRUE (`1`) si esta vacio, FALSE (`0`) en caso contrario.
 */
int binary_search_tree_is_empty(binary_search_tree_t *t);

/**
 * @brief Indica si el arbol esta balanceado: todos los niveles estan llenos
 * salvo, como maximo, el ultimo.
 * @param t Puntero al arbol.
 * @return bool_t TRUE si esta balanceado, FALSE en caso contrario.
 */
bool_t binary_search_tree_is_balanced(binary_search_tree_t *t);

/**
 * @brief Indica si el arbol es perfecto: existen todas las posiciones hasta
 * el ultimo nivel (equivale a completo hasta el nivel altura - 1).
 * @param t Puntero al arbol.
 * @return bool_t TRUE si es perfecto, FALSE en caso contrario.
 */
bool_t binary_search_tree_is_complete(binary_search_tree_t *t);

/**
 * @brief Indica si el arbol cumple la propiedad AVL: en todo nodo, la
 * diferencia de altura entre sus dos subarboles es a lo sumo 1.
 * @param t Puntero al arbol.
 * @return bool_t TRUE si es AVL, FALSE en caso contrario.
 */
bool_t binary_search_tree_is_avl(binary_search_tree_t *t);

/**
 * @brief Elimina un nodo del arbol que contenga el valor buscado.
 * @param t Puntero al arbol.
 * @param buff Valor a eliminar.
 * @param tam Tamano del valor.
 * @param cmp Funcion de comparacion.
 * @return int TREE_SUCCESS si se elimino, TREE_ERR_EMPTY si no se encontro.
 */
int binary_search_tree_delete_node(binary_search_tree_t *t, void *buff,
                                   const unsigned tam, cmp_t cmp);

int binary_search_tree_search_r(const binary_search_tree_t *tree,
                                const void *key, void *buf, size_t n,
                                cmp_t cmp);

/**
 * @brief Balancea el arbol con el algoritmo DSW (Day-Stout-Warren): lo
 * convierte en una vina (vine) y la comprime con rotaciones hasta alcanzar la
 * altura minima posible para la cantidad de nodos.
 * @param t Puntero al arbol.
 */
void binary_search_tree_balanced(binary_search_tree_t *t);

#endif // ARBOL_H
