#include <double_list.h>
#include "test_framework.h"
/* Helpers */
int Comp(const void *a, const void *b)
{
	return *(int *)a - *(int *)b;
}
void sumarAExistente(void *param, const void *dato)
{
	*(int *)param += *(int *)dato;
}

/* Llamada para tests LIFO */
void verificarSecuenciaEnterosLIFO(list_t *l, int *vec, int cant)
{
	int i;
	int aux;
	int res;

	for (i = 0; i < cant; i++) {
		res = list_pull_last(l, &aux, sizeof(int));
		printf("%d \n", aux);
		ASSERT_EQUAL(OK, res, "list_pull_last debe retornar OK");
		ASSERT_EQUAL(vec[i], aux,
			     "La secuencia de la lista es incorrecta");
	}
}
void cargarEnterosEnLista(list_t *l, int *vec, int cant)
{
	int i;
	for (i = 0; i < cant; i++) {
		int aux = vec[i];
		ASSERT_EQUAL(OK, list_push_last(l, &aux, sizeof(int)),
			     "list_push_last debe retornar OK");
	}
}
void printLista(const void *dato)
{
	int datoInt = *(int *)dato;
	printf("%d\n", datoInt);
}

void verificarSecuenciaEnteros(list_t *l, int *vec, int cant)
{
	int i;
	int aux;
	int res;

	for (i = 0; i < cant; i++) {
		res = list_pull_first(l, &aux, sizeof(int));
		printf("%d \n", aux);
		ASSERT_EQUAL(OK, res, "list_pull_first debe retornar OK");
		ASSERT_EQUAL(vec[i], aux,
			     "La secuencia de la lista es incorrecta");
	}
}
/* Tests */
TEST(lista_recien_creada_esta_vacia)
{
	list_t l;
	list_create(&l);
	ASSERT_TRUE(list_is_empty(&l) == TRUE,
		    "Lista recien creada debe estar vacia");
	TEST_PASSED("lista vacia luego de crearla");
}
TEST(lista_llena_en_condicion_normal_es_falsa)
{
	list_t l;
	list_create(&l);
	ASSERT_FALSE(list_is_full(&l, sizeof(int)),
		     "Una lista dinamica normal no deberia estar llena");
	TEST_PASSED("list_is_full en condicion normal");
}
TEST(insertar_al_principio)
{
	list_t l;
	int a, b, x;
	list_create(&l);
	a = 1;
	b = 2;
	ASSERT_EQUAL(OK, list_push_first(&l, &a, sizeof(int)),
		     "list_push_first debe retornar OK");
	ASSERT_EQUAL(OK, list_push_first(&l, &b, sizeof(int)),
		     "list_push_first debe retornar OK");
	ASSERT_EQUAL(OK, list_pull_first(&l, &x, sizeof(int)),
		     "list_pull_first debe retornar OK");
	ASSERT_EQUAL(2, x,
		     "El ultimo insertado al principio debe salir primero");
	ASSERT_EQUAL(OK, list_pull_first(&l, &x, sizeof(int)),
		     "list_pull_first debe retornar OK");
	ASSERT_EQUAL(1, x, "El anterior debe salir segundo");
	TEST_PASSED("list_push_first funciona");
}
TEST(insertar_al_final)
{
	list_t l;
	int a, b, x;
	list_create(&l);
	a = 1;
	b = 2;
	ASSERT_EQUAL(OK, list_push_last(&l, &a, sizeof(int)),
		     "list_push_last debe retornar OK");
	ASSERT_EQUAL(OK, list_push_last(&l, &b, sizeof(int)),
		     "list_push_last debe retornar OK");
	ASSERT_EQUAL(OK, list_pull_first(&l, &x, sizeof(int)),
		     "list_pull_first debe retornar OK");
	ASSERT_EQUAL(1, x, "Debe respetar FIFO");
	ASSERT_EQUAL(OK, list_pull_first(&l, &x, sizeof(int)),
		     "list_pull_first debe retornar OK");
	ASSERT_EQUAL(2, x, "Debe respetar FIFO");
	TEST_PASSED("list_push_last funciona");
}
TEST(sacar_primero_de_lista)
{
	list_t l;
	int v[] = { 10, 20, 30 };
	int x;
	list_create(&l);
	cargarEnterosEnLista(&l, v, 3);
	ASSERT_EQUAL(OK, list_pull_first(&l, &x, sizeof(int)),
		     "list_pull_first debe retornar OK");
	ASSERT_EQUAL(10, x, "Debe salir el primer elemento");
	ASSERT_EQUAL(OK, list_pull_first(&l, &x, sizeof(int)),
		     "list_pull_first debe retornar OK");
	ASSERT_EQUAL(20, x, "Debe salir el segundo elemento");
	ASSERT_EQUAL(OK, list_pull_first(&l, &x, sizeof(int)),
		     "list_pull_first debe retornar OK");
	ASSERT_EQUAL(30, x, "Debe salir el tercer elemento");
	ASSERT_TRUE(list_is_empty(&l), "La lista debe quedar vacia");
	TEST_PASSED("list_pull_first funciona");
}
TEST(sacar_ultimo_de_lista)
{
	list_t l;
	int v[] = { 10, 20, 30 };
	int x;
	list_create(&l);
	cargarEnterosEnLista(&l, v, 3);
	ASSERT_EQUAL(OK, list_pull_last(&l, &x, sizeof(int)),
		     "list_pull_last debe retornar OK");
	ASSERT_EQUAL(30, x, "Debe salir el ultimo elemento");
	ASSERT_EQUAL(OK, list_pull_last(&l, &x, sizeof(int)),
		     "list_pull_last debe retornar OK");
	ASSERT_EQUAL(20, x, "Debe salir el penultimo elemento");
	ASSERT_EQUAL(OK, list_pull_last(&l, &x, sizeof(int)),
		     "list_pull_last debe retornar OK");
	ASSERT_EQUAL(10, x, "Debe salir el primer elemento");
	ASSERT_TRUE(list_is_empty(&l), "La lista debe quedar vacia");
	TEST_PASSED("list_pull_last funciona");
}
TEST(ver_primero_sin_quitar)
{
	list_t l;
	int v[] = { 10, 20, 30 };
	int x, y;
	list_create(&l);
	cargarEnterosEnLista(&l, v, 3);
	ASSERT_EQUAL(OK, list_see_first(&l, &x, sizeof(int)),
		     "list_see_first debe retornar OK");
	ASSERT_EQUAL(10, x, "Debe ver el primer elemento");
	ASSERT_EQUAL(OK, list_pull_first(&l, &y, sizeof(int)),
		     "list_pull_first debe retornar OK");
	ASSERT_EQUAL(10, y, "El primero sigue siendo 10");
	TEST_PASSED("list_see_first funciona sin quitar");
}

