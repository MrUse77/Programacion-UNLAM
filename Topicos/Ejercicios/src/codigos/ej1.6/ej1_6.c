
#include "../../headers/ejercicios.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../headers/strings.h"
/* Desarrollar una función que determine si una cadena de caracteres es un palíndromo.
*/
#define MAX 100

bool isPalindromo(const char *str);
void ej1_6()
{
	char *str = malloc(sizeof(char) * MAX);
	printf("Ingrese una cadena de caracteres: ");
	scanf("%s", str);
	if (isPalindromo(str)) {
		printf("La cadena '%s' es un palindromo.\n", str);
	} else {
		printf("La cadena '%s' no es un palindromo.\n", str);
	}
}

bool isPalindromo(const char *str)
{
	size_t len = stringSetLen(str);
	size_t i, j;
	bool isPalindromo = true;
	for (i = 0, j = len - 1; i < j; i++, j--) {
		if (str[i] != str[j]) {
			isPalindromo = false;
			break;
		}
	}
	return isPalindromo;
}
