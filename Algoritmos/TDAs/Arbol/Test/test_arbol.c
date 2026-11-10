#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "binary_search_tree.h"
#include "test_framework.h"
#include <time.h>

int cmpInt(const void *ctx, const void *dato)
{
	return *(int *)ctx - *(int *)dato;
}

void print(void *dato, void *param)
{
	(void)param;
	printf("%d\n", *(int *)dato);
}

/* Acumulador de secuencias para verificar recorridos.
 *
 * Convencion del TDA: BST estandar. insert() manda al hijo IZQUIERDO cuando
 * cmp(ctx, d) > 0, o sea cuando d < ctx (menores a la izquierda), asi que el
 * in-order de este arbol es ASCENDENTE. */
#define BAL_MAX 64

typedef struct {
	int vals[BAL_MAX];
	int n;
} inorder_t;

static void recolectar_in_order(void *dato, void *param)
{
	inorder_t *acc = param;
	if (acc->n < BAL_MAX) {
		acc->vals[acc->n++] = *(int *)dato;
	}
}

static void recolectar(binary_search_tree_t *a, inorder_t *acc)
{
	acc->n = 0;
	binary_search_tree_walk_in_order(a, recolectar_in_order, acc);
}

/* Compara la secuencia acumulada contra el orden exacto esperado. */
static int secuencia_igual(const inorder_t *acc, const int *esperado, int n)
{
	int i;
	if (acc->n != n) {
		return 0;
	}
	for (i = 0; i < n; i++) {
		if (acc->vals[i] != esperado[i]) {
			return 0;
		}
	}
	return 1;
}

TEST(insertar_en_arbol)
{
	binary_search_tree_t a;
	binary_search_tree_init(&a);
	int i, num[] = { 2, 10, 8, 5, 9, 7, 6 };
	for (i = 0; i < 7; i++) {
		int code = binary_search_tree_insert(&a, &num[i], sizeof(int),
						     cmpInt);
		printf("\n");
		printf("code: %d\n", code);
		printf("%d\n", num[i]);
	}
	printf("\n");
	printf("In order\n");
	binary_search_tree_walk_in_order(&a, print, NULL);

	/* In-order ascendente esperado: 2, 5, 6, 7, 8, 9, 10 */
	int esperado[] = { 2, 5, 6, 7, 8, 9, 10 };
	inorder_t acc = { .n = 0 };
	binary_search_tree_walk_in_order(&a, recolectar_in_order, &acc);
	ASSERT_TRUE(secuencia_igual(&acc, esperado, 7),
		    "In-order devuelve 2,5,6,7,8,9,10");

	TEST_PASSED("Arbol sin duplicados");
}

TEST(contar_nodos)
{
	binary_search_tree_t a;
	binary_search_tree_init(&a);
	int i, num[] = { 2, 10, 8, 5, 9, 7, 6 }, cant = 7;
	for (i = 0; i < 7; i++) {
		int code = binary_search_tree_insert(&a, &num[i], sizeof(int),
						     cmpInt);
		printf("\n");
		printf("code: %d\n", code);
		printf("%d\n", num[i]);
	}
	printf("\n");
	i = binary_search_tree_node_count(&a);
	ASSERT_TRUE(i == cant, "Nodos contados");

	TEST_PASSED("Contar Nodos funciona ");
}

TEST(contar_hojas)
{
	binary_search_tree_t a;
	binary_search_tree_init(&a);
	int i, num[] = { 2, 10, 8, 5, 9, 7, 6 }, cant = 2;
	for (i = 0; i < 7; i++) {
		int code = binary_search_tree_insert(&a, &num[i], sizeof(int),
						     cmpInt);
		printf("\n");
		printf("code: %d\n", code);
		printf("%d\n", num[i]);
	}
	printf("\n");
	i = binary_search_tree_node_leaves_count(&a);
	printf("%d", i);
	ASSERT_TRUE(i == cant, "Nodos hojas contados");

	TEST_PASSED("Contar Nodos hojas funciona ");
}

TEST(contar_nodos_x_izq)
{
	binary_search_tree_t a;
	binary_search_tree_init(&a);
	int i, num[] = { 2, 10, 8, 5, 9, 7, 6 }, cant = 3;
	for (i = 0; i < 7; i++) {
		int code = binary_search_tree_insert(&a, &num[i], sizeof(int),
						     cmpInt);
		printf("\n");
		printf("code: %d\n", code);
		printf("%d\n", num[i]);
	}
	printf("\n");
	i = binary_search_tree_node_wl_count(&a);
	printf("%d", i);
	ASSERT_TRUE(i == cant, "Nodos contados");

	TEST_PASSED("Contar Nodos funciona ");
}

TEST(altura_arbol)
{
	binary_search_tree_t a;
	binary_search_tree_init(&a);
	int i, num[] = { 2, 10, 8, 5, 9, 7, 6 }, cant = 6;
	for (i = 0; i < 7; i++) {
		int code = binary_search_tree_insert(&a, &num[i], sizeof(int),
						     cmpInt);
		printf("\n");
		printf("code: %d\n", code);
		printf("%d\n", num[i]);
	}
	printf("\n");
	i = binary_search_tree_height(&a);
	printf("%d", i);
	ASSERT_TRUE(i == cant, "Altura contada");

	TEST_PASSED("Contar altura funciona ");
}

