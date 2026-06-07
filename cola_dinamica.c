#include "cola_dinamica.h"

#define MIN(A,B) ((A > B)? (B):(A))

void crearCola(t_cola* cola)
{
    cola->pPrimero = NULL;
    cola->pUltimo = NULL;
}

int colaVacia(const t_cola* cola)
{
    return !cola->pPrimero;
}

int colaLlena(const  t_cola* cola, unsigned tamElemento)
{
    return 1;
}

int acolar(t_cola * cola,const void * elemento, unsigned tamElemento)
{
    tNodoCola* nue;

    nue = malloc(sizeof(tNodoCola));
    if(!nue || !(nue->elemento = malloc(tamElemento))){
        free(nue);
        return 0;
    }

    memcpy(nue->elemento,elemento,tamElemento);
    nue->tamElemento = tamElemento;
    nue->sig = NULL;

    if(!cola->pPrimero)
    {
        cola->pPrimero = nue;
    } else {
        cola->pUltimo->sig = nue;
    }
    cola->pUltimo = nue;

    return 1;
}

int desacolar(t_cola* cola, void* destino, unsigned tamanio)
{
    tNodoCola * elim;

    if(!cola || !cola->pPrimero) return 0;

    elim = cola->pPrimero;
    memcpy(destino, elim->elemento,MIN(elim->tamElemento,tamanio));

    cola->pPrimero = elim->sig;

    if(!cola->pPrimero)
    {
        cola->pUltimo = NULL;
    }

    free(elim->elemento);
    free(elim);

    return 1;
}

void vaciarCola(t_cola* cola)
{
    tNodoCola* nodoeliminado;

    if (!cola) {
        return;
    }

    while (cola->pPrimero != NULL)
    {
        nodoeliminado = cola->pPrimero;
        cola->pPrimero = nodoeliminado->sig;

        free(nodoeliminado->elemento);
        free(nodoeliminado);
    }

    cola->pUltimo = NULL;
}

int verTopeCola(const t_cola * cola, void* destino, unsigned tamanio)
{
    if (!cola || !cola->pPrimero) return 0;

    memcpy(destino, cola->pPrimero->elemento, MIN(cola->pPrimero->tamElemento, tamanio));

    return 1;
}



