
#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERR 2
#define ERR_MEM 101
#define ERR_LLENA 301
#define ERR_VACIA 302

#ifdef COLA_ESTATICA
#define TAM_COLA 10000
typedef struct {
  char Vector[TAM_COLA];
  unsigned prim;
  unsigned ult;
  unsigned tamDisp;
} t_Cola;
#else

typedef struct sNodo { // sNodo defino la estructura
  void *dato;
  unsigned tamDato;
  struct sNodo *sig;
} t_Nodo; // inicializo el tipo de dato
typedef struct {
  t_Nodo *prim, *ult;
} t_Cola;
#endif

void crearCola(t_Cola *c);
int ponerEnCola(t_Cola *c, const void *d, unsigned tamDato);
int colaLlena(t_Cola *c, unsigned tam);
int verPrimero(t_Cola *c, void *buff, unsigned tamDato);
int sacarDeCola(t_Cola *c, void *buff, unsigned tamDato);
int colaVacia(t_Cola *c);
void vaciarCola(t_Cola *c);

#endif // COLA_H_INCLUDED
