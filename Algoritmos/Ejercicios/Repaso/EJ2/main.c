#include <stdio.h>
#include <stdlib.h>

typedef void(Op)(float o1, float o2, float *res);
void *calcular(float o1, float o2, Op op);
void suma(float o1, float o2, float *res);
void resta(float o1, float o2, float *res);
void producto(float o1, float o2, float *res);
void division(float o1, float o2, float *res);

int main()
{
	printf("Hello world!\n");
	void *op = suma;
	int opcion = 1;
	float o1;
	float o2;
	while (opcion != 0) {
		printf("Introduzca operacion a realizar: \n");
		printf("1. Suma (Defecto) \n");
		printf("2. Resta \n");
		printf("3. Producto \n");
		printf("4. Division \n");

		printf("0. Cerrar \n");

		scanf("%d", &opcion);
		switch (opcion) {
		// case 1 por defecto ya seleccionado
		case 2:
			op = resta;
			break;
		case 3:
			op = producto;
			break;
		case 4:
			op = division;
			break;
		case 0:
			return 0;
		}
		printf("Introduzca N1 y N2 (si es division N2 es el divisor): \n");
		scanf("%f %f", &o1, &o2);
		void *res = calcular(o1, o2, op);
		if (res == NULL) {
			return -1;
		}
		printf("Res: %f", *(float *)res);
	}
	return 0;
}

void *calcular(float o1, float o2, Op op)
{
	float *res = malloc(sizeof(float));
	if (!o1 || !o2) {
		return 0;
	}

	op(o1, o2, res);
	return res;
}
void suma(float o1, float o2, float *res)
{
	*res = o1 + o2;
}
void resta(float o1, float o2, float *res)
{
	*res = o1 - o2;
}
void producto(float o1, float o2, float *res)
{
	if (o1 == 0 || o2 == 0) {
		*res = 0;
		return;
	}
	*res = o1 * o2;
}
void division(float o1, float o2, float *res)
{
	if (o2 == 0) {
		res = NULL;
		return;
	}
	if (o1 == 0) {
		*res = 0;
		return;
	}

	*res = o1 / o2;
}