TEST(longitud_lista_vacia)
{
	list_t l;

	list_create(&l);

	ASSERT_EQUAL(0, list_len(&l), "Una lista vacia debe medir 0");
	TEST_PASSED("list_len en lista vacia");
}

TEST(longitud_lista_con_elementos)
{
	list_t l;
	int v[] = { 10, 20, 30 };

	list_create(&l);
	cargarEnterosEnLista(&l, v, 3);

	ASSERT_EQUAL(3, list_len(&l), "La longitud debe coincidir");
	TEST_PASSED("list_len con elementos");
}

TEST(ver_elemento_en_posicion)
{
	list_t l;
	int v[] = { 10, 20, 30 };
	int x;

	list_create(&l);
	cargarEnterosEnLista(&l, v, 3);

	ASSERT_EQUAL(OK, list_see_in_pos(&l, &x, sizeof(int), 1),
		     "list_show_in_position debe retornar OK");
	ASSERT_EQUAL(20, x, "La posicion 1 debe ser 20");
	TEST_PASSED("list_show_in_position funciona");
}

TEST(buscar_en_lista)
{
	list_t l;
	int v[] = { 10, 20, 30 };
	int clave;

	list_create(&l);
	cargarEnterosEnLista(&l, v, 3);

	clave = 20;
	ASSERT_EQUAL(OK, list_search(&l, &clave, sizeof(int), Comp),
		     "list_search debe retornar OK");
	ASSERT_EQUAL(20, clave, "Debe devolver el elemento buscado");

	clave = 99;
	ASSERT_EQUAL(LIST_ERR_NOT_FOUND,
		     list_search(&l, &clave, sizeof(int), Comp),
		     "Debe informar no encontrado");
	TEST_PASSED("list_search funciona");
}

