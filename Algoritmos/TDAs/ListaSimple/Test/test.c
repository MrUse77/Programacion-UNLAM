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

void verificarSecuenciaEnteros(t_Lista *l, int *vec, int cant)
{
	int i;
	int aux;
	int res;

	for (i = 0; i < cant; i++) {
		res = sacarPrimeroLista(l, &aux, sizeof(int));
		printf("%d \n", aux);
		ASSERT_EQUAL(OK, res, "sacarPrimeroLista debe retornar OK");
		ASSERT_EQUAL(vec[i], aux,
			     "La secuencia de la lista es incorrecta");
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

TEST(longitud_lista_vacia)
{
	t_Lista l;

	crearLista(&l);

	ASSERT_EQUAL(0, longitudLista(&l), "Una lista vacia debe medir 0");
	TEST_PASSED("longitudLista en lista vacia");
}

TEST(longitud_lista_con_elementos)
{
	t_Lista l;
	int v[] = { 10, 20, 30 };

	crearLista(&l);
	cargarEnterosEnLista(&l, v, 3);

	ASSERT_EQUAL(3, longitudLista(&l), "La longitud debe coincidir");
	TEST_PASSED("longitudLista con elementos");
}

TEST(ver_elemento_en_posicion)
{
	t_Lista l;
	int v[] = { 10, 20, 30 };
	int x;

	crearLista(&l);
	cargarEnterosEnLista(&l, v, 3);

	ASSERT_EQUAL(OK, verElementoEnPosicion(&l, &x, sizeof(int), 1),
		     "verElementoEnPosicion debe retornar OK");
	ASSERT_EQUAL(20, x, "La posicion 1 debe ser 20");
	TEST_PASSED("verElementoEnPosicion funciona");
}

TEST(buscar_en_lista)
{
	t_Lista l;
	int v[] = { 10, 20, 30 };
	int clave;

	crearLista(&l);
	cargarEnterosEnLista(&l, v, 3);

	clave = 20;
	ASSERT_EQUAL(OK, buscarEnLista(&l, &clave, sizeof(int), Comp),
		     "buscarEnLista debe retornar OK");
	ASSERT_EQUAL(20, clave, "Debe devolver el elemento buscado");

	clave = 99;
	ASSERT_EQUAL(ERR_LISTA_NO_ENCONTRADO,
		     buscarEnLista(&l, &clave, sizeof(int), Comp),
		     "Debe informar no encontrado");
	TEST_PASSED("buscarEnLista funciona");
}

TEST(copiar_lista)
{
	t_Lista l;
	t_Lista copia;
	int v[] = { 10, 20, 30 };
	int x;
	int esperado[] = { 10, 20, 30 };

	crearLista(&l);
	crearLista(&copia);
	cargarEnterosEnLista(&l, v, 3);

	ASSERT_EQUAL(OK, copiarLista(&l, &copia),
		     "copiarLista debe retornar OK");

	verificarSecuenciaEnteros(&copia, esperado, 3);
	ASSERT_TRUE(listaVacia(&copia),
		    "La copia debe quedar vacia al consumirla");

	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &x, sizeof(int)),
		     "La lista original debe seguir intacta");
	ASSERT_EQUAL(10, x, "La copia no debe afectar a la original");
	TEST_PASSED("copiarLista funciona");
}
/*
TEST(clonar_lista)
{
	t_Lista l;
	t_Lista clon;
	int v[] = { 10, 20, 30 };
	int x;
	int esperado[] = { 10, 20, 30 };

	crearLista(&l);
	crearLista(&clon);
	cargarEnterosEnLista(&l, v, 3);

	ASSERT_EQUAL(OK, clonarLista(&l, &clon),
		     "clonarLista debe retornar OK");

	verificarSecuenciaEnteros(&clon, esperado, 3);
	ASSERT_TRUE(listaVacia(&clon), "El clon debe quedar vacio al consumirlo");

	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &x, sizeof(int)),
		     "La lista original debe seguir intacta");
	ASSERT_EQUAL(10, x, "El clon no debe afectar a la original");
	TEST_PASSED("clonarLista funciona");
}
*/
TEST(insertar_en_posicion)
{
	t_Lista l;
	int v[] = { 1, 3 };
	int x;
	int nuevo = 2;

	crearLista(&l);
	cargarEnterosEnLista(&l, v, 2);

	ASSERT_EQUAL(OK, insertarEnPosicion(&l, &nuevo, sizeof(int), 1),
		     "insertarEnPosicion debe retornar OK");

	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &x, sizeof(int)),
		     "sacarPrimeroLista debe retornar OK");
	ASSERT_EQUAL(1, x, "Debe quedar primero el 1");
	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &x, sizeof(int)),
		     "sacarPrimeroLista debe retornar OK");
	ASSERT_EQUAL(2, x, "Debe insertarse en la posicion pedida");
	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &x, sizeof(int)),
		     "sacarPrimeroLista debe retornar OK");
	ASSERT_EQUAL(3, x, "Debe quedar ultimo el 3");
	TEST_PASSED("insertarEnPosicion funciona");
}

TEST(eliminar_por_posicion)
{
	t_Lista l;
	int v[] = { 1, 2, 3 };
	int x;

	crearLista(&l);
	cargarEnterosEnLista(&l, v, 3);

	ASSERT_EQUAL(OK, eliminarPorPosicion(&l, &x, sizeof(int), 1),
		     "eliminarPorPosicion debe retornar OK");
	ASSERT_EQUAL(2, x, "Debe eliminar la posicion solicitada");

	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &x, sizeof(int)),
		     "sacarPrimeroLista debe retornar OK");
	ASSERT_EQUAL(1, x, "La lista quedo mal");
	ASSERT_EQUAL(OK, sacarPrimeroLista(&l, &x, sizeof(int)),
		     "sacarPrimeroLista debe retornar OK");
	ASSERT_EQUAL(3, x, "La lista quedo mal");
	TEST_PASSED("eliminarPorPosicion funciona");
}

