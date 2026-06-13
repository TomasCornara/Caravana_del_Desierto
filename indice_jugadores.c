#include "indice_jugadores.h"
#include "listaSimple.h"

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

int cmp_id(const void* a, const void* b) {
    return ((t_acum*)a)->id - ((t_acum*)b)->id;
}

void acumular_stats(void* destino, const void* origen) {
    t_acum* dest = (t_acum*)destino;
    const t_acum* orig = (const t_acum*)origen;

    dest->puntos_totales += orig->puntos_totales;
    dest->cant_movimientos_t += orig->cant_movimientos_t;
}

int cmp_puntos_desc(const void* a, const void* b) {
    const t_acum* actual = (const t_acum*)a;
    const t_acum* en_lista = (const t_acum*)b;
    return (int)en_lista->puntos_totales - (int)actual->puntos_totales;
}

void imprimir_fila_ranking(int pos, const t_acum *datos) {
    char nombre[MAX_NOMBRE];

    if (jugadores_buscar_nombre(datos->id, nombre)) {
        printf("  %-4d %-20s %8u %8u\n", pos, nombre, datos->puntos_totales, datos->cant_movimientos_t);
    } else {
        printf("  %-4d %-20s %8u %8u\n", pos, "???", datos->puntos_totales, datos->cant_movimientos_t);
    }
}

void imprimir_header_ranking(void){
    printf("\n  %-4s %-20s %8s %8s\n", "Pos.", "Jugador", "Puntos", "T_Movs");
    printf("  %-4s %-20s %8s  %8s\n", "----", "--------------------", "--------" , "--------");
}

void ranking_mostrar() {
    FILE *f;
    t_reg_partida reg;
    t_lista lista_acum;
    int k;
    t_acum acum_aux;

    crearLista(&lista_acum);

    f = abrir_bin(ARCHIVO_PARTIDAS, "rb");
    if (!f) {
        printf("  No hay partidas registradas todavia.\n");
        return;
    }

    while (fread(&reg, sizeof(t_reg_partida), 1, f) == 1) {
        acum_aux.id = reg.id_jugador;
        acum_aux.puntos_totales = reg.puntos;
        acum_aux.cant_movimientos_t = reg.cant_movimientos;

        ponerEnLista(&lista_acum, &acum_aux, sizeof(t_acum), cmp_id, acumular_stats);
    }
    fclose(f);

    if (listaVacia(&lista_acum)) {
        printf("  No hay partidas registradas todavia.\n");
        return;
    }

    ordenarLista(&lista_acum, cmp_puntos_desc);

    imprimir_header_ranking();

    for (k = 0; k < TOP_RANKING && sacarPrimero(&lista_acum, &acum_aux, sizeof(t_acum)); k++) {
        imprimir_fila_ranking(k + 1, &acum_aux);
    }
    printf("\n");

    vaciarLista(&lista_acum);
}

#if DEBUG_GENERAR_PARTIDAS
int generarRegistros(void){
    t_reg_jugador jugadores[] =
        {
            {1005, "EVA"}, // Puntos totales: 43
            {1002, "ANA"}, // Puntos totales: 40
            {1008, "ZOE"}, // Puntos totales: 11
            {1001, "TOM"}, // Puntos totales: 59
            {1006, "DAN"}, // Puntos totales: 59
            {1004, "MAX"}, // Puntos totales: 91
            {1003, "LUC"}, // Puntos totales: 81
            {1007, "LEO"}  // Puntos totales: 80
        };

        t_reg_partida partidas[] =
        {
            {1001, 1, 32, 12},
            {1002, 2, 18,  8},
            {1003, 3, 45, 17},
            {1001, 4, 27, 10},
            {1004, 5, 50, 19},
            {1005, 6, 14,  6},
            {1006, 7, 39, 15},
            {1002, 8, 22,  9},
            {1007, 9, 47, 18},
            {1008, 10, 11,  5},
            {1003, 11, 36, 13},
            {1005, 12, 29, 11},
            {1004, 13, 41, 16},
            {1006, 14, 20,  7},
            {1007, 15, 33, 14}
        };

        FILE* f_jugadores = fopen("jugadores.dat", "wb");
        FILE* f_partidas  = fopen("partidas.dat", "wb");

        if(!f_jugadores || !f_partidas)
        {
            if(f_jugadores)
                fclose(f_jugadores);

            if(f_partidas)
                fclose(f_partidas);

            fprintf(stderr, "Error al crear archivos de prueba.\n");
            return 0;
        }

        fwrite(
            jugadores,
            sizeof(t_reg_jugador),
            sizeof(jugadores) / sizeof(jugadores[0]),
            f_jugadores
        );

        fwrite(
            partidas,
            sizeof(t_reg_partida),
            sizeof(partidas) / sizeof(partidas[0]),
            f_partidas
        );

        fclose(f_jugadores);
        fclose(f_partidas);

        return 1;
}
#endif
