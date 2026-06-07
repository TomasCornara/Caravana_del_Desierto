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

typedef void (*tAccion)(void*a, void * parametro_extra);
typedef int (*tCmp)(void*a, void *b);

void crear_lista(tLista* pl);
int lista_vacia(const tLista* pl);
int lista_llena(const tLista* pl);
void vaciar_lista(tLista *pl);
void map_lista(const tLista* pl, tAccion accion, void* parms);
void modificar_elemento_segun_clave(tLista * pl, void * clave,tCmp cmp, tAccion accion, void * parametro_extra);
int agregar_ord_en_lista(tLista * pl, void * dato, unsigned tam, tCmp cmp);
void vaciar_lista_doble(tLista *pl);
int obtener_de_lista(const tLista* pl,const void* key, void* buffer, unsigned tam_dato, tCmp cmp);
void * obtener_de_lista_dir_dato(const tLista* pl,const void* key, tCmp cmp);
#endif // LISTA_CIRCULAR_DOBLE_H_INCLUDED
