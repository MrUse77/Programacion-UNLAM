#include <stdio.h>
#include <stdlib.h>

#define ARG_BIN 1
#define ARG_TXT 2
#define ARG_TIPO_TXT 3
#define ERR_ARCHIVO 1
#define ERR_MEM 2
#define TODO_OK 0
#define TAM_APYN 20

typedef struct {
  int dia;
  int mes;
  int anio;
} Fecha;

typedef struct {
  int dni;
  char apyn[TAM_APYN + 1];
  char sexo;
  Fecha fIngr;
  float sueldo;
} Empleado;

typedef void (*BinATxt)(const void *reg, FILE *archTxt);

int generarEmpleadosBin(const char *bin);
int convBinATexto(const char *bin, const char *txt, size_t tamReg,
                  BinATxt BinATxt);
void empeadoBinATxt(const void *reg, FILE *archTxt);
void empeadoBinATxtF(const void *reg, FILE *archTxt);
// ConversorBinATxt.exe Empleados.dat Empleados.txt V
int main(int argc, char *argv[]) {
  int ret;
  ret = generarEmpleadosBin(argv[ARG_BIN]);
  char tipoTxt = argv[ARG_TIPO_TXT][0];
  ret = convBinATexto(argv[ARG_BIN], argv[ARG_TXT], sizeof(Empleado),
                      tipoTxt == 'V' ? empeadoBinATxt : empeadoBinATxtF);
  return ret;
}

int convBinATexto(const char *bin, const char *txt, size_t tamReg,
                  BinATxt BinATxt) {
  void *reg = malloc(1024);
  if (reg == NULL) {
    return ERR_MEM;
  }
  FILE *archBin = fopen(bin, "rb");
  if (archBin == NULL) {
    free(reg);
    return ERR_ARCHIVO;
  }
  FILE *archTxt = fopen(txt, "wt");
  if (archTxt == NULL) {
    free(reg);
    fclose(archBin);
    return ERR_ARCHIVO;
  }
  while (fread(reg, tamReg, 1, archBin)) {
    BinATxt(reg, archTxt);
  }
  fclose(archBin);
  fclose(archTxt);
  free(reg);
  return TODO_OK;
}

void empeadoBinATxt(const void *reg, FILE *archTxt) {
  const Empleado *emp = (const Empleado *)reg;
  fprintf(archTxt, "%d|%s|%c|%d/%d/%d|%.2f\n", emp->dni, emp->apyn, emp->sexo,
          emp->fIngr.dia, emp->fIngr.mes, emp->fIngr.anio, emp->sueldo);
}
void empeadoBinATxtF(const void *reg, FILE *archTxt) {
  const Empleado *emp = (const Empleado *)reg;
  fprintf(archTxt, "%08d%-20s%c%02d%02d%04d%011.2f\n", emp->dni, emp->apyn,
          emp->sexo, emp->fIngr.dia, emp->fIngr.mes, emp->fIngr.anio,
          emp->sueldo);
}
int generarEmpleadosBin(const char *bin) {
  FILE *archBin = fopen(bin, "wb");
  if (archBin == NULL) {
    return ERR_ARCHIVO;
  }
  Empleado vEmpls[10] = {
      {12345678, "Juan Perez", 'M', {1, 1, 2020}, 500000.00},
      {23456789, "Ana Gomez", 'F', {15, 3, 2019}, 600000.50},
      {34567890, "Luis Lopez", 'M', {20, 5, 2021}, 550000.75},
      {45678901, "Maria Garcia", 'F', {10, 7, 2018}, 700000.25},
      {56789012, "Carlos Martinez", 'M', {30, 9, 2022}, 480000.00},
      {67890123, "Laura Rodriguez", 'F', {5, 11, 2020}, 520000.80},
      {78901234, "Pedro Sanchez", 'M', {12, 12, 2017}, 580000.90},
      {89012345, "Sofia Torres", 'F', {25, 2, 2023}, 620000.10},
      {90123456, "Javier Ramirez", 'M', {18, 4, 2016}, 540000.30},
      {12345679, "Lucia Fernandez", 'F', {22, 6, 2024}, 610000.40}};
  fwrite(&vEmpls, sizeof(Empleado), 10, archBin);
  fclose(archBin);
  return TODO_OK;
}
