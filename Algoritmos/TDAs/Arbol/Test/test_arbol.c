#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Arbol.h"
#include "test_framework.h"
#include <time.h>

int cmpInt(const void *buff, const void *dato)
{
	return *(int *)buff - *(int *)dato;
}

void print(const void *dato)
{
	printf("%d\n", *(int *)dato);
}

TEST(insertar_en_arbol)
{
	t_Arbol a;
	crearArbol(&a);
	int i, num[] = { 2, 10, 8, 5, 9, 7, 6 };
	for (i = 0; i < 7; i++) {
		int code = insertarEnArbol(&a, &num[i], sizeof(int), cmpInt);
		printf("\n");
		printf("code: %d\n", code);
		printf("%d\n", num[i]);
	}
	printf("\n");
	printf("In order\n");
	recorrerArbolInOrder(&a, print);
	TEST_PASSED("Arbol sin duplicados");
}

int main()
{
	srand(time(NULL));
	printf("\n");
	printf("╔════════════════════════════════════════════════╗\n");
	printf("║   TEST SUITE: Arbol (TDA)                      ║\n");
	printf("╚════════════════════════════════════════════════╝\n");

	RUN_TEST(insertar_en_arbol);

	TEST_SUMMARY();
	return 0;
}
