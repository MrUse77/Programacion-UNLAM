#ifndef UTILSPILA_INCLUDE_H
#define UTILSPILA_INCLUDE_H

#include <Pila.h>
#include <stdlib.h>
#define ERR_FILE -2

typedef void (*FmtTxt)(void *elem, const void *linea);
typedef size_t (*tamElem)(char *ctx);
typedef void (*FmtPrnt)(const void *d);

int crearPilaDesdeArchivo_TXT(t_Pila *p, const char *nomArch, int header,
                              tamElem obtenetTamElem, FmtTxt format);
int crearPilaDesdeArchivo_BIN(t_Pila *p, const char *nomArch,
                              tamElem obtenerTamElem);

#endif
