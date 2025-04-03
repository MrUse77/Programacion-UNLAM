#include "myShortCuts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char code[5];
  char desc[51];
  int stock;
} STOCK;
typedef struct {
  char code[5];
  int falta;
} FALTANTE;
void Carga(FILE *, int[2]);
STOCK BUSCAR(FILE *, char[5]);
FALTANTE BuscarFaltante(FILE *, char[5]);
void error(FILE *, FALTANTE);

int main() {
  FILE *f = fopen("archivosParaEjercicios/parciales/p4/Stock.dat", "r+b");
  int v[2]; //[0]=realizadas;[1]=no realizadas
  Carga(f, v);
  printf(
      "cantidad de ventas realizadas: %d, cantidad de ventas no realizadas: %d",
      v[0], v[1]);
  return 0;
}

void Carga(FILE *f, int v[2]) {
  FILE *f2 = fopen("archivosParaEjercicios/parciales/p4/Errores.dat", "w+b");
  STOCK s;
  FALTANTE fal;
  char code[5];
  int venta, a, faltante;

  printf("Ingrese codigo de articulo (SALE para finalizar) ");
  fsgets(code, 5);
  while (strcmp(code, "SALE") != 0) {
    s = BUSCAR(f, code);
    if (s.stock != -1) {
      printf("Ingrese cantidad vendida");
      scanfInt(&a, 0, 9999, 9999);
      if (s.stock < a) {
        fal = BuscarFaltante(f2, code);
        if (fal.falta != -1) {
          fal.falta += (a - s.stock);
        } else {
          fal.falta = a - s.stock;
          strcpy(fal.code, s.code);
        }
        v[1] += (a - s.stock);
        v[0] += s.stock;
        s.stock = 0;
        error(f2, fal);
      } else {
        s.stock -= a;
        v[0] += a;
      }
      fseek(f, -sizeof(STOCK), SEEK_CUR);
      fwrite(&s, sizeof(STOCK), 1, f);
    } else {
      printf("El codigo no existe\n");
    }
    printf("Ingrese codigo de articulo (SALE para finalizar) ");
    fsgets(code, 5);
  }
}

STOCK BUSCAR(FILE *f, char c[5]) {
  STOCK s;
  int b = 0;
  rewind(f);
  fread(&s, sizeof(STOCK), 1, f);
  while (!feof(f) && b == 0) {
    if (strcmp(c, s.code) == 0) {
      b = 1;
    }
  }
  if (b == 0) {
    s.stock = -1;
  }
  return s;
}

FALTANTE BuscarFaltante(FILE *f, char c[5]) {
  FALTANTE fal;
  int b = 0;
  rewind(f);

  fread(&fal, sizeof(STOCK), 1, f);
  while (!feof(f) && b == 0) {
    if (strcmp(c, fal.code) == 0) {
      b = 1;
    }
  }
  if (b == 0) {
    fal.falta = -1;
  }
  return fal;
}
void error(FILE *f, FALTANTE fal) {
  fseek(f, -sizeof(FALTANTE), SEEK_CUR);
  fwrite(&fal, sizeof(FALTANTE), 1, f);
}
