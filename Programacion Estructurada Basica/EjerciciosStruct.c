#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void flush(FILE *stream) {
  int c;
  do {
    c = getchar();
  } while (c != EOF && c != '\n');
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
  int code;
  float price;
  char description[30];
  int unidadesVendidas;
  float total;
} product;

void ej1();
int Buscar(product[], int, int);

typedef struct {
  int code;
  float price;
  int stock;
} medicamentos;

void ej2();
void INGRESO2(medicamentos[]);
void BUSQUEDA_MEDI(medicamentos[]);
void INFORME(medicamentos[]);
int BuscarMedicamentos2(medicamentos[], int, int);

typedef struct {
  int dni;
  char name[80];
  int nota1;
  int nota2;
  float promedio;
} alumno;
void ej3();
int INGRESO3(alumno[]);
void RESULTADO3(alumno[], int);
void INFORME_PROM(alumno[], int);
int BuscarAlumno(alumno[], int, int);

int main() {
  int opcion;
  printf("EJERCICIOS DE STRUCT\n");
  printf("Elija el ejercicio que desea realizar: \n");
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
  system("pause");
  return 0;
}
/*
 * */
void ej1() {
  int i = 0, j = 2, codigo, unidades, venta[2] = {0};
  float total[2] = {0};
  product productos[2] = {0};
  ;
  char fin[] = "fin";
  do {
    do {
      printf("Ingrese el codigo del producto: ");
      scanf("%d", &productos[i].code);

    } while (productos[i].code < 10000 || productos[i].code > 99999 ||
             Buscar(productos, productos[i].code, j) != i);
    printf("Ingrese el precio del producto: ");
    scanf("%f", &productos[i].price);
    do {
      printf("Ingrese la descripcion del producto: ");
      scanf("%s", productos[i].description);
    } while (strlen(productos[i].description) > 30);
    printf("Ingrese la cantidad de unidades vendidas: ");
    scanf("%d", &productos[i].unidadesVendidas);
    // convertir int en float para que no haya problemas con la multiplicacion
    productos[i].total =
        productos[i].price * (float)productos[i].unidadesVendidas;
    i++;

  } while (i != j && strcmp(productos[i - 1].description, fin) != 0);
  do {
    int codigoProducto = 0, aux = 0;
    do {
      printf("Ingrese Codigo de producto");
      scanf("%d", &codigo);
    } while ((codigo < 10000 || codigo > 99999) &&
             Buscar(productos, codigo, j) == -1);
    codigoProducto = Buscar(productos, codigo, j);
    printf("Ingrese cantidad de unidades vendidas");
    scanf("%d", &unidades);
    venta[codigoProducto] += unidades;
    total[codigoProducto] +=
        productos[codigoProducto].price * venta[codigoProducto];
  } while (unidades != 0);
  for (i = 0; i < j; i++) {
    productos[i].unidadesVendidas = venta[i];
    productos[i].total = total[i];
  }
  printf("DESCRIPCION \t CANTIDAD DE UNIDADES VENDIDAS \t IMPORTE TOTAL "
         "VENDIDO\n");
  for (i = 0; i < j; i++) {
    printf("%s \t %d \t %f\n", productos[i].description,
           productos[i].unidadesVendidas, productos[i].total);
  }
}
int Buscar(product productos[], int code, int j) {
  int i = 0;
  while (i < j) {
    if (productos[i].code == code) {
      return i;
    }
    i++;
  }
  return -1;
}
void ej2() {
  medicamentos medicamento[50];
  srand((float)time(NULL));
  INGRESO2(medicamento);
  // system("cls");
  BUSQUEDA_MEDI(medicamento);
  INFORME(medicamento);
}
void INGRESO2(medicamentos medicamento[]) {
  int i = 0, code, stock;
  float price;
  for (i = 0; i < 50; i++) {
    // nuemro random, positivo y menor a 100
    code = rand() % 100;
    // numero random, positivo, real y menor a 350
    price = (float)rand() / (float)350;
    stock = rand() % 100;
    printf("Ingrese el codigo del medicamento: \n");
    medicamento[i].code = code; // scanf("%d", &medicamento[i].code);
    printf("%d\n", medicamento[i].code);
    printf("Ingrese el precio del medicamento: \n");
    medicamento[i].price = price; // scanf("%f", &medicamento[i].price);
    printf("%.2f\n", medicamento[i].price);
    printf("Ingrese el stock del medicamento: \n");
    medicamento[i].stock = stock; // scanf("%d", &medicamento[i].stock);
    printf("%d\n", medicamento[i].stock);
  }
}
void BUSQUEDA_MEDI(medicamentos medicamento[]) {
  int i = 0, codigo;
  do {
    printf("Ingrese el codigo del medicamento que desea buscar: ");
    scanf("%d", &codigo);
    i = BuscarMedicamentos2(medicamento, codigo, 50);
    if (i == -1) {
      printf("No se encontro el medicamento\n");
    } else {
      printf("CODIGO \t PRECIO \t STOCK\n");
      printf("%d \t %f \t %d\n", medicamento[i].code, medicamento[i].price,
             medicamento[i].stock);
    }

  } while (codigo > 0 || i == -1);
}
void INFORME(medicamentos medicamento[]) {
  int i;
  printf("CODIGO \t PRECIO \t STOCK\n");
  for (i = 0; i < 50; i++) {
    if (medicamento[i].stock < 10) {
      printf("%d \t %.2f \t %d\n", medicamento[i].code, medicamento[i].price,
             medicamento[i].stock);
    }
  }
}
int BuscarMedicamentos2(medicamentos medicamento[], int codigo, int j) {
  int i = 0;
  while (i < j) {
    if (medicamento[i].code == codigo) {
      return i;
    }
    i++;
  }
  return -1;
}
void ej3() {
  alumno alumnos[80];
  int i = 0;
  i = INGRESO3(alumnos);
  system("cls");
  RESULTADO3(alumnos, i);
  INFORME_PROM(alumnos, i);
}
int INGRESO3(alumno alumnos[]) {
  int i = 0;
  do {
    do {
      printf("Ingrese el DNI del alumno: ");
      scanf("%d", &alumnos[i].dni);
      printf("%d\n", BuscarAlumno(alumnos, alumnos[i].dni, i + 1));
      // con 0 termina
    } while (((alumnos[i].dni < 10000000 || alumnos[i].dni > 99999999) &&
              alumnos[i].dni != 0) ||
             BuscarAlumno(alumnos, alumnos[i].dni, i + 1) != i);

    do {
      printf("Ingrese el nombre del alumno: ");
      fsgets(alumnos[i].name, 80);
    } while (strlen(alumnos[i].name) > 80);
    do {
      printf("Ingrese la nota 1 del alumno: ");
      scanf("%d", &alumnos[i].nota1);
    } while (alumnos[i].nota1 < 0 || alumnos[i].nota1 > 10);
    do {
      printf("Ingrese la nota 2 del alumno: ");
      scanf("%d", &alumnos[i].nota2);
    } while (alumnos[i].nota2 < 0 || alumnos[i].nota2 > 10);
    alumnos[i].promedio = (float)(alumnos[i].nota1 + alumnos[i].nota2) / 2;
    i++;
  } while (alumnos[i - 1].dni > 0 && i < 80);
  return i;
}
void RESULTADO3(alumno alumnos[], int cant) {
  int i, j = 0, k = 0;
  alumno aprobados[cant], reprobados[cant];
  for (i = 0; i < cant; i++) {
    if (alumnos[i].nota1 >= 4 && alumnos[i].nota2 >= 4) {
      aprobados[j] = alumnos[i];
      j++;
    } else {
      reprobados[k] = alumnos[i];
      k++;
    }
    printf("%d\n", j);
    printf("%d\n", k);
  }
  printf("APROBADOS\n");
  printf("\t DNI \t NOMBRE \t NOTA 1 \t NOTA 2 \t PROMEDIO\n");
  for (i = 0; i < j; i++) {
    printf("%d \t %s \t %d \t %d \t %.2f\n", aprobados[i].dni,
           aprobados[i].name, aprobados[i].nota1, aprobados[i].nota2,
           aprobados[i].promedio);
  }
  printf("REPROBADOS\n");
  printf("\t DNI \t NOMBRE \t NOTA 1 \t NOTA 2 \t PROMEDIO\n");
  for (i = 0; i < k; i++) {
    printf("%d \t %s \t %d \t %d \t %.2f\n", reprobados[i].dni,
           reprobados[i].name, reprobados[i].nota1, reprobados[i].nota2,
           reprobados[i].promedio);
  }
}
void INFORME_PROM(alumno alumnos[], int cant) {
  int i = 0, j = 0;
  alumno promocionado[cant];
  for (i = 0; i < cant; i++) {
    if (alumnos[i].nota1 >= 7 && alumnos[i].nota2 >= 7) {
      promocionado[j] = alumnos[i];
      j++;
    }
  }
  printf("PROMOCIONADOS\n");
  printf("DNI \t NOMBRE \t NOTA 1 \t NOTA 2 \t PROMEDIO\n");
  for (i = 0; i < j; i++) {
    printf("%d \t %s \t %d \t %d \t %.2f\n", promocionado[i].dni,
           promocionado[i].name, promocionado[i].nota1, promocionado[i].nota2,
           promocionado[i].promedio);
  }
}
int BuscarAlumno(alumno alumnos[], int dni, int j) {
  int i = 0;
  while (i < j) {
    if (alumnos[i].dni == dni) {
      return i;
    }
    i++;
  }
  return -1;
}
void ej4() {}
