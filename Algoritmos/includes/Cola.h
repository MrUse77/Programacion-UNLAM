
#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED

#define TAM_COLA 100
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERR 2

typedef struct {
  char Vector[TAM_COLA];
  char *prim;
  char *ult;
  int tamDisp;
} t_Cola;

void crearCola(t_Cola *c);
int ponerEnCola(t_Cola *c, void *d, unsigned tamDato);
int colaLlena(t_Cola *c);
int verPrimero(t_Cola *c, void *buff, unsigned tamDato);
int sacarDeCola(t_Cola *c, void *buff, unsigned tamDato);
int colaVacia(t_Cola *c);
void vaciarCola(t_Cola *c);

#endif // COLA_H_INCLUDED
