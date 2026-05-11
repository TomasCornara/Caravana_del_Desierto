#ifndef MANEJO_ARCHIVOS_H_INCLUDED
#define MANEJO_ARCHIVOS_H_INCLUDED
#define MAX_BUFFER 256
#define EXTENSION ".txt"
#define EXT_LEN (sizeof(EXTENSION) - 1)
#define ARCHIVO_CONFIG  "config.txt"
#include <stdio.h>

typedef struct {
    int cantidad_posiciones;
    int vidas_inicio;
    int maximo_bandidos;
    int maximo_premios;
    int maximo_vidas_extra;
    int maximo_oasis;
    int maximo_tormentas;
} Configuracion;

FILE* abrir_txt(const char* nombre_archivo, const char* modo);
int   juego_cargar_config(Configuracion *config);
#endif // MANEJO_ARCHIVOS_H_INCLUDED
