#include <lista_simple.h>
#include <test_framework.h>
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
	ASSERT_EQUAL(LIST_SUCCESS, list_pull_first(&l, &aux, sizeof(int)),
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
	ASSERT_EQUAL(LIST_SUCCESS, list_pull_last(&l, &aux, sizeof(int)),
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
	ASSERT_EQUAL(LIST_SUCCESS, list_pull_in_pos(&l, &aux, sizeof(int), 3),
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
		     list_pull_in_pos(&l, &aux, sizeof(int), 3),
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
	ASSERT_EQUAL(LIST_SUCCESS, list_pull_first(&l, &aux, sizeof(int)),
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
	ASSERT_EQUAL(LIST_ERR_EMPTY, list_pull_first(&l, &aux, sizeof(int)),
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
	ASSERT_EQUAL(LIST_SUCCESS, list_pull_first(&l, &aux, sizeof(char)),
		     "No se desencolo correctamente el elemento");
	c = 'C';
	ASSERT_EQUAL(c, aux, "No se desencolo correctamente el elemento");
	ASSERT_EQUAL(LIST_SUCCESS, list_pull_first(&l, &aux, sizeof(char)),
		     "No se desencolo correctamente el elemento");
	c = 'B';
	ASSERT_EQUAL(c, aux, "No se desencolo correctamente el elemento");
	ASSERT_EQUAL(LIST_SUCCESS, list_pull_first(&l, &aux, sizeof(char)),
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

void contar(void *param, const void *dato)
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
	list_walk(&l, &cont, contar);
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

void sumar(void *param, const void *dato)
{
	int *aux = (int *)param, *data = (int *)dato;
	*aux += *data;
}

TEST(reducir_lista)
{
	list_t l;
	int c = 1, aux = 0;
	list_init(&l);
	ASSERT_EQUAL(LIST_SUCCESS, list_push_last(&l, &c, sizeof(char)),
		     "No se encolo correctamente el elemento");
	c++;
	ASSERT_EQUAL(LIST_SUCCESS, list_push_last(&l, &c, sizeof(char)),
		     "No se encolo correctamente el elemento");
	c++;
	ASSERT_EQUAL(LIST_SUCCESS, list_push_last(&l, &c, sizeof(char)),
		     "No se encolo correctamente el elemento");

	list_reduce(&l, &aux, sumar);
	ASSERT_EQUAL(6, aux, "No se reducio correctamente");
	TEST_PASSED("lista reducida correctamente");
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
	TEST_SUMMARY();
	return 0;
}
