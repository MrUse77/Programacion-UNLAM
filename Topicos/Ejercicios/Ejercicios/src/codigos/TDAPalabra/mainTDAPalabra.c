#include "../../headers/SecuenciaPalabra.h"
#include <stdio.h>

char *normalizar(char *cadDest, const char *cadOrig);

int mainTDAPalabra() {
  char cadOrig[] = "#%cAdeNA&/(a)&/{/}/()=)NOrMaLiZaR&/()";
  char cadDest[51];

  normalizar(cadDest, cadOrig);

  puts(cadDest);
  return 0;
}

char *normalizar(char *cadDest, const char *cadOrig) {

  secuenciaPalabra secPal;

  secuenciaPalabraCrear(&secPal, cadOrig);
  secuenciaPalabra secE;
  secuenciaPalabraCrear(&secE, cadDest);
  Palabra pal;
  secuenciaPalabraLeer(&secPal, &pal);
  while (!secuenciaPalabraFin(&secPal)) {
    //    palabraMostrar(*pal);
    palabraATitulo(&pal);
    secuenciaPalabraEscribir(&secE, &pal);
    secuenciaPalabraEscribirCaracter(&secE, ' ');
    secuenciaPalabraLeer(&secPal, &pal);
  }
  secuenciaPalabraCerrar(&secE);
  return cadDest;
}
