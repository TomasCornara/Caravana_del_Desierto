#include "lista_circular_doble.h"

void crear_lista(tLista* pl)
{
    *pl = NULL;
}

int lista_vacia(const tLista* pl)
{
    return NULL == (*pl);
}

int lista_llena(const tLista* pl)
{
    return 0;
}

int agregar_ord_lista(tLista *pl, void* dato, unsigned tam, int(*comparar)(const void*a,const void*b))
{
    tNodo *actual, *aux_ant, *aux_sig;
    if(*pl != NULL)
    {
       actual = *pl;
       while(comparar(dato, actual->info) > 0 && (actual->sig != NULL))
       {
            actual = actual->sig;
       }
       while(comparar(dato, actual->info) < 0 && (actual->ant != NULL))
       {
            actual = actual->ant;
       }
       if(comparar(dato, actual->info) == 0)
       {
           return 1; // Duplicado
       }
       if(comparar(dato, actual->info) > 0)
       {
        aux_sig = actual->sig;
        aux_ant = actual;
       } else {
        aux_sig = actual;
        aux_ant = actual->ant;
       }
    } else {
        aux_ant = *pl;
        aux_sig = *pl;
    }
    tNodo *nue = (tNodo*)malloc(sizeof(tNodo));
    if(nue == NULL)
    {
        return 1;
    }
    nue->info = malloc(tam);
    if(nue->info == NULL)
    {
        free(nue);
        return 1;
    }
    memcpy(nue->info, dato, tam);
    nue->tamInfo = tam;
    nue->ant = aux_ant;
    nue->sig = aux_sig;
    if(aux_ant != NULL)
    {
        aux_ant->sig = nue;
    }
    if(aux_sig != NULL)
    {
        aux_sig->ant = nue;
    }
    *pl = nue;
    return 0;
}

int ver_dato_lista(const tLista *pl, void* dato, unsigned tam) {
    tNodo * top = *pl;

    if(top == NULL)
    {
        return 0;
    }

    memcpy(dato, top->info, MIN(tam, top->tamInfo));
    return 1;
}

int agregar_final_lista(tLista *pl, const void *dato, unsigned tam)
{
    tNodo *nue, *ult;
    tNodo *inicio = (*pl);

    nue = (tNodo*)malloc(sizeof(tNodo));
    if(nue == NULL) return 0;

    nue->info = malloc(tam);
    if(nue->info == NULL) {
        free(nue);
        return 0;
    }

    memcpy(nue->info, dato, tam);
    nue->tamInfo = tam;
    nue->sig = NULL;

    if(*pl == NULL) {
        nue->ant = nue;
        nue->sig = nue;
        *pl = nue;
    } else {
        ult = *pl;
        while(ult->sig != (inicio)) {
            ult = ult->sig;
        }
        nue->sig = ult->sig;
        ult->sig = nue;
        nue->ant = ult;
        inicio->ant = nue;
        //*pl = nue;
        (*pl) = inicio;
    }
    return 1;
}

int agregar_inicio_lista(tLista *pl, const void *dato, unsigned tam)
{
    tNodo *nue, *pri;

    nue = (tNodo*)malloc(sizeof(tNodo));
    if(nue == NULL) return 0;

    nue->info = malloc(tam);
    if(nue->info == NULL) {
        free(nue);
        return 0;
    }

    memcpy(nue->info, dato, tam);
    nue->tamInfo = tam;
    nue->ant = NULL;

    if(*pl == NULL) {
        nue->sig = NULL;
        *pl = nue;
    } else {
        pri = *pl;
        while(pri->ant != NULL) {
            pri = pri->ant;
        }
        pri->ant = nue;
        nue->sig = pri;
        *pl = nue;
    }
    return 1;
}

int eliminar_lista(tLista *pl, void *dato, unsigned tam, int (*comparar)(const void*, const void*))
{
    tNodo *act;
    if(*pl == NULL) return 0;

    act = *pl;
    // Busco hacia adelante
    while(act->sig != NULL && comparar(act->info, dato) != 0) {
        act = act->sig;
    }
    // Si no lo encontré, busco hacia atrás
    if(comparar(act->info, dato) != 0) {
        act = *pl;
        while(act->ant != NULL && comparar(act->info, dato) != 0) {
            act = act->ant;
        }
    }

    if(comparar(act->info, dato) != 0) return 0; // No existe

    if(dato != NULL)
        memcpy(dato, act->info, MIN(tam, act->tamInfo));

    if(act->ant != NULL)
        act->ant->sig = act->sig;
    if(act->sig != NULL)
        act->sig->ant = act->ant;

    // Muevo el cursor de la lista
    if(act->sig != NULL)
        *pl = act->sig;
    else if(act->ant != NULL)
        *pl = act->ant;
    else
        *pl = NULL;

    free(act->info);
    free(act);
    return 1;
}

void vaciar_lista(tLista *pl)
{
    tNodo *act, *aux;
    if(*pl == NULL) return;

    act = *pl;
    // Voy al primero
    while(act->ant != NULL) {
        act = act->ant;
    }
    // Borro todo hacia adelante
    while(act != NULL) {
        aux = act->sig;
        free(act->info);
        free(act);
        act = aux;
    }
    *pl = NULL;
}

void recorrer_adelante(tLista *pl)
{
    tNodo *nue = *pl;
    while(nue != NULL)
    {
        int dato = *(int*)nue->info;
        printf("%d\n", dato);
        nue = nue->sig;
    }
}
void recorrer_adelante_accion(tLista *pl, tAccion accion, void*parametro_extra)
{
    tNodo *nue = (*pl);
    tNodo *inicio = (*pl);

    while(nue->sig != inicio)
    {
        accion(nue->info,parametro_extra);
        nue = nue->sig;
    }
    (*pl) = inicio;
}

void recorrer_atras(tLista *pl)
{
    tNodo *nue = *pl;
    while(nue != NULL)
    {
        int dato = *(int*)nue->info;
        printf("%d\n", dato);
        nue = nue->ant;
    }
}

void recorrer_todoasc(tLista *pl)
{
    if(*pl == NULL) return;
    tNodo *nue = *pl;
    while(nue->ant != NULL)
    {
        nue = nue->ant;
    }
    while(nue != NULL)
    {
        int dato = *(int*)nue->info;
        printf("%d\n", dato);
        nue = nue->sig;
    }
}

void recorrer_tododes(tLista *pl)
{
    if(*pl == NULL) return;
    tNodo *nue = *pl;
    while(nue->sig != NULL)
    {
        nue = nue->sig;
    }
    while(nue != NULL)
    {
        int dato = *(int*)nue->info;
        printf("%d\n", dato);
        nue = nue->ant;
    }
}
