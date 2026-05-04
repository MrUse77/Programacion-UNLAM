#include <ListaSimple.h>
#include "test_framework.h"
#include <Comun.h>
/* Helpers */
int Comp(const void *a, const void *b)
{
	return *(int *)a - *(int *)b;
}
void sumarAExistente(void *param, const void *dato)
{
	*(int *)param += *(int *)dato;
}
void cargarEnterosEnLista(t_Lista *l, int *vec, int cant)
{
	int i;
	for (i = 0; i < cant; i++) {
		int aux = vec[i];
		ASSERT_EQUAL(OK, insertarAlFinalDeLista(l, &aux, sizeof(int)),
			     "insertarAlFinalDeLista debe retornar OK");
	}
}
/* Tests */
TEST(lista_recien_creada_esta_vacia)
{
	t_Lista l;
	crearLista(&l);
	ASSERT_TRUE(listaVacia(&l) == TRUE,
		    "Lista recien creada debe estar vacia");
	TEST_PASSED("lista vacia luego de crearla");
}
TEST(lista_llena_en_condicion_normal_es_falsa)
{
	t_Lista l;
	crearLista(&l);
	ASSERT_FALSE(listaLlena(&l, sizeof(int)),
		     "Una lista dinamica normal no deberia estar llena");
	TEST_PASSED("listaLlena en condicion normal");
}
TEST(insertar_al_principio)
{
	t_Lista l;
	int a, b, x;
	crearLista(&l);
	a = 1;
	b = 2;
	ASSERT_EQUAL(OK, insertarAlPrincipioDeLista(&l, &a, sizeof(int)),
		     "insertarAlPrincipioDeLista debe retornar OK");
	ASSERT_EQUAL(OK, insertarAlPrincipioDeLista(&l, &b, sizeof(int)),
		     "insertarAlPrincipioDeLista debe retornar OK");
	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &x, sizeof(int)),
		     "sacarPrimeroLista debe retornar OK");
	ASSERT_EQUAL(2, x,
		     "El ultimo insertado al principio debe salir primero");
	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &x, sizeof(int)),
		     "sacarPrimeroLista debe retornar OK");
	ASSERT_EQUAL(1, x, "El anterior debe salir segundo");
	TEST_PASSED("insertarAlPrincipioDeLista funciona");
}
TEST(insertar_al_final)
{
	t_Lista l;
	int a, b, x;
	crearLista(&l);
	a = 1;
	b = 2;
	ASSERT_EQUAL(OK, insertarAlFinalDeLista(&l, &a, sizeof(int)),
		     "insertarAlFinalDeLista debe retornar OK");
	ASSERT_EQUAL(OK, insertarAlFinalDeLista(&l, &b, sizeof(int)),
		     "insertarAlFinalDeLista debe retornar OK");
	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &x, sizeof(int)),
		     "sacarPrimeroLista debe retornar OK");
	ASSERT_EQUAL(1, x, "Debe respetar FIFO");
	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &x, sizeof(int)),
		     "sacarPrimeroLista debe retornar OK");
	ASSERT_EQUAL(2, x, "Debe respetar FIFO");
	TEST_PASSED("insertarAlFinalDeLista funciona");
}
TEST(sacar_primero_de_lista)
{
	t_Lista l;
	int v[] = { 10, 20, 30 };
	int x;
	crearLista(&l);
	cargarEnterosEnLista(&l, v, 3);
	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &x, sizeof(int)),
		     "sacarPrimeroLista debe retornar OK");
	ASSERT_EQUAL(10, x, "Debe salir el primer elemento");
	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &x, sizeof(int)),
		     "sacarPrimeroLista debe retornar OK");
	ASSERT_EQUAL(20, x, "Debe salir el segundo elemento");
	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &x, sizeof(int)),
		     "sacarPrimeroLista debe retornar OK");
	ASSERT_EQUAL(30, x, "Debe salir el tercer elemento");
	ASSERT_TRUE(listaVacia(&l), "La lista debe quedar vacia");
	TEST_PASSED("sacarPrimeroLista funciona");
}
TEST(sacar_ultimo_de_lista)
{
	t_Lista l;
	int v[] = { 10, 20, 30 };
	int x;
	crearLista(&l);
	cargarEnterosEnLista(&l, v, 3);
	ASSERT_EQUAL(OK, sacarUltimoDeLista(&l, &x, sizeof(int)),
		     "sacarUltimoDeLista debe retornar OK");
	ASSERT_EQUAL(30, x, "Debe salir el ultimo elemento");
	ASSERT_EQUAL(OK, sacarUltimoDeLista(&l, &x, sizeof(int)),
		     "sacarUltimoDeLista debe retornar OK");
	ASSERT_EQUAL(20, x, "Debe salir el penultimo elemento");
	ASSERT_EQUAL(OK, sacarUltimoDeLista(&l, &x, sizeof(int)),
		     "sacarUltimoDeLista debe retornar OK");
	ASSERT_EQUAL(10, x, "Debe salir el primer elemento");
	ASSERT_TRUE(listaVacia(&l), "La lista debe quedar vacia");
	TEST_PASSED("sacarUltimoDeLista funciona");
}
TEST(ver_primero_sin_quitar)
{
	t_Lista l;
	int v[] = { 10, 20, 30 };
	int x, y;
	crearLista(&l);
	cargarEnterosEnLista(&l, v, 3);
	ASSERT_EQUAL(OK, verPrimeroDeLista(&l, &x, sizeof(int)),
		     "verPrimeroDeLista debe retornar OK");
	ASSERT_EQUAL(10, x, "Debe ver el primer elemento");
	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &y, sizeof(int)),
		     "sacarPrimeroLista debe retornar OK");
	ASSERT_EQUAL(10, y, "El primero sigue siendo 10");
	TEST_PASSED("verPrimeroDeLista funciona sin quitar");
}
TEST(ver_ultimo_sin_quitar)
{
	t_Lista l;
	int v[] = { 10, 20, 30 };
	int x, y;
	crearLista(&l);
	cargarEnterosEnLista(&l, v, 3);
	ASSERT_EQUAL(OK, verUltimoDeLista(&l, &x, sizeof(int)),
		     "verUltimoDeLista debe retornar OK");
	ASSERT_EQUAL(30, x, "Debe ver el ultimo elemento");
	ASSERT_EQUAL(OK, sacarUltimoDeLista(&l, &y, sizeof(int)),
		     "sacarUltimoDeLista debe retornar OK");
	ASSERT_EQUAL(30, y, "El ultimo sigue siendo 30");
	TEST_PASSED("verUltimoDeLista funciona sin quitar");
}
TEST(vaciar_lista)
{
	t_Lista l;
	int v[] = { 1, 2, 3 };
	int x;
	crearLista(&l);
	cargarEnterosEnLista(&l, v, 3);
	vaciarLista(&l);
	ASSERT_TRUE(listaVacia(&l) == TRUE, "La lista debe quedar vacia");
	ASSERT_EQUAL(OK, insertarAlFinalDeLista(&l, &v[0], sizeof(int)),
		     "La lista vaciada debe seguir funcionando");
	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &x, sizeof(int)),
		     "Debe poder sacar luego de vaciar");
	ASSERT_EQUAL(1, x, "Debe salir el valor insertado");
	TEST_PASSED("vaciarLista funciona");
}
TEST(insertar_ordenado_sin_duplicados)
{
	t_Lista l;
	int v[] = { 3, 1, 4, 2 };
	int x;
	crearLista(&l);
	ASSERT_EQUAL(OK,
		     insertarOrdenadoEnLista(&l, &v[0], sizeof(int), Comp, 0,
					     NULL),
		     "insertarOrdenadoEnLista debe retornar OK");
	ASSERT_EQUAL(OK,
		     insertarOrdenadoEnLista(&l, &v[1], sizeof(int), Comp, 0,
					     NULL),
		     "insertarOrdenadoEnLista debe retornar OK");
	ASSERT_EQUAL(OK,
		     insertarOrdenadoEnLista(&l, &v[2], sizeof(int), Comp, 0,
					     NULL),
		     "insertarOrdenadoEnLista debe retornar OK");
	ASSERT_EQUAL(OK,
		     insertarOrdenadoEnLista(&l, &v[3], sizeof(int), Comp, 0,
					     NULL),
		     "insertarOrdenadoEnLista debe retornar OK");
	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &x, sizeof(int)),
		     "sacarPrimeroLista debe retornar OK");
	ASSERT_EQUAL(1, x, "Orden incorrecto");
	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &x, sizeof(int)),
		     "sacarPrimeroLista debe retornar OK");
	ASSERT_EQUAL(2, x, "Orden incorrecto");
	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &x, sizeof(int)),
		     "sacarPrimeroLista debe retornar OK");
	ASSERT_EQUAL(3, x, "Orden incorrecto");
	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &x, sizeof(int)),
		     "sacarPrimeroLista debe retornar OK");
	ASSERT_EQUAL(4, x, "Orden incorrecto");
	TEST_PASSED("insertarOrdenadoEnLista sin duplicados funciona");
}
TEST(insertar_ordenado_con_duplicados_sin_insertar)
{
	t_Lista l;
	int v[] = { 3, 1, 3, 2 };
	int x;
	crearLista(&l);
	ASSERT_EQUAL(OK,
		     insertarOrdenadoEnLista(&l, &v[0], sizeof(int), Comp, 0,
					     NULL),
		     "insertarOrdenadoEnLista debe retornar OK");
	ASSERT_EQUAL(OK,
		     insertarOrdenadoEnLista(&l, &v[1], sizeof(int), Comp, 0,
					     NULL),
		     "insertarOrdenadoEnLista debe retornar OK");
	ASSERT_EQUAL(OK,
		     insertarOrdenadoEnLista(&l, &v[2], sizeof(int), Comp, 0,
					     NULL),
		     "insertarOrdenadoEnLista debe retornar OK");
	ASSERT_EQUAL(OK,
		     insertarOrdenadoEnLista(&l, &v[3], sizeof(int), Comp, 0,
					     NULL),
		     "insertarOrdenadoEnLista debe retornar OK");
	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &x, sizeof(int)),
		     "sacarPrimeroLista debe retornar OK");
	ASSERT_EQUAL(1, x, "Orden incorrecto");
	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &x, sizeof(int)),
		     "sacarPrimeroLista debe retornar OK");
	ASSERT_EQUAL(2, x, "Orden incorrecto");
	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &x, sizeof(int)),
		     "sacarPrimeroLista debe retornar OK");
	ASSERT_EQUAL(3, x, "Orden incorrecto");
	ASSERT_TRUE(listaVacia(&l), "Debe quedar solo una instancia del 3");
	TEST_PASSED("insertarOrdenadoEnLista sin duplicados funciona");
}
TEST(insertar_ordenado_con_duplicados_y_accion)
{
	t_Lista l;
	int v[] = { 5, 5 };
	int x;
	crearLista(&l);
	ASSERT_EQUAL(OK,
		     insertarOrdenadoEnLista(&l, &v[0], sizeof(int), Comp, 1,
					     sumarAExistente),
		     "insertarOrdenadoEnLista debe retornar OK");
	ASSERT_EQUAL(OK,
		     insertarOrdenadoEnLista(&l, &v[1], sizeof(int), Comp, 1,
					     sumarAExistente),
		     "insertarOrdenadoEnLista debe retornar OK");
	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &x, sizeof(int)),
		     "sacarPrimeroLista debe retornar OK");
	ASSERT_EQUAL(
		10, x,
		"La accion debe haber acumulado el duplicado en el existente");
	TEST_PASSED("insertarOrdenadoEnLista con accion funciona");
}
TEST(eliminar_por_clave_existente)
{
	t_Lista l;
	int v[] = { 1, 2, 3, 4, 5 };
	int clave, x;
	crearLista(&l);
	cargarEnterosEnLista(&l, v, 5);
	clave = 3;
	ASSERT_EQUAL(OK, eliminarPorClave(&l, &clave, sizeof(int), Comp),
		     "eliminarPorClave debe retornar OK");
	ASSERT_EQUAL(3, clave, "Debe devolver el eliminado");
	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &x, sizeof(int)),
		     "sacarPrimeroLista debe retornar OK");
	ASSERT_EQUAL(1, x, "La lista quedo mal");
	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &x, sizeof(int)),
		     "sacarPrimeroLista debe retornar OK");
	ASSERT_EQUAL(2, x, "La lista quedo mal");
	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &x, sizeof(int)),
		     "sacarPrimeroLista debe retornar OK");
	ASSERT_EQUAL(4, x, "Debe faltar el 3");
	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &x, sizeof(int)),
		     "sacarPrimeroLista debe retornar OK");
	ASSERT_EQUAL(5, x, "La lista quedo mal");
	TEST_PASSED("eliminarPorClave existente funciona");
}
TEST(eliminar_por_clave_inexistente)
{
	t_Lista l;
	int v[] = { 1, 2, 4, 5 };
	int clave;
	crearLista(&l);
	cargarEnterosEnLista(&l, v, 4);
	clave = 3;
	ASSERT_EQUAL(ERR_LISTA_NO_ENCONTRADO,
		     eliminarPorClave(&l, &clave, sizeof(int), Comp),
		     "Debe devolver no encontrado");
	TEST_PASSED("eliminarPorClave inexistente funciona");
}
int main(void)
{
	RUN_TEST(lista_recien_creada_esta_vacia);
	RUN_TEST(lista_llena_en_condicion_normal_es_falsa);
	RUN_TEST(insertar_al_principio);
	RUN_TEST(insertar_al_final);
	RUN_TEST(sacar_primero_de_lista);
	RUN_TEST(sacar_ultimo_de_lista);
	RUN_TEST(ver_primero_sin_quitar);
	RUN_TEST(ver_ultimo_sin_quitar);
	RUN_TEST(vaciar_lista);
	RUN_TEST(insertar_ordenado_sin_duplicados);
	RUN_TEST(insertar_ordenado_con_duplicados_sin_insertar);
	RUN_TEST(insertar_ordenado_con_duplicados_y_accion);
	RUN_TEST(eliminar_por_clave_existente);
	RUN_TEST(eliminar_por_clave_inexistente);
	return 0;
}
