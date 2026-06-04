#include <Cola.h>
#include "Comun.h"
#include <stdlib.h>
#include <string.h>

/* =============================================================================
   Implementación Dinámica - Cola con Linked List Tradicional (head/tail)
   Cada nodo contiene puntero a datos, tamaño y siguiente nodo
   ============================================================================= */

void queue_create(queue_t *c) {
    /* Inicializar cola dinámica vacía: no hay nodos */
    c->prim = NULL;  /* No hay primer nodo */
    c->ult = NULL;   /* No hay último nodo */
}

int queue_push(queue_t *c, const void *d, unsigned tamDato) {
    t_Nodo *n = (t_Nodo *)malloc(sizeof(t_Nodo));
    
    if (!n) {
        return ERR_MEM_LLENA;  /* Error: sin memoria para el nodo */
    }
    
    n->dato = malloc(tamDato);
    if (!n->dato) {
        free(n);
        return ERR_MEM_LLENA;  /* Error: sin memoria para los datos */
    }
    
    memcpy(n->dato, d, tamDato);
    n->tamDato = tamDato;
    n->sig = NULL;
    
    if (c->ult == NULL) {
        /* Cola vacía: el nuevo nodo es tanto primero como último */
        c->prim = n;
        c->ult = n;
    } else {
        /* Cola no vacía: conectar después del último nodo */
        c->ult->sig = n;
        c->ult = n;
    }
    
    return OK;
}

int queue_is_full(queue_t *c, unsigned tam) {
    /* Para implementación dinámica: siempre hay espacio (solo falla malloc) */
    /* Función mantenida por compatibilidad API con estática */
    (void)c;  /* Evitar advertencia de variable no usada */
    (void)tam;
    return FALSE;
}

int queue_see_first(queue_t *c, void *buff, unsigned tamDato) {
    if (c->prim == NULL) {
        return ERR_COLA_VACIA;  /* Cola vacía: no hay primer elemento */
    }
    
    memcpy(buff, c->prim->dato, MIN(tamDato, c->prim->tamDato));
    return OK;
}

int queue_pull(queue_t *c, void *buff, unsigned tamDato) {
    if (c->prim == NULL) {
        return ERR_COLA_VACIA;  /* Cola vacía: no hay elementos */
    }
    
    /* Obtener datos del primer nodo */
    memcpy(buff, c->prim->dato, MIN(tamDato, c->prim->tamDato));
    
    t_Nodo *aux = c->prim;
    
    if (c->prim == c->ult) {
        /* Solo hay un elemento: eliminarlo y dejar cola vacía */
        free(aux->dato);  /* Liberar datos del nodo */
        free(aux);        /* Liberar el nodo */
        c->prim = NULL;
        c->ult = NULL;
    } else {
        /* Hay múltiples elementos: mover el siguiente al frente */
        t_Nodo *sig = aux->sig;  /* Guardar siguiente antes de liberar */
        
        free(aux->dato);         /* Liberar datos del nodo */
        free(aux);               /* Liberar el nodo */
        
        c->prim = sig;           /* Nuevo primero es el antiguo segundo */
    }
    
    return OK;
}

int queue_is_empty(queue_t *c) {
    return (c->prim == NULL && c->ult == NULL) ? TRUE : FALSE;
}

void queue_clear(queue_t *c) {
    if (c->prim == NULL && c->ult == NULL) {
        /* Cola ya vacía: nada por hacer */
        return;
    }
    
    /* Liberar todos los nodos de la lista */
    t_Nodo *aux = c->prim;
    
    while (aux != NULL) {
        t_Nodo *sig = aux->sig;  /* Guardar siguiente antes de liberar */
        
        if (aux->dato != NULL) {
            free(aux->dato);      /* Liberar datos del nodo */
        }
        free(aux);                /* Liberar el nodo */
        aux = sig;               /* Avanzar al siguiente nodo */
    }
    
    c->prim = NULL;
    c->ult = NULL;
}

