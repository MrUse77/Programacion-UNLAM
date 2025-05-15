#include <stdio.h>
#include <stdlib.h>
#define FILAS 4
#define COLUMNAS 4
#define ORDEN 4
void **crearMatriz(size_t tamElem, int fil, int col);
void destruirMatriz(void **mat, int fil);
void newMatriz(int fil, int col, int **mat);
void mostrarMatriz(int **mat, int filas);
int sumaDiagonalPrincipal(int orden, int **mat);
int sumaDiagonalSecundaria(int orden, int **mat);
int sumaTriangularInferiorPrincipal(int orden, int **mat);
int sumaTriangularSuperiorSecundaria(int orden, int **mat);
int main() {
  //  int matriz[FILAS][COLUMNAS] = {
  //     {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};

  int orden = ORDEN;
  // matriz con punteros
  // int(*matriz)[COLUMNAS] = malloc(sizeof(int) * COLUMNAS * COLUMNAS); //
  int **matriz = (int **)crearMatriz(sizeof(int), FILAS, COLUMNAS);
  if (!matriz) {
    return 1;
  }
  newMatriz(orden, orden, matriz);
  mostrarMatriz(matriz, 4);

  int sumaSecundaria = sumaDiagonalSecundaria(4, matriz);
  printf("suma diagonal: %d\n", sumaDiagonalPrincipal(4, matriz));
  printf("suma diagonal secundaria: %d\n", sumaSecundaria);
  printf("suma triangular inferior principal: %d\n",

         sumaTriangularInferiorPrincipal(4, matriz));
  printf("suma triangular superior secundaria: %d\n",
         sumaTriangularSuperiorSecundaria(4, matriz));
  destruirMatriz((void **)matriz, FILAS);
  return 0;
}

void mostrarMatriz(int **mat, int filas) {
  int i, j;

  for (i = 0; i < filas; i++) {
    for (j = 0; j < COLUMNAS; j++) {
      printf("%3d", mat[i][j]);
    }
    putchar('\n');
  }
}

int sumaDiagonalPrincipal(int orden, int **mat) {
  int suma = 0;
  for (int i = 0; i < orden; i++) {
    suma += mat[i][i];
  }
  return suma;
}

int sumaDiagonalSecundaria(int orden, int **matriz) {

  int suma = 0;
  for (int i = 0; i < orden; i++) {
    suma += matriz[i][orden - 1 - i];
  }
  return suma;
}

int sumaTriangularInferiorPrincipal(int orden, int **matriz) {
  int suma = 0;

  for (int i = 1; i < orden; i++) {
    for (int j = 0; j < i; j++) {
      suma += matriz[i][j];
    }
  }
  return suma;
}

int sumaTriangularSuperiorSecundaria(int orden, int **matriz) {
  int suma = 0;

  for (int i = 0; i < orden; i++) {
    for (int j = 0; j < orden - 1 - i; j++) {
      suma += matriz[i][j];
    }
  }
  return suma;
}
void newMatriz(int fil, int col, int **mat) {
  int cont = 1;
  for (int i = 0; i < fil; i++) {
    for (int j = 0; j < col; j++) {
      mat[i][j] = cont++;
    }
  }
}

void **crearMatriz(size_t tamElem, int fil, int col) {
  void **matriz = malloc(fil * sizeof(void *));
  if (!matriz) {
    return NULL;
  }
  void **ult = matriz + fil - 1;
  for (void **i = matriz; i <= ult; i++) {
    *i = malloc(tamElem * col);
    if (!*i) {
      destruirMatriz(matriz, i - matriz);
    }
  }
  return matriz;
}

void destruirMatriz(void **mat, int fil) {
  void **ult = mat + fil - 1;
  for (void **i = mat; i <= ult; i++) {
    free(*i);
  }
  free(mat);
}
