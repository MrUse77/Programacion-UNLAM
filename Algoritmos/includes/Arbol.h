#ifndef ARBOL_H
#define ARBOL_H
#include <Comun.h>
#include <stdlib.h>

#define ARBOL_VACIO 1
#define MISMO_VALOR 2

typedef void (*t_Prnt)(const void *dato);
typedef int (*t_Cmp)(const void *menor, const void *mayor);
typedef struct s_Nodo {
  void *dato;
  unsigned tam;
  struct s_Nodo *izq, *der;
} t_Nodo;

typedef t_Nodo *t_Arbol;

void crearArbol(t_Arbol *a);
void recorrerArbolInOrder(t_Arbol *a, t_Prnt mostrar);
void recorrerArbolPostOrder(t_Arbol *a, t_Prnt mostrar);
void recorrerArbolPreOrder(t_Arbol *a, t_Prnt mostrar);
int insertarEnArbol(t_Arbol *a, const void *d, const size_t tam,
                    const t_Cmp cmp);
int insertarEnArbolIterativo(t_Arbol *a, const void *d, const size_t tam,
                             const t_Cmp cmp);

#endif
