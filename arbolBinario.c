#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "arbolBinario.h"

#define ABS(X) ((X >= 0)? (X):((X) * (-1)))


void crearArbol(t_arbol* arbol){
    *arbol = NULL;
}

int arbolVacio(const t_arbol* arbol){
    if(!arbol) return 0;

    return !(*arbol);
}

int arbolLleno(const t_arbol* arbol, unsigned tam){
    if(!arbol) return 0;

    return 1;
}

void vaciarArbol(t_arbol* arbol){
    t_nodo* actual;

    if(!arbol || !(*arbol)){
        return;
    }

    actual = *arbol;

    if(actual->izq) vaciarArbol(&actual->izq);
    if(actual->der) vaciarArbol(&actual->der);

    free(actual->dato);
    free(actual);

    *arbol = NULL;
}

void destruirArbol(t_arbol* arbol){
    t_nodo* elim;

    if(!(*arbol)) return;

    elim = *arbol;

    if(elim->izq) destruirArbol(&elim->izq);
    if(elim->der) destruirArbol(&elim->der);

    free(elim->dato);
    free(elim);

    return;
}

void recorrerPosOrder(const t_arbol* arbol, ptr_func ptr){
    t_nodo* actual;

    if(!arbol || !(*arbol)) return;

    actual = *arbol;


    if(actual->izq) recorrerInOrder(&actual->izq,ptr);
    if(actual->der) recorrerInOrder(&actual->der,ptr);
    ptr(actual->dato);

    return;
}

void recorrerPreOrder(const t_arbol* arbol, ptr_func ptr){
    t_nodo* actual;

    if(!arbol || !(*arbol)) return;

    actual = *arbol;

    ptr(actual->dato);
    if(actual->izq) recorrerInOrder(&actual->izq,ptr);

    if(actual->der) recorrerInOrder(&actual->der,ptr);


    return;
}

void recorrerInOrder(const t_arbol* arbol, ptr_func ptr){
    t_nodo* actual;

    if(!arbol || !(*arbol)) return;

    actual = *arbol;

    if(actual->izq) recorrerInOrder(&actual->izq,ptr);
    ptr(actual->dato);
    if(actual->der) recorrerInOrder(&actual->der,ptr);

    return;
}

int ponerOrdenado(t_arbol* arbol, void* dato, unsigned tam_dato, cmp_func cmp, dup_func dup){
    t_nodo* nue;
    int c;

    if(!arbol) return 0;

    //Busqueda de posicion
    while(*arbol){
        c = cmp(dato,(*arbol)->dato);

        //Caso repetido
        if(c == 0){
            if(dup){
                dup((*arbol)->dato,dato);
                return 1;
            }
        }

        if(c > 0) arbol = &(*arbol)->der;
        if(c < 0) arbol = &(*arbol)->izq;
    }


    //Memoria
    nue = malloc(sizeof(t_nodo));
    if(!nue || !(nue->dato = malloc(tam_dato))){
        free(nue);
        return 0;
    }

    //Carga del nodo
    memcpy(nue->dato,dato,tam_dato);
    nue->tam_dato = tam_dato;
    nue->der = NULL;
    nue->izq = NULL;

    //Enlaze final
    *arbol = nue;

    return 1;
}

int calcular_altura(t_arbol* arbol){
    unsigned alt_izq;
    unsigned alt_der;
     t_nodo* actual;

    if(!arbol || !(*arbol)) return 0;

     actual = *arbol;

     alt_der = calcular_altura(&actual->der);
     alt_izq = calcular_altura(&actual->izq);

    return 1 + ((alt_der > alt_izq)? alt_der : alt_izq);
}

int buscarEnArbol(const t_arbol* arbol, void* dato, unsigned cantBytes, cmp_func cmp) {
    int c;
    if(*arbol == NULL) return 0;
    c = cmp(dato, (*arbol)->dato);
    if(c == 0) {
        memcpy(dato, (*arbol)->dato, cantBytes);
        return 1;
    }
    if(c > 0) return buscarEnArbol(&(*arbol)->der, dato, cantBytes, cmp);
    return buscarEnArbol(&(*arbol)->izq, dato, cantBytes, cmp);
}
