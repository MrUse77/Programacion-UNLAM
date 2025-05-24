
#include "../../headers/Vector.h"
#include "../../headers/ejercicios.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int mainTDAVector() {
  // Change the code page to support spanish characters in Windows terminal
  system("chcp 65001 > nul");

  Vector miVec;
  if (!vectorCrear(&miVec)) {
    return SIN_MEM;
  }

  srand(time(NULL));

  for (int i = 1; i <= 100000; i++) {
    vectorInsertarAlFinal(&miVec, rand());
  }

  // puts("Antes de ordenar:");
  // vectorMostrar(&miVec);

  time_t segIni = time(NULL);
  vectorOrdenar(&miVec, INSERCION);

  time_t segFin = time(NULL);

  printf("Tiempo de ordenamiento: %ld\n", segFin - segIni);

  // puts("Después de ordenar:");
  // vectorMostrar(&miVec);

  vectorDestruir(&miVec);

  return 0;
}
