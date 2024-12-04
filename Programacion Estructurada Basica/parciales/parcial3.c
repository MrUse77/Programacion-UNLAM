#include <flush.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
  int CUE;
  char NyA[32];
  int anio;
} repitencia;
typedef struct {
  int CUE;
  char Nombre[41];
  char Provincia[41];
  int region;
} escuela;
typedef struct {
  int cue;
  int cAlumnos;
} informe;
void fsgets(char texto[], int largo) {
  int i = 0;
  flush(stdin);
  fgets(texto, largo, stdin);
  while (texto[i] != '\0') {
    if (texto[i] == '\n') {
      texto[i] = '\0';
    } else {
      i++;
    }
  }
}
int BuscarCue(int, escuela *, FILE *, char[]);
void InformarEscuela(escuela *, int);
void GenerarArchivo(int[], int[]);
void Ordenar(int[]);
void MaxCue(int[]);
void MaxRepitencia(int[]);
int main() {
  FILE *frep, *fEsc;
  frep = fopen("CANDIDATOS_REPITENCIA_2023.dat", "r+b"); // ordenado por cue
  fEsc = fopen("ESCUELAS.dat", "r+b");
  if (frep == NULL || fEsc == NULL) {
    printf("Error al abrir los archivos\n");
    exit(1);
  }
  repitencia rep, *r;
  escuela esc, *e;
  char prov[41];
  r = &rep;
  e = &esc;
  int cueAnt, pos, cAlumnos[2625] = {0}, cues[2625] = {0}, i = 0,
                   maxReg[15] = {0}, maxCue[15] = {0};
  printf("Ingrese Provincia: ");
  fsgets(prov, 41);
  fread(r, sizeof(repitencia), 1, frep);
  while (!feof(frep)) {
    cueAnt = r->CUE;
    pos = BuscarCue(cueAnt, e, fEsc, prov);
    if (pos == -1) {
      printf("No se encontro la escuela\n");
    } else {
      cues[i] = cueAnt;
      do {
        cAlumnos[i]++;
        fread(r, sizeof(repitencia), 1, frep);
      } while (!feof(frep) && cueAnt == r->CUE);
      if (cAlumnos[i] > maxReg[e->region]) {
        maxReg[e->region] = cAlumnos[i];
        maxCue[e->region] = cueAnt;
      }
      InformarEscuela(e, cAlumnos[i]);
      fread(r, sizeof(repitencia), 1, frep);
      i++;
    }
    Ordenar(cAlumnos);
    GenerarArchivo(cAlumnos, cues);
    MaxRepitencia(maxCue);
  }
  return 0;
}
int BucarCue(int cue, escuela *e, FILE *fEsc, char prov[]) {
  int pos = -1;
  rewind(fEsc);
  fread(e, sizeof(escuela), 1, fEsc);
  while (!feof(fEsc) && pos == -1) {
    if (e->CUE == cue && strcmp(e->Provincia, prov) == 0) {
      pos = ftell(fEsc) - sizeof(escuela);
    } else {
      fread(e, sizeof(escuela), 1, fEsc);
    }
  }
  return pos;
}
void InformarEscuela(escuela *e, int cAlumnos) {
  if (cAlumnos > 0) {
    printf("CUE: %d\n", e->CUE);
    printf("Nombre: %s\n", e->Nombre);
    printf("Provincia: %s\n", e->Provincia);
    printf("Region: %d\n", e->region);
    printf("Cantidad de alumnos: %d\n", cAlumnos);
    printf("_____________________________________\n");
  }
}
void GenerarArchivo(int c[], int cues[]) {
  FILE *f;
  informe i;
  f = fopen("informe.dat", "wb");
  if (f == NULL) {
    printf("Error al abrir el archivo\n");
    exit(1);
  }
  for (int j = 0; j < 2625; j++) {
    if (c[j] < 0) {
      i.cue = cues[j];
      i.cAlumnos = c[j];
      fwrite(&i, sizeof(informe), 1, f);
    }
  }
  fclose(f);
}
void Ordenar(int cAlumnos[]) {
  int i, j, aux;
  for (i = 0; i < 2625; i++) {
    for (j = 0; j < 2625 - i - 1; j++) {
      if (cAlumnos[i] > cAlumnos[j]) {
        aux = cAlumnos[i];
        cAlumnos[i] = cAlumnos[j];
        cAlumnos[j] = aux;
      }
    }
  }
}
void MaxCue(int c[]) {
  for (int i = 0; i < 15; i++) {
    printf("Region: %d\n", i);
    printf("CUE: %d\n", c[i]);
    printf("_____________________________________\n");
  }
}
