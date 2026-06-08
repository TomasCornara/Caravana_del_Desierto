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

void guardar_ranking_rec(const t_raking* arbol, FILE* f) {
    unsigned char marca;
    if (*arbol == NULL) {
        marca = 0;
        fwrite(&marca, sizeof(unsigned char), 1, f);
        return;
    }
    marca = 1;
    fwrite(&marca, sizeof(unsigned char), 1, f);
    fwrite((*arbol)->dato, sizeof(t_rankeo), 1, f);
    guardar_ranking_rec(&(*arbol)->izq, f);
    guardar_ranking_rec(&(*arbol)->der, f);
}

void guardar_ranking_bin(const t_raking* arbol, const char* archivo) {
    FILE* f = abrir_bin(archivo, "wb");
    if (!f) { perror("guardar_ranking_bin"); return; }
    guardar_ranking_rec(arbol, f);
    fclose(f);
}

void cargar_ranking_rec(t_raking* arbol, FILE* f) {
    unsigned char marca;
    t_rankeo dato;

    if (fread(&marca, sizeof(unsigned char), 1, f) != 1 || marca == 0) {
        *arbol = NULL;
        return;
    }
    fread(&dato, sizeof(t_rankeo), 1, f);
    *arbol = (t_Nodo*)malloc(sizeof(t_Nodo));
    if (!*arbol) return;
    (*arbol)->dato = malloc(sizeof(t_rankeo));
    if (!(*arbol)->dato) { free(*arbol); *arbol = NULL; return; }
    (*arbol)->tam  = sizeof(t_rankeo);
    memcpy((*arbol)->dato, &dato, sizeof(t_rankeo));
    (*arbol)->izq = NULL;
    (*arbol)->der = NULL;
    cargar_ranking_rec(&(*arbol)->izq, f);
    cargar_ranking_rec(&(*arbol)->der, f);
}

void cargar_ranking_bin(t_raking* arbol, const char* archivo) {
    FILE* f = abrir_bin(archivo, "rb");
    if (!f) return;
    cargar_ranking_rec(arbol, f);
    fclose(f);
}