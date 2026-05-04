
#ifndef LISTA_SIMPLE_H
#define LISTA_SIMPLE_H

#include "Comun.h"
#include <stdbool.h>

typedef int (*t_Cmp)(const void *a, const void *b);
typedef void (*t_Accion)(void *param, const void *dato);

/**
 * @def ERR_LISTA_VACIA
 * @brief Error: Lista sin elementos
 */
#define ERR_LISTA_VACIA 301

#define ERR_LISTA_NO_ENCONTRADO 302

/**
 * @struct s_Nodo
 * @brief Representa un dato, con su tamaño y cual es el proximo nodo
 */
typedef struct s_Nodo {
  void *dato;
  unsigned tam;
  struct s_Nodo *sig;
} t_Nodo;

/**
 * @brief Puntuero a puntero para poder formar la lista
 */
typedef t_Nodo *t_Lista;

/**
 * @brief Crea una lista de tamaño dinamico
 * @param l puntero a lista
 */
void crearLista(t_Lista *l);

/**
 * @brief Revisa si la lista esta esta vacia
 * @param l Puntero a lista
 * @returns int Si esta vacia, devuelve TRUE, si no, devuelve FALSE
 */
int listaVacia(const t_Lista *l);

/**
 * @brief Revisa si no hay mas espacio disponible en la lista
 * @param l Puntero a lista
 * @param tam Tamaño del elemento
 * @return int Si hay espacio disponible devuelve FALSE, si no hay mas espacio
 * devuelve ERR_MEM_LLENA (`101`)
 */
int listaLlena(const t_Lista *l, const unsigned tam);

/**
 * @brief Vacia la lista entera
 * @param l Puntero a lista
 */
void vaciarLista(t_Lista *l);

/**
 * @brief Coloca el elemento al inicio de la lista
 * @param l Puntero a lista
 * @param d Puntero a elemento
 * @param tam Tamaño del elemento
 * @return int Devuevle OK (`200`) si se realizo exitosamente, devuelve
 * ERR_MEM_LLENA (`101`) si no hay espacio
 */
int insertarAlPrincipioDeLista(t_Lista *l, const void *d, const unsigned tam);

// int ponerEnLista(t_Lista *l, const void *d, const unsigned tam);

/**
 * @brief Se obtiene el primer elemento de la lista y se elimina de la lista
 * @param l Puntero a lista
 * @param d Puntero a elemento
 * @param tam Tamaño del elemento
 * @return int Devuevle OK (`200`) si se realizo exitosamente, devuelve
 * ERR_LISTA_VACIA si no hay espacio
 */
int sacarPrimeroLista(t_Lista *l, void *buff, const unsigned tam);

/**
 * @brief Se coloca un elemento en la ultima posicion de la lista
 * @param l Puntero a lista
 * @param d Puntero a elemento
 * @param tam Tamaño del elemento
 * @return int Devuevle OK (`200`) si se realizo exitosamente, devuelve
 * ERR_LISTA_VACIA si no hay espacio
 */
int insertarAlFinalDeLista(t_Lista *l, const void *d, const unsigned tam);

/**
 * @brief Se obtiene el ultimo elemento de la lista y se elimina de la lista
 * @param l Puntero a lista
 * @param d Puntero a elemento
 * @param tam Tamaño del elemento
 * @return int Devuevle OK (`200`) si se realizo exitosamente, devuelve
 * ERR_LISTA_VACIA si no hay espacio
 */
int sacarUltimoDeLista(t_Lista *l, void *d, const unsigned tam);

/**
 * @brief Se obtiene el ultimo elemento de la lista sin eliminarlo de la lista
 * @param l Puntero a lista
 * @param d Puntero a elemento
 * @param tam Tamaño del elemento
 * @return int Devuevle OK (`200`) si se realizo exitosamente, devuelve
 * ERR_LISTA_VACIA si no hay espacio
 */
int verUltimoDeLista(t_Lista *l, void *buff, const unsigned tam);

/**
 * @brief Se inserta un elemento en la lista, eligiendo si se permite duplicado
 y como se ordena
 * @param l Puntero a lista
 * @param d Puntero a dato
 * @param tam Tamaño del elemento
 * @param cmp Callback de comparacion
 * @param conDup Define si se aceptan duplicados
 * @param accion Callback de lo que se hace hay duplicados
 * @return int Devuelve OK (`200`) si se realizo exitosamente, devuelve
 * ERR_LISTA_VACIA si no hay espacio
 */
int insertarOrdenadoEnLista(t_Lista *l, const void *d, const unsigned tam,
                            t_Cmp cmp, const int conDup, t_Accion accion);

/**
 * @brief Se obtiene el primer elemento de la lista sin eliminarlo de la lista
 * @param l Puntero a lista
 * @param d Puntero a elemento
 * @param tam Tamaño del elemento
 * @return int Devuevle OK (`200`) si se realizo exitosamente, devuelve
 * ERR_LISTA_VACIA si no hay espacio
 */
int verPrimeroDeLista(t_Lista *l, void *buff, const unsigned tam);

int eliminarPorClave(t_Lista *l, void *buff, const unsigned tam, t_Cmp cmp);

int verElementoEnPosicion(t_Lista *l, void *buff, const unsigned tam, int pos);

int longitudLista(t_Lista *l);

int buscarEnLista(t_Lista *l, void *buff, const unsigned tam, t_Cmp cmp);

int copiarLista(const t_Lista *l, t_Lista *lCopia);

int clonarLista(const t_Lista *l, t_Lista *lClon);

int insertarEnPosicion(t_Lista *l, const void *buff, const unsigned tam,
                       const int pos);

int insertarDespuesDeClave(t_Lista *l, const void *buff, const unsigned tam,
                           const t_Cmp cmp);

int insertarAntesDeClave(t_Lista *l, const void *buff, const unsigned tam,
                         const t_Cmp cmp);

int eliminarPorPosicion(t_Lista *l, void *buff, const unsigned int tam,
                        const int pos);

int eliminarAntesDeClave(t_Lista *l, void *buff, const unsigned int tam,
                         const int pos);

int eliminarDespuesDeClave(t_Lista *l, void *buff, const unsigned int tam,
                           const int pos);

int mostrarLista(const t_Lista *l);

int invertirLista(t_Lista *l);

int concatenarListas(t_Lista *l1, t_Lista *l2);

int contarApariciones(t_Lista *l, int *res, const t_Cmp cmp);

int listaContiene(t_Lista *l, const t_Cmp cmp);

#endif // FECHA_H
