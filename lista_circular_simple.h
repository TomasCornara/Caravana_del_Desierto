#ifndef LISTA_CIRCULAR_SIMPLE_H_INCLUDED
#define LISTA_CIRCULAR_SIMPLE_H_INCLUDED
#define MIN(A,B) (((A)>(B))? (B):(A))


typedef struct s_nodo{
    void* dato;
    unsigned tam_dato;
    struct s_nodo* sig;
}t_nodo;

typedef t_nodo* t_listaCircular;

void crearListaSimple(t_listaCircular* l);
int listaSimpleLlena(const t_listaCircular* l, unsigned tam);
int listaSimpleVacia(const t_listaCircular* l);
int ponerAlPrincipioEnListaSimple(t_listaCircular* l, const void* dato, unsigned tam_dato);
void vaciarListaSimple(t_listaCircular* l);
void moverAlSiguiente(t_listaCircular* l);
int verPrimeroListaSimple(const t_listaCircular* l, void* dato, unsigned tam_dato);

#endif // LISTA_CIRCULAR_SIMPLE_H_INCLUDED
