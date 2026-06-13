#ifndef INDICE_JUGADORES_H_INCLUDED
#define INDICE_JUGADORES_H_INCLUDED
#include "arbolBinario.h"
#include "juego.h"
#include <stdio.h>
typedef struct{
    char nombre[MAX_NOMBRE];
}t_clave;

typedef struct{
    t_clave clave;
    unsigned pos;
}t_indice;

typedef struct {
    unsigned id_jugador;
    unsigned id_partida;
    unsigned puntos;
    unsigned cant_movimientos;
} t_reg_partida;

typedef struct {
    unsigned id_jugador;
    char nombre[MAX_NOMBRE];///clave
} t_reg_jugador;

#if DEBUG_GENERAR_PARTIDAS
    int generarRegistros(void);
#endif // DEBUG_GENERAR_PARTIDAS

int ponerEnIndice(t_arbol* arbol, int ini, int fin, FILE* arch);
int cmp_indices_persona(const void* elemA, const void* elemB);
void print_indices(const void* elemA);
void guardarIndice(const t_arbol* arbol, FILE* arch);
int file_a_arbolIndice(t_arbol* arbol, const char* nomb_arch);
int ponerEnIndice(t_arbol* arbol, int ini, int fin, FILE* arch);
void guardarIndiceINorden(const t_arbol* arbol, FILE* arch);

unsigned buscar_id(t_indice* punt);
unsigned jugadores_proximo_id();
int jugadores_agregar(unsigned id, const char *nombre);
int jugadores_buscar_nombre(unsigned id, char *nombre_dest);
int partidas_agregar(unsigned id_jugador, unsigned puntos, unsigned cant_movimientos);
void ranking_mostrar();
int indice_comparar(const void *a, const void *b);
int indice_buscar(t_raking *arbol, const char *nombre, t_indice *dest);

int crear_indice(FILE* arch);
void guardarIndiceINorden(const t_arbol* arbol, FILE* arch);
#endif // INDICE_JUGADORES_H_INCLUDED
