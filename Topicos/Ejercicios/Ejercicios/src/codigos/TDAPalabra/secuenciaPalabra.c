#include "../../headers/SecuenciaPalabra.h"

void secuenciaPalabraCrear(secuenciaPalabra *sec, const char *cadOrig) {
  sec->cursor = (char *)cadOrig;
  sec->fin = false;
}

bool secuenciaPalabraLeer(secuenciaPalabra *sec, Palabra *pal) {

  // tambien se puede escribir como sec->cursor != '\0'
  while (*sec->cursor && !esLetra(*sec->cursor)) {
    sec->cursor++;
  }

  // tambien se puede escribir como sec->cursor != '\0'
  if (!*sec->cursor) {
    sec->fin = true;
    return false;
  }

  char *iPal = pal->vPal;
  do {

    *iPal = *sec->cursor;
    iPal++;
    sec->cursor++;
  } while (*sec->cursor && esLetra(*sec->cursor));
  *iPal = '\0';
  return true;
}

bool secuenciaPalabraFin(const secuenciaPalabra *sec) { return sec->fin; }
void secuenciaPalabraEscribir(secuenciaPalabra *sec, const Palabra *pal) {
  char *iPal = (char *)pal->vPal;
  while (*iPal) {
    *sec->cursor = *iPal;
    iPal++;
    sec->cursor++;
  }
}
void secuenciaPalabraEscribirCaracter(secuenciaPalabra *sec, char c) {
  *sec->cursor = c;
  sec->cursor++;
}
void secuenciaPalabraCerrar(secuenciaPalabra *sec) { *sec->cursor = '\0'; }
