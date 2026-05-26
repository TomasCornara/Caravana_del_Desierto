#ifndef ARBOLBINARIO_H_INCLUDED
#define ARBOLBINARIO_H_INCLUDED
#define ABS(X) ((X >= 0)? (X):((X) * (-1)))

typedef struct sNodo{
    void* dato;
    unsigned tam;
    struct sNodo* der;
    struct sNodo* izq;
}t_Nodo;

typedef t_Nodo* t_Arbol;
typedef int(*comFunc)(const void* elem1, const void* elem2);
typedef void(*ptrFunc)(const void* elem);


void crearArbol(t_Arbol* arbol);
int ponerEnArbol(t_Arbol* arbol, const void* dato, unsigned cantBytes, comFunc cmp);
int buscarEnArbol(const t_Arbol* arbol, void* dato, unsigned cantBytes, comFunc cmp);
void destruirArbol(t_Arbol* arbol);
void printInOrden(const t_Arbol* arbol,ptrFunc ptr);
void printPreOrden(const t_Arbol* arbol,ptrFunc ptr);
void printPosOrden(const t_Arbol* arbol,ptrFunc ptr);
int calcularAltura(const t_Arbol* arbol);
int esAVL(const t_Arbol* arbol);
void mostrarArbol(const t_Arbol* arbol, ptrFunc ptr);
void mostrarArbol_recursivo(const t_Arbol* arbol, unsigned nivel, ptrFunc ptr);
int contarHojas(t_Arbol* arbol);
int contarRamas(t_Arbol* arbol);
int eliminarNodo(t_Arbol* arbol, const void* key, comFunc cmp);
int cargarDesdeArchivo(t_Arbol* arbol, unsigned ce,unsigned cantBytes, FILE* origen);
int esAVL2(const t_Arbol* arbol);

#endif // ARBOLBINARIO_H_INCLUDED
