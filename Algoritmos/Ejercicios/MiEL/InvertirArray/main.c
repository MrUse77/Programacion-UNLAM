#include <string.h>
#include <stdio.h>

void invertirArray(int *ini, int *fin)
{
	int aux;
	if (ini >= fin) {
		return;
	}
	aux = *ini;
	*ini = *fin;
	*fin = aux;
	invertirArray(ini + 1, fin - 1);
	printf("%d", *ini);
}

int main()
{
	int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
	invertirArray(arr, arr + 9);
}
