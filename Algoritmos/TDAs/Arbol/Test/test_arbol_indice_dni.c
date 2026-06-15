#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "binary_tree.h"
#include "test_framework.h"

/* ---------- Estructura de datos ---------- */
typedef struct {
	char nombre[50];
	int dni;
} persona_t;

typedef struct {
	int dni;
	int idx;
} personaIDX_t;

int dniABUscar = 0;

/* ---------- Nombres para generar datos aleatorios ---------- */
static const char *NOMBRES[] = {
	"Juan",	 "Maria",   "Carlos", "Ana",	  "Pedro",
	"Laura", "Diego",   "Sofia",  "Martin",	  "Valentina",
	"Lucas", "Camila",  "Mateo",  "Isabella", "Tomas",
	"Mia",	 "Joaquin", "Emma",   "Santiago", "Catalina"
};
static const int CANT_NOMBRES = sizeof(NOMBRES) / sizeof(NOMBRES[0]);

/* ---------- Callback que escribe el DNI en el archivo ---------- */
static void escribir_dni_en_archivo(void *param, const void *dato)
{
	FILE *f = (FILE *)param;
	const personaIDX_t *p = (const personaIDX_t *)dato;
	fwrite(p, sizeof(personaIDX_t), 1, f);
}

void print(void *param, const void *dato)
{
	(void)param;
	personaIDX_t idx = *(personaIDX_t *)dato;

	printf("indice: %d, dni: %d\n", idx.idx, idx.dni);
}
/* ---------- Comparador por DNI ---------- */
static int cmp_dni(const void *a, const void *b)
{
	const personaIDX_t *pa = (const personaIDX_t *)a;
	const personaIDX_t *pb = (const personaIDX_t *)b;
	return pa->dni - pb->dni;
}

/* ---------- Genera archivo con N personas aleatorias ---------- */
static int generar_archivo_personas(const char *path, int cantidad)
{
	FILE *f = fopen(path, "wb");
	if (!f) {
		perror("No se pudo crear archivo de personas");
		return -1;
	}

	persona_t p;
	for (int i = 0; i < cantidad; i++) {
		/* Nombre aleatorio */
		int idx = rand() % CANT_NOMBRES;
		strncpy(p.nombre, NOMBRES[idx], sizeof(p.nombre) - 1);
		p.nombre[sizeof(p.nombre) - 1] = '\0';

		/* DNI aleatorio entre 1.000.000 y 50.000.000 */
		p.dni = 1000000 + rand() % 49000000;
		if (i == 27) {
			dniABUscar = p.dni;
		}

		fwrite(&p, sizeof(persona_t), 1, f);
	}

	fclose(f);
	return 0;
}

/* ---------- Lee archivo e inserta en el arbol ---------- */
static int cargar_personas_en_arbol(tree_t *arbol, const char *path)
{
	FILE *f = fopen(path, "rb");
	if (!f) {
		perror("No se pudo abrir archivo de personas");
		return -1;
	}

	persona_t p;
	int indice = 1;
	personaIDX_t idx;
	while (fread(&p, sizeof(persona_t), 1, f) == 1) {
		idx.dni = p.dni;
		idx.idx = indice;
		int code =
			tree_insert(arbol, &idx, sizeof(personaIDX_t), cmp_dni);
		if (code == TREE_EQUAL_VALUE) {
			/* DNI duplicado, lo salteamos */
			continue;
		}
		if (code != TREE_SUCCESS) {
			fprintf(stderr, "Error insertando DNI %d (code=%d)\n",
				p.dni, code);
			fclose(f);
			return -1;
		}
		indice++;
	}

	fclose(f);
	return 0;
}

