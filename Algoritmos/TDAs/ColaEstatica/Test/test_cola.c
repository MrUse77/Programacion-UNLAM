#include "Cola.h"
#include "test_framework.h"

TEST(cola_recien_creada_esta_vacia)
{
	t_Cola c;

	//Creamos cola
	crearCola(&c);

	ASSERT_TRUE(colaVacia(&c) == TRUE,
		    "Cola recien creada debe estar vacia");
	TEST_PASSED("Cola vacia luego de crearla");
}

TEST(insertar_elemento_en_cola)
{
	t_Cola c;

	//Creamos cola
	crearCola(&c);

	int num = 3;

	//insertamos elemento
	int res = ponerEnCola(&c, &num, sizeof(int));

	ASSERT_EQUAL(OK, res, "enqueue debe retornar OK");
	ASSERT_FALSE(colaVacia(&c), "Cola debe tener elementos");
	TEST_PASSED("Encolar elemento funciona correctamente");
}

TEST(eliminar_elemento_en_cola)
{
	t_Cola c;

	//Creamos cola
	crearCola(&c);

	int num = 3;

	//insertamos elemento
	int res = ponerEnCola(&c, &num, sizeof(int));
	int aux;
	res = sacarDeCola(&c, &aux, sizeof(int));

	ASSERT_EQUAL(OK, res, "dequeue debe retornar OK");
	ASSERT_EQUAL(num, aux, "aux debe ser igual al elemento puesto en cola");
	TEST_PASSED("Desencolar elemento funciona correctamente");
}

TEST(ver_elemento_sin_quitar)
{
	t_Cola c;

	//Creamos cola
	crearCola(&c);

	int num = 3;

	//insertamos elemento
	int res = ponerEnCola(&c, &num, sizeof(int));
	int aux;
	res = verPrimero(&c, &aux, sizeof(int));

	ASSERT_EQUAL(OK, res, "verPrimero debe retornar OK");
	ASSERT_EQUAL(num, aux, "aux debe ser igual al elemento puesto en cola");
	sacarDeCola(&c, &num, sizeof(int));
	ASSERT_EQUAL(num, aux, "aux debe ser igual al primer elemento");
	TEST_PASSED("verPrimero funciona correctamente");
}

TEST(insertar_varios_elementos_en_cola)
{
	t_Cola c;

	//Creamos cola
	crearCola(&c);

	int num[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

	//insertamos elemento
	for (int i = 0; i < 10; i++) {
		int aux = num[i];
		int res = ponerEnCola(&c, &aux, sizeof(int));
		ASSERT_EQUAL(OK, res, "enqueue debe retornar OK");
	}
	ASSERT_FALSE(colaVacia(&c), "Cola debe tener elementos");
	TEST_PASSED("Encolar elemento funciona correctamente");
}
TEST(eliminar_varios_elementos_en_cola)
{
	t_Cola c;

	//Creamos cola
	crearCola(&c);

	int num[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

	int aux;

	for (int i = 0; i < 10; i++) {
		aux = num[i];
		int res = ponerEnCola(&c, &aux, sizeof(int));
		ASSERT_EQUAL(OK, res, "enqueue debe retornar OK");
	}
	for (int i = 0; i < 10; i++) {
		aux = num[i];
		int resNum;
		int res = sacarDeCola(&c, &resNum, sizeof(int));
		ASSERT_EQUAL(OK, res, "dequeue  debe retornar OK");
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
	return 0;
}
