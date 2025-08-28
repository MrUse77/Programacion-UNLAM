#include "../../headers/TDAPalabra.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

void palabraMostrar(const Palabra *pal) { puts(pal->vPal); }

void palabraATitulo(Palabra *pal) {
  char *iPal = pal->vPal;
  *iPal = toupper(*iPal);
  iPal++;
  while (*iPal) {
    if (*(iPal - 1) == ' ') {
      *iPal = toupper(*iPal);
    } else {
      *iPal = tolower(*iPal);
    }
    iPal++;
  }
}
