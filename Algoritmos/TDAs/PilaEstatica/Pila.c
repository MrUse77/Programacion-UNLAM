#include "static_stack.h"
#include "Comun.h"
#include <string.h>

#define MIN(a, b) (a > b ? b : a)

void stack_init(stack_t *p)
{
	p->tope = TAM_PILA;
}

int stack_is_full(stack_t *p, unsigned tam)
{
	if (p->tope < tam + sizeof(tam)) {
		return TRUE;
	}
	return FALSE;
}

int stack_is_empty(stack_t *p)
{
	return p->tope == TAM_PILA ? TRUE : FALSE;
}

void stack_clear(stack_t *p)
{
	p->tope = TAM_PILA;
}

int stack_push(stack_t *p, const void *d, unsigned tam)
{
	if (stack_is_full(p, tam)) {
		return ERR_PILA_LLENA;
	}
	//De atras para adelante se apila, primero el dato
	p->tope -= tam;
	memcpy(p->Vector + p->tope, d, tam);
	//Luego va el tamaño del dato
	p->tope -= sizeof(tam);
	memcpy(p->Vector + p->tope, &tam, sizeof(tam));
	return OK;
}

int stack_pull(stack_t *p, void *buffer, unsigned tamDato)
{
	unsigned tam;
	if (stack_is_empty(p)) {
		return ERR_PILA_VACIA;
	}
	//El mas cercano a la direccion incial, siempre va a ser el tamaño del dato
	memcpy(&tam, p->Vector + p->tope, sizeof(tam));
	//Liberamos tope
	p->tope += sizeof(tam);
	//Luego el dato, se usa el minimo tamaño entre tam y tamDato;
	memcpy(buffer, p->Vector + p->tope, MIN(tam, tamDato));
	//Liberamos tope. SIEMPRE SERA EL TAMAÑO DEL ELEMENTO GUARDADO NUNCA EL DEL TAMDATO.
	//SI TAMDATO < TAM, SE PIERDE LA INFO
	p->tope += tam;
	return OK;
}

int stack_see_top(stack_t *p, void *buffer, unsigned tamDato)
{
	unsigned tam;
	if (p->tope == TAM_PILA) {
		return ERR_PILA_VACIA;
	}
	//Obtenemos tamaño del dato
	memcpy(&tam, p->Vector + p->tope, sizeof(tam));
	//Obtenemos el minimo entre tam y tamDato y lo pasamos al buffer
	memcpy(buffer, p->Vector + p->tope + sizeof(tam), MIN(tam, tamDato));
	return OK;
}
