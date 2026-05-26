#ifndef COLADINAMICA_H_INCLUDED
#define COLADINAMICA_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(A,B) (((A)>(B))? (B):(A))

#define OK 1
#define ERROR 0

typedef struct sNodo
{
    struct sNodo* sig;
    unsigned tamElemento;
    void* elemento;
}tNodo;

typedef struct
{
    struct sNodo * pPrimero;
    struct sNodo * pUltimo;
}t_cola;

void crearCola(t_cola* cola);
int colaVacia(const t_cola* cola);
int colaLlena(const t_cola* cola, unsigned tamElemento);
int acolar(t_cola * cola,const void * elemento, unsigned tamElemento);
int desacolar(t_cola* cola, void*destino, unsigned tamanio);
void vaciarCola(t_cola* cola);
int verTopeCola(const t_cola * cola, void*destino, unsigned tamanio);

#endif // COLADINAMICA_H_INCLUDED
