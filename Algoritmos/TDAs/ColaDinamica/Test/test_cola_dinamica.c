#include <cola_dinamica.h>

#include <test_framework.h>
#include <stdlib.h>
#include <string.h>

TEST(cola_esta_vacia_al_iniciar)
{
	queue_t q;
	queue_init(&q);
	ASSERT_EQUAL(TRUE, queue_is_empty(&q),
		     "La cola debe estar vacia al iniciar");
	TEST_PASSED("Cola vacia despues de iniciarla");
}

TEST(poner_elemento_en_cola)
{
	queue_t q;
	queue_init(&q);
	int num = 7;
	ASSERT_EQUAL(QUEUE_SUCCESS, queue_push(&q, &num, sizeof(int)),
		     "No se encolo correctamente el elemento");
	TEST_PASSED("Elemento colocado en la cola");
}

TEST(sacar_elemento_en_cola)
{
	queue_t q;
	queue_init(&q);
	int num = 7, aux;
	ASSERT_EQUAL(QUEUE_SUCCESS, queue_push(&q, &num, sizeof(int)),
		     "No se encolo correctamente el elemento");
	ASSERT_EQUAL(QUEUE_SUCCESS, queue_pull(&q, &aux, sizeof(int)),
		     "No se desencolo correctamente el elemento");
	ASSERT_EQUAL(num, aux, "No se desencolo correctamente el elemento");
	TEST_PASSED("Elemento desencolocado de la cola");
}

TEST(desencolar_unico_elemento_deja_cola_vacia)
{
	queue_t q;
	queue_init(&q);
	int num = 7, aux;
	ASSERT_EQUAL(QUEUE_SUCCESS, queue_push(&q, &num, sizeof(int)),
		     "No se encolo correctamente el elemento");
	ASSERT_EQUAL(QUEUE_SUCCESS, queue_pull(&q, &aux, sizeof(int)),
		     "No se desencolo correctamente el elemento");
	ASSERT_EQUAL(num, aux, "No se desencolo correctamente el elemento");
	ASSERT_EQUAL(TRUE, queue_is_empty(&q), "La cola debe estar vacia");
	TEST_PASSED("Elemento desencolocado de la cola");
}

TEST(desencolar_sin_elemento_devuelve_error)
{
	queue_t q;
	int aux;
	queue_init(&q);
	ASSERT_EQUAL(QUEUE_ERR_EMPTY, queue_pull(&q, &aux, sizeof(int)),
		     "Desencolar debe retornar error");
	TEST_PASSED("Desencolar devuelve error de vacio");
}

TEST(desencolar_varios_elementos)
{
	queue_t q;
	char c = 'A', aux;
	queue_init(&q);
	ASSERT_EQUAL(QUEUE_SUCCESS, queue_push(&q, &c, sizeof(char)),
		     "No se encolo correctamente el elemento");
	c = 'B';
	ASSERT_EQUAL(QUEUE_SUCCESS, queue_push(&q, &c, sizeof(char)),
		     "No se encolo correctamente el elemento");
	c = 'C';
	ASSERT_EQUAL(QUEUE_SUCCESS, queue_push(&q, &c, sizeof(char)),
		     "No se encolo correctamente el elemento");
	ASSERT_EQUAL(QUEUE_SUCCESS, queue_pull(&q, &aux, sizeof(char)),
		     "No se desencolo correctamente el elemento");
	c = 'A';
	ASSERT_EQUAL(c, aux, "No se desencolo correctamente el elemento");
	ASSERT_EQUAL(QUEUE_SUCCESS, queue_pull(&q, &aux, sizeof(char)),
		     "No se desencolo correctamente el elemento");
	c = 'B';
	ASSERT_EQUAL(c, aux, "No se desencolo correctamente el elemento");
	ASSERT_EQUAL(QUEUE_SUCCESS, queue_pull(&q, &aux, sizeof(char)),
		     "No se desencolo correctamente el elemento");
	c = 'C';
	ASSERT_EQUAL(c, aux, "No se desencolo correctamente el elemento");
	ASSERT_EQUAL(TRUE, queue_is_empty(&q), "La cola debe estar vacia");
	TEST_PASSED("Elementos desencolocados de la cola");
}

TEST(vaciar_cola)
{
	queue_t q;
	char c = 'A';
	queue_init(&q);
	ASSERT_EQUAL(QUEUE_SUCCESS, queue_push(&q, &c, sizeof(char)),
		     "No se encolo correctamente el elemento");
	c = 'B';
	ASSERT_EQUAL(QUEUE_SUCCESS, queue_push(&q, &c, sizeof(char)),
		     "No se encolo correctamente el elemento");
	c = 'C';
	ASSERT_EQUAL(QUEUE_SUCCESS, queue_push(&q, &c, sizeof(char)),
		     "No se encolo correctamente el elemento");

	ASSERT_EQUAL(QUEUE_SUCCESS, queue_clear(&q),
		     "No se vacio correctamente la cola");
	ASSERT_EQUAL(TRUE, queue_is_empty(&q),
		     "No se vacio correctamente la cola");
	TEST_PASSED("Cola vaciada correctamente");
}

int main()
{
	RUN_TEST(cola_esta_vacia_al_iniciar);
	RUN_TEST(poner_elemento_en_cola);
	RUN_TEST(sacar_elemento_en_cola);
	RUN_TEST(desencolar_unico_elemento_deja_cola_vacia);
	RUN_TEST(desencolar_sin_elemento_devuelve_error);
	RUN_TEST(desencolar_varios_elementos);
	RUN_TEST(vaciar_cola);
	TEST_SUMMARY();
	return 0;
}
