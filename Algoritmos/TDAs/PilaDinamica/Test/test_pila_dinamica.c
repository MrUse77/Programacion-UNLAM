#include <stdio.h>
#include <string.h>
#include "Pila.h"
#include "test_framework.h"

// ============================================
// TESTS: Casos fronteras de Pila
// ============================================

TEST(pila_recien_creada_esta_vacia)
{
	// ARRANGE
	t_Pila p;

	// ACT
	crearPila(&p);

	// ASSERT
	ASSERT_TRUE(pilaVacia(&p), "Pila recién creada debe estar vacía");
	TEST_PASSED("Pila vacía después de crearPila()");
}

TEST(apilar_un_elemento_pila_no_vacia)
{
	// ARRANGE
	t_Pila p;
	int valor = 42;

	// ACT
	crearPila(&p);
	int resultado = apilar(&p, &valor, sizeof(valor));

	// ASSERT
	ASSERT_EQUAL(OK, resultado, "apilar() debe retornar OK");
	ASSERT_FALSE(pilaVacia(&p),
		     "Pila NO debe estar vacía después de apilar");
	TEST_PASSED("Apilar un elemento funciona correctamente");
}

TEST(desapilar_de_pila_vacia_retorna_error)
{
	// ARRANGE
	t_Pila p;
	int valor;

	// ACT
	crearPila(&p);
	int resultado = desapilar(&p, &valor, sizeof(valor));

	// ASSERT
	ASSERT_EQUAL(ERR_VACIA, resultado,
		     "desapilar() de pila vacía debe retornar ERR_VACIA");
	ASSERT_TRUE(pilaVacia(&p), "Pila debe seguir vacía");
	TEST_PASSED("Desapilar de pila vacía retorna ERR correctamente");
}

TEST(apilar_y_desapilar_mismo_valor)
{
	// ARRANGE
	t_Pila p;
	int valor_original = 999;
	int valor_recuperado;

	// ACT
	crearPila(&p);
	apilar(&p, &valor_original, sizeof(valor_original));
	int resultado =
		desapilar(&p, &valor_recuperado, sizeof(valor_recuperado));

	// ASSERT
	ASSERT_EQUAL(OK, resultado, "desapilar() debe retornar OK");
	ASSERT_EQUAL(valor_original, valor_recuperado,
		     "El valor desapilado debe ser el mismo que se apiló");
	ASSERT_TRUE(
		pilaVacia(&p),
		"Pila debe estar vacía después de desapilar único elemento");
	TEST_PASSED("Apilar y desapilar recupera el valor correcto");
}

TEST(apilar_multiples_elementos_orden_LIFO)
{
	// ARRANGE
	t_Pila p;
	int valores[] = { 10, 20, 30, 40, 50 };
	int recuperado;

	// ACT
	crearPila(&p);
	for (int i = 0; i < 5; i++) {
		apilar(&p, &valores[i], sizeof(valores[i]));
	}

	// ASSERT - Desapilar en orden inverso (LIFO: Last In, First Out)
	for (int i = 4; i >= 0; i--) {
		int resultado = desapilar(&p, &recuperado, sizeof(recuperado));
		ASSERT_EQUAL(OK, resultado, "desapilar() debe retornar OK");
		ASSERT_EQUAL(valores[i], recuperado, "El orden debe ser LIFO");
	}

	ASSERT_TRUE(pilaVacia(&p),
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

	t_Pila p;
	Persona original = { "Agustin", 25, 1.75 };
	Persona recuperada;

	// ACT
	crearPila(&p);
	int resultado_apilar = apilar(&p, &original, sizeof(original));
	int resultado_desapilar =
		desapilar(&p, &recuperada, sizeof(recuperada));

	// ASSERT
	ASSERT_EQUAL(OK, resultado_apilar, "apilar() debe retornar OK");
	ASSERT_EQUAL(OK, resultado_desapilar, "desapilar() debe retornar OK");
	ASSERT_TRUE(strcmp(original.nombre, recuperada.nombre) == 0,
		    "Nombre debe ser igual");
	ASSERT_EQUAL(original.edad, recuperada.edad, "Edad debe ser igual");
	TEST_PASSED("Apilar/desapilar estructuras complejas funciona");
}

TEST(apilar_hasta_llenar_pila_estatica)
{
	// ARRANGE
	t_Pila p;
	int valor = 1;
	int elementos_apilados = 0;

	// ACT
	crearPila(&p);

	// Apilar hasta que falle (si es pila estática)
	while (apilar(&p, &valor, sizeof(valor)) == OK) {
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
		int resultado = apilar(&p, &valor, sizeof(valor));
		ASSERT_EQUAL(ERR, resultado,
			     "Apilar en pila llena debe retornar ERR");
	}

	TEST_PASSED("Pila maneja correctamente el llenado");
}

TEST(ver_tope_no_modifica_pila)
{
	// ARRANGE
	t_Pila p;
	int valor_original = 777;
	int valor_tope;

	// ACT
	crearPila(&p);
	apilar(&p, &valor_original, sizeof(valor_original));

	// Verificar tope múltiples veces
	verTope(&p, &valor_tope, sizeof(valor_tope));
	ASSERT_EQUAL(valor_original, valor_tope,
		     "verTope() debe retornar el valor correcto");

	verTope(&p, &valor_tope, sizeof(valor_tope));
	ASSERT_EQUAL(valor_original, valor_tope,
		     "verTope() no debe modificar la pila");

	// ASSERT - La pila NO debe estar vacía
	ASSERT_FALSE(pilaVacia(&p), "verTope() no debe desapilar");

	TEST_PASSED("verTope() no modifica la pila");
}

TEST(vaciar_pila_deja_pila_vacia)
{
	// ARRANGE
	t_Pila p;
	int valores[] = { 1, 2, 3, 4, 5 };

	// ACT
	crearPila(&p);
	for (int i = 0; i < 5; i++) {
		apilar(&p, &valores[i], sizeof(valores[i]));
	}

	vaciarPila(&p);

	// ASSERT
	ASSERT_TRUE(pilaVacia(&p), "vaciarPila() debe dejar la pila vacía");
	TEST_PASSED("vaciarPila() funciona correctamente");
}

// ============================================
// MAIN: Ejecutar todos los tests
// ============================================

int main()
{
	printf("\n");
	printf("╔════════════════════════════════════════════════╗\n");
	printf("║   TEST SUITE: Pila Dinamica (TDA)              ║\n");
	printf("╔════════════════════════════════════════════════╝\n");

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
