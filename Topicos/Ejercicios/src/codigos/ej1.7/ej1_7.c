#include "../../headers/ejercicios.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../headers/strings.h"

#define MAX 100
void ej1_7()
{
	char *str = malloc(sizeof(char) * MAX);
	int resultInt = 0;
	float resultFloat = 0.0;
	printf("Ingrese una los numeros: ");
	scanf("%s", str);
	resultInt = strParseInt(str);
	if (resultInt != -1) {
		printf("El numero entero es: %d\n", resultInt);
	} else {
		resultFloat = strParseFloat(str);
		if (resultFloat != -1) {
			printf("El numero flotante es: %f\n", resultFloat);
		} else {
			printf("Error al parsear el numero.\n");
		}
	}
	free(str);
}
