#ifndef ARBOLBINARIO_H_INCLUDED
#define ARBOLBINARIO_H_INCLUDED

typedef struct s_nodo{
    void* dato;
    unsigned tam_dato;
    struct s_nodo* der;
    struct s_nodo* izq;
}t_nodo;

typedef t_nodo* t_arbol;
typedef void(*ptr_func)(const void*);
typedef int(*cmp_func)(const void*,const void*);
typedef void(*dup_func)(void*,const void*);

void crearArbol(t_arbol* arbol);
int arbolLleno(const t_arbol* arbol, unsigned tam);
int arbolVacio(const t_arbol* arbol);
void destruirArbol(t_arbol* arbol);
int ponerOrdenado(t_arbol* arbol, void* dato, unsigned tam_dato, cmp_func cmp, dup_func dup);
void recorrerInOrder(const t_arbol* arbol, ptr_func ptr);
void vaciarArbol(t_arbol* arbol);
void recorrerPosOrder(const t_arbol* arbol, ptr_func ptr);
void recorrerPreOrder(const t_arbol* arbol, ptr_func ptr);
int calcular_altura(t_arbol* arbol);
int eliminarNodo(t_arbol* arbol, const void* key, cmp_func cmp);
int buscarEnArbol(const t_arbol* arbol, void* dato, unsigned cantBytes, cmp_func cmp);
#endif // ARBOLBINARIO_H_INCLUDED
