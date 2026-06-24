/**
 * @file Arbol.h
 * @brief Interfaz de un arbol binario de busqueda (BST).
 */

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
  TREE_ERR_INVAL = -4     /* Parámetros inválidos */
} tree_status_t;

/**
 * @struct tree_node_s
 * @brief Nodo del arbol binario de busqueda.
 */
typedef struct tree_node_s {
  void *dato;
  unsigned tam;
  struct tree_node_s *izq, *der;
} tree_node_t;

/**
 * @brief Tipo de dato que representa el arbol binario de busqueda.
 */
typedef tree_node_t *tree_t;

/**
 * @brief Funcion de impresion para recorrer y mostrar los nodos del arbol.
 */
typedef void (*prnt_t)(const void *dato);

/**
 * @brief Funcion de comparacion entre dos elementos.
 * Devuelve negativo si menor es menor que mayor, cero si son iguales, positivo
 * si menor es mayor.
 */
typedef int (*cmp_t)(const void *menor, const void *mayor);

/**
 * @brief Inicializa un arbol vacio.
 * @param a Puntero al arbol.
 */
void tree_init(tree_t *a);

/**
 * @brief Recorre el arbol en orden (in-order): izquierda, raiz, derecha.
 * @param a Puntero al arbol.
 * @param accion Funcion a ejecutar por cada nodo (recibe contexto + dato).
 * @param param Puntero opaco que se pasa como contexto a la funcion.
 */
void tree_walk_in_order(tree_t *a, accion_t accion, void *param);

/**
 * @brief Recorre el arbol en orden post-orden (post-order): izquierda, derecha,
 * raiz.
 * @param a Puntero al arbol.
 * @param accion Funcion a ejecutar por cada nodo (recibe contexto + dato).
 * @param param Puntero opaco que se pasa como contexto a la funcion.
 */
void tree_walk_post_order(tree_t *a, accion_t accion, void *param);

/**
 * @brief Recorre el arbol en orden pre-orden (pre-order): raiz, izquierda,
 * derecha.
 * @param a Puntero al arbol.
 * @param accion Funcion a ejecutar por cada nodo (recibe contexto + dato).
 * @param param Puntero opaco que se pasa como contexto a la funcion.
 */
void tree_walk_pre_order(tree_t *a, accion_t accion, void *param);

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
int tree_insert(tree_t *a, const void *d, const size_t tam, const cmp_t cmp);

int tree_load_to_bin_file_sorted(tree_t *t, const char *path, unsigned tamInfo);

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
int tree_insert_iter(tree_t *a, const void *d, const size_t tam,
                     const cmp_t cmp);

/**
 * @brief Devuelve la cantidad total de nodos del arbol.
 * @param a Puntero al arbol.
 * @return int Cantidad de nodos.
 */
int tree_node_count(const tree_t *a);

/**
 * @brief Devuelve la cantidad de nodos hoja (sin hijos) del arbol.
 * @param a Puntero al arbol.
 * @return int Cantidad de nodos hoja.
 */
int tree_node_leaves_count(const tree_t *a);

/**
 * @brief Devuelve la cantidad de nodos que poseen hijo izquierdo.
 * @param a Puntero al arbol.
 * @return int Cantidad de nodos con hijo izquierdo.
 */
int tree_node_wl_count(const tree_t *a);

/**
 * @brief Devuelve la cantidad de nodos que poseen hijo derecho.
 * @param a Puntero al arbol.
 * @return int Cantidad de nodos con hijo derecho.
 */
int tree_node_wr_count(const tree_t *a);

/**
 * @brief Devuelve la cantidad de nodos cuyo dato coincide con el valor pasado
 * como referencia mediante la funcion de comparacion.
 * @param a Puntero al arbol.
 * @param cmp Funcion de comparacion.
 * @param d Dato de referencia para la busqueda.
 * @return int Cantidad de nodos que coinciden con el dato.
 */
int tree_node_w_cond_count(const tree_t *a, cmp_t cmp, void *d);

/**
 * @brief Devuelve la altura del arbol (cantidad de niveles).
 * @param a Puntero al arbol.
 * @return int Altura del arbol.
 */
int tree_height(const tree_t *a);

/**
 * @brief Devuelve la cantidad de nodos en un nivel especifico.
 * @param a Puntero al arbol.
 * @param height Nivel solicitado.
 * @return int Cantidad de nodos en el nivel indicado.
 */
int tree_node_count_in_level(const tree_t *a, int height);

/**
 * @brief Devuelve la cantidad de nodos por debajo de un nivel especifico
 * (excluyendo ese nivel).
 * @param a Puntero al arbol.
 * @param height Nivel de referencia.
 * @return int Cantidad de nodos por debajo del nivel indicado.
 */
int tree_node_count_below_level(const tree_t *a, int height);

/**
 * @brief Devuelve la cantidad de nodos hasta un nivel especifico inclusive.
 * @param a Puntero al arbol.
 * @param height Nivel de referencia.
 * @return int Cantidad de nodos hasta el nivel indicado (inclusive).
 */
int tree_node_count_within_level(const tree_t *a, int height);

/**
 * @brief Devuelve la cantidad de nodos desde un nivel especifico inclusive
 * hacia abajo.
 * @param a Puntero al arbol.
 * @param height Nivel de referencia.
 * @return int Cantidad de nodos desde el nivel indicado (inclusive) hasta las
 * hojas.
 */
int tree_node_count_from_level_inclusive(const tree_t *a, int height);

/**
 * @brief Devuelve la cantidad de nodos por encima de un nivel especifico.
 * @param a Puntero al arbol.
 * @param height Nivel de referencia.
 * @return int Cantidad de nodos por encima del nivel indicado.
 */
int tree_node_count_beyond_level(const tree_t *a, int height);

/**
 * @brief Elimina todas las hojas del arbol.
 * @param t Puntero al arbol.
 * @return int OK (`200`) si se realizo exitosamente.
 */
int tree_cut_leaves(tree_t *t);

/**
 * @brief Destruye el arbol liberando toda la memoria asignada a los nodos y
 * datos.
 * @param t Puntero al arbol.
 * @return int OK (`200`) si se realizo exitosamente, 0 si ya estaba vacio.
 */
int tree_destroy(tree_t *t);

/**
 * @brief Indica si el arbol esta vacio.
 * @param t Puntero al arbol.
 * @return int TRUE (`1`) si esta vacio, FALSE (`0`) en caso contrario.
 */
int tree_is_empty(tree_t *t);

/**
 * @brief Elimina un nodo del arbol que contenga el valor buscado.
 * @param t Puntero al arbol.
 * @param buff Valor a eliminar.
 * @param tam Tamano del valor.
 * @param cmp Funcion de comparacion.
 * @return int TREE_SUCCESS si se elimino, TREE_ERR_EMPTY si no se encontro.
 */
int tree_delete_node(tree_t *t, void *buff, const unsigned tam, cmp_t cmp);

int binarysearchtree_search_r(const tree_t *tree, const void *key, void *buf,
                              size_t n, cmp_t cmp);

#endif // ARBOL_H
