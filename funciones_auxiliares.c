#include"funciones_auxiliares.h"
void limp_pant() {
    system("cls");
}

void pausa() {
    printf("\n  Presione ENTER para continuar...");
    while (getchar() != '\n');
    getchar();
}
void mostrar_menu() {
    printf("\n");
    printf("  ------------------------------------\n");
    printf("  |     CARAVANA DEL DESIERTO        |\n");
    printf("  ------------------------------------\n");
    printf("  |  1. Jugar nueva partida          |\n");
    printf("  |  2. Ver ranking                  |\n");
    printf("  |  3. Salir                        |\n");
    printf("  ------------------------------------\n");
    printf("  Opcion: ");
}
