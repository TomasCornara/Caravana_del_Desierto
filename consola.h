#ifndef CONSOLA_H_INCLUDED
#define CONSOLA_H_INCLUDED

#define ANCHO_CONSOLA 120
#define ALTO_CONSOLA 30

//Utiles
void limpiar_pantalla();
void mostrarHeader(void);
void mostrarFooter(void);
void mostrarEstadisticas(unsigned vidas, unsigned puntos);
void pausa();
void mostrar_menu();

//Pantallas
void mostrarBienvenida(void);
void printBandido(void);
void printTormenta(void);
void printCaravana(void);
void printOasis(void);
void printPremio(void);
void printVidaExtra(void);


#endif // CONSOLA_H_INCLUDED
