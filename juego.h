#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

#include "lista_circular_doble.h"
#include "manejo_archivos.h"
#include "cola_dinamica.h"
#include "arbolBinario.h"
#include "consola.h"

#define MAX_NOMBRE 4
#define TIPO_INICIO 1
#define TIPO_FIN 2
#define TIPO_NORMAL 3
#define TIPO_OASIS 4
#define TIPO_TORMENTA 5
#define TIPO_VIDA_EXTRA 6
#define TIPO_PREMIO 7

typedef t_Arbol t_raking;
typedef void(*animCasillero)(void);
typedef tLista t_mapa;
typedef t_mapa t_pos;

typedef struct{
    char nombre[MAX_NOMBRE];
    unsigned vidas;
    unsigned puntos;
    bool efectoTormenta;
    bool efectoOasis;
    t_pos pos;
}t_jugador;

typedef struct{
    unsigned nro_posicion;
    unsigned tipo_casillero;
    bool presencia_jugador;
    unsigned cant_bandidos;
    animCasillero animacion;
}t_casillero;

typedef t_cola t_movimientos;

typedef struct{
    unsigned pos_inicial;
    unsigned pos_final;
    bool jugador_humano;
}t_movimiento;

typedef struct{
    char puesto;
    char nombre[MAX_NOMBRE];
    unsigned puntos;
}t_rankeo;

typedef struct {
    int cantidad_posiciones;
    int vidas_inicio;
    int maximo_bandidos;
    int maximo_premios;
    int maximo_vidas_extra;
    int maximo_oasis;
    int maximo_tormentas;
} t_config;

void poner_tipo_random(t_casillero *temp, int total, unsigned tipo, int cantidad);
void poner_bandidos_random(t_casillero *temp, int total, int cantidad);
int juego_generar_mapa(t_config *config, t_mapa *mapa);
int   juego_cargar_config(t_config *config);
int juego_validar_config(t_config *config);

int   juego_cargar_config(t_config *config);

#endif // JUEGO_H_INCLUDED
