#ifndef ARCHIVOS_INCLUDED
#define ARCHIVOS_INCLUDED
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vector.h"

typedef void (*BinATxt)(const void *reg, FILE *archTxt);
typedef int (*TxtABin)(char *linea, const void *reg);
typedef bool (*EsErrorFatal)(int ret);
typedef void (*CrearRegIdx)(void *, unsigned int, void *);
int convTxtABin(const char *bin, const char *txt, size_t tamReg,
		TxtABin TxtABin, EsErrorFatal errorFatal);
int convBinATexto(const char *bin, const char *txt, size_t tamReg,
		  BinATxt BinATxt);
void cambiarExtension(const char *nomArch, const char *ext,
		      char *nomArchCambiado);
int crearIndice(const char *nombArch, size_t tam, size_t tamRegIdx,
		CrearRegIdx crearRegIdx, Cmp cmpRegIdx);

#endif // ARCHIVOS_INCLUDED