TEST(recorrido_pre_order)
{
	binary_search_tree_t a;
	binary_search_tree_init(&a);
	/* BST estandar: este arreglo arma 5(izq 3(izq 1), der 7(der 9)) */
	int num[] = { 5, 3, 7, 1, 9 }, esperado[] = { 5, 3, 1, 7, 9 }, i;
	inorder_t acc = { .n = 0 };

	for (i = 0; i < 5; i++) {
		binary_search_tree_insert(&a, &num[i], sizeof(int), cmpInt);
	}
	binary_search_tree_walk_pre_order(&a, recolectar_in_order, &acc);
	ASSERT_TRUE(secuencia_igual(&acc, esperado, 5),
		    "Pre-order: raiz, izquierda, derecha");

	TEST_PASSED("Pre-order traversal funciona");
}

TEST(recorrido_post_order)
{
	binary_search_tree_t a;
	binary_search_tree_init(&a);
	/* Mismo arbol que recorrido_pre_order: 5(izq 3(izq 1), der 7(der 9)) */
	int num[] = { 5, 3, 7, 1, 9 }, esperado[] = { 1, 3, 9, 7, 5 }, i;
	inorder_t acc = { .n = 0 };

	for (i = 0; i < 5; i++) {
		binary_search_tree_insert(&a, &num[i], sizeof(int), cmpInt);
	}
	binary_search_tree_walk_post_order(&a, recolectar_in_order, &acc);
	ASSERT_TRUE(secuencia_igual(&acc, esperado, 5),
		    "Post-order: izquierda, derecha, raiz");

	TEST_PASSED("Post-order traversal funciona");
}

TEST(contar_nodos_x_der)
{
	binary_search_tree_t a;
	binary_search_tree_init(&a);
	/* Build tree: 5(root), 3(L), 7(R), 1(LL), 9(RR) */
	int num[] = { 5, 3, 7, 1, 9 }, i;
	for (i = 0; i < 5; i++) {
		binary_search_tree_insert(&a, &num[i], sizeof(int), cmpInt);
	}
	/* Nodes with right child: 5 (->7), 3 (no right), 7 (->9), 1 (no children) */
	/* Actually: 5 has right(7), 3 has no right, 7 has right(9), 1 has no children */
	/* Nodes with right child: 5, 7 = 2 */
	int count = binary_search_tree_node_wr_count(&a);
	ASSERT_TRUE(count == 2, "Nodos con hijo derecho contados");

	TEST_PASSED("Contar nodos con hijo derecho funciona");
}

TEST(contar_nodos_condicional)
{
	binary_search_tree_t a;
	binary_search_tree_init(&a);
	int num[] = { 5, 3, 7, 1, 9 }, i;
	for (i = 0; i < 5; i++) {
		binary_search_tree_insert(&a, &num[i], sizeof(int), cmpInt);
	}
	/* Count nodes equal to 5 (root) */
	int target = 5;
	int count = binary_search_tree_node_w_cond_count(&a, cmpInt, &target);
	ASSERT_TRUE(count == 1, "Encontro 1 nodo igual a 5");

	/* Count nodes equal to 99 (not in tree) */
	target = 99;
	count = binary_search_tree_node_w_cond_count(&a, cmpInt, &target);
	ASSERT_TRUE(count == 0, "No encontro nodos igual a 99");

	/* Count nodes equal to 1 (leaf) */
	target = 1;
	count = binary_search_tree_node_w_cond_count(&a, cmpInt, &target);
	ASSERT_TRUE(count == 1, "Encontro 1 nodo igual a 1");

	TEST_PASSED("Contar nodos condicional funciona");
}

TEST(contar_nodos_en_nivel)
{
	binary_search_tree_t a;
	binary_search_tree_init(&a);
	/* Tree: 5(root), 3(L), 7(R), 1(LL), 9(RR) */
	int num[] = { 5, 3, 7, 1, 9 }, i;
	for (i = 0; i < 5; i++) {
		binary_search_tree_insert(&a, &num[i], sizeof(int), cmpInt);
	}
	/* Level 0: root (5) = 1 node */
	ASSERT_TRUE(binary_search_tree_node_count_in_level(&a, 0) == 1,
		    "Nivel 0 tiene 1 nodo");
	/* Level 1: 3, 7 = 2 nodes */
	ASSERT_TRUE(binary_search_tree_node_count_in_level(&a, 1) == 2,
		    "Nivel 1 tiene 2 nodos");
	/* Level 2: 1, 9 = 2 nodes */
	ASSERT_TRUE(binary_search_tree_node_count_in_level(&a, 2) == 2,
		    "Nivel 2 tiene 2 nodos");
	/* Level 3: no nodes */
	ASSERT_TRUE(binary_search_tree_node_count_in_level(&a, 3) == 0,
		    "Nivel 3 tiene 0 nodos");

	TEST_PASSED("Contar nodos en nivel funciona");
}

