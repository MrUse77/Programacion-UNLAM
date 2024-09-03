#include "flush.h"
#include <stdio.h>
#include <string.h>

void fsgets(char texto[], int largo) {
  int i = 0;
  // flush(stdin);
  fgets(texto, largo, stdin);
  while (texto[i] != '\0') {
    if (texto[i] == '\n') {
      texto[i] = '\0';
    } else {
      i++;
    }
  }
}
void ejercicio1();
void ejercicio2();
void ejercicio3();
int Buscar3(char nomBusqueda[], char nombre[50][12], int alumnos);
void ejercicio4();
void ejercicio5();
void ejercicio6();

int main() {
  int option;
  printf("Elija el ejercicio (1 a 6): ");
  scanf("%d", &option);
  switch (option) {
  case 1:
    ejercicio1();
    break;
  case 2:
    ejercicio2();
    break;
  case 3:
    ejercicio3();
    break;
  case 4:
    ejercicio4();
    break;
  case 5:
    ejercicio5();
    break;
  case 6:
    ejercicio6();
    break;
  default:
    printf("Ejercicio no encontrado\n");
  }
  return 0;
}
// Ingresar una frase de hasta 500 caracteres y contar cuántas palabras contiene
// dicha frase. a. Considerar que las palabras están separadas por un único
// espacio. b. Considerar que las palabras pueden estar separadas por más de un
// espacio consecutivo.
void ejercicio1() {
  char frase[500] = "Hola buenos dias, c es lo mejor al igual que javascript, "
                    "python, etc. Java es horrible. Linux es mejor que "
                    "windows, aguante GNU y el software libre";
  // contar cuantas palabras contiene
  int palabras = 0;
  int i = 0;
  while (frase[i] != '\0') {
    if (frase[i] == ' ') {
      palabras++;
    }
    i++;
  }
  printf("La frase tiene %d palabras\n", palabras + 1);
}
void ejercicio2() {
  flush(stdin);
  char nombre[10], apellido[10], completo[25];
  printf("Ingrese nombre: ");
  fsgets(nombre, 10);
  printf("Ingrese apellido: ");
  fsgets(apellido, 10);
  printf("%s %s\n", nombre, apellido);
  strcpy(completo, nombre);
  strcat(completo, ", ");
  strcat(completo, apellido);
  printf("Nombre completo: %s\n", completo);
}
void ejercicio3() {
  flush(stdin);
  char nombre[50][12], nomBusqueda[12];
  int dni[50], i = 0, alumnos = 0;
  printf("Ingrese nombre y dni de una persona\n");
  fsgets(nombre[0], 10);
  scanf("%d", &dni[0]);
  while (strcmp(nombre[i], "FIN") != 0) {
    if (strcmp(nombre[i], "FIN") == 0) {
      break;
    }
    alumnos++;

    i++;
    printf("Ingrese nombre y dni de una persona\n");
    flush(stdin);
    fsgets(nombre[i], 10);
    scanf("%d", &dni[i]);
  }
  printf("Buscar por nombre: ");
  flush(stdin);
  fsgets(nomBusqueda, 12);
  while (strcmp(nomBusqueda, "NOBUSCARMAS") != 0) {
    i = Buscar3(nomBusqueda, nombre, alumnos);
    printf("DNI: %d\n", dni[i]);
    printf("Buscar por nombre: ");
    fsgets(nomBusqueda, 12);
  }

  char aux[12];
  for (i = 0; i < alumnos - 1; i++) {
    // alfabeticamente
    // ordenar por nombre
    for (int j = 0; j < alumnos - 1 - i; j++) {
      if (strcmp(nombre[j], nombre[j + 1]) >= 0) {
        // intercambiar
        strcpy(aux, nombre[j]);
        strcpy(nombre[j], nombre[j + 1]);
        strcpy(nombre[j + 1], aux);
      }
    }
  }
  for (i = 0; i < alumnos; i++) {
    printf("%s\n", nombre[i]);
  }
}
int Buscar3(char nomBusqueda[], char nombre[][12], int alumnos) {
  int i;
  printf("%d\n", alumnos);
  for (i = 0; i < alumnos; i++) {
    printf("%d", i);
    if (strcmp(nomBusqueda, nombre[i]) == 0) {
      return i;
    }
  }
  return -1;
}
void ejercicio4() {}
void ejercicio5() {}
void ejercicio6() {}
