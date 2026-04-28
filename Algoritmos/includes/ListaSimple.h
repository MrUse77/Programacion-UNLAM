
#ifndef LISTA_SIMPLE_H
#define LISTA_SIMPLE_H

#include <stdbool.h>
#define TRUE 1
#define FALSE 0
#define ERR_MEM_LLENA 101
#define OK 200
#define ERR_LISTA_VACIA 301
#define ERR_ASIGNACION 302

typedef struct sNodo {
  void *dato;
  unsigned tam;
  struct sNodo *sig;
} t_Nodo;
typedef t_Nodo *t_Lista;

void crearLista(t_Lista *l);
int listaVacia(const t_Lista *l);
int listaLlena(const t_Lista *l, const unsigned tam);
void vaciarLista(t_Lista *l);
int ponerAlComienzoDeLista(t_Lista *l, const void *d, const unsigned tam);
int ponerEnLista(t_Lista *l, const void *d, const unsigned tam);
int sacarPrimeroLista(t_Lista *l, void *buff, const unsigned tam);
int ponerAlFinalDeLista(t_Lista *l, const void *d, const unsigned tam);
int sacarUltimoDeLista(t_Lista *l, void *d, unsigned tam);
int verUltimoDeLista(t_Lista *l, void *buff, unsigned tam);

#endif // FECHA_H
