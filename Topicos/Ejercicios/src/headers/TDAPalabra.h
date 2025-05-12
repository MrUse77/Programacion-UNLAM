#ifndef TDAPALABRA_H
#define TDAPALABRA_H
#define TAMP_PAL 51
#include <stdbool.h>

typedef struct {
  char vPal[TAMP_PAL];

} Palabra;

void palabraMostrar(const Palabra *pal);
bool esLetra(char c);
void palabraATitulo(Palabra *pal);
#endif
