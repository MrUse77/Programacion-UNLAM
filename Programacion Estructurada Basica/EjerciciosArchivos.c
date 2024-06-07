#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
void flush(FILE *stream) {
  int c;
  if (stream != stdin) {
    // generar error
    exit(EXIT_FAILURE);
  }
  do {
    c = getchar();
  } while (c != EOF && c != '\n');
}
void fsgetsP(char *texto, int largo) {
  int i = 0;
  flush(stdin);
  fgets(texto, largo, stdin);
  while (*(texto + i) != '\0') {
    if (*(texto + i) == '\n') {
      *(texto + i) = '\0';
    } else {
      i++;
    }
  }
}
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
typedef struct {
  int dni;
  char nombre[81];
  int nota1, nota2;
  float promedio;
} Alumno;
void ej1();
void LECTURA(FILE *);
void ej2();
typedef struct {
  int mes;
  int anio;
  int dia;
  int codigo;
  float importe;
} venta;
void ej3();
void crearVentas3();
typedef struct {
  int code;
  float price;
  char desc[51];
} precios4;
void ej4();

int main() {
  srand(time(NULL));
  int opcion;
  printf("Ejercicios de Archivos\n");
  printf("Elija el ejercicio: \n");
  scanf("%d", &opcion);
  switch (opcion) {
  case 1:
    ej1();
    break;
  case 2:
    ej2();
    break;
  case 3:
    ej3();
    break;
  }
  return 0;
}

