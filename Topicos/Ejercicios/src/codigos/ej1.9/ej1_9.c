/*
 *
 *	Desarrollar una función que normalice la cadena de texto que se le pasa como argumento. En este
	caso, la cadena se encontrará normalizada cuando la primera letra de cada palabra sea mayúscula y
	las siguientes minúsculas. La cadena normalizada no deberá contener espacios o tabulaciones al
	inicio o al final. En el caso de que las palabras de la cadena se encuentren separadas por más de un
	espacio o tabulación, se deberán eliminar los excedentes. Se debe modificar la cadena pasada como
	argumento. No puede realizar una o más copias locales de la cadena original. Contemple las
	condiciones de borde y haga un listado de éstas.
 * */
#include "../../headers/ejercicios.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../headers/strings.h"

void ej1_9()
{
	char *str = malloc(sizeof(char) * 100);
	char *originalStr = str;
	printf("Ingrese una cadena de texto: ");
	fgets(str, 100, stdin);
	// Eliminar espacios al inicio
	while (*str == ' ' || *str == '\t') {
		str++;
	}
	// Eliminar espacios al final
	char *end = str + strLen(str) - 1;
	while (end > str && (*end == ' ' || *end == '\t' || *end == '\n')) {
		end--;
	}
	*(end + 1) = '\0'; // Terminar la cadena
	// Normalizar cada palabra
	bool newWord = true;
	for (char *p = str; *p != '\0'; p++) {
		if (*p == ' ' || *p == '\t') {
			newWord = true; // Siguiente palabra
		} else if (newWord) {
			if (*p >= 'a' && *p <= 'z') {
				*p -= ('a' - 'A'); // Convertir a mayúscula
			}
			newWord = false;
		} else if (*p >= 'A' && *p <= 'Z') {
			*p += ('a' - 'A'); // Convertir a minúscula
		}
	}
	printf("Cadena normalizada: '%s'\n", str);
	free(originalStr);
}
