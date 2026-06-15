#include "lista_circular_doble.h"
#define MIN(A,B) ((A > B)? (B):(A))

void crear_lista(tLista* pl)
{
    (*pl) = NULL;
}

int lista_vacia(const tLista* pl)
{
    return NULL == (*pl);
}

int lista_llena(const tLista* pl)
{
    return 0;
}

void vaciar_lista(tLista *pl)
{
    return;
}

void map_lista_inverso(const tLista* pl, tAccion accion, void* parms)
{
    tNodo *act,
          *primero;

    if(!pl || !*pl)
        return;

    primero = *pl;
    act = primero;

    do
    {
        accion(act->info, parms);
        act = act->ant;
    }
    while(act != primero);
}

void map_lista(const tLista* pl, tAccion accion, void* parms)
{
    tNodo *act,
          *primero;

    if(!pl || !*pl) return;

    primero = *pl;
    act = primero;

    do
    {
        accion(act->info, parms);
        act = act->sig;
    }
    while(act != primero);
}

void modificar_elemento_segun_clave(tLista * pl, void * clave,tCmp cmp, tAccion accion, void * parametro_extra){
   tNodo * inicio = (*pl);
   tNodo * actual = (*pl);

   if(NULL == inicio){
     return;
   }

   while(actual->sig != inicio){
     if( cmp(clave,actual->info) == 0 ){
        accion(actual->info,parametro_extra);
        return;
     }
     actual = actual->sig;
   }

   if( actual->sig == inicio){
     if(cmp(clave,actual->info) == 0){
        accion(actual->info,parametro_extra);
     }
   }
}

unsigned cantidad_elementos_lista(tLista * pl){
  tNodo * inicio = (*pl);
  tNodo * actual = (*pl);
  unsigned acumulador = 0;

  if( NULL == inicio ){
    return acumulador;
  }

  if( actual->sig == inicio){
    return acumulador;
  }

  do{
   acumulador++;
   actual = actual->sig;
  }while(actual->sig != inicio);

  return acumulador;
}

int agregar_ord_en_lista(tLista * pl, void * dato, unsigned tam, tCmp cmp){
   tNodo * inicio = (*pl);
   tNodo * actual = (*pl);
   tNodo * nuevo_nodo;

   if( actual == NULL ){
      nuevo_nodo = (tNodo*)malloc(sizeof(tNodo));
      if(!nuevo_nodo){
         return -1;
      }
      nuevo_nodo->info = malloc(tam);

      if(!nuevo_nodo->info){
        free(nuevo_nodo);
        return -1;
      }
      nuevo_nodo->tamInfo = tam;
      memcpy(nuevo_nodo->info,dato,tam);

      nuevo_nodo->ant = nuevo_nodo;
      nuevo_nodo->sig = nuevo_nodo;
      (*pl) = nuevo_nodo;

      return 1;
   }

   while(actual->ant->sig != inicio && cmp(dato,actual->info) < 0){
        actual = actual->ant;
   }

   while( actual->sig != inicio && cmp(dato,actual->sig->info) > 0){
        actual= actual->sig;
   }

   nuevo_nodo = (tNodo*)malloc(sizeof(tNodo));
   if(!nuevo_nodo){
      return -1;
   }
   nuevo_nodo->info = malloc(tam);

   if(!nuevo_nodo->info){
      free(nuevo_nodo);
      return -1;
   }
   nuevo_nodo->tamInfo = tam;
   memcpy(nuevo_nodo->info,dato,tam);

   if( cmp(dato,actual->info) < 0 ){
    nuevo_nodo->ant = actual->ant;
    nuevo_nodo->sig = actual;
    actual->ant->sig = nuevo_nodo;
    actual->ant = nuevo_nodo;

    (*pl) = nuevo_nodo;

   }else{
    nuevo_nodo->ant = actual;
    nuevo_nodo->sig = actual->sig;

    actual->sig->ant = nuevo_nodo;
    actual->sig = nuevo_nodo;
   }

   return 1;
}

void vaciar_lista_doble(tLista *pl)
{
    tNodo *act,
          *aux,
          *primero;

    if(!pl || !*pl) return;

    primero = *pl;
    act = primero->sig;

    while(act != primero)
    {
        aux = act;
        act = act->sig;

        free(aux->info);
        free(aux);
    }

    free(primero->info);
    free(primero);

    *pl = NULL;
}

int obtener_de_lista(const tLista* pl,const void* key, void* buffer, unsigned tam_dato, tCmp cmp)
{
    tNodo *act,
          *primero;

    if(!pl || !*pl || !buffer || !cmp)
        return 0;

    primero = *pl;
    act = primero;

    do
    {
        if(cmp(key, act->info) == 0)
        {
            memcpy(buffer, act->info, MIN(tam_dato, act->tamInfo));
            return 1;
        }

        act = act->sig;
    }
    while(act != primero);

    return 0;
}

void* obtener_de_lista_dir_dato(const tLista* pl,const void* key, tCmp cmp)
{
    tNodo *act,
          *primero;

    if(!pl || !*pl || !key || !cmp)
        return NULL;

    primero = *pl;
    act = primero;

    do
    {
        if(cmp(key, act->info) == 0)
        {
            return act->info;
        }

        act = act->sig;
    }
    while(act != primero);

    return NULL;
}