TEST(contar_nodos_debajo_nivel)
{
	binary_search_tree_t a;
	binary_search_tree_init(&a);
	/* Tree: 5(root), 3(L), 7(R), 1(LL), 9(RR) */
	int num[] = { 5, 3, 7, 1, 9 }, i;
	for (i = 0; i < 5; i++) {
		binary_search_tree_insert(&a, &num[i], sizeof(int), cmpInt);
	}
	/* height=0: count nodes at levels > 0, i.e., below root = 4 */
	ASSERT_TRUE(binary_search_tree_node_count_below_level(&a, 0) == 4,
		    "Debajo nivel 0 hay 4 nodos");
	/* height=1: count nodes at levels > 1 = 2 (nodes 1, 9) */
	ASSERT_TRUE(binary_search_tree_node_count_below_level(&a, 1) == 2,
		    "Debajo nivel 1 hay 2 nodos");
	/* height=2: count nodes at levels > 2 = 0 */
	ASSERT_TRUE(binary_search_tree_node_count_below_level(&a, 2) == 0,
		    "Debajo nivel 2 hay 0 nodos");

	TEST_PASSED("Contar nodos debajo de nivel funciona");
}

TEST(contar_nodos_mas_alla_nivel)
{
	binary_search_tree_t a;
	binary_search_tree_init(&a);
	/* Tree: 5(root), 3(L), 7(R), 1(LL), 9(RR) */
	int num[] = { 5, 3, 7, 1, 9 }, i;
	for (i = 0; i < 5; i++) {
		binary_search_tree_insert(&a, &num[i], sizeof(int), cmpInt);
	}
	printf("%d \n", binary_search_tree_node_count_beyond_level(&a, 2));
	/* "Por encima de h" = niveles < h: el nivel h NO se cuenta. */
	/* height=2: niveles 0 y 1 = 3 (5, 3, 7) */
	ASSERT_TRUE(binary_search_tree_node_count_beyond_level(&a, 2) == 3,
		    "Por encima del nivel 2 hay 3 nodos");
	/* height=1: nivel 0 = 1 (5) */
	ASSERT_TRUE(binary_search_tree_node_count_beyond_level(&a, 1) == 1,
		    "Por encima del nivel 1 hay 1 nodo");
	/* height=0: nada esta por encima del nivel 0 */
	ASSERT_TRUE(binary_search_tree_node_count_beyond_level(&a, 0) == 0,
		    "Por encima del nivel 0 hay 0 nodos");

	TEST_PASSED("Contar nodos mas alla de nivel funciona");
}

TEST(contar_nodos_desde_nivel_inclusive)
{
	binary_search_tree_t a;
	binary_search_tree_init(&a);
	/* Tree: 5(root), 3(L), 7(R), 1(LL), 9(RR) */
	int num[] = { 5, 3, 7, 1, 9 }, i;
	for (i = 0; i < 5; i++) {
		binary_search_tree_insert(&a, &num[i], sizeof(int), cmpInt);
	}
	/* height=0: count nodes at levels >= 0 (all) = 5 */
	ASSERT_TRUE(binary_search_tree_node_count_from_level_inclusive(&a, 0) ==
			    5,
		    "Desde nivel 0 inclusive hay 5 nodos");
	/* height=1: count nodes at levels >= 1 = 4 (3,7,1,9) */
	ASSERT_TRUE(binary_search_tree_node_count_from_level_inclusive(&a, 1) ==
			    4,
		    "Desde nivel 1 inclusive hay 4 nodos");
	/* height=2: count nodes at levels >= 2 = 2 (1,9) */
	ASSERT_TRUE(binary_search_tree_node_count_from_level_inclusive(&a, 2) ==
			    2,
		    "Desde nivel 2 inclusive hay 2 nodos");

	TEST_PASSED("Contar nodos desde nivel inclusive funciona");
}

TEST(cortar_hojas)
{
	binary_search_tree_t a;
	binary_search_tree_init(&a);
	/* Tree: 5(root), 3(L), 7(R), 1(LL), 9(RR) */
	int num[] = { 5, 3, 7, 1, 9 }, i;
	for (i = 0; i < 5; i++) {
		binary_search_tree_insert(&a, &num[i], sizeof(int), cmpInt);
	}
	/* Leaves: 1, 9 = 2 leaves */
	int cut = binary_search_tree_cut_leaves(&a);
	ASSERT_TRUE(cut == TREE_SUCCESS, "Corto 2 hojas");
	/* After cutting, tree should have 3 nodes: 5, 3, 7 */
	ASSERT_TRUE(binary_search_tree_node_count(&a) == 3, "Quedan 3 nodos");

	/* Cut leaves again: now 3 and 7 are leaves */
	cut = binary_search_tree_cut_leaves(&a);
	ASSERT_TRUE(cut == TREE_SUCCESS, "Corto 2 hojas mas");
	/* Only root remains */
	ASSERT_TRUE(binary_search_tree_node_count(&a) == 1,
		    "Queda 1 nodo (raiz)");

	/* Cut again: root is now a leaf */
	cut = binary_search_tree_cut_leaves(&a);
	ASSERT_TRUE(cut == TREE_SUCCESS, "Corto la ultima hoja");
	ASSERT_TRUE(binary_search_tree_node_count(&a) == 0, "Arbol vacio");

	TEST_PASSED("Cortar hojas funciona");
}

TEST(arbol_vacio)
{
	binary_search_tree_t a;
	binary_search_tree_init(&a);

	ASSERT_TRUE(binary_search_tree_node_count(&a) == 0,
		    "Arbol vacio tiene 0 nodos");
	ASSERT_TRUE(binary_search_tree_node_leaves_count(&a) == 0,
		    "Arbol vacio tiene 0 hojas");
	ASSERT_TRUE(binary_search_tree_height(&a) == 0,
		    "Arbol vacio tiene altura 0");
	ASSERT_TRUE(binary_search_tree_node_wl_count(&a) == 0,
		    "Arbol vacio tiene 0 nodos con izq");
	ASSERT_TRUE(binary_search_tree_node_wr_count(&a) == 0,
		    "Arbol vacio tiene 0 nodos con der");

	TEST_PASSED("Arbol vacio funciona");
}

