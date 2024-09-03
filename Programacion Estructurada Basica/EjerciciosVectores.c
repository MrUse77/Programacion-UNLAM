#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void flush() {
  int c;
  do {
    c = getchar();
  } while (c != EOF && c != '\n');
}
void ej1();
void ej2();
void IngresarCodigos(int[]);
int Buscar2(int[], int);
void Mayor2(int[], int[]);
void Menor2(int[], int[]);
void ej3();
void ej4();
int Buscar4(int, int[], int);
void Listado4(int[], int[], int);
void ej5();
void Ordenar5(int[], int);
int Buscar5(int[], int, int);
void menosVentas5(int[], int);
void Listado5(int[], int[], int);
void ej6();
void ej7();
int main() {
  int opcion;
  srand(time(NULL));
  printf("Ingrese Ejercicio: ");
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
  }
  return 0;
}

void ej1() {
  int n[20], num, i;
  for (i = 0; i < 20; i++) {
    n[i] = rand() % 100 + 100;
  }
  int menor = n[0], rep, mayor = n[0], pos[20] = {0}, j = 1;
  for (i = 1; i < 20; i++) {
    if (n[i] < menor) {
      menor = n[i];
      rep = 0;
    } else if (menor == n[i]) {
      rep++;
    }
    if (n[i] > mayor) {
      mayor = n[i];
      pos[0] = i;
    } else if (mayor == n[i]) {
      pos[j] = i;
      j++;
    }
  }
  printf("Valor menor: %d, se repite: %d\n", menor, rep);
  printf("Valor mayor: %d, se repite en las posiciones: ", mayor);
  for (i = 0; i < j; i++) {
    printf("%d \n", pos[i]);
  }
}
void ej2() {
  int vend[10], cant[10] = {0}, num, i, aux = 0;
  IngresarCodigos(vend);
  printf("Ingrese codigo de producto:"), scanf("%d", &num);
  i = Buscar2(vend, num);
  while (num != 0) {
    if (i != -1) {
      printf("Ingrese cantidad vendida:"), scanf("%d", &aux);
      cant[i] += aux;
    }
    printf("Ingrese codigo de producto:"), scanf("%d", &num);
    i = Buscar2(vend, num);
  }
  for (i = 0; i < 10; i++) {
    printf("Codigo: %d\tCantidad: %d\n", vend[i], cant[i]);
  }
  Mayor2(vend, cant);
  Menor2(vend, cant);
}
void IngresarCodigos(int vend[]) {
  int i;
  for (i = 0; i < 10; i++) {
    vend[i] = rand() % 10000;
  }
  for (i = 0; i < 10; i++) {
    printf("%d\n", vend[i]);
  }
}
int Buscar2(int vend[], int num) {
  int i;
  for (i = 0; i < 10; i++) {
    if (vend[i] == num) {
      return i;
    }
  }
  return -1;
}
void Mayor2(int vend[], int cant[]) {
  int i, max = 0;
  for (i = 0; i < 10; i++) {
    if (cant[i] > max) {
      max = i;
    }
  }
  printf("Mayor cantidad vendida: %d, del producto: %d ", cant[max], vend[max]);
}
void Menor2(int vend[], int cant[]) {
  int i, min = 0;
  for (i = 0; i < 10; i++) {
    if (cant[i] < min) {
      min = i;
    }
  }
  printf("Menor cantidad vendida: %d, del producto: %d", cant[min], vend[min]);
}

