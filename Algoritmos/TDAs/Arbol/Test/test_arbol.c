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
	tree_t a;
	tree_init(&a);
	int i, num[] = { 2, 10, 8, 5, 9, 7, 6 };
	for (i = 0; i < 7; i++) {
		int code = tree_insert(&a, &num[i], sizeof(int), cmpInt);
		printf("\n");
		printf("code: %d\n", code);
		printf("%d\n", num[i]);
	}
	printf("\n");
	printf("In order\n");
	tree_walk_in_order(&a, print);
	TEST_PASSED("Arbol sin duplicados");
}

TEST(contar_nodos)
{
	tree_t a;
	tree_init(&a);
	int i, num[] = { 2, 10, 8, 5, 9, 7, 6 }, cant = 7;
	for (i = 0; i < 7; i++) {
		int code = tree_insert(&a, &num[i], sizeof(int), cmpInt);
		printf("\n");
		printf("code: %d\n", code);
		printf("%d\n", num[i]);
	}
	printf("\n");
	i = tree_node_count(&a);
	ASSERT_TRUE(i == cant, "Nodos contados");

	TEST_PASSED("Contar Nodos funciona ");
}

TEST(contar_hojas)
{
	tree_t a;
	tree_init(&a);
	int i, num[] = { 2, 10, 8, 5, 9, 7, 6 }, cant = 2;
	for (i = 0; i < 7; i++) {
		int code = tree_insert(&a, &num[i], sizeof(int), cmpInt);
		printf("\n");
		printf("code: %d\n", code);
		printf("%d\n", num[i]);
	}
	printf("\n");
	i = tree_node_leaves_count(&a);
	printf("%d", i);
	ASSERT_TRUE(i == cant, "Nodos hojas contados");

	TEST_PASSED("Contar Nodos hojas funciona ");
}

TEST(contar_nodos_x_izq)
{
	tree_t a;
	tree_init(&a);
	int i, num[] = { 2, 10, 8, 5, 9, 7, 6 }, cant = 3;
	for (i = 0; i < 7; i++) {
		int code = tree_insert(&a, &num[i], sizeof(int), cmpInt);
		printf("\n");
		printf("code: %d\n", code);
		printf("%d\n", num[i]);
	}
	printf("\n");
	i = tree_node_wl_count(&a);
	printf("%d", i);
	ASSERT_TRUE(i == cant, "Nodos contados");

	TEST_PASSED("Contar Nodos funciona ");
}

TEST(altura_arbol)
{
	tree_t a;
	tree_init(&a);
	int i, num[] = { 2, 10, 8, 5, 9, 7, 6 }, cant = 6;
	for (i = 0; i < 7; i++) {
		int code = tree_insert(&a, &num[i], sizeof(int), cmpInt);
		printf("\n");
		printf("code: %d\n", code);
		printf("%d\n", num[i]);
	}
	printf("\n");
	i = tree_height(&a);
	printf("%d", i);
	ASSERT_TRUE(i == cant, "Altura contada");

	TEST_PASSED("Contar altura funciona ");
}

int main()
{
	srand(time(NULL));
	printf("\n");
	printf("╔════════════════════════════════════════════════╗\n");
	printf("║   TEST SUITE: Arbol (TDA)                      ║\n");
	printf("╚════════════════════════════════════════════════╝\n");

	RUN_TEST(insertar_en_arbol);
	RUN_TEST(contar_nodos);
	RUN_TEST(contar_hojas);
	RUN_TEST(contar_nodos_x_izq);
	RUN_TEST(altura_arbol);

	TEST_SUMMARY();
	return 0;
}
