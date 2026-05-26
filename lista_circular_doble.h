#ifndef LISTA_CIRCULAR_DOBLE_H_INCLUDED
#define LISTA_CIRCULAR_DOBLE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globales.h"

typedef struct sNodo {
    void *info;
    unsigned tamInfo;
    struct sNodo *sig;
    struct sNodo *ant;
} tNodo;

typedef tNodo* tLista;

void crear_lista(tLista* pl);
int lista_vacia(const tLista* pl);
int lista_llena(const tLista* pl);

int agregar_ord_lista(tLista *pl, void *dato, unsigned tam, int(*comparar)(const void*a,const void*b));


int agregar_final_lista(tLista *pl, const void *dato, unsigned tam);

int agregar_inicio_lista(tLista *pl, const void *dato, unsigned tam);

int eliminar_lista(tLista *pl, void *dato, unsigned tam, int (*comparar)(const void*, const void*));

void vaciar_lista(tLista *pl);

int ver_dato_lista(const tLista *pl, void *dato, unsigned tam);

void recorrer_adelante(tLista *pl);
void recorrer_atras(tLista *pl);
void recorrer_todoasc(tLista *pl);
void recorrer_tododes(tLista *pl);

#endif // LISTA_CIRCULAR_DOBLE_H_INCLUDED
