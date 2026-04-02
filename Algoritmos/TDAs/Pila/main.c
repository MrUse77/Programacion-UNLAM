#include "Pila.h"
#include <string.h>
#define MIN(a,b) (a > b ? b : a);


void crearPila(t_Pila *p){
	p->tope=TAM_PILA;
}

int pilaLlena(const t_Pila *p, unsigned tam){
	if(p->tope == 0){
		return TRUE;
	}
	return FALSE;
}

int pilaVacia(const t_Pila *p){
	return p->tope == TAM_PILA ? TRUE : FALSE;
}

void vaciarPila(t_Pila *p);

int apilar(t_Pila *p, const void *d, unsigned tam){
	if(pilaLlena(p, tam)){
		return ERR; 
	}
	p->tope -= tam;
	memcpy(p->Vector + p->tope, d, tam);
	p->tope -= sizeof(tam);
	memcpy(p->Vector+p->tope, &tam, sizeof(tam));
	return OK;
}

int desapilar(t_Pila *p, void *buffer, unsigned tamDato){
	unsigned tam;
	if(p->tope == TAM_PILA){
		return FALSE;
	}
	memcpy(&tam,p->Vector+p->tope,sizeof(tamDato));
	p->tope+=sizeof(tam);
	memcpy(buffer,p->Vector+p->tope,tam>tamDato ? tamDato : tam);
	p->tope+=tam;
	return OK;
}

int verTope(const t_Pila *p, void *buffer, unsigned tam);
