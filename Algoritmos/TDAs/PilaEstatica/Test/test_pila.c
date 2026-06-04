#include <stdio.h>
#include <string.h>
#define PILA_ESTATICA
#include "Pila.h"
#include "test_framework.h"

// ============================================
// TESTS: Casos fronteras de Pila
// ============================================

TEST(pila_recien_creada_esta_vacia)
{
	// ARRANGE
	stack_t p;

	// ACT
	stack_init(&p);

	// ASSERT
	ASSERT_TRUE(stack_is_empty(&p), "Pila recién creada debe estar vacía");
	TEST_PASSED("Pila vacía después de stack_init()");
}

TEST(apilar_un_elemento_pila_no_vacia)
{
	// ARRANGE
	stack_t p;
	int valor = 42;

	// ACT
	stack_init(&p);
	int resultado = stack_push(&p, &valor, sizeof(valor));

	// ASSERT
	ASSERT_EQUAL(OK, resultado, "stack_push() debe retornar OK");
	ASSERT_FALSE(stack_is_empty(&p),
		     "Pila NO debe estar vacía después de apilar");
	TEST_PASSED("Apilar un elemento funciona correctamente");
}

TEST(desapilar_de_pila_vacia_retorna_error)
{
	// ARRANGE
	stack_t p;
	int valor;

	// ACT
	stack_init(&p);
	int resultado = stack_pull(&p, &valor, sizeof(valor));

	// ASSERT
	ASSERT_EQUAL(ERR_PILA_VACIA, resultado,
		     "stack_pull() de pila vacía debe retornar ERR");
	ASSERT_TRUE(stack_is_empty(&p), "Pila debe seguir vacía");
	TEST_PASSED("Desapilar de pila vacía retorna ERR correctamente");
}

TEST(apilar_y_desapilar_mismo_valor)
{
	// ARRANGE
	stack_t p;
	int valor_original = 999;
	int valor_recuperado;

	// ACT
	stack_init(&p);
	stack_push(&p, &valor_original, sizeof(valor_original));
	int resultado =
		stack_pull(&p, &valor_recuperado, sizeof(valor_recuperado));

	// ASSERT
	ASSERT_EQUAL(OK, resultado, "stack_pull() debe retornar OK");
	ASSERT_EQUAL(valor_original, valor_recuperado,
		     "El valor desapilado debe ser el mismo que se apiló");
	ASSERT_TRUE(
		stack_is_empty(&p),
		"Pila debe estar vacía después de desapilar único elemento");
	TEST_PASSED("Apilar y desapilar recupera el valor correcto");
}

TEST(apilar_multiples_elementos_orden_LIFO)
{
	// ARRANGE
	stack_t p;
	int valores[] = { 10, 20, 30, 40, 50 };
	int recuperado;

	// ACT
	stack_init(&p);
	for (int i = 0; i < 5; i++) {
		stack_push(&p, &valores[i], sizeof(valores[i]));
	}

	// ASSERT - Desapilar en orden inverso (LIFO: Last In, First Out)
	for (int i = 4; i >= 0; i--) {
		int resultado = stack_pull(&p, &recuperado, sizeof(recuperado));
		ASSERT_EQUAL(OK, resultado, "stack_pull() debe retornar OK");
		ASSERT_EQUAL(valores[i], recuperado, "El orden debe ser LIFO");
	}

	ASSERT_TRUE(stack_is_empty(&p),
		    "Pila debe estar vacía después de desapilar todo");
	TEST_PASSED("Múltiples elementos se apilan/desapilan en orden LIFO");
}

