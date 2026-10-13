#include <lista_simple.h>
#include <test_framework.h>

int Comp(const void *a, const void *b)
{
	return *(int *)a - *(int *)b;
}
void sumarAExistente(void *param, const void *dato)
{
	*(int *)param += *(int *)dato;
}
void cargarEnterosEnLista(list_t *l, int *vec, int cant)
{
	int i;
	for (i = 0; i < cant; i++) {
		int aux = vec[i];
		ASSERT_EQUAL(LIST_SUCCESS, list_push_last(l, &aux, sizeof(int)),
			     "list_push_last debe retornar LIST_SUCCESS");
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
		res = list_pop_first(l, &aux, sizeof(int));
		printf("%d \n", aux);
		ASSERT_EQUAL(LIST_SUCCESS, res,
			     "list_pop_first debe retornar LIST_SUCCESS");
		ASSERT_EQUAL(vec[i], aux,
			     "La secuencia de la lista es incorrecta");
	}
}

TEST(lista_esta_vacia_al_iniciar)
{
	list_t l;
	list_init(&l);
	ASSERT_EQUAL(TRUE, list_is_empty(&l),
		     "La lista debe estar vacia al iniciar");
	TEST_PASSED("Cola vacia despues de iniciarla");
}

TEST(poner_elemento_primero_en_lista)
{
	list_t l;
	list_init(&l);
	int num = 7;
	ASSERT_EQUAL(LIST_SUCCESS, list_push_first(&l, &num, sizeof(int)),
		     "No se encolo correctamente el elemento");
	TEST_PASSED("Elemento colocado en la lista");
}
TEST(poner_elemento_ultimo_en_lista)
{
	list_t l;
	list_init(&l);
	int num = 7;
	ASSERT_EQUAL(LIST_SUCCESS, list_push_last(&l, &num, sizeof(int)),
		     "No se encolo correctamente el elemento");
	TEST_PASSED("Elemento colocado en la lista");
}

TEST(sacar_primer_elemento_en_lista)
{
	list_t l;
	list_init(&l);
	int num = 7, aux;
	ASSERT_EQUAL(LIST_SUCCESS, list_push_first(&l, &num, sizeof(int)),
		     "No se encolo correctamente el elemento");
	ASSERT_EQUAL(LIST_SUCCESS, list_pop_first(&l, &aux, sizeof(int)),
		     "No se desencolo correctamente el elemento");
	ASSERT_EQUAL(num, aux, "No se desencolo correctamente el elemento");
	TEST_PASSED("Elemento desencolocado de la lista");
}
TEST(sacar_ultimo_elemento_en_lista)
{
	list_t l;
	list_init(&l);
	int num = 7, aux;
	ASSERT_EQUAL(LIST_SUCCESS, list_push_last(&l, &num, sizeof(int)),
		     "No se encolo correctamente el elemento");
	ASSERT_EQUAL(LIST_SUCCESS, list_pop_last(&l, &aux, sizeof(int)),
		     "No se desencolo correctamente el elemento");
	ASSERT_EQUAL(num, aux, "No se desencolo correctamente el elemento");
	TEST_PASSED("Elemento desencolocado de la lista");
}

TEST(poner_elemento_en_pos_en_lista)
{
	list_t l;
	list_init(&l);
	int num = 7;
	list_push_last(&l, &num, sizeof(int));
	list_push_last(&l, &num, sizeof(int));
	list_push_last(&l, &num, sizeof(int));
	list_push_last(&l, &num, sizeof(int));
	list_push_last(&l, &num, sizeof(int));
	list_push_last(&l, &num, sizeof(int));
	ASSERT_EQUAL(LIST_SUCCESS, list_push_in_pos(&l, &num, sizeof(int), 3),
		     "No se encolo correctamente el elemento");

	TEST_PASSED("Elemento colocado de la lista");
}
TEST(sacar_elemento_de_pos_en_lista)
{
	list_t l;
	list_init(&l);
	int num = 7, aux;
	list_push_last(&l, &num, sizeof(int));
	list_push_last(&l, &num, sizeof(int));
	list_push_last(&l, &num, sizeof(int));
	list_push_last(&l, &num, sizeof(int));
	list_push_last(&l, &num, sizeof(int));
	list_push_last(&l, &num, sizeof(int));
	ASSERT_EQUAL(LIST_SUCCESS, list_push_in_pos(&l, &num, sizeof(int), 3),
		     "No se encolo correctamente el elemento");
	ASSERT_EQUAL(LIST_SUCCESS, list_pop_in_pos(&l, &aux, sizeof(int), 3),
		     "No se desencolo correctamente el elemento");
	ASSERT_EQUAL(num, aux, "No se desencolo correctamente el elemento");
	TEST_PASSED("Elemento desencolocado de la lista");
}

