#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED
#define TAM_MAX 10
#define VERDADERO 1
#define FALSO 0
#define DUPLICADO 2
#define LLENO 3
typedef struct {
  int vector[TAM_MAX];
  int ce;
} Vector;

void crearVector(Vector *v);
int llenarVector(const Vector *v);
int vaciarVector(const Vector *v);
int setVectorOrdenado(Vector *v, int elemento);
int deleteElemento(Vector *v, int elemento);
void destoryVector(Vector *v);

#endif // VECTOR_H_INCLUDED
