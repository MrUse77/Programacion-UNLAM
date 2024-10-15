//
// Created by agusd on 23/5/2024.
//
#include "flush.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
void fsgetsP(char *texto, int largo) {

  flush(stdin);
  fgets(texto, largo, stdin);
  while (*(texto) != '\0') {
    if (*(texto) == '\n') {
      *(texto) = '\0';
    } else {
      texto++;
    }
  }
}
void ej1();
void ej2();
void ej3();
void Carga3(int *, float *, char *);
void ej4();
void Carga4(int *, int);
void Mostrar4(int *, int);
void ej5();
int *Buscar5(int[], int, int);
void ej6();
int *BuscarMayor6(int[], int);
typedef struct {
  int codigo;
  char descripcion[31];
  float precio;
} sProductos;
void ej7();
typedef struct {
  char nom[31];
  int dni;
} alumno;
void ej8();
int Cargar8(alumno *, int);
void Mostrar8(alumno *, int);
int main() {
  srand(time(NULL));
  int opcion;
  printf("EJERCICIOS DE PUNTEROS\n");
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
  }
  //  system("pause");
  return 0;
}
// 5.1 Crear una variable entera y un puntero a dicha variable
// Asignar el valor 10 a la variable mediante el puntero
// Mostrar:
// a) la dirección de la variable
//  b) la dirección del puntero
// c) el contenido de la variable
//  d) el contenido de la variable accediendo mediante el puntero
//  e) el contenido del puntero

void ej1() {
  int num = rand();
  int *p;
  p = &num;
  *p = 10;
  printf("Numero aleatorio: %d\n", *p);
  printf("Direccion de la variable: %p\n", &num);
  printf("Direccion de memoria del puntero: %p\n", &p);
  printf("Contenido de la variable: %d\n", num);
  printf("contenido de la variable accediendo mediante el puntero: %d\n", *p);
  printf("Contenido del puntero: %p\n", p);
}
void ej2() {
  int a, b, *pa, *pb, suma;
  a = rand() % 100;
  b = rand() % 100;
  pa = &a;
  pb = &b;
  suma = *pa + *pb;
  printf("La suma de %d + %d es: %d\n", b, a, suma);
}
void ej3() {
  int a, *pa;
  float b, *pb;
  char c, *pc;
  pa = &a;
  pb = &b;
  pc = &c;
  Carga3(pa, pb, pc);

  printf("El valor entero es: %d\n", a);
  printf("El valor flotante es: %f\n", b);
  printf("El caracter es: %c\n", c);
}

void Carga3(int *a, float *b, char *c) {
  printf("Ingrese un valor entero: ");
  scanf("%d", a);
  printf("Ingrese un valor flotante: ");
  scanf("%f", b);
  printf("Ingrese un caracter: ");
  scanf(" %c", c);
}

void ej4() {
  int vector[10];
  Carga4(vector, 10);
  Mostrar4(vector, 10);
}
void Carga4(int *v, int N) {
  int i;
  for (i = 0; i < N; i++) {
    printf("Ingrese un valor: \n");
    scanf("%d", v + i);
  }
}
void Mostrar4(int *v, int N) {
  int i;
  for (i = 0; i < N; i++) {
    printf("%d\n", *(v + i));
  }
}
// 5.5 Al programa anterior agregarle una función que reciba la dirección de
// inicio del vector y un número a buscar
//  y retorne un puntero al dato encontrado o NULL sino lo encuentra. En el main
//  agregar un proceso de búsqueda que se repita hasta ingresar un número
//  negativo o cero a buscar. Si se lo encontró se debe
// indicar en que posición del vector estaba (calcular dicha posición utilizando
// la dirección retornada)
void ej5() {
  int vector[10], num, *p;
  Carga4(vector, 10);
  Mostrar4(vector, 10);
  do {
    printf("Ingrese el numero a buscar: ");
    scanf("%d", &num);
    p = Buscar5(vector, num, 10);
    printf("\n%p\n", p);
    printf("\n%p\n", p - vector);
    printf("\n%p\n", vector);
    if (p == NULL) {
      printf("El numero a buscar no se encuentra en el vector\n");
    } else {

      printf("El numero a buscar esta en la posicion: %d\n", (p - vector));
    }
  } while (num > 0);
}

int *Buscar5(int *v, int num, int N) {
  int i = 0;
  while (i <= N) {
    if (*(v + i) == num) {
      printf("%p,%d\n", (v + i), i);
      return (v + i);
    }
    i++;
  }
  return NULL;
}

// 5.6 Cargar un vector de enteros con números aleatorios de 3 cifras máximo.
// Realizar una función que retorne un puntero al mayor valor del vector
// Mostrar el máximo valor mediante el puntero y la posición en la cual se
// encuentra
void ej6() {
  int vec[10];

  for (int i = 0; i < 10; i++) {
    // numero de 3 cifras maximo
    vec[i] = rand() % 1000;
    printf("%d\n", vec[i]);
  }
  int *ptr = BuscarMayor6(vec, 10);
  printf("El mayor es: %d, en la pos %p\n", *ptr, ptr);
}
int *BuscarMayor6(int *vec, int n) {
  int *ptr = vec;
  for (int i = 0; i < n; i++) {
    if (*ptr < *(vec + i)) {
      ptr = (vec + i);
    }
  }
  return ptr;
}

void ej7() {
  sProductos producto;
  sProductos *pProducto;
  pProducto = &producto;
  printf("Ingrese el codigo del producto: ");
  scanf("%d", &pProducto->codigo);

  printf("Ingrese la descripcion del producto: ");
  fsgetsP(pProducto->descripcion, 31);
  printf("Ingrese el precio del producto: ");
  scanf("%f", &pProducto->precio);
  printf("Codigo: %d\n", pProducto->codigo);
  printf("Descripcion: %s\n", pProducto->descripcion);
  printf("Precio: %.2f\n", pProducto->precio);
}

void ej8() {
  alumno alu[30];
  alumno *pAlu;
  pAlu = alu;
  int i = 0;
  i = Cargar8(pAlu, 30);
  Mostrar8(pAlu, i);
}

int Cargar8(alumno *alu, int n) {
  int i;
  for (i = 0; i < n; i++) {
    printf("Ingrese el nombre del alumno: ");
    fsgetsP(alu->nom, 31);
    printf("Ingrese el DNI del alumno: ");
    scanf("%d", &alu->dni);
    if (alu->dni < 0) {
      return i;
    }
    alu++;
  }
  return i;
}

void Mostrar8(alumno *alu, int n) {
  for (int i = 0; i < n; i++) {
    printf("Nombre: %s \n", alu->nom);
    printf("DNI: %d \n", alu->dni);
    alu++;
  }
}
