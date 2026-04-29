#include "Pila.h"
#include "Comun.h"
#include <string.h>

#define MIN(a, b) (a > b ? b : a)
/** 
 * Crea una pila de tamaño definido 
 * @param *p - puntero a pila
 */
void crearPila(t_Pila *p)
{
	p->tope = TAM_PILA;
}

/** 
 * Revisa si la pila esta llena para el elemento que se quiera insertar
 * @param *p - puntero a pila
 * @param tam - tamaño del elemento
 * @returns {int} - Si hay espacio, devuelve FALSE, si no hay, devuelve TRUE
 */
int pilaLlena(const t_Pila *p, unsigned tam)
{
	if (p->tope < tam + sizeof(tam)) {
		return TRUE;
	}
	return FALSE;
}

/** 
 * Revisa si la pila esta vacia
 * @param *p - puntero a pila 
 * @returns {int} - Si esta vacia, devuelve TRUE, si no hay, devuelve FALSE
 */
int pilaVacia(const t_Pila *p)
{
	return p->tope == TAM_PILA ? TRUE : FALSE;
}

/** 
 * Vacia la pila entera
 * @param *p - puntero a pila 
 */
void vaciarPila(t_Pila *p)
{
	p->tope = TAM_PILA;
}

/** 
 * Coloca un elemento en el tope de la pila
 * @param *p - puntero a pila 
 * @param const *d - puntero a elemento 
 * @param tam - tamaño del elemento
 * @returns {int} - Si salio bien reotrna OK, si la pila esta llena devuelve ERR
 */
int apilar(t_Pila *p, const void *d, unsigned tam)
{
	if (pilaLlena(p, tam)) {
		return ERR;
	}
	//De atras para adelante se apila, primero el dato
	p->tope -= tam;
	memcpy(p->Vector + p->tope, d, tam);
	//Luego va el tamaño del dato
	p->tope -= sizeof(tam);
	memcpy(p->Vector + p->tope, &tam, sizeof(tam));
	return OK;
}

/** 
 * Se obtiene el elemento del tope de la pila y se descarta
 * @param *p - puntero a pila 
 * @param *buffer - puntero a buffer para obtener el elemento
 * @param tamDato - tamaño del buffer
 * Puede ocurrir que se obtenga un tamaño menor a tamDato;
 * si el tamaño del dato es mayor que tamDato, el sobrante del dato se pierde
 * @returns {int} - Si salio bien reotrna OK, si la pila esta vacia devuelve ERR
 */
int desapilar(t_Pila *p, void *buffer, unsigned tamDato)
{
	unsigned tam;
	if (pilaVacia(p)) {
		return ERR;
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

/** 
 *  Se obtiene el dato del tope de la pila sin eliminarlo
 * @param *p - puntero a pila 
 * @param *buffer - puntero a buffer para obtener el elemento
 * @param tamDato - tamaño del buffer
 * @returns {int} - Si salio bien reotrna OK, si la pila esta vacia devuelve ERR
 */
int verTope(const t_Pila *p, void *buffer, unsigned tamDato)
{
	unsigned tam;
	if (p->tope == TAM_PILA) {
		return FALSE;
	}
	//Obtenemos tamaño del dato
	memcpy(&tam, p->Vector + p->tope, sizeof(tam));
	//Obtenemos el minimo entre tam y tamDato y lo pasamos al buffer
	memcpy(buffer, p->Vector + p->tope + sizeof(tam), MIN(tam, tamDato));
	return OK;
}