TEST(copiar_lista)
{
	list_t l;
	list_t copia;
	int v[] = { 10, 20, 30 };
	int x;
	int esperado[] = { 10, 20, 30 };

	list_create(&l);
	list_create(&copia);
	cargarEnterosEnLista(&l, v, 3);

	ASSERT_EQUAL(OK, list_copy(&l, &copia), "list_copy debe retornar OK");

	verificarSecuenciaEnteros(&copia, esperado, 3);
	ASSERT_TRUE(list_is_empty(&copia),
		    "La copia debe quedar vacia al consumirla");

	ASSERT_EQUAL(OK, list_pull_first(&l, &x, sizeof(int)),
		     "La lista original debe seguir intacta");
	ASSERT_EQUAL(10, x, "La copia no debe afectar a la original");
	TEST_PASSED("list_copy funciona");
}
/*
TEST(clonar_lista)
{
	list_t l;
	list_t clon;
	int v[] = { 10, 20, 30 };
	int x;
	int esperado[] = { 10, 20, 30 };

	list_create(&l);
	list_create(&clon);
	cargarEnterosEnLista(&l, v, 3);

	ASSERT_EQUAL(OK, clonarLista(&l, &clon),
		     "clonarLista debe retornar OK");

	verificarSecuenciaEnteros(&clon, esperado, 3);
	ASSERT_TRUE(list_is_empty(&clon), "El clon debe quedar vacio al consumirlo");

	ASSERT_EQUAL(OK, list_pull_first(&l, &x, sizeof(int)),
		     "La lista original debe seguir intacta");
	ASSERT_EQUAL(10, x, "El clon no debe afectar a la original");
	TEST_PASSED("clonarLista funciona");
}
*/
TEST(insertar_en_posicion)
{
	list_t l;
	int v[] = { 1, 3 };
	int x;
	int nuevo = 2;

	list_create(&l);
	cargarEnterosEnLista(&l, v, 2);

	ASSERT_EQUAL(OK, list_push_in_pos(&l, &nuevo, sizeof(int), 1),
		     "list_push_in_pos debe retornar OK");

	ASSERT_EQUAL(OK, list_pull_first(&l, &x, sizeof(int)),
		     "list_pull_first debe retornar OK");
	ASSERT_EQUAL(1, x, "Debe quedar primero el 1");
	ASSERT_EQUAL(OK, list_pull_first(&l, &x, sizeof(int)),
		     "list_pull_first debe retornar OK");
	ASSERT_EQUAL(2, x, "Debe insertarse en la posicion pedida");
	ASSERT_EQUAL(OK, list_pull_first(&l, &x, sizeof(int)),
		     "list_pull_first debe retornar OK");
	ASSERT_EQUAL(3, x, "Debe quedar ultimo el 3");
	TEST_PASSED("list_push_in_pos funciona");
}

TEST(eliminar_por_posicion)
{
	list_t l;
	int v[] = { 1, 2, 3 };
	int x;

	list_create(&l);
	cargarEnterosEnLista(&l, v, 3);

	ASSERT_EQUAL(OK, list_delete_pos(&l, &x, sizeof(int), 1),
		     "list_delete_pos debe retornar OK");
	ASSERT_EQUAL(2, x, "Debe eliminar la posicion solicitada");

	ASSERT_EQUAL(OK, list_pull_first(&l, &x, sizeof(int)),
		     "list_pull_first debe retornar OK");
	ASSERT_EQUAL(1, x, "La lista quedo mal");
	ASSERT_EQUAL(OK, list_pull_first(&l, &x, sizeof(int)),
		     "list_pull_first debe retornar OK");
	ASSERT_EQUAL(3, x, "La lista quedo mal");
	TEST_PASSED("list_delete_pos funciona");
}

