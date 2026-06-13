#include "indice_jugadores.h"

int cmp_indices_jugadores(const void* elemA, const void* elemB){
    t_indice* indiceA = (t_indice*)elemA;
    t_indice* indiceB = (t_indice*)elemB;
    return strcmp(indiceA->clave.nombre,indiceB->clave.nombre);
}

void print_indices(const void* elemA){
    t_indice* indice_aux = (t_indice*)elemA;

    printf("CLAVE: %s | POS: %u\n",indice_aux->clave.nombre,indice_aux->pos);
}

void guardarIndice(const t_arbol* arbol, FILE* arch){
    t_indice* indice;
    t_nodo* actual;

    if(!arbol || !(*arbol)) return;

    actual = *arbol;
    indice = (t_indice*)actual->dato;


    fwrite(indice,sizeof(t_indice),1,arch);
    if(actual->izq) guardarIndice(&actual->izq,arch);

    if(actual->der) guardarIndice(&actual->der,arch);


    return;
}

int file_a_arbolIndice(t_arbol* arbol, const char* nomb_arch){
    unsigned tamArchivo,
             cantRegistros;

    FILE *arch=fopen(nomb_arch,"rb");
    if(!arbol || !arch){
        return 0;
    }

    fseek(arch,0,SEEK_END);
    tamArchivo = ftell(arch);
    cantRegistros = tamArchivo/sizeof(t_indice);

    if(cantRegistros){
        return ponerEnIndice(arbol,0,cantRegistros - 1,arch);
    }
    fclose(arch);
    return 0;
}

int ponerEnIndice(t_arbol* arbol, int ini, int fin, FILE* arch){
    int r_padre,r_izq,r_der;
    unsigned medio;
    t_indice buffer_indice_aux;

    if(!arbol || !arch) return 0;

    if(ini > fin) return 1;

    medio = ini + (fin - ini) / 2;

    fseek(arch, medio * sizeof(t_indice), SEEK_SET);
    fread(&buffer_indice_aux,sizeof(t_indice),1,arch);

   /* strcpy(indice_aux.clave.nombre,buffer_reg_jugador.nombre);
    indice_aux.pos = medio;*/


    r_padre = ponerOrdenado(arbol,&buffer_indice_aux,sizeof(t_indice),cmp_indices_jugadores,NULL);
    r_izq = ponerEnIndice(arbol, ini, medio - 1, arch);
    r_der = ponerEnIndice(arbol, medio + 1, fin, arch);

    return r_padre * r_izq * r_der;
}

int crear_indice(FILE* arch)
{
    int cont=0;
    t_indice indice_aux;
    t_reg_jugador buffer_reg_jugador;
    t_arbol ord_arbol;

    FILE * idx=fopen(ARCHIVO_IDX,"wb");///crear define
    if(!idx)
    {
        return 0;
    }
    if( !arch) return 0;
    crearArbol(&ord_arbol);

    while(fread(&buffer_reg_jugador,sizeof(t_reg_jugador),1,arch))
    {
        indice_aux.pos=cont;
        strcpy(indice_aux.clave.nombre,buffer_reg_jugador.nombre);
        ponerOrdenado(&ord_arbol,&indice_aux,sizeof(t_indice),cmp_indices_jugadores,NULL);
        cont++;
    }
    guardarIndiceINorden(&ord_arbol,idx);
    fclose(idx);
    return 1;
}
void guardarIndiceINorden(const t_arbol* arbol, FILE* arch){
    t_indice* indice;
    t_nodo* actual;

    if(!arbol || !(*arbol)) return;

    actual = *arbol;
    indice = (t_indice*)actual->dato;

    if(actual->izq) guardarIndiceINorden(&actual->izq,arch);

    fwrite(indice,sizeof(t_indice),1,arch);

    if(actual->der) guardarIndiceINorden(&actual->der,arch);


    return;
}


unsigned buscar_id(t_indice* punt)
{
    t_reg_jugador reg;

    reg.id_jugador = 0;

    FILE *f = abrir_bin(ARCHIVO_JUGADORES, "rb");

    if (f == NULL) {
        printf("Error: No se pudo abrir el archivo de jugadores.\n");
        return 0;
    }

    fseek(f, punt->pos*sizeof(t_reg_jugador), SEEK_SET);

    if (fread(&reg, sizeof(t_reg_jugador), 1, f) != 1) {
        printf("Error: No se pudo leer el registro en la posicion %d\n", punt->pos);
    }

    fclose(f);

    return reg.id_jugador;
}

