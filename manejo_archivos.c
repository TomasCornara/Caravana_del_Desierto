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


char* leerLinea(const FILE* arch_txt){

}
