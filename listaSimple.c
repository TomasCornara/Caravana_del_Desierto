#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "listaSimple.h"

void crearLista(t_lista* l){
    *l = NULL;
}

int listaVacia(const t_lista* l){
    return !(*l);
}

int listaLlena(const t_lista* l, unsigned tam){
    return 0;
}

int ponerEnLista(t_lista* l, const void* dato, unsigned tam_dato,cmp comparar, act accion){
    t_nodoLista* nue;
    int c;

    if(!l || !tam_dato) return 0;

    if(!(nue = malloc(sizeof(t_nodoLista) + tam_dato))) return 0;

    while(*l && (c = comparar(dato,(*l)->dato)) <= 0){
        if(!c){
            if(accion) accion((*l)->dato, dato);
            free(nue);
            return 1;
        }
        l = &(*l)->sig;
    }


    nue->sig = *l;
    nue->tam = tam_dato;
    nue->dato = nue + 1;
    memcpy(nue->dato,dato,tam_dato);

    *l = nue;

    return 1;
}

int verPrimero(const t_lista* l, void* buffer, unsigned tam){
    if(!l || !(*l)) return 0;

    memcpy(buffer,(*l)->dato,MIN(tam,(*l)->tam));
    return 1;
}

void mapLista(const t_lista* l,act accion){
    while(*l){
        accion((*l)->dato,NULL);
        l = &(*l)->sig;
    }
}

void ordenarLista(t_lista* l, cmp comparar) {
    t_lista ordenada = NULL;
    t_nodoLista* actual = *l;
    t_nodoLista* sig;

    while (actual) {
        sig = actual->sig;
        t_lista* aux = &ordenada;

        while (*aux && comparar(actual->dato, (*aux)->dato) > 0) {
            aux = &(*aux)->sig;
        }

        actual->sig = *aux;
        *aux = actual;
        actual = sig;
    }
    *l = ordenada;
}

void vaciarLista(t_lista* l){
    t_nodoLista* aux;
    while (*l) {
        aux = *l;
        *l = aux->sig;
        free(aux);
    }
}

int sacarPrimero(t_lista* l, void* buffer, unsigned tam) {
    t_nodoLista* elim;

    if(!l || !(*l)) return 0;

    elim = *l;

    memcpy(buffer, elim->dato, MIN(tam, elim->tam));

    *l = elim->sig;
    free(elim);

    return 1;
}