TEST(arbol_duplicados)
{
	binary_search_tree_t a;
	binary_search_tree_init(&a);
	int val = 5;
	binary_search_tree_insert(&a, &val, sizeof(int), cmpInt);
	binary_search_tree_insert(&a, &val, sizeof(int), cmpInt);
	binary_search_tree_insert(&a, &val, sizeof(int), cmpInt);

	ASSERT_TRUE(binary_search_tree_node_count(&a) == 1,
		    "Solo 1 nodo (duplicados rechazados)");

	TEST_PASSED("Duplicados rechazados");
}

TEST(eliminar_hoja)
{
	binary_search_tree_t a;
	binary_search_tree_init(&a);
	/* Tree: 5(root), 3(L), 7(R), 1(LL), 9(RR) */
	int num[] = { 5, 3, 7, 1, 9 }, i;
	for (i = 0; i < 5; i++) {
		binary_search_tree_insert(&a, &num[i], sizeof(int), cmpInt);
	}
	ASSERT_TRUE(binary_search_tree_node_count(&a) == 5,
		    "Arbol tiene 5 nodos antes de borrar");

	/* Delete leaf 1 (hoja, sin hijos) */
	int target = 1;
	int ret = binary_search_tree_delete_node(&a, &target, sizeof(int),
						 cmpInt);
	ASSERT_TRUE(ret == TREE_SUCCESS, "Eliminar hoja retorna TREE_SUCCESS");
	ASSERT_TRUE(binary_search_tree_node_count(&a) == 4,
		    "Quedan 4 nodos tras borrar hoja");

	/* In-order ascendente esperado tras borrar el 1: 3, 5, 7, 9 */
	/* Delete leaf 9 */
	target = 9;
	ret = binary_search_tree_delete_node(&a, &target, sizeof(int), cmpInt);
	ASSERT_TRUE(ret == TREE_SUCCESS,
		    "Eliminar segunda hoja retorna TREE_SUCCESS");
	ASSERT_TRUE(binary_search_tree_node_count(&a) == 3, "Quedan 3 nodos");

	TEST_PASSED("Eliminar hoja funciona");
}

TEST(eliminar_nodo_con_hijo_unico)
{
	binary_search_tree_t a;
	binary_search_tree_init(&a);
	/* Tree: 5(root), 3(L), 7(R), 1(LL) */
	/* Node 3 has only left child (1) */
	int num[] = { 5, 3, 7, 1 }, i;
	for (i = 0; i < 4; i++) {
		binary_search_tree_insert(&a, &num[i], sizeof(int), cmpInt);
	}
	ASSERT_TRUE(binary_search_tree_node_count(&a) == 4,
		    "Arbol tiene 4 nodos antes de borrar");

	/* Delete node 3 (tiene un solo hijo izquierdo: 1) */
	int target = 3;
	int ret = binary_search_tree_delete_node(&a, &target, sizeof(int),
						 cmpInt);
	ASSERT_TRUE(ret == TREE_SUCCESS,
		    "Eliminar nodo con 1 hijo retorna TREE_SUCCESS");
	ASSERT_TRUE(binary_search_tree_node_count(&a) == 3, "Quedan 3 nodos");

	/* In-order ascendente esperado: 1, 5, 7 */
	/* Delete leaf 9 (no existe) */
	target = 9;
	ret = binary_search_tree_delete_node(&a, &target, sizeof(int), cmpInt);
	ASSERT_TRUE(ret == TREE_ERR_NOT_FOUND,
		    "Eliminar inexistente retorna error");

	TEST_PASSED("Eliminar nodo con hijo unico funciona");
}

TEST(eliminar_raiz)
{
	binary_search_tree_t a;
	binary_search_tree_init(&a);
	int num[] = { 5, 3, 7 }, i;

	for (i = 0; i < 3; i++) {
		binary_search_tree_insert(&a, &num[i], sizeof(int), cmpInt);
	}
	ASSERT_TRUE(binary_search_tree_node_count(&a) == 3,
		    "Arbol tiene 3 nodos antes de borrar");

	/* Delete root (5) */
	int target = 5;
	int ret = binary_search_tree_delete_node(&a, &target, sizeof(int),
						 cmpInt);
	ASSERT_TRUE(ret == TREE_SUCCESS, "Eliminar raiz retorna TREE_SUCCESS");
	ASSERT_TRUE(binary_search_tree_node_count(&a) == 2, "Quedan 2 nodos");

	/* In-order should still be valid: 3, 7 (o 7, 3 segun reemplazo) */
	TEST_PASSED("Eliminar raiz funciona");
}

TEST(eliminar_en_arbol_vacio)
{
	binary_search_tree_t a;
	binary_search_tree_init(&a);
	int target = 5;
	int ret = binary_search_tree_delete_node(&a, &target, sizeof(int),
						 cmpInt);
	ASSERT_TRUE(ret == TREE_ERR_EMPTY,
		    "Eliminar en arbol vacio retorna error");
	ASSERT_TRUE(binary_search_tree_node_count(&a) == 0, "Sigue vacio");

	TEST_PASSED("Eliminar en arbol vacio funciona");
}

