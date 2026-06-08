#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "arbolBinario.h"


void crearArbol(t_Arbol* arbol) {
    *arbol = NULL;
}

void destruirArbol(t_Arbol* arbol) {
    if(*arbol == NULL) return;
    destruirArbol(&(*arbol)->izq);
    destruirArbol(&(*arbol)->der);
    free(*arbol);
    *arbol = NULL;
}

int grabarEnArchivoRecursivo(const t_Arbol* arbol, FILE* destino, unsigned cantBytes) {
    if (*arbol == NULL) return 1;

    fwrite((*arbol)->dato, cantBytes, 1, destino);
    
    grabarEnArchivoRecursivo(&(*arbol)->izq, destino, cantBytes);
    grabarEnArchivoRecursivo(&(*arbol)->der, destino, cantBytes);

    return 1;
}

int grabarArbolEnArchivo(const t_Arbol* arbol, FILE* destino, unsigned cantBytes) {
    if (!destino) return 0;
    return grabarEnArchivoRecursivo(arbol, destino, cantBytes);
}

int buscarEnArbol(const t_Arbol* arbol, void* dato, unsigned cantBytes, comFunc cmp) {
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

int ponerEnArbol(t_Arbol* arbol, const void* dato, unsigned cantBytes, comFunc cmp) {
    int c;
    if(*arbol == NULL) {
        //Poner y return;
        t_Nodo* nue = malloc(sizeof(t_Nodo) + cantBytes);
        if(!nue) return 0;

        nue->dato = nue + 1;
        memcpy(nue->dato,dato,cantBytes);
        nue->tam = cantBytes;
        nue->der = NULL;
        nue->izq = NULL;
        *arbol = nue;
        return 1;
    }

    c = cmp(dato,(*arbol)->dato);

    if(c > 0) {
        return ponerEnArbol(&(*arbol)->der, dato, cantBytes, cmp);
    }

    if(c < 0) {
        return ponerEnArbol(&(*arbol)->izq, dato, cantBytes, cmp);
    }


    return 0;
}


void printInOrden(const t_Arbol* arbol,ptrFunc ptr) {

    if(*arbol == NULL) return;

    if((*arbol)->izq)
        printInOrden(&(*arbol)->izq,ptr);

    ptr((*arbol)->dato);

    if((*arbol)->der)
        printInOrden(&(*arbol)->der,ptr);
}

void printPreOrden(const t_Arbol* arbol,ptrFunc ptr) {

    if(*arbol == NULL) return;

    ptr((*arbol)->dato);

    if((*arbol)->izq)
        printPreOrden(&(*arbol)->izq,ptr);



    if((*arbol)->der)
        printPreOrden(&(*arbol)->der,ptr);
}

void printPosOrden(const t_Arbol* arbol,ptrFunc ptr) {

    if(*arbol == NULL) return;

    if((*arbol)->izq)
        printPosOrden(&(*arbol)->izq,ptr);

    if((*arbol)->der)
        printPosOrden(&(*arbol)->der,ptr);

    ptr((*arbol)->dato);
}

int calcularAltura(const t_Arbol* arbol) {
    if(*arbol == NULL)
        return 0;

    int altIzq = calcularAltura(&(*arbol)->izq);
    int altDer = calcularAltura(&(*arbol)->der);

    return 1 + (altIzq > altDer ? altIzq : altDer);
}

int esAVL(const t_Arbol* arbol) {
    int altIzq;
    int altDer;

    if(*arbol == NULL)
        return 1;

    altIzq = calcularAltura(&(*arbol)->izq);
    altDer = calcularAltura(&(*arbol)->der);

    if(ABS(altIzq - altDer) > 1)
        return 0;

    return esAVL(&(*arbol)->izq) && esAVL(&(*arbol)->der);
}

void mostrarArbol(const t_Arbol* arbol, ptrFunc ptr) {
    mostrarArbol_recursivo(arbol,0,ptr);
}

void mostrarArbol_recursivo(const t_Arbol* arbol, unsigned nivel, ptrFunc ptr) {
    if(*arbol == NULL) {
        return;
    }

    mostrarArbol_recursivo(&(*arbol)->der,nivel + 1, ptr);
    for(int i = 0; i < nivel; i++)
        printf("    ");

    ptr((*arbol)->dato);
    printf("\n");

    mostrarArbol_recursivo(&(*arbol)->izq,nivel + 1, ptr);
}

int contarHojas(t_Arbol* arbol) {
    //int hojasDer,
    //   hojasIzq;

    if(*arbol == NULL)
        return 0;

    //hojasDer = contarHojas(&(*arbol)->der);
    //hojasIzq = contarHojas(&(*arbol)->izq);

    return ((((*arbol)->der || (*arbol)->izq))? 0:1) + contarHojas(&(*arbol)->der) + contarHojas(&(*arbol)->izq);
}

int contarRamas(t_Arbol* arbol) {
    if(*arbol == NULL)
        return 0;

    return ((((*arbol)->der || (*arbol)->izq))? 1:0) + contarRamas(&(*arbol)->der) + contarRamas(&(*arbol)->izq);
}

int eliminarNodo(t_Arbol* arbol, const void* key, comFunc cmp) {
    int c;
    t_Nodo* elim = *arbol;

    ///Proceso de busqueda
    //Caso base: No se encontro el nodo
    if(elim == NULL) return 0;

    //Busqueda del nodo
    c = cmp(key,elim->dato);
    if(c > 0) return eliminarNodo(&elim->der,key,cmp);
    if(c < 0) return eliminarNodo(&elim->izq,key,cmp);

    ///Proceso de eliminacion
    //Caso 1: Sin hijos
    if(!elim->der && !elim->izq){
        free(elim);
        *arbol = NULL;
        return 1;
    }

    //Caso 2: Dos hijos
    if(elim->der && elim->izq){
        t_Nodo* min = elim->der;
        while(min->izq){
            min = min->izq;
        }
        memcpy(elim->dato, min->dato, elim->tam);
        return eliminarNodo(&elim->der, min->dato, cmp);
    }

    //Caso 3: Un hijo
    if(elim->der){
        *arbol = elim->der;
    } else {
        *arbol = elim->izq;
    }
    free(elim);
    return 1;
}

int cargarDesdeArchivo(t_Arbol* arbol,
                       unsigned ce,
                       unsigned cantBytes,
                       FILE* origen)
{
    if(ce == 0)
        return 1;

    int medio = ce / 2;

    cargarDesdeArchivo(&(*arbol)->izq, medio, cantBytes, origen);

    t_Nodo* nue = malloc(sizeof(t_Nodo) + cantBytes);
    if(!nue)
        return 0;

    nue->tam = cantBytes;

    fread(nue->dato, cantBytes, 1, origen);

    cargarDesdeArchivo(&(*arbol)->der,
                       ce - medio - 1,
                       cantBytes,
                       origen);

    return 1;
}

int esAVL2(const t_Arbol* arbol){
    int altIzq, altDer;

    if(*arbol == NULL){
        return 1;
    }

    altIzq = calcularAltura(&(*arbol)->der);
    altDer = calcularAltura(&(*arbol)->izq);

    if(ABS(altIzq - altDer) > 1){
        return 0;
    }

    return(esAVL2(&(*arbol)->izq) && esAVL2(&(*arbol)->der));
}

void recorrerInOrden(const t_Arbol* arbol, ptrFunc ptr) {
    if (!arbol || !(*arbol)) return;
    recorrerInOrden(&(*arbol)->izq, ptr);
    ptr((*arbol)->dato);
    recorrerInOrden(&(*arbol)->der, ptr);
}