#include "myShortCuts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void ejercicio1();
void ejercicio2();
void ejercicio3();
int Buscar3(char nomBusqueda[], char nombre[50][12], int alumnos);
void ejercicio4();
int Buscar4(char codigo[], char codigos[][4], int productos);
void Listado4(char[][4], int[], int[], int);
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
void ejercicio4() {
	flush(stdin);
	int precio[50], auxP, i = 0, productos = 0, encontrado = 0, venta[50],
		total = 0;
	char codigo[50][4], aux[4];
	printf("Ingrese el codigo y el precio de un producto\n");
	fsgets(aux, 10);
	auxP = rand() % 100; //  scanf("%d", &auxP);
	printf("%d\n", auxP);
	while (strcmp(aux, "FIN") != 0 && productos < 50) {
		strcpy(codigo[i], aux);
		precio[i] = auxP;
		productos++;
		i++;
		printf("Ingrese el codigo y el precio de un producto\n");
		fsgets(aux, 4);
		flush(stdin);
		auxP = rand() % 101; //  scanf("%d", &auxP);
		printf("%d\n", auxP);
	}
	// flush(stdin);
	do {
		printf("Ingrese el codigo de un producto a buscar: ");
		fsgets(aux, 4);
		encontrado = Buscar4(aux, codigo, productos);
		if (encontrado == -1) {
			printf("Producto no encontrado\n");
		}
	} while (encontrado == -1);
	do {
		printf("Indique unidades vendidas: \n");
		auxP = rand() % 10; //  scanf("%d", &auxP);
		printf("%d\n", auxP);
	} while (auxP < 0);
	while (auxP != 0 && encontrado != -1) {

		venta[encontrado] += auxP;

		printf("Ingrese el codigo de un producto a buscar: ");
		fsgets(aux, 4);
		encontrado = Buscar4(aux, codigo, productos);

		flush(stdin);
		do {
			printf("Indique unidades vendidas: ");
			auxP = rand() % 10; //  scanf("%d", &auxP);
		} while (auxP < 0);
	}
	for (i = 0; i < productos; i++) {
		total += precio[i] * venta[i];
	}
	printf("Total de la venta: %d\n", total);
	Listado4(codigo, precio, venta, productos);
}

int Buscar4(char codigo[], char codigos[][4], int productos) {
	int i;
	for (i = 0; i < productos; i++) {
		if (strcmp(codigo, codigos[i]) == 0) {
			return i;
		}
	}
	return -1;
}
void Listado4(char codigos[][4], int precios[], int ventas[], int productos) {
	int i, j;
	for (i = 0; i < productos - 1; i++) {
		for (j = 0; j < productos - 1 - i; j++) {
			if (strcmp(codigos[j], codigos[j + 1]) > 0) {
				// intercambiar
				char aux[3];
				int aux2;
				aux2 = precios[j];
				precios[j] = precios[j + 1];
				precios[j + 1] = aux2;
				strcpy(aux, codigos[j]);
				strcpy(codigos[j], codigos[j + 1]);
				strcpy(codigos[j + 1], aux);
			}
		}
	}
	for (i = 0; i < productos; i++) {
		printf("%s %d %d\n", codigos[i], precios[i], ventas[i]);
	}
}
void ejercicio5() {}
void ejercicio6() {}