TEST(invertir_lista)
{
	list_t l;
	int v[] = { 1, 2, 3 };
	int esperado[] = { 3, 2, 1 };

	list_create(&l);
	cargarEnterosEnLista(&l, v, 3);

	ASSERT_EQUAL(OK, list_invert(&l), "list_invert debe retornar OK");
	verificarSecuenciaEnteros(&l, esperado, 3);
	ASSERT_TRUE(list_is_empty(&l),
		    "La lista debe quedar vacia al consumirla");
	TEST_PASSED("list_invert funciona");
}

TEST(concatenar_listas)
{
	list_t l1;
	list_t l2;
	int v1[] = { 1, 2 };
	int v2[] = { 3, 4 };
	int esperado[] = { 1, 2, 3, 4 };

	list_create(&l1);
	list_create(&l2);
	cargarEnterosEnLista(&l1, v1, 2);
	cargarEnterosEnLista(&l2, v2, 2);

	ASSERT_EQUAL(OK, list_concat(&l1, &l2), "list_concat debe retornar OK");

	verificarSecuenciaEnteros(&l1, esperado, 4);
	ASSERT_TRUE(list_is_empty(&l1), "l1 debe quedar vacia al consumirla");
	ASSERT_TRUE(list_is_empty(&l2),
		    "l2 debe quedar vacia luego de concatenar");
	TEST_PASSED("list_concat funciona");
}
TEST(ver_ultimo_sin_quitar)
{
	list_t l;
	int v[] = { 10, 20, 30 };
	int x, y;
	list_create(&l);
	cargarEnterosEnLista(&l, v, 3);
	ASSERT_EQUAL(OK, list_see_last(&l, &x, sizeof(int)),
		     "list_see_last debe retornar OK");
	ASSERT_EQUAL(30, x, "Debe ver el ultimo elemento");
	ASSERT_EQUAL(OK, list_pull_last(&l, &y, sizeof(int)),
		     "list_pull_last debe retornar OK");
	ASSERT_EQUAL(30, y, "El ultimo sigue siendo 30");
	TEST_PASSED("list_see_last funciona sin quitar");
}
TEST(vaciar_lista)
{
	list_t l;
	int v[] = { 1, 2, 3 };
	int x;
	list_create(&l);
	cargarEnterosEnLista(&l, v, 3);
	list_clear(&l);
	ASSERT_TRUE(list_is_empty(&l) == TRUE, "La lista debe quedar vacia");
	ASSERT_EQUAL(OK, list_push_last(&l, &v[0], sizeof(int)),
		     "La lista vaciada debe seguir funcionando");
	ASSERT_EQUAL(OK, list_pull_first(&l, &x, sizeof(int)),
		     "Debe poder sacar luego de vaciar");
	ASSERT_EQUAL(1, x, "Debe salir el valor insertado");
	TEST_PASSED("list_clear funciona");
}
TEST(insertar_ordenado_sin_duplicados)
{
	list_t l;
	int v[] = { 3, 1, 4, 2 };
	int x;
	list_create(&l);
	ASSERT_EQUAL(OK, list_push_orderer(&l, &v[0], sizeof(int), Comp, NULL),
		     "list_push_orderer debe retornar OK");
	ASSERT_EQUAL(OK, list_push_orderer(&l, &v[1], sizeof(int), Comp, NULL),
		     "list_push_orderer debe retornar OK");
	ASSERT_EQUAL(OK, list_push_orderer(&l, &v[2], sizeof(int), Comp, NULL),
		     "list_push_orderer debe retornar OK");
	ASSERT_EQUAL(OK, list_push_orderer(&l, &v[3], sizeof(int), Comp, NULL),
		     "list_push_orderer debe retornar OK");
	ASSERT_EQUAL(OK, list_pull_first(&l, &x, sizeof(int)),
		     "list_pull_first debe retornar OK");
	ASSERT_EQUAL(1, x, "Orden incorrecto");
	ASSERT_EQUAL(OK, list_pull_first(&l, &x, sizeof(int)),
		     "list_pull_first debe retornar OK");
	ASSERT_EQUAL(2, x, "Orden incorrecto");
	ASSERT_EQUAL(OK, list_pull_first(&l, &x, sizeof(int)),
		     "list_pull_first debe retornar OK");
	ASSERT_EQUAL(3, x, "Orden incorrecto");
	ASSERT_EQUAL(OK, list_pull_first(&l, &x, sizeof(int)),
		     "list_pull_first debe retornar OK");
	ASSERT_EQUAL(4, x, "Orden incorrecto");
	TEST_PASSED("list_push_orderer sin duplicados funciona");
}
TEST(insertar_ordenado_con_duplicados_sin_insertar)
{
	list_t l;
	int v[] = { 3, 1, 3, 2 };
	int x;
	list_create(&l);
	ASSERT_EQUAL(OK, list_push_orderer(&l, &v[0], sizeof(int), Comp, NULL),
		     "list_push_orderer debe retornar OK");
	ASSERT_EQUAL(OK, list_push_orderer(&l, &v[1], sizeof(int), Comp, NULL),
		     "list_push_orderer debe retornar OK");
	ASSERT_EQUAL(OK, list_push_orderer(&l, &v[2], sizeof(int), Comp, NULL),
		     "list_push_orderer debe retornar OK");
	ASSERT_EQUAL(OK, list_push_orderer(&l, &v[3], sizeof(int), Comp, NULL),
		     "list_push_orderer debe retornar OK");
	ASSERT_EQUAL(OK, list_pull_first(&l, &x, sizeof(int)),
		     "list_pull_first debe retornar OK");
	ASSERT_EQUAL(1, x, "Orden incorrecto");
	ASSERT_EQUAL(OK, list_pull_first(&l, &x, sizeof(int)),
		     "list_pull_first debe retornar OK");
	ASSERT_EQUAL(2, x, "Orden incorrecto");
	ASSERT_EQUAL(OK, list_pull_first(&l, &x, sizeof(int)),
		     "list_pull_first debe retornar OK");
	ASSERT_EQUAL(3, x, "Orden incorrecto");
	ASSERT_TRUE(list_is_empty(&l), "Debe quedar solo una instancia del 3");
	TEST_PASSED("list_push_orderer sin duplicados funciona");
}
TEST(insertar_ordenado_con_duplicados_y_accion)
{
	list_t l;
	int v[] = { 5, 5 };
	int x;
	list_create(&l);
	ASSERT_EQUAL(OK,
		     list_push_orderer(&l, &v[0], sizeof(int), Comp,
				       sumarAExistente),
		     "list_push_orderer debe retornar OK");
	ASSERT_EQUAL(OK,
		     list_push_orderer(&l, &v[1], sizeof(int), Comp,
				       sumarAExistente),
		     "list_push_orderer debe retornar OK");
	ASSERT_EQUAL(OK, list_pull_first(&l, &x, sizeof(int)),
		     "list_pull_first debe retornar OK");
	ASSERT_EQUAL(
		10, x,
		"La accion debe haber acumulado el duplicado en el existente");
	TEST_PASSED("list_push_orderer con accion funciona");
}
TEST(eliminar_por_clave_existente)
{
	list_t l;
	int v[] = { 1, 2, 3, 4, 5 };
	int clave, x;
	list_create(&l);
	cargarEnterosEnLista(&l, v, 5);
	clave = 3;
	ASSERT_EQUAL(OK, list_delete_by_key(&l, &clave, sizeof(int), Comp),
		     "list_delete_by_key debe retornar OK");
	ASSERT_EQUAL(3, clave, "Debe devolver el eliminado");
	ASSERT_EQUAL(OK, list_pull_first(&l, &x, sizeof(int)),
		     "list_pull_first debe retornar OK");
	ASSERT_EQUAL(1, x, "La lista quedo mal");
	ASSERT_EQUAL(OK, list_pull_first(&l, &x, sizeof(int)),
		     "list_pull_first debe retornar OK");
	ASSERT_EQUAL(2, x, "La lista quedo mal");
	ASSERT_EQUAL(OK, list_pull_first(&l, &x, sizeof(int)),
		     "list_pull_first debe retornar OK");
	ASSERT_EQUAL(4, x, "Debe faltar el 3");
	ASSERT_EQUAL(OK, list_pull_first(&l, &x, sizeof(int)),
		     "list_pull_first debe retornar OK");
	ASSERT_EQUAL(5, x, "La lista quedo mal");
	TEST_PASSED("list_delete_by_key existente funciona");
}
TEST(eliminar_por_clave_inexistente)
{
	list_t l;
	int v[] = { 1, 2, 4, 5 };
	int clave;
	list_create(&l);
	cargarEnterosEnLista(&l, v, 4);
	clave = 3;
	ASSERT_EQUAL(LIST_ERR_NOT_FOUND,
		     list_delete_by_key(&l, &clave, sizeof(int), Comp),
		     "Debe devolver no encontrado");
	TEST_PASSED("list_delete_by_key inexistente funciona");
}

