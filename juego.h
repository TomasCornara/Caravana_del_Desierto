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

#define ARCHIVO_JUGADORES "jugadores.dat"
#define ARCHIVO_PARTIDAS  "partidas.dat"
#define ARCHIVO_INDICE    "jugadores.idx"
#define TOP_RANKING       5
#define OK 1

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

typedef struct {
    unsigned id;
    char     nombre[MAX_NOMBRE];
} t_reg_jugador;

typedef struct {
    unsigned id_jugador;
    unsigned puntos;
    unsigned cant_movimientos;
} t_reg_partida;

typedef struct {
    char     nombre[MAX_NOMBRE];
    long     posicion;
} t_reg_indice;

typedef struct {
    unsigned id;
    unsigned puntos_totales;
    unsigned cant_movimientos_t;
} t_acum;

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
    unsigned pos_final;
    bool jugador_humano;
}t_movimiento;
/*
typedef struct{
    unsigned pos_inicial; // 0
    unsigned cantidad_movimiento; // 5
    unsigned pos_final; // 5
    char orientacion; // F
    bool jugador_humano; // TRUE
}t_movimiento;
*/

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
int guardar_movimiento(t_movimientos *cola, unsigned pos_inicial, unsigned pos_final, bool jugador_humano);
void mover_jugador(t_jugador *jugador, unsigned pasos, int lado);
void mover_bandido(t_mapa *mapa, t_movimientos *cola_movimientos);
void resolver_casillero_actual(t_jugador *jugador, t_casillero *casillero_actual, t_movimientos *cola_movimientos);
void resolver_bandido_en_casillero(t_jugador *jugador, t_casillero *casillero_actual, t_movimientos *cola_movimientos);
void printCaravana(FILE *archivo, t_mapa *mapa);


int calcular_pos_final_del_jugador(unsigned pos_inicial_del_jugador,
                                   unsigned cantidad_nodos_lista,
                                   unsigned cantidad_pasos,
                                   char direccion);
void calcular_bandidos(t_mapa * mapa, t_movimientos * cola);
void situar_bandidos(void * a, void * parametro_extra);


int comparar_posicion_casilleros(const void* elem_a,const void* elem_b);
int comparar_clave_casillero(const void* elem_a,const void* elem_b);
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

//Arbol
int  comparar_rankeo(const void* a, const void* b);
void imprimir_rankeo(const void* dato, int puesto);
void callback_recolectar(const void *dato);
void actualizar_ranking(t_raking *arbol, const char* nombre, unsigned puntos);
void mostrar_ranking(const t_raking *arbol);
int rankear(const t_raking *arbol, int puesto);

/* Persistencia */
void guardar_ranking_bin(const t_raking* arbol, const char* archivo);
void cargar_ranking_bin(t_raking* arbol, const char* archivo);
int mostrar_movimientos(t_movimientos *cola, const char* nombre_jugador);
void guardar_mapa_txt(t_mapa *mapa, const char* nombre_jugador, int vidas, int puntos, int turno, const char* archivo);

//Pantallas
void mostrarBienvenida(void);
void printBandido(void);
void printTormenta(void);
void victoria(void);
void printOasis(void);
void printPremio(void);
void printVidaExtra(void);
void gameOver(void);

unsigned jugadores_proximo_id(void);
int      jugadores_agregar(unsigned id, const char *nombre);
int      jugadores_buscar_nombre(unsigned id, char *nombre_dest);

int      indice_comparar(const void *a, const void *b);
int      indice_buscar(t_raking *arbol, const char *nombre, t_reg_indice *dest);
void     indice_insertar(t_raking *arbol, t_reg_indice *reg);
void     indice_guardar(t_raking *arbol);
void     indice_cargar(t_raking *arbol);

int      partidas_agregar(unsigned id_jugador, unsigned puntos, unsigned cant_movimientos);
unsigned buscar_id(t_reg_indice* punt);
void     ranking_mostrar(void);
#endif // JUEGO_H_INCLUDED
