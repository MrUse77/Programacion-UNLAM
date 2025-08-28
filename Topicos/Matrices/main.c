#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define FILAS 4
#define COLUMNAS 4
#define ORDEN 4
void **crearMatriz(size_t tamElem, int fil, int col);
void destruirMatriz(void **mat, int fil);
void newMatriz(int fil, int col, int **mat);
void mostrarMatriz(int **mat, int filas);
int sumaDiagonalPrincipal(int orden, int **mat);
int sumaTriangularInferiorPrincipal(int orden, int **mat);
int sumaTriangularInferiorPrincipalCD(int orden, int **matriz);
int sumaTriangularSuperiorPrincipal(int orden, int **matriz);
int sumaTriangularSuperiorPrincipalCD(int orden, int **matriz);
int sumaDiagonalSecundaria(int orden, int **mat);
int sumaTriangularSuperiorSecundaria(int orden, int **mat);
int sumaTriangularSuperiorSecundariaCD(int orden, int **matriz);
int sumaTriangularInferiorSecundaria(int orden, int **matriz);
int sumaTriangularInferiorSecundariaCD(int orden, int **matriz);
bool esMatrizDiagonal(int orden, int **matriz);
bool esMatrizIdentidad(int orden, int **matriz);

int main()
{
	//  int matriz[FILAS][COLUMNAS] = {
	//     {1, 2, 3, 4},
	//     {5, 6, 7, 8},
	//     {9, 10, 11, 12},
	//     {13, 14, 15, 16}};

	int orden = ORDEN;
	// matriz con punteros
	// int(*matriz)[COLUMNAS] = malloc(sizeof(int) * COLUMNAS * COLUMNAS); //
	int **matriz = (int **)crearMatriz(sizeof(int), FILAS, COLUMNAS);
	if (!matriz) {
		return 1;
	}
	newMatriz(orden, orden, matriz);
	mostrarMatriz(matriz, 4);

	int sumaSecundaria = sumaDiagonalSecundaria(4, matriz);
	printf("suma diagonal: %d\n", sumaDiagonalPrincipal(4, matriz));
	printf("suma diagonal secundaria: %d\n", sumaSecundaria);
	printf("suma triangular inferior principal: %d\n",
	       sumaTriangularInferiorPrincipal(4, matriz));
	printf("suma triangular superior principal: %d\n",
	       sumaTriangularSuperiorPrincipal(4, matriz));
	printf("suma triangular inferior principal con diagonal: %d\n",
	       sumaTriangularInferiorPrincipalCD(4, matriz));
	printf("suma triangular superior principal con diagonal: %d\n",
	       sumaTriangularSuperiorPrincipalCD(4, matriz));
	printf("suma triangular superior secundaria: %d\n",
	       sumaTriangularSuperiorSecundaria(4, matriz));
	printf("suma triangular superior secundaria con diagonal: %d\n",
	       sumaTriangularSuperiorSecundariaCD(4, matriz));
	printf("suma triangular inferior secundaria: %d\n",
	       sumaTriangularInferiorSecundaria(4, matriz));
	printf("suma triangular inferior secundaria con diagonal: %d\n",
	       sumaTriangularInferiorSecundariaCD(4, matriz));
	if (esMatrizDiagonal(4, matriz)) {
		printf("La matriz es diagonal\n");
	} else {
		printf("La matriz no es diagonal\n");
	}
	if (esMatrizIdentidad(4, matriz)) {
		printf("La matriz es identidad\n");
	} else {
		printf("La matriz no es identidad\n");
	}
	destruirMatriz((void **)matriz, FILAS);
	return 0;
}

bool esMatrizIdentidad(int orden, int **matriz)
{
	for (int i = 0; i < orden; i++) {
		for (int j = 0; j < orden; j++) {
			if (i == j) {
				if (matriz[i][j] != 1) {
					return false; // Si hay un elemento en la diagonal que no es 1, no es identidad
				}
			} else {
				if (matriz[i][j] != 0) {
					return false; // Si hay un elemento fuera de la diagonal que no es cero, no es identidad
				}
			}
		}
	}
	return true;
}

bool esMatrizDiagonal(int orden, int **matriz)
{
	for (int i = 0; i < orden; i++) {
		for (int j = 0; j < orden; j++) {
			if (i != j && matriz[i][j] != 0) {
				return false; // Si hay un elemento fuera de la diagonal que no es cero, no es diagonal
			} else if (i == j && matriz[i][j] == 0) {
				return false; // Si hay un elemento en la diagonal que es cero, no es diagonal
			}
		}
	}
	return true;
}