TEST(merge_sort_lista_vacia)
{
	list_t l;
	list_create(&l);
	ASSERT_EQUAL(LIST_ERR_EMPTY, list_order(&l, MERGE, Comp),
		     "Merge sort sobre lista vacia debe andar");
	ASSERT_TRUE(list_is_empty(&l), "Debe seguir vacia");
	TEST_PASSED("merge sort lista vacia");
}
TEST(merge_sort_par)
{
	list_t l;
	int v[] = { 4, 1, 3, 2 };
	int esperado[] = { 1, 2, 3, 4 };
	list_create(&l);
	cargarEnterosEnLista(&l, v, 4);
	ASSERT_EQUAL(OK, list_order(&l, MERGE, Comp),
		     "Merge sort debe retornar OK");
	verificarSecuenciaEnteros(&l, esperado, 4);
	TEST_PASSED("merge sort par");
}
TEST(merge_sort_impar)
{
	list_t l;
	int v[] = { 5, 1, 4, 2, 3 };
	int esperado[] = { 1, 2, 3, 4, 5 };
	list_create(&l);
	cargarEnterosEnLista(&l, v, 5);
	ASSERT_EQUAL(OK, list_order(&l, MERGE, Comp),
		     "Merge sort debe retornar OK");
	verificarSecuenciaEnteros(&l, esperado, 5);
	TEST_PASSED("merge sort impar");
}