void ej1() {
  Alumno alumno[5] = {{44937522, "Agustin Doricich", 10, 10},
                      {24387232, "Mash Burndead", 1, 1},
                      {1238912, "Maximiliano Barata Vaz", 9, 10},
                      {12328, "El Gabi", 7, 6},
                      {129378291, "lolero puto", 2, 1}};
  FILE *f;
  int i;
  f = fopen("../archivosParaEjercicios/alumnos.dat", "wb");
  if (f == NULL) {
    printf("Error al abrir el archivo");
    exit(1);
  }
  for (i = 0; i < 5; i++) {
    fwrite(&alumno[i], sizeof(Alumno), 1, f);
  }
  fclose(f);
  LECTURA(f);
}
void LECTURA(FILE *f) {
  Alumno alumno[5];
  int i = 0;
  f = fopen("../archivosParaEjercicios/alumnos.dat", "rb");
  if (f == NULL) {
    printf("Error al abrir el archivo");
    exit(1);
  }
  fread(&alumno[i], sizeof(Alumno), 1, f);
  while (!feof(f)) {
    printf("DNI: %d\n", alumno[i].dni);
    printf("Nombre: %s\n", alumno[i].nombre);
    printf("Nota 1: %d\n", alumno[i].nota1);
    printf("Nota 2: %d\n", alumno[i].nota2);
    printf("Promedio: %.2f\n", alumno[i].promedio);
    i++;
    fread(&alumno[i], sizeof(Alumno), 1, f);
  }
  fclose(f);
}
void ej2() {
  FILE *f, *aprob, *prom, *reprob;
  Alumno alumno[5];
  int i = 0;
  f = fopen("../archivosParaEjercicios/alumnos.dat", "rb");
  if (f == NULL) {
    printf("Error al abrir el archivo");
    exit(1);
  }
  aprob = fopen("../archivosParaEjercicios/APROBADOS.dat", "wb");
  if (aprob == NULL) {
    printf("Error al abrir el archivo");
    exit(1);
  }
  prom = fopen("../archivosParaEjercicios/PROMOCIONADOS.dat", "wb");
  if (prom == NULL) {
    printf("Error al abrir el archivo");
    exit(1);
  }
  reprob = fopen("../archivosParaEjercicios/REPROBADOS.dat", "wb");
  if (reprob == NULL) {
    printf("Error al abrir el archivo");
    exit(1);
  }
  fread(alumno, sizeof(Alumno), 1, f);
  while (!feof(f)) {
    if ((alumno[i].nota1 >= 4 && alumno[i].nota2 >= 4) &&
        (alumno[i].nota1 < 7 || alumno[i].nota2 < 7)) {
      printf("%d\n", alumno[i].dni);
      fwrite(&alumno[i], sizeof(Alumno), 1, aprob);
    }
    if (alumno[i].nota1 >= 7 && alumno[i].nota2 >= 7) {
      printf("%d\n", alumno[i].dni);
      fwrite(&alumno[i], sizeof(Alumno), 1, prom);
    }
    if (alumno[i].nota1 < 4 || alumno[i].nota2 < 4) {
      printf("%d\n", alumno[i].dni);
      fwrite(&alumno[i], sizeof(Alumno), 1, reprob);
    }
    i++;
    fread(&alumno[i], sizeof(Alumno), 1, f);
  }
  fclose(aprob);
  fclose(prom);
  fclose(reprob);
  fclose(f);
  aprob = fopen("../archivosParaEjercicios/APROBADOS.dat", "rb");
  prom = fopen("../archivosParaEjercicios/PROMOCIONADOS.dat", "rb");
  reprob = fopen("../archivosParaEjercicios/REPROBADOS.dat", "rb");

  i = 0;

  printf("PROMOCIONADOS\n");
  printf("DNI \t NOMBRE \t NOTA 1 \t NOTA 2 \t PROMEDIO\n");
  fread(&alumno[i], sizeof(Alumno), 1, prom);
  while (!feof(prom)) {
    printf("%d \t %s \t %d \t %d \t %.2f\n", alumno[i].dni, alumno[i].nombre,
           alumno[i].nota1, alumno[i].nota2, alumno[i].promedio);
    i++;
    fread(&alumno[i], sizeof(Alumno), 1, prom);
  }
  printf("APROBADOS\n");
  printf("DNI \t NOMBRE \t NOTA 1 \t NOTA 2 \t PROMEDIO\n");
  fread(&alumno[i], sizeof(Alumno), 1, aprob);
  while (!feof(aprob)) {
    printf("%d \t %s \t %d \t %d \t %.2f\n", alumno[i].dni, alumno[i].nombre,
           alumno[i].nota1, alumno[i].nota2, alumno[i].promedio);
    i++;
    fread(&alumno[i], sizeof(Alumno), 1, aprob);
  }
  printf("REPROBADOS\n");
  printf("DNI \t NOMBRE \t NOTA 1 \t NOTA 2 \t PROMEDIO\n");
  fread(&alumno[i], sizeof(Alumno), 1, reprob);
  while (!feof(reprob)) {
    printf("%d \t %s \t %d \t %d \t %.2f\n", alumno[i].dni, alumno[i].nombre,
           alumno[i].nota1, alumno[i].nota2, alumno[i].promedio);
    i++;
    fread(&alumno[i], sizeof(Alumno), 1, reprob);
  }
  fclose(aprob);
  fclose(prom);
  fclose(reprob);
}
// Se pide ingresar un rango de años y mostrar un cuadro ventas realizadas en
// cada mes de cada año. Por ejemplo,
//  si se ingresa desde 2020 a 2022 debe mostrar lo siguiente:
//      Mes 1 Mes 2 Mes 3 ……. Mes 12
// 2020  xxx    xxx   xxx       xxx
// 2021  xxx    xxx   xxx       xxx
// 2022  xxx    xxx   xxx       xxx
// Una vez configurados los años debe poder cambiarse la vista para en lugar de
// visualizar cantidad de ventas, ver el
//  detalle de importe por mes/año. El usuario podrá alternar las vistas las
//  veces que quiera. Agregar una opción para finalizar el programa.
void ej3() {
  FILE *f;
  venta v[100], *vp;
  vp = v;
  int anio1, anio2;
  int ventas[12][10];
  float importes[12][10];
  crearVentas3();
  f = fopen("../archivosParaEjercicios/ventas.dat", "rb");
  if (f == NULL) {
    printf("Error al abrir el archivo");
    exit(1);
  }
  printf("Ingrewsa un rango de anios: ");
  printf("Desde: ");
  scanf("%d", &anio1);
  printf("Hasta: ");
  scanf("%d", &anio2);
  for (int i = 0; i < 12; i++) {
    for (int j = anio1; j < anio2 + 1; j++) {
      ventas[i][j - anio1] = 0;
      importes[i][j - anio1] = 0;
    }
  }
  fread(v, sizeof(venta), 1, f);
  while (!feof(f)) {
    if (vp->anio >= anio1 && vp->anio <= anio2) {
      ventas[vp->mes - 1][vp->anio - anio1]++;
      importes[vp->mes - 1][vp->anio - anio1] += vp->importe;
    }
    fread(v, sizeof(venta), 1, f);
  }
  fread(v, sizeof(venta), 1, f);

  fclose(f);
  printf("Detalle de ventas por mes/año\n\t");
  for (int mes = 1; mes <= 12; mes++) {
    printf("Mes %2d\t", mes);
  }
  printf("\n");
  for (int i = 0; i < anio2 - anio1 + 1; i++) {
    printf("%4d", anio1 + i);
    for (int j = 0; j < 12; j++) {
      printf(" \t%d", ventas[j][i]);
    }
    printf("\n");
  }
  printf("Desea ver el detalle de importes por mes/año? (s/n): ");
  char c;
  scanf(" %c", &c);
  if (c == 's') {
    printf("Detalle de importes por mes/año\n\t");
    for (int mes = 1; mes <= 12; mes++) {
      printf("Mes %2d\t", mes);
    }
    printf("\n");
    for (int i = 0; i < anio2 - anio1 + 1; i++) {
      printf("%4d\t", anio1 + i);
      for (int j = 0; j < 12; j++) {
        printf("%.2f\t", importes[j][i]);
      }
      printf("\n");
    }
  }
  int opcion;
  do {
    printf("Seleccione la opcion que quiera: \n 1. Ver el detalle de importes "
           "por mes/año\n 2. Ver el detalle de ventas mes/año \n 3. Finalizar "
           "el programa\n");
    scanf("%d", &opcion);
    switch (opcion) {
    case 1:
      printf("Detalle de importes por mes/año\n\t");
      for (int mes = 1; mes <= 12; mes++) {
        printf("Mes %2d\t", mes);
      }
      printf("\n");
      for (int i = 0; i < anio2 - anio1 + 1; i++) {
        printf("%4d\t", anio1 + i);
        for (int j = 0; j < 12; j++) {
          printf("%.2f\t", importes[j][i]);
        }
        printf("\n");
      }
      break;
    case 2:
      printf("Detalle de ventas por mes/año\n\t");
      for (int mes = 1; mes <= 12; mes++) {
        printf("Mes %2d\t", mes);
      }
      printf("\n");
      for (int i = 0; i < anio2 - anio1 + 1; i++) {
        printf("%4d", anio1 + i);
        for (int j = 0; j < 12; j++) {
          printf(" \t%d", ventas[j][i]);
        }
        printf("\n");
      }
      break;
    }

  } while (opcion != 3);
}
void crearVentas3() {
  FILE *f;
  venta v[100];
  f = fopen("../archivosParaEjercicios/ventas.dat", "wb");
  if (f == NULL) {
    printf("Error al abrir el archivo");
    exit(1);
  }
  for (int i = 0; i < 100; i++) {
    v->mes = 1 + rand() % 12;
    v->anio = 2014 + rand() % (2023 - 2014 + 1);
    v->dia = 1 + rand() % 31;
    v->codigo = 1 + rand() % 1000;
    v->importe =
        100.00 + (float)rand() / ((float)RAND_MAX / (1000.00 - 100.00));
    fwrite(v, sizeof(venta), 1, f);
  }
  fclose(f);
}