TEST(eliminar_nodo_con_dos_hijos)
{
	binary_search_tree_t a;
	binary_search_tree_init(&a);
	/*
	 * Tree:
	 *        10
	 *       /  \
	 *      5    15
	 *     / \   / \
	 *    3   7 12  20
	 */
	int num[] = { 10, 5, 15, 3, 7, 12, 20 };
	for (int i = 0; i < 7; i++) {
		binary_search_tree_insert(&a, &num[i], sizeof(int), cmpInt);
	}
	ASSERT_TRUE(binary_search_tree_node_count(&a) == 7,
		    "Arbol tiene 7 nodos antes de borrar");

	/* Delete node 5 (tiene dos hijos: 3 y 7) */
	int target = 5;
	int ret = binary_search_tree_delete_node(&a, &target, sizeof(int),
						 cmpInt);
	printf("%d\n", binary_search_tree_node_count(&a));
	ASSERT_TRUE(ret == TREE_SUCCESS,
		    "Eliminar nodo con 2 hijos retorna TREE_SUCCESS");
	ASSERT_TRUE(binary_search_tree_node_count(&a) == 6, "Quedan 6 nodos");

	/* Delete node 15 (tiene dos hijos: 12 y 20) */
	target = 15;
	ret = binary_search_tree_delete_node(&a, &target, sizeof(int), cmpInt);
	printf("%d\n", binary_search_tree_node_count(&a));
	ASSERT_TRUE(ret == TREE_SUCCESS,
		    "Eliminar segundo nodo con 2 hijos retorna TREE_SUCCESS");
	ASSERT_TRUE(binary_search_tree_node_count(&a) == 5, "Quedan 5 nodos");

	TEST_PASSED("Eliminar nodo con dos hijos funciona");
}

/* ==========================================================================
 * Balanceo (DSW)
 *
 * Convencion del TDA: BST estandar. insert() manda al hijo IZQUIERDO cuando
 * d < ctx (menores a la izquierda) y al DERECHO cuando d > ctx. Por eso el
 * in-order devuelve los valores en orden ASCENDENTE.
 *
 * No se usa binary_search_tree_height() aca: hoy tiene un bug de precedencia
 * (1 + izq < der ? der : izq). La altura se deriva del conteo publico por
 * nivel, que si es correcto.
 * ========================================================================== */

/* Altura derivada de la cantidad de nodos por nivel. */
static int altura_por_niveles(const binary_search_tree_t *a)
{
	int h = 0;
	while (binary_search_tree_node_count_in_level(a, h) > 0) {
		h++;
	}
	return h;
}

/* Altura minima posible para n nodos: floor(log2(n)) + 1. */
static int altura_minima(int n)
{
	int h = 0;
	while ((1 << h) - 1 < n) {
		h++;
	}
	return h;
}

/* Todos los niveles salvo el ultimo deben estar completos. */
static int niveles_completos_salvo_ultimo(const binary_search_tree_t *a,
					  int altura)
{
	int nivel;
	for (nivel = 0; nivel < altura - 1; nivel++) {
		if (binary_search_tree_node_count_in_level(a, nivel) !=
		    (1 << nivel)) {
			return 0;
		}
	}
	return 1;
}

/* Verifica que el in-order sea exactamente 1, 2, ..., n. */
static int in_order_ascendente(const inorder_t *acc, int n)
{
	int i;
	if (acc->n != n) {
		return 0;
	}
	for (i = 0; i < acc->n; i++) {
		if (acc->vals[i] != i + 1) {
			return 0;
		}
	}
	return 1;
}

/* Inserta 1..n en forma ascendente: con esta convencion arma una vina
 * derecha (el peor caso). */
static void armar_vina_ascendente(binary_search_tree_t *a, int n)
{
	int i;
	binary_search_tree_init(a);
	for (i = 1; i <= n; i++) {
		binary_search_tree_insert(a, &i, sizeof(int), cmpInt);
	}
}

TEST(balancear_arbol_vacio)
{
	binary_search_tree_t a;
	binary_search_tree_init(&a);

	binary_search_tree_balanced(&a);

	ASSERT_TRUE(binary_search_tree_is_empty(&a),
		    "Un arbol vacio sigue vacio");
	ASSERT_EQUAL(0, binary_search_tree_node_count(&a), "Sin nodos");

	TEST_PASSED("Balancear un arbol vacio no hace nada");
}

TEST(balancear_un_solo_nodo)
{
	binary_search_tree_t a;
	inorder_t acc;
	int valor = 42;

	binary_search_tree_init(&a);
	binary_search_tree_insert(&a, &valor, sizeof(int), cmpInt);
	binary_search_tree_balanced(&a);
	recolectar(&a, &acc);

	ASSERT_EQUAL(1, binary_search_tree_node_count(&a),
		     "Conserva el unico nodo");
	ASSERT_EQUAL(1, altura_por_niveles(&a), "Altura 1");
	ASSERT_EQUAL(42, acc.vals[0], "Conserva el valor");

	TEST_PASSED("Balancear un solo nodo lo deja igual");
}