void ej3() {
  int orden[5], i;
  for (i = 0; i < 5; i++) {
    // ordenar de forma aleatoria 1,2,3,4,5 sin repetir
    do {
      orden[i] = rand() % 5 + 1;
    } while (orden[i] == orden[i - 1] || orden[i] == orden[i - 2] ||
             orden[i] == orden[i - 3] || orden[i] == orden[i - 4]);
  }
  for (i = 0; i < 5; i++) {
    printf("%d\n", orden[i]);
  }
}
void ej4() {
  int dni[60], nota[60], i = 0, exist, aux;
  printf("Ingrese DNI\n");
  printf("DNI: "), scanf("%d", &aux);
  while (aux > 0) {
    exist = Buscar4(aux, dni, i);
    dni[i] = aux;
    if (!exist) {
      do {
        printf("Nota: "), scanf("%d", &nota[i]);
      } while (nota[i] < 0 || nota[i] > 10);
      i++;
    }
    printf("Ingrese DNI\n");
    printf("DNI: "), scanf("%d", &aux);
  }
  Listado4(dni, nota, i);
}
int Buscar4(int aux, int dni[], int i) {
  int j;
  for (j = 0; j < i; j++) {
    if (dni[j] == aux) {
      return 1;
    }
  }
  return 0;
}
void Listado4(int dni[], int nota[], int i) {
  int j, k, max;
  for (j = 0; j < i - 1; j++) {
    for (k = 0; k < i - j - 1; k++) {
      if (nota[k] < nota[k + 1]) {
        max = nota[k];
        nota[k] = nota[k + 1];
        nota[k + 1] = max;
        max = dni[k];
        dni[k] = dni[k + 1];
        dni[k + 1] = max;
      }
    }
  }
  for (j = 0; j < i; j++) {
    printf("DNI: %d\tNota: %d\n", dni[j], nota[j]);
  }
}
void ej5() {
  int codigo[15], i, j, aux, aux2, ventasV[5] = {0}, ventasP[15] = {0}, pos,
                                   vend;
  for (i = 0; i < 15; i++) {
    codigo[i] = i;
  }
  Ordenar5(codigo, 15);
  for (i = 0; i < 15; i++) {
    printf("%d\n", codigo[i]);
  }
  printf("Ingrese codigo de vendedor (1001 a 1005) (0 para terminar)\n");
  scanf("%d", &vend);
  while (vend != 0) {
    vend -= 1000;
    printf("Ingrese codigo de producto\n");
    scanf("%d", &aux);
    pos = Buscar5(codigo, aux, 15);
    if (pos != -1) {
      printf("Ingrese cantidad vendida\n");
      scanf("%d", &aux2);
      ventasV[vend - 1] += aux2;
      ventasP[pos] += aux2;
    }
    printf("Ingrese codigo de vendedor (1001 a 1005) (0 para terminar)\n");
    scanf("%d", &vend);
  }
  Listado5(codigo, ventasP, 15);
  menosVentas5(ventasV, 5);
}

void Ordenar5(int codigo[], int n) {
  int i, j, aux;
  for (i = 0; i < n - 1; i++) {
    for (j = 0; j < n - i - 1; j++) {
      if (codigo[j] > codigo[j + 1]) {
        aux = codigo[j];
        codigo[j] = codigo[j + 1];
        codigo[j + 1] = aux;
      }
    }
  }
}

int Buscar5(int codigo[], int aux, int n) {
  int i;
  for (i = 0; i < n; i++) {
    if (codigo[i] == aux) {
      return i;
    }
  }
  return -1;
}
void Listado5(int codigo[], int ventasP[], int n) {
  int i;
  for (i = 0; i < n; i++) {
    printf("Codigo: %d\tVentas: %d\n", codigo[i], ventasP[i]);
  }
}

void menosVentas5(int ventasV[], int n) {
  int i, min = 0;
  for (i = 0; i < n; i++) {
    if (ventasV[i] < ventasV[min]) {
      min = i;
    }
  }
  printf("Vendedor con menos ventas: %d\tVentas: %d\n", min + 1001,
         ventasV[min]);
}

void ej6() {

  int vec[13];
  int i, j, num;

  // Ingresar los 10 números ordenados
  printf("Ingrese 10 enteros ordenados de menor a mayor:\n");
  for (i = 0; i < 10; ++i) {
    scanf("%d", &vec[i]);
  }

  // Mostrar el vector inicial
  printf("Vector inicial:\n");
  for (i = 0; i < 10; ++i) {
    printf("%d ", vec[i]);
  }
  printf("\n");

  // Ingresar 3 números más no ordenados y insertarlos
  printf("Ingrese 3 números no ordenados para insertar:\n");
  for (i = 0; i < 3; ++i) {
    scanf("%d", &num);

    // Encontrar la posición correcta e insertar el número
    j = 9 + i; // índice del último elemento del vector original más los ya
               // insertados
    while (j >= 0 && vec[j] > num) {
      vec[j + 1] = vec[j];
      j--;
    }
    vec[j + 1] = num;
  }

  // Mostrar el vector final
  printf("Vector final:\n");
  for (i = 0; i < 13; ++i) {
    printf("%d ", vec[i]);
  }
  printf("\n");
}
void ej7() {
  int vec[10];
  int num, i, count = 0;

  // Ingresar los 10 números en el vector
  printf("Ingrese 10 enteros:\n");
  for (i = 0; i < 10; ++i) {
    scanf("%d", &vec[i]);
  }

  // Ingresar el número a buscar
  printf("Ingrese el número a buscar: ");
  scanf("%d", &num);

  // Contar cuántas veces aparece el número en el vector
  for (i = 0; i < 10; ++i) {
    if (vec[i] == num) {
      count++;
    }
  }

  // Si el número se encontró, crear un nuevo vector para los índices
  if (count > 0) {
    int indices[count];
    int index = 0;

    for (i = 0; i < 10; ++i) {
      if (vec[i] == num) {
        indices[index++] = i;
      }
    }

    // Mostrar los índices encontrados
    printf("El número %d se encuentra en los índices: ", num);
    for (i = 0; i < count; ++i) {
      printf("%d ", indices[i]);
    }
    printf("\n");
  } else {
    printf("El número %d no se encuentra en el vector.\n", num);
  }
}
