#include <UtilsPila.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define TAM_LINEA 1024
int crearPilaDesdeArchivo_TXT(t_Pila *p, const char *nomArch, int header,
			      tamElem obtenerTamElem, FmtTxt format)
{
	crearPila(p);
	FILE *f = fopen(nomArch, "rt");
	if (!f) {
		return ERR_FILE;
	}
	size_t cap = TAM_LINEA;
	size_t len = 0;
	char *linea = malloc(cap);
	char aux[TAM_LINEA];
	while (fgets(aux, TAM_LINEA, f)) {
		size_t trozo = strlen(aux);
		if (len + trozo + 1 > cap) {
			cap *= 2;
			char *tmp = realloc(linea, cap);
			if (!tmp) {
				free(linea);
				return ERR_MEM;
			}
			linea = tmp;
		}
		memcpy(linea + len, aux, trozo + 1);
		len += trozo;
		if (strchr(aux, '\n') != NULL) {
			if (header != 1) {
				size_t tam = obtenerTamElem(linea);
				void *elem = malloc(tam);
				if (!elem) {
					return ERR_MEM;
				}
				format(elem, linea);
				int code = apilar(p, elem, tam);
				if (code != OK) {
					return ERR_PIL_LLENA;
				}
				free(elem);
			} else {
				header--;
			}
			len = 0;
		}
	}
	free(linea);
	fclose(f);
	return OK;
}

int crearPilaDesdeArchivo_BIN(t_Pila *p, const char *nomArch,
			      tamElem obtenerTamElem)
{
	crearPila(p);
	FILE *f = fopen(nomArch, "rb");
	if (!f) {
		return ERR_FILE;
	}

	size_t tam = obtenerTamElem(NULL);
	void *elem = malloc(tam);
	if (!elem) {
		return ERR_MEM;
	}
	while (fread(elem, tam, 1, f)) {
		int code = apilar(p, elem, tam);
		if (code != OK) {
			return ERR_PIL_LLENA;
		}
	}
	fclose(f);
	return OK;
}