TEST(balancear_vina_ascendente_15)
{
	binary_search_tree_t a;
	inorder_t acc;

	armar_vina_ascendente(&a, 15);
	ASSERT_EQUAL(15, altura_por_niveles(&a),
		     "Antes de balancear es una vina de altura 15");

	binary_search_tree_balanced(&a);
	recolectar(&a, &acc);

	ASSERT_EQUAL(15, binary_search_tree_node_count(&a),
		     "Conserva los 15 nodos");
	ASSERT_TRUE(in_order_ascendente(&acc, 15),
		    "Conserva el orden in-order (1..15)");
	ASSERT_EQUAL(4, altura_por_niveles(&a), "Altura minima para 15 nodos");
	ASSERT_TRUE(niveles_completos_salvo_ultimo(&a, 4),
		    "Niveles completos salvo el ultimo");

	TEST_PASSED("Balancea una vina ascendente de 15 nodos");
}

TEST(balancear_vina_descendente_15)
{
	binary_search_tree_t a;
	inorder_t acc;
	int i;

	binary_search_tree_init(&a);
	for (i = 15; i >= 1; i--) {
		binary_search_tree_insert(&a, &i, sizeof(int), cmpInt);
	}
	ASSERT_EQUAL(15, altura_por_niveles(&a),
		     "Antes de balancear es una vina de altura 15");

	binary_search_tree_balanced(&a);
	recolectar(&a, &acc);

	ASSERT_EQUAL(15, binary_search_tree_node_count(&a),
		     "Conserva los 15 nodos");
	ASSERT_TRUE(in_order_ascendente(&acc, 15),
		    "Conserva el orden in-order (1..15)");
	ASSERT_EQUAL(4, altura_por_niveles(&a), "Altura minima para 15 nodos");
	ASSERT_TRUE(niveles_completos_salvo_ultimo(&a, 4),
		    "Niveles completos salvo el ultimo");

	TEST_PASSED("Balancea una vina descendente de 15 nodos");
}

TEST(balancear_arbol_ya_balanceado)
{
	binary_search_tree_t a;
	inorder_t acc;
	int i, valores[] = { 8, 4, 12, 2, 6, 10, 14 };

	binary_search_tree_init(&a);
	for (i = 0; i < 7; i++) {
		binary_search_tree_insert(&a, &valores[i], sizeof(int), cmpInt);
	}
	ASSERT_EQUAL(3, altura_por_niveles(&a),
		     "El arbol de partida ya esta balanceado");

	binary_search_tree_balanced(&a);
	recolectar(&a, &acc);

	ASSERT_EQUAL(7, binary_search_tree_node_count(&a),
		     "Conserva los 7 nodos");
	ASSERT_EQUAL(3, altura_por_niveles(&a), "Sigue con altura 3");
	ASSERT_TRUE(niveles_completos_salvo_ultimo(&a, 3),
		    "Sigue completo salvo el ultimo nivel");
	ASSERT_EQUAL(2, acc.vals[0], "Conserva el primer valor in-order");
	ASSERT_EQUAL(14, acc.vals[6], "Conserva el ultimo valor in-order");

	TEST_PASSED("Balancear un arbol ya balanceado no lo rompe");
}

/* n = 6 es el caso donde la primera compresion no alcanza: hacen falta las
 * compresiones sucesivas del segundo ciclo. */
TEST(balancear_seis_nodos)
{
	binary_search_tree_t a;
	inorder_t acc;

	armar_vina_ascendente(&a, 6);
	binary_search_tree_balanced(&a);
	recolectar(&a, &acc);

	ASSERT_EQUAL(6, binary_search_tree_node_count(&a),
		     "Conserva los 6 nodos");
	ASSERT_TRUE(in_order_ascendente(&acc, 6),
		    "Conserva el orden in-order (6..1)");
	ASSERT_EQUAL(3, altura_por_niveles(&a), "Altura minima para 6 nodos");
	ASSERT_TRUE(niveles_completos_salvo_ultimo(&a, 3),
		    "Niveles completos salvo el ultimo");

	TEST_PASSED("Balancea 6 nodos (requiere compresiones sucesivas)");
}

TEST(balancear_conserva_in_order)
{
	binary_search_tree_t a;
	inorder_t acc;
	int i, valores[] = { 5, 1, 9, 3, 7, 2, 8, 4, 10, 6 };

	binary_search_tree_init(&a);
	for (i = 0; i < 10; i++) {
		binary_search_tree_insert(&a, &valores[i], sizeof(int), cmpInt);
	}

	binary_search_tree_balanced(&a);
	recolectar(&a, &acc);

	ASSERT_EQUAL(10, binary_search_tree_node_count(&a),
		     "Conserva los 10 nodos");
	ASSERT_TRUE(in_order_ascendente(&acc, 10),
		    "El in-order sigue siendo 1..10");
	ASSERT_EQUAL(4, altura_por_niveles(&a), "Altura minima para 10 nodos");
	ASSERT_TRUE(niveles_completos_salvo_ultimo(&a, 4),
		    "Niveles completos salvo el ultimo");

	TEST_PASSED("Balancear conserva el recorrido in-order");
}

TEST(balancear_es_idempotente)
{
	binary_search_tree_t a;
	inorder_t antes, despues;
	int altura;

	armar_vina_ascendente(&a, 12);
	binary_search_tree_balanced(&a);
	recolectar(&a, &antes);
	altura = altura_por_niveles(&a);

	binary_search_tree_balanced(&a);
	recolectar(&a, &despues);

	ASSERT_EQUAL(12, binary_search_tree_node_count(&a),
		     "Conserva los 12 nodos");
	ASSERT_EQUAL(altura, altura_por_niveles(&a),
		     "La altura no cambia al balancear de nuevo");
	ASSERT_EQUAL(antes.n, despues.n, "Misma cantidad de valores");
	ASSERT_TRUE(memcmp(antes.vals, despues.vals, sizeof(int) * antes.n) ==
			    0,
		    "El in-order no cambia al balancear de nuevo");

	TEST_PASSED("Balancear dos veces es idempotente");
}

