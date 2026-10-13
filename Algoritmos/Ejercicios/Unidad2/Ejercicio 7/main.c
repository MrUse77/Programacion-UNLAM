#include <stdio.h>
#include "pila_estatica.h"
#include <string.h>
#define FILE_URL_TXT "prod.txt"
#define TAM_LINEA 65
#define ERR_LLENO -1
#define ERR_FILE -2
#define OK 0

int sumar(stack_t *p1, stack_t *p2, stack_t *pRes);
int cargarArchivoEnPila(const char *nombreArchivo, stack_t *p)
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
			code = stack_push(p, &num, sizeof(int));
			if (code != STACK_SUCCESS) {
				return code;
			}
		}
	}
	fclose(f);
	return OK;
}

int main()
{
	stack_t p1, p2, pRes;
	stack_init(&p2);
	stack_init(&p1);
	int code;
	code = cargarArchivoEnPila("nro1.txt", &p1);
	if (code != OK) {
		return code;
	}
	code = cargarArchivoEnPila("nro2.txt", &p2);
	if (code != OK) {
		return code;
	}
	stack_init(&pRes);
	sumar(&p1, &p2, &pRes);
	return 0;
}

int sumar(stack_t *p1, stack_t *p2, stack_t *pRes)
{
	// 2. Proceso de suma (Lógica de arquitectura de sistemas)
	int acarreo = 0;
	while (!stack_is_empty(p1) || !stack_is_empty(p2) || acarreo > 0) {
		int d1 = 0;
		stack_pop(p1, &d1, sizeof(int));
		int d2 = 0;
		stack_pop(p2, &d2, sizeof(int));

		int suma = d1 + d2 + acarreo;
		int digito = suma % 10;
		stack_push(pRes, &digito, sizeof(int));
		acarreo = suma / 10;
	}

	// 3. Volcar resultado al archivo
	FILE *resF = fopen("resultado.txt", "w");
	if (!resF) {
		printf("Error al crear resultado.txt\n");
		return 1;
	}

	while (!stack_is_empty(pRes)) {
		int res;
		stack_pop(pRes, &res, sizeof(int));

		fprintf(resF, "%d", res);
	}

	fclose(resF);
	printf("Suma completada con exito en resultado.txt\n");

	return 0;
}