TEST(poner_elemento_en_pos_fuera_devuelve_error)
{
	list_t l;
	list_init(&l);
	int num = 7;
	list_push_last(&l, &num, sizeof(int));
	ASSERT_EQUAL(LIST_ERR_INVAL, list_push_in_pos(&l, &num, sizeof(int), 3),
		     "Func debe tirar Error");
	TEST_PASSED("Falla correctamente");
}
TEST(sacar_elemento_de_pos_fuera_devuelve_error)
{
	list_t l;
	list_init(&l);
	int num = 7, aux;
	list_push_last(&l, &num, sizeof(int));
	ASSERT_EQUAL(LIST_ERR_NOT_FOUND,
		     list_pop_in_pos(&l, &aux, sizeof(int), 3),
		     "Func debe tirar Error");
	TEST_PASSED("Falla correctamente");
}

TEST(desenlistar_unico_elemento_deja_lista_vacia)
{
	list_t l;
	list_init(&l);
	int num = 7, aux;
	ASSERT_EQUAL(LIST_SUCCESS, list_push_first(&l, &num, sizeof(int)),
		     "No se encolo correctamente el elemento");
	ASSERT_EQUAL(LIST_SUCCESS, list_pop_first(&l, &aux, sizeof(int)),
		     "No se desencolo correctamente el elemento");
	ASSERT_EQUAL(num, aux, "No se desencolo correctamente el elemento");
	ASSERT_EQUAL(TRUE, list_is_empty(&l), "La lista debe estar vacia");
	TEST_PASSED("Elemento desencolocado de la lista");
}

TEST(desenlistar_sin_elemento_devuelve_error)
{
	list_t l;
	int aux;
	list_init(&l);
	ASSERT_EQUAL(LIST_ERR_EMPTY, list_pop_first(&l, &aux, sizeof(int)),
		     "Desenlistar debe retornar error");
	TEST_PASSED("Desenlistar devuelve error de vacio");
}

TEST(desenlistar_varios_elementos)
{
	list_t l;
	char c = 'A', aux;
	list_init(&l);
	ASSERT_EQUAL(LIST_SUCCESS, list_push_first(&l, &c, sizeof(char)),
		     "No se encolo correctamente el elemento");
	c = 'B';
	ASSERT_EQUAL(LIST_SUCCESS, list_push_first(&l, &c, sizeof(char)),
		     "No se encolo correctamente el elemento");
	c = 'C';
	ASSERT_EQUAL(LIST_SUCCESS, list_push_first(&l, &c, sizeof(char)),
		     "No se encolo correctamente el elemento");
	ASSERT_EQUAL(LIST_SUCCESS, list_pop_first(&l, &aux, sizeof(char)),
		     "No se desencolo correctamente el elemento");
	c = 'C';
	ASSERT_EQUAL(c, aux, "No se desencolo correctamente el elemento");
	ASSERT_EQUAL(LIST_SUCCESS, list_pop_first(&l, &aux, sizeof(char)),
		     "No se desencolo correctamente el elemento");
	c = 'B';
	ASSERT_EQUAL(c, aux, "No se desencolo correctamente el elemento");
	ASSERT_EQUAL(LIST_SUCCESS, list_pop_first(&l, &aux, sizeof(char)),
		     "No se desencolo correctamente el elemento");
	c = 'A';
	ASSERT_EQUAL(c, aux, "No se desencolo correctamente el elemento");
	ASSERT_EQUAL(TRUE, list_is_empty(&l), "La lista debe estar vacia");
	TEST_PASSED("Elementos desencolocados de la lista");
}

TEST(vaciar_lista)
{
	list_t l;
	char c = 'A';
	list_init(&l);
	ASSERT_EQUAL(LIST_SUCCESS, list_push_first(&l, &c, sizeof(char)),
		     "No se encolo correctamente el elemento");
	c = 'B';
	ASSERT_EQUAL(LIST_SUCCESS, list_push_first(&l, &c, sizeof(char)),
		     "No se encolo correctamente el elemento");
	c = 'C';
	ASSERT_EQUAL(LIST_SUCCESS, list_push_first(&l, &c, sizeof(char)),
		     "No se encolo correctamente el elemento");
	list_clear(&l);

	ASSERT_EQUAL(TRUE, list_is_empty(&l),
		     "No se vacio correctamente la lista");
	TEST_PASSED("Cola vaciada correctamente");
}

