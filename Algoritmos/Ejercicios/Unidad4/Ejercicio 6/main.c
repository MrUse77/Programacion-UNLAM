
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lista_simple.h>

#define NA 8 /* agrupaciones cargadas */
#define ND 20 /* distritos posibles (columnas del array de votos) */
#define DIST_ACTIVOS 6 /* distritos 1..6 con votos; 7..20 quedan en cero */
#define AGRUPACIONES "agrupaciones.txt"
#define VOTOS "votos.dat"

/* --- contrato del archivo binario ------------------------------------- */
typedef struct {
	int nagrup; /* numero de agrupacion, 1..9999 */
	int region; /* numero de region,     1..9  (no se usa en el proceso) */
	int distri; /* numero de distrito,   1..20 */
} voto_t; /* 12 bytes exactos */
/* --- agrupaciones del archivo de texto -------------------------------- */
typedef struct {
	int num;
	const char nom[256]; /* puede superar los 25 caracteres: a proposito */
} agrup_t;
typedef struct {
	int votos;
	const char nom[256]; /* puede superar los 25 caracteres: a proposito */
} votos_x_agrup_t;

void leer_txt(agrup_t *a, char *nom);
void leer_bin(agrup_t *a, voto_t *v, int votos[DIST_ACTIVOS][NA], char *nom);
int buscar_idx_agrup(int num, agrup_t *a, int cant_agrups);
void procesar_votos(list_t *l, agrup_t *a, int cant_agrups,
		    int votos[DIST_ACTIVOS][NA]);
void mostrar_top(list_t *l, int top);

int main()
{
	int votos[DIST_ACTIVOS][NA] = { 0 };
	agrup_t a[NA] = { 0 };
	voto_t v;
	list_t l;
	list_init(&l);
	leer_txt(a, AGRUPACIONES);
	leer_bin(a, &v, votos, VOTOS);
	procesar_votos(&l, a, NA, votos);
	mostrar_top(&l, 5);
}

void leer_txt(agrup_t *a, char *nom)
{
	FILE *f = fopen(nom, "rt");
	char linea[1024];
	if (!f) {
		return;
	}
	while (fgets(linea, sizeof(linea), f)) {
		sscanf(linea, "%d%[^\n]", &a->num, (char *)a->nom);
		a++;
	}
	fclose(f);
}

void leer_bin(agrup_t *a, voto_t *v, int votos[DIST_ACTIVOS][NA], char *nom)
{
	FILE *f = fopen(nom, "rb");
	if (!f) {
		return;
	}
	while (fread(v, sizeof(voto_t), 1, f)) {
		int idx = buscar_idx_agrup(v->nagrup, a, NA);
		if (idx >= 0 && v->distri > 0 && v->distri <= DIST_ACTIVOS &&
		    v->region > 1 && v->region < 10) {
			votos[v->distri - 1][idx]++;
		}
	}
	fclose(f);
}
int buscar_idx_agrup(int num, agrup_t *a, int cant_agrups)
{
	int cont = 0;
	while (cont < cant_agrups) {
		if (num == a->num) {
			return cont;
		}
		cont++;
		a++;
	}
	return -1;
}
int cmp_votos(const void *a, const void *b)
{
	return ((votos_x_agrup_t *)b)->votos - ((votos_x_agrup_t *)a)->votos;
}
void procesar_votos(list_t *l, agrup_t *a, int cant_agrups,
		    int votos[DIST_ACTIVOS][NA])
{
	votos_x_agrup_t va;
	int agrup_cont = 0;
	while (agrup_cont < cant_agrups) {
		strcpy((char *)va.nom, a->nom);
		va.votos = 0;
		for (int i = 0; i < DIST_ACTIVOS; i++) {
			va.votos += votos[i][agrup_cont];
		}
		list_push_in_order(l, &va, sizeof(votos_x_agrup_t), cmp_votos);
		agrup_cont++;
		a++;
	}
}
void print_votos(const void *dato)
{
	votos_x_agrup_t *va = (votos_x_agrup_t *)dato;
	printf("Agrupacion: %s, Votos: %d\n", va->nom, va->votos);
}
void mostrar_top(list_t *l, int top)
{
	list_t l_top;
	list_init(&l_top);
	list_top(l, &l_top, top);
	list_show(&l_top, print_votos);
	list_clear(&l_top);
}