/* ---------- TEST PRINCIPAL ---------- */
TEST(indice_arbol_por_dni)
{
	const char *archivo_entrada = "personas.dat";
	const char *archivo_salida = "dnis_ordenados.idx";
	const int CANT_PERSONAS = 100;

	/* Paso 1: Generar archivo con personas aleatorias */
	ASSERT_TRUE(generar_archivo_personas(archivo_entrada, CANT_PERSONAS) ==
			    0,
		    "Archivo de personas generado");

	/* Paso 2: Crear arbol y cargar personas */
	tree_t arbol;
	tree_init(&arbol);
	ASSERT_TRUE(cargar_personas_en_arbol(&arbol, archivo_entrada) == 0,
		    "Personas cargadas en el arbol");

	int nodos = tree_node_count(&arbol);
	printf("\n  Nodos en el arbol: %d (de %d generados)\n", nodos,
	       CANT_PERSONAS);

	printf("ANTES\n");
	tree_walk_in_order(&arbol, print, NULL);

	/* Paso 3: Recorrer inorden y escribir DNIs en archivo de salida */
	FILE *out = fopen(archivo_salida, "wb");
	ASSERT_TRUE(out != NULL, "Archivo de salida abierto");

	tree_walk_in_order(&arbol, escribir_dni_en_archivo, out);

	fclose(out);
	tree_destroy(&arbol);

	tree_init(&arbol);
	int code = tree_load_to_bin_file_sorted(&arbol, archivo_salida,
						sizeof(personaIDX_t));
	printf("DESPUES\n");
	tree_walk_in_order(&arbol, print, NULL);

	printf("codigo: %d\n", code);
	int altura = tree_height(&arbol);
	int nodosCont = tree_node_count(&arbol);
	printf("Altura: %d, Num Nodos: %d\n", altura, nodosCont);

	/* Limpieza */
	//	remove(archivo_entrada);
	//	remove(archivo_salida);

	TEST_PASSED("Indice de arbol por DNI con recorrido inorden a archivo");
}

TEST(menu_indice)
{
	const char *archivo_entrada = "personas.dat";
	const char *archivo_salida = "dnis_ordenados.idx";
	const int CANT_PERSONAS = 100;
	int state = 0;

	/* Paso 1: Generar archivo con personas aleatorias */
	ASSERT_TRUE(generar_archivo_personas(archivo_entrada, CANT_PERSONAS) ==
			    0,
		    "Archivo de personas generado");

	/* Paso 2: Crear arbol y cargar personas */
	tree_t arbol;
	tree_init(&arbol);
	ASSERT_TRUE(cargar_personas_en_arbol(&arbol, archivo_entrada) == 0,
		    "Personas cargadas en el arbol");

	tree_walk_in_order(&arbol, print, NULL);

	/* Paso 3: Recorrer inorden y escribir DNIs en archivo de salida */

	printf("seleccione opcion: \n");
	printf("1 Eliminar persona: \n");
	printf("2 Ingresar persona: \n");
	printf("3 Mostrar personas: \n");
	printf("0 Salir: \n");
	scanf("%d", &state);
	while (state != 0) {
		switch (state) {
		case 2: {
			printf("Ingrese DNI: ");

			persona_t pIdx = { dniABUscar, 0 };
			tree_delete_node(&arbol, &pIdx, sizeof(personaIDX_t),
					 cmp_dni);
			fwrite();
			break;
		}
		}
	}

	FILE *out = fopen(archivo_salida, "wb");
	ASSERT_TRUE(out != NULL, "Archivo de salida abierto");

	tree_walk_in_order(&arbol, escribir_dni_en_archivo, out);

	fclose(out);
	TEST_PASSED("Indice de arbol por DNI con recorrido inorden a archivo");
}

int main()
{
	srand(time(NULL));

	printf("\n");
	printf("╔════════════════════════════════════════════════╗\n");
	printf("║   TEST: Indice Arbol por DNI                   ║\n");
	printf("╚════════════════════════════════════════════════╝\n");

	RUN_TEST(indice_arbol_por_dni);

	TEST_SUMMARY();
	return 0;
}
