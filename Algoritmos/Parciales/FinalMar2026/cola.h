#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED

typedef struct nodo_s {
  struct nodo_s *sig;
  void *dato;
  unsigned tam_dato;
} nodo_t;

typedef struct {
  nodo_t *prim, *ult;
} cola_t;

void cola_iniciar(cola_t *c);
int cola_insertar(cola_t *c, const void *d, unsigned tam);
int cola_sacar(cola_t *c, void *buff, unsigned tam);
int cola_ver_primero(cola_t *c, void *buff, unsigned tam);
int cola_llena(cola_t *c, unsigned tam);
int cola_vacia(cola_t *c);
int cola_vaciar(cola_t *c);

#endif // COLA_H_INCLUDED
