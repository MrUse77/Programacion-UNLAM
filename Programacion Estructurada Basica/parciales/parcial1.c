#include <flush.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
  char nombre[51];
  int total;
  int precio;
} TOUR; // ordenado por nombre;
typedef struct {
  char agencia[26];
  char nomTour[51];
  int trimestre;
  int vendido;
} VENTAS; // ordenado por agencia
typedef struct {
  char agencia[26];
  int cTours;
  int total;
  float prom;
} TOTAL;
void err(char[]);
void Informe(FILE *, int[][4]);
int BuscarTour(TOUR *, FILE *, char[]);
int main() {
  FILE *f1, *f2, *f3, *f4;
  f1 = fopen("archivosParaEjercicios/parciales/p1/tours.dat", "r+b");
  f2 = fopen("archivosParaEjercicios/parciales/p1/ventas25.dat", "r+b");
  f3 = fopen("archivosParaEjercicios/parciales/p1/agencia.dat", "wb");
  f4 = fopen("archivosParaEjercicios/parciales/p1/error.dat", "wb");
  if (!f1 || !f2 || !f3 || !f4)
    err("Error al abrir archivos");
  TOUR t, *tp;
  VENTAS v, *vp;
  TOTAL total, *top;
  tp = &t;
  vp = &v;
  top = &total;
  int pos = 0, i = 0, rec[35][4];
  char agenAnt[51];
  fread(vp, sizeof(VENTAS), 1, f2);
  while (!feof(f2)) {
    strcpy(agenAnt, vp->agencia);
    do {
      pos = BuscarTour(tp, f1, vp->nomTour);
      if (pos == -1) {
        fwrite(vp, sizeof(VENTAS), 1, f4);
      } else {
        total.cTours += vp->vendido;
        total.total += vp->vendido * tp->precio;
        t.total += vp->vendido;
        fseek(f1, -sizeof(TOUR), SEEK_CUR);
        fwrite(tp, sizeof(TOUR), 1, f1);
        rec[pos][vp->trimestre - 1] += vp->vendido * t.precio;
      }
      fread(vp, sizeof(VENTAS), 1, f2);
    } while (strcmp(vp->agencia, agenAnt) == 0 && !feof(f2));
    strcpy(total.agencia, agenAnt);
    total.prom = (float)total.total / total.cTours;
    fwrite(top, sizeof(TOTAL), 1, f3);
    i++;
    fread(vp, sizeof(VENTAS), 1, f2);
  }
  Informe(f1, rec);
  fclose(f1);
  fclose(f2);
  fclose(f3);
  fclose(f4);
  return 0;
}
void Informe(FILE *f1, int rec[][4]) {
  TOUR t;
  int i = 0, j = 0;
  char tourAnt[51];
  rewind(f1);
  fread(&t, sizeof(TOUR), 1, f1);
  printf("Tour\tTrimestre 1\tTrimestre 2\tTrimestre 3\tTrimestre 4\n");
  while (!feof(f1)) {
    strcpy(tourAnt, t.nombre);
    do {
      printf("%s\t", t.nombre);
      for (j = 0; j < 4; j++) {
        printf("%d\t", rec[i][j]);
      }
      printf("\n");
    } while (strcmp(t.nombre, tourAnt) == 0 && !feof(f1));
    fread(&t, sizeof(TOUR), 1, f1);
  }
}
int BuscarTour(TOUR *tp, FILE *f1, char tour[]) {
  int pos = -1, i = 0;
  rewind(f1);
  fread(tp, sizeof(TOUR), 1, f1);
  while (!feof(f1) && pos == -1) {
    if (strcmp(tp->nombre, tour) == 0)
      pos = i;
    else {
      fread(tp, sizeof(VENTAS), 1, f1);
      i++;
    }
  }
  return pos;
}
void err(char msg[]) {
  printf("%s\n", msg);
  exit(1);
}
