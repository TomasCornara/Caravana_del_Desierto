#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include <stdbool.h>

#include "cola_dinamica.h"

#define MAX_NOMBRE 4
#define TIPO_INICIO 1
#define TIPO_FIN 2
#define TIPO_NORMAL 3
#define TIPO_OASIS 4
#define TIPO TORMENTA 5
#define TIPO_VIDA_EXTRA 6
#define TIPO_PREMIO 7

#define CONFIG_DEFAULT_CANTIDAD_POSICIONES   25
#define CONFIG_DEFAULT_VIDAS_INICIO           3
#define CONFIG_DEFAULT_MAXIMO_BANDIDOS        2
#define CONFIG_DEFAULT_MAXIMO_PREMIOS         3
#define CONFIG_DEFAULT_MAXIMO_VIDAS_EXTRA     1
#define CONFIG_DEFAULT_MAXIMO_OASIS           2
#define CONFIG_DEFAULT_MAXIMO_TORMENTAS       3



typedef void (*animCasillero)(void);

typedef t_arbol t_raking;
//typedef t_mapa t_pos;
//typedef t_listaCirDoble t_mapa;

typedef struct{
    char nombre[MAX_NOMBRE];
    unsigned vidas;
    unsigned puntos;
    bool efectoTormenta;
    bool efectoOasis;
    //t_posicion pos;
}t_jugador;

typedef struct{
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

typedef struct{
    unsigned cantidad_posiciones;
    unsigned vidas_inicio;
    unsigned maximo_bandidos;
    unsigned maximo_premios;
    unsigned maximo_vidas_extra;
    unsigned maximo_oasis;
    unsigned maximo_tormentas;
} t_config;

#endif // JUEGO_H_INCLUDED
