#ifndef MATRIZ_MATRIZ_H
#define MATRIZ_MATRIZ_H

#include <stddef.h>


void** matrizCrear(int fil, int col, size_t tamElem);
void matrizDestruir(void** mat, int fil);
void matrizMostrar(int fil, int col, int** mat);


#endif //MATRIZ_MATRIZ_H