unsigned jugadores_proximo_id() {
    t_reg_jugador aux;
    FILE *f = abrir_bin(ARCHIVO_JUGADORES, "rb");
    if(!f)
    {
        return -1;
    }
    if(fseek(f, -(long)sizeof(t_reg_jugador), SEEK_END)!=0)
    {
        fclose(f);
        return 0;
    }
    fread(&aux,sizeof(t_reg_jugador),1,f);
    fclose(f);
    return aux.id_jugador+1;
}
int jugadores_agregar(unsigned id, const char *nombre) {
    t_reg_jugador reg;
    FILE *f = abrir_bin(ARCHIVO_JUGADORES, "ab");
    if (!f) return 0;
    reg.id_jugador = id;
    strncpy(reg.nombre, nombre, MAX_NOMBRE - 1);
    *(reg.nombre + MAX_NOMBRE - 1) = '\0';
    fwrite(&reg, sizeof(t_reg_jugador), 1, f);
    fclose(f);
    return 1;
}
int jugadores_buscar_nombre(unsigned id, char *nombre_dest) {
    t_reg_jugador reg;
    FILE *f = abrir_bin(ARCHIVO_JUGADORES, "rb");
    if (!f) return 0;
    while (fread(&reg, sizeof(t_reg_jugador), 1, f) == 1) {
        if (reg.id_jugador == id) {
            strncpy(nombre_dest, reg.nombre, MAX_NOMBRE - 1);
            *(nombre_dest + MAX_NOMBRE - 1) = '\0';
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}
int indice_comparar(const void *a, const void *b) {
    const t_indice *ra = (const t_indice*)a;
    const t_indice *rb = (const t_indice*)b;
    return strcmp(ra->clave.nombre, rb->clave.nombre);
}
int indice_buscar(t_raking *arbol, const char *nombre, t_indice *dest) {
    t_indice busq;
    strncpy(busq.clave.nombre, nombre, MAX_NOMBRE - 1);
    *(busq.clave.nombre + MAX_NOMBRE - 1) = '\0';
    busq.pos = 0;
    if (!buscarEnArbol(arbol, &busq, sizeof(t_indice), indice_comparar))
        return 0;
    *dest = busq;
    return 1;
}

int partidas_agregar(unsigned id_jugador, unsigned puntos, unsigned cant_movimientos) {
    t_reg_partida reg;
    FILE *f = abrir_bin(ARCHIVO_PARTIDAS, "ab");
    if (!f) return 0;
    reg.id_jugador       = id_jugador;
    reg.puntos           = puntos;
    reg.cant_movimientos = cant_movimientos;
    fwrite(&reg, sizeof(t_reg_partida), 1, f);
    fclose(f);
    return 1;
}
void ranking_mostrar() {
    FILE *f;
    t_reg_partida reg;
    t_acum acum[512];
    int cant_acum = 0;
    int i, j, k;
    t_acum top[TOP_RANKING];
    int cant_top;
    char nombre[MAX_NOMBRE];

    f = abrir_bin(ARCHIVO_PARTIDAS, "rb");
    if (!f) {
        printf("  No hay partidas registradas todavia.\n");
        return;
    }

    while (fread(&reg, sizeof(t_reg_partida), 1, f) == 1) {
        int encontrado = 0;
        for (i = 0; i < cant_acum; i++) {
            if ((acum + i)->id == reg.id_jugador) {
                (acum + i)->puntos_totales += reg.puntos;
                (acum + i)->cant_movimientos_t += reg.cant_movimientos;
                encontrado = 1;
                break;
            }
        }
        if (!encontrado && cant_acum < 512) {
            (acum + cant_acum)->id             = reg.id_jugador;
            (acum + cant_acum)->puntos_totales = reg.puntos;
            (acum + cant_acum)->cant_movimientos_t=reg.cant_movimientos;
            cant_acum++;
        }
    }
    fclose(f);

    if (cant_acum == 0) {
        printf("  No hay partidas registradas todavia.\n");
        return;
    }

    cant_top = (cant_acum < TOP_RANKING) ? cant_acum : TOP_RANKING;
    for (i = 0; i < cant_top; i++) {
        int idx_max = 0;
        for (j = 1; j < cant_acum; j++) {
            if ((acum + j)->puntos_totales > (acum + idx_max)->puntos_totales)
                idx_max = j;
        }
        *(top + i) = *(acum + idx_max);
        (acum + idx_max)->puntos_totales = 0;
        (acum + idx_max)->id = 0;
        (acum + idx_max)->cant_movimientos_t=0;
    }

    printf("\n  %-4s %-20s %8s %8s\n", "Pos.", "Jugador", "Puntos", "T_Movs");
    printf("  %-4s %-20s %8s  %8s\n", "----", "--------------------", "--------" , "--------");
    for (k = 0; k < cant_top; k++) {
        if (jugadores_buscar_nombre((top + k)->id, nombre))
            printf("  %-4d %-20s %8u %8u\n", k + 1, nombre, (top + k)->puntos_totales,(top + k)->cant_movimientos_t);
        else
            printf("  %-4d %-20s %8u\n", k + 1, "???", (top + k)->puntos_totales);
    }
    printf("\n");
}
