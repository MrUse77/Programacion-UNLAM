#include <stdio.h>
#include <stdlib.h>
#include "../Fecha/Fecha.h"
void ingresarEnteroPositivo0(int *num);

int main()
{
	Fecha fechaX, fechaY;
	int num;
	ingresarFecha(&fechaX);
	mostrarFecha(&fechaX);
	putchar('\n');
	ingresarEnteroPositivo0(&num);
	fechaY = fechaSumarDias(&fechaX, num);
	printf("Fecha %d dias despues: \n", num);
	mostrarFecha(&fechaY);
	int dif = fechaDiferencia(&fechaX, &fechaY);
	printf("\nHay %d dias de diferencia", dif);
	fechaDeTxt("19/07/2004", &fechaY);
	printf("fecha pasada por texto: %d/%d/%d", fechaY.dia, fechaY.mes,
	       fechaY.anio);

	return 0;
}

void ingresarEnteroPositivo0(int *num)
{
	do {
		printf("Ingrese un numero entero positivo o 0: ");
		scanf("%d", num);
	} while (*num < 0);
}
