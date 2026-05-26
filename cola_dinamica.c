#include "cola_dinamica.h"


void crearCola(t_cola* cola)
{
    cola->pPrimero = NULL;
    cola->pUltimo = NULL;
}

int colaVacia(const t_cola* cola)
{
    return NULL == cola->pPrimero ? OK : ERROR;
}

int colaLlena(const  t_cola* cola, unsigned tamElemento)
{
    return OK;
}

int acolar(t_cola * cola,const void * elemento, unsigned tamElemento)
{
    tNodoCola * bufferNodo = malloc(sizeof(tNodoCola));
    if(!bufferNodo)
    {
        return ERROR;
    }

    void * bufferDato = malloc(tamElemento);
    if(!bufferDato)
    {
        free(bufferNodo);
        return ERROR;
    }

    memcpy(bufferDato, elemento, tamElemento);
    bufferNodo->elemento = bufferDato;
    bufferNodo->tamElemento = tamElemento;
    bufferNodo->sig = NULL;

    if(cola->pPrimero == NULL)
    {
        cola->pPrimero = bufferNodo;
    }
    else
    {
        cola->pUltimo->sig = bufferNodo;
    }
    cola->pUltimo = bufferNodo;

    return OK;
}

int desacolar(t_cola* cola, void* destino, unsigned tamanio)
{
    tNodoCola * nodoeliminado;
    int minimo;

    if(cola->pPrimero == NULL)
    {
        return ERROR;
    }

    nodoeliminado = (tNodoCola *)cola->pPrimero;
    minimo = MIN(tamanio, nodoeliminado->tamElemento);
    memcpy(destino, nodoeliminado->elemento, minimo);

    cola->pPrimero = nodoeliminado->sig;

    if(cola->pPrimero == NULL)
    {
        cola->pUltimo = NULL;
    }

    free(nodoeliminado->elemento);
    free(nodoeliminado);

    return OK;
}

void vaciarCola(t_cola* cola)
{
    tNodoCola * nodoeliminado;

    while(cola->pPrimero != NULL)
    {
        nodoeliminado = (tNodoCola *)cola->pPrimero;
        cola->pPrimero = nodoeliminado->sig;

        free(nodoeliminado->elemento);
        free(nodoeliminado);
    }
    cola->pUltimo = NULL;
}

int verTopeCola(const t_cola * cola, void* destino, unsigned tamanio)
{
    int minimo;
    if( colaVacia(cola) == OK )
    {
        return ERROR;
    }

    minimo = MIN( tamanio, (cola->pPrimero->tamElemento));

    memcpy(destino,cola->pPrimero->elemento,minimo);

    return OK;
}




