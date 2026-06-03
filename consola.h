#ifndef CONSOLA_H_INCLUDED
#define CONSOLA_H_INCLUDED
#include <stdio.h>

#define ANCHO_CONSOLA 120
#define ALTO_CONSOLA 30

//Utiles
void limpiar_pantalla();
void mostrarHeader(void);
void mostrarFooter(void);
void mostrarEstadisticas(unsigned vidas, unsigned puntos, char* nombre);
void pausa();
void mostrar_menu();
void pedir_nombre(char *nombre);
void limpiar_buffer(void);


//Pantallas
void mostrarBienvenida(void);
void printBandido(void);
void printTormenta(void);
void victoria(void);
void printOasis(void);
void printPremio(void);
void printVidaExtra(void);
void gameOver(void);

#endif // CONSOLA_H_INCLUDED
