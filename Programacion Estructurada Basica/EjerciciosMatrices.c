#include "flush.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void ej1();
void ej2();
void SumaVector(int[5][4]);
void MultiplicacionVector(int[5][4]);
void ej3();
void MaxValor3(int[5][8]);
void ej4();
void MaxVend4(int[15][10]);
void ej5();
int Asiento5(int);
void MayorEspectadores5(char[12][9]);
void ej6();
void ej7();
void ej8();
void ej9();
void ej10();
void ej11();

int main() {
  int i;
  srand(time(NULL));
  printf("Introduce un número: ");
  scanf("%d", &i);
  switch (i) {
  case 1:
    ej1();
    break;
  case 2:
    ej2();
    break;
  case 3:
    ej3();
    break;
  case 4:
    ej4();
    break;
  case 5:
    ej5();
    break;
  case 6:
    ej6();
    break;
  case 7:
    ej7();
    break;
  case 8:
    ej8();
    break;
  case 9:
    ej9();
    break;
  case 10:
    ej10();
    break;
  case 11:
    ej11();
    break;
  }
  return 0;
}

void ej1() {
  int matriz[3][3];
  int i, j, mult;
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      printf("Ingrese numero en [%d][%d] ", i, j);
      scanf("%d", &matriz[i][j]);
    }
    printf("\n");
  }
  printf("ingrese el multiplicador: ");
  scanf("%d", &mult);
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      printf("%d ", mult * matriz[i][j]);
    }
    printf("\n");
  }
}
void ej2() {
  int matriz[5][4];
  int i, j;
  for (i = 0; i < 5; i++) {
    for (j = 0; j < 4; j++) {
      printf("Ingrese numero en [%d][%d] ", i, j);
      scanf("%d", &matriz[i][j]);
    }
    printf("\n");
  }
  SumaVector(matriz);
  MultiplicacionVector(matriz);
}

void SumaVector(int matriz[5][4]) {
  int i, j, suma[5] = {0};
  for (i = 0; i < 5; i++) {
    for (j = 0; j < 4; j++) {
      suma[i] += matriz[i][j];
    }
    printf("La suma de la fila %d es: %d\n", i, suma[i]);
  }
}

void MultiplicacionVector(int matriz[5][4]) {
  int i, j,
      mult[4] = {
          1,
          1,
          1,
          1,
      };
  for (i = 0; i < 5; i++) {
    for (j = 0; j < 4; j++) {
      mult[j] *= matriz[i][j];
    }
  }
  for (i = 0; i < 4; i++) {
    printf("La multiplicacion de la columna %d es: %d\n", i, mult[i]);
  }
}
void ej3() {
  int matriz[5][8], i, j;
  for (i = 0; i < 5; i++) {
    for (j = 0; j < 8; j++) {
      matriz[i][j] = rand() % 100;
      printf("%d ", matriz[i][j]);
    }
  }
  MaxValor3(matriz);
}

void MaxValor3(int matriz[5][8]) {
  int max = matriz[0][0], pos[40], z = 0;
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 8; j++) {
      if (matriz[i][j] > max) {
        max = matriz[i][j];
        z = 0;
        pos[z] = i * 8 + j;
      }
      if (matriz[i][j] == max) {
        z++;
        pos[z] = i * 8 + j;
      }
    }
  }
  printf("El valor maximo es %d\n", max);
  printf("Se encuentra en la/s posicion/es:\n");
  for (int i = 0; i < z; i++) {
    printf("%d\n", pos[i]);
  }
  if (z > 1) {
    printf("Repitiendose: %d veces\n", z);
  } else {
    printf("Sin repetirse\n");
  }
}

