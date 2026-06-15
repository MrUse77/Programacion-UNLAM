#include "files.h"
#include <string.h>
#include <stdio.h>

int escribirLote(char *nom, lote_t *l, const int cant)
{
	FILE *f = fopen(nom, "wt");
	int i = 0;
	if (!f) {
		return -1;
	}
	fprintf(f, "%d", cant);
	for (; i < cant; i++) {
		fprintf(f, "%s-%s-%s", l->op, l->id, l->emp);
	}
	return 0;
}
