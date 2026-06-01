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

#define VALORES_POR_LINEA_CONFIG 2
#define MAX_CLAVE_CONFIG 64
#define ARCHIVO_CONFIG "config.txt"
#define ARCHIVO_RANKING "ranking.txt"
#define ARCHIVO_MAPA "mapa.txt"
#define CARAS_DADO 6

//Valores del juego
#define MAX_NOMBRE 4
#define TIPO_INICIO 1
#define TIPO_FIN 2
#define TIPO_NORMAL 3
#define TIPO_OASIS 4
#define TIPO_TORMENTA 5
#define TIPO_VIDA_EXTRA 6
#define TIPO_PREMIO 7

// Valores por defecto para la configuración del juego
#define CONFIG_CANTIDAD_POSICIONES_DEFAULT 25
#define CONFIG_VIDAS_INICIO_DEFAULT 3
#define CONFIG_MAXIMO_BANDIDOS_DEFAULT 6
#define CONFIG_MAXIMO_PREMIOS_DEFAULT 3
#define CONFIG_MAXIMO_VIDAS_EXTRA_DEFAULT 1
#define CONFIG_MAXIMO_OASIS_DEFAULT 2
#define CONFIG_MAXIMO_TORMENTAS_DEFAULT 3

typedef t_Arbol t_raking;
typedef tLista t_mapa;
typedef t_mapa t_pos;
typedef void(*animCasillero)(void);
typedef t_cola t_movimientos;

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
void poner_casilleros_especiales(t_casillero *temp, int total, t_config *config);
int juego_generar_mapa(t_config *config, t_mapa *mapa);
int juego_cargar_config(t_config *config);
int juego_validar_config(t_config *config);
void jugar_partida(t_mapa *mapa, t_config *config);
void inicializar_jugador(t_jugador *jugador, t_config *config, t_mapa mapa);
int guardar_movimiento(t_movimientos *cola, unsigned pos_inicial, unsigned pos_final, bool jugador_humano);
void mover_jugador(t_jugador *jugador, unsigned pasos, t_movimientos *cola_movimientos, int lado);
void mover_bandido(t_mapa *mapa, t_movimientos *cola_movimientos);
void resolver_casillero_actual(t_jugador *jugador, t_casillero *casillero_actual, t_movimientos *cola_movimientos);
void resolver_bandido_en_casillero(t_jugador *jugador, t_casillero *casillero_actual, t_movimientos *cola_movimientos);
void mostrarMapa(t_mapa *mapa);


#endif // JUEGO_H_INCLUDED
