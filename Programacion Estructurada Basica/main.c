#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define TAM 10

int ej1();

int ej2();

int ej3();

int ej4();

void ej5(int [], int);

void ej6();

void CargaAleatoria(int [], int, int);

void Mostrar(int [], int);

void Carga(int [], int);

int Maximo(int [], int);

void MostrarMaximo(int [], int, int);

int Buscar(int [], int, int);


int main() {
    int v[10];
    //ej1();
    ej2();
    //ej3();
    //ej4();
    //ej5(v, 10);
    //ej6();
    return 0;
}

// Ejemplo 1
int ej1() {
    int num;
    srand(time(NULL));
    num = rand();
    printf("Numero aleatorio: %d\n", num);
    return 0;
}

// Ejemplo 2 - Carga de un vector con numeros aleatorios de 2 cifras
int ej2() {
    int vec[TAM];
    srand(time(NULL));
    CargaAleatoria(vec, TAM, 100);
    Mostrar(vec, TAM);

    return 0;
}

void CargaAleatoria(int v[], int ce, int max) {
    int i;
    for (i = 0; i < ce; i++) {
        v[i] = rand() % max;
    }
}

void Mostrar(int v[], int ce) {
    int i;
    for (i = 0; i < ce; i++) {
        printf("%d\n", v[i]);
    }
}

// Ejemplo 3 - Maximos y minimos
int ej3() {
    int vec[10], max;
    Carga(vec, 10);
    max = Maximo(vec, 10);
    MostrarMaximo(vec, max, 10);
    return 0;
}

void Carga(int v[], int N) {
    int i;
    for (i = 0; i < N; i++) {
        printf("Ingrese un valor (%d): ", i + 1);
        scanf("%d", &v[i]);
    }
}

int Maximo(int v[], int N) {
    int i, max = v[0];
    max = v[0];
    for (i = 1; i < N; i++) {
        if (v[i] > max) {
            max = v[i];
        }
    }
    return max;
}

void MostrarMaximo(int v[], int max, int N) {
    int i, cont = 0;
    printf("El maximo es: %d y se encuentra en las siguientes posiciones del vector:\n", max);
    for (i = 0; i < N; i++) {
        if (v[i] == max) {
            printf("%d\n", i);
            cont++;
        }
    }
    printf("Se repite %d veces\n", cont);
}

//Ejemplo 4 = Busqueda secuencial en vectores
int ej4() {
    int vec[10], num, pos;
    Carga(vec, 10);
    printf("Ingrese el numero a buscar: ");
    scanf("%d", &num);
    pos = Buscar(vec, num, 10);
    if (pos == -1) printf("El numero a buscar no se encuentra en el vector\n");
    else printf("El numero a buscar esta en la posicion: %d\n", pos);
    return 0;
}

int Buscar(int v[], int datoABuscar, int cantElem) {
    int i = 0, pos = -1;
    while (pos == -1 && i < cantElem) {
        if (v[i] == datoABuscar) pos = i;
        else i++;
    }
    return pos;
}

//Ejemplo 5 - Carga de un vector sin admitir valores repetidos
void ej5(int v[], int ce) {
    int i, pos, aux;
    for (i = 0; i < ce; i++) {
        do {
            printf("Ingrese un valor: ");
            scanf("%d", &aux);
            pos = Buscar(v, aux, i);
            if (pos != -1) printf("El valor ya fue ingresado\n");
        } while (pos != -1);
        v[i] = aux;
    }
    printf("Vector cargado\n");
    Mostrar(v, ce);
}

void ej6(){
  char cadena[]="hola";//vector de  5 elementos, siempre hay un \0 al final
  char cadena2[20];
  printf("%s\n",cadena);
  printf("ingrese texto:");
  fgets(cadena2,20,stdin);//Permite espacios, scanf no permite espacios se usa %s), gets y scanf no evaluan limites, por eso se usa fgets
  printf("%s\n",cadena2);
  printf("Longitud de la cadena: %d\n",strlen(cadena2));
  int tam = strlen("apellido");
  printf("%d\n",tam);
  char copy[20] ;
  strcpy(copy,cadena);
  printf("%s\n",copy);
  strcat(copy," mundo");
  printf("%s\n",copy);
  strcmp("hola","hola");//0 si son iguales, -1 si la primera es menor, 1 si la primera es mayor

}