#include "myShortCuts.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char code[10];
  int cVenta;
} vtaDiaSuc;

typedef struct {
  int sucursal;
  int cTotal;
} sucursal;

void ActualizarSucursal(FILE *);
void contarVentas(char[], int *, int *, int *);
void ordenarVentas(int[], int[]);

int main() {
  FILE *f = fopen("archivosParaEjercicios/parciales/p5/sucursales.dat", "r+b");
  if (!f)
    Err("ERROR");
  ActualizarSucursal(f);
  fclose(f);
  return 0;
}

void ActualizarSucursal(FILE *f) {
  FILE *dSuc;
  sucursal suc;
  int cProd[5] = {0}, ventas[5] = {0};
  fread(&suc, sizeof(sucursal), 1, f);
  while (!feof(f)) {
    switch (suc.sucursal) {
    case 1:
      contarVentas("vtasdiasuc1.dat", &suc.cTotal, &cProd[suc.sucursal],
                   &ventas[suc.sucursal - 1]);
      break;
    case 2:
      contarVentas("vtasdiasuc2.dat", &suc.cTotal, &cProd[suc.sucursal - 1],
                   &ventas[suc.sucursal - 1]);
      break;
    case 3:
      contarVentas("vtasdiasuc3.dat", &suc.cTotal, &cProd[suc.sucursal - 1],
                   &ventas[suc.sucursal - 1]);
      break;

    case 4:
      contarVentas("vtasdiasuc4.dat", &suc.cTotal, &cProd[suc.sucursal - 1],
                   &ventas[suc.sucursal - 1]);
      break;
    case 5:
      contarVentas("vtasdiasuc5.dat", &suc.cTotal, &cProd[suc.sucursal - 1],
                   &ventas[suc.sucursal - 1]);
      break;
    }
    fseek(f, -sizeof(sucursal), SEEK_CUR);
    fwrite(&suc, sizeof(sucursal), 1, f);
    fseek(f, -sizeof(sucursal), SEEK_CUR);
    fread(&suc, sizeof(sucursal), 1, f);
  }
  ordenarVentas(cProd, ventas);
}
void contarVentas(char dir[17], int *total, int *cProd, int *ventas) {
  FILE *f = fopen(dir, "rb");
  vtaDiaSuc vds;
  fread(&vds, sizeof(vtaDiaSuc), 1, f);
  while (!feof(f)) {
    (*cProd)++;
    *ventas += vds.cVenta;
    fread(&vds, sizeof(vtaDiaSuc), 1, f);
  }
  fclose(f);
}

void ordenarVentas(int cProd[], int ventas[]) {
  int codes[5] = {1, 2, 3, 4, 5}, i, j, aux;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 3 - i; j++) {
      if (ventas[j] < ventas[j + 1]) {
        aux = ventas[j];
        ventas[j] = ventas[j + 1];
        ventas[j + 1] = aux;
        aux = cProd[j];
        cProd[j] = cProd[j + 1];
        cProd[j + 1] = aux;
        aux = codes[j];
        codes[j] = codes[j + 1];
        codes[j + 1] = aux;
      }
    }
  }
  printf("codigo\tventas\tproductos");
  for (i = 0; i < 5; i++) {
    printf("%d\t%d\t%d", codes[i], ventas[i], cProd[i]);
  }
}
