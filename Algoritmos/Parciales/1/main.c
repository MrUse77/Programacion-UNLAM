#include "stack.h"
#include "containers.h"
#include <stdio.h>
#include <string.h>
#define CANT_COLS 3

int main()
{
	stack_t s[CANT_COLS], s_aux;
	int i = 0, n = 3, code_stack = 0,
	    cont_encontrado = 0; //n hardcodeado hasta hacer func de registro

	realizarMovimientos("movimientos.txt", CANT_COLS);

	// SIM INGRESO

	i = 0;
	do {
		code_stack = stack_is_empty(&s[i]);
		i++;
	} while (i < CANT_COLS && code_stack != TRUE);

	if (code_stack == FALSE) {
		perror("Codigo de anomalia: SCD");
		return -1;
	}
	stack_push(&s[i], &ejemplo, sizeof(container_t));

	// SIN EGRESO

	i = 0;
	do {
		if (stack_is_empty(&s[i])) {
			stack_pull(&s[i], &cont_aux, sizeof(container_t));
			if (strcmp(cont_aux.id, ejemplo.id) == 0) {
				cont_encontrado = 1;
				while (!stack_is_full(&s[i],
						      sizeof(container_t))) {
					stack_pull(&s_aux, &cont_aux,
						   sizeof(container_t));
					stack_push(&s[i], &cont_aux,
						   sizeof(container_t));
				}
			} else {
				stack_push(&s_aux, &cont_aux,
					   sizeof(container_t));
			}
		} else {
			while (!stack_is_full(&s[i], sizeof(container_t))) {
				stack_pull(&s_aux, &cont_aux,
					   sizeof(container_t));
				stack_push(&s[i], &cont_aux,
					   sizeof(container_t));
			}
			i++;
		}
	} while (i < CANT_COLS && !cont_encontrado);
	if (!cont_encontrado) {
		perror("Codigo de anomalia: SCD");
		return -1;
	}
}
