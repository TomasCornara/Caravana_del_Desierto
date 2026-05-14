#include "colaDinamica.h"

void crearCola(struct tCola * cola)
{
    cola->pPrimero = NULL;
    cola->pUltimo = NULL;
}

int colaVacia(const struct  tCola* cola)
{
    return NULL == cola->pPrimero ? OK : ERROR;
}

int colaLlena(const struct  tCola* cola, unsigned tamElemento)
{
    return OK;
}

int acolar(struct tCola * cola,const void * elemento, unsigned tamElemento)
{

    struct sNodo nodonuevo;
    void * bufferDato = malloc(tamElemento);

    if(!bufferDato)
    {
        return ERROR;
    }

    void * bufferNodo = malloc(sizeof(struct sNodo));
    if(!bufferNodo)
    {
        free(bufferDato);
        return ERROR;
    }

    nodonuevo.elemento = bufferDato;
    memcpy(nodonuevo.elemento,elemento,tamElemento);
    nodonuevo.tamElemento = tamElemento;
    nodonuevo.sig = NULL;

    memcpy(bufferNodo,&nodonuevo,sizeof(struct sNodo));

    if( cola->pPrimero == NULL )
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

int desacolar(struct tCola*cola, void*destino, unsigned tamanio)
{

    struct sNodo nodoeliminado;
    int minimo;

    if( colaVacia(cola) == OK )
    {
        return ERROR;
    }

    memcpy(&nodoeliminado,cola->pPrimero,sizeof(struct sNodo));

    minimo = MIN(tamanio, nodoeliminado.tamElemento);

    memcpy(destino,nodoeliminado.elemento,minimo);

    free(cola->pPrimero->elemento);
    free(cola->pPrimero);

    cola->pPrimero = nodoeliminado.sig;

    if(NULL == cola->pPrimero)
    {
        cola->pUltimo = NULL;
    }

    return OK;
}

void vaciarCola(struct tCola* cola)
{

    struct sNodo nodoElmiminado;

    while(cola->pPrimero != NULL)
    {
        memcpy(&nodoElmiminado,cola->pPrimero,sizeof(struct sNodo));

        free(cola->pPrimero->elemento);
        free(cola->pPrimero);

        cola->pPrimero = nodoElmiminado.sig;
    }
}

int verTopeCola(const struct tCola * cola, void*destino, unsigned tamanio)
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




