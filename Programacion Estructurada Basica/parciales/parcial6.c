#include "myShortCuts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char email[41];
  char apellido[21];
  int code;
} visitantes;

int buscar(FILE *f, char[]);
int BuscarCode(int[], int, int);
int Depurar(FILE *f, int[], int[]);
void ordenarYguardar(int[], int[], int);

int main() {
  FILE *f = fopen("visitantes.dat", "rb");
  int codes[1000] = {0}, cVisit[1000] = {0}, cant;
  if (!f) {
    Err("Error");
  }
  cant = Depurar(f, codes, cVisit);
  ordenarYguardar(codes, cVisit, cant);
  return 0;
}

int Depurar(FILE *f, int codes[], int cVisit[]) {
  FILE *aux = fopen("aux.dat", "w+b");
  if (!aux) {
    Err("Error");
  }
  visitantes v;
  int b = 0, i = 0, a = 0;
  fread(&v, sizeof(visitantes), 1, f);
  fwrite(&v, sizeof(visitantes), 1, aux);
  while (!feof(f)) {
    b = buscar(aux, v.email);
    if (b == -1) {
      fwrite(&v, sizeof(visitantes), 1, aux);
      a = BuscarCode(codes, v.code, i);
      if (a != -1) {
        codes[i] = v.code;
        cVisit[i]++;
        i++;
      } else {
        cVisit[a]++;
      }
    }
    fread(&v, sizeof(visitantes), 1, f);
  }
  fclose(f);
  fclose(aux);

  remove("sucursales.dat");
  rename("aux.dat", "sucursales.dat");
  return i;
}

void ordenarYguardar(int c[], int v[], int cant) {
  FILE *csv = fopen("resumen.csv", "wt");
  if (!csv) {
    Err("Error");
  }
  int aux, i, j;
  for (i = 0; i < cant - 1; i++) {
    for (j = 0; j < cant - 1 - i; j++) {
      if (v[j] < v[j + 1]) {
        aux = v[j];
        v[j] = v[j + 1];
        v[j + 1] = aux;
        aux = c[j];
        c[j] = c[j + 1];
        c[j + 1] = aux;
      }
    }
  }
  fprintf(csv, "codigo Postal;Cantidad Vendida\n");
  for (i = 0; i < cant; i++) {
    fprintf(csv, "%d;%d\n", c[i], v[i]);
  }
  fclose(csv);
}

int buscar(FILE *f, char email[]) {
  visitantes v;
  int b = -1;
  fread(&v, sizeof(visitantes), 1, f);
  while (!feof(f) && b == -1) {
    if (strcmp(v.email, email) == 0) {
      b = v.code;
    } else {
      fread(&v, sizeof(visitantes), 1, f);
    }
  }
  return b;
}

int BuscarCode(int codes[], int c, int i) {
  int a = -1, b = 0;
  while (b != i && a == -1) {
    if (codes[b] == c) {
      a = b;
    } else {
      b++;
    }
  }
  return a;
}
