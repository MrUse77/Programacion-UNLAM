#include <stdio.h>
#include <stdlib.h>

int esta_ordenado(int *arr, int tam)
{
	printf("%d %d\n", *arr, *(arr + 1));
	if (tam - 1 <= 0) {
		return 0;
	}
	if (*arr > *(arr + 1)) {
		return -1;
	}
	return esta_ordenado(arr + 1, tam - 1);
}

int main()
{
	int arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int arr2[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
	int code, code2;
	code = esta_ordenado(arr, 10);
	if (code != 0) {
		printf("No esta ordenado\n");

	} else {
		printf("Esta ordenado\n");
	}

	code2 = esta_ordenado(arr2, 10);
	if (code2 != 0) {
		printf("No esta ordenado\n");

	} else {
		printf("Esta ordenado\n");
	}
}
