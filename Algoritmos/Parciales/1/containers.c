#include "containers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define INGRESO "IN"
#define EGRESO "EG"

static int ponerContenedor(stack_t *s, container_t *c, const int cant_cols)
{
	int i = 0;
	while (i < cant_cols && stack_is_full(&s[i], sizeof(container_t))) {
		i++;
	}
	if (i + 1 == cant_cols) {
		return -1;
	}
	stack_push(&s[i], c, sizeof(container_t));
	return 0;
}

static int sacarContenedor(stack_t *s, stack_t *s_aux, container_t *c,
			   const int cant_cols)
{
	int i = 0, encontrado = 0;
	container_t c_aux;
	while (i < cant_cols && encontrado == 0) {
		while (!stack_is_empty(&s[i])) {
			stack_pull(&s[i], &c_aux, sizeof(container_t));
			if (strcmp(c_aux.id, (*c).id) == 0) {
				encontrado = 1;
			} else {
				stack_push(s_aux, &c_aux, sizeof(container_t));
			}
		}
		while (!stack_is_full(&s[i], sizeof(container_t))) {
			stack_pull(s_aux, &c_aux, sizeof(container_t));
			stack_push(&s[i], &c_aux, sizeof(container_t));
		}
	}
	if (i + 1 == cant_cols && encontrado != 1) {
		return -1;
	}
	return 0;
}

void realizarMovimientos(char *nom, const int cant_cols)
{
	stack_t s_aux, s[cant_cols];
	int i = 0, max_alt = 0;
	char *aux = malloc(sizeof(lote_t) + 3), op[3];
	container_t c;
	FILE *f = fopen(nom, "wt");
	if (!f) {
		return;
	}
	fgets(aux, 1, f);
	max_alt = atoi(aux);
	stack_init(&s_aux, sizeof(container_t) * max_alt);
	for (; i < cant_cols; i++) {
		stack_init(&s[i], sizeof(container_t) * max_alt);
	}
	i = 0;

	while (!fgets(aux, sizeof(lote_t) + 2, f)) {
		sscanf(aux, "%s-%s-%s", op, c.id, c.name);
		if (strcmp(op, INGRESO) == 0) {
			int code = ponerContenedor(s, &c, cant_cols);
		} else {
			int code = sacarContenedor(s, &s_aux, &c, cant_cols);
		}
	}
}
