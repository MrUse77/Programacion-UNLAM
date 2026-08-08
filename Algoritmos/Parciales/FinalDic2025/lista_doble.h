#ifndef LISTA_DOBLE_H_INCLUDED
#define LISTA_DOBLE_H_INCLUDED

typedef struct nodo_s {
  struct nodo_s *sig, *ant;
  void *dato;
  unsigned tam;
} nodo_t;

typedef nodo_t *lista_doble_t;

typedef int (*cmp_t)(void *a, void *b);
typedef void (*acum_t)(void *ctx, void *param);
typedef void (*accion_t)(void *ctx, void *param);

void lista_doble_iniciar(lista_doble_t *l);
int lista_doble_insertar_ultimo(lista_doble_t *l, const void *d, unsigned tam);
int lista_doble_sacar(lista_doble_t *l, void *buff, unsigned tam);
int lista_doble_vaciar(lista_doble_t *l);
int lista_doble_ver_pos(lista_doble_t *l, void *buff, unsigned tam, int pos);
int lista_doble_eliminar_dup(lista_doble_t *l, cmp_t cmp, accion_t acum);
int lista_doble_reduce(lista_doble_t *l, acum_t acum, void *buff);
void lista_doble_mostrar(lista_doble_t *l, accion_t accion, void *param);

#endif // LISTA_DOBLE_H_INCLUDED
