#include <lista_simple.h>
#include <stdio.h>
#include <stdlib.h>
#define FIL 5
#define COL 6

typedef struct {
	int fil;
	int col;
	int valor;
} matriz;

int cmp_matriz(const void *a, const void *b)
{
	matriz *maa = (matriz *)a, *mab = (matriz *)b;
	if (maa->fil != mab->fil) {
		return mab->col - maa->col;
	}
	return mab->fil - mab->fil;
}
void menu()
{
	printf("Seleccione una opcion: \n");
	printf("1. Ver un valor de la matriz\n");
	printf("2. Ver elementos de una fila \n");
	printf("3. Ver elementos de una columna \n");
	printf("4. Ver todos los elementos de la lista \n");
	printf("0. Salir \n");
}

void cargar_matriz_a_lista(list_t *l)
{
	matriz ma;
	int m[FIL][COL] = {
		{ 0, 2, 0, 0, 7, 0 }, /* fila 0 */
		{ 5, 0, 0, 0, 0, 0 }, /* fila 1 */
		{ 0, 0, 0, 0, 0, 0 }, /* fila 2 — muerta: todo ceros */
		{ 0, 0, 3, 0, 1, 0 }, /* fila 3 */
		{ 0, 9, 0, 0, 0, 4 } /* fila 4 */
	};
	for (int i = 0; i < COL; i++) {
		for (int j = 0; j < FIL; j++) {
			ma.col = i;
			ma.fil = j;
			ma.valor = m[j][i];
			list_push_in_order(l, &ma, sizeof(matriz), cmp_matriz);
		}
	}
}

int seleccionar_opcion()
{
	int aux;
	do {
		scanf("%d", &aux);
		if (aux < 0 || aux > 4) {
			printf("Elegir una opcion correcta");
		}

	} while (aux < 0 || aux > 4);
	return aux;
}

void ver_valor(list_t *l)
{
	matriz ma;
	int res;

	printf("Ingrese fila: \n");
	do {
		scanf("%d", &ma.fil);

	} while (ma.fil < 0 || ma.fil > FIL);
	ma.fil--;
	printf("Ingrese columna: \n");
	do {
		scanf("%d", &ma.col);
	} while (ma.col < 0 || ma.col > COL);
	ma.col--;
	res = list_find(l, &ma, sizeof(matriz), cmp_matriz, FALSE);
	if (res == LIST_ERR_NOT_FOUND) {
		printf("Valor no encontrado");
		return;
	}
	printf("Valor en [%d][%d]: %d\n", ma.fil, ma.col, ma.valor);
}

void print_fila(void *data, void *param)
{
	int *fila = (int *)param;
	matriz *ma = (matriz *)data;
	if (*fila == ma->fil) {
		printf("%d ", ma->valor);
	}
}

void ver_fila(list_t *l)
{
	int fil;
	printf("Ingrese fila: \n");
	do {
		scanf("%d", &fil);
	} while (fil < 1 || fil > FIL);
	fil--;
	list_foreach(l, &fil, print_fila);
	printf("\n");
}

void print_columna(void *data, void *param)
{
	int *columna = (int *)param;
	matriz *ma = (matriz *)data;
	if (*columna == ma->col) {
		printf("%d ", ma->valor);
	}
}

void ver_columna(list_t *l)
{
	int col;
	printf("Ingrese columna: \n");
	do {
		scanf("%d", &col);
	} while (col < 1 || col > COL);
	col--;
	list_foreach(l, &col, print_columna);
	printf("\n");
}

void print_matriz(void *data, void *param)
{
	matriz *ma = (matriz *)data;
	printf("Pos [%d][%d]: %d\n", ma->fil, ma->col, ma->valor);
}

void ver_matriz(list_t *l)
{
	list_foreach(l, NULL, print_matriz);
	printf("\n");
}
void cargar_opcion(int opcion, list_t *l)
{
	switch (opcion) {
	case 1:
		ver_valor(l);
		printf("\n");
		break;
	case 2:
		ver_fila(l);
		printf("\n");
		break;
	case 3:
		ver_columna(l);
		printf("\n");
		break;
	case 4:
		ver_matriz(l);
		printf("\n");
		break;
	}
}

int main()
{
	list_t l;
	int opcion;
	list_init(&l);
	cargar_matriz_a_lista(&l);
	do {
		menu();
		opcion = seleccionar_opcion();
		cargar_opcion(opcion, &l);
	} while (opcion != 0);
}
