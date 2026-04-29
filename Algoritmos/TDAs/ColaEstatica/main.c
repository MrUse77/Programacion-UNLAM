#include <Cola.h>
#include <stdlib.h>
#include <string.h>
#include <Comun.h>
#define MIN(a, b) (a >= b ? b : a)

void crearCola(t_Cola *c)
{
	c->prim = TAM_COLA - 20;
	c->ult = TAM_COLA - 20;
	c->tamDisp = TAM_COLA;
}
int ponerEnCola(t_Cola *c, const void *d, unsigned tamDato)
{
	if (colaLlena(c, tamDato)) {
		return ERR_COLA_LLENA;
	}

	unsigned ini, fin;
	ini = MIN(sizeof(tamDato), TAM_COLA - c->ult);
	fin = sizeof(tamDato) - ini;
	c->tamDisp -= sizeof(tamDato) + tamDato;
	if (ini != 0) {
		memcpy(c->Vector + c->ult, &tamDato, ini);
	}
	if (fin != 0) {
		memcpy(c->Vector, ((char *)&tamDato) + ini, fin);
	}
	c->ult = fin ? fin : c->ult + ini;
	ini = MIN(tamDato, TAM_COLA - c->ult);
	fin = tamDato - ini;
	if (ini != 0) {
		memcpy(c->Vector + c->ult, d, ini);
	}
	if (fin != 0) {
		memcpy(c->Vector, ((char *)d) + ini, fin);
	}
	c->ult = fin ? fin : c->ult + ini;
	return OK;
}
int colaLlena(t_Cola *c, unsigned tam)
{
	return c->tamDisp > tam + sizeof(tam) ? FALSE : TRUE;
}
int verPrimero(t_Cola *c, void *buff, unsigned tamDato)
{
	if (colaVacia(c)) {
		return ERR_COLA_VACIA;
	}

	unsigned ini, fin, tam, pos = c->prim;
	ini = MIN(sizeof(tamDato), TAM_COLA - pos);
	fin = sizeof(tamDato) - ini;
	if (ini != 0) {
		memcpy(&tam, c->Vector + pos, ini);
	}
	if (fin != 0) {
		memcpy(((char *)&tam) + ini, c->Vector, fin);
	}
	pos = fin ? fin : pos + ini;
	ini = MIN(MIN(tam, tamDato), TAM_COLA - pos);
	fin = tam - ini;
	if (ini != 0) {
		memcpy(buff, c->Vector + pos, ini);
	}
	if (fin != 0) {
		memcpy(((char *)buff) + ini, c->Vector, fin);
	}
	return OK;
}

int sacarDeCola(t_Cola *c, void *buff, unsigned tamDato)
{
	if (colaVacia(c)) {
		return ERR_COLA_VACIA;
	}

	unsigned ini, fin, tam;
	ini = MIN(sizeof(tamDato), TAM_COLA - c->prim);
	fin = sizeof(tamDato) - ini;
	c->tamDisp += sizeof(tamDato) + tamDato;
	if (ini != 0) {
		memcpy(&tam, c->Vector + c->prim, ini);
	}
	if (fin != 0) {
		memcpy(((char *)&tam) + ini, c->Vector, fin);
	}
	c->prim = fin ? fin : c->prim + ini;
	ini = MIN(MIN(tam, tamDato), TAM_COLA - c->prim);
	fin = tam - ini;
	if (ini != 0) {
		memcpy(buff, c->Vector + c->prim, ini);
	}
	if (fin != 0) {
		memcpy(((char *)buff) + ini, c->Vector, fin);
	}
	c->prim = fin ? fin : c->prim + ini;
	return OK;
}
int colaVacia(t_Cola *c)
{
	return c->tamDisp == TAM_COLA ? TRUE : FALSE;
}
void vaciarCola(t_Cola *c)
{
	c->tamDisp = TAM_COLA;
}