TEST(insertar_despues_de_balancear)
{
	binary_search_tree_t a;
	inorder_t acc;
	int i, valor = 100;

	armar_vina_ascendente(&a, 7);
	binary_search_tree_balanced(&a);
	binary_search_tree_insert(&a, &valor, sizeof(int), cmpInt);
	recolectar(&a, &acc);

	ASSERT_EQUAL(8, binary_search_tree_node_count(&a),
		     "Quedan 8 nodos tras insertar");
	ASSERT_EQUAL(100, acc.vals[acc.n - 1],
		     "El valor nuevo queda ultimo in-order");
	for (i = 0; i < acc.n - 1; i++) {
		ASSERT_EQUAL(i + 1, acc.vals[i],
			     "Se conserva el resto in-order");
	}
	ASSERT_EQUAL(4, altura_por_niveles(&a),
		     "Altura 4 para 8 nodos tras insertar");

	TEST_PASSED("El arbol balanceado sigue siendo un BST valido");
}

/* Barrido exhaustivo de tamanos: para todo n, el resultado debe tener altura
 * minima y todos los niveles completos salvo el ultimo. */
TEST(balancear_todos_los_tamanos)
{
	int n;

	for (n = 0; n <= 31; n++) {
		binary_search_tree_t a;
		inorder_t acc;
		int h;

		armar_vina_ascendente(&a, n);
		binary_search_tree_balanced(&a);
		recolectar(&a, &acc);
		h = altura_por_niveles(&a);

		printf("  n=%d altura=%d (minima=%d)\n", n, h,
		       altura_minima(n));
		ASSERT_EQUAL(n, binary_search_tree_node_count(&a),
			     "Conserva la cantidad de nodos");
		ASSERT_TRUE(in_order_ascendente(&acc, n),
			    "Conserva el orden in-order");
		ASSERT_EQUAL(altura_minima(n), h, "Alcanza la altura minima");
		ASSERT_TRUE(niveles_completos_salvo_ultimo(&a, h),
			    "Niveles completos salvo el ultimo");
	}

	TEST_PASSED("Balancea correctamente todos los tamanos de 0 a 31");
}

/* ==========================================================================
 * Propiedades estructurales: AVL, completo y balanceado
 *
 * Se testean las primitivas del TDA:
 *   binary_search_tree_is_avl(t)
 *   binary_search_tree_is_balanced(t)
 *   binary_search_tree_is_complete(t)
 *
 * Convencion (apunte): "completo hasta el nivel n" = existen todas las
 * posiciones de los niveles 0..n. Con eso:
 *   is_complete: todas las posiciones hasta el ULTIMO nivel -> perfecto.
 *   is_balanced: todas las posiciones hasta el PENULTIMO nivel -> todos los
 *                niveles llenos salvo, como maximo, el ultimo.
 *
 * Arboles de referencia (BST estandar):
 *   {8,4,12,2,6,10,14}  perfecto: AVL, balanceado y completo
 *   {8,4,12,2,6}        ultimo nivel parcial: balanceado, NO completo
 *   {8,4,12,6}          hueco en el ultimo nivel: NO balanceado, NO completo
 *   {3,2,1,4,5}         AVL, NO completo
 *   {4,2,6,7}           niveles 0 y 1 llenos: balanceado, NO completo
 *   {4,2,1,3}           NO AVL; tampoco balanceado (el nivel 1 no esta lleno)
 *   vina 1..10          ninguna de las tres hasta llamar a balanced()
 * ========================================================================== */

TEST(arbol_es_avl)
{
	binary_search_tree_t a;
	int perfecto[] = { 8, 4, 12, 2, 6, 10, 14 };
	int avl_no_completo[] = { 3, 2, 1, 4, 5 };
	int desbalanceado[] = { 4, 2, 1, 3 };
	int i;

	binary_search_tree_init(&a);
	for (i = 0; i < 7; i++) {
		binary_search_tree_insert(&a, &perfecto[i], sizeof(int), cmpInt);
	}
	ASSERT_TRUE(binary_search_tree_is_avl(&a) == TRUE,
		    "Un arbol perfecto es AVL");
	binary_search_tree_destroy(&a);

	binary_search_tree_init(&a);
	for (i = 0; i < 5; i++) {
		binary_search_tree_insert(&a, &avl_no_completo[i], sizeof(int),
					  cmpInt);
	}
	ASSERT_TRUE(binary_search_tree_is_avl(&a) == TRUE,
		    "Un AVL que no es completo sigue siendo AVL");
	binary_search_tree_destroy(&a);

	binary_search_tree_init(&a);
	for (i = 0; i < 4; i++) {
		binary_search_tree_insert(&a, &desbalanceado[i], sizeof(int),
					  cmpInt);
	}
	ASSERT_TRUE(binary_search_tree_is_avl(&a) == FALSE,
		    "Una raiz con 2 niveles a la izquierda y 0 a la derecha no es AVL");
	binary_search_tree_destroy(&a);

	armar_vina_ascendente(&a, 10);
	ASSERT_TRUE(binary_search_tree_is_avl(&a) == FALSE,
		    "Una vina de 10 nodos no es AVL");
	binary_search_tree_balanced(&a);
	ASSERT_TRUE(binary_search_tree_is_avl(&a) == TRUE,
		    "Despues de balancear, es AVL");
	binary_search_tree_destroy(&a);

	TEST_PASSED("is_avl distingue AVL de no-AVL");
}

