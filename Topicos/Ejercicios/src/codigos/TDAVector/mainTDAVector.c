
#include "../../headers/Vector.h"
#include "../../headers/ejercicios.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
  int d;
  int m;
  int a;
} Fecha;

typedef struct {
  int legajo;
  char nya[51];
} Empleado;

void PrintEntero(const void *elem);
void PrintFecha(const void *elem);
int CompareFecha(const void *a, const void *b);
int CompareEmp(const void *a, const void *b);
int mainTDAVector() {
  // Change the code page to support spanish characters in Windows terminal
  // system("chcp 65001 > nul");

  Vector miVec;
  if (!vectorCrear(&miVec, sizeof(Empleado))) {
    return SIN_MEM;
  }

  srand(time(NULL));
  // Fecha f;
  Empleado e;
  for (int i = 1; i <= 10; i++) {
    /*
        f.d = rand() % 31 + 1;
        f.m = rand() % 12 + 1;
        f.a = rand() % 100 + 1926; // Años entre 1920 y 2019
    */

    e.legajo = i;
    strcpy(e.nya, "Juan Perez");

    printf("Empleado %d: Legajo: %d, Nombre y Apellido: %s\n", i, e.legajo,
           e.nya);
    vectorInsertarAlFinal(&miVec, &e);
    //   vectorOrdInsertar(&miVec, &e, CompareEmp);
  }
  Empleado eABuscar;
  eABuscar.legajo = 2;
  int pos = vectorOrdBuscar(&miVec, &eABuscar, CompareEmp);

  printf("Posición del empleado con legajo %d: %d\n", eABuscar.legajo, pos);
  printf("Empleado encontrado: Legajo: %d, Nombre y Apellido: %s\n",
         eABuscar.legajo, e.nya);

  //  puts("Antes de ordenar:");
  // vectorMostrar(&miVec, PrintFecha);
  /*
    time_t segIni = time(NULL);
    vectorOrdenar(&miVec, QSORT, CompareFecha);
    time_t segFin = time(NULL);
  */
  //  printf("Tiempo de ordenamiento: %ld\n", segFin - segIni);

  // puts("Después de ordenar:");
  //   vectorMostrar(&miVec, PrintFecha);

  vectorDestruir(&miVec);

  return 0;
}
void PrintEntero(const void *elem) {
  const int *entero = (int *)elem;
  printf("%3d", *entero);
}
void PrintFecha(const void *elem) {
  const Fecha *fecha = (Fecha *)elem;
  printf("%02d/%02d/%04d\n", fecha->d, fecha->m, fecha->a);
}
int CompareFecha(const void *a, const void *b) {
  const Fecha *f1 = a;
  const Fecha *f2 = b;
  return (f1->a * 10000 + f1->m * 100 + f1->d) -
         (f2->a * 10000 + f2->m * 100 + f2->d);
}
int CompareEmp(const void *a, const void *b) {
  const Empleado *e1 = a;
  const Empleado *e2 = b;
  return e1->legajo - e2->legajo; // Comparar por legajo
}