TEST(invertir_lista)
{
	t_Lista l;
	int v[] = { 1, 2, 3 };
	int esperado[] = { 3, 2, 1 };

	crearLista(&l);
	cargarEnterosEnLista(&l, v, 3);

	ASSERT_EQUAL(OK, invertirLista(&l), "invertirLista debe retornar OK");
	verificarSecuenciaEnteros(&l, esperado, 3);
	ASSERT_TRUE(listaVacia(&l), "La lista debe quedar vacia al consumirla");
	TEST_PASSED("invertirLista funciona");
}

TEST(concatenar_listas)
{
	t_Lista l1;
	t_Lista l2;
	int v1[] = { 1, 2 };
	int v2[] = { 3, 4 };
	int esperado[] = { 1, 2, 3, 4 };

	crearLista(&l1);
	crearLista(&l2);
	cargarEnterosEnLista(&l1, v1, 2);
	cargarEnterosEnLista(&l2, v2, 2);

	ASSERT_EQUAL(OK, concatenarListas(&l1, &l2),
		     "concatenarListas debe retornar OK");

	verificarSecuenciaEnteros(&l1, esperado, 4);
	ASSERT_TRUE(listaVacia(&l1), "l1 debe quedar vacia al consumirla");
	ASSERT_TRUE(listaVacia(&l2),
		    "l2 debe quedar vacia luego de concatenar");
	TEST_PASSED("concatenarListas funciona");
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

TEST(merge_sort_lista_vacia)
{
	t_Lista l;
	crearLista(&l);
	ASSERT_EQUAL(ERR_LISTA_VACIA, ordenarLista(&l, MERGE, Comp),
		     "Merge sort sobre lista vacia debe andar");
	ASSERT_TRUE(listaVacia(&l), "Debe seguir vacia");
	TEST_PASSED("merge sort lista vacia");
}
TEST(merge_sort_par)
{
	t_Lista l;
	int v[] = { 4, 1, 3, 2 };
	int esperado[] = { 1, 2, 3, 4 };
	crearLista(&l);
	cargarEnterosEnLista(&l, v, 4);
	ASSERT_EQUAL(OK, ordenarLista(&l, MERGE, Comp),
		     "Merge sort debe retornar OK");
	verificarSecuenciaEnteros(&l, esperado, 4);
	TEST_PASSED("merge sort par");
}
TEST(merge_sort_impar)
{
	t_Lista l;
	int v[] = { 5, 1, 4, 2, 3 };
	int esperado[] = { 1, 2, 3, 4, 5 };
	crearLista(&l);
	cargarEnterosEnLista(&l, v, 5);
	ASSERT_EQUAL(OK, ordenarLista(&l, MERGE, Comp),
		     "Merge sort debe retornar OK");
	verificarSecuenciaEnteros(&l, esperado, 5);
	TEST_PASSED("merge sort impar");
}

TEST(quick_sort_lista_vacia)
{
	t_Lista l;
	crearLista(&l);
	ASSERT_EQUAL(ERR_LISTA_VACIA, ordenarLista(&l, QUICK, Comp),
		     "Quick sort sobre lista vacia debe andar");
	ASSERT_TRUE(listaVacia(&l), "Debe seguir vacia");
	TEST_PASSED("quick sort lista vacia");
}
TEST(quick_sort_par)
{
	t_Lista l;
	int v[] = { 4, 1, 3, 2 };
	int esperado[] = { 1, 2, 3, 4 };
	crearLista(&l);
	cargarEnterosEnLista(&l, v, 4);
	ASSERT_EQUAL(OK, ordenarLista(&l, QUICK, Comp),
		     "Quick sort debe retornar OK");
	verificarSecuenciaEnteros(&l, esperado, 4);
	TEST_PASSED("quick sort par");
}
TEST(quick_sort_impar)
{
	t_Lista l;
	int v[] = { 5, 1, 4, 2, 3 };
	int esperado[] = { 1, 2, 3, 4, 5 };
	crearLista(&l);
	cargarEnterosEnLista(&l, v, 5);
	ASSERT_EQUAL(OK, ordenarLista(&l, QUICK, Comp),
		     "Quick sort debe retornar OK");
	verificarSecuenciaEnteros(&l, esperado, 5);
	TEST_PASSED("quick sort impar");
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
	RUN_TEST(longitud_lista_vacia);
	RUN_TEST(longitud_lista_con_elementos);
	RUN_TEST(ver_elemento_en_posicion);
	RUN_TEST(buscar_en_lista);
	RUN_TEST(copiar_lista);
	//RUN_TEST(clonar_lista);
	RUN_TEST(insertar_en_posicion);
	RUN_TEST(eliminar_por_posicion);
	RUN_TEST(invertir_lista);
	RUN_TEST(concatenar_listas);
	RUN_TEST(ver_ultimo_sin_quitar);
	RUN_TEST(vaciar_lista);
	RUN_TEST(insertar_ordenado_sin_duplicados);
	RUN_TEST(insertar_ordenado_con_duplicados_sin_insertar);
	RUN_TEST(insertar_ordenado_con_duplicados_y_accion);
	RUN_TEST(eliminar_por_clave_existente);
	RUN_TEST(eliminar_por_clave_inexistente);
	RUN_TEST(merge_sort_lista_vacia);
	RUN_TEST(merge_sort_par);
	RUN_TEST(merge_sort_impar);
	RUN_TEST(quick_sort_lista_vacia);
	RUN_TEST(quick_sort_par);
	RUN_TEST(quick_sort_impar);
	return 0;
}