TEST(arbol_es_completo)
{
	binary_search_tree_t a;
	int perfecto[] = { 8, 4, 12, 2, 6, 10, 14 };
	int completo[] = { 8, 4, 12, 2, 6 };
	int con_hueco[] = { 8, 4, 12, 6 };
	int i;

	binary_search_tree_init(&a);
	for (i = 0; i < 7; i++) {
		binary_search_tree_insert(&a, &perfecto[i], sizeof(int), cmpInt);
	}
	ASSERT_TRUE(binary_search_tree_is_complete(&a) == TRUE,
		    "Un arbol perfecto es completo");
	binary_search_tree_destroy(&a);

	binary_search_tree_init(&a);
	for (i = 0; i < 5; i++) {
		binary_search_tree_insert(&a, &completo[i], sizeof(int), cmpInt);
	}
	ASSERT_TRUE(binary_search_tree_is_complete(&a) == FALSE,
		    "5 nodos: el ultimo nivel parcial no es completo (es balanceado)");
	ASSERT_TRUE(binary_search_tree_is_balanced(&a) == TRUE,
		    "... aunque si es balanceado");
	binary_search_tree_destroy(&a);

	binary_search_tree_init(&a);
	for (i = 0; i < 4; i++) {
		binary_search_tree_insert(&a, &con_hueco[i], sizeof(int), cmpInt);
	}
	ASSERT_TRUE(binary_search_tree_is_complete(&a) == FALSE,
		    "El hijo derecho del 4 deja un hueco en el ultimo nivel");
	binary_search_tree_destroy(&a);

	armar_vina_ascendente(&a, 10);
	ASSERT_TRUE(binary_search_tree_is_complete(&a) == FALSE,
		    "Una vina de 10 nodos no es completa");
	binary_search_tree_balanced(&a);
	ASSERT_TRUE(binary_search_tree_is_complete(&a) == FALSE,
		    "Balancear 10 nodos deja un nivel parcial: no es perfecto");
	ASSERT_TRUE(binary_search_tree_is_balanced(&a) == TRUE,
		    "... pero si esta balanceado");
	binary_search_tree_destroy(&a);

	/* 7 = 2^3 - 1: el resultado del DSW es perfecto. */
	armar_vina_ascendente(&a, 7);
	binary_search_tree_balanced(&a);
	ASSERT_TRUE(binary_search_tree_is_complete(&a) == TRUE,
		    "Balancear 7 nodos da un arbol perfecto: completo");
	binary_search_tree_destroy(&a);

	TEST_PASSED("is_complete reconoce solo arboles perfectos");
}

TEST(arbol_es_balanceado)
{
	binary_search_tree_t a;
	int perfecto[] = { 8, 4, 12, 2, 6, 10, 14 };
	int balanceado_no_completo[] = { 4, 2, 6, 7 };
	int i;

	binary_search_tree_init(&a);
	for (i = 0; i < 7; i++) {
		binary_search_tree_insert(&a, &perfecto[i], sizeof(int), cmpInt);
	}
	ASSERT_TRUE(binary_search_tree_is_balanced(&a) == TRUE,
		    "Un arbol perfecto esta balanceado");
	binary_search_tree_destroy(&a);

	binary_search_tree_init(&a);
	for (i = 0; i < 4; i++) {
		binary_search_tree_insert(&a, &balanceado_no_completo[i],
					  sizeof(int), cmpInt);
	}
	ASSERT_TRUE(binary_search_tree_is_balanced(&a) == TRUE,
		    "Niveles 0 y 1 llenos: balanceado aunque el ultimo este a la derecha");
	ASSERT_TRUE(binary_search_tree_is_complete(&a) == FALSE,
		    "... y por eso mismo no es completo");
	binary_search_tree_destroy(&a);

	armar_vina_ascendente(&a, 10);
	ASSERT_TRUE(binary_search_tree_is_balanced(&a) == FALSE,
		    "Una vina de 10 nodos no esta balanceada");
	binary_search_tree_balanced(&a);
	ASSERT_TRUE(binary_search_tree_is_balanced(&a) == TRUE,
		    "Despues de balancear queda balanceado");
	binary_search_tree_destroy(&a);

	TEST_PASSED("is_balanced distingue balanceado de vina");
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
	RUN_TEST(balancear_arbol_vacio);
	RUN_TEST(balancear_un_solo_nodo);
	RUN_TEST(balancear_vina_ascendente_15);
	RUN_TEST(balancear_vina_descendente_15);
	RUN_TEST(balancear_arbol_ya_balanceado);
	RUN_TEST(balancear_seis_nodos);
	RUN_TEST(balancear_conserva_in_order);
	RUN_TEST(balancear_es_idempotente);
	RUN_TEST(insertar_despues_de_balancear);
	RUN_TEST(balancear_todos_los_tamanos);
	RUN_TEST(arbol_es_avl);
	RUN_TEST(arbol_es_completo);
	RUN_TEST(arbol_es_balanceado);

	TEST_SUMMARY();
	return 0;
}
