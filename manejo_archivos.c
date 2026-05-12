#include <string.h>
#include "manejo_archivos.h"

char* leerLineaABuffer(FILE* arch, char* buffer, unsigned tam){
    if(!arch || !buffer || !tam) return NULL;

    if(!fgets(buffer,tam,arch)) return NULL;

    return buffer;
}

FILE* abrir_txt(const char* nombre_archivo, const char* modo)
{
    char buffer[MAX_BUFFER];
    size_t len_nombre;

    if (!nombre_archivo || !modo)
        return NULL;

    if (strcmp(modo, "r") != 0 && strcmp(modo, "w") != 0)
        return NULL;

    len_nombre = strlen(nombre_archivo);
    if (len_nombre + EXT_LEN >= MAX_BUFFER)
        return NULL;

    strcpy(buffer, nombre_archivo);

    if (len_nombre < EXT_LEN ||
        strcmp(buffer + len_nombre - EXT_LEN, EXTENSION) != 0)
    {
        strcat(buffer, EXTENSION);
    }

    return fopen(buffer, modo);
}

int juego_cargar_config(t_config *config) {
    char clave[64];
    int  valor;
    FILE *f;

    f = abrir_txt(ARCHIVO_CONFIG, "r");
    if (!f) {
        /* Valores por defecto si no existe el archivo */
        config->cantidad_posiciones = CONFIG_DEFAULT_CANTIDAD_POSICIONES;
        config->vidas_inicio        = CONFIG_DEFAULT_VIDAS_INICIO;
        config->maximo_bandidos     = CONFIG_DEFAULT_MAXIMO_BANDIDOS;
        config->maximo_premios      = CONFIG_DEFAULT_MAXIMO_PREMIOS;
        config->maximo_vidas_extra  = CONFIG_DEFAULT_MAXIMO_VIDAS_EXTRA;
        config->maximo_oasis        = CONFIG_DEFAULT_MAXIMO_OASIS;
        config->maximo_tormentas    = CONFIG_DEFAULT_MAXIMO_TORMENTAS;

        printf("[Aviso] config.txt no encontrado. Usando valores por defecto.\n");
        return 0;
    }

    while (fscanf(f, "%63[^:]:%d\n", clave, &valor) == 2) {
        if      (!strcmp(clave, "cantidad_posiciones")) config->cantidad_posiciones = valor;
        else if (!strcmp(clave, "vidas_inicio"))        config->vidas_inicio        = valor;
        else if (!strcmp(clave, "maximo_bandidos"))     config->maximo_bandidos     = valor;
        else if (!strcmp(clave, "maximo_premios"))      config->maximo_premios      = valor;
        else if (!strcmp(clave, "maximo_vidas_extra"))  config->maximo_vidas_extra  = valor;
        else if (!strcmp(clave, "maximo_oasis"))        config->maximo_oasis        = valor;
        else if (!strcmp(clave, "maximo_tormentas"))    config->maximo_tormentas    = valor;
    }
    fclose(f);
    return 1;
}
