/**
 * @file ListaSimple.h
 * @brief Interfaz de una lista simplemente enlazada.
 */

#ifndef LISTA_SIMPLE_H
#define LISTA_SIMPLE_H

#include "Comun.h"
#include <stdbool.h>

typedef int (*t_Cmp)(const void *a, const void *b);
typedef void (*t_Accion)(void *param, const void *dato);
typedef void (*t_Prnt)(const void *dato);

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
 * @struct s_Nodo
 * @brief Nodo de la lista simplemente enlazada.
 */
typedef struct s_Nodo {
  void *dato;
  unsigned tam;
  struct s_Nodo *sig;
} t_Nodo;

/**
 * @brief Tipo de dato que representa la lista.
 */
typedef t_Nodo *t_Lista;

/**
 * @brief Crea una lista vacia.
 * @param l Puntero a la lista.
 */
void crearLista(t_Lista *l);

/**
 * @brief Indica si la lista esta vacia.
 * @param l Puntero a la lista.
 * @return int TRUE si esta vacia, FALSE en caso contrario.
 */
int listaVacia(const t_Lista *l);

/**
 * @brief Indica si no hay mas espacio disponible en la lista.
 * @param l Puntero a la lista.
 * @param tam Tamano del elemento.
 * @return int FALSE si hay espacio disponible, ERR_MEM_LLENA (`101`) si no.
 */
int listaLlena(const t_Lista *l, const unsigned tam);

/**
 * @brief Vacia la lista completa.
 * @param l Puntero a la lista.
 */
void vaciarLista(t_Lista *l);

/**
 * @brief Inserta un elemento al principio de la lista.
 * @param l Puntero a la lista.
 * @param d Puntero al dato a insertar.
 * @param tam Tamano del dato.
 * @return int OK (`200`) si se realizo exitosamente, ERR_MEM_LLENA (`101`) si no hay espacio.
 */
int insertarAlPrincipioDeLista(t_Lista *l, const void *d, const unsigned tam);

// int ponerEnLista(t_Lista *l, const void *d, const unsigned tam);

/**
 * @brief Obtiene y elimina el primer elemento de la lista.
 * @param l Puntero a la lista.
 * @param buff Buffer de salida.
 * @param tam Tamano del buffer.
 * @return int OK (`200`) si se realizo exitosamente, ERR_LISTA_VACIA si la lista esta vacia.
 */
int sacarPrimeroLista(t_Lista *l, void *buff, const unsigned tam);

/**
 * @brief Inserta un elemento al final de la lista.
 * @param l Puntero a la lista.
 * @param d Puntero al dato a insertar.
 * @param tam Tamano del dato.
 * @return int OK (`200`) si se realizo exitosamente, ERR_MEM_LLENA (`101`) si no hay espacio.
 */
int insertarAlFinalDeLista(t_Lista *l, const void *d, const unsigned tam);

/**
 * @brief Obtiene y elimina el ultimo elemento de la lista.
 * @param l Puntero a la lista.
 * @param d Buffer de salida.
 * @param tam Tamano del buffer.
 * @return int OK (`200`) si se realizo exitosamente, ERR_LISTA_VACIA si la lista esta vacia.
 */
int sacarUltimoDeLista(t_Lista *l, void *d, const unsigned tam);

/**
 * @brief Obtiene el ultimo elemento de la lista sin eliminarlo.
 * @param l Puntero a la lista.
 * @param buff Buffer de salida.
 * @param tam Tamano del buffer.
 * @return int OK (`200`) si se realizo exitosamente, ERR_LISTA_VACIA si la lista esta vacia.
 */
int verUltimoDeLista(t_Lista *l, void *buff, const unsigned tam);

/**
 * @brief Inserta un elemento en la lista con control de duplicados y orden.
 * @param l Puntero a la lista.
 * @param d Puntero al dato a insertar.
 * @param tam Tamano del dato.
 * @param cmp Funcion de comparacion.
 * @param conDup Define si se aceptan duplicados.
 * @param accion Accion a ejecutar cuando hay duplicados.
 * @return int OK (`200`) si se realizo exitosamente, ERR_MEM_LLENA (`101`) si no hay espacio.
 */
int insertarOrdenadoEnLista(t_Lista *l, const void *d, const unsigned tam,
                            t_Cmp cmp, const int conDup, t_Accion accion);

/**
 * @brief Obtiene el primer elemento de la lista sin eliminarlo.
 * @param l Puntero a la lista.
 * @param buff Buffer de salida.
 * @param tam Tamano del buffer.
 * @return int OK (`200`) si se realizo exitosamente, ERR_LISTA_VACIA si la lista esta vacia.
 */
int verPrimeroDeLista(t_Lista *l, void *buff, const unsigned tam);

