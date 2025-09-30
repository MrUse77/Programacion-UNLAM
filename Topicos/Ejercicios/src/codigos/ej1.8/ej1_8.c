
#include "../../headers/ejercicios.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../headers/strings.h"

#define MAX 100

int contarPalabra(const char *txt, const char *str);
void ej1_8()
{
	char *txt =
		"Hola, soy un texto de prueba. Cuando alguien diga que una prueba es buena, desconfia. No tiene sentido pero esto es tambien una.\n";
	char *str = malloc(sizeof(char) * MAX);

	printf("%s", txt);
	printf("Ingrese una palabra del txt para contar: ");
	scanf("%s", str);
	int num = contarPalabra(txt, str);
	printf("La palabra '%s' aparece %d veces en el texto.\n", str, num);
	free(str);
}

int contarPalabra(const char *txt, const char *str)
{
	char *start = (char *)str;
	char *act = (char *)str; // Cast to char* to allow modification
	const char *end = str + strLen(str);

	int count = 0;
	while (*txt != '\0') {
		if (*txt == *start) {
			count++;
			while (*txt != ' ' && *txt != '\0' && act < end) {
				if (*txt != *act) {
					count--;
					break;
				}
				txt++;
				act++;
			}
		}
		txt++;
		act = start; // Reset act to the start of str
	}
	return count;
}
