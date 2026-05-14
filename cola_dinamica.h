#ifndef COLADINAMICA_H_INCLUDED
#define COLADINAMICA_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(A,B) (((A)>(B))? (B):(A))

#define OK 1
#define ERROR 0

struct sNodo
{
    struct sNodo*sig;
    unsigned tamElemento;
    void * elemento;
};

struct tCola
{
    struct sNodo * pPrimero;
    struct sNodo * pUltimo;
};

void crearCola(struct tCola * cola);
int colaVacia(const struct  tCola* cola);
int colaLlena(const struct  tCola* cola, unsigned tamElemento);
int acolar(struct tCola * cola,const void * elemento, unsigned tamElemento);
int desacolar(struct tCola*cola, void*destino, unsigned tamanio);
void vaciarCola(struct tCola* cola);
int verTopeCola(const struct tCola * cola, void*destino, unsigned tamanio);

#endif // COLADINAMICA_H_INCLUDED