void mostrarMatriz(int **mat, int filas)
{
	int i, j;

	for (i = 0; i < filas; i++) {
		for (j = 0; j < COLUMNAS; j++) {
			printf("%3d", mat[i][j]);
		}
		putchar('\n');
	}
}

int sumaDiagonalPrincipal(int orden, int **mat)
{
	int suma = 0;
	for (int i = 0; i < orden; i++) {
		suma += mat[i][i];
	}
	return suma;
}

int sumaDiagonalSecundaria(int orden, int **matriz)
{
	int suma = 0;
	for (int i = 0; i < orden; i++) {
		suma += matriz[i][orden - 1 - i];
	}
	return suma;
}

int sumaTriangularInferiorPrincipalCD(int orden, int **matriz)
{
	int suma = 0;
	for (int i = 1; i < orden; i++) {
		for (int j = 0; j < i; j++) {
			suma += matriz[i][j];
		}
	}
	suma += sumaDiagonalPrincipal(orden, matriz);
	return suma;
}
int sumaTriangularInferiorPrincipal(int orden, int **matriz)
{
	int suma = 0;

	for (int i = 1; i < orden; i++) {
		for (int j = 0; j < i; j++) {
			suma += matriz[i][j];
		}
	}
	return suma;
}

int sumaTriangularSuperiorPrincipalCD(int orden, int **matriz)
{
	int suma = 0;

	for (int i = 1; i < orden; i++) {
		for (int j = 0; j < i; j++) {
			suma += matriz[j][i];
		}
	}
	suma += sumaDiagonalPrincipal(orden, matriz);
	return suma;
}
int sumaTriangularSuperiorPrincipal(int orden, int **matriz)
{
	int suma = 0;

	for (int i = 1; i < orden; i++) {
		for (int j = 0; j < i; j++) {
			suma += matriz[j][i];
		}
	}
	return suma;
}
int sumaTriangularInferiorSecundaria(int orden, int **matriz)
{
	int suma = 0;

	for (int i = orden - 1; i > 0; i--) {
		for (int j = orden - 1; j > orden - i - 1; j--) {
			suma += matriz[i][j];
		}
	}
	return suma;
}
int sumaTriangularInferiorSecundariaCD(int orden, int **matriz)
{
	int suma = 0;

	for (int i = orden - 1; i > 0; i--) {
		for (int j = orden - 1; j > orden - i - 1; j--) {
			suma += matriz[i][j];
		}
	}
	suma += sumaDiagonalSecundaria(orden, matriz);
	return suma;
}
int sumaTriangularSuperiorSecundaria(int orden, int **matriz)
{
	int suma = 0;

	for (int i = 0; i < orden; i++) {
		for (int j = 0; j < orden - 1 - i; j++) {
			suma += matriz[i][j];
		}
	}
	return suma;
}
int sumaTriangularSuperiorSecundariaCD(int orden, int **matriz)
{
	int suma = 0;

	for (int i = 0; i < orden; i++) {
		for (int j = 0; j < orden - 1 - i; j++) {
			suma += matriz[i][j];
		}
	}
	suma += sumaDiagonalSecundaria(orden, matriz);
	return suma;
}
void newMatriz(int fil, int col, int **mat)
{
	int cont = 1;
	for (int i = 0; i < fil; i++) {
		for (int j = 0; j < col; j++) {
			if (i == j) {
				// Si es diagonal, asignar el valor de la diagonal
				mat[i][j] = cont++;
			} else {
				// Si es superior a la diagonal, asignar 0
				mat[i][j] = 0;
			}
		}
	}
}

void **crearMatriz(size_t tamElem, int fil, int col)
{
	void **matriz = malloc(fil * sizeof(void *));
	if (!matriz) {
		return NULL;
	}
	void **ult = matriz + fil - 1;
	for (void **i = matriz; i <= ult; i++) {
		*i = malloc(tamElem * col);
		if (!*i) {
			destruirMatriz(matriz, i - matriz);
		}
	}
	return matriz;
}

void destruirMatriz(void **mat, int fil)
{
	void **ult = mat + fil - 1;
	for (void **i = mat; i <= ult; i++) {
		free(*i);
	}
	free(mat);
}
