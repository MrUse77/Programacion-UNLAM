
#include "../../headers/ejercicios.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../headers/strings.h"

#define MAX_LENGTH 100

bool esLetra(char c);
void ej1_10()
{
	char txt[] =
		"Nj qemh v ljs kraenkqbres; lj oqe qemh es oqevorme sgn ellhs --Istqt Asdmgj";
	char *p = txt;
	char *desofuscado = "abcdghijkoqtuv";
	while (*p != '\0') {
		int count = 1;
		while (esLetra(*p)) {
			int index = strIndexOf(desofuscado, *p);
			if (index != -1) {
				if (index + count >= strLen(desofuscado)) {
					*p = *(desofuscado + index + count -
					       strLen(desofuscado));
				} else {
					*p = *(desofuscado + index + count);
				}
			}
			count++;
			p++;
		}
		p++;
	}

	printf("Res: %s", txt);
}

bool esLetra(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
