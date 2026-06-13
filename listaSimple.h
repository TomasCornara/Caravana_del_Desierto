#ifndef LISTASIMPLE_H_INCLUDED
#define LISTASIMPLE_H_INCLUDED

#define MIN(A,B) (((A)>(B))? (B):(A))

typedef struct s_nodoLista{
    void* dato;
    unsigned tam;
    struct s_nodoLista* sig;
} t_nodoLista;

typedef t_nodoLista* t_lista;
typedef int(*cmp)(const void*, const void*);
typedef void(*act)(void* destino, const void* origen);

void crearLista(t_lista* l);
int listaVacia(const t_lista* l);
int listaLlena(const t_lista* l, unsigned tam);
int ponerEnLista(t_lista* l, const void* dato, unsigned tam_dato,cmp comparar, act accion);
int verPrimero(const t_lista* l, void* buffer, unsigned tam);
void mapLista(const t_lista* l,act accion);
void ordenarLista(t_lista* l, cmp comparar);
int sacarPrimero(t_lista* l, void* buffer, unsigned tam);
void vaciarLista(t_lista* l);

#endif // LISTASIMPLE_H_INCLUDED
