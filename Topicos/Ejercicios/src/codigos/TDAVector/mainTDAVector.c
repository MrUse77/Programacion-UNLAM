
#include "../../headers/Vector.h"
#include "../../headers/ejercicios.h"
#include <stdlib.h>
#include <time.h>
int mainTDAVector() {
  // Change the code page to support spanish characters
  Vector miVec;

  if (!vectorCrear(&miVec)) {
    return SIN_MEM;
  }

  srand(time(NULL));

  for (int i = 1; i <= 100000; i++) {
    vectorOrdInsertar(&miVec, rand());
  }

  for (int i = 1; i <= 100000; i++) {
    vectorOrdEliminar(&miVec, rand());
  }

  for (int i = 0; i <= RAND_MAX; i++) {
    vectorOrdEliminar(&miVec, i);
  }
  vectorDestruir(&miVec);

  return 0;
}
