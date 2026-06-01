#include <stdio.h>
#include <stdlib.h>
#include "juego.h"

#define SALIR 3
#define JUGAR 1
#define RANKING 2

//Funciones auxiliares para el menu
void limpiar_buffer_entrada(void);
int leer_opcion_menu(void);


int main() {
    srand(time(NULL));

    ///BLOQUE DECLARACION
    int  opcion;
    t_config config;
    t_mapa mapa;
    //t_raking arbol_jugadores;

    ///BLOQUE DE EJECUCION
    printf("MAXIME ESTA VENTANA PARA PODER VISUALIZAR EL JUEGO CORRECTAMENTE\n");

    pausa();
    limpiar_pantalla();

    // Cargar configuracion y validar
    juego_cargar_config(&config);
    if (!juego_validar_config(&config)) {
        fprintf(stderr, "Error: Configuracion invalida en config.txt\n");
        pausa();
        return 1;
    }

    printf("Configuracion cargada correctamente.\n");
    printf("Casilleros: %u | Vidas: %u | Bandidos: %u\n",
           config.cantidad_posiciones, config.vidas_inicio, config.maximo_bandidos);
    juego_generar_mapa(&config,&mapa);

    pausa();
    limpiar_pantalla();

    // Menu principal
    printf("\n  === CARAVANA DEL DESIERTO ===\n");

    do {
        limpiar_pantalla();
        mostrar_menu();
        opcion = leer_opcion_menu();

        switch (opcion) {
            case JUGAR:
                if (juego_generar_mapa(&config, &mapa)) {
                    jugar_partida(&mapa, &config);
                    vaciar_lista(&mapa);
                } else {
                    printf("No se pudo generar el mapa.\n");
                }
                pausa();
                break;

            case RANKING:
                limpiar_pantalla();
                printf("\n  RANKING DE JUGADORES  \n");
                printf("\n  PROXIMAMENTE\n");
                //ranking_mostrar(&arbol_jugadores); creo que seria algo asi
                pausa();
                break;

            case SALIR:
                printf("\n  Hasta la proxima. El desierto espera!\n\n");
                break;

            default:
                printf("  Opcion invalida.\n");
                pausa();
                break;
        }

    } while (opcion != SALIR);

    return 0;
}

void limpiar_buffer_entrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int leer_opcion_menu(void) {
    int opcion;

    if (scanf("%d", &opcion) != 1) {
        opcion = 0;
    }

    limpiar_buffer_entrada();

    return opcion;
}
