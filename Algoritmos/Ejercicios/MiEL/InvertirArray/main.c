#include <stdio.h>

void _invertirArray(int *ini, int *fin, int *aux)
{
	if (ini >= fin) {
		return;
	}
	*aux = *ini;
	*ini = *fin;
	*fin = *aux;
	printf("%d", *ini);
	_invertirArray(ini + 1, fin - 1, aux);
	printf("%d", *fin);
}

void invertirArray(int *arr, int tam)
{
	int aux;
	int *ini = arr;
	int *fin = arr + tam;
	if (ini == fin) {
		return;
	}
	_invertirArray(ini, fin, &aux);
}

int main()
{
	int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
	invertirArray(arr, 9);
}