TEST(quick_sort_lista_vacia)
{
	list_t l;
	list_create(&l);
	ASSERT_EQUAL(LIST_ERR_EMPTY, list_order(&l, QUICK, Comp),
		     "Quick sort sobre lista vacia debe andar");
	ASSERT_TRUE(list_is_empty(&l), "Debe seguir vacia");
	TEST_PASSED("quick sort lista vacia");
}
TEST(quick_sort_par)
{
	list_t l;
	int v[] = { 4, 1, 3, 2 };
	int esperado[] = { 1, 2, 3, 4 };
	list_create(&l);
	cargarEnterosEnLista(&l, v, 4);
	ASSERT_EQUAL(OK, list_order(&l, QUICK, Comp),
		     "Quick sort debe retornar OK");
	verificarSecuenciaEnteros(&l, esperado, 4);
	TEST_PASSED("quick sort par");
}
TEST(quick_sort_impar)
{
	list_t l;
	int v[] = { 5, 1, 4, 2, 3 };
	int esperado[] = { 1, 2, 3, 4, 5 };
	list_create(&l);
	cargarEnterosEnLista(&l, v, 5);
	ASSERT_EQUAL(OK, list_order(&l, QUICK, Comp),
		     "Quick sort debe retornar OK");
	verificarSecuenciaEnteros(&l, esperado, 5);
	TEST_PASSED("quick sort impar");
}
TEST(mostrar_lista_invertida)
{
	list_t l;
	int v[] = { 1, 2, 3, 4, 5 };
	list_create(&l);
	cargarEnterosEnLista(&l, v, 5);

	list_show_invert(&l, printLista);
	TEST_PASSED("mostrar lista invertida exitoso");
}
int main(void)
{
	RUN_TEST(mostrar_lista_invertida);
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
