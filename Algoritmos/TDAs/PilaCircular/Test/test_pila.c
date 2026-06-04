#include <stdio.h>
#include <string.h>
#include <Pila.h>
#include "test_framework.h"

// ============================================
// TESTS: Casos fronteras de Pila
// ============================================

TEST(stack_recien_creado_esta_vacio)
{
	// ARRANGE
	stack_t s;

	// ACT
	stack_init(&s);

	// ASSERT
	ASSERT_TRUE(stack_is_empty(&s), "Pila recién creada debe estar vacía");
	TEST_PASSED("Stack vacío después de stack_init()");
}

TEST(push_un_elemento_stack_no_vacio)
{
	// ARRANGE
	stack_t s;
	int valor = 42;

	// ACT
	stack_init(&s);
	int resultado = stack_push(&s, &valor, sizeof(valor));

	// ASSERT
	ASSERT_EQUAL(OK, resultado, "stack_push() debe retornar OK");
	ASSERT_FALSE(stack_is_empty(&s),
		     "Pila NO debe estar vacía después de push");
	TEST_PASSED("Push un elemento funciona correctamente");
}

TEST(pop_de_stack_vacia_retorna_error)
{
	// ARRANGE
	stack_t s;
	int valor;

	// ACT
	stack_init(&s);
	int resultado = stack_pull(&s, &valor, sizeof(valor));

	// ASSERT
	ASSERT_EQUAL(ERR_PILA_VACIA, resultado,
		     "pop() de stack vacía debe retornar ERR_VACIA");
	ASSERT_TRUE(stack_is_empty(&s), "Pila debe seguir vacía");
	TEST_PASSED("Pop de pila vacía retorna error correctamente");
}

TEST(push_y_pop_mismo_valor)
{
	// ARRANGE
	stack_t s;
	int valor_original = 999;
	int valor_recuperado;

	// ACT
	stack_init(&s);
	stack_push(&s, &valor_original, sizeof(valor_original));
	int resultado =
		stack_pull(&s, &valor_recuperado, sizeof(valor_recuperado));

	// ASSERT
	ASSERT_EQUAL(OK, resultado, "pop() debe retornar OK");
	ASSERT_EQUAL(valor_original, valor_recuperado,
		     "El valor poppeado debe ser el mismo que se pushó");
	ASSERT_TRUE(stack_is_empty(&s),
		    "Stack debe estar vacío después de pop único elemento");
	TEST_PASSED("Push y pop recupera el valor correcto");
}

TEST(push_multiples_elementos_orden_LIFO)
{
	// ARRANGE
	stack_t s;
	int valores[] = { 10, 20, 30, 40, 50 };
	int recuperado;

	// ACT
	stack_init(&s);
	for (int i = 0; i < 5; i++) {
		stack_push(&s, &valores[i], sizeof(valores[i]));
	}

	// ASSERT - Pop en orden inverso (LIFO: Last In, First Out)
	for (int i = 4; i >= 0; i--) {
		int resultado = stack_pull(&s, &recuperado, sizeof(recuperado));
		ASSERT_EQUAL(OK, resultado, "pop() debe retornar OK");
		ASSERT_EQUAL(valores[i], recuperado, "El orden debe ser LIFO");
	}

	ASSERT_TRUE(stack_is_empty(&s),
		    "Pila debe estar vacía después de pop todo");
	TEST_PASSED("Múltiples elementos se pushan/popean en orden LIFO");
}

TEST(push_estructuras_complejas)
{
	// ARRANGE
	typedef struct {
		char nombre[20];
		int edad;
		float altura;
	} Persona;

	stack_t s;
	Persona original = { "Agustin", 25, 1.75 };
	Persona recuperado;

	// ACT
	stack_init(&s);
	int resultado_push = stack_push(&s, &original, sizeof(original));
	int resultado_pop = stack_pull(&s, &recuperado, sizeof(recuperado));

	// ASSERT
	ASSERT_EQUAL(OK, resultado_push, "push() debe retornar OK");
	ASSERT_EQUAL(OK, resultado_pop, "pop() debe retornar OK");
	ASSERT_TRUE(strcmp(original.nombre, recuperado.nombre) == 0,
		    "Nombre debe ser igual");
	ASSERT_EQUAL(original.edad, recuperado.edad, "Edad debe ser igual");
	TEST_PASSED("Push/pop estructuras complejas funciona");
}

TEST(push_hasta_llenar_stack_estatica)
{
	// ARRANGE
	stack_t s;
	int valor = 1;
	int elementos_pushados = 0;

	// ACT
	stack_init(&s);

	// Push hasta que falle (si es stack estático)
	while (stack_push(&s, &valor, sizeof(valor)) == OK) {
		elementos_pushados++;
		if (elementos_pushados > 10000) {
			// Evitar loop infinito si es stack dinámico
			printf("  → Pila parece ser dinámica (10000+ elementos)\n");
			break;
		}
	}

	// ASSERT
	if (elementos_pushados < 10000) {
		printf("  → Stack estático con capacidad: %d elementos\n",
		       elementos_pushados);

		// Intentar push uno más debe fallar
		int resultado = stack_push(&s, &valor, sizeof(valor));
		ASSERT_EQUAL(ERR_MEM_LLENA, resultado,
			     "Push en stack lleno debe retornar ERR");
	}

	TEST_PASSED("Stack maneja correctamente el llenado");
}

TEST(peek_no_modifica_stack)
{
	// ARRANGE
	stack_t s;
	int valor_original = 777;
	int valor_tope;

	// ACT
	stack_init(&s);
	stack_push(&s, &valor_original, sizeof(valor_original));

	// Verificar tope múltiples veces
	stack_see_top(&s, &valor_tope, sizeof(valor_tope));
	ASSERT_EQUAL(valor_original, valor_tope,
		     "peek() debe retornar el valor correcto");

	stack_see_top(&s, &valor_tope, sizeof(valor_tope));
	ASSERT_EQUAL(valor_original, valor_tope,
		     "peek() no debe modificar la pila");

	// ASSERT - El stack NO debe estar vacío
	ASSERT_FALSE(stack_is_empty(&s), "peek() no debe poppear");

	TEST_PASSED("peek() no modifica el stack");
}

TEST(clear_deja_stack_vacio)
{
	// ARRANGE
	stack_t s;
	int valores[] = { 1, 2, 3, 4, 5 };

	// ACT
	stack_init(&s);
	for (int i = 0; i < 5; i++) {
		stack_push(&s, &valores[i], sizeof(valores[i]));
	}

	stack_clear(&s);

	// ASSERT
	ASSERT_TRUE(stack_is_empty(&s), "clear() debe dejar el stack vacío");
	TEST_PASSED("clear() funciona correctamente");
}

// ============================================
// MAIN: Ejecutar todos los tests
// ============================================

int main()
{
	printf("\n");
	printf("╔════════════════════════════════════════╗\n");
	printf("║   TEST SUITE: Stack (TDA)              ║\n");
	printf("╚════════════════════════════════════════╝\n");

	RUN_TEST(stack_recien_creado_esta_vacio);
	RUN_TEST(push_un_elemento_stack_no_vacio);
	RUN_TEST(pop_de_stack_vacia_retorna_error);
	RUN_TEST(push_y_pop_mismo_valor);
	RUN_TEST(push_multiples_elementos_orden_LIFO);
	RUN_TEST(push_estructuras_complejas);
	RUN_TEST(push_hasta_llenar_stack_estatica);
	RUN_TEST(peek_no_modifica_stack);
	RUN_TEST(clear_deja_stack_vacio);

	TEST_SUMMARY();
}
