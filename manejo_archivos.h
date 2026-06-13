#ifndef MANEJO_ARCHIVOS_H_INCLUDED
#define MANEJO_ARCHIVOS_H_INCLUDED
#define MAX_BUFFER 256
#define EXTENSION ".txt"
#define EXT_LEN (sizeof(EXTENSION) - 1)
#define ARCHIVO_CONFIG  "config.txt"
#include <stdio.h>
#include "juego.h"

FILE* abrir_txt(const char* nombre_archivo, const char* modo);
char* leerLineaABuffer(FILE* arch, char* buffer, unsigned tam);
FILE* abrir_bin(const char* nombre_archivo, const char* modo);
#endif // MANEJO_ARCHIVOS_H_INCLUDED
