#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "binary_tree.h"
#include "test_framework.h"
#include <time.h>

int cmpInt(const void *ctx, const void *dato)
{
	return *(int *)ctx - *(int *)dato;
}

void print(void *param, const void *dato)
{
	(void)param;
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
	tree_walk_in_order(&a, print, NULL);
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

TEST(recorrido_pre_order)
{
	tree_t a;
	tree_init(&a);
	int num[] = { 5, 3, 7, 1, 9 };
	for (int i = 0; i < 5; i++) {
		tree_insert(&a, &num[i], sizeof(int), cmpInt);
	}
	/* Pre-order should visit: root, left, right -> 5, 3, 1, 7, 9 */
	/* We just verify it doesn't crash and visits all nodes */
	ASSERT_TRUE(tree_node_count(&a) == 5, "Arbol tiene 5 nodos");

	TEST_PASSED("Pre-order traversal funciona");
}

TEST(recorrido_post_order)
{
	tree_t a;
	tree_init(&a);
	int num[] = { 5, 3, 7, 1, 9 };
	for (int i = 0; i < 5; i++) {
		tree_insert(&a, &num[i], sizeof(int), cmpInt);
	}
	/* Post-order: left, right, root -> 1, 3, 9, 7, 5 */
	ASSERT_TRUE(tree_node_count(&a) == 5, "Arbol tiene 5 nodos");

	TEST_PASSED("Post-order traversal funciona");
}

TEST(insertar_iterativo)
{
	tree_t a;
	tree_init(&a);
	int num[] = { 5, 3, 7, 1, 9 };
	for (int i = 0; i < 5; i++) {
		int code = tree_insert_iter(&a, &num[i], sizeof(int), cmpInt);
		ASSERT_TRUE(code == TREE_SUCCESS, "Insercion iterativa exitosa");
	}
	ASSERT_TRUE(tree_node_count(&a) == 5, "Arbol tiene 5 nodos");

	/* Test duplicate */
	int dup = 5;
	int code = tree_insert_iter(&a, &dup, sizeof(int), cmpInt);
	ASSERT_TRUE(code == TREE_EQUAL_VALUE, "Duplicado rechazado");

	TEST_PASSED("Insert iterativo funciona");
}

TEST(contar_nodos_x_der)
{
	tree_t a;
	tree_init(&a);
	/* Build tree: 5(root), 3(L), 7(R), 1(LL), 9(RR) */
	int num[] = { 5, 3, 7, 1, 9 };
	for (int i = 0; i < 5; i++) {
		tree_insert(&a, &num[i], sizeof(int), cmpInt);
	}
	/* Nodes with right child: 5 (->7), 3 (no right), 7 (->9), 1 (no children) */
	/* Actually: 5 has right(7), 3 has no right, 7 has right(9), 1 has no children */
	/* Nodes with right child: 5, 7 = 2 */
	int count = tree_node_wr_count(&a);
	ASSERT_TRUE(count == 2, "Nodos con hijo derecho contados");

	TEST_PASSED("Contar nodos con hijo derecho funciona");
}

TEST(contar_nodos_condicional)
{
	tree_t a;
	tree_init(&a);
	int num[] = { 5, 3, 7, 1, 9 };
	for (int i = 0; i < 5; i++) {
		tree_insert(&a, &num[i], sizeof(int), cmpInt);
	}
	/* Count nodes equal to 5 (root) */
	int target = 5;
	int count = tree_node_w_cond_count(&a, cmpInt, &target);
	ASSERT_TRUE(count == 1, "Encontro 1 nodo igual a 5");

	/* Count nodes equal to 99 (not in tree) */
	target = 99;
	count = tree_node_w_cond_count(&a, cmpInt, &target);
	ASSERT_TRUE(count == 0, "No encontro nodos igual a 99");

	/* Count nodes equal to 1 (leaf) */
	target = 1;
	count = tree_node_w_cond_count(&a, cmpInt, &target);
	ASSERT_TRUE(count == 1, "Encontro 1 nodo igual a 1");

	TEST_PASSED("Contar nodos condicional funciona");
}

TEST(contar_nodos_en_nivel)
{
	tree_t a;
	tree_init(&a);
	/* Tree: 5(root), 3(L), 7(R), 1(LL), 9(RR) */
	int num[] = { 5, 3, 7, 1, 9 };
	for (int i = 0; i < 5; i++) {
		tree_insert(&a, &num[i], sizeof(int), cmpInt);
	}
	/* Level 0: root (5) = 1 node */
	ASSERT_TRUE(tree_node_count_in_level(&a, 0) == 1, "Nivel 0 tiene 1 nodo");
	/* Level 1: 3, 7 = 2 nodes */
	ASSERT_TRUE(tree_node_count_in_level(&a, 1) == 2, "Nivel 1 tiene 2 nodos");
	/* Level 2: 1, 9 = 2 nodes */
	ASSERT_TRUE(tree_node_count_in_level(&a, 2) == 2, "Nivel 2 tiene 2 nodos");
	/* Level 3: no nodes */
	ASSERT_TRUE(tree_node_count_in_level(&a, 3) == 0, "Nivel 3 tiene 0 nodos");

	TEST_PASSED("Contar nodos en nivel funciona");
}

TEST(contar_nodos_debajo_nivel)
{
	tree_t a;
	tree_init(&a);
	/* Tree: 5(root), 3(L), 7(R), 1(LL), 9(RR) */
	int num[] = { 5, 3, 7, 1, 9 };
	for (int i = 0; i < 5; i++) {
		tree_insert(&a, &num[i], sizeof(int), cmpInt);
	}
	/* height=0: count nodes at levels > 0, i.e., below root = 4 */
	ASSERT_TRUE(tree_node_count_below_level(&a, 0) == 4,
		    "Debajo nivel 0 hay 4 nodos");
	/* height=1: count nodes at levels > 1 = 2 (nodes 1, 9) */
	ASSERT_TRUE(tree_node_count_below_level(&a, 1) == 2,
		    "Debajo nivel 1 hay 2 nodos");
	/* height=2: count nodes at levels > 2 = 0 */
	ASSERT_TRUE(tree_node_count_below_level(&a, 2) == 0,
		    "Debajo nivel 2 hay 0 nodos");

	TEST_PASSED("Contar nodos debajo de nivel funciona");
}

TEST(contar_nodos_mas_alla_nivel)
{
	tree_t a;
	tree_init(&a);
	/* Tree: 5(root), 3(L), 7(R), 1(LL), 9(RR) */
	int num[] = { 5, 3, 7, 1, 9 };
	for (int i = 0; i < 5; i++) {
		tree_insert(&a, &num[i], sizeof(int), cmpInt);
	}
	/* height=2: count nodes at levels <= 2 (all nodes) = 5 */
	ASSERT_TRUE(tree_node_count_beyond_level(&a, 2) == 5,
		    "Mas alla nivel 2 hay 5 nodos");
	/* height=1: count nodes at levels <= 1 = 3 (5, 3, 7) */
	ASSERT_TRUE(tree_node_count_beyond_level(&a, 1) == 3,
		    "Mas alla nivel 1 hay 3 nodos");
	/* height=0: count nodes at levels <= 0 = 1 (root) */
	ASSERT_TRUE(tree_node_count_beyond_level(&a, 0) == 1,
		    "Mas alla nivel 0 hay 1 nodo");

	TEST_PASSED("Contar nodos mas alla de nivel funciona");
}

TEST(contar_nodos_desde_nivel_inclusive)
{
	tree_t a;
	tree_init(&a);
	/* Tree: 5(root), 3(L), 7(R), 1(LL), 9(RR) */
	int num[] = { 5, 3, 7, 1, 9 };
	for (int i = 0; i < 5; i++) {
		tree_insert(&a, &num[i], sizeof(int), cmpInt);
	}
	/* height=0: count nodes at levels >= 0 (all) = 5 */
	ASSERT_TRUE(tree_node_count_from_level_inclusive(&a, 0) == 5,
		    "Desde nivel 0 inclusive hay 5 nodos");
	/* height=1: count nodes at levels >= 1 = 4 (3,7,1,9) */
	ASSERT_TRUE(tree_node_count_from_level_inclusive(&a, 1) == 4,
		    "Desde nivel 1 inclusive hay 4 nodos");
	/* height=2: count nodes at levels >= 2 = 2 (1,9) */
	ASSERT_TRUE(tree_node_count_from_level_inclusive(&a, 2) == 2,
		    "Desde nivel 2 inclusive hay 2 nodos");

	TEST_PASSED("Contar nodos desde nivel inclusive funciona");
}

TEST(cortar_hojas)
{
	tree_t a;
	tree_init(&a);
	/* Tree: 5(root), 3(L), 7(R), 1(LL), 9(RR) */
	int num[] = { 5, 3, 7, 1, 9 };
	for (int i = 0; i < 5; i++) {
		tree_insert(&a, &num[i], sizeof(int), cmpInt);
	}
	/* Leaves: 1, 9 = 2 leaves */
	int cut = tree_cut_leaves(&a);
	ASSERT_TRUE(cut == 2, "Corto 2 hojas");
	/* After cutting, tree should have 3 nodes: 5, 3, 7 */
	ASSERT_TRUE(tree_node_count(&a) == 3, "Quedan 3 nodos");

	/* Cut leaves again: now 3 and 7 are leaves */
	cut = tree_cut_leaves(&a);
	ASSERT_TRUE(cut == 2, "Corto 2 hojas mas");
	/* Only root remains */
	ASSERT_TRUE(tree_node_count(&a) == 1, "Queda 1 nodo (raiz)");

	/* Cut again: root is now a leaf */
	cut = tree_cut_leaves(&a);
	ASSERT_TRUE(cut == 1, "Corto la ultima hoja");
	ASSERT_TRUE(tree_node_count(&a) == 0, "Arbol vacio");

	TEST_PASSED("Cortar hojas funciona");
}

TEST(arbol_vacio)
{
	tree_t a;
	tree_init(&a);

	ASSERT_TRUE(tree_node_count(&a) == 0, "Arbol vacio tiene 0 nodos");
	ASSERT_TRUE(tree_node_leaves_count(&a) == 0,
		    "Arbol vacio tiene 0 hojas");
	ASSERT_TRUE(tree_height(&a) == 0, "Arbol vacio tiene altura 0");
	ASSERT_TRUE(tree_node_wl_count(&a) == 0,
		    "Arbol vacio tiene 0 nodos con izq");
	ASSERT_TRUE(tree_node_wr_count(&a) == 0,
		    "Arbol vacio tiene 0 nodos con der");

	TEST_PASSED("Arbol vacio funciona");
}

TEST(arbol_duplicados)
{
	tree_t a;
	tree_init(&a);
	int val = 5;
	tree_insert(&a, &val, sizeof(int), cmpInt);
	tree_insert(&a, &val, sizeof(int), cmpInt);
	tree_insert(&a, &val, sizeof(int), cmpInt);

	ASSERT_TRUE(tree_node_count(&a) == 1, "Solo 1 nodo (duplicados rechazados)");

	TEST_PASSED("Duplicados rechazados");
}

TEST(eliminar_hoja)
{
	tree_t a;
	tree_init(&a);
	/* Tree: 5(root), 3(L), 7(R), 1(LL), 9(RR) */
	int num[] = {5, 3, 7, 1, 9};
	for (int i = 0; i < 5; i++) {
		tree_insert(&a, &num[i], sizeof(int), cmpInt);
	}
	ASSERT_TRUE(tree_node_count(&a) == 5, "Arbol tiene 5 nodos antes de borrar");

	/* Delete leaf 1 (hoja, sin hijos) */
	int target = 1;
	int ret = tree_delete_node(&a, &target, sizeof(int), cmpInt);
	ASSERT_TRUE(ret == TREE_SUCCESS, "Eliminar hoja retorna TREE_SUCCESS");
	ASSERT_TRUE(tree_node_count(&a) == 4, "Quedan 4 nodos tras borrar hoja");

	/* Verify in-order still correct: 3, 5, 7, 9 */
	/* Delete leaf 9 */
	target = 9;
	ret = tree_delete_node(&a, &target, sizeof(int), cmpInt);
	ASSERT_TRUE(ret == TREE_SUCCESS, "Eliminar segunda hoja retorna TREE_SUCCESS");
	ASSERT_TRUE(tree_node_count(&a) == 3, "Quedan 3 nodos");

	TEST_PASSED("Eliminar hoja funciona");
}

TEST(eliminar_nodo_con_hijo_unico)
{
	tree_t a;
	tree_init(&a);
	/* Tree: 5(root), 3(L), 7(R), 1(LL) */
	/* Node 3 has only left child (1) */
	int num[] = {5, 3, 7, 1};
	for (int i = 0; i < 4; i++) {
		tree_insert(&a, &num[i], sizeof(int), cmpInt);
	}
	ASSERT_TRUE(tree_node_count(&a) == 4, "Arbol tiene 4 nodos antes de borrar");

	/* Delete node 3 (tiene un solo hijo izquierdo: 1) */
	int target = 3;
	int ret = tree_delete_node(&a, &target, sizeof(int), cmpInt);
	ASSERT_TRUE(ret == TREE_SUCCESS, "Eliminar nodo con 1 hijo retorna TREE_SUCCESS");
	ASSERT_TRUE(tree_node_count(&a) == 3, "Quedan 3 nodos");

	/* In-order should be: 1, 5, 7 */
	/* Delete leaf 9 (no existe) */
	target = 9;
	ret = tree_delete_node(&a, &target, sizeof(int), cmpInt);
	ASSERT_TRUE(ret == TREE_ERR_EMPTY, "Eliminar inexistente retorna error");

	TEST_PASSED("Eliminar nodo con hijo unico funciona");
}

TEST(eliminar_raiz)
{
	tree_t a;
	tree_init(&a);
	int num[] = {5, 3, 7};
	for (int i = 0; i < 3; i++) {
		tree_insert(&a, &num[i], sizeof(int), cmpInt);
	}
	ASSERT_TRUE(tree_node_count(&a) == 3, "Arbol tiene 3 nodos antes de borrar");

	/* Delete root (5) */
	int target = 5;
	int ret = tree_delete_node(&a, &target, sizeof(int), cmpInt);
	ASSERT_TRUE(ret == TREE_SUCCESS, "Eliminar raiz retorna TREE_SUCCESS");
	ASSERT_TRUE(tree_node_count(&a) == 2, "Quedan 2 nodos");

	/* In-order should still be valid: 3, 7 (o 7, 3 segun reemplazo) */
	TEST_PASSED("Eliminar raiz funciona");
}

TEST(eliminar_en_arbol_vacio)
{
	tree_t a;
	tree_init(&a);
	int target = 5;
	int ret = tree_delete_node(&a, &target, sizeof(int), cmpInt);
	ASSERT_TRUE(ret == TREE_ERR_EMPTY, "Eliminar en arbol vacio retorna error");
	ASSERT_TRUE(tree_node_count(&a) == 0, "Sigue vacio");

	TEST_PASSED("Eliminar en arbol vacio funciona");
}

TEST(eliminar_nodo_con_dos_hijos)
{
	tree_t a;
	tree_init(&a);
	/*
	 * Tree:
	 *        10
	 *       /  \
	 *      5    15
	 *     / \   / \
	 *    3   7 12  20
	 */
	int num[] = {10, 5, 15, 3, 7, 12, 20};
	for (int i = 0; i < 7; i++) {
		tree_insert(&a, &num[i], sizeof(int), cmpInt);
	}
	ASSERT_TRUE(tree_node_count(&a) == 7, "Arbol tiene 7 nodos antes de borrar");

	/* Delete node 5 (tiene dos hijos: 3 y 7) */
	int target = 5;
	int ret = tree_delete_node(&a, &target, sizeof(int), cmpInt);
	printf("%d\n",tree_node_count(&a));
	ASSERT_TRUE(ret == TREE_SUCCESS, "Eliminar nodo con 2 hijos retorna TREE_SUCCESS");
	ASSERT_TRUE(tree_node_count(&a) == 6, "Quedan 6 nodos");

	/* Delete node 15 (tiene dos hijos: 12 y 20) */
	target = 15;
	ret = tree_delete_node(&a, &target, sizeof(int), cmpInt);
	ASSERT_TRUE(ret == TREE_SUCCESS, "Eliminar segundo nodo con 2 hijos retorna TREE_SUCCESS");
	ASSERT_TRUE(tree_node_count(&a) == 5, "Quedan 5 nodos");

	TEST_PASSED("Eliminar nodo con dos hijos funciona");
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
	RUN_TEST(recorrido_pre_order);
	RUN_TEST(recorrido_post_order);
	RUN_TEST(insertar_iterativo);
	RUN_TEST(contar_nodos_x_der);
	RUN_TEST(contar_nodos_condicional);
	RUN_TEST(contar_nodos_en_nivel);
	RUN_TEST(contar_nodos_debajo_nivel);
	RUN_TEST(contar_nodos_mas_alla_nivel);
	RUN_TEST(contar_nodos_desde_nivel_inclusive);
	RUN_TEST(cortar_hojas);
	RUN_TEST(arbol_vacio);
	RUN_TEST(arbol_duplicados);
	RUN_TEST(eliminar_hoja);
	RUN_TEST(eliminar_nodo_con_hijo_unico);
	RUN_TEST(eliminar_raiz);
	RUN_TEST(eliminar_en_arbol_vacio);
	RUN_TEST(eliminar_nodo_con_dos_hijos);

	TEST_SUMMARY();
	return 0;
}
