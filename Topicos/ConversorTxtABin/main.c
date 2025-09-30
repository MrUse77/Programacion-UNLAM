#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARG_TXT 1
#define ARG_TIPO_TXT 2
#define ARG_BIN 3
#define ERR_ARCHIVO 1
#define ERR_MEM 2
#define ERR_LINEA_LARGA 3
#define TODO_OK 0
#define TAM_APYN 20
#define TAM_LINEA 501

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

typedef int (*TxtABin)(char *linea, const void *reg);
typedef bool (*EsErrorFatal)(int ret);
int generarEmpleadosBin(const char *bin);
int convTxtABin(const char *bin, const char *txt, size_t tamReg,
                TxtABin TxtABin, EsErrorFatal errorFatal);
int empleadoTxtVABin(char *linea, const void *reg);
int empleadoTxtFABin(char *linea, const void *reg);
bool esErrorFatalEmpleado(int cod);
int mostrarEmpleadosBin(const char *nomArchBin);
// ConversorTxtABin.exe Empleados.txt V Empleados.dat
int main(int argc, char *argv[]) {
  int ret;
  char tipoTxt = argv[ARG_TIPO_TXT][0];
  ret = convTxtABin(argv[ARG_BIN], argv[ARG_TXT], sizeof(Empleado),
                    tipoTxt == 'V' ? empleadoTxtVABin : empleadoTxtFABin,
                    esErrorFatalEmpleado);
  mostrarEmpleadosBin(argv[ARG_BIN]);
  return ret;
}

int convTxtABin(const char *bin, const char *txt, size_t tamReg,
                TxtABin TxtABin, EsErrorFatal errorFatal) {
  void *reg = malloc(1024);
  if (reg == NULL) {
    return ERR_MEM;
  }
  FILE *archTxt = fopen(txt, "rt");
  if (archTxt == NULL) {
    free(reg);
    return ERR_ARCHIVO;
  }
  FILE *archBin = fopen(bin, "wb");
  if (archBin == NULL) {
    free(reg);
    fclose(archTxt);
    return ERR_ARCHIVO;
  }
  char linea[TAM_LINEA];
  int ret = TODO_OK;
  while (!errorFatal(ret) && fgets(linea, TAM_LINEA, archTxt) != NULL) {
    ret = TxtABin(linea, reg);
    if (ret == TODO_OK) {
      fwrite(reg, tamReg, 1, archBin);
    }
  }
  fclose(archBin);
  fclose(archTxt);
  free(reg);
  return TODO_OK;
}

int empleadoTxtVABin(char *linea, const void *reg) {
  Empleado *empl = (Empleado *)reg;
  char *act = strchr(linea, '\n');

  if (!act) {
    return ERR_LINEA_LARGA;
  }

  *act = '\0';
  act = strrchr(linea, '|');
  sscanf(act + 1, "%f", &empl->sueldo);

  *act = '\0';
  act = strrchr(linea, '|');
  sscanf(act + 1, "%d/%d/%d", &empl->fIngr.dia, &empl->fIngr.mes,
         &empl->fIngr.anio);

  *act = '\0';
  act = strrchr(linea, '|');
  empl->sexo = *(act + 1);

  *act = '\0';
  act = strrchr(linea, '|');
  strcpy(empl->apyn, act + 1);

  *act = '\0';
  sscanf(linea, "%d", &empl->dni);

  return TODO_OK;
}
int empleadoTxtFABin(char *linea, const void *reg) {
  Empleado *empl = (Empleado *)reg;
  char *act = strchr(linea, '\n');

  if (!act) {
    return ERR_LINEA_LARGA;
  }

  *act = '\0';
  act -= 11;
  sscanf(act, "%f", &empl->sueldo);

  *act = '\0';
  act -= 8;
  sscanf(act, "%2d%2d%4d", &empl->fIngr.dia, &empl->fIngr.mes,
         &empl->fIngr.anio);

  *act = '\0';
  act--;
  empl->sexo = *act;

  *act = '\0';
  act -= TAM_APYN;
  strcpy(empl->apyn, act);

  *act = '\0';
  sscanf(linea, "%d", &empl->dni);

  return TODO_OK;
}
int generarEmpleadosTxt(const char *txt) {
  FILE *archBin = fopen(txt, "wt");
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
  for (int i = 0; i < 10; i++) {
    fprintf(archBin, "%d|%s|%c|%d/%d/%d|%.2f\n", vEmpls[i].dni, vEmpls[i].apyn,
            vEmpls[i].sexo, vEmpls[i].fIngr.dia, vEmpls[i].fIngr.mes,
            vEmpls[i].fIngr.anio, vEmpls[i].sueldo);
  }
  fclose(archBin);
  return TODO_OK;
}
bool esErrorFatalEmpleado(int cod) {
  switch (cod) {
  case ERR_LINEA_LARGA:
    return true;

  default:
    return false;
  }
}

int mostrarEmpleadosBin(const char *nomArchBin) {
  FILE *archBin = fopen(nomArchBin, "rb");

  if (!archBin) {
    return ERR_ARCHIVO;
  }

  Empleado empl;

  while (fread(&empl, sizeof(Empleado), 1, archBin) == 1) {
    printf("%d | %s | %c | %02d/%02d/%04d | %.2f\n", empl.dni, empl.apyn,
           empl.sexo, empl.fIngr.dia, empl.fIngr.mes, empl.fIngr.anio,
           empl.sueldo);
  }

  fclose(archBin);

  return TODO_OK;
}
