#include <stdio.h>
#include <stdlib.h>

#include "manejo_archivos.h"
#include "consola.h"
//#include "lista_circular_simple.h"
#include"funciones_auxiliares.h"
#include "juego.h"
int main() {
    int  opcion;
    Configuracion config;
    t_mapa mapa;
    juego_cargar_config(&config);
    if (!juego_validar_config(&config)) {
        printf("Error: Configuracion invalida en config.txt\n");
        pausa();
        return 1;
    }

    printf("Configuracion cargada correctamente.\n");
    printf("Casilleros: %u | Vidas: %u | Bandidos: %u\n",
           config.cantidad_posiciones, config.vidas_inicio, config.maximo_bandidos);
    juego_generar_mapa(&config,&mapa);

    pausa();
    limp_pant();
    printf("\n  === CARAVANA DEL DESIERTO ===\n");

    do {
        limp_pant();
        mostrar_menu();
        if (scanf("%d", &opcion) != 1) opcion = 0;

        switch (opcion) {
            case 1:
                printf("\n  PROXIMAMENTE\n");
                //jugar_partida();creo que seria algo asi
                pausa();
                break;

            case 2:
                limp_pant();
                printf("\n  RANKING DE JUGADORES  \n");
                printf("\n  PROXIMAMENTE\n");
                //ranking_mostrar(&arbol_jugadores); creo que seria algo asi
                pausa();
                break;

            case 3:
                printf("\n  Hasta la proxima. ¡El desierto espera!\n\n");
                break;

            default:
                printf("  Opcion invalida.\n");
                pausa();
                break;
        }
    } while (opcion != 3);

    return 0;
}

/*
}*/