TEST(apilar_estructuras_complejas)
{
	// ARRANGE
	typedef struct {
		char nombre[20];
		int edad;
		float altura;
	} Persona;

	stack_t p;
	Persona original = { "Agustin", 25, 1.75 };
	Persona recuperada;

	// ACT
	stack_init(&p);
	int resultado_apilar = stack_push(&p, &original, sizeof(original));
	int resultado_desapilar =
		stack_pull(&p, &recuperada, sizeof(recuperada));

	// ASSERT
	ASSERT_EQUAL(OK, resultado_apilar, "stack_push() debe retornar OK");
	ASSERT_EQUAL(OK, resultado_desapilar, "stack_pull() debe retornar OK");
	ASSERT_TRUE(strcmp(original.nombre, recuperada.nombre) == 0,
		    "Nombre debe ser igual");
	ASSERT_EQUAL(original.edad, recuperada.edad, "Edad debe ser igual");
	TEST_PASSED("Apilar/desapilar estructuras complejas funciona");
}

TEST(apilar_hasta_llenar_pila_estatica)
{
	// ARRANGE
	stack_t p;
	int valor = 1;
	int elementos_apilados = 0;

	// ACT
	stack_init(&p);

	// Apilar hasta que falle (si es pila estática)
	while (stack_push(&p, &valor, sizeof(valor)) == OK) {
		elementos_apilados++;
		if (elementos_apilados > 10000) {
			// Evitar loop infinito si es pila dinámica
			printf("  → Pila parece ser dinámica (10000+ elementos)\n");
			break;
		}
	}

	// ASSERT
	if (elementos_apilados < 10000) {
		printf("  → Pila estática con capacidad: %d elementos\n",
		       elementos_apilados);

		// Intentar apilar uno más debe fallar
		int resultado = stack_push(&p, &valor, sizeof(valor));
		ASSERT_EQUAL(ERR_PILA_LLENA, resultado,
			     "Apilar en pila llena debe retornar ERR");
	}

	TEST_PASSED("Pila maneja correctamente el llenado");
}

TEST(ver_tope_no_modifica_pila)
{
	// ARRANGE
	stack_t p;
	int valor_original = 777;
	int valor_tope;

	// ACT
	stack_init(&p);
	stack_push(&p, &valor_original, sizeof(valor_original));

	// Verificar tope múltiples veces
	stack_see_top(&p, &valor_tope, sizeof(valor_tope));
	ASSERT_EQUAL(valor_original, valor_tope,
		     "verTope() debe retornar el valor correcto");

	stack_see_top(&p, &valor_tope, sizeof(valor_tope));
	ASSERT_EQUAL(valor_original, valor_tope,
		     "verTope() no debe modificar la pila");

	// ASSERT - La pila NO debe estar vacía
	ASSERT_FALSE(stack_is_empty(&p), "verTope() no debe desapilar");

	TEST_PASSED("verTope() no modifica la pila");
}

TEST(vaciar_pila_deja_pila_vacia)
{
	// ARRANGE
	stack_t p;
	int valores[] = { 1, 2, 3, 4, 5 };

	// ACT
	stack_init(&p);
	for (int i = 0; i < 5; i++) {
		stack_push(&p, &valores[i], sizeof(valores[i]));
	}

	stack_clear(&p);

	// ASSERT
	ASSERT_TRUE(stack_is_empty(&p), "vaciarPila() debe dejar la pila vacía");
	TEST_PASSED("vaciarPila() funciona correctamente");
}

// ============================================
// MAIN: Ejecutar todos los tests
// ============================================

int main()
{
	printf("\n");
	printf("╔════════════════════════════════════════╗\n");
	printf("║   TEST SUITE: Pila (TDA)              ║\n");
	printf("╔════════════════════════════════════════╝\n");

	RUN_TEST(pila_recien_creada_esta_vacia);
	RUN_TEST(apilar_un_elemento_pila_no_vacia);
	RUN_TEST(desapilar_de_pila_vacia_retorna_error);
	RUN_TEST(apilar_y_desapilar_mismo_valor);
	RUN_TEST(apilar_multiples_elementos_orden_LIFO);
	RUN_TEST(apilar_estructuras_complejas);
	RUN_TEST(apilar_hasta_llenar_pila_estatica);
	RUN_TEST(ver_tope_no_modifica_pila);
	RUN_TEST(vaciar_pila_deja_pila_vacia);

	TEST_SUMMARY();
}