void contar(void *dato, void *param)
{
	(*(int *)param)++;
}
TEST(caminar_por_lista)
{
	list_t l;
	char c = 'A';
	int cont = 0;
	list_init(&l);
	ASSERT_EQUAL(LIST_SUCCESS, list_push_first(&l, &c, sizeof(char)),
		     "No se encolo correctamente el elemento");
	c = 'B';
	ASSERT_EQUAL(LIST_SUCCESS, list_push_first(&l, &c, sizeof(char)),
		     "No se encolo correctamente el elemento");
	c = 'C';
	ASSERT_EQUAL(LIST_SUCCESS, list_push_first(&l, &c, sizeof(char)),
		     "No se encolo correctamente el elemento");
	list_foreach(&l, &cont, contar);
	ASSERT_EQUAL(cont, 3, "La lista no camino bien");

	TEST_PASSED("Cola vaciada correctamente");
}

void mostrar(const void *dato)
{
	printf("%c\n", *(char *)dato);
	ASSERT(1 == 1, "mostrar no funciona correctamente");
}
TEST(mostrar_lista)
{
	list_t l;
	char c = 'A';
	list_init(&l);
	ASSERT_EQUAL(LIST_SUCCESS, list_push_first(&l, &c, sizeof(char)),
		     "No se encolo correctamente el elemento");
	c = 'B';
	ASSERT_EQUAL(LIST_SUCCESS, list_push_first(&l, &c, sizeof(char)),
		     "No se encolo correctamente el elemento");
	c = 'C';
	ASSERT_EQUAL(LIST_SUCCESS, list_push_first(&l, &c, sizeof(char)),
		     "No se encolo correctamente el elemento");
	list_show(&l, mostrar);

	TEST_PASSED("Cola vaciada correctamente");
}

void sumar(void *dato, void *param)
{
	int *aux = (int *)param, *data = (int *)dato;
	*aux += *data;
}

TEST(reducir_lista)
{
	list_t l;
	int c = 1, aux = 0;
	list_init(&l);
	ASSERT_EQUAL(LIST_SUCCESS, list_push_last(&l, &c, sizeof(int)),
		     "No se encolo correctamente el elemento");
	c++;
	ASSERT_EQUAL(LIST_SUCCESS, list_push_last(&l, &c, sizeof(int)),
		     "No se encolo correctamente el elemento");
	c++;
	ASSERT_EQUAL(LIST_SUCCESS, list_push_last(&l, &c, sizeof(int)),
		     "No se encolo correctamente el elemento");

	list_reduce(&l, &aux, sumar);
	ASSERT_EQUAL(6, aux, "No se reducio correctamente");
	TEST_PASSED("lista reducida correctamente");
}

TEST(merge_sort_lista_vacia)
{
	list_t l;
	list_init(&l);
	ASSERT_EQUAL(LIST_SUCCESS, list_order(&l, MERGE, Comp),
		     "Merge sort sobre lista vacia debe andar");
	ASSERT_TRUE(list_is_empty(&l), "Debe seguir vacia");
	TEST_PASSED("merge sort lista vacia");
}
TEST(merge_sort_par)
{
	list_t l;
	int v[] = { 4, 1, 3, 2 };
	int esperado[] = { 1, 2, 3, 4 };
	list_init(&l);
	cargarEnterosEnLista(&l, v, 4);
	ASSERT_EQUAL(LIST_SUCCESS, list_order(&l, MERGE, Comp),
		     "Merge sort debe retornar LIST_SUCESS");
	verificarSecuenciaEnteros(&l, esperado, 4);
	TEST_PASSED("merge sort par");
}
TEST(merge_sort_impar)
{
	list_t l;
	int v[] = { 5, 1, 4, 2, 3 };
	int esperado[] = { 1, 2, 3, 4, 5 };
	list_init(&l);
	cargarEnterosEnLista(&l, v, 5);
	ASSERT_EQUAL(LIST_SUCCESS, list_order(&l, MERGE, Comp),
		     "Merge sort debe retornar LIST_SUCESS");
	verificarSecuenciaEnteros(&l, esperado, 5);
	TEST_PASSED("merge sort impar");
}

