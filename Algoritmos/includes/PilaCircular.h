
#ifndef PILA_CIRCULAR_H_INCLUDED
#define PILA_CIRCULAR_H_INCLUDED

#include <Comun.h>
/**
 * @def ERR_PIL_VACIA
 * @brief Error: pila vacía.
 */
#define ERR_PIL_VACIA 301

/**
 * @def ERR_PIL_LLENA
 * @brief Error: pila llena.
 */
#define ERR_PIL_LLENA 302

/**
 * @struct s_Nodo
 * @brief Representa una pila, solo se puede sacar, ver o poner en la cima.
 */
typedef struct s_Nodo { // sNodo defino la estructura
  void *dato;
  unsigned tamDato;
  struct s_Nodo *sig;
} t_Nodo; // inicializo el tipo de dato

/**
 * @brief Puntuero a puntero para poder formar la Pila.
 */
typedef t_Nodo *t_Pila;

/**
 * @brief Crea una pila de tamaño fijo o dinamica dependiendo de la version que
 * se use
 * @param p puntero a pila
 */
void crearPila(t_Pila *p);

/**
 * @brief Revisa si la pila esta llena para el elemento que se quiera insertar
 * @brief Es compatible con memoria dinamica pero se recomienda su uso sobre la
 * estatica
 * @param p Puntero a pila
 * @param tam Tamaño del elemento
 * @returns int Si hay espacio, devuelve FALSE, si no hay, devuelve TRUE
 */
int pilaLlena(const t_Pila *p, unsigned tam);

/**
 * @brief Revisa si la pila esta esta vacia
 * @param p Puntero a pila
 * @returns int Si esta vacia, devuelve TRUE, si no, devuelve FALSE
 */
int pilaVacia(const t_Pila *p);

/**
 * @brief Vacia la pila entera
 * @param p Puntero a pila
 */
void vaciarPila(t_Pila *p);

/**
 * @brief Coloca un elemento en el tope de la pila
 * @param p Puntero a pila
 * @param d Puntero a elemento
 * @param tam Tamaño del elemento
 * @returns int Si salio bien reotrna OK, si la pila esta llena devuelve
 * ERR_PIL_LLENA
 */
int apilar(t_Pila *p, const void *d, unsigned tam);

/**
 * @brief Se obtiene el elemento del tope de la pila y se descarta
 * Puede ocurrir que se obtenga un tamaño menor a tamDato;
 * si el tamaño del dato es mayor que tamDato, el sobrante del dato se pierde
 * @param p puntero a pila
 * @param buffer puntero a buffer para obtener el elemento
 * @param tam tamaño del buffer
 * @returns int Si salio bien reotrna OK, si la pila esta vacia devuelve
 * ERR_PIL_VACIA
 */
int desapilar(t_Pila *p, void *buffer, unsigned tam);

/**
 * @brief Se obtiene el dato del tope de la pila sin eliminarlo
 * @param p puntero a pila
 * @param buffer puntero a buffer para obtener el elemento
 * @param tam tamaño del buffer
 * @returns int Si salio bien reotrna OK, si la pila esta vacia devuelve
 * ERR_PIL_VACIA
 */
int verTope(const t_Pila *p, void *buffer, unsigned tam);

#endif // PILA_H_INCLUDED
