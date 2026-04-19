#include <stdio.h>
#include "Pila.h"
#include <string.h>
#define FILE_URL_TXT "prod.txt"
#define TAM_LINEA 65
#define ERR_LLENO -1
#define ERR_FILE -2

int sumar(t_Pila *p1, t_Pila *p2, t_Pila *pRes);
int cargarArchivoEnPila(const char *nombreArchivo, t_Pila *p)
{
	FILE *f = fopen(nombreArchivo, "rt");
	if (!f) {
		printf("Error: No se pudo abrir %s\n", nombreArchivo);
		return ERR_FILE;
	}
	char c;
	int code;
	while ((c = fgetc(f)) != EOF) {
		if (c >= '0' && c <= '9') {
			int num = c - '0';
			code = apilar(p, &num, sizeof(int));
			if (code != OK) {
				return code;
			}
		}
	}
	fclose(f);
	return OK;
}

int main()
{
	t_Pila p1, p2, pRes;
	crearPila(&p2);
	crearPila(&p1);
	int code;
	code = cargarArchivoEnPila("nro1.txt", &p1);
	if (code != OK) {
		return code;
	}
	code = cargarArchivoEnPila("nro2.txt", &p2);
	if (code != OK) {
		return code;
	}
	crearPila(&pRes);
	sumar(&p1, &p2, &pRes);
	return 0;
}

int sumar(t_Pila *p1, t_Pila *p2, t_Pila *pRes)
{
	// 2. Proceso de suma (Lógica de arquitectura de sistemas)
	int acarreo = 0;
	while (!pilaVacia(p1) || !pilaVacia(p2) || acarreo > 0) {
		int d1=0;
		desapilar(p1, &d1, sizeof(int));
		int d2=0;
		desapilar(p2, &d2, sizeof(int));

		int suma = d1 + d2 + acarreo;
		int digito = suma % 10;
		apilar(pRes, &digito, sizeof(int));
		acarreo = suma / 10;
	}

	// 3. Volcar resultado al archivo
	FILE *resF = fopen("resultado.txt", "w");
	if (!resF) {
		printf("Error al crear resultado.txt\n");
		return 1;
	}

	while (!pilaVacia(pRes)) {
		int res;
		desapilar(pRes, &res, sizeof(int));

		fprintf(resF, "%d", res);
	}

	fclose(resF);
	printf("Suma completada con exito en resultado.txt\n");

	return 0;
}
