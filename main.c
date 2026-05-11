#include <stdio.h>
#include <stdlib.h>

#include "manejo_archivos.h"
#include "consola.h"
#include "lista_circular_simple.h"
#include"funciones_auxiliares.h"
int main() {
    int  opcion;

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