void ej4() {
  int matriz[15][10], i, j, cant;
  for (i = 0; i < 15; i++) {
    for (j = 0; j < 10; j++) {
      matriz[i][j] = 0;
    }
  }
  do {
    do {

      printf("Ingrese agencia:");
      scanf("%d", &i);
    } while (i < 1 || i > 15);
    do {

      printf("Ingrese tour:");
      scanf("%d", &j);
    } while (j < 1 || j > 10);
    do {
      printf("Ingrese cantidad de personas:");
      scanf("%d", &cant);
    } while (cant < 0 || cant > 20);
    matriz[i - 1][j - 1] = cant;
    printf("Desea ingresar otro tour? 1-Si 0-No");
    flush(stdin);
    scanf("%d", &i);
  } while (i);
  for (i = 0; i < 10; i++) {
    for (j = 0; j < 15; j++) {
      cant = 0;
      cant += matriz[j][i];
    }
    printf("El tour %d tiene %d personas\n", i, cant);
  }
  MaxVend4(matriz);
}

void MaxVend4(int matriz[15][10]) {
  int max = 0, pos[15], z = 0;
  for (int i = 0; i < 15; i++) {
    for (int j = 0; j < 10; j++) {
      if (matriz[i][j] > max) {
        max = matriz[i][j];
        z = 0;
        pos[z] = i;
      }
      if (matriz[i][j] == max) {
        z++;
        pos[z] = i;
      }
    }
  }
  printf("La agencia que vendio mas tours es: %d\n", max);
  printf("Se encuentra en la/s posicion/es:\n");
  for (int i = 0; i < z; i++) {
    printf("%d\n", pos[i]);
  }
  if (z > 1) {
    printf("Repitiendose: %d veces\n", z);
  } else {
    printf("Sin repetirse\n");
  }
}

void ej5() {
  char sala[12][9];
  int fila, num, D = 12 * 9, R = 0, vacio = 1;
  printf("Hola");
  // 8 6 4 2 1 3 5 7 9
  for (int i = 0; i < 12; i++) {
    for (int j = 0; j < 9; j++) {
      sala[i][j] = 'D';
      printf("%c\t", sala[i][j]);
    }
    printf("\n");
  }
  printf("Ingrese la fila:");
  scanf("%d", &fila);
  printf("Ingrese numero de asiento:");
  scanf("%d", &num);
  while (fila >= 0) {
    fila = fila - 1;
    num = Asiento5(num);
    if (sala[fila][num] == 'D') {
      sala[fila][num] = 'R';
      R++;
      D--;
    } else if (sala[fila][num] == 'R') {
      printf("Asiento ocupado\n");
    }
    for (int i = 0; i < 12; i++) {
      for (int j = 0; j < 9; j++) {
        printf("%c\t", sala[i][j]);
      }
      printf("\n");
    }

    printf("Ingrese la fila:");
    scanf("%d", &fila);
    printf("Ingrese numero de asiento:");
    scanf("%d", &num);
  }
  printf("Asientos reservados: %d", R);
  printf("ASientos disponibles: %d", D);
  printf("Filas Vacias: \n");
  for (int i = 0; i < 12; i++) {
    for (int j = 0; j < 9; j++) {
      if (sala[i][j] == 'R') {
        vacio = 0;
      }
    }
    if (vacio) {
      printf("%d\n", i + 1);
    }
    vacio = 1;
  }
  MayorEspectadores5(sala);
}
int Asiento5(int num) {
  int asientos[9] = {8, 6, 4, 2, 1, 3, 5, 7, 9};
  int mapeo[9];

  for (int i = 0; i < 9; i++) {
    mapeo[asientos[i]] = i;
  }
  return mapeo[num];
}
void MayorEspectadores5(char sala[12][9]) {
  // filas con mayores espectadores
  int max = 0, pos[12], z = 0, k = 0;
  for (int i = 0; i < 12; i++) {
    for (int j = 0; j < 9; j++) {
      if (sala[i][j] == 'R') {
        max++;
      }
    }
    if (max > z) {
      z = max;
      pos[0] = i + 1;
      k = 0;
    }
    if (max == z) {
      pos[z] = i + 1;
      k++;
    }
    max = 0;
  }

  printf("Las fila/s con mas espectadores son: \n");
  for (int i = 0; i < k; i++) {
    printf("%d\n", pos[i]);
  }
}
void ej6() {}

void ej7() {}
void ej8() {}
void ej9() {}
void ej10() {}
void ej11() {}
