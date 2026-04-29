#include <ListaSimple.h>
#include "test_framework.h"
#include <Comun.h>

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
	int res = insertarAlFinalDeLista(&l, &num, sizeof(int));

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
	int res = insertarAlFinalDeLista(&l, &num, sizeof(int));
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
	int res = insertarAlFinalDeLista(&l, &num, sizeof(int));
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
		int res = insertarAlFinalDeLista(&l, &aux, sizeof(int));
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
		int res = insertarAlFinalDeLista(&l, &aux, sizeof(int));
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

int Comp(const void *a, const void *b)
{
	return *(int *)a - *(int *)b;
}

TEST(insertar_elemento_en_orden)
{
	t_Lista l;
	crearLista(&l);
	int num[18] = { 3, 4, 6, 8, 1, 3, 4, 7, 3, 4, 6, 7, 4, 5, 6, 7, 9, 1 };
	int aux;
	for (int i = 0; i < 18; i++) {
		aux = num[i];
		int res = insertarOrdenadoEnLista(&l, &aux, sizeof(int), Comp,
						  0, NULL);
		ASSERT_EQUAL(OK, res, "enqueue debe retornar OK");
	}
	for (int i = 0; i < 8; i++) {
		int res = sacarPrimeroLista(&l, &aux, sizeof(int));
		ASSERT_EQUAL(OK, res, "Lista debe retornar OK");
		printf("%d", aux);
	}
	TEST_PASSED("Insertar ordenado funciona correctamente");
}

TEST(eliminar_elemento_por_clave)
{
	t_Lista l;
	crearLista(&l);
	int num[18] = { 3, 4, 6, 8, 1, 3, 4, 7, 3, 4, 6, 7, 4, 5, 6, 7, 9, 1 };
	int aux;
	for (int i = 0; i < 18; i++) {
		aux = num[i];
		int res = insertarOrdenadoEnLista(&l, &aux, sizeof(int), Comp,
						  0, NULL);
		ASSERT_EQUAL(OK, res, "enqueue debe retornar OK");
	}
	aux = 8;
	int res = elimminarPorClave(&l, &aux, sizeof(int), Comp);
	ASSERT_EQUAL(OK, res, "Lista debe retornar OK");
	printf("%d", aux);
	TEST_PASSED("Insertar ordenado funciona correctamente");
}

int main()
{
	RUN_TEST(lista_recien_creada_esta_vacia);
	RUN_TEST(insertar_elemento_en_lista);
	RUN_TEST(insertar_varios_elementos_en_lista);
	RUN_TEST(eliminar_elemento_en_lista);
	RUN_TEST(eliminar_varios_elementos_en_lista);
	RUN_TEST(ver_elemento_sin_quitar);
	RUN_TEST(insertar_elemento_en_orden);
	RUN_TEST(eliminar_elemento_por_clave);
	return 0;
}
