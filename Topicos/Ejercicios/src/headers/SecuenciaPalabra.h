#ifndef SECUENCIAPALABRA_H
#define SECUENCIAPALABRA_H
#include "TDAPalabra.h"
#include <stdbool.h>

typedef struct {
  char *cursor;
  bool fin;
} secuenciaPalabra;

void secuenciaPalabraCrear(secuenciaPalabra *sec, const char *cadOrig);
bool secuenciaPalabraLeer(secuenciaPalabra *sec, Palabra *pal);
bool secuenciaPalabraFin(const secuenciaPalabra *sec);
void secuenciaPalabraEscribir(secuenciaPalabra *sec, const Palabra *pal);
void secuenciaPalabraEscribirCaracter(secuenciaPalabra *sec, char c);
void secuenciaPalabraCerrar(secuenciaPalabra *sec);
#endif