/**
 * @brief Elimina un elemento por clave.
 * @param l Puntero a la lista.
 * @param buff Buffer de salida.
 * @param tam Tamano del buffer.
 * @param cmp Funcion de comparacion.
 * @return int OK (`200`) si se realizo exitosamente, ERR_LISTA_NO_ENCONTRADO si no existe.
 */
int eliminarPorClave(t_Lista *l, void *buff, const unsigned tam, t_Cmp cmp);

/**
 * @brief Obtiene el elemento de una posicion.
 * @param l Puntero a la lista.
 * @param buff Buffer de salida.
 * @param tam Tamano del buffer.
 * @param pos Posicion solicitada.
 * @return int OK (`200`) si se realizo exitosamente.
 */
int verElementoEnPosicion(t_Lista *l, void *buff, const unsigned tam, int pos);

/**
 * @brief Devuelve la longitud de la lista.
 * @param l Puntero a la lista.
 * @return int Cantidad de elementos.
 */
int longitudLista(t_Lista *l);

/**
 * @brief Busca un elemento en la lista.
 * @param l Puntero a la lista.
 * @param buff Dato a buscar.
 * @param tam Tamano del dato.
 * @param cmp Funcion de comparacion.
 * @return int OK (`200`) si se encuentra, ERR_LISTA_NO_ENCONTRADO si no.
 */
int buscarEnLista(t_Lista *l, void *buff, const unsigned tam, t_Cmp cmp);

/**
 * @brief Copia una lista en otra.
 * @param l Puntero a la lista origen.
 * @param lCopia Puntero a la lista destino.
 * @return int OK (`200`) si se realizo exitosamente.
 */
int copiarLista(t_Lista *l, t_Lista *lCopia);

/**
 * @brief Inserta un elemento en una posicion especifica.
 * @param l Puntero a la lista.
 * @param buff Puntero al dato a insertar.
 * @param tam Tamano del dato.
 * @param pos Posicion de insercion.
 * @return int OK (`200`) si se realizo exitosamente.
 */
int insertarEnPosicion(t_Lista *l, const void *buff, const unsigned tam,
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
int insertarDespuesDeClave(t_Lista *l, const void *d, const unsigned tam,
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
int insertarAntesDeClave(t_Lista *l, const void *d, const unsigned tam,
                         const void *clave, const t_Cmp cmp);

/**
 * @brief Elimina un elemento por posicion.
 * @param l Puntero a la lista.
 * @param buff Buffer de salida.
 * @param tam Tamano del buffer.
 * @param pos Posicion a eliminar.
 * @return int OK (`200`) si se realizo exitosamente.
 */
int eliminarPorPosicion(t_Lista *l, void *buff, const unsigned int tam,
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
int eliminarAntesDeClave(t_Lista *l, void *buff, const unsigned int tam,
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
int eliminarDespuesDeClave(t_Lista *l, void *buff, const unsigned int tam,
                           const void *clave, const t_Cmp cmp);

/**
 * @brief Muestra todos los elementos de la lista.
 * @param l Puntero a la lista.
 * @param prnt Funcion de impresion.
 */
void mostrarLista(const t_Lista *l, const t_Prnt prnt);

/**
 * @brief Invierte el orden de los elementos de la lista.
 * @param l Puntero a la lista.
 * @return int OK (`200`) si se realizo exitosamente.
 */
int invertirLista(t_Lista *l);

/**
 * @brief Concatena la segunda lista al final de la primera.
 * @param l1 Lista destino.
 * @param l2 Lista a concatenar.
 * @return int OK (`200`) si se realizo exitosamente.
 */
int concatenarListas(t_Lista *l1, t_Lista *l2);

/**
 * @brief Cuenta cuantas veces aparece un dato en la lista.
 * @param l Puntero a la lista.
 * @param d Dato a contar.
 * @param res Puntero al resultado.
 * @param cmp Funcion de comparacion.
 * @return int OK (`200`) si se realizo exitosamente.
 */
int contarApariciones(t_Lista *l, const void *d, int *res, const t_Cmp cmp);

/**
 * @brief Indica si la lista contiene un dato.
 * @param l Puntero a la lista.
 * @param d Dato a buscar.
 * @param cmp Funcion de comparacion.
 * @return int OK (`200`) si lo contiene, ERR_LISTA_NO_ENCONTRADO si no.
 */
int listaContiene(t_Lista *l, const void *d, const t_Cmp cmp);

/**
 * @brief Ordena la lista con el algoritmo indicado.
 * @param l Puntero a la lista.
 * @param ordenamiento Tipo de ordenamiento a usar.
 * @param cmp Funcion de comparacion.
 * @return int OK (`200`) si se realizo exitosamente.
 */
int ordenarLista(t_Lista *l, const int ordenamiento, const t_Cmp cmp);

#endif // LISTA_SIMPLE_H
