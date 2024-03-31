//
// Created by agusd on 29/3/2024.
//
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define TAM 20

int ej1();

int ej2();

int Min(const int[], int);

int Max(const int[], int);

int Buscar(int[], int, int);

void IngresarCodigos(int[], int);

int IngresarCantidades(int[], int);

void BuscarMayorCantidad(int[], int[], int);

void BuscarMenorCantidad(int[], int[], int);


int main() {
    printf("EJERCICIOS:\n");
    printf("EJ 1:\n");
    ej1();
    /*   printf("EJ 2:\n");
       ej2();*/
    return 0;
}

//Ejercicio 1.1: Cargar de forma aleatoria un vector de 20 elementos con números de 3 cifras.
//a. Mostrar el menor valor e indicar cuantas veces se repite
//b. Mostrar el mayor valor e indicar en que posición/posiciones se encuentra
int ej1() {
    int ce = 20;
    int v[20];
    int min, max;
    srand(time(NULL));
    int i;


    for (i = 0; i < ce; i++) {
        v[i] = rand() % 1000;
    }

    min = Min(v, 20);
    max = Max(v, 20);
    printf("El menor valor es %d\n", min);
    printf("El mayor valor es %d\n", max);
    return 0;
}

int Min(const int v[], int N) {
    int i, min;
    min = v[0];
    for (i = 1; i < N; i++) {
        if (v[i] < min) {
            min = v[i];
        }
    }
    return min;
}

int Max(const int v[], int N) {
    int i, max;
    max = v[0];
    for (i = 1; i < N; i++) {
        if (v[i] > max) {
            max = v[i];
        }
    }
    return max;
}
//Ejercicio 1.2: Una empresa debe registrar los pedidos recibidos de cada uno sus 10 productos a lo largo del día. Cada
//producto está identificado por un código de 4 cifras. Los códigos deben ingresarse al inicio del programa
//mediante la función IngresaCódigos y no pueden repetirse.
//Por cada pedido se recibe:
//• Código de producto
//• Cantidad de unidades solicitadas
//Se puede recibir más de un pedido por producto.
// La carga de pedidos finaliza cuando se ingresa un producto igual a 0
//Al finalizar se debe:
//a) Emitir un listado con código y cantidad de unidades solicitadas de cada producto.
//b) El / los productos del cual se solicitaron mayor cantidad de unidades.
//c) El / los productos del cual se solicitaron menos cantidad de unidades.

int ej2() {
    int cant = 3;
    int products[10];
    int cants[10] = {0};

    int codMax, codMin;

    for (int i = 0; i < cant; i++) {
        IngresarCodigos(products, i);
        IngresarCantidades(cants, i);
    }

    for (int i = 0; i < cant; i++) {
        printf("Codigo: %d, Cantidad: %d\n", products[i], cants[i]);
    }

    BuscarMayorCantidad(products, cants, cant);
    BuscarMenorCantidad(products, cants, cant);

    return 0;
}

void IngresarCodigos(int v[], int i) {
    int pos;
    int aux;
    do {
        printf("Ingrese el codigo del producto %d: ", i + 1);
        scanf("%d", &aux);
        pos = Buscar(v, aux, i);
        if (pos != -1) printf("Codigo incorrecto\n");
    } while (pos != -1);
    v[i] = aux;
}

int Buscar(int v[], int datoABuscar, int cantElem) {
    int i = 0, pos = -1;
    if (datoABuscar < 1000 || datoABuscar > 9999) return 1;
    while (pos == -1 && i < cantElem) {
        if (v[i] == datoABuscar) {
            pos = i;
        } else i++;
    }
    return pos;
}

int IngresarCantidades(int v[], int c) {
    int aux = 0;


    do {
        printf("Ingrese la cantidad de unidades solicitadas(con 0 terminan las solicitudes del producto): ");
        scanf("%d", &aux);
        v[c] += aux;

    } while (aux != 0);

    return 0;
}

void BuscarMayorCantidad(int v[], int c[], int N) {
    int i, max[2];
    max[0] = v[0];
    max[1] = c[0];
    for (i = 0; i < N; i++) {
        if (c[i] > max[1]) {
            max[0] = v[i];
            max[1] = c[i];
        }
    }
    printf("La mayor cantidad de unidades, con codigo %d, solicitadas es: %d\n", max[0], max[1]);


}

void BuscarMenorCantidad(int v[], int c[], int N) {
    int i, min[2];
    min[0] = v[0];
    min[1] = c[0];
    for (i = 0; i < N; i++) {
        if (c[i] < min[1]) {
            min[0] = v[i];
            min[1] = c[i];
        }
    }
    printf("La menor cantidad de unidades, con codigo %d, solicitadas es: %d\n", min[0], min[1]);

}

//Ejercicio 1.3  En un curso de la maestría en informática los estudiantes deben exponer en forma grupal sus investigaciones
//de un tema dado. El curso está formado por 5 grupos. Se pide realizar un programa para sortear el orden de
//exposición de dichos grupos. Cada vez que se ejecute el programa deberá mostrar un orden de exposición
//distinto para cada uno de los 5 grupos.