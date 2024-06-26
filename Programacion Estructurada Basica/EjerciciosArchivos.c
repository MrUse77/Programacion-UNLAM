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
} PRECIOS;
void ej4();
typedef struct {
  int code;
  float price;
  char desc[51];
} PRODUCTOS;
void ej5();
int buscar5(FILE *, int);
void ej6();
typedef struct {
  int code;
  int stock;
  char desc[51];
} STOCK;
typedef struct {
  int code;
  int stockF;
} FALTANTES;
void ej7();
int buscar7(FILE *, int);
int buscar7Faltante(FILE *, int);
typedef struct {
  char code[11];
  int day;
  int vuelo;
  float price;
  int pasajeros;
} VUELOS;
typedef struct {
  int dni;
  int vuelo;
} PASAJEROS;
void ej8();
typedef struct {
  int dni;
  char AyN[21];
  int Pagado;
} INSCRIPTOS;
typedef struct {
  int dni;
  int tel;
  char AyN[21];
} INTERESADOS;
void ej9();
int buscar9(FILE *, int);
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
/*Realizar un programa que permite actualizar una lista de precios en forma
masiva, ingresando un porcentaje de incremento. El archivo se llama precios.dat
y fue generado utilizando la siguiente estructura: • Código (entero) • Precio
(float) • Descripción (de hasta 50 caracteres)*/
void ej4() {
  FILE *f = fopen("../archivosParaEjercicios/PRECIOS.dat", "r + b");
  if (f == NULL) {
    printf("Error al abrir el archivo");
    exit(1);
  }
  PRECIOS precios, *p;
  p = &precios;
  float porcentaje;
  printf("Ingrese el porcentaje de incremento: ");
  scanf("%f", &porcentaje);
  fread(&precios, sizeof(PRECIOS), 1, f);
  while (!feof(f)) {
    printf("Codigo: %d\n", p->code);
    printf("Precio Sin actualizar: %.2f\n", p->price);
    p->price += p->price * porcentaje / 100;
    printf("Precio actualizado: %.2f\n", p->price);
    printf("Descripcion: %s\n", p->desc);
    long desp = -sizeof(PRECIOS);
    fseek(f, desp, SEEK_CUR); // Retrocedo 60 bytes
    fwrite(p, sizeof(PRECIOS), 1,
           f);             // Avanzo 60 bytes (escribo en la misma posicion)
    fseek(f, 0, SEEK_CUR); // Retrocedo 60 bytes
    fread(p, sizeof(PRECIOS), 1,
          f); // Avanzo 60 bytes (leo la posicion siguiente a la que escribi)//
  }
}
void ej5() {
  FILE *f = fopen("../archivosParaEjercicios/PRODUCTOS.dat", "r + b");
  if (f == NULL) {
    printf("Error al abrir el archivo");
    exit(1);
  }
  PRODUCTOS productos, *p;
  p = &productos;
  int code;
  printf("Codigos");
  fread(p, sizeof(PRODUCTOS), 1, f);
  while (!feof(f)) {
    printf("%d\n", p->code);
    fread(p, sizeof(PRODUCTOS), 1, f);
  }
  rewind(f);
  printf("Ingrese el codigo del producto a buscar: ");
  scanf("%d", &code);
  while (code != 0) {
    int pos = buscar5(f, code);
    if (pos == -1) {
      printf("No se encontro el producto\n");
    } else {
      fseek(f, pos * sizeof(PRODUCTOS), SEEK_SET);
      printf("Ingrese el nuevo precio: ");
      scanf("%f", &p->price);
      fwrite(p, sizeof(PRODUCTOS), 1, f);
    }
    printf("Ingrese el codigo del producto a buscar: ");
    scanf("%d", &code);
  }
  // exportar a csv
  FILE *csv = fopen("../archivosParaEjercicios/productos.csv", "wt");
  if (csv == NULL) {
    printf("Error al abrir el archivo");
    exit(1);
  }
  fprintf(csv, "Codigo; Precio; Descripcion\n");
  fread(&productos, sizeof(PRODUCTOS), 1, f);
  while (!feof(f)) {
    fprintf(csv, "%d; %.2f; %s\n", p->code, p->price, p->desc);
    fread(&productos, sizeof(PRODUCTOS), 1, f);
  }
  fclose(f);
  fclose(csv);
}
int buscar5(FILE *f, int code) {
  PRODUCTOS productos, *p;
  p = &productos;
  int i = -1;
  fread(p, sizeof(PRODUCTOS), 1, f);
  while (!feof(f)) {
    printf("Codigo: %d\n", p->code);
    printf("Codigo a buscar: %d\n", code);
    if (p->code == code) {
      i = ftell(f) / sizeof(PRODUCTOS);
    }
    fread(p, sizeof(PRODUCTOS), 1, f);
  }
  return i;
}
void ej6() {
  FILE *f = fopen("../archivosParaEjercicios/PRODUCTOS.dat", "r+b"),
       *f2 = fopen("../archivosParaEjercicios/PRODUCTOS2.dat", "w+b");
  PRODUCTOS productos, *p;

  p = &productos;
  int code;
  printf("Codigos");
  fread(&productos, sizeof(PRODUCTOS), 1, f);
  while (!feof(f)) {
    printf("%d\n", p->code);
    fread(&productos, sizeof(PRODUCTOS), 1, f);
  }
  printf("Ingrese el codigo para buscarlo y eliminarlo: ");
  scanf("%d", &code);
  while (code != 0) {
    rewind(f);
    fread(p, sizeof(PRODUCTOS), 1, f);
    while (!feof(f)) {
      if (p->code != code) {
        fwrite(p, sizeof(PRODUCTOS), 1, f2);
      }
      fread(p, sizeof(PRODUCTOS), 1, f);
    }
    fclose(f);
    fclose(f2);
    remove("../archivosParaEjercicios/PRODUCTOS.dat");
    rename("../archivosParaEjercicios/PRODUCTOS2.dat",
           "../archivosParaEjercicios/PRODUCTOS.dat");
    f = fopen("../archivosParaEjercicios/PRODUCTOS.dat", "r+b");
    f2 = fopen("../archivosParaEjercicios/PRODUCTOS2.dat", "w+b");
    printf("Codigos");
    rewind(f);
    fread(p, sizeof(PRODUCTOS), 1, f);
    while (!feof(f)) {
      printf("%d\n", p->code);
      fread(p, sizeof(PRODUCTOS), 1, f);
    }

    printf("Ingrese el codigo para buscarlo y eliminarlo: ");
    scanf("%d", &code);
  }
  fclose(f);
  fclose(f2);
}
void ej7() {
  FILE *FSTOCK = fopen("../archivosParaEjercicios/STOCK.dat", "r+b"),
       *FALTANTE = fopen("../archivosParaEjercicios/Faltantes.dat", "w+b");
  if (!FSTOCK || !FALTANTE) {
    perror("Error al abrir los archivos");
    return;
  }
  STOCK stock, *s;
  FALTANTES faltante = {0, 0}, *f;
  s = &stock;
  f = &faltante;
  int code, venta;
  printf("CODGIOS: \n");
  fread(s, sizeof(STOCK), 1, FSTOCK);
  while (!feof(FSTOCK)) {
    s->stock = rand() % 100;
    fwrite(s, sizeof(STOCK), 1, FSTOCK);
    fseek(FSTOCK, 0, SEEK_CUR);
    printf("%d\n", s->code);
    printf("%d\n", s->stock);
    fread(s, sizeof(STOCK), 1, FSTOCK);
  }
  printf("Ingrese el codigo del producto a buscar: ");
  scanf("%d", &code);
  while (code != 0) {
    int pos = buscar7(FSTOCK, code);
    if (pos == -1) {
      printf("No se encontro el producto\n");
    } else {
      fseek(FSTOCK, (pos - 1) * sizeof(STOCK), SEEK_SET);
      fread(s, sizeof(STOCK), 1, FSTOCK);

      printf("Ingrese la cantidad que se vendio: ");
      venta = rand() % 100;
      printf("%d\n", venta);

      if (venta > s->stock) {
        int posF = buscar7Faltante(FALTANTE, code);

        printf("%d\n", posF);
        if (posF == -1) {
          fseek(FALTANTE, 0, SEEK_END);
        } else {
          fseek(FALTANTE, (posF - 1) * sizeof(FALTANTES), SEEK_SET);
        }
        printf("Posicion: %ld\t\n", ftell(FALTANTE));

        if (fread(f, sizeof(FALTANTES), 1, FALTANTE) == 1) {
          fseek(FALTANTE, -sizeof(FALTANTES), SEEK_CUR);
          printf("Posicion: %ld\t,stockF: %d\n", ftell(FALTANTE), f->stockF);
        } else {
          f->stockF = 0;
          f->code = code;

          fseek(FALTANTE, 0, SEEK_CUR);
        }
        printf("Posicion: %ld\t,stockF: %d\n", ftell(FALTANTE), f->stockF);
        f->stockF += venta - s->stock;
        s->stock = 0;
        f->code = code;

        printf("Posicion: %ld\t,stockF: %d\n", ftell(FALTANTE), f->stockF);

        fwrite(f, sizeof(FALTANTES), 1, FALTANTE);
        fflush(FALTANTE);
      } else {
        s->stock -= venta;
      }
      fseek(FSTOCK, -sizeof(STOCK), SEEK_CUR);
      printf("Posicion: %ld\t,stock: %d\n", ftell(FSTOCK), s->stock);
      fwrite(s, sizeof(STOCK), 1, FSTOCK);
      fflush(FSTOCK);
      printf("Posicion: %ld\n", ftell(FSTOCK));
    }
    printf("Ingrese el codigo del producto a buscar: ");

    scanf("%d", &code);
  }
  rewind(FSTOCK);
  fread(s, sizeof(STOCK), 1, FSTOCK);
  if (!feof(FSTOCK)) {
    printf("STOCK ACTUALIZADO\n");
  }
  while (!feof(FSTOCK)) {
    printf("Codigo: %d\n", s->code);
    printf("Stock: %d\n", s->stock);
    printf("Descripcion: %s\n", s->desc);
    fread(s, sizeof(STOCK), 1, FSTOCK);
  }
  rewind(FALTANTE);
  fread(f, sizeof(FALTANTES), 1, FALTANTE);
  if (!feof(FALTANTE))
    printf("FALTANTES\n");
  while (!feof(FALTANTE)) {
    printf("Codigo: %d\n", f->code);
    printf("Stock faltante: %d\n", f->stockF);
    fread(f, sizeof(FALTANTES), 1, FALTANTE);
  }
}
int buscar7(FILE *FSTOCK, int code) {
  STOCK stock, *s;
  s = &stock;
  int i = -1;

  rewind(FSTOCK);
  fread(s, sizeof(STOCK), 1, FSTOCK);
  while (!feof(FSTOCK)) {
    if (s->code == code) {
      i = ftell(FSTOCK) / sizeof(STOCK);
    }
    fread(s, sizeof(STOCK), 1, FSTOCK);
  }
  return i;
}
int buscar7Faltante(FILE *FALTANTE, int code) {
  FALTANTES faltante, *f;
  f = &faltante;
  int i = -1;

  rewind(FALTANTE);
  fread(f, sizeof(FALTANTES), 1, FALTANTE);
  while (!feof(FALTANTE)) {
    printf("Codigo: %d\n", f->code);
    if (f->code == code) {
      i = ftell(FALTANTE) / sizeof(FALTANTES);
    }
    fread(f, sizeof(FALTANTES), 1, FALTANTE);
  }
  return i;
}
void ej8() {
  FILE *f = fopen("../archivosParaEjercicios/PASAJEROS.dat", "r+b"),
       *f2 = fopen("../archivosParaEjercicios/VUELOS.dat", "r+b");
  if (!f || !f2) {
    perror("Error al abrir los archivos");
    exit(1);
  }
  PASAJEROS pasajeros, *p;
  VUELOS vuelos, *v;
  p = &pasajeros;
  v = &vuelos;
  fread(v, sizeof(VUELOS), 1, f2);
  while (!feof(f2)) {
    v->pasajeros = 0;
    printf("Codigo de vuelo: %s\n", v->code);
    if (strcmp(v->code, "AERO1") == 0) {

      fread(p, sizeof(PASAJEROS), 1, f);
      while (!feof(f)) {
        printf("Codigo: %d\n", p->vuelo);
        if (v->vuelo == p->vuelo) {
          v->pasajeros++;
          printf("Pasajeros: %d\n", v->pasajeros);
        }
        fread(p, sizeof(PASAJEROS), 1, f);
      }
    }
    fseek(f, 0, SEEK_SET);
    fseek(f2, -sizeof(VUELOS), SEEK_CUR);
    printf("\n-----------------------------------------------------\n");
    printf("Codigo: %s\n", v->code);
    printf("Dia: %d\n", v->day);
    printf("Vuelo: %d\n", v->vuelo);
    printf("Precio: %.2f\n", v->price);
    printf("Pasajeros: %d\n", v->pasajeros);

    printf("\n-----------------------------------------------------\n");
    fwrite(v, sizeof(VUELOS), 1, f2);

    fflush(f2);
    fread(v, sizeof(VUELOS), 1, f2);
  }
  FILE *csv = fopen("../archivosParaEjercicios/vuelos.csv", "wt");
  if (!csv) {
    perror("Error al abrir el archivo");
    exit(1);
  }
  fseek(f2, 0, SEEK_SET);
  fprintf(csv, "Codigo; Dia; Vuelo; Precio; Pasajeros\n");
  fread(v, sizeof(VUELOS), 1, f2);
  while (!feof(f2)) {
    fprintf(csv, "%s; %d; %d; %.2f; %d\n", v->code, v->day, v->vuelo, v->price,
            v->pasajeros);
    fread(v, sizeof(VUELOS), 1, f2);
  }
  fclose(f);
  fclose(f2);
  fclose(csv);
}
void ej9() {
  FILE *f = fopen("../archivosParaEjercicios/INSCRIPTOS.dat", "r+b"),
       *f2 = fopen("../archivosParaEjercicios/Interesados.dat", "w+b");
  int dni;
  fseek(f, 0, SEEK_END);
  int cupos = ftell(f) / sizeof(INSCRIPTOS);
  rewind(f);
  INSCRIPTOS inscriptos, *i;
  INTERESADOS interesados, *in;
  in = &interesados;
  i = &inscriptos;
  if (!f || !f2) {
    perror("Error al abrir los archivos");
    exit(1);
  }
  printf("Ingrese el DNI: ");
  scanf("%d", &dni);
  while (dni > 0) {
    int pos = buscar9(f, dni);
    if (pos == -1) {
      if (cupos < 60) {
        printf("Ingrese el nombre: ");
        scanf("%s", i->AyN);
        i->dni = dni;
        i->Pagado = 0;
        cupos++;
      } else {
        printf("Quiere ingresar a la lista de interesados? (s/n): ");
        char c;
        scanf(" %c", &c);
        if (c == 's') {
          printf("Ingrese el nombre: ");
          scanf("%s", in->AyN);
          in->dni = dni;
          printf("Ingrese el telefono: ");
          scanf("%d", &in->tel);
          fwrite(in, sizeof(INTERESADOS), 1, f2);
        }
      }
    } else {
      fseek(f, pos * sizeof(INSCRIPTOS), SEEK_SET);
      fread(i, sizeof(INSCRIPTOS), 1, f);
    }
    if (i->Pagado == 1) {
      printf("Ya se encuentra inscripto\n");
    } else if (i->Pagado == 0) {
      printf("Pago pendiente. Desea registrar el pago?\n");
      char c;
      scanf(" %c", &c);
      if (c == 's') {
        i->Pagado = 1;
        fseek(f, pos * sizeof(INSCRIPTOS), SEEK_SET);
        fwrite(i, sizeof(INSCRIPTOS), 1, f);
        fflush(f);
      }
    }
    printf("Ingrese el DNI: ");
    scanf("%d", &dni);
  }
  fclose(f);
  fclose(f2);
}
int buscar9(FILE *f, int dni) {
  INSCRIPTOS inscriptos, *i;
  i = &inscriptos;
  int pos = -1;
  rewind(f);
  fread(i, sizeof(INSCRIPTOS), 1, f);
  while (!feof(f)) {
    if (i->dni == dni) {
      pos = ftell(f) / sizeof(INSCRIPTOS);
    }
    fread(i, sizeof(INSCRIPTOS), 1, f);
  }
  return pos;
}
