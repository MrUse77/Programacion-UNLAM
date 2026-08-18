
#include <test_framework.h>
#include <pila_estatica.h>

TEST(pila_esta_vacia_al_iniciar)
{
	stack_t q;
	stack_init(&q);
	ASSERT_EQUAL(TRUE, stack_is_empty(&q),
		     "La pila debe estar vacia al iniciar");
	TEST_PASSED("Cola vacia despues de iniciarla");
}

TEST(poner_elemento_en_pila)
{
	stack_t q;
	stack_init(&q);
	int num = 7;
	ASSERT_EQUAL(STACK_SUCCESS, stack_push(&q, &num, sizeof(int)),
		     "No se encolo correctamente el elemento");
	TEST_PASSED("Elemento colocado en la pila");
}

TEST(sacar_elemento_en_pila)
{
	stack_t q;
	stack_init(&q);
	int num = 7, aux;
	ASSERT_EQUAL(STACK_SUCCESS, stack_push(&q, &num, sizeof(int)),
		     "No se encolo correctamente el elemento");
	ASSERT_EQUAL(STACK_SUCCESS, stack_pull(&q, &aux, sizeof(int)),
		     "No se desencolo correctamente el elemento");
	ASSERT_EQUAL(num, aux, "No se desencolo correctamente el elemento");
	TEST_PASSED("Elemento desencolocado de la pila");
}

TEST(desapilar_unico_elemento_deja_pila_vacia)
{
	stack_t q;
	stack_init(&q);
	int num = 7, aux;
	ASSERT_EQUAL(STACK_SUCCESS, stack_push(&q, &num, sizeof(int)),
		     "No se encolo correctamente el elemento");
	ASSERT_EQUAL(STACK_SUCCESS, stack_pull(&q, &aux, sizeof(int)),
		     "No se desencolo correctamente el elemento");
	ASSERT_EQUAL(num, aux, "No se desencolo correctamente el elemento");
	ASSERT_EQUAL(TRUE, stack_is_empty(&q), "La pila debe estar vacia");
	TEST_PASSED("Elemento desencolocado de la pila");
}

TEST(desapilar_sin_elemento_devuelve_error)
{
	stack_t q;
	int aux;
	stack_init(&q);
	ASSERT_EQUAL(STACK_ERR_EMPTY, stack_pull(&q, &aux, sizeof(int)),
		     "Desenpilar debe retornar error");
	TEST_PASSED("Desenpilar devuelve error de vacio");
}

TEST(desenpilar_varios_elementos)
{
	stack_t q;
	char c = 'A', aux;
	stack_init(&q);
	ASSERT_EQUAL(STACK_SUCCESS, stack_push(&q, &c, sizeof(char)),
		     "No se encolo correctamente el elemento");
	c = 'B';
	ASSERT_EQUAL(STACK_SUCCESS, stack_push(&q, &c, sizeof(char)),
		     "No se encolo correctamente el elemento");
	c = 'C';
	ASSERT_EQUAL(STACK_SUCCESS, stack_push(&q, &c, sizeof(char)),
		     "No se encolo correctamente el elemento");
	ASSERT_EQUAL(STACK_SUCCESS, stack_pull(&q, &aux, sizeof(char)),
		     "No se desencolo correctamente el elemento");
	c = 'C';
	ASSERT_EQUAL(c, aux, "No se desencolo correctamente el elemento");
	ASSERT_EQUAL(STACK_SUCCESS, stack_pull(&q, &aux, sizeof(char)),
		     "No se desencolo correctamente el elemento");
	c = 'B';
	ASSERT_EQUAL(c, aux, "No se desencolo correctamente el elemento");
	ASSERT_EQUAL(STACK_SUCCESS, stack_pull(&q, &aux, sizeof(char)),
		     "No se desencolo correctamente el elemento");
	c = 'A';
	ASSERT_EQUAL(c, aux, "No se desencolo correctamente el elemento");
	ASSERT_EQUAL(TRUE, stack_is_empty(&q), "La pila debe estar vacia");
	TEST_PASSED("Elementos desencolocados de la pila");
}

TEST(vaciar_pila)
{
	stack_t q;
	char c = 'A';
	stack_init(&q);
	ASSERT_EQUAL(STACK_SUCCESS, stack_push(&q, &c, sizeof(char)),
		     "No se encolo correctamente el elemento");
	c = 'B';
	ASSERT_EQUAL(STACK_SUCCESS, stack_push(&q, &c, sizeof(char)),
		     "No se encolo correctamente el elemento");
	c = 'C';
	ASSERT_EQUAL(STACK_SUCCESS, stack_push(&q, &c, sizeof(char)),
		     "No se encolo correctamente el elemento");

	stack_clear(&q);
	ASSERT_EQUAL(TRUE, stack_is_empty(&q),
		     "No se vacio correctamente la pila");
	TEST_PASSED("Cola vaciada correctamente");
}

int main()
{
	RUN_TEST(pila_esta_vacia_al_iniciar);
	RUN_TEST(poner_elemento_en_pila);
	RUN_TEST(sacar_elemento_en_pila);
	RUN_TEST(desapilar_unico_elemento_deja_pila_vacia);
	RUN_TEST(desapilar_sin_elemento_devuelve_error);
	RUN_TEST(desenpilar_varios_elementos);
	RUN_TEST(vaciar_pila);
	TEST_SUMMARY();
	return 0;
}
