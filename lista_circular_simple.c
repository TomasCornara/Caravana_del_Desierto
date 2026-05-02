#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lista_circular_simple.h"

void crearListaSimple(t_listaCircular* l){
    *l = NULL;
}

int listaSimpleLlena(const t_listaCircular* l, unsigned tam){
    return 0;
}

int listaSimpleVacia(const t_listaCircular* l){
    return !(*l);
}

int ponerAlPrincipioEnListaSimple(t_listaCircular* l, const void* dato, unsigned tam_dato){
    t_nodo* nue;

    if(!l || !tam_dato) return 0;

    nue = malloc(sizeof(t_nodo) + tam_dato);
    if(!nue) return 0;

    nue->dato = nue + 1;
    nue->tam_dato = tam_dato;
    memcpy(nue->dato,dato,tam_dato);

    if(*l){
        nue->sig = (*l)->sig;
        (*l)->sig = nue;
    } else {
        nue->sig = nue;
    }

    *l = nue;

    return 1;
}

int verPrimeroListaSimple(const t_listaCircular* l, void* dato, unsigned tam_dato){
    if(!l || !(*l)) return 0;

    memcpy(dato,(*l)->dato,MIN(tam_dato,(*l)->tam_dato));

    return 1;

}

void vaciarListaSimple(t_listaCircular* l){
    if(!l || !*l)
        return;

    t_nodo* actual = *l;
    t_nodo* sig;

    do {
        sig = actual->sig;
        //free(actual->dato);
        free(actual);
        actual = sig;
    } while(actual != *l);

    *l = NULL;
}

void moverAlSiguiente(t_listaCircular* l){
    if(!l || !(*l)) return;

    *l = (*l)->sig;
}
