#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

#include "lista_circular_doble.h"
#include "manejo_archivos.h"
#include "cola_dinamica.h"
#include "arbolBinario.h"

#define VALORES_POR_LINEA_CONFIG 2
#define MAX_CLAVE_CONFIG 64
#define ARCHIVO_CONFIG "config.txt"
#define ARCHIVO_MAPA "caravana.txt"
#define ARCHIVO_RANKING "ranking.txt"
#define CARAS_DADO 6

//Valores del juego
#define ANCHO_CONSOLA 120
#define ALTO_CONSOLA 30
#define MAX_NOMBRE 4
#define TIPO_INICIO 1
#define TIPO_FIN 2
#define TIPO_NORMAL 3
#define TIPO_OASIS 4
#define TIPO_TORMENTA 5
#define TIPO_VIDA_EXTRA 6
#define TIPO_PREMIO 7
#define AVANZAR 'A'
#define RETROCEDER 'R'

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
    unsigned pos_en_mapa;
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
    unsigned cantidad_movimiento;
    unsigned pos_final;
    char orientacion;
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


//Funciones logica de juego
void poner_tipo_random(t_mapa *mapa, int total, unsigned tipo, int cantidad);
void poner_bandidos_random(t_mapa *mapa, int total, int cantidad);
void poner_casilleros_especiales(t_mapa *mapa, t_config *config);
int juego_generar_mapa(t_config *config, t_mapa *mapa);
int juego_cargar_config(t_config *config);
int juego_validar_config(t_config *config);
void jugar_partida(t_mapa *mapa, t_config *config);
void inicializar_jugador(t_jugador *jugador, t_config *config);
int guardar_movimiento(t_movimientos *cola, unsigned pos_inicial, unsigned pos_final,char orientacion,unsigned cant_movimiento,bool jugador_humano);
void mover_jugador(t_mapa*mapa,t_jugador *jugador,t_movimiento* movimiento);
void mover_bandido(t_mapa *mapa, t_movimiento* movimiento_bandido);
void resolver_casillero_actual(t_mapa * mapa,t_jugador *jugador);
void resolver_bandido_en_casillero(t_mapa*mapa,t_jugador *jugador, t_casillero *casillero_actual);
void printCaravana(FILE *archivo, t_mapa *mapa);


unsigned calcular_pos_final_del_jugador(unsigned pos_inicial_del_jugador,
                                   unsigned cantidad_nodos_lista,
                                   unsigned cantidad_pasos,
                                   char direccion);

void calcular_bandidos(t_mapa * mapa, t_movimientos * cola);
void situar_bandidos(void * a, void * parametro_extra);


int comparar_posicion_casilleros(const void* elem_a,const void* elem_b);
int comparar_clave_casillero(const void* elem_a,const void* elem_b);
int comparar_posicion_del_jugador_en_mapa(const void * a,const void * b);
void quitar_jugador(void* a,void* parametro_extra);
void ponerlo_jugador(void* a,void* parametro_extra);
void poner_bandido(void* a, void* parametro_extra);
void quitar_bandido(void *a, void* parametro_extra);


//Funciones consola

//Utiles
void limpiar_pantalla();
void mostrarHeader(void);
void mostrarFooter(void);
void mostrarEstadisticas(unsigned vidas, unsigned puntos, char* nombre);
void pausa();
void mostrar_menu();
void pedir_nombre(char *nombre);
void limpiar_buffer(void);;


//Pantallas
void mostrarBienvenida(void);
void printBandido(void);
void printTormenta(void);
void victoria(void);
void printOasis(void);
void printPremio(void);
void printVidaExtra(void);
void gameOver(void);

#endif // JUEGO_H_INCLUDED
