#include "dron.h"
#include <stdio.h>
#include <stdlib.h>

int guardar_secuencia(char *nom, dron_t *d)
{
	FILE *f = fopen(nom, "rt");
	char aux[3];
	queue_init(&d->queue_sec);
	if (!f) {
		return -1;
	}
	while (fgets(aux, 3, f)) {
		if (queue_push(&d->queue_sec, &aux[0], sizeof(char)) == -1) {
			return -2;
		}
		if (aux[0] == '#') {
			if (aux[1] != '\n') {
				int c;
				while ((c = fgetc(f)) != '\n' && c != EOF)
					;
			}
		}
	}
	fclose(f);
	return 0;
}
static void mover_dron(const char dir, char *dir_dron)
{
	if (dir == 'A') { // Antihorario
		if (*dir_dron == NORTE)
			*dir_dron = OESTE;
		else if (*dir_dron == OESTE)
			*dir_dron = SUR;
		else if (*dir_dron == SUR)
			*dir_dron = ESTE;
		else if (*dir_dron == ESTE)
			*dir_dron = NORTE;
	} else if (dir == 'H') { // Horario
		if (*dir_dron == NORTE)
			*dir_dron = ESTE;
		else if (*dir_dron == ESTE)
			*dir_dron = SUR;
		else if (*dir_dron == SUR)
			*dir_dron = OESTE;
		else if (*dir_dron == OESTE)
			*dir_dron = NORTE;
	}
}
int procesar_secuencia(dron_t *d, queue_t *sec)
{
	int cont = 0;
	queue_init(sec);
	char dir;
	while (!queue_is_empty(&d->queue_sec) && d->dir != SUR) {
		queue_pull(&d->queue_sec, &dir, sizeof(char));
		switch (dir) {
		case 'D':
			cont = 1;
			d->dir = NORTE;
			queue_clear(sec);

			queue_push(sec, &dir, sizeof(char));
			break;
		case 'A':
		case 'H':
			mover_dron(dir, &d->dir);
			queue_push(sec, &dir, sizeof(char));
			cont++;
			break;
		case 'C':
			queue_pull(&d->queue_sec, &dir, sizeof(char));
			break;
		case '#':
			break;
		}
	}
	if (d->dir != SUR) {
		cont = -1;
	}
	return cont;
}