void sumar_map(void *dato, void *param)
{
	int *aux = (int *)param, *data = (int *)dato;
	*data += *aux;
}
TEST(map_lista)
{
	list_t l, l2;
	int c = 1, aux = 1;
	list_init(&l);
	list_init(&l2);
	ASSERT_EQUAL(LIST_SUCCESS, list_push_last(&l, &c, sizeof(int)),
		     "No se encolo correctamente el elemento");
	c++;
	ASSERT_EQUAL(LIST_SUCCESS, list_push_last(&l, &c, sizeof(int)),
		     "No se encolo correctamente el elemento");
	c++;
	ASSERT_EQUAL(LIST_SUCCESS, list_push_last(&l, &c, sizeof(int)),
		     "No se encolo correctamente el elemento");

	list_show(&l, printLista);
	list_map(&l, &l2, &aux, sumar_map);
	list_show(&l2, printLista);
	ASSERT_EQUAL(LIST_SUCCESS, list_pop_last(&l2, &c, sizeof(int)),
		     "No se encolo correctamente el elemento");

	ASSERT_EQUAL(4, c, "No se mapeo correctamente");
	ASSERT_EQUAL(LIST_SUCCESS, list_pop_last(&l2, &c, sizeof(int)),
		     "No se encolo correctamente el elemento");

	ASSERT_EQUAL(3, c, "No se mapeo correctamente");
	ASSERT_EQUAL(LIST_SUCCESS, list_pop_last(&l2, &c, sizeof(int)),
		     "No se encolo correctamente el elemento");

	ASSERT_EQUAL(2, c, "No se mapeo correctamente");
	TEST_PASSED("lista mapeada correctamente");
}

int cmp_list(const void *a, const void *b)
{
	if (*(int *)a == 2 || *(int *)a == 1) {
		return 0;
	}
	return -1;
}
TEST(filtrar_lista)
{
	list_t l, l2;
	int c = 1, aux = 1;
	list_init(&l);
	list_init(&l2);
	ASSERT_EQUAL(LIST_SUCCESS, list_push_last(&l, &c, sizeof(int)),
		     "No se encolo correctamente el elemento");
	c++;
	ASSERT_EQUAL(LIST_SUCCESS, list_push_last(&l, &c, sizeof(int)),
		     "No se encolo correctamente el elemento");
	c++;
	ASSERT_EQUAL(LIST_SUCCESS, list_push_last(&l, &c, sizeof(int)),
		     "No se encolo correctamente el elemento");

	list_show(&l, printLista);
	list_filter(&l, &l2, &aux, cmp_list);
	list_show(&l2, printLista);
	ASSERT_EQUAL(LIST_SUCCESS, list_pop_last(&l2, &c, sizeof(int)),
		     "No se encolo correctamente el elemento");

	ASSERT_EQUAL(2, c, "No se filtro correctamente");
	ASSERT_EQUAL(LIST_SUCCESS, list_pop_last(&l2, &c, sizeof(int)),
		     "No se encolo correctamente el elemento");

	ASSERT_EQUAL(1, c, "No se filtro correctamente");
	TEST_PASSED("lista filtrada correctamente");
}
int main()
{
	RUN_TEST(lista_esta_vacia_al_iniciar);
	RUN_TEST(poner_elemento_primero_en_lista);
	RUN_TEST(sacar_primer_elemento_en_lista);
	RUN_TEST(poner_elemento_ultimo_en_lista);
	RUN_TEST(sacar_ultimo_elemento_en_lista);
	RUN_TEST(poner_elemento_en_pos_en_lista);
	RUN_TEST(sacar_elemento_de_pos_en_lista);
	RUN_TEST(poner_elemento_en_pos_fuera_devuelve_error);
	RUN_TEST(sacar_elemento_de_pos_fuera_devuelve_error);
	RUN_TEST(caminar_por_lista);
	RUN_TEST(mostrar_lista);
	RUN_TEST(reducir_lista);
	RUN_TEST(desenlistar_unico_elemento_deja_lista_vacia);
	RUN_TEST(desenlistar_sin_elemento_devuelve_error);
	RUN_TEST(desenlistar_varios_elementos);
	RUN_TEST(vaciar_lista);
	RUN_TEST(merge_sort_lista_vacia);
	RUN_TEST(merge_sort_par);
	RUN_TEST(merge_sort_impar);
	RUN_TEST(map_lista);
	RUN_TEST(filtrar_lista);
	TEST_SUMMARY();
	return 0;
}
