#ifndef COLADINAMICA_H_INCLUDED
#define COLADINAMICA_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globales.h"

typedef struct sNodoCola
{
    struct sNodoCola* sig;
    unsigned tamElemento;
    void* elemento;
}tNodoCola;

typedef struct
{
    tNodoCola * pPrimero;
    tNodoCola * pUltimo;
}t_cola;

void crearCola(t_cola* cola);
int colaVacia(const t_cola* cola);
int colaLlena(const  t_cola* cola, unsigned tamElemento);
int acolar(t_cola * cola,const void * elemento, unsigned tamElemento);
int desacolar(t_cola* cola, void* destino, unsigned tamanio);
void vaciarCola(t_cola* cola);
int verTopeCola(const t_cola * cola, void* destino, unsigned tamanio);

#endif // COLADINAMICA_H_INCLUDED
