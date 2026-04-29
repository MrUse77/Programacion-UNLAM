#include <ListaSimple.h>
#include "test_framework.h"

TEST(lista_recien_creada_esta_vacia)
{
	t_Lista l;

	//Creamos lista
	crearLista(&l);

	ASSERT_TRUE(listaVacia(&l) == TRUE,
		    "Lista recien creada debe estar vacia");
	TEST_PASSED("lista vacia luego de crearla");
}

TEST(insertar_elemento_en_lista)
{
	t_Lista l;

	//Creamos lista
	crearLista(&l);

	int num = 3;

	//insertamos elemento
	int res = ponerAlFinalDeLista(&l, &num, sizeof(int));

	ASSERT_EQUAL(OK, res, "enqueue debe retornar OK");
	ASSERT_FALSE(listaVacia(&l), "Lista debe tener elementos");
	TEST_PASSED("Enlistar elemento funciona correctamente");
}

TEST(eliminar_elemento_en_lista)
{
	t_Lista l;

	//Creamos lista
	crearLista(&l);

	int num = 3;

	//insertamos elemento
	int res = ponerAlFinalDeLista(&l, &num, sizeof(int));
	int aux;
	res = sacarUltimoDeLista(&l, &aux, sizeof(int));

	ASSERT_EQUAL(OK, res, "dequeue debe retornar OK");
	ASSERT_EQUAL(num, aux,
		     "aux debe ser igual al elemento puesto en lista");
	TEST_PASSED("Desenlistar elemento funciona correctamente");
}

TEST(ver_elemento_sin_quitar)
{
	t_Lista l;

	//Creamos lista
	crearLista(&l);

	int num = 3;

	//insertamos elemento
	int res = ponerAlFinalDeLista(&l, &num, sizeof(int));
	int aux;
	res = verUltimoDeLista(&l, &aux, sizeof(int));

	ASSERT_EQUAL(OK, res, "verPrimero debe retornar OK");
	ASSERT_EQUAL(num, aux,
		     "aux debe ser igual al elemento puesto en lista");
	sacarUltimoDeLista(&l, &num, sizeof(int));
	ASSERT_EQUAL(num, aux, "aux debe ser igual al primer elemento");
	TEST_PASSED("verPrimero funciona correctamente");
}

TEST(insertar_varios_elementos_en_lista)
{
	t_Lista l;

	//Creamos lista
	crearLista(&l);

	int num[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

	//insertamos elemento
	for (int i = 0; i < 10; i++) {
		int aux = num[i];
		int res = ponerAlFinalDeLista(&l, &aux, sizeof(int));
		ASSERT_EQUAL(OK, res, "enqueue debe retornar OK");
	}
	ASSERT_FALSE(listaVacia(&l), "Lista debe tener elementos");
	TEST_PASSED("Enlistar elemento funciona correctamente");
}
TEST(eliminar_varios_elementos_en_lista)
{
	t_Lista l;

	//Creamos lista
	crearLista(&l);

	int num[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

	int aux;

	for (int i = 0; i < 10; i++) {
		aux = num[i];
		int res = ponerAlFinalDeLista(&l, &aux, sizeof(int));
		ASSERT_EQUAL(OK, res, "enqueue debe retornar OK");
	}
	for (int i = 9; i > 0; i--) {
		aux = num[i];
		int resNum;
		int res = sacarUltimoDeLista(&l, &resNum, sizeof(int));
		ASSERT_EQUAL(OK, res, "dequeue  debe retornar OK");
		ASSERT_EQUAL(resNum, aux,
			     "aux debe ser igual al elemento puesto en lista");
	}
	TEST_PASSED("Desenlistar elemento funciona correctamente");
}

int main()
{
	RUN_TEST(lista_recien_creada_esta_vacia);
	RUN_TEST(insertar_elemento_en_lista);
	RUN_TEST(insertar_varios_elementos_en_lista);
	RUN_TEST(eliminar_elemento_en_lista);
	RUN_TEST(eliminar_varios_elementos_en_lista);
	RUN_TEST(ver_elemento_sin_quitar);
	return 0;
}
