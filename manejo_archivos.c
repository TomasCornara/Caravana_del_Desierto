#include <string.h>
#include "manejo_archivos.h"


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

int juego_cargar_config(Configuracion *config) {
    FILE *f = abrir_txt(ARCHIVO_CONFIG, "r");
    if (!f) {
        /* Valores por defecto si no existe el archivo */
        config->cantidad_posiciones = 25;
        config->vidas_inicio        = 3;
        config->maximo_bandidos     = 2;
        config->maximo_premios      = 3;
        config->maximo_vidas_extra  = 1;
        config->maximo_oasis        = 2;
        config->maximo_tormentas    = 3;
        printf("[Aviso] config.txt no encontrado. Usando valores por defecto.\n");
        return 0;
    }
    char clave[64];
    int  valor;
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
/*char* leerLinea(const FILE* arch_txt){

}*/
