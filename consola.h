#ifndef CONSOLA_H_INCLUDED
#define CONSOLA_H_INCLUDED

#define ANCHO_CONSOLA 120
#define ALTO_CONSOLA 30

//Utiles
void limpiar_pantalla();
void mostrarHeader(void);
void mostrarFooter(void);
void mostrarEstadisticas(unsigned vidas, unsigned puntos);

//Pantallas
void mostrarBienvenida(void);
void printBandido(void);
void printTormenta(void);
void printCaravana(void);
void printOasis(void);


#endif // CONSOLA_H_INCLUDED
