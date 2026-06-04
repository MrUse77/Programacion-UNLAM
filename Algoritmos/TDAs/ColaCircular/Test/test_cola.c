#include <Cola.h>
#include "test_framework.h"
TEST(cola_recien_creada_esta_vacia)
{
	queue_t c;

	//Creamos cola
	queue_create(&c);

	ASSERT_TRUE(queue_is_empty(&c) == TRUE,
		    "Cola recien creada debe estar vacia");
	TEST_PASSED("Cola vacia luego de crearla");
}

TEST(insertar_elemento_en_cola)
{
	queue_t c;

	//Creamos cola
	queue_create(&c);

	int num = 3;

	//insertamos elemento
	int res = queue_push(&c, &num, sizeof(int));

	ASSERT_EQUAL(OK, res, "enqueue debe retornar OK");
	ASSERT_FALSE(queue_is_empty(&c), "Cola debe tener elementos");
	TEST_PASSED("Encolar elemento funciona correctamente");
}

TEST(eliminar_elemento_en_cola)
{
	queue_t c;

	//Creamos cola
	queue_create(&c);

	int num = 3;

	//insertamos elemento
	int res = queue_push(&c, &num, sizeof(int));
	int aux;
	res = queue_pull(&c, &aux, sizeof(int));

	ASSERT_EQUAL(OK, res, "dequeue debe retornar OK");
	ASSERT_EQUAL(num, aux, "aux debe ser igual al elemento puesto en cola");
	TEST_PASSED("Desencolar elemento funciona correctamente");
}

TEST(ver_elemento_sin_quitar)
{
	queue_t c;

	//Creamos cola
	queue_create(&c);

	int num = 3;

	//insertamos elemento
	int res = queue_push(&c, &num, sizeof(int));
	int aux;
	res = queue_see_first(&c, &aux, sizeof(int));

	ASSERT_EQUAL(OK, res, "verPrimero debe retornar OK");
	ASSERT_EQUAL(num, aux, "aux debe ser igual al elemento puesto en cola");
	queue_pull(&c, &num, sizeof(int));
	ASSERT_EQUAL(num, aux, "aux debe ser igual al primer elemento");
	TEST_PASSED("verPrimero funciona correctamente");
}

TEST(insertar_varios_elementos_en_cola)
{
	queue_t c;

	//Creamos cola
	queue_create(&c);

	int num[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

	//insertamos elemento
	for (int i = 0; i < 10; i++) {
		int aux = num[i];
		int res = queue_push(&c, &aux, sizeof(int));
		ASSERT_EQUAL(OK, res, "enqueue debe retornar OK");
	}
	ASSERT_FALSE(queue_is_empty(&c), "Cola debe tener elementos");
	TEST_PASSED("Encolar elemento funciona correctamente");
}

TEST(eliminar_varios_elementos_en_cola)
{
	queue_t c;

	//Creamos cola
	queue_create(&c);

	int num[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

	int aux;

	for (int i = 0; i < 10; i++) {
		aux = num[i];
		int res = queue_push(&c, &aux, sizeof(int));
		ASSERT_EQUAL(OK, res, "enqueue debe retornar OK");
	}
	for (int i = 0; i < 10; i++) {
		aux = num[i];
		int resNum;
		int res = queue_pull(&c, &resNum, sizeof(int));
		ASSERT_EQUAL(OK, res, "dequeue debe retornar OK");
		ASSERT_EQUAL(resNum, aux,
			     "aux debe ser igual al elemento puesto en cola");
	}

	TEST_PASSED("Desencolar elemento funciona correctamente");
}

int main()
{
	RUN_TEST(cola_recien_creada_esta_vacia);
	RUN_TEST(insertar_elemento_en_cola);
	RUN_TEST(insertar_varios_elementos_en_cola);
	RUN_TEST(eliminar_elemento_en_cola);
	RUN_TEST(eliminar_varios_elementos_en_cola);
	RUN_TEST(ver_elemento_sin_quitar);
	TEST_SUMMARY();
	return 0;
}
